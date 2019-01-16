#pragma once

#ifndef __CONE_APPROX_H__
#define __CONE_APPROX_H__

#include "GeomObjectApprox.h"
#include "CircleGeometric.h"

class ConeApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PointGeometric	PointBottomSurfaceCenter;
	PointGeometric	PointTopSurfaceCenter;

public:
	double	Angle,
			RadiusSmaller,
			HeightPhantom;

	std::vector<PointGeometric> pointsTopCircleEdge_Copy;
	std::vector<PointGeometric> pointsBottomCircleEdge_Copy;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ConeApprox();
	~ConeApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double stepSize = 1.0f);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);

	void			FindPointForBottomCenter(PointGeometric *points, int arraySize);

};

#endif /* __CONE_APPROX_H__ */ 