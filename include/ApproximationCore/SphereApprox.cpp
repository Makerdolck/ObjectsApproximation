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

	double	angle = 2 * acos((Radius - inAccuracy) / Radius) * 180.f / PI_Approx,
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
// ---																										//	---	Intersections ---

// ---																										// IntersectionSphereAndPlane
int	SphereApprox::IntersectionSphereAndPlane(RectangleApprox Plane, CircleGeometric* Circle) {
	double D;
	D = (-1) * (Plane.Line.Vector.X * Plane.Line.Point.X + Plane.Line.Vector.Y * Plane.Line.Point.Y + Plane.Line.Vector.Z * Plane.Line.Point.Z);
	double t;
	t = (-1) * ((D + Plane.Line.Vector.X * Line.Point.X + Plane.Line.Vector.Y * Line.Point.Y + Plane.Line.Vector.Z * Line.Point.Z) / (pow(Plane.Line.Vector.X, 2) + pow(Plane.Line.Vector.Y, 2) + pow(Plane.Line.Vector.Z, 2)));
	Circle->Line.Point.X = Line.Point.X + t * Plane.Line.Vector.X;
	Circle->Line.Point.Y = Line.Point.Y + t * Plane.Line.Vector.Y;
	Circle->Line.Point.Z = Line.Point.Z + t * Plane.Line.Vector.Z;
	double dist;
	dist = sqrt(pow(Line.Point.X - Circle->Line.Point.X, 2) + pow(Line.Point.Y - Circle->Line.Point.Y, 2) + pow(Line.Point.Z - Circle->Line.Point.Z, 2));
	/*Circle->Radius =sqrt( pow(Radius,2) - pow(dist,2));
	Circle->Line.Vector = Plane.Line.Vector;*/
	if (Radius > dist) {
		Circle->Radius = sqrt(pow(Radius, 2) - pow(dist, 2));
		Circle->Line.Vector = Plane.Line.Vector;
		return 0;
	}
	else if (Radius < dist) {
		return 1;
	}
	else {
		return 2;
	}


}
// ---																										// IntersectionSphereAndLine
int	SphereApprox::IntersectionSphereAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2) {
	double a;
	double b;
	double c;
	double diskr, kor;
	double t1, t2;
	a = pow(Lline.Line.Vector.X, 2) + pow(Lline.Line.Vector.Y, 2) + pow(Lline.Line.Vector.Z, 2);
	b = (-2) * (Lline.Line.Vector.X * (Line.Point.X - Lline.Line.Point.X) + Lline.Line.Vector.Y * (Line.Point.Y - Lline.Line.Point.Y) + Lline.Line.Vector.Z * (Line.Point.Z - Lline.Line.Point.Z));
	c = pow(Line.Point.X - Lline.Line.Point.X, 2) + pow(Line.Point.Y - Lline.Line.Point.Y, 2) + pow(Line.Point.Z - Lline.Line.Point.Z, 2) - pow(Radius, 2);
	diskr = b * b - 4 * a * c;
	kor = sqrt(diskr);
	if (kor > 0) {
		t1 = ((-1) * b + kor) / (2 * a);
		t2 = ((-1) * b - kor) / (2 * a);
		point1->X = Lline.Line.Point.X + t1 * Lline.Line.Vector.X;
		point1->Y = Lline.Line.Point.Y + t1 * Lline.Line.Vector.Y;
		point1->Z = Lline.Line.Point.Z + t1 * Lline.Line.Vector.Z;

		point2->X = Lline.Line.Point.X + t2 * Lline.Line.Vector.X;
		point2->Y = Lline.Line.Point.Y + t2 * Lline.Line.Vector.Y;
		point2->Z = Lline.Line.Point.Z + t2 * Lline.Line.Vector.Z;

		return 0;
	}
	else if (diskr == 0) {
		return 2;
	}
	else {
		return 1;
	}
}
// ---																										// IntersectionSphereAndCircle
int	SphereApprox::IntersectionSphereAndCircle(CircleApprox Circle, PointGeometric* point1, PointGeometric* point2) {
	double D;
	PlaneGeometric Plane(Circle.Line);
	CircleGeometric Circle1;
	CircleGeometric Circle2(Circle.Line, Circle.Radius);

	D = (-1) * (Plane.Line.Vector.X * Plane.Line.Point.X + Plane.Line.Vector.Y * Plane.Line.Point.Y + Plane.Line.Vector.Z * Plane.Line.Point.Z);
	double t;
	t = (-1) * ((D + Plane.Line.Vector.X * Line.Point.X + Plane.Line.Vector.Y * Line.Point.Y + Plane.Line.Vector.Z * Line.Point.Z) / (pow(Plane.Line.Vector.X, 2) + pow(Plane.Line.Vector.Y, 2) + pow(Plane.Line.Vector.Z, 2)));
	Circle1.Line.Point.X = Line.Point.X + t * Plane.Line.Vector.X;
	Circle1.Line.Point.Y = Line.Point.Y + t * Plane.Line.Vector.Y;
	Circle1.Line.Point.Z = Line.Point.Z + t * Plane.Line.Vector.Z;
	double dist;
	dist = sqrt(pow(Line.Point.X - Circle1.Line.Point.X, 2) + pow(Line.Point.Y - Circle1.Line.Point.Y, 2) + pow(Line.Point.Z - Circle1.Line.Point.Z, 2));
	if (Radius > dist) {
		Circle1.Radius = sqrt(pow(Radius, 2) - pow(dist, 2));
		Circle1.Line.Vector = Plane.Line.Vector;
		int Res;
		Res = Circle2.CircleIntersection(Circle1, point1, point2);
		return 0;
	}
	else if (Radius < dist) {
		return 1;
	}

	return 1;
}
// ---																										// PointIntersection
PointGeometric SphereApprox::PointIntersection(PointApprox pointOut) {
	PointGeometric PointSp(Line.Point);
	PointGeometric point(pointOut.X, pointOut.Y, pointOut.Z);
	VectorGeometric vectorNewLine(pointOut.X - Line.Point.X, pointOut.Y - Line.Point.Y, pointOut.Z - Line.Point.Z);
	LineGeometric line1(PointSp, vectorNewLine);
	return line1.CreatePointOnDistance(Radius);
}
