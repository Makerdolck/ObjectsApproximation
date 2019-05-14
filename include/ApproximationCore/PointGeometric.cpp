#include "stdafx.h"

#include "PointGeometric.h"


// ---																										// Constructors
PointGeometric::PointGeometric(double x, double y , double z){ X = x; Y = y; Z = z; }

PointGeometric::~PointGeometric(){}
// ---																										// Distance to Point
double PointGeometric::DistanceToPoint(PointGeometric point)
{
	return (double)sqrt(
						(X - point.X)*(X - point.X) +
						(Y - point.Y)*(Y - point.Y) +
						(Z - point.Z)*(Z - point.Z));
}
// ---																										// Point * coefficient
PointGeometric	PointGeometric::operator*(double coef)
{
	PointGeometric	pointRezult;

	pointRezult.X = X * coef;
	pointRezult.Y = Y * coef;
	pointRezult.Z = Z * coef;

	return pointRezult;
}
// ---																										// Point / coefficient
PointGeometric	PointGeometric::operator/(double coef)
{
	PointGeometric	pointRezult;

	pointRezult.X = X / coef;
	pointRezult.Y = Y / coef;
	pointRezult.Z = Z / coef;

	return pointRezult;
}
// ---																										// Point - Point
PointGeometric PointGeometric::operator-(PointGeometric point)
{
	PointGeometric pointRezult;

	pointRezult.X = X - point.X;
	pointRezult.Y = Y - point.Y;
	pointRezult.Z = Z - point.Z;

	return pointRezult;
}
// ---																										// Point + Point
PointGeometric PointGeometric::operator+(PointGeometric point)
{
	PointGeometric pointRezult;

	pointRezult.X = X + point.X;
	pointRezult.Y = Y + point.Y;
	pointRezult.Z = Z + point.Z;

	return pointRezult;
}
// ---																										// Point + Vector
PointGeometric PointGeometric::operator+(VectorGeometric vector)
{
	PointGeometric pointRezult;

	pointRezult.X = X + vector.X;
	pointRezult.Y = Y + vector.Y;
	pointRezult.Z = Z + vector.Z;

	return pointRezult;
}
// ---																										// Point - Vector
PointGeometric PointGeometric::operator-(VectorGeometric vector)
{
	PointGeometric pointRezult;

	pointRezult.X = X - vector.X;
	pointRezult.Y = Y - vector.Y;
	pointRezult.Z = Z - vector.Z;

	return pointRezult;
}
// ---																										// Point == Point
bool PointGeometric::operator==(PointGeometric point)
{
	if (X != point.X)
		return (false);
	if (Y != point.Y)
		return (false);
	if (Z != point.Z)
		return (false);

	return (true);
}
// ---																										//	---	Intersections ---

// ---																										// Point_Between_two_points
PointGeometric PointGeometric::PointBetween(PointGeometric point) {
	PointGeometric pointMean;
	pointMean.X = (X + point.X) / 2;
	pointMean.Y = (Y + point.Y) / 2;
	pointMean.Z = (Z + point.Z) / 2;
	return pointMean;
}
