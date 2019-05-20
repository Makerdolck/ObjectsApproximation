#include "stdafx.h"

#include "GlobalFunctions.h"

#include <iostream>
#include <math.h>


// ---																										// Rounding Of
double RoundingOf(double value, int accuracy)
{
	return ((double)int(value*pow(10, accuracy))) / pow(10, accuracy);
}
// ---																										// Gauss Method
void GaussMethod(double **coefficients, double *freeCoefficients, int dimension, double *result)
{
	double d, s;

	for (int k = 0; k < dimension; k++) // прямой ход
	{
		for (int j = k + 1; j < dimension; j++)
		{
			d = coefficients[j][k] / coefficients[k][k]; // формула (1)
			for (int i = k; i < dimension; i++)
			{
				coefficients[j][i] = coefficients[j][i] - d * coefficients[k][i]; // формула (2)
			}
			freeCoefficients[j] = freeCoefficients[j] - d * freeCoefficients[k]; // формула (3)
		}
	}

	for (int k = dimension-1; k >= 0; k--) // обратный ход
	{
		d = 0;
		for (int j = k + 1; j < dimension; j++)
		{
			s = coefficients[k][j] * result[j]; // формула (4)
			d = d + s; // формула (4)
		}
		result[k] = (freeCoefficients[k] - d) / coefficients[k][k]; // формула (4)
	}
}
// ---																										// Kramer Method
void KramerMethod(double** gaussCoefficients, double* gaussFreeCoefficients, double* gaussResult, double D)
{

	double D1	= gaussFreeCoefficients[0] * gaussCoefficients[1][1] * gaussCoefficients[2][2] 
				+ gaussCoefficients[0][1] * gaussCoefficients[1][2] * gaussFreeCoefficients[2] 
				+ gaussFreeCoefficients[1] * gaussCoefficients[2][1] * gaussCoefficients[0][2]
				- gaussFreeCoefficients[2] * gaussCoefficients[1][1] * gaussCoefficients[0][2]
				- gaussFreeCoefficients[1] * gaussCoefficients[0][1] * gaussCoefficients[2][2]
				- gaussFreeCoefficients[0] * gaussCoefficients[2][1] * gaussCoefficients[1][2];

	double D2	= gaussCoefficients[0][0] * gaussFreeCoefficients[1] * gaussCoefficients[2][2]
				+ gaussFreeCoefficients[0] * gaussCoefficients[1][2] * gaussCoefficients[2][0]
				+ gaussCoefficients[1][0] * gaussFreeCoefficients[2] * gaussCoefficients[0][2]
				- gaussCoefficients[2][0] * gaussFreeCoefficients[1] * gaussCoefficients[0][2]
				- gaussCoefficients[1][0] * gaussFreeCoefficients[0] * gaussCoefficients[2][2]
				- gaussCoefficients[0][0] * gaussFreeCoefficients[2] * gaussCoefficients[1][2];

	double D3	= gaussCoefficients[0][0] * gaussCoefficients[1][1] * gaussFreeCoefficients[2]
				+ gaussCoefficients[0][1] * gaussFreeCoefficients[1] * gaussCoefficients[2][0]
				+ gaussCoefficients[1][0] * gaussCoefficients[2][1] * gaussFreeCoefficients[0]
				- gaussCoefficients[2][0] * gaussCoefficients[1][1] * gaussFreeCoefficients[0]
				- gaussCoefficients[1][0] * gaussCoefficients[0][1] * gaussFreeCoefficients[2]
				- gaussCoefficients[0][0] * gaussCoefficients[2][1] * gaussFreeCoefficients[1];

	gaussResult[0] = D1 / D;
	gaussResult[1] = D2 / D;
	gaussResult[2] = D3 / D;
}
// ---																										// Transfer Point To New CoordinateSystem
PointGeometric TransferPointToNewCoordinateSystem(	PointGeometric	point,
													PointGeometric	pointCenterNewCoordinateSystem,
													VectorGeometric	vectorX,
													VectorGeometric	vectorY,
													VectorGeometric vectorZ)
{
	VectorGeometric vectorR(PointGeometric(0, 0, 0), pointCenterNewCoordinateSystem, false);

	VectorGeometric vectorPoint = vectorR + vectorX * point.X + vectorY * point.Y + vectorZ * point.Z;

	return PointGeometric(vectorPoint.X, vectorPoint.Y, vectorPoint.Z);
}