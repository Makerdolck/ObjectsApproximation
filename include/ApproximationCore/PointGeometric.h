#pragma once

#ifndef __POINT_GEOMETRIC_H__
#define __POINT_GEOMETRIC_H__

#define PI_Approx 3.1415926535897932384626433832795

#include <math.h>

class VectorGeometric;
#include "VectorGeometric.h"

class PointGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	double X, Y, Z;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	PointGeometric(double x = 0.0f, double y = 0.0f, double z = 0.0f);
	~PointGeometric();

	double			DistanceToPoint(PointGeometric point);

	PointGeometric	operator*(double coef);					// Point * coefficient
	PointGeometric	operator/(double coef);					// Point / coefficient

	PointGeometric	operator-(PointGeometric point);		// Point - Point		()
	PointGeometric	operator+(PointGeometric point);		// Point + Point		()

	PointGeometric	operator+(VectorGeometric vector);		// Point + Vector		(displacement by direction of vector)
	PointGeometric	operator-(VectorGeometric vector);		// Point - Vector		(displacement against direction of vector)
};

#endif /* __POINT_GEOMETRIC_H__ */ 