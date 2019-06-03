#include "stdafx.h"

#include "PlaneGeometric.h"

#define _USE_MATH_DEFINES
#include "math.h"

// ---																										// Constructors
PlaneGeometric::PlaneGeometric(){}
PlaneGeometric::PlaneGeometric(LineGeometric line){ Line = line; }
PlaneGeometric::PlaneGeometric(VectorGeometric vector) 
{ 
	Line.Vector = vector; 
	Normalize(); 
}
PlaneGeometric::PlaneGeometric(PointGeometric point, VectorGeometric vector) 
{ 
	Line = LineGeometric(point, vector); 
	Normalize(); 
}
PlaneGeometric::PlaneGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3)
{
	//double A, B, C/*, D*/;

	//A = point1.Y*(point2.Z - point3.Z) +
	//	point2.Y*(point3.Z - point1.Z) +
	//	point3.Y*(point1.Z - point2.Z);

	//B = point1.Z*(point2.X - point3.X) +
	//	point2.Z*(point3.X - point1.X) +
	//	point3.Z*(point1.X - point2.X);

	//C = point1.X*(point2.Y - point3.Y) +
	//	point2.X*(point3.Y - point1.Y) +
	//	point3.X*(point1.Y - point2.Y);

	/*D = (-1)*(
		point1.X*(point2.Y*point3.Z - point3.Y*point2.Z) +
		point2.X*(point3.Y*point1.Z - point1.Y*point3.Z) +
		point3.X*(point1.Y*point2.Z - point2.Y*point1.Z)
		);*/

	Line.Point	= point1;
	//Line.Vector = VectorGeometric(A, B, C);
	Line.Vector = VectorGeometric(point1, point2) ^ VectorGeometric(point1, point3);
}

PlaneGeometric::~PlaneGeometric(){}
// ---																										// NormaliZe
void PlaneGeometric::Normalize()
{
	Line.Normalize();
}
// ---																										// Point Projection
PointGeometric PlaneGeometric::PointProjection(PointGeometric pointOut)
{
	PointGeometric	pointProjection;
	VectorGeometric vectorBetween(Line.Point, pointOut, false);	// Vector between point on and out of plane
	double			distance = vectorBetween * Line.Vector;		// Scalar distance from point to plane along the normal

	pointProjection.X = pointOut.X - distance * Line.Vector.X;
	pointProjection.Y = pointOut.Y - distance * Line.Vector.Y;
	pointProjection.Z = pointOut.Z - distance * Line.Vector.Z;

	return pointProjection;
}
// ---																										// Vector Projection
VectorGeometric PlaneGeometric::VectorProjection(VectorGeometric vectorOut, bool flagNormalize)
{
	VectorGeometric vectorProjection;
	
	vectorProjection = vectorOut - Line.Vector * (vectorOut * Line.Vector);

	if (flagNormalize)
		vectorProjection.Normalize();

	return vectorProjection;
}
// ---																										//	---	Intersections ---

