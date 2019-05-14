#pragma once

#ifndef __CIRCLE_APPROX_H__
#define __CIRCLE_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneApprox.h"
#include "SphereApprox.h"

#include "PointApprox.h"

class CircleApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PlaneApprox		Plane;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CircleApprox();
	~CircleApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double inAccuracy = 1.0f);

	int				LineIntersection(LineGeometric line, PointGeometric* point1, PointGeometric* point2);
	int				CircleIntersection(CircleApprox Circle2, PointGeometric* point1, PointGeometric* point2);
	PointGeometric	PointIntersectionMiddle(PointApprox pointOut);
	PointGeometric	PointIntersection(PointApprox pointOut);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __CIRCLE_APPROX_H__ */ 