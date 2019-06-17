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
	double angle, Phi;
	double mixedProduct;
	VectorGeometric VectorPoints(Line2.Point, Point);
	mixedProduct = (Line2.Vector.Y*VectorPoints.Z - VectorPoints.Y*Line2.Vector.Z)*Vector.X - Line2.Vector.X*(Vector.Y*VectorPoints.Z - VectorPoints.Y*Vector.Z) + VectorPoints.X*(Vector.Y*Line2.Vector.Z - Line2.Vector.Y*Vector.Z);
	double l = RoundingOf(mixedProduct, 3);
	if (RoundingOf(mixedProduct, 3) == 0) {
		Phi = fabs(Vector.X * Line2.Vector.X + Vector.Y * Line2.Vector.Y + Vector.Z * Line2.Vector.Z) / (sqrt(pow(Vector.X, 2) + pow(Vector.Y, 2) + pow(Vector.Z, 2)) * sqrt(pow(Line2.Vector.X, 2) + pow(Line2.Vector.Y, 2) + pow(Line2.Vector.Z, 2)));
		angle = acos(Phi) * 180 / PI_Approx;

		if (angle > 0 && angle <= 90) {
			PointGeometric	pointResult;


			double	gaussFreeCoefficients[3],
				gaussResult[2],
				**gaussCoefficients = new double*[3];

			for (int i = 0; i < 3; i++)
				gaussCoefficients[i] = new double[3];
			gaussCoefficients[0][0] = -Vector.X;	gaussCoefficients[0][1] = Line2.Vector.X;
			gaussFreeCoefficients[0] = Point.X - Line2.Point.X;

			gaussCoefficients[1][0] = -Vector.Y;	gaussCoefficients[1][1] = Line2.Vector.Y;
			gaussFreeCoefficients[1] = Point.Y - Line2.Point.Y;

			gaussCoefficients[2][0] = -Vector.Z;	gaussCoefficients[2][1] = Line2.Vector.Z;
			gaussFreeCoefficients[2] = Point.Z - Line2.Point.Z;

			double tmp;
			if (gaussCoefficients[0][0] == 0) {

				if (gaussCoefficients[1][0] == 0) {
					tmp = gaussCoefficients[0][0];
					gaussCoefficients[0][0] = gaussCoefficients[2][0];
					gaussCoefficients[2][0] = tmp;

					tmp = gaussCoefficients[0][1];
					gaussCoefficients[0][1] = gaussCoefficients[2][1];
					gaussCoefficients[2][1] = tmp;

					tmp = gaussFreeCoefficients[0];
					gaussFreeCoefficients[0] = gaussFreeCoefficients[2];
					gaussFreeCoefficients[2] = tmp;
				}
				else {
					tmp = gaussCoefficients[0][0];
					gaussCoefficients[0][0] = gaussCoefficients[1][0];
					gaussCoefficients[1][0] = tmp;

					tmp = gaussCoefficients[0][1];
					gaussCoefficients[0][1] = gaussCoefficients[1][1];
					gaussCoefficients[1][1] = tmp;

					tmp = gaussFreeCoefficients[0];
					gaussFreeCoefficients[0] = gaussFreeCoefficients[1];
					gaussFreeCoefficients[1] = tmp;
				}
			}

			if (gaussCoefficients[1][1] == 0) {
				tmp = gaussCoefficients[1][0];
				gaussCoefficients[1][0] = gaussCoefficients[2][0];
				gaussCoefficients[2][0] = tmp;

				tmp = gaussCoefficients[1][1];
				gaussCoefficients[1][1] = gaussCoefficients[2][1];
				gaussCoefficients[2][1] = tmp;

				tmp = gaussFreeCoefficients[1];
				gaussFreeCoefficients[1] = gaussFreeCoefficients[2];
				gaussFreeCoefficients[2] = tmp;

			}

			GaussMethod(gaussCoefficients, &gaussFreeCoefficients[0], 2, &gaussResult[0]);

			pointResult.X = Point.X + Vector.X * gaussResult[0];
			pointResult.Y = Point.Y + Vector.Y * gaussResult[0];
			pointResult.Z = Point.Z + Vector.Z * gaussResult[0];

			*pointout = pointResult;
			for (int i = 0; i < 3; i++)
				delete[] gaussCoefficients[i];

			delete[] gaussCoefficients;

			return 0;
		}
		else {
			return 1;
		}
	}
	else
	{
		VectorGeometric VectorProd = Vector ^ Line2.Vector;
		PointGeometric	pointLine1;
		PointGeometric	pointLine2;

		double	gaussFreeCoefficients[3],
			gaussResult[3],
			**gaussCoefficients = new double*[3];

		for (int i = 0; i < 3; i++)
			gaussCoefficients[i] = new double[3];
		gaussCoefficients[0][0] = Line2.Vector.X;	gaussCoefficients[0][1] = -Vector.X;	gaussCoefficients[0][2] = -VectorProd.X;
		gaussFreeCoefficients[0] = Line2.Point.X - Point.X;

		gaussCoefficients[1][0] = Line2.Vector.Y;	gaussCoefficients[1][1] = -Vector.Y;	gaussCoefficients[1][2] = -VectorProd.Y;
		gaussFreeCoefficients[1] = Line2.Point.Y - Point.Y;

		gaussCoefficients[2][0] = Line2.Vector.Z;	gaussCoefficients[2][1] = -Vector.Z;	gaussCoefficients[2][2] = -VectorProd.Z;
		gaussFreeCoefficients[2] = Line2.Point.Z - Point.Z;

		double D = gaussCoefficients[0][0] * gaussCoefficients[1][1] * gaussCoefficients[2][2]
			+ gaussCoefficients[0][1] * gaussCoefficients[1][2] * gaussCoefficients[2][0]
			+ gaussCoefficients[1][0] * gaussCoefficients[2][1] * gaussCoefficients[0][2]
			- gaussCoefficients[2][0] * gaussCoefficients[1][1] * gaussCoefficients[0][2]
			- gaussCoefficients[1][0] * gaussCoefficients[0][1] * gaussCoefficients[2][2]
			- gaussCoefficients[0][0] * gaussCoefficients[2][1] * gaussCoefficients[1][2];

		KramerMethod(gaussCoefficients, &gaussFreeCoefficients[0], &gaussResult[0], D);

		pointLine1.X = Line2.Point.X + Line2.Vector.X * gaussResult[0];
		pointLine1.Y = Line2.Point.Y + Line2.Vector.Y * gaussResult[0];
		pointLine1.Z = Line2.Point.Z + Line2.Vector.Z * gaussResult[0];

		pointLine2.X = Point.X + Vector.X * gaussResult[1];
		pointLine2.Y = Point.Y + Vector.Y * gaussResult[1];
		pointLine2.Z = Point.Z + Vector.Z * gaussResult[1];

		PointGeometric	pointResult;
		pointResult = pointLine1.PointBetween(pointLine2);


		*pointout = pointResult;
		for (int i = 0; i < 3; i++)
			delete[] gaussCoefficients[i];

		delete[] gaussCoefficients;


		return 2;
	}
}