#include "stdafx.h"

#include "RectangleApprox.h"

#include "GlobalFunctions.h"


// ---																										// Constructors
RectangleApprox::RectangleApprox() { Width = 0, Height = 0; objectApproxName = (char*)"rectangle"; WanderingPoints = nullptr; }

RectangleApprox::~RectangleApprox() { delete [] WanderingPoints; }
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

	FindWidthHeightMinMaxXY(&points[0], arraySize);
}
// ---																										// --- APPROXIMATION ---
double RectangleApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;

	WanderingCenter = Plane.PointProjection(WanderingCenter);
	VectorX = Plane.VectorProjection(VectorX);
	VectorY = VectorX ^ VectorZ;

	FindPointsCoordinates(&points[0], arraySize);

	sum += pow(fabs(fabs(maxX) - fabs(minX)), 2);
	sum += pow(fabs(fabs(maxY) - fabs(minY)), 2);
	sum += Width;
	sum += Height;

	/*for (int i = 0; i < arraySize; i++)
	{
		
	}*/
	return sum;
}

void RectangleApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	WanderingPoints = new PointGeometric[arraySize];

	Plane.FindByPoints(&points[0], arraySize, accuracy);
	Line = Plane.Line;

	VectorZ = Line.Vector;
	VectorX = VectorGeometric(Line.Point, Plane.PointProjection(points[0]));
	VectorY = VectorZ ^ VectorX;

	for (int i = 0; i < arraySize; i++)
	{
		points[i] = Plane.PointProjection(points[i]);
	}

	WanderingCenter = Line.Point;

	FindPointsCoordinates(&points[0], arraySize);
	//FindWidthHeightMinMaxXY(&points[0], arraySize);



//	Line.Vector = PlaneGeometric(points[0], points[1], points[2]).Line.Vector;	// Find center vector
//	Line.Point = CenterByPoints(points, arraySize);																				// Find center point
//
//	PointGeometric tmpCenter = Line.Point;
//
	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		///////////

		Approximation(points, arraySize, accuracy, &VectorX, &VectorX.X);	// Changing X - vector
		Approximation(points, arraySize, accuracy, &VectorX, &VectorX.Y);	// Changing Y - vector
		Approximation(points, arraySize, accuracy, &VectorX, &VectorX.Z);	// Changing Z - vector

		Approximation(points, arraySize, accuracy, &VectorX, &WanderingCenter.X);		// Changing X - center
		Approximation(points, arraySize, accuracy, &VectorX, &WanderingCenter.Y);		// Changing Y - center
		Approximation(points, arraySize, accuracy, &VectorX, &WanderingCenter.Z);		// Changing Z - center

		///////////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Plane.PointProjection(WanderingCenter);

	//	---	---	--- Triangulation

	Triangulation(1.0f);
}
// ---																										// Triangulation
void RectangleApprox::Triangulation(double stepSize)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();


	double	LengthX = fabs(minX) + fabs(maxX),
			HeightY = fabs(minY) + fabs(maxY);

	double	tmpStepSizeX = stepSize,
			tmpStepSizeY = stepSize;


	Mesh.points.push_back(PointGeometric(minX, minY));										// -|-
	Mesh.points.push_back(PointGeometric(maxX, minY));										// +|-
	Mesh.points.push_back(PointGeometric(minX, maxY));										// -|+

	Mesh.points.push_back(PointGeometric(maxX, maxY));										// +|+
	Mesh.points.push_back(PointGeometric(maxX, minY));										// +|-
	Mesh.points.push_back(PointGeometric(minX, maxY));										// -|+


	//for (double stepSumY = stepSize; stepSumY <= HeightY; stepSumY += tmpStepSizeY)
	//{
	//	tmpStepSizeX = stepSize;
	//
	//	for (double stepSumX = stepSize; stepSumX <= LengthX; stepSumX += tmpStepSizeX)
	//	{
	//		Mesh.points.push_back(PointGeometric(stepSumX - stepSize, stepSumY - stepSize));		// -|-
	//		Mesh.points.push_back(PointGeometric(stepSumX, stepSumY - stepSize));					// 0|-
	//		Mesh.points.push_back(PointGeometric(stepSumX - stepSize, stepSumY));					// -|0
	//
	//		Mesh.points.push_back(PointGeometric(stepSumX, stepSumY));								// 0|0
	//		Mesh.points.push_back(PointGeometric(stepSumX, stepSumY - stepSize));					// 0|-
	//		Mesh.points.push_back(PointGeometric(stepSumX - stepSize, stepSumY));					// -|0
	//
	//		if ((LengthX - stepSumX) < stepSize && (LengthX - stepSumX) != 0)
	//		{
	//			tmpStepSizeX = LengthX - stepSumX;
	//		}
	//	}
	//
	//	if ((HeightY - stepSumY) < stepSize && (HeightY - stepSumY) != 0)
	//	{
	//		tmpStepSizeY = HeightY - stepSumY;
	//	}
	//}


	//	---	---	Transfer points from XY plane to cylinder bottom surface 

	PlaneGeometric tmpPlane = Line;

	VectorGeometric vectorZ, vectorY, vectorX;
	vectorX = VectorX;
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	PointGeometric tmpPoint = Line.Point;	// Center point of new coordinate system

	for (int i = 0; i < (int)Mesh.points.size(); i++)
	{
		Mesh.points[i] = TransferPointToNewCoordinateSystem(	Mesh.points[i],
																tmpPoint,
																vectorX,
																vectorY,
																vectorZ);
	}


	Mesh.vectorsNormal.push_back(VectorZ);
}




