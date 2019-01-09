#include "stdafx.h"

#include "CircleApprox.h"
#include <iostream>

// ---																										// Constructors
CircleApprox::CircleApprox() { objectApproxName = (char*)"circle"; }

CircleApprox::~CircleApprox(){}
// ---																										// --- APPROXIMATION ---
double CircleApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;
	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.Point.DistanceToPoint(points[i]) - Radius, 2);
	return sum;
}

void CircleApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	Plane.FindByPoints(&points[0], arraySize, accuracy);
	Line	= Plane.Line;
	
	//VectorX = Plane.VectorX;
	//VectorY = Plane.VectorY;

	//VectorGeometric vectorR;

	for (int i = 0; i < arraySize; i++)
	{
		points[i] = Plane.PointProjection(points[i]);

		//vectorR = VectorGeometric(Line.Point, points[i], false);

		//points[i].X = vectorR * VectorX;
		//points[i].Y = vectorR * VectorY;
		//points[i].Z = 0;				// vectorR * Line.Vector;
	}

	CenterByPoints(points, arraySize);

	for (int i = 0; i < arraySize; i++)
	{
		Radius += Line.Point.DistanceToPoint(points[i]);
	}
	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;


	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);	// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);	// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);	// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius

																			///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);
}