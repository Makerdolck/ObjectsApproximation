#include "stdafx.h"

#include "SphereApprox.h"

#include "CircleApprox.h"
#include "GlobalFunctions.h"

// ---																										// Constructors
SphereApprox::SphereApprox(){ objectApproxName = (char*)"sphere"; }

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
	{
		Radius += Line.Point.DistanceToPoint(points[i]);
	}
	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;


	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);	// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);	// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);	// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius

																					///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Height = Radius * 2;

	//	---	---	--- Triangulation

	Triangulation(0.5f);
}
// ---																										// Triangulation
void SphereApprox::Triangulation(double stepSize)
{
	PointGeometric				tmpPoint;

	PlaneGeometric				tmpPlane;

	LineGeometric				tmpLine,
								lineAxisZ,
								lineAxisY;

	double						profileRadius,
								tmpStepSize = stepSize;

	CircleApprox				circleFirst,
								circleSecond;

	std::vector<PointGeometric>	pointsMidleCircle;

	//	---	---	---	---	---	First half of First Hemisphere

	lineAxisZ.Vector		= VectorGeometric(0, 0, 1);
	lineAxisZ.Point			= PointGeometric();

	tmpLine.Vector			= lineAxisZ.Vector;
	circleFirst.Line.Vector = lineAxisZ.Vector;
	circleFirst.Line.Point	= PointGeometric(0, 0, 0);
	circleFirst.Radius		= Radius;

	circleFirst.Triangulation(stepSize);

	pointsMidleCircle.insert(pointsMidleCircle.begin(), circleFirst.Mesh.points.begin(), circleFirst.Mesh.points.end());

	for (double stepSum = stepSize; stepSum <= Radius / 1.2f; stepSum += tmpStepSize)
	{
		profileRadius = sqrt(Radius*Radius - stepSum * stepSum);

		tmpLine.Vector = lineAxisZ.Vector;
		tmpLine.Point = pointsMidleCircle[0];
		
		tmpPoint = tmpLine.CreatePointOnDistance(stepSum);

		tmpLine.Point = tmpPoint;
		tmpLine.Vector = VectorGeometric(tmpPoint, lineAxisZ.PointProjection(tmpPoint));

		circleSecond.Mesh.points.push_back(tmpLine.CreatePointOnDistance(Radius - profileRadius));

		for (int i = 1; i < (int)circleFirst.Mesh.points.size(); i++)
		{
			tmpLine.Vector = lineAxisZ.Vector;
			tmpLine.Point = pointsMidleCircle[i];

			tmpPoint = tmpLine.CreatePointOnDistance(stepSum);

			tmpLine.Point = tmpPoint;
			tmpLine.Vector = VectorGeometric(tmpPoint, lineAxisZ.PointProjection(tmpPoint));

			circleSecond.Mesh.points.push_back(tmpLine.CreatePointOnDistance(Radius - profileRadius));

			Mesh.points.push_back(circleFirst.Mesh.points[i - 1]);
			Mesh.points.push_back(circleFirst.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i - 1]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

			
			Mesh.points.push_back(circleFirst.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i - 1]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		}

		Mesh.points.push_back(circleFirst.Mesh.points[0]);
		Mesh.points.push_back(circleFirst.Mesh.points[circleFirst.Mesh.points.size()-1]);
		Mesh.points.push_back(circleSecond.Mesh.points[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		Mesh.points.push_back(circleFirst.Mesh.points[circleFirst.Mesh.points.size()-1]);
		Mesh.points.push_back(circleSecond.Mesh.points[circleSecond.Mesh.points.size()-1]);
		Mesh.points.push_back(circleSecond.Mesh.points[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		circleFirst.Mesh.points.clear();

		circleFirst.Mesh.points.insert(circleFirst.Mesh.points.end(), circleSecond.Mesh.points.begin(), circleSecond.Mesh.points.end());

		circleSecond.Mesh.points.clear();

		if ((Radius - stepSum) < stepSize && (Radius - stepSum) != 0)
		{
			tmpStepSize = Radius - stepSum;
		}
	}

	circleFirst.Mesh.points.clear();
	circleSecond.Mesh.points.clear();
	pointsMidleCircle.clear();

	//	---	---	---	---	---	Second half of First Hemisphere

	lineAxisY.Vector		= VectorGeometric(0, 1, 0);
	lineAxisY.Point			= PointGeometric();

	tmpLine.Vector			= lineAxisY.Vector;
	circleFirst.Line.Vector = lineAxisZ.Vector;
	circleFirst.Line.Point	= PointGeometric(0, 0, 0);
	circleFirst.Radius		= Radius;

	circleFirst.Triangulation(stepSize);

	//	---	Transfer points from XY plane to spher's coordinate system 

	for (int i = 0; i < (int)circleFirst.Mesh.points.size(); i++)
	{
		circleFirst.Mesh.points[i].Z = circleFirst.Mesh.points[i].Y;
		circleFirst.Mesh.points[i].Y = 0;
	}

	//	---

	pointsMidleCircle.insert(pointsMidleCircle.begin(), circleFirst.Mesh.points.begin(), circleFirst.Mesh.points.end());

	tmpStepSize = stepSize;

	for (double stepSum = stepSize; stepSum <= Radius / 1.2f; stepSum += tmpStepSize)
	{
		profileRadius = sqrt(Radius*Radius - stepSum * stepSum);

		tmpLine.Vector = lineAxisY.Vector;
		tmpLine.Point = pointsMidleCircle[0];

		tmpPoint = tmpLine.CreatePointOnDistance(stepSum);

		tmpLine.Point = tmpPoint;
		tmpLine.Vector = VectorGeometric(tmpPoint, lineAxisY.PointProjection(tmpPoint));

		circleSecond.Mesh.points.push_back(tmpLine.CreatePointOnDistance(Radius - profileRadius));

		for (int i = 1; i < (int)circleFirst.Mesh.points.size(); i++)
		{
			tmpLine.Vector = lineAxisY.Vector;
			tmpLine.Point = pointsMidleCircle[i];

			tmpPoint = tmpLine.CreatePointOnDistance(stepSum);

			tmpLine.Point = tmpPoint;
			tmpLine.Vector = VectorGeometric(tmpPoint, lineAxisY.PointProjection(tmpPoint));

			circleSecond.Mesh.points.push_back(tmpLine.CreatePointOnDistance(Radius - profileRadius));

			Mesh.points.push_back(circleFirst.Mesh.points[i - 1]);
			Mesh.points.push_back(circleFirst.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i - 1]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


			Mesh.points.push_back(circleFirst.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i]);
			Mesh.points.push_back(circleSecond.Mesh.points[i - 1]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		}

		Mesh.points.push_back(circleFirst.Mesh.points[0]);
		Mesh.points.push_back(circleFirst.Mesh.points[circleFirst.Mesh.points.size() - 1]);
		Mesh.points.push_back(circleSecond.Mesh.points[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		Mesh.points.push_back(circleFirst.Mesh.points[circleFirst.Mesh.points.size() - 1]);
		Mesh.points.push_back(circleSecond.Mesh.points[circleSecond.Mesh.points.size() - 1]);
		Mesh.points.push_back(circleSecond.Mesh.points[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		circleFirst.Mesh.points.clear();

		circleFirst.Mesh.points.insert(circleFirst.Mesh.points.end(), circleSecond.Mesh.points.begin(), circleSecond.Mesh.points.end());

		circleSecond.Mesh.points.clear();

		if ((Radius - stepSum) < stepSize && (Radius - stepSum) != 0)
		{
			tmpStepSize = Radius - stepSum;
		}
	}

	//	---	---	---	---	---	Second Hemisphere

	int countPoints = Mesh.points.size();

	for (int i = 0; i < countPoints; i++)
	{
		Mesh.points.push_back(Mesh.points[i] * (-1));

		if (i % 3 == 2)
		{
			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
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



	pointsMidleCircle.clear();
}