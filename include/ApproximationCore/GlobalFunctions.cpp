#include "stdafx.h"

#include "GlobalFunctions.h"

// ---																										// Rounding Of
double RoundingOf(double value, int accuracy)
{
	return ((double)int(value*pow(10, accuracy))) / pow(10, accuracy);
}

