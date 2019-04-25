#pragma once

#ifndef __VECTOR_GEOMETRIC_H__
#define __VECTOR_GEOMETRIC_H__

class PointGeometric;
#include "PointGeometric.h"

class VectorGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	double X, Y, Z;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	VectorGeometric(double x = 0.0f, double y = 0.0f, double z = 0.0f, bool flagNormalize = true);
	VectorGeometric(PointGeometric pointStart, PointGeometric pointEnd, bool flagNormalize = true);
	~VectorGeometric();

	void Normalize();

	PointGeometric	PointProjection(PointGeometric pointOut, PointGeometric pointOn);
	double			DistanceToPoint(PointGeometric pointOut, PointGeometric pointOn);

	VectorGeometric	operator^(VectorGeometric vector);		// Vector	Product
	double			operator*(VectorGeometric vector);		// Scalar	Product
	VectorGeometric	operator+(VectorGeometric vector);		// Addition
	VectorGeometric	operator-(VectorGeometric vector);		// Subtraction

	VectorGeometric	operator-();							// - Vector

	void			operator=(PointGeometric point);		// Vector = Point
	double			operator*(PointGeometric point);		// Vector * Point (Scalar Product)

	VectorGeometric	operator*(double coef);					// Vector * coefficient
};


#endif /* __VECTOR_GEOMETRIC_H__ */