#pragma once

#ifndef __RECTANGLE_APPROX_H__
#define __RECTANGLE_APPROX_H__

#include "GeomObjectApprox.h"
#include "PlaneApprox.h"

class RectangleApprox :	public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PlaneApprox		Plane;

	VectorGeometric VectorY,
					VectorZ;

	double			maxX,
					minX,
					maxY,
					minY;

	PointGeometric	WanderingCenter,
					*WanderingPoints;

public:
	VectorGeometric	VectorX;

	double			Width;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	RectangleApprox();
	~RectangleApprox();

private:
	void			FindWidthHeightMinMaxXY(PointGeometric *points, int arraySize);
	void			FindPointsCoordinates(PointGeometric *points, int arraySize);

public:
	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __RECTANGLE_APPROX_H__ */ 