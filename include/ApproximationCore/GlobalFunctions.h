#pragma once

#ifndef __GLOBAL_FUNCTIONS_H__
#define __GLOBAL_FUNCTIONS_H__

#include "PointGeometric.h"

#include <math.h>

double			RoundingOf(	double	value, 
							int		accuracy);	

void			GaussMethod(double	**coefficients, 
							double	*freeCoefficients, 
							int		dimension, 
							double	*result);

PointGeometric	TransferPointToNewCoordinateSystem(	PointGeometric	point, 
													PointGeometric	pointCenterNewCoordinateSystem,
													VectorGeometric	vectorX,
													VectorGeometric	vectorY,
													VectorGeometric vectorZ);

#endif /* __GLOBAL_FUNCTIONS_H__ */