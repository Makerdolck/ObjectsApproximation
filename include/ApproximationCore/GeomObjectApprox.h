#pragma once

#ifndef __GEOM_OBJECT_APPROX_H__
#define __GEOM_OBJECT_APPROX_H__

#include "ApproximationApprox.h"
#include "LineGeometric.h"

#include "MeshTrianglesStructure.h"


class GeomObjectApprox : public ApproximationApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	std::vector<PointGeometric>		PointsForApprox;

	LineGeometric					Line;

	double							Radius,
									Height;

	MeshTriangles					Mesh;

protected:
	char							*objectApproxName;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	GeomObjectApprox();
	~GeomObjectApprox();

	char*			GetName();

	virtual void	Triangulation(double inAccuracy = 1.0f);

protected:
	virtual void	FindHeight(PointGeometric *points, int arraySize);	
	PointGeometric	CenterByPoints(PointGeometric *points, int arraySize);
};

#endif /* __GEOM_OBJECT_APPROX_H__ */ 