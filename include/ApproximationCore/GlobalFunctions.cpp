#include "stdafx.h"
#include<iostream>
#include "GlobalFunctions.h"

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

