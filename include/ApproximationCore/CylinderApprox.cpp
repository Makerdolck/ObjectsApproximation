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

	double	angle		= 2 * acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
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
// ---																										//	---	Intersections ---

// ---																										// IntersectionCylinderAndPlane
int	CylinderApprox::IntersectionCylinderAndPlane(RectangleApprox Plane, CircleGeometric* Circle) {
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
			Circle->Line.Vector = plane.Line.Vector;
			Circle->Radius = Radius;
			PointGeometric pointC = plane.PointProjection(Line.Point);
			Circle->Line.Point = pointC;
			return 0;
		}
	}
	return 1;
}
// ---																										// IntersectionCylinderAndLine
int	CylinderApprox::IntersectionCylinderAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2) {
	PointGeometric pointC = Line.CreatePointOnDistance(Height / 2, false);
	CircleGeometric circle;
	circle.Line.Point = pointC;
	circle.Radius = Radius;
	circle.Line.Vector = Line.Vector;
	PlaneGeometric plane(circle.Line);
	PointGeometric point1p, point2p;
	PointGeometric pointE = plane.PointProjection(Lline.PointEnd);
	PointGeometric pointS = plane.PointProjection(Lline.PointStart);
	PointGeometric pointCr = pointE.PointBetween(pointS);
	VectorGeometric vectorNewLine(pointS, pointE);
	LineGeometric line1(pointCr, vectorNewLine);
	double angle, Phi;
	Phi = fabs(Line.Vector.X * Lline.Vector.X + Line.Vector.Y * Lline.Vector.Y + Line.Vector.Z * Lline.Vector.Z) / (sqrt(pow(Line.Vector.X, 2) + pow(Line.Vector.Y, 2) + pow(Line.Vector.Z, 2)) * sqrt(pow(Lline.Vector.X, 2) + pow(Lline.Vector.Y, 2) + pow(Lline.Vector.Z, 2)));
	angle = acos(Phi) * 180 / PI_Approx;
	if (angle > 15 && angle <= 90) {
		int Res;
		Res = circle.LineIntersectionCircle(line1, &point1p, &point2p);

		if (Res == 2) {
			LineGeometric lineOtv1(point1p, Line.Vector);
			LineGeometric lineOtv2(point2p, Line.Vector);
			LineGeometric lineLline;
			lineLline.Point = Lline.Point;
			lineLline.Vector = Lline.Vector;
			PointGeometric point1pp, point2pp;
			int Res2, Res3;
			Res3 = lineOtv2.LineBetweenLine(lineLline, &point2pp);
			Res2 = lineOtv1.LineBetweenLine(lineLline, &point1pp);
			point1->X = point1pp.X;
			point1->Y = point1pp.Y;
			point1->Z = point1pp.Z;

			point2->X = point2pp.X;
			point2->Y = point2pp.Y;
			point2->Z = point2pp.Z;
			return 2;
		}
		if (Res == 1) {
			LineGeometric lineOtv1(point1p, Line.Vector);
			LineGeometric lineLline;
			lineLline.Point = Lline.Point;
			lineLline.Vector = Lline.Vector;
			PointGeometric point1pp, point2pp;
			int Res2;
			Res2 = lineOtv1.LineBetweenLine(lineLline, &point1pp);

			point1->X = point1pp.X;
			point1->Y = point1pp.Y;
			point1->Z = point1pp.Z;
			return 1;
		}
	}
	return 0;
}
// ---																										// IntersectionTwoCylinder
int	CylinderApprox::IntersectionTwoCylinder(CylinderApprox cylinder2, PointGeometric* point1) {
	return Line.LineBetweenLine(cylinder2.Line, point1);
}
// ---																										// IntersectionCylinderCone
int	CylinderApprox::IntersectionCylinderCone(ConeApprox cone, PointGeometric* point1) {
	return Line.LineBetweenLine(cone.Line, point1);
}
// ---																										// IntersectionCylinderAndCircle
int	CylinderApprox::IntersectionCylinderAndCircle(CircleApprox circle, PointGeometric* point1, PointGeometric* point2) {
	PlaneGeometric tmpPlane = circle.Line;
	VectorGeometric VectorX, VectorY, VectorZ;
	VectorX = VectorGeometric(circle.Line.Point, PointGeometric(circle.Line.Point.X + 10, circle.Line.Point.Y + 10, circle.Line.Point.Z + 10));
	VectorX = tmpPlane.VectorProjection(VectorX);
	VectorZ = circle.Line.Vector;
	VectorY = VectorX ^ VectorZ;
	PointGeometric	tmpPoint;
	tmpPoint = circle.Line.Point;

	PointGeometric PointAp, PointAp1, PointAp2;

	double	angle = 0, angelsSum = 0, xCompon, yCompon, Function;
	xCompon = cos(angelsSum * PI_Approx / 180.0f) * circle.Radius; 		// X component
	yCompon = sin(angelsSum * PI_Approx / 180.0f) * circle.Radius;			// Y component		
	PointGeometric Point(xCompon, yCompon, 0);

	PointAp = TransferPointToNewCoordinateSystem(Point,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);

	Function = pow(Radius - Line.DistanceToPoint(PointAp), 2);
	double	deviationSum, deviationSumOld;
	deviationSum = Function;
	double	step = 0.5, angle1 = 0, angle2 = 0;
	bool	negativeDirection = false;
	int n = 0, k = 0;
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

		deviationSum = pow(Radius - Line.DistanceToPoint(PointAp), 2);
		if (deviationSum > deviationSumOld&& n == 0)
		{
			/*PointAp1 = PointAp;*/
			n += 1;
			angle1 = angelsSum;
		}
		if (deviationSum < deviationSumOld&& k == 0)
		{
			/*PointAp2 = PointAp;*/
			angle2 = angelsSum;
			k += 1;
		}

	} while (angelsSum != 360) /*(angle2 != angelsSum)*/;

	angelsSum = angle1;
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

		deviationSum = pow(Radius - Line.DistanceToPoint(PointAp1), 2);
		if (deviationSum > deviationSumOld)
		{
			negativeDirection = !negativeDirection;
			step /= 2;
		}
	} while (fabs(deviationSumOld - deviationSum) > 0.0001);

	angelsSum = angle2;
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

		deviationSum = pow(Radius - Line.DistanceToPoint(PointAp2), 2);
		if (deviationSum > deviationSumOld)
		{
			negativeDirection = !negativeDirection;
			step /= 2;
		}
	} while (fabs(deviationSumOld - deviationSum) > 0.0001);
	double l1, l2;

	l1 = pow(RoundingOf(Radius, 3) - RoundingOf(Line.DistanceToPoint(PointAp1), 3), 2);
	l2 = pow(RoundingOf(Radius, 3) - RoundingOf(Line.DistanceToPoint(PointAp2), 3), 2);
	if (fabs(l1) < 0.0003 && fabs(l2) < 0.0003) {
		point1->X = PointAp1.X;
		point1->Y = PointAp1.Y;
		point1->Z = PointAp1.Z;

		point2->X = PointAp2.X;
		point2->Y = PointAp2.Y;
		point2->Z = PointAp2.Z;
		return 2;
	}
	else if (fabs(l2) < 0.0003) {
		point2->X = PointAp2.X;
		point2->Y = PointAp2.Y;
		point2->Z = PointAp2.Z;
		return 1;
	}
	else if (fabs(l1) < 0.0003) {
		point1->X = PointAp1.X;
		point1->Y = PointAp1.Y;
		point1->Z = PointAp1.Z;
		return 1;
	}
	return 0;
}
// ---																										// IntersectionCylinderAndPoint
PointGeometric CylinderApprox::IntersectionCylinderAndPoint(PointApprox pointOut) {
	LineGeometric lineForProjection(Line);
	PointGeometric point(pointOut.X, pointOut.Y, pointOut.Z);
	PointGeometric pointProjection = lineForProjection.PointProjection(point);
	VectorGeometric vectorNewLine(pointProjection, point);
	LineGeometric line1(pointProjection, vectorNewLine);
	return line1.CreatePointOnDistance(Radius);
}

