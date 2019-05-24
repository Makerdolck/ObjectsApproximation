#pragma once

#ifndef __TOLERANCE_OBJECT_H__
#define __TOLERANCE_OBJECT_H__

#include <ApproximationCore/_ALLincludesCore.h>



class ToleranceObject
{
public:
	ToleranceObject();
	~ToleranceObject();

	enum ToleranceObjectType {
		UNDEFINED,
		SIZE_LINE,
		DIAMETER_LINE,
		AXIAL_LINE,
		BASE_LINE,
		TOLERANCE_FRAME,
		FORM_ROUDNESS

	};

	GeomObjectApprox* objMath;

	bool flagSelected;
	bool isVisible; // ��������� �� ���

	PointGeometric PointPosition; // ����� ��������� �����

	virtual ToleranceObjectType getType(); // ��� �������

protected:
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);

};



#endif /* __TOLERANCE_OBJECT_H__ */