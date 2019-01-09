#include "stdafx.h"

#include "ConeApprox.h"


// ---																										// Constructors
ConeApprox::ConeApprox() { objectApproxName = (char*)"cone"; Angle = 45; RadiusSmaller = 0; }

ConeApprox::~ConeApprox(){}
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
		PointForBottomCenter = PointMin;
	else
		PointForBottomCenter = PointMax;

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
	PointBottomCenter = Line.PointProjection(PointForBottomCenter);

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

	PointGeometric pointsForFindCenter[2];
	pointsForFindCenter[0] = circleBottom.Point;
	pointsForFindCenter[1] = circleTop.Point;

	CenterByPoints(pointsForFindCenter, 2);

	///////////		Find primary Vector
	VectorGeometric	vector1(points[0], points[1]),
					vector2(points[0], points[2]);

	Line.Vector = vector1 ^ vector2;

	Line.Normalize();										// Vector Normalization
	
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

	double		Catheter = fabs(Line.DistanceToPoint(PointMax) - Line.DistanceToPoint(PointMin));

	Angle = atan(Height / Catheter) * 180 / PI_Approx;		//* 180.0 / PI_Approx

	/////////////		Find primary Radius
	
	FindPointForBottomCenter(points, arraySize);

	Radius	= Line.DistanceToPoint(PointForBottomCenter);

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

		//Approximation(points, accuracy, &Line.Vector.X);	// Changing X - vector
		//Approximation(points, accuracy, &Line.Vector.Y);	// Changing Y - vector
		//Approximation(points, accuracy, &Line.Vector.Z);	// Changing Z - vector

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

	RadiusSmaller = Radius - Height * tan(Angle * PI_Approx / 180);
	
	//
	HeightPhantom = Radius * pow(tan(Angle * PI_Approx / 180), -1);

	//
	Angle = 90 - Angle;
}
