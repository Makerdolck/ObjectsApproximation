#pragma once

#ifndef __POINT_APPROX_H__
#define __POINT_APPROX_H__

#include "GeomObjectApprox.h"


class PointApprox :	public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	double				X,
						Y,
						Z;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	PointApprox(double x = 0, double y = 0, double z = 0);
	~PointApprox();

	PointApprox&	operator=(PointGeometric point);					// PointApprox = PointGeometric
	operator		PointGeometric ();									// Conversation operator to PointGeometric
	
	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

	PointGeometric	PointBetweenPointMiddle(PointApprox point);
};

#endif /* __POINT_APPROX_H__ */