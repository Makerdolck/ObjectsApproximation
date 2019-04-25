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
	PointGeometric	point;

	point.X = X * coef;
	point.Y = Y * coef;
	point.Z = Z * coef;

	return point;
}
// ---																										// Point / coefficient
PointGeometric	PointGeometric::operator/(double coef)
{
	PointGeometric	point;

	point.X = X / coef;
	point.Y = Y / coef;
	point.Z = Z / coef;

	return point;
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
	PointGeometric point;

	point.X = X + vector.X;
	point.Y = Y + vector.Y;
	point.Z = Z + vector.Z;

	return point;
}
// ---																										// Point - Vector
PointGeometric PointGeometric::operator-(VectorGeometric vector)
{
	PointGeometric point;

	point.X = X - vector.X;
	point.Y = Y - vector.Y;
	point.Z = Z - vector.Z;

	return point;
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