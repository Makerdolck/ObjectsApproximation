#include "stdafx.h"

#include "CylinderApprox.h"

#include "GlobalFunctions.h"
#include <algorithm>

// ---																										// Constructors
CylinderApprox::CylinderApprox() { objectApproxName = (char*)"cylinder"; }

CylinderApprox::~CylinderApprox(){}
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
	CenterByPoints(points, arraySize);		// Find center point

	PointGeometric tmpCenter = Line.Point;

	VectorGeometric	vector1(points[0], points[1]),
		vector2(points[0], points[2]);

	Line.Vector = vector1 ^ vector2;

	Line.Normalize();						// Vector Normalization

	for (int i = 0; i < arraySize; i++)		// Finding model radius
	{
		Radius += Line.DistanceToPoint(points[i]);
	}
	Radius /= (double)arraySize;

	///////////		Start Approximation

	double	globalDeviation = 0,
			globalDeviationOld = 0;

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

		///////////

		//Approximation(points, accuracy, &Line.Vector.x);	// Changing X - vector
		//Approximation(points, accuracy, &Line.Vector.y);	// Changing Y - vector
		//Approximation(points, accuracy, &Line.Vector.z);	// Changing Z - vector

		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.x);		// Changing X - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.y);		// Changing Y - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &lineCenterM.pointM.z);		// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);		// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);		// Changing 2 - vector


		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate1);		// Changing 1 - center
		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate2);		// Changing 2 - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);					// R

																							///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);


	Line.Point = Line.PointProjection(tmpCenter);

	FindHeight(points, arraySize);

	PointBottomSurface = Line.CreatePointOnDistance(Height / 2, false);

	//	---	---	--- Triangulation

	Triangulation(2.0f);
}
// ---																										// Triangulation
void CylinderApprox::Triangulation(double stepSize)
{
	LineGeometric	tmpLine;
	PointGeometric	tmpPoint;

	std::vector<PointGeometric> pointsBottomCircleEdge;

	int countPointsBottomCircle = 0;

	double coefficients[3][3], freeCoefficients[3], gausResult[3];

	//	---	---	---	--- Bottom Surface

	//	---	---	--- Points on bottom Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle (with Heron's formula)

	double p = (Radius + Radius + stepSize) / 2;
	double h = 2 * sqrt(p*(p - Radius)*(p - Radius)*(p - stepSize)) / Radius;

	double angle = (asin(h / Radius) * 180.0 / PI_Approx), angelsSum;
	
	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum+=angle)
	{
		p = cos(angelsSum * PI_Approx / 180.0f)*Radius;;	// X component
		h = sin(angelsSum * PI_Approx / 180.0f)*Radius;		// Y component		

		pointsBottomCircleEdge.push_back(PointGeometric(p, h, 0));
	}

	if (angelsSum < 90 || (angelsSum - angle) < 90)
	{
		pointsBottomCircleEdge.push_back(PointGeometric(0, Radius, 0));
	}

	//	---	---	Points on circle in Another quarters

	std::vector<PointGeometric> pointsNPquarter, pointsNNquarter, pointsPNquarter;

	int pointQuartetCount = pointsBottomCircleEdge.size();

	for (int i = 0; i < pointQuartetCount; i++)
	{
		//	-X	+Y
		if (i!=0&&i!= pointQuartetCount-1)
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

	PlaneGeometric tmpPlane = Line;

	VectorGeometric vectorZ, vectorY, vectorX = VectorGeometric(Line.Point, PointGeometric(Line.Point.X+10, Line.Point.Y+10, Line.Point.Z+10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;
	   
	double gaussFreeCoefficients[3], gaussResult[3];
	double **gaussCoefficients = new double*[3];
	for (int i = 0; i < 3; i++)
		gaussCoefficients[i] = new double[3];

	tmpPoint = Line.Point;	// Center point of new coordinate system

	countPointsBottomCircle = pointsBottomCircleEdge.size();	// Count of points on one circle Edge

	/*Mesh.insert(Mesh.end(), pointsBottomCircleEdge.begin(), pointsBottomCircleEdge.end());*/

	for (int i=0; i< countPointsBottomCircle; i++)
	{
		gaussCoefficients[0][0] = vectorX.X;	gaussCoefficients[0][1] = vectorX.Y;	gaussCoefficients[0][2] = vectorX.Z;	
		gaussFreeCoefficients[0]	= pointsBottomCircleEdge[i].X + tmpPoint.X*vectorX.X + tmpPoint.Y*vectorX.Y + tmpPoint.Z*vectorX.Z;

		gaussCoefficients[1][0] = vectorY.X;	gaussCoefficients[1][1] = vectorY.Y;	gaussCoefficients[1][2] = vectorY.Z;
		gaussFreeCoefficients[1]	= pointsBottomCircleEdge[i].Y + tmpPoint.X*vectorY.X + tmpPoint.Y*vectorY.Y + tmpPoint.Z*vectorY.Z;

		gaussCoefficients[2][0] = vectorZ.X;	gaussCoefficients[2][1] = vectorZ.Y;	gaussCoefficients[2][2] = vectorZ.Z;
		gaussFreeCoefficients[2]	= pointsBottomCircleEdge[i].Z + tmpPoint.X*vectorZ.X + tmpPoint.Y*vectorZ.Y + tmpPoint.Z*vectorZ.Z;

		GaussMethod(gaussCoefficients, &gaussFreeCoefficients[0], 3, &gaussResult[0]);


		pointsBottomCircleEdge[i] = PointGeometric(gaussResult[0], gaussResult[1], gaussResult[2]);
	}

	//	---	---	---	---	---	Top Circle for test
	/*pointsTopCircleEdge.insert(pointsTopCircleEdge.end(), pointsBottomCircleEdge.begin(), pointsBottomCircleEdge.end());

	tmpLine.Vector = Line.Vector;
	for (int i = 0; i < pointsTopCircleEdge.size(); i++)
	{
		tmpLine.Point = pointsTopCircleEdge[i];
		pointsTopCircleEdge[i] = tmpLine.CreatePointOnDistance(Height);
	}
	pointsTBttmCircleEdge.insert(pointsTBttmCircleEdge.end(), pointsBottomCircleEdge.begin(), pointsBottomCircleEdge.end());*/
	
	//	---	---	---	--- Lateral Surface

	std::vector<PointGeometric> pointsUpperCircle;

	tmpLine.Vector = Line.Vector;

	for (double stepSum = stepSize; stepSum <= Height; stepSum += stepSize)
	{
		tmpLine.Point = pointsBottomCircleEdge[0];
		pointsUpperCircle.push_back(tmpLine.CreatePointOnDistance(stepSize));

		for (int i = 1; i < countPointsBottomCircle; i++)
		{
			tmpLine.Point = pointsBottomCircleEdge[i];
			pointsUpperCircle.push_back(tmpLine.CreatePointOnDistance(stepSize));


			tmpLine.Point = pointsBottomCircleEdge[i-1];
			
			// First triangle
			Mesh.push_back(pointsBottomCircleEdge[i - 1]);
			Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));
			Mesh.push_back(pointsBottomCircleEdge[i]);

			// Second triangle
			Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));

			tmpLine.Point = pointsBottomCircleEdge[i];

			Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));
			Mesh.push_back(pointsBottomCircleEdge[i]);			
		}

		tmpLine.Point = pointsBottomCircleEdge[countPointsBottomCircle-1];

		// First triangle
		Mesh.push_back(pointsBottomCircleEdge[countPointsBottomCircle - 1]);
		Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));
		Mesh.push_back(pointsBottomCircleEdge[0]);

		// Second triangle
		Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));

		tmpLine.Point = pointsBottomCircleEdge[0];

		Mesh.push_back(tmpLine.CreatePointOnDistance(stepSize));
		Mesh.push_back(pointsBottomCircleEdge[0]);

		
		pointsUpperCircle.swap(pointsBottomCircleEdge);
		pointsUpperCircle.clear();

		if ((Height - stepSum) < stepSize && (Height - stepSum)!=0)
		{
			stepSum = Height - stepSize;
		}
	}

	

}