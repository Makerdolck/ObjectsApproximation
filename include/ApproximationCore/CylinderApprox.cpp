#include "stdafx.h"

#include "CylinderApprox.h"


// ---																										// Constructors
CylinderApprox::CylinderApprox() { objectApproxName = (char*)"cylinder"; }

CylinderApprox::~CylinderApprox(){}
// ---																										// --- APPROXIMATION ---
double CylinderApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;
	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.DistanceToPoint(points[i]) - Radius, 2);
	return sum;
}

void CylinderApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	CenterByPoints(points, arraySize);		// Find center point

	PointGeometric tmpCenter = Line.Point;

	VectorGeometric	vector1(points[0], points[1]),
		vector2(points[0], points[2]);

	Line.Vector = vector1 ^ vector2;

	Line.Normalize();						// Vector Normalization

	for (int i = 0; i < arraySize; i++)		// Finding model radius
	{
		Radius += Line.DistanceToPoint(points[i]);
	}
	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;

	//	---	---	--- Exclude max Vector/\Point component

	double	*vectorMCoordinate1, *vectorMCoordinate2;
	double	*pointMCoordinate1, *pointMCoordinate2;

	// Find Max Vector/\Point Element
	if ((fabs(Line.Vector.X) > fabs(Line.Vector.Y)) && (fabs(Line.Vector.X) > fabs(Line.Vector.Z)))
	{
		//lineCenterM.vectorM.X;
		vectorMCoordinate1 = &Line.Vector.Y;
		vectorMCoordinate2 = &Line.Vector.Z;

		pointMCoordinate1 = &Line.Point.Y;
		pointMCoordinate2 = &Line.Point.Z;
	}
	else
	{
		if ((fabs(Line.Vector.Y) > fabs(Line.Vector.X)) && (fabs(Line.Vector.Y) > fabs(Line.Vector.Z)))
		{
			//Line.Vector.Y;
			vectorMCoordinate1 = &Line.Vector.X;
			vectorMCoordinate2 = &Line.Vector.Z;

			pointMCoordinate1 = &Line.Point.X;
			pointMCoordinate2 = &Line.Point.Z;
		}
		else
		{
			//Line.Vector.Z;
			vectorMCoordinate1 = &Line.Vector.Y;
			vectorMCoordinate2 = &Line.Vector.X;

			pointMCoordinate1 = &Line.Point.Y;
			pointMCoordinate2 = &Line.Point.X;
		}
	}


	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		///////////

		//Approximation(points, accuracy, &Line.Vector.x);	// Changing X - vector
		//Approximation(points, accuracy, &Line.Vector.y);	// Changing Y - vector
		//Approximation(points, accuracy, &Line.Vector.z);	// Changing Z - vector

		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.x);		// Changing X - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.y);		// Changing Y - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.z);		// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);		// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);		// Changing 2 - vector


		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate1);		// Changing 1 - center
		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate2);		// Changing 2 - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);					// R

																							///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Line.PointProjection(tmpCenter);

	FindHeight(points, arraySize);

	PointBottomEdge = Line.CreatePointOnDistance(Height / 2, false);
}
