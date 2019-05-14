#include "stdafx.h"

#include "PlaneApprox.h"


// ---																										// Constructors
PlaneApprox::PlaneApprox() 
{
	objectApproxName = (char*)"plane";
}

PlaneApprox::~PlaneApprox(){}
// ---																										// Point Projection
PointGeometric PlaneApprox::PointProjection(PointGeometric pointOut)
{
	Plane.Line = Line;

	return Plane.PointProjection(pointOut);
}
// ---																										// Vector Projection
VectorGeometric PlaneApprox::VectorProjection(VectorGeometric vectorOut, bool flagNormalize)
{
	Plane.Line = Line;

	return Plane.VectorProjection(vectorOut, flagNormalize);
}
// ---																										// --- APPROXIMATION ---
double PlaneApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;

	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.Point.DistanceToPoint(Line.PointProjection(points[i])), 2);

	return sum;
}

void PlaneApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	Line.Vector = PlaneGeometric(points[0], points[1], points[2]).Line.Vector;	// Find center vector
	Line.Point	= CenterByPoints(points, arraySize);							// Find center point

	PointGeometric tmpCenter = Line.Point;

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;

	double	*vectorMCoordinate1, *vectorMCoordinate2;

	// Find Max vector Element
	if ((fabs(Line.Vector.X) > fabs(Line.Vector.Y)) && (fabs(Line.Vector.X) > fabs(Line.Vector.Z)))
	{
		//Line.Vector.X;
		vectorMCoordinate1 = &Line.Vector.Y;
		vectorMCoordinate2 = &Line.Vector.Z;
	}
	else
	{
		if ((fabs(Line.Vector.Y) > fabs(Line.Vector.X)) && (fabs(Line.Vector.Y) > fabs(Line.Vector.Z)))
		{
			//Line.Vector.Y;
			vectorMCoordinate1 = &Line.Vector.X;
			vectorMCoordinate2 = &Line.Vector.Z;
		}
		else
		{
			//Line.Vector.Z;
			vectorMCoordinate1 = &Line.Vector.Y;
			vectorMCoordinate2 = &Line.Vector.X;
		}
	}

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);	// Changing 2 - vector

		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);		// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);		// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);		// Changing Z - center


		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Line.Vector.PointProjection(tmpCenter, Line.Point);
}