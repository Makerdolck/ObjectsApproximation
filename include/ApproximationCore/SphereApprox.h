#pragma once

#ifndef __SPHERE_APPROX_H__
#define __SPHERE_APPROX_H__

#include "GeomObjectApprox.h"

class RectangleApprox;
#include "RectangleApprox.h"
#include "CircleGeometric.h"
#include "LineSegmentApprox.h"
class CylinderApprox;
#include "CylinderApprox.h"
#include "ConeApprox.h"

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

	int				IntersectionSphereAndPlane(RectangleApprox Plane, CircleGeometric* Circle, PointGeometric* pointOut);
	int				IntersectionSphereAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2);
	int				IntersectionSphereAndCircle(CircleApprox Circle, PointGeometric* point1, PointGeometric* point2);
	PointGeometric	IntersectionSphereAndPoint(PointApprox pointOut);
	int				IntersectionTwoSphere(SphereApprox Sphere2, CircleGeometric* Circle, PointGeometric* pointTouch);
	PointGeometric	IntersectionSphereCylinder(CylinderApprox Cylinder);
	PointGeometric	IntersectionSphereCone(ConeApprox Cone);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __SPHERE_APPROX_H__ */