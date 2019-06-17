#pragma once

#ifndef __TOLERANCE_BASE_H__
#define __TOLERANCE_BASE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "AxialLine.h"
#include "ToleranceObject.h"
#include <stack>


class ToleranceBase : public ToleranceObject
{
public:
	static std::vector <CString> charStack;
	ToleranceBase();
	ToleranceBase(CylinderApprox* obj);
	ToleranceBase(LineSegmentApprox* obj);
	ToleranceBase(PlaneApprox* obj);
	ToleranceBase(CircleApprox* obj);
	ToleranceBase(AxialLine* obj);
	~ToleranceBase();

	PointGeometric PointStart; // ��������� ����� �����
	CString baseChar; // ����� ����
	ToleranceObject* toleranceObject = nullptr; // � ������ ������� ��������� ����

	ToleranceObjectType getType();
};



#endif