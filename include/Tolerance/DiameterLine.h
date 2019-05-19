#pragma once


#ifndef __DIAMETER_LINE_H__
#define __DIAMETER_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"


class DiameterLine :
	public ToleranceObject
{
public:

	DiameterLine(CircleApprox* circle, bool isOutdoor);
	DiameterLine(CylinderApprox* cylinder, bool isOutdoor);
	~DiameterLine();

	PointGeometric centerPoint;
	double diameter;

	bool isOutdoor = false; // –азмерна€ лини€ выходит за окружность или нет

	ToleranceObjectType getType();
};

#endif