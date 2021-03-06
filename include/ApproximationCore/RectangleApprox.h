#pragma once

#ifndef __RECTANGLE_APPROX_H__
#define __RECTANGLE_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneApprox.h"

class CircleApprox;
class PointApprox;
#include "CircleApprox.h"
#include "LineSegmentApprox.h"

class RectangleApprox :	public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PlaneApprox		Plane;

	PointGeometric	WanderingCenter,
					*WanderingPoints;

public:
	VectorGeometric	VectorY,
					VectorX;

	double			Width,
					maxX,
					minX,
					maxY,
					minY;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	RectangleApprox();
	~RectangleApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double inAccuracy = 1.0f);

	int				PlaneIntersectionPlane(RectangleApprox Plane2, LineGeometric* line);
	int				PlaneBetweenLine(LineGeometric line1, PointGeometric* point1);
	int				PlaneIntersectionCircle(CircleApprox Circle, PointGeometric* point1, PointGeometric* point2);
	PointGeometric	PointBetweenPlaneMiddle(PointApprox point);
	PointGeometric	PointBetweenPlane(PointApprox point);
	int				PlaneProjectionLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2);
	PlaneGeometric  MiddlePlane(RectangleApprox Plane2);

private:
	void			FindWidthHeightMinMaxXY(PointGeometric *points, int arraySize);
	void			FindPointsCoordinates(PointGeometric *points, int arraySize);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __RECTANGLE_APPROX_H__ */