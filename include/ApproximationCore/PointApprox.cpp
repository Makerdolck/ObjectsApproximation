#include "stdafx.h"

#include "PointApprox.h"


// ---																										// Constructors
PointApprox::PointApprox(double x, double y, double z) 
{
	objectApproxName = (char*)"point";

	X = x;
	Y = y;
	Z = z;
}

PointApprox::~PointApprox() {}
// ---																										// --- APPROXIMATION ---
void PointApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	CenterByPoints(points, arraySize);

	X = Line.Point.X;
	Y = Line.Point.Y;
	Z = Line.Point.Z;
}
// ---																										// Copy from PointGeometric
PointApprox& PointApprox::operator=(PointGeometric point)
{
	X = point.X;
	Y = point.Y;
	Z = point.Z;

	return *this;
}
// ---																										// Conversation operator to PointGeometric
PointApprox::operator PointGeometric()
{
	return PointGeometric(X, Y, Z);
}