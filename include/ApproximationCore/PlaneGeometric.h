#pragma once

#ifndef __PLANE_GEOMETRIC_H__
#define __PLANE_GEOMETRIC_H__

#include "LineGeometric.h"

class PlaneGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	LineGeometric Line;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	PlaneGeometric();
	PlaneGeometric(LineGeometric line);
	PlaneGeometric(VectorGeometric vector);
	PlaneGeometric(PointGeometric point, VectorGeometric vector);
	PlaneGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3);
	~PlaneGeometric();

	void			Normalize();

	PointGeometric	PointProjection(PointGeometric pointOut);
	VectorGeometric	VectorProjection(VectorGeometric vectorOut, bool flagNormalize = true);
};

#endif /* __PLANE_GEOMETRIC_H__ */ 