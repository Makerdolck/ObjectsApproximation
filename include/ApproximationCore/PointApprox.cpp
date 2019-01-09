#include "stdafx.h"

#include "PointApprox.h"


// ---																										// Constructors
PointApprox::PointApprox() { objectApproxName = (char*)"point"; X = 0; Y = 0; Z = 0; }

PointApprox::~PointApprox(){}
// ---																										// Constructors
PointApprox&	PointApprox::operator=(PointGeometric point)
{
	X = point.X;
	Y = point.Y;
	Z = point.Z;

	return *this;
}
// ---																										// --- APPROXIMATION ---
void PointApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	CenterByPoints(points, arraySize);

	X = Line.Point.X;
	Y = Line.Point.Y;
	Z = Line.Point.Z;
}