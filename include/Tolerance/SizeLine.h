#pragma once


#ifndef __SIZE_LINE_H__
#define __SIZE_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"



class SizeLine : public ToleranceObject
{
public:
	SizeLine(ObjectApprox *obj1, ObjectApprox* obj2);
	~SizeLine();

	PointGeometric	PointStart;
	PointGeometric	PointEnd; 
	GeomObjectApprox* objMath2;

	ToleranceObjectType getType();
	double length();

};


#endif