// ---																										// PointBetweenPlaneMiddle
PointGeometric PlaneGeometric::PointBetweenPlaneMiddle(PointGeometric point) {

	PointGeometric PointProec;
	PointProec = PointProjection(point);

	return point.PointBetween(PointProec);
}
// ---																										// PlaneBetweenLine
int PlaneGeometric::PlaneBetweenLine(LineGeometric vectorLine, PointGeometric* pointOut) {
	PointGeometric pointintersec;

	double ckolar;
	//double ckolar1;
	//ckolar = Line.Vector.X*vectorLine.Vector.X + Line.Vector.Y*vectorLine.Vector.Y + Line.Vector.Z*vectorLine.Vector.Z ;
	ckolar = Line.Vector * vectorLine.Vector;
	if (ckolar == 0) {
		int znach;
		znach = (int)(Line.Vector * vectorLine.Point);
		if (znach == 0) {//прямая лежит в плосости
			return 1;
		}
		else {//прямая параллельна плоскости
			return 1;
		}
	}

	if (ckolar != 0) {
		double lambda;
		double D = 0;
		D = (-1) * (Line.Vector.X * Line.Point.X + Line.Vector.Y * Line.Point.Y + Line.Vector.Z * Line.Point.Z);
		//lambda = (-1)*(vectorLine.Point.X*Line.Vector.X + vectorLine.Point.Y*Line.Vector.Y + vectorLine.Point.Z*Line.Vector.Z) / (vectorLine.Vector.X*Line.Vector.X + vectorLine.Vector.Y*Line.Vector.Y + vectorLine.Vector.Z*Line.Vector.Z);
		lambda = (-1) * (Line.Vector.X * vectorLine.Point.X + Line.Vector.Y * vectorLine.Point.Y + Line.Vector.Z * vectorLine.Point.Z + D) / (Line.Vector.X * vectorLine.Vector.X + Line.Vector.Y * vectorLine.Vector.Y + Line.Vector.Z * vectorLine.Vector.Z);
		pointintersec.X = lambda * vectorLine.Vector.X + vectorLine.Point.X;
		pointintersec.Y = lambda * vectorLine.Vector.Y + vectorLine.Point.Y;
		pointintersec.Z = lambda * vectorLine.Vector.Z + vectorLine.Point.Z;
		*pointOut = pointintersec;
		return 0;
	}

	return 1;
}
// ---																										// PlaneIntersectionPlane
int PlaneGeometric::PlaneIntersectionPlane(PlaneGeometric Plane2, LineGeometric* line) {
	double D1 = 0;
	double D2 = 0;
	D1 = 1 * (Line.Vector.X * Line.Point.X + Line.Vector.Y * Line.Point.Y + Line.Vector.Z * Line.Point.Z);
	D2 = 1 * (Plane2.Line.Vector.X * Plane2.Line.Point.X + Plane2.Line.Vector.Y * Plane2.Line.Point.Y + Plane2.Line.Vector.Z * Plane2.Line.Point.Z);

	VectorGeometric VectorPr;
	LineGeometric LineInter;
	VectorPr = Line.Vector ^ Plane2.Line.Vector;
	LineInter.Vector = VectorGeometric(VectorPr.X, VectorPr.Y, VectorPr.Z, false);
	if ((VectorPr.X == 0) && (VectorPr.Y == 0) && (VectorPr.Z = 0)) {
		return 1;
	}

	else {
		/*if (VectorPr.X != 0) {
			double del;
			del = Line.Vector.Y * Plane2.Line.Vector.Z - Line.Vector.Z*Plane2.Line.Vector.Y;
			LineInter.Point.Y = (D1*Plane2.Line.Vector.Z - Line.Vector.Z*D2) / del;
			LineInter.Point.Z = (Line.Vector.Y*D2 - Plane2.Line.Vector.Y*D1) / del;

		}*/
		double del;
		del = Line.Vector.Y * Plane2.Line.Vector.Z - Line.Vector.Z * Plane2.Line.Vector.Y;
		if (del != 0) {
			LineInter.Point.Y = (D1 * Plane2.Line.Vector.Z - Line.Vector.Z * D2) / del;
			LineInter.Point.Z = (Line.Vector.Y * D2 - Plane2.Line.Vector.Y * D1) / del;
		}
		if (del == 0) {
			del = Line.Vector.X * Plane2.Line.Vector.Z - Line.Vector.Z * Plane2.Line.Vector.X;
			LineInter.Point.X = (D1 * Plane2.Line.Vector.Z - Line.Vector.Z * D2) / del;
			LineInter.Point.Z = (Line.Vector.X * D2 - Plane2.Line.Vector.X * D1) / del;
		}

		*line = LineInter;
		//return 1;
		//return LineInter;
		return 0;
	}

}
// ---																										// PlaneAngleLine
int PlaneGeometric::PlaneAngleLine(LineGeometric Lline) {
	double degreeCOS;
	degreeCOS = fabs((Lline.Vector.X * Line.Vector.X + Lline.Vector.Y * Line.Vector.Y + Lline.Vector.Z * Line.Vector.Z)) / (sqrt(pow(Lline.Vector.X, 2) + pow(Lline.Vector.Y, 2) + pow(Lline.Vector.Z, 2)) * sqrt(pow(Line.Vector.X, 2) + pow(Line.Vector.Y, 2) + pow(Line.Vector.Z, 2)));
	double angle;
	double angle1;
	angle1 = acos(degreeCOS) * 180 / PI_Approx;
	angle = (PI_Approx / 2) * 180 / PI_Approx - angle1;
	if ((angle >= 89.2) && (angle <= 90.9)) {
		return 0;
	}
	else
		return 1;
}