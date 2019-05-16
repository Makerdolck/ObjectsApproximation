#pragma once


#ifndef __SIZE_LINE_H__
#define __SIZE_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"



class SizeLine : public ToleranceObject
{
public:
	SizeLine();
	SizeLine(LineSegmentApprox *lineSegment);
	SizeLine(CylinderApprox* cylinder);
	SizeLine(ConeApprox* cone);
	~SizeLine();

	PointGeometric	PointStart;
	PointGeometric	PointEnd; 
	double offset = 0; // Расстояние от объекта до размерной линии


	void paint(); // Построить линию
	ToleranceObjectType getType();



};


#endif
