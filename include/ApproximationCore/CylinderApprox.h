#pragma once

#ifndef __CYLINDER_APPROX_H__
#define __CYLINDER_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneApprox.h"

#include "RectangleApprox.h"
#include "LineSegmentApprox.h"
#include "ConeApprox.h"

class CylinderApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	bool						IsHole;
	PointGeometric				PointBottomSurfaceCenter,
								PointTopSurfaceCenter;

	std::vector<PointGeometric> pointsTopCircleEdge;
	std::vector<PointGeometric> pointsBottomCircleEdge;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CylinderApprox();
	~CylinderApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double inAccuracy = 1.0f);

	int				IntersectionCylinderAndPlane(RectangleApprox Plane, CircleGeometric* Circle);
	int				IntersectionCylinderAndLine(LineSegmentApprox Lline, PointGeometric* point1, PointGeometric* point2);
	int				IntersectionCylinderAndCircle(CircleApprox circle, PointGeometric* point1, PointGeometric* point2);
	int				IntersectionTwoCylinder(CylinderApprox cylinder2, PointGeometric* point1);
	int				IntersectionCylinderCone(ConeApprox cone, PointGeometric* point1);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __CYLINDER_APPROX_H__ */