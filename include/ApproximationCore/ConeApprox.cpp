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
		tmpLine.Vector = VectorGeometric(tmpLine.Point, tmpPoint);		// Что-то тут не  так

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
int	ConeApprox::IntersectionConeAndPlane(RectangleApprox Plane, CircleGeometric* Сircle, PointGeometric* point) {
	PlaneGeometric plane;
	plane.Line = Plane.Line;
	LineGeometric line = Line;
	PointGeometric point1;

	int Res;
	Res = plane.PlaneBetweenLine(line, &point1);
	if (Res == 0) {
		int Res2;
		Res2 = plane.PlaneAngleLine(line);
		if (Res2 == 1) {
			double Vertex;
			Vertex = Radius / tan(Angle* PI_Approx / 180);
			PointGeometric pointVertex = Line.CreatePointOnDistance(Vertex - Height / 2, true);
			PointGeometric pointProjectionLine = Line.PointProjection(Plane.Line.Point);
			VectorGeometric vectorVertexAPlane(pointVertex, pointProjectionLine);
			VectorGeometric vectorVertexACenter(pointVertex, Line.Point);
			double scalar = vectorVertexAPlane * vectorVertexACenter;
			if (scalar > 0) {
				if (fabs(pointVertex.X - pointProjectionLine.X) < 0.001, fabs(pointVertex.Y - pointProjectionLine.Y) < 0.001, fabs(pointVertex.Z - pointProjectionLine.Z) < 0.001) {
					point->X = pointVertex.X;
					point->Y = pointVertex.Y;
					point->Z = pointVertex.Z;
					return 2;
				}
				else {
					double HeightInter;
					double rd;
					HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
					PointGeometric pointC = plane.PointProjection(Line.Point);
					rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
					Сircle->Radius = rd;
					Сircle->Line.Vector = plane.Line.Vector;
					Сircle->Line.Point = pointC;
					return 0;
				}
			}
		}
	}
	else { return 1; }
}
// ---																										// IntersectionConeAndPoint
int  ConeApprox::IntersectionConeAndPoint(PointApprox pointOut, PointGeometric* point1) {
	PointGeometric point(pointOut.X, pointOut.Y, pointOut.Z);
	double Vertex;
	Vertex = Radius / tan(Angle* PI_Approx / 180);
	PointGeometric pointVertex = Line.CreatePointOnDistance(Vertex - Height / 2, true);
	PointGeometric pointProjectionLine = Line.PointProjection(point);
	VectorGeometric vectorVertexAPlane(pointVertex, pointProjectionLine);
	VectorGeometric vectorVertexACenter(pointVertex, Line.Point);
	double scalar = vectorVertexAPlane * vectorVertexACenter;
	if (scalar > 0) {
		double HeightInter;
		double rd;
		HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
		rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
		VectorGeometric vectorNewLine(pointProjectionLine, point);
		LineGeometric line1(pointProjectionLine, vectorNewLine);
		PointGeometric PointRes = line1.CreatePointOnDistance(rd);
		point1->X = PointRes.X;
		point1->Y = PointRes.Y;
		point1->Z = PointRes.Z;
		return 0;
	}
	else {
		return 1;
	}
}
// ---																										// IntersectionConeAndCircle
int	ConeApprox::IntersectionConeAndCircle(CircleApprox circle, PointGeometric* point1, PointGeometric* point2) {
	PlaneGeometric tmpPlane = circle.Line;
	VectorGeometric VectorX, VectorY, VectorZ;
	VectorX = VectorGeometric(circle.Line.Point, PointGeometric(circle.Line.Point.X + 10, circle.Line.Point.Y + 10, circle.Line.Point.Z + 10));
	VectorX = tmpPlane.VectorProjection(VectorX);
	VectorZ = circle.Line.Vector;
	VectorY = VectorX ^ VectorZ;
	PointGeometric	tmpPoint;
	tmpPoint = circle.Line.Point;
	PointGeometric PointAp;
	PointGeometric PointAp1;//
	PointGeometric PointAp2;//
	double	/*angle,*/ angelsSum = 0, xCompon, yCompon, Function;
	xCompon = cos(angelsSum * PI_Approx / 180.0f) * circle.Radius; 		// X component
	yCompon = sin(angelsSum * PI_Approx / 180.0f) * circle.Radius;			// Y component		
	PointGeometric Point(xCompon, yCompon, 0);

	PointAp = TransferPointToNewCoordinateSystem(Point,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	// находим расстояние до вершины
	double Vertex;
	Vertex = Radius / tan(Angle* PI_Approx / 180);
	//проекция точки с окружности на ось конуса
	PointGeometric pointProjectionLine = Line.PointProjection(PointAp);
	//точка на вершине
	PointGeometric pointVertex = Line.CreatePointOnDistance(Vertex - Height / 2, true);
	//нахождение радиуса
	double HeightInter;
	double rd;
	HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
	rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
	//

	Function = pow(rd - Line.DistanceToPoint(PointAp), 2);
	double	deviationSum, deviationSum1, deviationSum2,
		deviationSumOld;
	deviationSum = Function;
	double	step = 0.5;
	bool	negativeDirection = false;
	int n = 0, k = 0;
	double angle1 = 0, angle2 = 0;
	do
	{
		deviationSumOld = deviationSum;
		angelsSum += (1 + (-2) * (int)negativeDirection) * step;
		xCompon = cos(angelsSum * PI_Approx / 180.0f) * circle.Radius; 		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f) * circle.Radius;			// Y component		
		PointGeometric Point(xCompon, yCompon, 0);

		PointAp = TransferPointToNewCoordinateSystem(Point,
			tmpPoint,
			VectorX,
			VectorY,
			VectorZ);
		//перерасчет радиуса по положению точки
		pointProjectionLine = Line.PointProjection(PointAp);
		HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
		rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
		//
		deviationSum = pow(rd - Line.DistanceToPoint(PointAp), 2);
		if (deviationSum > deviationSumOld&& n == 0)
		{
			//PointAp1 = PointAp;
			deviationSum1 = deviationSum;
			n += 1;
			angle1 = angelsSum;
		}
		if (deviationSum < deviationSumOld&& k == 0 && n == 1)
		{
			//PointAp2 = PointAp;
			deviationSum2 = deviationSum;
			angle2 = angelsSum;
			k += 1;
		}

	} while (angelsSum != 360) /*(angle2 != angelsSum)*/;

	angelsSum = angle1;
	deviationSum = deviationSum1;
	do
	{
		deviationSumOld = deviationSum;
		angelsSum += (1 + (-2) * (int)negativeDirection) * step;
		xCompon = cos(angelsSum * PI_Approx / 180.0f) * circle.Radius; 		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f) * circle.Radius;			// Y component		
		PointGeometric Point(xCompon, yCompon, 0);



		PointAp1 = TransferPointToNewCoordinateSystem(Point,
			tmpPoint,
			VectorX,
			VectorY,
			VectorZ);
		//перерасчет радиуса по положению точки
		pointProjectionLine = Line.PointProjection(PointAp1);
		HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
		rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
		//
		deviationSum = pow(rd - Line.DistanceToPoint(PointAp1), 2);
		if (deviationSum > deviationSumOld)
		{
			negativeDirection = !negativeDirection;
			step /= 2;
		}
	} while (fabs(deviationSumOld - deviationSum) > 0.0001);
	angelsSum = angle2;
	deviationSum = deviationSum2;
	do
	{
		deviationSumOld = deviationSum;
		angelsSum += (1 + (-2) * (int)negativeDirection) * step;
		xCompon = cos(angelsSum * PI_Approx / 180.0f) * circle.Radius; 		// X component
		yCompon = sin(angelsSum * PI_Approx / 180.0f) * circle.Radius;			// Y component		
		PointGeometric Point(xCompon, yCompon, 0);

		PointAp2 = TransferPointToNewCoordinateSystem(Point,
			tmpPoint,
			VectorX,
			VectorY,
			VectorZ);
		//перерасчет радиуса по положению точки
		pointProjectionLine = Line.PointProjection(PointAp2);
		HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
		rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
		//
		deviationSum = pow(rd - Line.DistanceToPoint(PointAp2), 2);
		if (deviationSum > deviationSumOld)
		{
			negativeDirection = !negativeDirection;
			step /= 2;
		}
	} while (fabs(deviationSumOld - deviationSum) > 0.0001);
	double l1, l2;

	//перерасчет радиуса по положению точки
	pointProjectionLine = Line.PointProjection(PointAp1);
	HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
	rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
	//
	l1 = pow(RoundingOf(rd, 3) - RoundingOf(Line.DistanceToPoint(PointAp1), 3), 2);
	//перерасчет радиуса по положению точки
	pointProjectionLine = Line.PointProjection(PointAp2);
	HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
	rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
	//
	l2 = pow(RoundingOf(rd, 3) - RoundingOf(Line.DistanceToPoint(PointAp2), 3), 2);

	if (fabs(l1) < 0.002 && fabs(l2) < 0.0002) {
		point1->X = PointAp1.X;
		point1->Y = PointAp1.Y;
		point1->Z = PointAp1.Z;

		point2->X = PointAp2.X;
		point2->Y = PointAp2.Y;
		point2->Z = PointAp2.Z;
		return 2;
	}
	else if (fabs(l2) < 0.0002) {
		point2->X = PointAp2.X;
		point2->Y = PointAp2.Y;
		point2->Z = PointAp2.Z;
		return 1;
	}
	else if (fabs(l1) < 0.0002) {
		point1->X = PointAp1.X;
		point1->Y = PointAp1.Y;
		point1->Z = PointAp1.Z;
		return 1;
	}
	return 0;
}
// ---																										// IntersectionConeAndLine
int	ConeApprox::IntersectionConeAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2) {
	//Vertex
	double Vertex;
	Vertex = Radius / tan(Angle* PI_Approx / 180);
	PointGeometric pointVertex = Line.CreatePointOnDistance(Vertex - Height / 2, true);
	//Создание удаленной точки на линиии
	PointGeometric pointStep;
	pointStep = Lline.Line.CreatePointOnDistance(200, false);
	//проекция точки с окружности на ось конуса
	PointGeometric pointProjectionLine = Line.PointProjection(pointStep);
	//
	LineGeometric line1;
	line1.Point = pointStep;
	line1.Vector = Lline.Line.Vector;
	//нахождение радиуса
	double HeightInter;
	double rd;
	HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
	rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
	//

	double step = 1, step1 = 0, step2 = 0;
	bool	negativeDirection = false;
	double Function = pow(rd - Line.DistanceToPoint(pointStep), 2);
	double	deviationSum, deviationSumOld;
	deviationSum = Function;
	double n = 0, k = 0;
	PointGeometric PointAp1;//
	PointGeometric PointAp2;//
	do
	{
		step += 1;
		deviationSumOld = deviationSum;
		pointStep = line1.CreatePointOnDistance(step);
		//перерасчет радиуса по положению точки
		pointProjectionLine = Line.PointProjection(pointStep);
		HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
		rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
		//
		deviationSum = pow(rd - Line.DistanceToPoint(pointStep), 2);
		if (deviationSum > deviationSumOld&& n == 0)
		{
			PointAp1 = pointStep;
			step1 = step;
			n += 1;
		}
		if (deviationSum < deviationSumOld&& k == 0 && n == 1)
		{
			PointAp2 = pointStep;
			step2 = step;
			k += 1;
		}

	} while (step != 400 || (k == 1 && n == 1));

	if (n != 0) {
		PointGeometric pointCentrLine; LineGeometric line2; PointGeometric pointStep1;
		if (k != 0) {
			pointCentrLine = PointAp1.PointBetween(PointAp2);
			line2.Point = pointCentrLine;
			line2.Vector = Lline.Line.Vector;
			pointStep1 = pointCentrLine;
		}
		else {
			pointStep1 = PointAp1;
			line2.Point = PointAp1;
			line2.Vector = Lline.Line.Vector;
		}
		double step0 = 5.0;
		deviationSum = pow(rd - Line.DistanceToPoint(pointStep1), 2);
		double st = 0;
		do
		{
			deviationSumOld = deviationSum;
			st += (1 + (-2) * (int)negativeDirection)*step0;
			pointStep1 = line2.CreatePointOnDistance(st);
			//перерасчет радиуса по положению точки
			pointProjectionLine = Line.PointProjection(pointStep1);
			HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
			rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
			//
			deviationSum = pow(rd - Line.DistanceToPoint(pointStep1), 2);
			if (deviationSum > deviationSumOld)
			{
				negativeDirection = !negativeDirection;
				step0 /= 2;
			}
		} while (fabs(deviationSumOld - deviationSum) > 0.0001);
		PointAp1 = pointStep1;
		if (k != 0) {
			PointGeometric pointStep2 = pointCentrLine;
			negativeDirection = false;
			double step3 = 20;
			deviationSum = pow(rd - Line.DistanceToPoint(pointStep2), 2);
			st = 0;
			do
			{
				deviationSumOld = deviationSum;
				st += (1 + (-2) * (int)negativeDirection) * step3;
				pointStep2 = line2.CreatePointOnDistance(st);
				//перерасчет радиуса по положению точки
				pointProjectionLine = Line.PointProjection(pointStep2);
				HeightInter = pointProjectionLine.DistanceToPoint(pointVertex);
				rd = HeightInter / tan((90 - Angle) * PI_Approx / 180);
				//
				deviationSum = pow(rd - Line.DistanceToPoint(pointStep2), 2);
				if (deviationSum > deviationSumOld)
				{
					negativeDirection = !negativeDirection;
					step3 /= 2;
				}
			} while (fabs(deviationSumOld - deviationSum) > 0.0001);
			PointAp2 = pointStep2;

			point1->X = PointAp1.X;
			point1->Y = PointAp1.Y;
			point1->Z = PointAp1.Z;

			point2->X = PointAp2.X;
			point2->Y = PointAp2.Y;
			point2->Z = PointAp2.Z;
			return 2;
		}
		point1->X = PointAp1.X;
		point1->Y = PointAp1.Y;
		point1->Z = PointAp1.Z;
		return 1;
	}
	return 0;
}