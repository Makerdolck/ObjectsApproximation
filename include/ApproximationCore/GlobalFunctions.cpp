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

	for (int k = 0; k < dimension; k++) // ������ ���
	{
		for (int j = k + 1; j < dimension; j++)
		{
			d = coefficients[j][k] / coefficients[k][k]; // ������� (1)
			for (int i = k; i < dimension; i++)
			{
				coefficients[j][i] = coefficients[j][i] - d * coefficients[k][i]; // ������� (2)
			}
			freeCoefficients[j] = freeCoefficients[j] - d * freeCoefficients[k]; // ������� (3)
		}
	}

	for (int k = dimension-1; k >= 0; k--) // �������� ���
	{
		d = 0;
		for (int j = k + 1; j < dimension; j++)
		{
			s = coefficients[k][j] * result[j]; // ������� (4)
			d = d + s; // ������� (4)
		}
		result[k] = (freeCoefficients[k] - d) / coefficients[k][k]; // ������� (4)
	}
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

	double	gaussFreeCoefficients[3], 
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

	GaussMethod(gaussCoefficients, &gaussFreeCoefficients[0], 3, &gaussResult[0]);


	pointResult = PointGeometric(gaussResult[0], gaussResult[1], gaussResult[2]);


	for (int i = 0; i < 3; i++)
		delete gaussCoefficients[i];
	delete gaussCoefficients;



	return pointResult;
}