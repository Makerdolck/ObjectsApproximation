#include "stdafx.h"

#include "PlaneGeometric.h"


// ---																										// Constructors
PlaneGeometric::PlaneGeometric(){}
PlaneGeometric::PlaneGeometric(LineGeometric line){ Line = line; }
PlaneGeometric::PlaneGeometric(VectorGeometric vector) { Line.Vector = vector; Normalize(); }
PlaneGeometric::PlaneGeometric(PointGeometric point, VectorGeometric vector) { Line = LineGeometric(point, vector); Normalize(); }
PlaneGeometric::PlaneGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3)
{
	double A, B, C/*, D*/;
	A = point1.Y*(point2.Z - point3.Z) +
		point2.Y*(point3.Z - point1.Z) +
		point3.Y*(point1.Z - point2.Z);

	B = point1.Z*(point2.X - point3.X) +
		point2.Z*(point3.X - point1.X) +
		point3.Z*(point1.X - point2.X);

	C = point1.X*(point2.Y - point3.Y) +
		point2.X*(point3.Y - point1.Y) +
		point3.X*(point1.Y - point2.Y);

	/*D = (-1)*(
		point1.X*(point2.Y*point3.Z - point3.Y*point2.Z) +
		point2.X*(point3.Y*point1.Z - point1.Y*point3.Z) +
		point3.X*(point1.Y*point2.Z - point2.Y*point1.Z)
		);*/

	Line.Point = point1;
	Line.Vector = VectorGeometric(A, B, C);
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
	PointGeometric pointProjection;
	VectorGeometric vectorBetween(Line.Point, pointOut, false);				// Vector between point on and out of plane
	double distance = vectorBetween * Line.Vector;							// Scalar distance from point to plane along the normal

	pointProjection.X = pointOut.X - distance * Line.Vector.X;
	pointProjection.Y = pointOut.Y - distance * Line.Vector.Y;
	pointProjection.Z = pointOut.Z - distance * Line.Vector.Z;

	return pointProjection;
}
// ---																										// Vector Projection
VectorGeometric PlaneGeometric::VectorProjection(VectorGeometric vectorOut, bool flagNormalize)
{
	VectorGeometric vectorProjection;
	
	vectorProjection = vectorOut - Line.Vector * ((vectorOut*Line.Vector) / (Line.Vector*Line.Vector));

	if (flagNormalize)
		vectorProjection.Normalize();

	return vectorProjection;
}