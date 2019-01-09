#pragma once

#ifndef __TORUS_APPROX_H__
#define __TORUS_APPROX_H__

#include "GeomObjectApprox.h"
#include "SphereApprox.h"
#include "PlaneGeometric.h"

class TorusApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	double RadiusCircle;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	TorusApprox();
	~TorusApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __TORUS_APPROX_H__ */ 