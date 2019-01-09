#pragma once

#ifndef __GEOM_OBJECT_APPROX_H__
#define __GEOM_OBJECT_APPROX_H__

#include "ApproximationApprox.h"
#include "LineGeometric.h"


class GeomObjectApprox : public ApproximationApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	std::vector<PointGeometric>		PointsForApprox;

	LineGeometric					Line;

	double							Radius,
									Height;

protected:
	char							*objectApproxName;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	GeomObjectApprox();
	~GeomObjectApprox();

	char*			GetName();

protected:
	virtual void	FindHeight(PointGeometric *points, int arraySize);
	PointGeometric	CenterByPoints(PointGeometric *points, int arraySize);
};

#endif /* __GEOM_OBJECT_APPROX_H__ */ 