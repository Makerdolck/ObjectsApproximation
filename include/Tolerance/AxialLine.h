#pragma once

#ifndef __AXIAL_LINE_H__
#define __AXIAL_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"




class AxialLine : public ToleranceObject
{
public:
	AxialLine(CircleApprox* obj); // Удалить
	AxialLine(CylinderApprox* obj);
	AxialLine(ConeApprox* obj);
	~AxialLine();

	PointGeometric startPoint;
	PointGeometric endPoint;
	

	ToleranceObjectType getType();
};

#endif