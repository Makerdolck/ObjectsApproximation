#include "stdafx.h"

#include "CircleGeometric.h"
#include "PlaneGeometric.h"

#include <OutSource/DVector/Dvector.h>
#include <OutSource/Reference.h>

// ---																										// Constructors
CircleGeometric::CircleGeometric() { Radius = 0; }
CircleGeometric::CircleGeometric(LineGeometric line, double radius)
{
	Line	= line;
	Radius	= radius;
}
CircleGeometric::CircleGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3)
{
	VectorGeometric vectorByP2P1, 
					vectorByP3P2, 
					vectorNormal;

	PointGeometric	pointBetweenP2P1,
					pointBetweenP3P2;

	double	scalarV21P21,
			scalarV32P32,
			scalarVNormalP1;

	vectorByP2P1		= VectorGeometric(point1, point2, false);
	pointBetweenP2P1	= (point1 + point2) / 2.f;
	scalarV21P21		= vectorByP2P1 * pointBetweenP2P1;

	vectorByP3P2		= VectorGeometric(point3, point2, false);
	pointBetweenP3P2	= (point2 + point3) / 2.f;
	scalarV32P32		= vectorByP3P2 * pointBetweenP3P2;

	vectorNormal	= vectorByP2P1 ^ vectorByP3P2;

	scalarVNormalP1 = vectorNormal * point1;

	// Finding center by intersection of three planes
	double scalarThreePlanesIntersection = vectorByP2P1 * (vectorByP3P2 ^ vectorNormal);


	Line.Point.X =	VectorGeometric(scalarV21P21, vectorByP2P1.Y, vectorByP2P1.Z, false) *
					(VectorGeometric(scalarV32P32, vectorByP3P2.Y, vectorByP3P2.Z, false) ^
					VectorGeometric(scalarVNormalP1, vectorNormal.Y, vectorNormal.Z, false)) / scalarThreePlanesIntersection;

	Line.Point.Y =	VectorGeometric(vectorByP2P1.X, scalarV21P21, vectorByP2P1.Z, false) *
					(VectorGeometric(vectorByP3P2.X, scalarV32P32, vectorByP3P2.Z, false) ^
					VectorGeometric(vectorNormal.X, scalarVNormalP1, vectorNormal.Z, false)) / scalarThreePlanesIntersection;

	Line.Point.Z =	VectorGeometric(vectorByP2P1.X, vectorByP2P1.Y, scalarV21P21, false) *
					(VectorGeometric(vectorByP3P2.X, vectorByP3P2.Y, scalarV32P32, false) ^
					VectorGeometric(vectorNormal.X, vectorNormal.Y, scalarVNormalP1, false)) / scalarThreePlanesIntersection;

	Radius = Line.Point.DistanceToPoint(point1);

	// Finding Vector like a normal of plane
	Line.Vector = PlaneGeometric(point1, point2, point3).Line.Vector;
}

CircleGeometric::~CircleGeometric(){}
// ---																										//	---	Intersections ---

