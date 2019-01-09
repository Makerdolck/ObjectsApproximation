#pragma once

#ifndef __CYLINDER_APPROX_H__
#define __CYLINDER_APPROX_H__

#include "GeomObjectApprox.h"


class CylinderApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	PointGeometric	PointBottomEdge;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CylinderApprox();
	~CylinderApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __CYLINDER_APPROX_H__ */ 