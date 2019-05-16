#include <stdafx.h>

#include "ToleranceObject.h"

ToleranceObject::ToleranceObject()
{
}

ToleranceObject::~ToleranceObject()
{
}

ToleranceObject::ToleranceObjectType ToleranceObject::getType()
{
	return UNDEFINED;
}

PointGeometric ToleranceObject::centerByPoints(PointGeometric* points, int arraySize)
{
	double sumX = 0, sumY = 0, sumZ = 0;

	for (int i = 0; i < arraySize; i++)
	{
		sumX += points[i].X;
		sumY += points[i].Y;
		sumZ += points[i].Z;
	}

	double x = sumX / (double)arraySize;	
	double y = sumY / (double)arraySize;
	double z = sumZ / (double)arraySize;

	return PointGeometric(x, y, z);
}