// ---																										// LineIntersection
int CircleGeometric::LineIntersectionCircle(LineGeometric line, PointGeometric* point1, PointGeometric* point2) {
	// CircleGeometric m_N - нормаль к плоскости окружности.
	CDVector Xp, Yp;
	CDVector Zp = CDVector(Line.Vector.X, Line.Vector.Y, Line.Vector.Z);
	CDVector Pl, Vl, Cc;

	Cc = CDVector(Line.Point.X, Line.Point.Y, Line.Point.Z);
	Vl = CDVector(line.Vector.X, line.Vector.Y, line.Vector.Z).Vector2Plane(Zp);
	Pl = CDVector(line.Point.X, line.Point.Y, line.Point.Z).Node2Plane(Cc, Zp);

	Xp = (Pl - Cc);

	double dist = Xp.Length();

	Xp.Normalize();

	Yp = Zp ^ Xp;
	Yp.Normalize();

	CDVector Vl2 = CDVector(Vl * Xp, Vl * Yp, 0);
	Vl2.Normalize();

	CDVector Pl2 = CDVector(dist, 0, 0);

	double a, b, c, root, t1, t2;
	a = pow(Vl2.x, 2) + pow(Vl2.y, 2);
	b = 2 * Pl2.x * Vl2.x;
	c = -1 * Radius * Radius + Pl2.x * Pl2.x;
	double discriminant = b * b - 4 * a * c;
	if (discriminant > -0.009) {

		root = sqrt(discriminant);
		t1 = (-b - root) / (2 * a);
		t2 = (-b + root) / (2 * a);

		CDVector otv1R2 = Pl2 + Vl2 * t1;
		CDVector otv2R2 = Pl2 + Vl2 * t2;

		CDVector Otv1R3 = Cc + Xp * otv1R2.x + Yp * otv1R2.y;
		CDVector Otv2R3 = Cc + Xp * otv2R2.x + Yp * otv2R2.y;

		point1->X = Otv1R3.x;
		point1->Y = Otv1R3.y;
		point1->Z = Otv1R3.z;

		point2->X = Otv2R3.x;
		point2->Y = Otv2R3.y;
		point2->Z = Otv2R3.z;

		if (point1->DistanceToPoint(*point2) > 0.002) {
			return 2;
		}
		else {
			/*t1 = (-b) / (2 * a);
			CDVector otv1 = Pl2 + Vl2 * t1;
			CDVector Otv31 = Cc + Xp * otv1.x + Yp * otv1.y;*/
			point1->X = Otv1R3.x;
			point1->Y = Otv1R3.y;
			point1->Z = Otv1R3.z;
			return 1;
		}
	}
	else {
		return 0;
	}
}
// ---																										// CircleIntersection
int CircleGeometric::CircleIntersection(CircleGeometric Circle2, PointGeometric* point1, PointGeometric* point2) {
	// CircleGeometric m_N - нормаль к плоскости окружности.
	CDVector Xp, Yp;
	CDVector Zp = CDVector(Line.Vector.X, Line.Vector.Y, Line.Vector.Z);
	CDVector Pl, Vl, Cc;

	Cc = CDVector(Line.Point.X, Line.Point.Y, Line.Point.Z);
	Vl = CDVector(Circle2.Line.Vector.X, Circle2.Line.Vector.Y, Circle2.Line.Vector.Z).Vector2Plane(Zp);
	Pl = CDVector(Circle2.Line.Point.X, Circle2.Line.Point.Y, Circle2.Line.Point.Z).Node2Plane(Cc, Zp);

	Xp = (Pl - Cc);

	double dist = Xp.Length();

	Xp.Normalize();

	Yp = Zp ^ Xp;
	Yp.Normalize();

	CDVector Vl2 = CDVector(Vl * Xp, Vl * Yp, 0);
	Vl2.Normalize();

	CDVector Pl2 = CDVector(dist, 0, 0);

	// l - расстояние до прямой пересечения
	// ax, ay - точки на прямой пересечения
	double  D, x1, y1, y2, x2, l, ax, ay, h;

	D = Pl2.x;
	l = (pow(Radius, 2) - pow(Circle2.Radius, 2) + pow(D, 2)) / (2 * D);
	h = sqrt(pow(Radius, 2) - pow(l, 2));
	ax = l * Pl2.x / D;
	ay = 0;

	x1 = ax;
	y1 = ay + h * Pl2.x / D;

	x2 = ax;
	y2 = ay - h * Pl2.x / D;

	if (fabs(x1 - x2) > 0.002 || fabs(y1 - y2) > 0.002) {
		CDVector Otv31 = Cc + Xp * x1 + Yp * y1;
		CDVector Otv32 = Cc + Xp * x1 + Yp * y2;

		point1->X = Otv31.x;
		point1->Y = Otv31.y;
		point1->Z = Otv31.z;

		point2->X = Otv32.x;
		point2->Y = Otv32.y;
		point2->Z = Otv32.z;
		return 2;
	}
	else if (fabs(x1 - x2) < 0.002 && fabs(y1 - y2) < 0.002) {
		CDVector Otv31 = Cc + Xp * x1 + Yp * y1;
		point1->X = Otv31.x;
		point1->Y = Otv31.y;
		point1->Z = Otv31.z;
		return 1;
	}
	else {
		return 0;
	}
}
// ---																										// PointIntersection
PointGeometric CircleGeometric::PointIntersectionCircle(PointGeometric pointOut) {
	PlaneGeometric plane(Line);
	PointGeometric pointPJ;
	pointPJ = plane.PointProjection(pointOut);
	VectorGeometric Vector1(Line.Point, pointPJ);
	LineGeometric Line1(Line.Point, Vector1);
	return Line1.CreatePointOnDistance(Radius);
}