PlaneGeometric	CylinderApprox::MiddlePlaneCylinderAndCone(ConeApprox cone) {
	PlaneGeometric tmpPlane(cone.Line);
	VectorGeometric VectorX, VectorY, VectorZ;
	VectorX = VectorGeometric(cone.Line.Point, PointGeometric(cone.Line.Point.X + 10, cone.Line.Point.Y + 10, cone.Line.Point.Z + 10));
	VectorX = tmpPlane.VectorProjection(VectorX);
	VectorZ = cone.Line.Vector;
	VectorY = VectorX ^ VectorZ;
	PlaneGeometric PlaneCo1;
	PlaneCo1.Line.Point = cone.Line.Point;
	PlaneCo1.Line.Vector = VectorX;
	PlaneGeometric PlaneCo2;
	PlaneCo2.Line.Point = cone.Line.Point;
	PlaneCo2.Line.Vector = VectorY;

	PointGeometric pointNewPlaneM1;
	pointNewPlaneM1 = cone.Line.CreatePointOnDistance(cone.Height / 2, true);//где малый радиус
	LineGeometric LineM1(pointNewPlaneM1, PlaneCo1.Line.Vector);
	PointGeometric pointNewPlaneM2;
	pointNewPlaneM2 = cone.Line.CreatePointOnDistance(cone.Height / 2, false); // где большой радиус
	LineGeometric LineM2(pointNewPlaneM2, PlaneCo1.Line.Vector);

	LineGeometric LineM3(pointNewPlaneM1, PlaneCo2.Line.Vector);  // где малый радиус
	LineGeometric LineM4(pointNewPlaneM2, PlaneCo2.Line.Vector); // где большой радиус
	//1прямая
	PointGeometric pointLineC1 = LineM2.CreatePointOnDistance(cone.Radius + 0.9);
	PointGeometric pointLineC11 = LineM1.CreatePointOnDistance(cone.RadiusSmaller + 0.7);
	VectorGeometric VectorConeL1(pointLineC1, pointLineC11);
	LineGeometric	LineCone1(pointLineC1, VectorConeL1);
	//2прямая
	PointGeometric pointLineC2 = LineM2.CreatePointOnDistance(cone.Radius + 0.9, false);
	PointGeometric pointLineC22 = LineM1.CreatePointOnDistance(cone.RadiusSmaller + 0.9, false);
	VectorGeometric VectorConeL2(pointLineC2, pointLineC22);
	LineGeometric	LineCone2(pointLineC2, VectorConeL2);
	//3прямая
	PointGeometric pointLineC3 = LineM4.CreatePointOnDistance(cone.Radius + 0.9);
	PointGeometric pointLineC33 = LineM3.CreatePointOnDistance(cone.RadiusSmaller + 0.9);
	VectorGeometric VectorConeL3(pointLineC3, pointLineC33);
	LineGeometric	LineCone3(pointLineC3, VectorConeL3);

	PlaneGeometric PlaneCy;
	PointGeometric pointCy;
	VectorGeometric VM(Line.Point, cone.Line.Point);
	LineGeometric lineMid(Line.Point, VM);
	pointCy = lineMid.CreatePointOnDistance(Height / 2); //проверить точку
	PlaneCy.Line.Point = pointCy;
	PlaneCy.Line.Vector = Line.Vector;
	PointGeometric PointIntP1;
	int Res1 = PlaneCy.PlaneBetweenLine(LineCone1, &PointIntP1);
	PointGeometric PointIntP2;
	int Res2 = PlaneCy.PlaneBetweenLine(LineCone2, &PointIntP2);
	PointGeometric PointIntP3;
	int Res3 = PlaneCy.PlaneBetweenLine(LineCone3, &PointIntP3);
	VectorGeometric otv1(pointCy, PointIntP1);
	LineGeometric lineO1(pointCy, otv1);
	PointGeometric PointIntP11 = lineO1.CreatePointOnDistance(Radius);
	LineGeometric lineOtv1(PointIntP11, Line.Vector);

	VectorGeometric otv2(pointCy, PointIntP2);
	LineGeometric lineO2(pointCy, otv2);
	PointGeometric PointIntP21 = lineO2.CreatePointOnDistance(Radius);
	LineGeometric lineOtv2(PointIntP21, Line.Vector);

	VectorGeometric otv3(pointCy, PointIntP3);
	LineGeometric lineO3(pointCy, otv3);
	PointGeometric PointIntP31 = lineO3.CreatePointOnDistance(Radius);
	LineGeometric lineOtv3(PointIntP31, Line.Vector);

	PointGeometric point1pp;
	int Res11;
	Res11 = lineOtv1.LineBetweenLine(LineCone1, &point1pp);
	PointGeometric point2pp;
	int Res22;
	Res22 = lineOtv2.LineBetweenLine(LineCone2, &point2pp);
	PointGeometric point3pp;
	int Res13;
	Res13 = lineOtv3.LineBetweenLine(LineCone3, &point3pp);

	PlaneGeometric PlaneBetween(point1pp, point2pp, point3pp);
	PlaneBetween.Line.Vector.Normalize();
	PlaneBetween.Line.Vector.X = RoundingOf(PlaneBetween.Line.Vector.X, 3); PlaneBetween.Line.Vector.Y = RoundingOf(PlaneBetween.Line.Vector.Y, 3); PlaneBetween.Line.Vector.Z = RoundingOf(PlaneBetween.Line.Vector.Z, 3);
	PlaneBetween.Line.Point.X = RoundingOf(PlaneBetween.Line.Point.X, 3); PlaneBetween.Line.Point.Y = RoundingOf(PlaneBetween.Line.Point.Y, 3); PlaneBetween.Line.Point.Z = RoundingOf(PlaneBetween.Line.Point.Z, 3);
	return PlaneBetween;
}