#include "stdafx.h"

#include "LineSegmentApprox.h"


// ---																										// Constructors
LineSegmentApprox::LineSegmentApprox()
{
	objectApproxName = (char*)"lineSegment";
}

LineSegmentApprox::~LineSegmentApprox(){}
// ---																										// Distance to Point
double LineSegmentApprox::DistanceToPoint(PointGeometric point)
{
	Line.Point	= Point;
	Line.Vector	= Vector;

	return Line.DistanceToPoint(point);
}
// ---																										// Point Projection to Line
PointGeometric LineSegmentApprox::PointProjection(PointGeometric point)
{
	Line.Point	= Point;
	Line.Vector = Vector;

	return Line.PointProjection(point);
}
// ---																										// --- APPROXIMATION ---
double LineSegmentApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;

	for (int i = 0; i < arraySize; i++)
		sum +=	pow(DistanceToPoint(points[i]), 2);

	return sum;
}

void LineSegmentApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	Vector = VectorGeometric(points[0], points[arraySize - 1]);			// Find center vector
	CenterByPoints(points, arraySize);									// Find center point

					Point		= Line.Point;
	PointGeometric	tmpCenter	= Line.Point;

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;

	double	*vectorMCoordinate1 = nullptr, 
			*vectorMCoordinate2 = nullptr;

	// Find Max vector Element
	if ((fabs(Vector.X) > fabs(Vector.Y)) && (fabs(Vector.X) > fabs(Vector.Z)))
	{
		vectorMCoordinate1 = &Vector.Y;
		vectorMCoordinate2 = &Vector.Z;
	}
	else
	{
		if ((fabs(Vector.Y) > fabs(Vector.X)) && (fabs(Vector.Y) > fabs(Vector.Z)))
		{
			vectorMCoordinate1 = &Vector.X;
			vectorMCoordinate2 = &Vector.Z;
		}
		else
		{
			vectorMCoordinate1 = &Vector.Y;
			vectorMCoordinate2 = &Vector.X;
		}
	}

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(points, arraySize, accuracy, &Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Vector, vectorMCoordinate2);	// Changing 2 - vector
		
		Approximation(points, arraySize, accuracy, &Vector, &Point.X);		// Changing X - center
		Approximation(points, arraySize, accuracy, &Vector, &Point.Y);		// Changing Y - center
		Approximation(points, arraySize, accuracy, &Vector, &Point.Z);		// Changing Z - center


		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Point = PointProjection(tmpCenter);

	//
	std::vector<PointGeometric> _points;

	PointStart	= Point;
	PointEnd	= Point;

	VectorGeometric		tmpVector;

	double distance = 0;

	for (int i = 0; i < arraySize; i++)
		_points.push_back(PointProjection(points[i]));

	for (int i = 0; i < arraySize; i++)
	{
		tmpVector = VectorGeometric(_points[i], Point);

		if ((tmpVector.X >= 0 && Vector.X >= 0) || (tmpVector.X <= 0 && Vector.X <= 0))
		{
			if (PointStart.DistanceToPoint(Point) < _points[i].DistanceToPoint(Point)) {
				PointStart = _points[i];
				continue;
			}
		}
		else
		{
			if (PointEnd.DistanceToPoint(Point) < _points[i].DistanceToPoint(Point)) {
				PointEnd = _points[i];
				continue;
			}
		}
	}

	Height = PointStart.DistanceToPoint(PointEnd);

	_points.clear();
}
// ---																										//	---	Intersections ---

// ---																										// PointBetweenLineMiddle
PointGeometric LineSegmentApprox::PointBetweenLineMiddle(PointApprox point) {

	LineGeometric line(Point, Vector);
	PointGeometric  pointout(point.X, point.Y, point.Z);
	return line.PointBetweenLineMiddle(pointout);
}
// ---																										// PointBetweenLine
PointGeometric LineSegmentApprox::PointBetweenLine(PointApprox point) {

	LineGeometric line(Point, Vector);
	PointGeometric  pointout(point.X, point.Y, point.Z);
	return line.PointProjection(pointout);
}
// ---																										// LineBetweenLine
int	LineSegmentApprox::LineBetweenLine(LineGeometric Line2, PointGeometric* pointout) {
	LineGeometric line(Point, Vector);
	return line.LineBetweenLine(Line2, pointout);
}