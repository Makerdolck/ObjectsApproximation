#include "stdafx.h"

#include "SphereApprox.h"

#include "CircleApprox.h"
#include "GlobalFunctions.h"

// ---																										// Constructors
SphereApprox::SphereApprox()
{
	objectApproxName	= (char*)"sphere"; 
	IsHole				= false;
}

SphereApprox::~SphereApprox(){}

double SphereApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - r
{
	double sum = 0;

	for (int i = 0; i < arraySize; i++)
		sum += pow(Line.Point.DistanceToPoint(points[i]) - Radius, 2);

	return sum;
}

void SphereApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	CenterByPoints(points, arraySize);

	for (int i = 0; i < arraySize; i++)
		Radius += Line.Point.DistanceToPoint(points[i]);

	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;


	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;


		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);	// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);	// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);	// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius


		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Height = Radius * 2;
}
// ---																										// Triangulation
void SphereApprox::Triangulation(double inAccuracy)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();


	PointGeometric				tmpPoint;

	PlaneGeometric				tmpPlane;

	CircleApprox				circleFirst,
								circleSecond;

	std::vector<PointGeometric>	pointsFirstCircle,
								pointsSecondCircle;

	//	---	---	---	---	---

	//	---	---	Finding the angle of displacement of a point along a circle (with Heron's formula)

	double	angle = acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
			angelsSum_Q,
			angelsSum_F;

	//	---	---				(spherical coordinate system)
	//	x = R * sinQ * cosF
	//	y = R * sinQ * sinF
	//	z = R * cosQ

	double	x, y, z,
			tmpAngle_Q = angle,
			tmpAngle_F = angle;

	//	--- First circle (preparation)
	
	angelsSum_F = 0.0f;

	for (angelsSum_Q = 0.0f; angelsSum_Q <= 360; angelsSum_Q += tmpAngle_Q)
	{
		x = Radius * sin(angelsSum_Q * PI_Approx / 180.0f) * cos(angelsSum_F * PI_Approx / 180.0f);
		y = Radius * sin(angelsSum_Q * PI_Approx / 180.0f) * sin(angelsSum_F * PI_Approx / 180.0f);
		z = Radius * cos(angelsSum_Q * PI_Approx / 180.0f);

		pointsFirstCircle.push_back(PointGeometric(x, y, z));

		if ((360 - angelsSum_Q) < angle && (360 - angelsSum_Q) != 0)
		{
			tmpAngle_Q = 360 - angelsSum_Q;
		}
	}

	//	--- Start

	for (angelsSum_F = angle; angelsSum_F <= 360; angelsSum_F += tmpAngle_F)
	{		
		tmpAngle_Q = angle;

		for (angelsSum_Q = 0.0f; angelsSum_Q <= 360; angelsSum_Q += tmpAngle_Q)
		{
			x = Radius * sin(angelsSum_Q * PI_Approx / 180.0f) * cos(angelsSum_F * PI_Approx / 180.0f);
			y = Radius * sin(angelsSum_Q * PI_Approx / 180.0f) * sin(angelsSum_F * PI_Approx / 180.0f);
			z = Radius * cos(angelsSum_Q * PI_Approx / 180.0f);

			pointsSecondCircle.push_back(PointGeometric(x, y, z));

			if ((360 - angelsSum_Q) < angle && (360 - angelsSum_Q) != 0)
			{
				tmpAngle_Q = 360 - angelsSum_Q;
			}
		}
		
		// --- Push Triangles

		for (int i = 1; i < (int)pointsFirstCircle.size(); i++)
		{
			// First triangle
			Mesh.points.push_back(pointsFirstCircle[i - 1]);
			Mesh.points.push_back(pointsSecondCircle[i - 1]);
			Mesh.points.push_back(pointsFirstCircle[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

			// Second triangle
			Mesh.points.push_back(pointsFirstCircle[i]);
			Mesh.points.push_back(pointsSecondCircle[i - 1]);
			Mesh.points.push_back(pointsSecondCircle[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		}

		pointsSecondCircle.swap(pointsFirstCircle);
		pointsSecondCircle.clear();

		if ((360 - angelsSum_F) < angle && (360 - angelsSum_F) != 0)
		{
			tmpAngle_F = 360 - angelsSum_F;
		}
	}


	//	---	---	---	---	---	Transfer points from XYZ system to spher's coordinate system 

	tmpPlane = Line;

	tmpPoint = Line.Point;						// Center point of new coordinate system

	VectorGeometric vectorZ, vectorY, vectorX;	// Vectors of new coordinate system

	vectorX = VectorGeometric(1.0f, 0.0f, 0.0f);
	vectorY = VectorGeometric(0.0f, 1.0f, 0.0f);
	vectorZ = VectorGeometric(0.0f, 0.0f, 1.0f);

	for (int i = 0; i < (int)Mesh.points.size(); i++)
	{
		Mesh.points[i] = TransferPointToNewCoordinateSystem(Mesh.points[i],
															tmpPoint,
															vectorX,
															vectorY,
															vectorZ);
	}

	pointsFirstCircle.clear();
	pointsSecondCircle.clear();
}
