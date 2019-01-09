#include "stdafx.h"

#include "LineGeometric.h"


// ---																										// Constructors
LineGeometric::LineGeometric(){}
LineGeometric::LineGeometric(VectorGeometric vector) { Vector = vector; }
LineGeometric::LineGeometric(PointGeometric point, VectorGeometric vector) { Point = point; Vector = vector; }

LineGeometric::~LineGeometric(){}
// ---																										// NormaliZe
void LineGeometric::Normalize()
{
	Vector.Normalize();
}
// ---																										// Create Point On Distance
PointGeometric LineGeometric::CreatePointOnDistance(double distance, bool flagPositiveDirection)
{
	Normalize();
	if(flagPositiveDirection)
		return Point + Vector * distance;
	else
		return Point - Vector * distance;
}
// ---																										// Point Projection to Line
PointGeometric LineGeometric::PointProjection(PointGeometric point)
{
	double	lambda,
		x = Vector.X,
		y = Vector.Y,
		z = Vector.Z;

	lambda =	(-1)*(x*Point.X + y * Point.Y + z * Point.Z +
				x * (-1)*(point.X) + y * (-1)*(point.Y) + z * (-1)*(point.Z)) /
				(x*x + y * y + z * z);

	PointGeometric pointProjection;

	pointProjection.X = Point.X + x * lambda;
	pointProjection.Y = Point.Y + y * lambda;
	pointProjection.Z = Point.Z + z * lambda;

	return pointProjection;
}
// ---																										// Distance to Point
double LineGeometric::DistanceToPoint(PointGeometric point)
{
	return point.DistanceToPoint(Vector.PointProjection(point, Point));
}
