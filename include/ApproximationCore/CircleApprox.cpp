#include "stdafx.h"

#include "CircleApprox.h"
#include "GlobalFunctions.h"

#include "CircleGeometric.h"

// ---																										// Constructors
CircleApprox::CircleApprox()
{
	objectApproxName = (char*)"circle";
}

CircleApprox::~CircleApprox(){}
// ---																										// --- APPROXIMATION ---
double CircleApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;

	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.Point.DistanceToPoint(points[i]) - Radius, 2);

	return sum;
}

void CircleApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	Plane.FindByPoints(points, arraySize, accuracy);
	Line = Plane.Line;

	std::vector<PointGeometric> pointsProjected;

	for (int i = 0; i < arraySize; i++)
		pointsProjected.push_back(Plane.PointProjection(points[i]));

	CenterByPoints(&pointsProjected[0], arraySize);

	for (int i = 0; i < arraySize; i++)
		Radius += Line.Point.DistanceToPoint(pointsProjected[i]);

	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;


	globalDeviation = FunctionApprox(&pointsProjected[0], arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(&pointsProjected[0], arraySize, accuracy, &Line.Vector, &Line.Point.X);	// Changing X - center
		Approximation(&pointsProjected[0], arraySize, accuracy, &Line.Vector, &Line.Point.Y);	// Changing Y - center
		Approximation(&pointsProjected[0], arraySize, accuracy, &Line.Vector, &Line.Point.Z);	// Changing Z - center

		Approximation(&pointsProjected[0], arraySize, accuracy, &Line.Vector, &Radius);	// Radius


		globalDeviation = FunctionApprox(&pointsProjected[0], arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);
}
// ---																										// Triangulation
void CircleApprox::Triangulation(double inAccuracy)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();

	PointGeometric	tmpPoint;

	//	---	---	---	--- Lateral Surface

	//	---	---	--- Points on bottom Circle

	//	---	---	Finding the angle of displacement of a point along a circle

	double	angle		= 2 * acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
			angelsSum,
			xCompon,
			yCompon;

	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum += angle)
	{
		xCompon = cos(angelsSum * PI_Approx / 180.0f) * Radius;;		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f) * Radius;			// Y component		

		Mesh.points.push_back(PointGeometric(xCompon, yCompon, 0));
	}

	if (angelsSum < 90 || (angelsSum - angle) < 90)
	{
		Mesh.points.push_back(PointGeometric(0, Radius, 0));
	}

	//	---	---	Points on circle in Another quarters

	std::vector<PointGeometric> pointsNPquarter, pointsNNquarter, pointsPNquarter;

	int pointQuartetCount = (int)Mesh.points.size();

	for (int i = 0; i < pointQuartetCount; i++)
	{
		//	-X	+Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsNPquarter.push_back(
				PointGeometric(Mesh.points[i].X*(-1), Mesh.points[i].Y, 0));
		//	-X	-Y
		pointsNNquarter.push_back(
			PointGeometric(Mesh.points[i].X*(-1), Mesh.points[i].Y*(-1), 0));
		//	+X	-Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsPNquarter.push_back(
				PointGeometric(Mesh.points[i].X, Mesh.points[i].Y*(-1), 0));
	}
	//	-X	+Y
	std::reverse(std::begin(pointsNPquarter), std::end(pointsNPquarter));
	Mesh.points.insert(Mesh.points.end(), pointsNPquarter.begin(), pointsNPquarter.end());
	pointsNPquarter.clear();
	//	-X	-Y
	Mesh.points.insert(Mesh.points.end(), pointsNNquarter.begin(), pointsNNquarter.end());
	pointsNNquarter.clear();
	//	+X	-Y
	std::reverse(std::begin(pointsPNquarter), std::end(pointsPNquarter));
	Mesh.points.insert(Mesh.points.end(), pointsPNquarter.begin(), pointsPNquarter.end());
	pointsPNquarter.clear();

	//	---	---	Transfer points from XY plane to circle bottom surface 

	PlaneGeometric tmpPlane = Line;

	VectorGeometric vectorZ, vectorY, vectorX = VectorGeometric(Line.Point, PointGeometric(Line.Point.X + 10, Line.Point.Y + 10, Line.Point.Z + 10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	tmpPoint = Line.Point;	// Center point of new coordinate system

	for (int i = 0; i < (int)Mesh.points.size(); i++)
	{
		Mesh.points[i] = TransferPointToNewCoordinateSystem(Mesh.points[i],
															tmpPoint,
															vectorX,
															vectorY,
															vectorZ);
	}

	
	Mesh.vectorsNormal.push_back(Line.Vector);

	pointsNPquarter.clear();
	pointsNNquarter.clear();
	pointsPNquarter.clear();
}
// ---																										//	---	Intersections ---

// ---																										// LineIntersection
int CircleApprox::LineIntersection(LineGeometric line, PointGeometric* point1, PointGeometric* point2)
{
	CircleGeometric circle(Line, Radius);
	return (circle.LineIntersection(line, point1, point2));
}
// ---																										// CircleIntersection
int CircleApprox::CircleIntersection(CircleApprox Circle2, PointGeometric* point1, PointGeometric* point2)
{
	CircleGeometric circle(Line, Radius);
	CircleGeometric circle2(Circle2.Line, Circle2.Radius);
	return (circle.CircleIntersection(circle2, point1, point2));
}
// ---																										// PointIntersectionMiddle
PointGeometric CircleApprox::PointIntersectionMiddle(PointApprox pointOut)
{
	CircleGeometric circle(Line, Radius);
	PointGeometric  point(pointOut.X, pointOut.Y, pointOut.Z);
	return (point.PointBetween(circle.Line.Point));
}
// ---																										// PointIntersection
PointGeometric CircleApprox::PointIntersection(PointApprox pointOut)
{
	CircleGeometric circle(Line, Radius);
	PointGeometric  point(pointOut.X, pointOut.Y, pointOut.Z);
	PlaneGeometric plane(circle.Line);
	PointGeometric pointPJ;
	pointPJ = plane.PointProjection(point);
	return (circle.PointIntersectionMiddle(pointPJ));
}