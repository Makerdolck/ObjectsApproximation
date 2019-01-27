#include "stdafx.h"

#include "GeomObjectApprox.h"


// ---																										// Constructors
GeomObjectApprox::GeomObjectApprox() { Radius = 0.0f; Height = 0.0f; objectApproxName = nullptr; }

GeomObjectApprox::~GeomObjectApprox() { PointsForApprox.clear(); Mesh.points.clear(); Mesh.vectorsNormal.clear(); }
// ---																										// Return the Object-Name
char* GeomObjectApprox::GetName()
{
	return objectApproxName;
}
// ---																										// Find Height
void GeomObjectApprox::FindHeight(PointGeometric *points, int arraySize)
{
	PointGeometric		pointMin = Line.Point,
						pointMax = Line.Point,
						tmpPoint;
	VectorGeometric*	tmpVector = nullptr;

	for (int i = 1; i < arraySize; i++)
	{
		tmpPoint = Line.Vector.PointProjection(points[i], Line.Point);

		delete tmpVector;
		tmpVector = new VectorGeometric(tmpPoint, Line.Point);

		if ((tmpVector->X >= 0 && Line.Vector.X >= 0) || (tmpVector->X <= 0 && Line.Vector.X <= 0))
		{
			if (pointMax.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				pointMax = tmpPoint;
				continue;
			}
		}
		else
		{
			if (pointMin.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				pointMin = tmpPoint;
				continue;
			}
		}
	}

	Height = pointMin.DistanceToPoint(pointMax);

	delete tmpVector;
}
// ---																										// Find Center
PointGeometric GeomObjectApprox::CenterByPoints(PointGeometric *points, int arraySize)
{
	double sumX = 0, sumY = 0, sumZ = 0;

	for (int i = 0; i < arraySize; i++)
	{
		sumX += points[i].X;
		sumY += points[i].Y;
		sumZ += points[i].Z;
	}

	Line.Point.X = sumX / (double)arraySize;		// Finding center point
	Line.Point.Y = sumY / (double)arraySize;
	Line.Point.Z = sumZ / (double)arraySize;

	return PointGeometric(Line.Point.X, Line.Point.Y, Line.Point.Z);
}
