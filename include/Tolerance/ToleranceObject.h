#pragma once

#ifndef __TOLERANCE_OBJECT_H__
#define __TOLERANCE_OBJECT_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include <iostream>


class ToleranceObject
{
public:
	ToleranceObject();
	~ToleranceObject();

	enum ToleranceObjectType {
		UNDEFINED,
		SIZE_LINE,
		AXIAL_LINE,
		BASE_LINE,
		FORM_ROUDNESS

	};

	GeomObjectApprox* objMath;

	bool flagSelected;
	bool isVisible; // Видимость во вью

	virtual ToleranceObjectType getType(); // Тип объекта

protected:
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);

};



#endif /* __TOLERANCE_OBJECT_H__ */