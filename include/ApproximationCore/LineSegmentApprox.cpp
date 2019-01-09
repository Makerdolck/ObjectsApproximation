#include "stdafx.h"

#include "LineSegmentApprox.h"


// ---																										// Constructors
LineSegmentApprox::LineSegmentApprox() { objectApproxName = (char*)"lineSegment"; }

LineSegmentApprox::~LineSegmentApprox(){}
// ---																										// Distance to Point
double LineSegmentApprox::DistanceToPoint(PointGeometric point)
{
	return Line.DistanceToPoint(point);
}
// ---																										// Point Projection to Line
PointGeometric LineSegmentApprox::PointProjection(PointGeometric point)
{
	return Line.PointProjection(point);
}
// ---																										// --- APPROXIMATION ---
double LineSegmentApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;
	for (int i = 0; i < arraySize; i++)
		sum +=	pow(
					DistanceToPoint(points[i])
				, 2);
	return sum;
}

void LineSegmentApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{


	Line.Vector = VectorGeometric(points[0], points[arraySize - 1]);	// Find center vector
	CenterByPoints(points, arraySize);									// Find center point

	PointGeometric tmpCenter = Line.Point;

	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;

	double	*vectorMCoordinate1, *vectorMCoordinate2;

	// Find Max vector Element
	if ((fabs(Line.Vector.X) > fabs(Line.Vector.Y)) && (fabs(Line.Vector.X) > fabs(Line.Vector.Z)))
	{
		//lineCenterM.Line.Vector.X;
		vectorMCoordinate1 = &Line.Vector.Y;
		vectorMCoordinate2 = &Line.Vector.Z;
	}
	else
	{
		if ((fabs(Line.Vector.Y) > fabs(Line.Vector.X)) && (fabs(Line.Vector.Y) > fabs(Line.Vector.Z)))
		{
			//lineCenterM.Line.Vector.Y;
			vectorMCoordinate1 = &Line.Vector.X;
			vectorMCoordinate2 = &Line.Vector.Z;
		}
		else
		{
			//lineCenterM.Line.Vector.Z;
			vectorMCoordinate1 = &Line.Vector.Y;
			vectorMCoordinate2 = &Line.Vector.X;
		}
	}

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		///////////

		//Approximation(points, accuracy, &lineCenterM.Line.Vector.X);	// Changing X - vector
		//Approximation(points, accuracy, &lineCenterM.Line.Vector.Y);	// Changing Y - vector
		//Approximation(points, accuracy, &lineCenterM.Line.Vector.Z);	// Changing Z - vector

		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);	// Changing 2 - vector


		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);		// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);		// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);		// Changing Z - center

																				///////
		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = PointProjection(tmpCenter);

	//
	PointGeometric		*_points	= points;
						PointStart	= Line.Point,
						PointEnd	= Line.Point;
	VectorGeometric*	tmpVector	= nullptr;
	double distance = 0;

	for (int i = 0; i < arraySize; i++)
		_points[i] = PointProjection(points[i]);

	for (int i = 0; i < arraySize; i++)
	{
		delete tmpVector;
		tmpVector = new VectorGeometric(_points[i], Line.Point);

		if ((tmpVector->X >= 0 && Line.Vector.X >= 0) || (tmpVector->X <= 0 && Line.Vector.X <= 0))
		{
			if (PointStart.DistanceToPoint(Line.Point) < _points[i].DistanceToPoint(Line.Point)) {
				PointStart = _points[i];
				continue;
			}
		}
		else
		{
			if (PointEnd.DistanceToPoint(Line.Point) < _points[i].DistanceToPoint(Line.Point)) {
				PointEnd = _points[i];
				continue;
			}
		}
	}

	delete tmpVector;

	Point  = Line.Point;
	Vector = Line.Vector;
}