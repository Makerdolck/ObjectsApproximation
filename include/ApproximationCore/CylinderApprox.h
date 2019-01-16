#pragma once

#ifndef __CYLINDER_APPROX_H__
#define __CYLINDER_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneApprox.h"

class CylinderApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	PointGeometric				PointBottomSurfaceCenter;

	std::vector<PointGeometric> pointsTopCircleEdge_Copy;
	std::vector<PointGeometric> pointsBottomCircleEdge_Copy;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CylinderApprox();
	~CylinderApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double stepSize = 1.0f);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __CYLINDER_APPROX_H__ */ 