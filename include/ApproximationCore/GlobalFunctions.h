#pragma once

#ifndef __GLOBAL_FUNCTIONS_H__
#define __GLOBAL_FUNCTIONS_H__

#include "PointGeometric.h"

double			RoundingOf(	double	value, 
							int		accuracy);	

void			GaussMethod(double	**coefficients, 
							double	*freeCoefficients, 
							int		dimension, 
							double	*result);

void			KramerMethod(	double** gaussCoefficients,
								double* gaussFreeCoefficients,
								double* gaussResult,
								int D);

PointGeometric	TransferPointToNewCoordinateSystem(	PointGeometric	point, 
													PointGeometric	pointCenterNewCoordinateSystem,
													VectorGeometric	vectorX,
													VectorGeometric	vectorY,
													VectorGeometric vectorZ);

#endif /* __GLOBAL_FUNCTIONS_H__ */