#pragma once


#ifndef __DIAMETER_LINE_H__
#define __DIAMETER_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"


class DiameterLine :
	public ToleranceObject
{
public:

	DiameterLine(CircleApprox* circle);
	DiameterLine(CylinderApprox* cylinder);
	~DiameterLine();

	PointGeometric centerPoint;
	double diameter;


	ToleranceObjectType getType();
};

#endif