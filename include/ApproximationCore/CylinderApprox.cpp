#include "stdafx.h"

#include "CylinderApprox.h"

#include "GlobalFunctions.h"
#include "CircleGeometric.h"


// ---																										// Constructors
CylinderApprox::CylinderApprox()
{
	objectApproxName	= (char*)"cylinder"; 
	IsHole				= false;
}

CylinderApprox::~CylinderApprox()
{
	pointsTopCircleEdge.clear();
	pointsBottomCircleEdge.clear();
}
// ---																										// --- APPROXIMATION ---
double CylinderApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;

	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.DistanceToPoint(points[i]) - Radius, 2);

	return sum;
}

void CylinderApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	CircleGeometric	circleBottom(points[0], points[1], points[2]);									// first cross-section
	CircleGeometric	circleTop(points[arraySize - 1], points[arraySize - 2], points[arraySize - 3]);	// last cross-section
	PointGeometric pointsForFindCenter[2];
	pointsForFindCenter[0] = circleBottom.Line.Point;
	pointsForFindCenter[1] = circleTop.Line.Point;

	CenterByPoints(pointsForFindCenter, 2);

	PointGeometric tmpCenter = Line.Point;

	VectorGeometric	vector1(points[0], points[1]),
					vector2(points[0], points[2]);

	Line.Vector = vector1 ^ vector2;

	Line.Normalize();								// Vector Normalization

	for (int i = 0; i < arraySize; i++)				// Finding model radius
		Radius += Line.DistanceToPoint(points[i]);

	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;

	//	---	---	--- Exclude max Vector/\Point component

	double	*vectorMCoordinate1, *vectorMCoordinate2;
	double	*pointMCoordinate1, *pointMCoordinate2;

	// Find Max Vector/\Point Element
	if ((fabs(Line.Vector.X) > fabs(Line.Vector.Y)) && (fabs(Line.Vector.X) > fabs(Line.Vector.Z)))
	{
		//lineCenterM.vectorM.X;
		vectorMCoordinate1 = &Line.Vector.Y;
		vectorMCoordinate2 = &Line.Vector.Z;

		pointMCoordinate1 = &Line.Point.Y;
		pointMCoordinate2 = &Line.Point.Z;
	}
	else
	{
		if ((fabs(Line.Vector.Y) > fabs(Line.Vector.X)) && (fabs(Line.Vector.Y) > fabs(Line.Vector.Z)))
		{
			//Line.Vector.Y;
			vectorMCoordinate1 = &Line.Vector.X;
			vectorMCoordinate2 = &Line.Vector.Z;

			pointMCoordinate1 = &Line.Point.X;
			pointMCoordinate2 = &Line.Point.Z;
		}
		else
		{
			//Line.Vector.Z;
			vectorMCoordinate1 = &Line.Vector.Y;
			vectorMCoordinate2 = &Line.Vector.X;

			pointMCoordinate1 = &Line.Point.Y;
			pointMCoordinate2 = &Line.Point.X;
		}
	}

	//	---	---	--- Approximation

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);		// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);		// Changing 2 - vector


		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate1);		// Changing 1 - center
		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate2);		// Changing 2 - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);					// R


		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Line.PointProjection(tmpCenter);

	FindHeight(points, arraySize);

	PointTopSurfaceCenter		= Line.CreatePointOnDistance(Height / 2);
	PointBottomSurfaceCenter	= Line.CreatePointOnDistance(Height / 2, false);
}
// ---																										// Triangulation
void CylinderApprox::Triangulation(double inAccuracy)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();
	pointsTopCircleEdge.clear();
	pointsBottomCircleEdge.clear();

	PointTopSurfaceCenter		= Line.CreatePointOnDistance(Height / 2);
	PointBottomSurfaceCenter	= Line.CreatePointOnDistance(Height / 2, false);


	//	---	---	--- Points on Bottom Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle

	double	angle		= acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
			angelsSum,
			xCompon,
			yCompon;

	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum += angle)
	{
		xCompon = cos(angelsSum * PI_Approx / 180.0f)*Radius;;		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f)*Radius;		// Y component		

		pointsBottomCircleEdge.push_back(PointGeometric(xCompon, yCompon, 0));
	}

	if (angelsSum < 90 || (angelsSum - angle) < 90)
	{
		pointsBottomCircleEdge.push_back(PointGeometric(0, Radius, 0));
	}

	//	---	---	Points on circle in Another quarters

	std::vector<PointGeometric> pointsNPquarter, pointsNNquarter, pointsPNquarter;

	int pointQuartetCount = (int)pointsBottomCircleEdge.size();

	for (int i = 0; i < pointQuartetCount; i++)
	{
		//	-X	+Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsNPquarter.push_back(
				PointGeometric(pointsBottomCircleEdge[i].X*(-1), pointsBottomCircleEdge[i].Y, 0));
		//	-X	-Y
		pointsNNquarter.push_back(
			PointGeometric(pointsBottomCircleEdge[i].X*(-1), pointsBottomCircleEdge[i].Y*(-1), 0));
		//	+X	-Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsPNquarter.push_back(
				PointGeometric(pointsBottomCircleEdge[i].X, pointsBottomCircleEdge[i].Y*(-1), 0));
	}
	//	-X	+Y
	std::reverse(std::begin(pointsNPquarter), std::end(pointsNPquarter));
	pointsBottomCircleEdge.insert(pointsBottomCircleEdge.end(), pointsNPquarter.begin(), pointsNPquarter.end());
	pointsNPquarter.clear();
	//	-X	-Y
	pointsBottomCircleEdge.insert(pointsBottomCircleEdge.end(), pointsNNquarter.begin(), pointsNNquarter.end());
	pointsNNquarter.clear();
	//	+X	-Y
	std::reverse(std::begin(pointsPNquarter), std::end(pointsPNquarter));
	pointsBottomCircleEdge.insert(pointsBottomCircleEdge.end(), pointsPNquarter.begin(), pointsPNquarter.end());
	pointsPNquarter.clear();

	//	---	---	Transfer points from XY plane to cylinder bottom surface 

	PlaneGeometric	tmpPlane = Line;
	PointGeometric	tmpPoint;

	tmpPoint = PointBottomSurfaceCenter;	// Center point of new coordinate system

	VectorGeometric vectorZ, vectorY, vectorX = VectorGeometric(tmpPoint, PointGeometric(tmpPoint.X + 10, tmpPoint.Y + 10, tmpPoint.Z + 10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	for (int i = 0; i < (int)pointsBottomCircleEdge.size(); i++)
	{
		pointsBottomCircleEdge[i] = TransferPointToNewCoordinateSystem(	pointsBottomCircleEdge[i], 
																		tmpPoint, 
																		vectorX, 
																		vectorY, 
																		vectorZ);
	}

	//	---	---	---	Points on Top Circle Edge 

	LineGeometric	tmpLine;

	tmpLine.Vector = Line.Vector;

	for (int i = 0; i < (int)pointsBottomCircleEdge.size(); i++)
	{
		tmpLine.Point = pointsBottomCircleEdge[i];
		pointsTopCircleEdge.push_back(tmpLine.CreatePointOnDistance(Height));
	}

	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Bottom&Top surfaces (Mesh)

	if (IsHole == false)
	{
		//	---	Triangles on top and bottom surfaces (which connect with center point)
		for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
		{
			//	.... Triangles on bottom surface
			Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
			Mesh.points.push_back(pointsBottomCircleEdge[i]);
			Mesh.points.push_back(PointBottomSurfaceCenter);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


			// ....	Triangles on Top surface
			Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
			Mesh.points.push_back(pointsTopCircleEdge[i]);
			Mesh.points.push_back(PointTopSurfaceCenter);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
			//	... ||
		}

		//	... Last triangles of top and bottom surfaces
		Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
		Mesh.points.push_back(pointsBottomCircleEdge[0]);
		Mesh.points.push_back(PointBottomSurfaceCenter);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		Mesh.points.push_back(pointsTopCircleEdge[pointsTopCircleEdge.size() - 1]);
		Mesh.points.push_back(pointsTopCircleEdge[0]);
		Mesh.points.push_back(PointTopSurfaceCenter);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		//	...	||	
	}
	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	Lateral Surface (Mesh)
	
	for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
	{		
		// First triangle
		Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
		Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
		Mesh.points.push_back(pointsBottomCircleEdge[i]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

		// Second triangle
		Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
		Mesh.points.push_back(pointsTopCircleEdge[i]);
		Mesh.points.push_back(pointsBottomCircleEdge[i]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	}

	// First triangle
	Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsTopCircleEdge[pointsTopCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsBottomCircleEdge[0]);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

	// Second triangle
	Mesh.points.push_back(pointsTopCircleEdge[pointsTopCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsTopCircleEdge[0]);
	Mesh.points.push_back(pointsBottomCircleEdge[0]);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);




	pointsNPquarter.clear();
	pointsNNquarter.clear();
	pointsPNquarter.clear();

}

