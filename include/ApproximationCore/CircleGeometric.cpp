#include "stdafx.h"

#include "CircleGeometric.h"
#include "PlaneGeometric.h"

// ---																										// Constructors
CircleGeometric::CircleGeometric() 
{ 
	Line.Point = PointGeometric();  
	Line.Vector = VectorGeometric(0, 0, 1); 
	Radius = 1;
}
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
