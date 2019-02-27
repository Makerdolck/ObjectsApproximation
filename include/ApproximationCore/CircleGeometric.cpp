#include "stdafx.h"

#include "CircleGeometric.h"


// ---																										// Constructors
CircleGeometric::CircleGeometric() { Line.Point = PointGeometric(); Radius = 1; }
CircleGeometric::CircleGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3)
{
	VectorGeometric vectorByP2P1, vectorByP3P2, vectorNormal;
	PointGeometric pointBetweenP2P1, pointBetweenP3P2;
	double scalarV21P21, scalarV32P32, scalarVNormalP1;

	vectorByP2P1 = VectorGeometric(point1, point2, false);
	pointBetweenP2P1 = (point1 + point2) / 2.f;
	scalarV21P21 = vectorByP2P1 * pointBetweenP2P1;

	vectorByP3P2 = VectorGeometric(point3, point2, false);
	pointBetweenP3P2 = (point2 + point3) / 2.f;
	scalarV32P32 = vectorByP3P2 * pointBetweenP3P2;

	vectorNormal = vectorByP2P1 ^ vectorByP3P2;

	scalarVNormalP1 = vectorNormal * point1;

	// Find center by intersection of three planes
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

	//CDNode P0(point1.X, point1.Y, point1.Z);
	//CDNode P1(point2.X, point2.Y, point2.Z);
	//CDNode P2(point3.X, point3.Y, point3.Z);
	//CDVector  n1, n2, n3;
	//CDNode  p0, p1, p2, p3;
	//double d1, d2, d3;

	//n1 = P1 - P0;
	////	fprintf( g_pLogFile, "n1 = %f\n", n1.Length());
	////if (n1.Length() < 0.01) return FALSE;

	//p1 = (P1 + P0) / 2.f;
	//d1 = n1 * p1;

	//n2 = P2 - P1;
	////	fprintf( g_pLogFile, "n2 = %f\n", n2.Length());
	////if (n2.Length() < 0.01) return FALSE;

	//p2 = (P2 + P1) / 2.f;
	//d2 = n2 * p2;

	//n3 = n1 ^ n2;
	////	fprintf( g_pLogFile, "n3 = %f\n", n3.Length());
	////if (n3.Length() < 0.001) return FALSE;
	//p3 = P0;
	//d3 = n3 * p3;


	//// Определяем центр пересечением трех плоскостей
	//double d = n1 * (n2 ^ n3);

	////***04.04 SVP	if( fabs(d) < 1.e-6 )			// ERROR точки лежат в одной плоскости
	////if (fabs(d) < 1.e-8)			// ERROR точки лежат на одной прямой 
	////{
	////	//		SetError( "Точки лежат в одной плоскости" );
	////	SetError("Точки лежат на одной прямой");
	////	return FALSE;
	////}
	//PointGeometric PointRRR;
	//PointRRR.X = CDVector(d1, n1.y, n1.z) * (CDVector(d2, n2.y, n2.z) ^ CDVector(d3, n3.y, n3.z)) / d;
	//PointRRR.Y = CDVector(n1.x, d1, n1.z) * (CDVector(n2.x, d2, n2.z) ^ CDVector(n3.x, d3, n3.z)) / d;
	//PointRRR.Z = CDVector(n1.x, n1.y, d1) * (CDVector(n2.x, n2.y, d2) ^ CDVector(n3.x, n3.y, d3)) / d;

}

CircleGeometric::~CircleGeometric(){}
