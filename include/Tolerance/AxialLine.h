#pragma once

#ifndef __AXIAL_LINE_H__
#define __AXIAL_LINE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"




class AxialLine :
	public ToleranceObject
{
public:
	AxialLine();
	AxialLine(CircleApprox* obj);
	AxialLine(CylinderApprox* obj);
	AxialLine(ConeApprox* obj);

	~AxialLine();

	VectorGeometric dirVector; // ������������ ������

	PointGeometric startPoint;
	PointGeometric endPoint;
	PointGeometric centerPoint;
	double offset; // ����������� ������� �������


	ToleranceObjectType getType();
	

	

};

#endif