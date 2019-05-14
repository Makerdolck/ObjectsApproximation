#pragma once

#ifndef __CONE_APPROX_H__
#define __CONE_APPROX_H__

#include "GeomObjectApprox.h"
#include "CircleGeometric.h"

#include "RectangleApprox.h"

class ConeApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	PointGeometric	PointBottomSurfaceCenter;
	PointGeometric	PointTopSurfaceCenter;

	bool			IsHole;

	double			Angle,
					RadiusSmaller,
					HeightPhantom;

	std::vector<PointGeometric> pointsTopCircleEdge;
	std::vector<PointGeometric> pointsBottomCircleEdge;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ConeApprox();
	~ConeApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double inAccuracy = 1.0f);

	int				IntersectionTwoCone(ConeApprox cone2, PointGeometric* point1);
	int				IntersectionConeAndPlane(RectangleApprox Plane, CircleGeometric* Ñircle);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);

	void			FindPointForBottomCenter(PointGeometric *points, int arraySize);

};

#endif /* __CONE_APPROX_H__ */ 