#pragma once

#ifndef __SPHERE_APPROX_H__
#define __SPHERE_APPROX_H__

#include "GeomObjectApprox.h"

class RectangleApprox;
#include "RectangleApprox.h"
#include "CircleGeometric.h"
#include "LineSegmentApprox.h"

class SphereApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	bool			IsHole;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	SphereApprox();
	~SphereApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double inAccuracy = 1.0f);

	int				IntersectionSphereAndPlane(RectangleApprox Plane, CircleGeometric* Circle);
	int				IntersectionSphereAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2);
	int				IntersectionSphereAndCircle(CircleApprox Circle, PointGeometric* point1, PointGeometric* point2);
	PointGeometric	PointIntersection(PointApprox pointOut);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __SPHERE_APPROX_H__ */ 