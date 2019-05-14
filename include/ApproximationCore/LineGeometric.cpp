#include "stdafx.h"

#include "LineGeometric.h"

#include "GlobalFunctions.h"

// ---																										// Constructors
LineGeometric::LineGeometric(){}
LineGeometric::LineGeometric(VectorGeometric vector)
{
	Vector = vector; 
}
LineGeometric::LineGeometric(PointGeometric point, VectorGeometric vector)
{ 
	Point	= point; 
	Vector	= vector; 
}

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

	if ((x == y) && (y == z) && (z == 0))
		lambda = 0;
	else
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
// ---																										//	---	Intersections ---

// ---																										// PointBetweenLineMiddle
PointGeometric  LineGeometric::PointBetweenLineMiddle(PointGeometric point) {
	PointGeometric PointProec;
	PointProec = PointProjection(point);

	PointGeometric pointMean;
	pointMean.X = (PointProec.X + point.X) / 2;
	pointMean.Y = (PointProec.Y + point.Y) / 2;
	pointMean.Z = (PointProec.Z + point.Z) / 2;
	return pointMean;
}
// ---																										// LineBetweenLine
int  LineGeometric::LineBetweenLine(LineGeometric Line2, PointGeometric* pointout) {
	double angle, degree;
	PointGeometric	pointResult;
	Line2.Normalize();
	Vector.Normalize();
	angle = (Vector.X * Line2.Vector.X + Vector.Y * Line2.Vector.Y + Vector.Z * Line2.Vector.Z) / (sqrt(pow(Vector.X, 2) + pow(Vector.Y, 2) + pow(Vector.Z, 2)) * sqrt(pow(Line2.Vector.X, 2) + pow(Line2.Vector.Y, 2) + pow(Line2.Vector.Z, 2)));
	degree = angle * 180 / PI_Approx;
	if ((degree >= 0 && degree < 180) || (degree <= 0 && degree > -180)) {

		double	gaussFreeCoefficients[3],
			gaussResult[3],
			** gaussCoefficients = new double* [3];

		for (int i = 0; i < 3; i++)
			gaussCoefficients[i] = new double[3];
		gaussCoefficients[0][0] = -Vector.X;	gaussCoefficients[0][1] = Line2.Vector.X;
		gaussFreeCoefficients[0] = Point.X - Line2.Point.X;

		gaussCoefficients[1][0] = -Vector.Y;	gaussCoefficients[1][1] = Line2.Vector.Y;
		gaussFreeCoefficients[1] = Point.Y - Line2.Point.Y;

		gaussCoefficients[2][0] = -Vector.Z;	gaussCoefficients[2][1] = Line2.Vector.Z;
		gaussFreeCoefficients[2] = Point.Z - Line2.Point.Z;
		GaussMethod(gaussCoefficients, &gaussFreeCoefficients[0], 3, &gaussResult[0]);

		pointResult.X = Point.X + Vector.X * gaussResult[0];
		pointResult.Y = Point.Y + Vector.Y * gaussResult[0];
		pointResult.Z = Point.Z + Vector.Z * gaussResult[0];

		*pointout = pointResult;
		for (int i = 0; i < 3; i++)
			delete[] gaussCoefficients[i];

		delete[] gaussCoefficients;


		return 0;
	}
	else
	{
		return 1;
	}
}