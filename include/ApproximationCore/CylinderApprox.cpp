#include "stdafx.h"

#include "CylinderApprox.h"

#include "GlobalFunctions.h"
#include <algorithm>

// ---																										// Constructors
CylinderApprox::CylinderApprox() { objectApproxName = (char*)"cylinder"; }

CylinderApprox::~CylinderApprox() { pointsTopCircleEdge_Copy.clear(); pointsBottomCircleEdge_Copy.clear(); }
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

	PointBottomSurfaceCenter = Line.CreatePointOnDistance(Height / 2, false);

	//	---	---	--- Triangulation

	Triangulation(2.0f);
}
// ---																										// Triangulation
void CylinderApprox::Triangulation(double stepSize)
{
	LineGeometric	tmpLine;
	PointGeometric	tmpPoint;

	std::vector<PointGeometric> pointsBottomCircleEdge;
	//std::vector<PointGeometric> pointsBottomCircleEdge_Copy;

	//	---	---	--- Points on bottom Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle (with Heron's formula)

	double p = (Radius + Radius + stepSize) / 2;
	double h = 2 * sqrt(p*(p - Radius)*(p - Radius)*(p - stepSize)) / Radius;

	double angle = (asin(h / Radius) * 180.0 / PI_Approx), angelsSum;

	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum += angle)
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

	PlaneGeometric tmpPlane = Line;

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

	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Bottom&Top surfaces (Mesh)

	std::vector<PointGeometric> pointsSecondCircle;

	pointsBottomCircleEdge_Copy.insert(pointsBottomCircleEdge_Copy.end(), pointsBottomCircleEdge.begin(), pointsBottomCircleEdge.end());

	for (double stepSum = stepSize; stepSum <= Radius; stepSum += stepSize)
	{
		for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
		{
			tmpLine.Vector = VectorGeometric(pointsBottomCircleEdge[i - 1], pointsBottomCircleEdge[i]);
			tmpLine.Point = pointsBottomCircleEdge[i - 1];
			// Point between two points on (outer) circle
			tmpPoint = tmpLine.CreatePointOnDistance(pointsBottomCircleEdge[i - 1].DistanceToPoint(pointsBottomCircleEdge[i]) / 2);

			tmpLine.Vector = VectorGeometric(tmpPoint, PointBottomSurfaceCenter);
			tmpLine.Point = tmpPoint;
			// Point on new (inner) circle 
			tmpPoint = tmpLine.CreatePointOnDistance(stepSize);

			pointsSecondCircle.push_back(tmpPoint);

			//	.... Triangles on bottom surface
			Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
			Mesh.points.push_back(pointsBottomCircleEdge[i]);
			Mesh.points.push_back(tmpPoint);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

			
			tmpLine.Vector = Line.Vector;			

			// Triangles on Top surface
			tmpLine.Point = pointsBottomCircleEdge[i - 1];
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
			tmpLine.Point = pointsBottomCircleEdge[i];
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
			tmpLine.Point = tmpPoint;
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
			//	...	||

			if (i > 1)
			{
				//	.... Triangles on bottom surface
				Mesh.points.push_back(pointsSecondCircle[i - 2]);
				Mesh.points.push_back(pointsSecondCircle[i - 1]);
				Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);

				tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
				Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


				tmpLine.Vector = Line.Vector;

				// Triangles on Top surface
				tmpLine.Point = pointsSecondCircle[i - 2];
				Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
				tmpLine.Point = pointsSecondCircle[i - 1];
				Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
				tmpLine.Point = pointsBottomCircleEdge[i - 1];
				Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

				tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
				Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
				//	... ||
			}
		}

		//	.... Triangles on bottom surface
		Mesh.points.push_back(pointsBottomCircleEdge[0]);
		Mesh.points.push_back(pointsSecondCircle[0]);
		Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size()-1]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		Mesh.points.push_back(pointsBottomCircleEdge[0]);
		Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size()-1]);
		Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size() - 1]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		tmpLine.Vector = Line.Vector;

		// Triangles on Top surface
		tmpLine.Point = pointsBottomCircleEdge[0];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = pointsSecondCircle[0];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = pointsSecondCircle[pointsSecondCircle.size() - 1];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		tmpLine.Point = pointsBottomCircleEdge[0];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = pointsSecondCircle[pointsSecondCircle.size() - 1];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		//	...	||

		pointsSecondCircle.swap(pointsBottomCircleEdge);
		pointsSecondCircle.clear();
		
	}

	//	---	Triangles on top and bottom surfaces (which connect with center point)
	for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
	{
		//	.... Triangles on bottom surface
		Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
		Mesh.points.push_back(pointsBottomCircleEdge[i]);
		Mesh.points.push_back(PointBottomSurfaceCenter);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		tmpLine.Vector = Line.Vector;

		// Triangles on Top surface
		tmpLine.Point = pointsBottomCircleEdge[i - 1];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = pointsBottomCircleEdge[i];
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
		tmpLine.Point = PointBottomSurfaceCenter;
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

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


	tmpLine.Vector = Line.Vector;

	tmpLine.Point = pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1];
	Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
	tmpLine.Point = pointsBottomCircleEdge[0];
	Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));
	tmpLine.Point = PointBottomSurfaceCenter;
	Mesh.points.push_back(tmpLine.CreatePointOnDistance(Height));

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	...	||
	
	pointsBottomCircleEdge.clear();
	pointsBottomCircleEdge.insert(pointsBottomCircleEdge.end(), pointsBottomCircleEdge_Copy.begin(), pointsBottomCircleEdge_Copy.end());
	
	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	Lateral Surface (Mesh)
	
	tmpLine.Vector = Line.Vector;

	double tmpStepSize = stepSize;
	
	for (double stepSum = stepSize; stepSum <= Height; stepSum += tmpStepSize)
	{
		tmpLine.Point = pointsBottomCircleEdge[0];
		pointsSecondCircle.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

		for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
		{
			tmpLine.Point = pointsBottomCircleEdge[i];
			pointsSecondCircle.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));


			tmpLine.Point = pointsBottomCircleEdge[i-1];
			
			// First triangle
			Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
			Mesh.points.push_back(pointsBottomCircleEdge[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

			// Second triangle
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

			tmpLine.Point = pointsBottomCircleEdge[i];

			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
			Mesh.points.push_back(pointsBottomCircleEdge[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		}

		tmpLine.Point = pointsBottomCircleEdge[pointsBottomCircleEdge.size()-1];

		// First triangle
		Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
		Mesh.points.push_back(pointsBottomCircleEdge[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

		// Second triangle
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

		tmpLine.Point = pointsBottomCircleEdge[0];

		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
		Mesh.points.push_back(pointsBottomCircleEdge[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

		
		pointsSecondCircle.swap(pointsBottomCircleEdge);
		pointsSecondCircle.clear();

		if ((Height - stepSum) < stepSize && (Height - stepSum)!=0)
		{
			tmpStepSize = Height - stepSum;
			/*stepSum = Height - stepSize;*/
		}
	}



	//	---	---	---	---	---	Top Circle Edge for test	---	---	---	---	---	---	---	\\//

	pointsTopCircleEdge_Copy.insert(pointsTopCircleEdge_Copy.end(), pointsBottomCircleEdge_Copy.begin(), pointsBottomCircleEdge_Copy.end());

	tmpLine.Vector = Line.Vector;
	for (int i = 0; i < (int)pointsTopCircleEdge_Copy.size(); i++)
	{
		tmpLine.Point = pointsTopCircleEdge_Copy[i];
		pointsTopCircleEdge_Copy[i] = tmpLine.CreatePointOnDistance(Height);
	}
	
	
	pointsSecondCircle.clear();
	pointsBottomCircleEdge.clear();

	pointsNPquarter.clear();
	pointsNNquarter.clear();
	pointsPNquarter.clear();

}

