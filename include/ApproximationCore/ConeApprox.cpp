#include "stdafx.h"

#include "ConeApprox.h"

#include "GlobalFunctions.h"
#include "PlaneGeometric.h"

// ---																										// Constructors
ConeApprox::ConeApprox() { objectApproxName = (char*)"cone"; Angle = 45; RadiusSmaller = 0; }

ConeApprox::~ConeApprox(){ pointsBottomCircleEdge_Copy.clear(); pointsTopCircleEdge_Copy.clear();}
// ---																										// Find Point For Bottom Center
void ConeApprox::FindPointForBottomCenter(PointGeometric *points, int arraySize)
{
	PointGeometric	PointMax = Line.Point,
					PointMin = Line.Point;

	PointGeometric		tmpPoint;
	VectorGeometric*	tmpVector = nullptr;

	for (int i = 1; i < arraySize; i++)
	{
		tmpPoint = Line.Vector.PointProjection(points[i], Line.Point);

		delete tmpVector;
		tmpVector = new VectorGeometric(tmpPoint, Line.Point);

		if ((tmpVector->X >= 0 && Line.Vector.X >= 0) || (tmpVector->X <= 0 && Line.Vector.X <= 0))
		{
			if (PointMax.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				PointMax = points[i];
				continue;
			}
		}
		else
		{
			if (PointMin.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				PointMin = points[i];
				continue;
			}
		}
	}
	delete tmpVector;

	if (Line.DistanceToPoint(PointMin) > Line.DistanceToPoint(PointMax))
		PointBottomSurfaceCenter = PointMin;
	else
		PointBottomSurfaceCenter = PointMax;

}
// ---																										// --- APPROXIMATION ---
double ConeApprox::FunctionApprox(PointGeometric *points, int arraySize)		// R - (r - h * tg(alpha))
{
	double	sum		= 0,
			height	= 0,
			R		= 0,
			r		= 0;	// r = Radius - height / tg(alpha)

	PointGeometric	PointBottomCenter;

	FindPointForBottomCenter(points, arraySize);
	PointBottomCenter = Line.PointProjection(PointBottomSurfaceCenter);

	for (int i = 0; i < arraySize; i++)
	{
		height = PointBottomCenter.DistanceToPoint(Line.PointProjection(points[i]));

		R = Line.DistanceToPoint(points[i]);

		r = Radius - height / tan(Angle* PI_Approx / 180);

		sum += pow(
			//Line.DistanceToPoint(points[i]) -
			//(Radius - Line.Point.DistanceToPoint(Line.PointProjection(points[i]))*
			//	tan(Angle* PI_Approx / 180))// 

			R - r

			, 2);
	}
	return sum;
}

void ConeApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	///////////		Find primary Center 
	
	CircleGeometric	circleBottom(points[0], points[1], points[2]);									// first cross-section
	CircleGeometric	circleTop(points[arraySize - 1], points[arraySize - 2], points[arraySize - 3]);	// last cross-section
	
	CircleGeometric tmpCircle;

	if (circleBottom.Radius < circleTop.Radius)
	{
		tmpCircle = circleBottom;
		circleBottom = circleTop;
		circleTop = tmpCircle;

	}


	PointGeometric pointsForFindCenter[2];
	pointsForFindCenter[0] = circleBottom.Point;
	pointsForFindCenter[1] = circleTop.Point;

	CenterByPoints(pointsForFindCenter, 2);

	///////////		Find primary Vector
	/*VectorGeometric	vector1(points[0], points[1]),
					vector2(points[0], points[2]);

	Line.Vector = vector1 ^ vector2;*/

	Line.Vector = VectorGeometric(circleBottom.Point, circleTop.Point);

	//Line.Normalize();										// Vector Normalization
	
	///////////		Find primary Angle

	PointGeometric	PointMax = Line.Point,
					PointMin = Line.Point,
					pointOnLineMax	= Line.Point,
					pointOnLineMin	= Line.Point,
					tmpP;

	PointGeometric		tmpPoint;
	VectorGeometric*	tmpVector = nullptr;

	for (int i = 0; i < arraySize; i++)
	{
		tmpP = points[i];
		tmpPoint = Line.Vector.PointProjection(points[i], Line.Point);

		delete tmpVector;
		tmpVector = new VectorGeometric(tmpPoint, Line.Point);

		if ((tmpVector->X >= 0 && Line.Vector.X >= 0) || (tmpVector->X <= 0 && Line.Vector.X <= 0))
		{
			if (pointOnLineMax.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				PointMax = points[i];
				pointOnLineMax = tmpPoint;
				continue;
			}
		}
		else
		{
			if (pointOnLineMin.DistanceToPoint(Line.Point) < tmpPoint.DistanceToPoint(Line.Point)) {
				PointMin = points[i];
				pointOnLineMin = tmpPoint;
				continue;
			}
		}
	}
	delete tmpVector;
	
	FindHeight(points, arraySize);
	//Height = pointOnLineMin.DistanceToPoint(pointOnLineMax);

	double		Catheter = fabs(Line.DistanceToPoint(PointMax) - Line.DistanceToPoint(PointMin));

	Angle = atan(Height / Catheter) * 180 / PI_Approx;		//* 180.0 / PI_Approx

	/////////////		Find primary Radius
	
	FindPointForBottomCenter(points, arraySize);

	Radius	= Line.DistanceToPoint(PointBottomSurfaceCenter);

	///////////		Start Approximation

	double	globalDeviation		= 0,
			globalDeviationOld	= 0;

	//	---	---	--- Exclude max Vector/\Point component

	double	*vectorMCoordinate1,	*vectorMCoordinate2;
	double	*pointMCoordinate1,		*pointMCoordinate2;

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

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		////////////	Vector

		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Vector.X);	// Changing X - vector
		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Vector.Y);	// Changing Y - vector
		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Vector.Z);	// Changing Z - vector

		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);	// Changing 2 - vector


		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);	// Changing X - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);	// Changing Y - center
		//Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);	// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate1);		// Changing 1 - center
		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate2);		// Changing 2 - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Angle);	// Angle

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius

																					///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);

	FindHeight(points, arraySize);

	//
	Angle = 90 - Angle;

	RadiusSmaller = Radius - Height * tan(Angle * PI_Approx / 180);
	
	//
	HeightPhantom = Radius * pow(tan(Angle * PI_Approx / 180), -1);

	//

	PointBottomSurfaceCenter = Line.CreatePointOnDistance(Height / 2, false);

	PointTopSurfaceCenter = LineGeometric(PointBottomSurfaceCenter, Line.Vector).CreatePointOnDistance(Height);

	//	---	---	--- Triangulation

	Triangulation(2.0f);
}
// ---																										// Triangulation
void ConeApprox::Triangulation(double stepSize)
{
	LineGeometric				tmpLine;
	PointGeometric				tmpPoint;


	std::vector<PointGeometric> pointsBottomCircleEdge;

	//	---	---	--- Points on bottom Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle (with Heron's formula)

	double p = (Radius + Radius + stepSize) / 2;
	double h = 2 * sqrt(p*(p - Radius)*(p - Radius)*(p - stepSize)) / Radius;

	double	angle = (asin(h / Radius) * 180.0 / PI_Approx), 
			angelsSum;

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

	//	---	---	Transfer points from XY plane to cone bottom surface 

	PlaneGeometric tmpPlane = Line;

	tmpPoint = PointBottomSurfaceCenter;	// Center point of new coordinate system

	VectorGeometric vectorZ, vectorY, vectorX = VectorGeometric(tmpPoint, PointGeometric(tmpPoint.X + 10, tmpPoint.Y + 10, tmpPoint.Z + 10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	for (int i = 0; i < (int) pointsBottomCircleEdge.size(); i++)
	{
		pointsBottomCircleEdge[i] = TransferPointToNewCoordinateSystem(pointsBottomCircleEdge[i],
			tmpPoint,
			vectorX,
			vectorY,
			vectorZ);
	}

	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Bottom surface (Mesh)

	std::vector<PointGeometric> pointsSecondCircle;

	pointsBottomCircleEdge_Copy.insert(pointsBottomCircleEdge_Copy.end(), pointsBottomCircleEdge.begin(), pointsBottomCircleEdge.end());

	//for (double stepSum = stepSize; stepSum <= Radius; stepSum += stepSize)
	//{
	//	for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
	//	{
	//		tmpLine.Vector = VectorGeometric(pointsBottomCircleEdge[i - 1], pointsBottomCircleEdge[i]);
	//		tmpLine.Point = pointsBottomCircleEdge[i - 1];
	//		// Point between two points on (outer) circle
	//		tmpPoint = tmpLine.CreatePointOnDistance(pointsBottomCircleEdge[i - 1].DistanceToPoint(pointsBottomCircleEdge[i]) / 2);
	//
	//		tmpLine.Vector = VectorGeometric(tmpPoint, PointBottomSurfaceCenter);
	//		tmpLine.Point = tmpPoint;
	//		// Point on new (inner) circle 
	//		tmpPoint = tmpLine.CreatePointOnDistance(stepSize);
	//
	//		pointsSecondCircle.push_back(tmpPoint);
	//
	//		//	.... Triangles on bottom surface
	//		Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
	//		Mesh.points.push_back(pointsBottomCircleEdge[i]);
	//		Mesh.points.push_back(tmpPoint);
	//
	//		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//		//	...	||
	//
	//		if (i > 1)
	//		{
	//			//	.... Triangles on bottom surface
	//			Mesh.points.push_back(pointsSecondCircle[i - 2]);
	//			Mesh.points.push_back(pointsSecondCircle[i - 1]);
	//			Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
	//
	//			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//			//	... ||
	//		}
	//	}
	//
	//	//	.... Triangles on bottom surface
	//	Mesh.points.push_back(pointsBottomCircleEdge[0]);
	//	Mesh.points.push_back(pointsSecondCircle[0]);
	//	Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size() - 1]);
	//
	//	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//
	//
	//	Mesh.points.push_back(pointsBottomCircleEdge[0]);
	//	Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
	//	Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size() - 1]);
	//
	//	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	//	...	||
	//
	//	pointsSecondCircle.swap(pointsBottomCircleEdge);
	//	pointsSecondCircle.clear();
	//
	//}
	
	//	---	Triangles on  bottom surfaces (which connect with center point)
	for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
	{
		//	.... Triangles on bottom surface
		Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
		Mesh.points.push_back(pointsBottomCircleEdge[i]);
		Mesh.points.push_back(PointBottomSurfaceCenter);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		//	... ||
	}

	//	... Last triangle of bottom surfaces
	Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsBottomCircleEdge[0]);
	Mesh.points.push_back(PointBottomSurfaceCenter);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	...	||

	pointsBottomCircleEdge.clear();
	pointsBottomCircleEdge.insert(pointsBottomCircleEdge.end(), pointsBottomCircleEdge_Copy.begin(), pointsBottomCircleEdge_Copy.end());

	// _________________________________
	// ---------------------------------

	std::vector<PointGeometric> pointsTopCircleEdge;
	/*std::vector<PointGeometric> pointsTopCircleEdge_Copy;*/

	//	---	---	--- Points on Top Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle (with Heron's formula)

	p = (RadiusSmaller + RadiusSmaller + stepSize) / 2;
	h = 2 * sqrt(p*(p - RadiusSmaller)*(p - RadiusSmaller)*(p - stepSize)) / RadiusSmaller;

	angle = (asin(h / RadiusSmaller) * 180.0 / PI_Approx);

	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum += angle)
	{
		p = cos(angelsSum * PI_Approx / 180.0f)*RadiusSmaller;;	// X component
		h = sin(angelsSum * PI_Approx / 180.0f)*RadiusSmaller;		// Y component		

		pointsTopCircleEdge.push_back(PointGeometric(p, h, 0));
	}

	if (angelsSum < 90 || (angelsSum - angle) < 90)
	{
		pointsTopCircleEdge.push_back(PointGeometric(0, RadiusSmaller, 0));
	}

	//	---	---	Points on circle in Another quarters

	pointQuartetCount = pointsTopCircleEdge.size();

	for (int i = 0; i < pointQuartetCount; i++)
	{
		//	-X	+Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsNPquarter.push_back(
				PointGeometric(pointsTopCircleEdge[i].X*(-1), pointsTopCircleEdge[i].Y, 0));
		//	-X	-Y
		pointsNNquarter.push_back(
			PointGeometric(pointsTopCircleEdge[i].X*(-1), pointsTopCircleEdge[i].Y*(-1), 0));
		//	+X	-Y
		if (i != 0 && i != pointQuartetCount - 1)
			pointsPNquarter.push_back(
				PointGeometric(pointsTopCircleEdge[i].X, pointsTopCircleEdge[i].Y*(-1), 0));
	}
	//	-X	+Y
	std::reverse(std::begin(pointsNPquarter), std::end(pointsNPquarter));
	pointsTopCircleEdge.insert(pointsTopCircleEdge.end(), pointsNPquarter.begin(), pointsNPquarter.end());
	pointsNPquarter.clear();
	//	-X	-Y
	pointsTopCircleEdge.insert(pointsTopCircleEdge.end(), pointsNNquarter.begin(), pointsNNquarter.end());
	pointsNNquarter.clear();
	//	+X	-Y
	std::reverse(std::begin(pointsPNquarter), std::end(pointsPNquarter));
	pointsTopCircleEdge.insert(pointsTopCircleEdge.end(), pointsPNquarter.begin(), pointsPNquarter.end());
	pointsPNquarter.clear();

	//	---	---	Transfer points from XY plane to cone top surface 

	tmpPlane = Line;

	tmpPoint = PointTopSurfaceCenter;	// Center point of new coordinate system

	vectorX = VectorGeometric(tmpPoint, PointGeometric(tmpPoint.X + 10, tmpPoint.Y + 10, tmpPoint.Z + 10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	for (int i = 0; i < (int)pointsTopCircleEdge.size(); i++)
	{
		pointsTopCircleEdge[i] = TransferPointToNewCoordinateSystem(pointsTopCircleEdge[i],
			tmpPoint,
			vectorX,
			vectorY,
			vectorZ);
	}

	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Top surface (Mesh)

	pointsSecondCircle.clear();

	pointsTopCircleEdge_Copy.insert(pointsTopCircleEdge_Copy.end(), pointsTopCircleEdge.begin(), pointsTopCircleEdge.end());

	//for (double stepSum = stepSize; stepSum <= RadiusSmaller; stepSum += stepSize)
	//{
	//	for (int i = 1; i < (int)pointsTopCircleEdge.size(); i++)
	//	{
	//		tmpLine.Vector = VectorGeometric(pointsTopCircleEdge[i - 1], pointsTopCircleEdge[i]);
	//		tmpLine.Point = pointsTopCircleEdge[i - 1];
	//		// Point between two points on (outer) circle
	//		tmpPoint = tmpLine.CreatePointOnDistance(pointsTopCircleEdge[i - 1].DistanceToPoint(pointsTopCircleEdge[i]) / 2);
	//
	//		tmpLine.Vector = VectorGeometric(tmpPoint, PointTopSurfaceCenter);
	//		tmpLine.Point = tmpPoint;
	//		// Point on new (inner) circle 
	//		tmpPoint = tmpLine.CreatePointOnDistance(stepSize);
	//
	//		pointsSecondCircle.push_back(tmpPoint);
	//
	//		//	.... Triangles on bottom surface
	//		Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
	//		Mesh.points.push_back(pointsTopCircleEdge[i]);
	//		Mesh.points.push_back(tmpPoint);
	//
	//		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//		//	...	||
	//
	//		if (i > 1)
	//		{
	//			//	.... Triangles on bottom surface
	//			Mesh.points.push_back(pointsSecondCircle[i - 2]);
	//			Mesh.points.push_back(pointsSecondCircle[i - 1]);
	//			Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
	//
	//			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//
	//			//	... ||
	//		}
	//	}
	//
	//	//	.... Triangles on bottom surface
	//	Mesh.points.push_back(pointsTopCircleEdge[0]);
	//	Mesh.points.push_back(pointsSecondCircle[0]);
	//	Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size() - 1]);
	//
	//	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	
	//
	//	Mesh.points.push_back(pointsTopCircleEdge[0]);
	//	Mesh.points.push_back(pointsTopCircleEdge[pointsTopCircleEdge.size() - 1]);
	//	Mesh.points.push_back(pointsSecondCircle[pointsSecondCircle.size() - 1]);
	//
	//	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	//	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	//	...	||
	//
	//	pointsSecondCircle.swap(pointsTopCircleEdge);
	//	pointsSecondCircle.clear();
	//
	//}

	//	---	Triangles on  bottom surfaces (which connect with center point)
	for (int i = 1; i < (int)pointsTopCircleEdge.size(); i++)
	{
		//	.... Triangles on bottom surface
		Mesh.points.push_back(pointsTopCircleEdge[i - 1]);
		Mesh.points.push_back(pointsTopCircleEdge[i]);
		Mesh.points.push_back(PointTopSurfaceCenter);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		//	... ||
	}

	//	... Last triangle of bottom surfaces
	Mesh.points.push_back(pointsTopCircleEdge[pointsTopCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsTopCircleEdge[0]);
	Mesh.points.push_back(PointTopSurfaceCenter);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
	//	...	||

	//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	Lateral Surface (Mesh)
	
	double tmpStepSize = stepSize;

	tmpLine.Point = PointBottomSurfaceCenter;
	tmpLine.Vector = Line.Vector;

	tmpPoint = tmpLine.CreatePointOnDistance(HeightPhantom);	// Most Top center for future Vector 

	pointsSecondCircle.clear();

	for (double stepSum = stepSize; stepSum <= Height; stepSum += tmpStepSize)
	{
		tmpLine.Point = pointsBottomCircleEdge[0];
		tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);
		pointsSecondCircle.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

		for (int i = 1; i < (int)pointsBottomCircleEdge.size(); i++)
		{
			tmpLine.Point = pointsBottomCircleEdge[i];
			tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);
			pointsSecondCircle.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));


			tmpLine.Point = pointsBottomCircleEdge[i - 1];
			tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);

			// First triangle
			Mesh.points.push_back(pointsBottomCircleEdge[i - 1]);
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
			Mesh.points.push_back(pointsBottomCircleEdge[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

			// Second triangle
			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

			tmpLine.Point = pointsBottomCircleEdge[i];
			tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);

			Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
			Mesh.points.push_back(pointsBottomCircleEdge[i]);

			tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
			Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);
		}

		tmpLine.Point = pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1];
		tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);

		// First triangle
		Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
		Mesh.points.push_back(pointsBottomCircleEdge[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

		// Second triangle
		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));

		tmpLine.Point = pointsBottomCircleEdge[0];
		tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);

		Mesh.points.push_back(tmpLine.CreatePointOnDistance(tmpStepSize));
		Mesh.points.push_back(pointsBottomCircleEdge[0]);

		tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
		Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);


		pointsSecondCircle.swap(pointsBottomCircleEdge);
		pointsSecondCircle.clear();

		if ((Height - stepSum) < stepSize && (Height - stepSum) != 0)
		{
			tmpStepSize = Height - stepSum;
			/*stepSum = Height - stepSize;*/
		}
	}



	pointsBottomCircleEdge.clear();	
	pointsTopCircleEdge.clear();
	pointsSecondCircle.clear();

	pointsNNquarter.clear();
	pointsNPquarter.clear();
	pointsPNquarter.clear();

}

