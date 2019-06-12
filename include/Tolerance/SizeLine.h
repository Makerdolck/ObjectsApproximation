#pragma once


#ifndef __SIZE_LINE_H__
#define __SIZE_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"



class SizeLine : public ToleranceObject
{
public:
	SizeLine(ObjectApprox *obj1, ObjectApprox* obj2);
	SizeLine(PointApprox *point1, PointApprox *point2);
	SizeLine(LineSegmentApprox *lineSegment);
	SizeLine(CylinderApprox* cylinder);
	SizeLine(ConeApprox* cone);
	~SizeLine();

	PointGeometric	PointStart;
	PointGeometric	PointEnd; 

	GeomObjectApprox* objMath2;
	//double offset = 0; // ���������� �� ������� �� ��������� �����

	ToleranceObjectType getType();
	double length();

};


#endif
