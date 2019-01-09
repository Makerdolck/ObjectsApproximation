#pragma once

#ifndef __SPHERE_APPROX_H__
#define __SPHERE_APPROX_H__

#include "GeomObjectApprox.h"


class SphereApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:


//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	SphereApprox();
	~SphereApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __SPHERE_APPROX_H__ */ 