#pragma once

#ifndef __GLOBAL_FUNCTIONS_H__
#define __GLOBAL_FUNCTIONS_H__

#include <math.h>

double	RoundingOf(double value, int accuracy);			
void	GaussMethod(double **coefficients, double *freeCoefficients, int dimension, double *result);

#endif /* __GLOBAL_FUNCTIONS_H__ */