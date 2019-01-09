#pragma once

#ifndef __POINT_APPROX_H__
#define __POINT_APPROX_H__

#include "GeomObjectApprox.h"


class PointApprox :	public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PointGeometric		Point;

public:
	double				X,
						Y,
						Z;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	PointApprox();
	~PointApprox();

	PointApprox&	operator=(PointGeometric point);					// PointApprox = PointGeometric

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
};

#endif /* __POINT_APPROX_H__ */ 