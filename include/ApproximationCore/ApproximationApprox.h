#pragma once

#ifndef __APPROXIMATION_APPROX_H__
#define __APPROXIMATION_APPROX_H__

//#include <afxtempl.h>
#include <math.h>
#include <vector>
#include <time.h>

#include "PointGeometric.h"
#include "VectorGeometric.h"

////////////////////////////////		Approximation

class ApproximationApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	
//	--- --- --- --- --- --- --- --- ---	// Methods
protected:
	virtual double	FunctionApprox(PointGeometric *points, int arraySize);
	void			Approximation(PointGeometric *points, int arraySize, double accuracy, VectorGeometric *vector, double *value);
};

#endif /* __APPROXIMATION_APPROX_H__ */