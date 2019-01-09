#include "stdafX.h"

#include "VectorGeometric.h"


// ---																										// Constructors
VectorGeometric::VectorGeometric(double x, double y, double z, bool flagNormalize)
{
	X = x;
	Y = y;
	Z = z;
	if (flagNormalize)
		Normalize();
}
VectorGeometric::VectorGeometric(PointGeometric pointStart, PointGeometric pointEnd, bool flagNormalize)
{
	X = pointEnd.X - pointStart.X;
	Y = pointEnd.Y - pointStart.Y;
	Z = pointEnd.Z - pointStart.Z;
	if (flagNormalize)
		Normalize();
}

VectorGeometric::~VectorGeometric(){}
// ---																										// NormaliZe
void VectorGeometric::Normalize()
{
	double length = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));
	X /= length;
	Y /= length;
	Z /= length;
}
// ---																										// Point Projection
PointGeometric VectorGeometric::PointProjection(PointGeometric pointOut, PointGeometric pointOn)
{
	double lambda;

	lambda = (-1)*(X*pointOn.X + Y * pointOn.Y + Z * pointOn.Z +
		X * (-1)*(pointOut.X) + Y * (-1)*(pointOut.Y) + Z * (-1)*(pointOut.Z)) /
		(X*X + Y * Y + Z * Z);

	PointGeometric pointProjection;

	pointProjection.X = pointOn.X + X * lambda;
	pointProjection.Y = pointOn.Y + Y * lambda;
	pointProjection.Z = pointOn.Z + Z * lambda;

	return pointProjection;
}
// ---																										// Distance To Point
double VectorGeometric::DistanceToPoint(PointGeometric pointOut, PointGeometric pointOn)
{
	return pointOut.DistanceToPoint(PointProjection(pointOut, pointOn));
}
// ---																										// Vector ^ (Vector Product)
VectorGeometric VectorGeometric::operator^(VectorGeometric vector)
{
	VectorGeometric vectorResult;

	vectorResult.X = Y * vector.Z - Z * vector.Y;
	vectorResult.Y = Z * vector.X - X * vector.Z;
	vectorResult.Z = X * vector.Y - Y * vector.X;

	return vectorResult;
}
// ---																										// Vector * (Scalar Product)
double VectorGeometric::operator*(VectorGeometric vector)
{
	return X * vector.X + Y * vector.Y + Z * vector.Z;
}
// ---																										// Vector * coefficient
VectorGeometric VectorGeometric::operator*(double coef)
{
	VectorGeometric vector;
	vector.X = X * coef;
	vector.Y = Y * coef;
	vector.Z = Z * coef;
	return vector;
}
// ---																										// Vector + Vector
VectorGeometric VectorGeometric::operator+(VectorGeometric vector)
{
	VectorGeometric vectorResult;

	vectorResult.X = X + vector.X;
	vectorResult.Y = Y + vector.Y;
	vectorResult.Z = Z + vector.Z;

	return vectorResult;
}
// ---																										// Vector - Vector
VectorGeometric VectorGeometric::operator-(VectorGeometric vector)
{
	VectorGeometric vectorResult;

	vectorResult.X = X - vector.X;
	vectorResult.Y = Y - vector.Y;
	vectorResult.Z = Z - vector.Z;

	return vectorResult;
}
// ---																										//  -Vector
VectorGeometric VectorGeometric::operator-()
{
	return VectorGeometric(-X, -Y, -Z);
}
// ---																										//  Vector = Point
void VectorGeometric::operator=(PointGeometric point)
{
	X = point.X;
	Y = point.Y;
	Z = point.Z;
}
// ---																										// Vector * Point (Scalar Product)
double VectorGeometric::operator*(PointGeometric point)
{
	return X * point.X + Y * point.Y + Z * point.Z;
}