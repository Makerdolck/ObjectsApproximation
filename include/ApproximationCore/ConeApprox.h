#pragma once

#ifndef __CONE_APPROX_H__
#define __CONE_APPROX_H__

#include "GeomObjectApprox.h"
#include "CircleGeometric.h"

class ConeApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	PointGeometric	PointBottomSurfaceCenter;

public:
	double	Angle,
			RadiusSmaller,
			HeightPhantom;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ConeApprox();
	~ConeApprox();

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);
	void			Triangulation(double stepSize = 1.0f);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);

	void			FindPointForBottomCenter(PointGeometric *points, int arraySize);

};

#endif /* __CONE_APPROX_H__ */ 