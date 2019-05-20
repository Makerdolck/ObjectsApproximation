#include "stdafx.h"

#include "RectangleApprox.h"

#include "GlobalFunctions.h"


// ---																										// Constructors
RectangleApprox::RectangleApprox()
{
	Width				= 0;
	Height				= 0;
	maxX				= 0;
	minX				= 0;
	maxY				= 0;
	minY				= 0;
	objectApproxName	= (char*)"rectangle";
	WanderingPoints		= nullptr;
}

RectangleApprox::~RectangleApprox()
{
	delete [] WanderingPoints;
}
// ---										 																// Find Height, Width and MaxMin X_Y
void RectangleApprox::FindWidthHeightMinMaxXY(PointGeometric *points, int arraySize)
{
	maxX = WanderingPoints[0].X,
	minX = WanderingPoints[0].X,
	maxY = WanderingPoints[0].Y,
	minY = WanderingPoints[0].Y;

	for (int i = 0; i < arraySize; i++)
	{
		if (maxX < WanderingPoints[i].X)	maxX = WanderingPoints[i].X;
		if (minX > WanderingPoints[i].X)	minX = WanderingPoints[i].X;
		if (maxY < WanderingPoints[i].Y)	maxY = WanderingPoints[i].Y;
		if (minY > WanderingPoints[i].Y)	minY = WanderingPoints[i].Y;
	}

	Width	= fabs(maxX) + fabs(minX);
	Height	= fabs(maxY) + fabs(minY);
}
// ---																										// Points Coordinates in new Placement
void RectangleApprox::FindPointsCoordinates(PointGeometric *points, int arraySize)
{
	VectorGeometric vectorR;
	for (int i = 0; i < arraySize; i++)
	{
		vectorR = VectorGeometric(WanderingCenter, points[i], false);

		WanderingPoints[i].X = vectorR * VectorX;
		WanderingPoints[i].Y = vectorR * VectorY;
		WanderingPoints[i].Z = 0;				// vectorR * Line.Vector;
	}

	FindWidthHeightMinMaxXY(points, arraySize);
}
// ---																										// --- APPROXIMATION ---
double RectangleApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;

	WanderingCenter = Plane.PointProjection(WanderingCenter);
	VectorX = Plane.VectorProjection(VectorX);
	VectorY = VectorX ^ Line.Vector;

	FindPointsCoordinates(points, arraySize);

	sum += pow(fabs(fabs(maxX) - fabs(minX)), 2);
	sum += pow(fabs(fabs(maxY) - fabs(minY)), 2);
	sum += Width;
	sum += Height;

	return sum;
}

void RectangleApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	std::vector<PointGeometric> pointsProjected;

	WanderingPoints = new PointGeometric[arraySize];

	Plane.FindByPoints(points, arraySize, accuracy);
	Line = Plane.Line;

	VectorX = VectorGeometric(Line.Point, Plane.PointProjection(points[0]));
	VectorY = Line.Vector ^ VectorX;

	for (int i = 0; i < arraySize; i++)
		pointsProjected.push_back(Plane.PointProjection(points[i]));

	WanderingCenter = Line.Point;

	FindPointsCoordinates(&pointsProjected[0], arraySize);

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;

	globalDeviation = FunctionApprox(&pointsProjected[0], arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &VectorX.X);	// Changing X - vector
		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &VectorX.Y);	// Changing Y - vector
		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &VectorX.Z);	// Changing Z - vector

		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &WanderingCenter.X);		// Changing X - center
		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &WanderingCenter.Y);		// Changing Y - center
		Approximation(&pointsProjected[0], arraySize, accuracy, &VectorX, &WanderingCenter.Z);		// Changing Z - center


		globalDeviation = FunctionApprox(&pointsProjected[0], arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Plane.PointProjection(WanderingCenter);
}
// ---																										// Triangulation
void RectangleApprox::Triangulation(double inAccuracy)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();


	Mesh.points.push_back(PointGeometric(minX, minY));										// -|-
	Mesh.points.push_back(PointGeometric(maxX, minY));										// +|-
	Mesh.points.push_back(PointGeometric(minX, maxY));										// -|+

	Mesh.points.push_back(PointGeometric(maxX, minY));										// +|-
	Mesh.points.push_back(PointGeometric(maxX, maxY));										// +|+
	Mesh.points.push_back(PointGeometric(minX, maxY));										// -|+


	//	---	---	Transfer points from XY plane to cylinder bottom surface 

	VectorGeometric vectorZ, vectorY, vectorX;

	vectorX = VectorX;
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;


	for (int i = 0; i < (int)Mesh.points.size(); i++)
	{
		Mesh.points[i] = TransferPointToNewCoordinateSystem(	Mesh.points[i],
																Line.Point,
																vectorX,
																vectorY,
																vectorZ);
	}


	Mesh.vectorsNormal.push_back(vectorZ);
}
// ---																										//	---	Intersections ---

// ---																										// PlaneIntersectionPlane
int	RectangleApprox::PlaneIntersectionPlane(RectangleApprox Plane2, LineGeometric* line) {
	PlaneGeometric plane(Line);
	PlaneGeometric plane2(Plane2.Line);
	return plane.PlaneIntersectionPlane(plane2, line);
}
// ---																										// PlaneBetweenLine
int	RectangleApprox::PlaneBetweenLine(LineGeometric line1, PointGeometric* point1) {
	PlaneGeometric plane(Line);
	//LineGeometric line(line1.Point.X, line1.Point.X);
	return plane.PlaneBetweenLine(line1, point1);
}
// ---																										// PointBetweenPlaneMiddle
PointGeometric RectangleApprox::PointBetweenPlaneMiddle(PointApprox point) {
	PlaneGeometric plane(Line);
	PointGeometric  pointout(point.X, point.Y, point.Z);
	return plane.PointBetweenPlaneMiddle(pointout);

}
// ---																										// PointBetweenPlane
PointGeometric RectangleApprox::PointBetweenPlane(PointApprox point) {
	PlaneGeometric plane(Line);
	PointGeometric  pointout(point.X, point.Y, point.Z);
	return plane.PointProjection(pointout);

}
// ---																										// PlaneIntersectionCircle
int	RectangleApprox::PlaneIntersectionCircle(CircleApprox Circle, PointGeometric* point1, PointGeometric* point2) {
	PlaneGeometric plane(Line);
	PlaneGeometric plane1(Circle.Line);
	LineGeometric LineP;
	int Res;
	Res = plane.PlaneIntersectionPlane(Circle.Line, &LineP);
	if (Res == 1)
	{
		return 1;
	}
	if (Res == 0)
	{
		CircleGeometric circle(Circle.Line, Circle.Radius);
		int Res2;
		Res2 = circle.LineIntersection(LineP, point1, point2);
		return 0;
	}

	return 1;
}