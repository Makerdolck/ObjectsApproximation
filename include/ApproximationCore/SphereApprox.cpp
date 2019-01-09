#include "stdafx.h"

#include "SphereApprox.h"


// ---																										// Constructors
SphereApprox::SphereApprox(){ objectApproxName = (char*)"sphere"; }

SphereApprox::~SphereApprox(){}

double SphereApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;
	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.Point.DistanceToPoint(points[i]) - Radius, 2);
	return sum;
}

void SphereApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
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


	Height = Radius * 2;
}