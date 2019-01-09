#pragma once

#ifndef __PLANE_APPROX_H__
#define __PLANE_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneGeometric.h"
#include "VectorGeometric.h"

class PlaneApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PlaneGeometric	Plane;

public:
	

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	PlaneApprox();
	~PlaneApprox();

	PointGeometric	PointProjection(PointGeometric pointOut);
	VectorGeometric VectorProjection(VectorGeometric vectorOut, bool flagNormalize = true);

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __RECTANGLE_APPROX_H__ */ 