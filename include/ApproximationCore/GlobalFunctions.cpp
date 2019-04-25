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
	PointGeometric	pointResult,
					pointCenter = pointCenterNewCoordinateSystem;

	double			gaussFreeCoefficients[3], 
					gaussResult[3],
					**gaussCoefficients = new double*[3];

	for (int i = 0; i < 3; i++)
		gaussCoefficients[i] = new double[3];


	gaussCoefficients[0][0] = vectorX.X;	gaussCoefficients[0][1] = vectorX.Y;	gaussCoefficients[0][2] = vectorX.Z;
	gaussFreeCoefficients[0] = point.X + pointCenter.X*vectorX.X + pointCenter.Y*vectorX.Y + pointCenter.Z*vectorX.Z;

	gaussCoefficients[1][0] = vectorY.X;	gaussCoefficients[1][1] = vectorY.Y;	gaussCoefficients[1][2] = vectorY.Z;
	gaussFreeCoefficients[1] = point.Y + pointCenter.X*vectorY.X + pointCenter.Y*vectorY.Y + pointCenter.Z*vectorY.Z;

	gaussCoefficients[2][0] = vectorZ.X;	gaussCoefficients[2][1] = vectorZ.Y;	gaussCoefficients[2][2] = vectorZ.Z;
	gaussFreeCoefficients[2] = point.Z + pointCenter.X*vectorZ.X + pointCenter.Y*vectorZ.Y + pointCenter.Z*vectorZ.Z;

	double D	= gaussCoefficients[0][0] * gaussCoefficients[1][1] * gaussCoefficients[2][2]
				+ gaussCoefficients[0][1] * gaussCoefficients[1][2] * gaussCoefficients[2][0]
				+ gaussCoefficients[1][0] * gaussCoefficients[2][1] * gaussCoefficients[0][2]
				- gaussCoefficients[2][0] * gaussCoefficients[1][1] * gaussCoefficients[0][2]
				- gaussCoefficients[1][0] * gaussCoefficients[0][1] * gaussCoefficients[2][2]
				- gaussCoefficients[0][0] * gaussCoefficients[2][1] * gaussCoefficients[1][2];
	
	if (D > 0)	// D != 0
		KramerMethod(gaussCoefficients, &gaussFreeCoefficients[0], &gaussResult[0], D);
	else
		GaussMethod(gaussCoefficients, &gaussFreeCoefficients[0], 3, &gaussResult[0]);

	pointResult = PointGeometric(gaussResult[0], gaussResult[1], gaussResult[2]);


	for (int i = 0; i < 3; i++)
		delete [] gaussCoefficients[i];
	delete [] gaussCoefficients;

	return pointResult;
}
