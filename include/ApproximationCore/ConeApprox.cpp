#include "stdafx.h"

#include "ConeApprox.h"

#include "GlobalFunctions.h"
#include "PlaneGeometric.h"

// ---																										// Constructors
ConeApprox::ConeApprox()
{
	objectApproxName	= (char*)"cone";
	Angle				= 0;
	RadiusSmaller		= 0;
	IsHole				= false;
}

ConeApprox::~ConeApprox()
{
	pointsBottomCircleEdge.clear();
	pointsTopCircleEdge.clear();
}
// ---																										// Find Point For Bottom Center
void ConeApprox::FindPointForBottomCenter(PointGeometric *points, int arraySize)
{
	PointGeometric	PointMax = Line.Point,
					PointMin = Line.Point;

	PointGeometric		tmpPoint;
	VectorGeometric		tmpVector;

	for (int i = 1; i < arraySize; i++)
	{
		tmpPoint = Line.Vector.PointProjection(points[i], Line.Point);

		tmpVector = VectorGeometric(tmpPoint, Line.Point);

		if ((tmpVector.X >= 0 && Line.Vector.X >= 0) || (tmpVector.X <= 0 && Line.Vector.X <= 0))
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

	FindHeight(points, arraySize);
	
	RadiusSmaller = Radius - Height / tan(Angle * PI_Approx / 180);

	for (int i = 0; i < arraySize; i++)
	{
		height = Height - PointBottomCenter.DistanceToPoint(Line.PointProjection(points[i]));

		R = Line.DistanceToPoint(points[i]);

		r = height / tan(Angle* PI_Approx / 180) + RadiusSmaller;
		
		sum += pow((R - r) * sin(Angle * PI_Approx / 180), 2);
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
		tmpCircle		= circleBottom;
		circleBottom	= circleTop;
		circleTop		= tmpCircle;
	}


	PointGeometric pointsForFindCenter[2];
	pointsForFindCenter[0] = circleBottom.Line.Point;
	pointsForFindCenter[1] = circleTop.Line.Point;

	CenterByPoints(pointsForFindCenter, 2);

	///////////		Find primary Vector

	Line.Vector = VectorGeometric(circleBottom.Line.Point, circleTop.Line.Point);

	///////////		Find primary Angle

	PointGeometric	PointMax = Line.Point,
					PointMin = Line.Point,
					pointOnLineMax	= Line.Point,
					pointOnLineMin	= Line.Point,
					tmpP;

	PointGeometric		tmpPoint;
	VectorGeometric		tmpVector;

	for (int i = 0; i < arraySize; i++)
	{
		tmpP = points[i];
		tmpPoint = Line.Vector.PointProjection(points[i], Line.Point);

		tmpVector =  VectorGeometric(tmpPoint, Line.Point);

		if ((tmpVector.X >= 0 && Line.Vector.X >= 0) || (tmpVector.X <= 0 && Line.Vector.X <= 0))
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
	
	FindHeight(points, arraySize);

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


		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);	// Changing 2 - vector

		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate1);		// Changing 1 - center
		Approximation(points, arraySize, accuracy, &Line.Vector, pointMCoordinate2);		// Changing 2 - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &Angle);	// Angle

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius


		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);

	FindHeight(points, arraySize);

	//
	Angle = 90 - Angle;

	RadiusSmaller = Radius - Height * tan(Angle * PI_Approx / 180);
	
	//
	HeightPhantom = Radius * pow(tan(Angle * PI_Approx / 180), -1);

	//

	PointBottomSurfaceCenter	= Line.CreatePointOnDistance(Height / 2, false);
	PointTopSurfaceCenter		= LineGeometric(PointBottomSurfaceCenter, Line.Vector).CreatePointOnDistance(Height);
}
// ---																										// Triangulation
void ConeApprox::Triangulation(double inAccuracy)
{
	Mesh.points.clear();
	Mesh.vectorsNormal.clear();
	pointsTopCircleEdge.clear();
	pointsBottomCircleEdge.clear();

	PointBottomSurfaceCenter	= Line.CreatePointOnDistance(Height / 2, false);
	PointTopSurfaceCenter		= LineGeometric(PointBottomSurfaceCenter, Line.Vector).CreatePointOnDistance(Height);

	//	---	---	--- Points on bottom Circle Edge

	//	---	---	Finding the angle of displacement of a point along a circle

	double	angle = 2 * acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
			angelsSum,
			xCompon,
			yCompon;

	//	---	---	Points on circle in Positive quarter:		+X	+Y

	for (angelsSum = 0.0f; angelsSum <= 90; angelsSum += angle)
	{
		xCompon = cos(angelsSum * PI_Approx / 180.0f) * Radius;;		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f) * Radius;		// Y component		

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

	//	---	---	Transfer points from XY plane to cone bottom surface 

	PlaneGeometric tmpPlane = Line;

	PointGeometric tmpPoint = PointBottomSurfaceCenter;	// Center point of new coordinate system

	VectorGeometric vectorZ, vectorY, vectorX = VectorGeometric(tmpPoint, PointGeometric(tmpPoint.X + 10, tmpPoint.Y + 10, tmpPoint.Z + 10));
	vectorX = tmpPlane.VectorProjection(vectorX);
	vectorZ = Line.Vector;
	vectorY = vectorX ^ vectorZ;

	for (int i = 0; i < (int) pointsBottomCircleEdge.size(); i++)
	{
		pointsBottomCircleEdge[i] = TransferPointToNewCoordinateSystem(	pointsBottomCircleEdge[i],
																		tmpPoint,
																		vectorX,
																		vectorY,
																		vectorZ);
	}

	//	---	---	---	Points on Top Circle Edge 

	LineGeometric	tmpLine;

	double offsetDistance = sqrt(Height * Height + pow((Radius - RadiusSmaller), 2));

	tmpLine.Point	= PointBottomSurfaceCenter;
	tmpLine.Vector	= Line.Vector;

	tmpPoint = tmpLine.CreatePointOnDistance(HeightPhantom);	// Most Top center for future Vector 

	for (int i = 0; i < (int)pointsBottomCircleEdge.size(); i++)
	{
		tmpLine.Point = pointsBottomCircleEdge[i];
		tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);		// ×òî-òî òóò íå  òàê

		pointsTopCircleEdge.push_back(tmpLine.CreatePointOnDistance(offsetDistance));
	}

	
	if (IsHole == false)
	{
		//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Bottom surface (Mesh)

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

		//	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	---	--- Top surface (Mesh)

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

	}
	
	//	...	||

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

	tmpLine.Point = pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1];
	tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);

	// First triangle
	Mesh.points.push_back(pointsBottomCircleEdge[pointsBottomCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsTopCircleEdge[pointsBottomCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsBottomCircleEdge[0]);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

	// Second triangle
	Mesh.points.push_back(pointsTopCircleEdge[pointsBottomCircleEdge.size() - 1]);
	Mesh.points.push_back(pointsTopCircleEdge[0]);
	Mesh.points.push_back(pointsBottomCircleEdge[0]);

	tmpPlane = PlaneGeometric(Mesh.points[Mesh.points.size() - 1], Mesh.points[Mesh.points.size() - 2], Mesh.points[Mesh.points.size() - 3]);
	Mesh.vectorsNormal.push_back(tmpPlane.Line.Vector);

	
	pointsNNquarter.clear();
	pointsNPquarter.clear();
	pointsPNquarter.clear();
}
// ---																										//	---	Intersections ---

// ---																										// IntersectionTwoCone
int	ConeApprox::IntersectionTwoCone(ConeApprox cone2, PointGeometric* point1) {
	return Line.LineBetweenLine(cone2.Line, point1);
}
// ---																										// IntersectionConeAndPlane
int	ConeApprox::IntersectionConeAndPlane(RectangleApprox Plane, CircleGeometric* Ñircle) {
	PlaneGeometric plane;
	plane.Line = Plane.Line;
	LineGeometric line = Line;
	PointGeometric point;
	int Res;
	Res = plane.PlaneBetweenLine(line, &point);
	if (Res == 0) {
		int Res2;
		Res2 = plane.PlaneAngleLine(line);
		if (Res2 == 0) {
			plane.Line.Vector.Normalize();
			PointGeometric IntersectionPoint;
			IntersectionPoint = Line.PointProjection(Plane.Line.Point);
			PointGeometric PointBotton;
			PointGeometric pointC = plane.PointProjection(Line.Point);
			double HeightInter;
			HeightInter = Height - IntersectionPoint.DistanceToPoint(PointBottomSurfaceCenter);
			double rd;
			rd = HeightInter * tan(Angle * PI_Approx / 180);
			Ñircle->Radius = rd + RadiusSmaller;

			Ñircle->Line.Vector = plane.Line.Vector;

			Ñircle->Line.Point = pointC;
			return 0;
		}
	}

	return 1;
}
