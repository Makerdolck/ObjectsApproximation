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
	static int baseCounter;
	static std::vector <char> *charStack;
	static bool isStackInit;
	ToleranceBase();
	ToleranceBase(ObjectApprox* obj);
	ToleranceBase(LineSegmentApprox* obj);
	ToleranceBase(PlaneApprox* obj);
	ToleranceBase(AxialLine* obj);
	ToleranceBase(ToleranceObject* obj);
	~ToleranceBase();


	PointGeometric PointStart; // ������ ��� �����


	CString baseName;
	char baseChar;
	ToleranceObjectType getType();
	ToleranceObject* toleranceObject;
	
private:
	char* baseCharArray = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


};



#endif