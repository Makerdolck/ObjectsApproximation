#include "stdafx.h"

#include "ApproximationApprox.h"

//-----------------------------------------------------------------------------
// --- Approximation Function
//----------------------------------------------------------------------------- 
double ApproximationApprox::FunctionApprox(PointGeometric *points, int arraySize) { return 0; }

//-----------------------------------------------------------------------------
// --- Approximation Method
//----------------------------------------------------------------------------- 
void ApproximationApprox::Approximation(PointGeometric *points, int arraySize, double accuracy, VectorGeometric *vector, double *value)
{
	double step = 0.05;
	bool negativeDirection = false;

	double deviationSum, deviationSumOld;

	deviationSum = FunctionApprox(points, arraySize);

	do
	{
		deviationSumOld = deviationSum;
		*value += (1 + (-2) * (int)negativeDirection) * step;

		vector->Normalize();

		deviationSum = FunctionApprox(points, arraySize);

		if (deviationSum > deviationSumOld)
		{
			negativeDirection = !negativeDirection;
			step /= 2;
		}

	} while (fabs(deviationSumOld - deviationSum)  > accuracy);
}
