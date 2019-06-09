#pragma once

#ifndef __TOLERANCE_OBJECT_H__
#define __TOLERANCE_OBJECT_H__

#include <ApproximationCore/_ALLincludesCore.h>


class ToleranceObject
{
public:
	enum ToleranceObjectType {
		UNDEFINED,
		SIZE_LINE,
		DIAMETER_LINE,
		AXIAL_LINE,
		BASE_LINE,
		TOLERANCE_FRAME,
		FORM_ROUDNESS

	};
	
	static int totalObjectCounter; // Общее количество созданных объектов типа ToleranceObject

	ToleranceObject(); // Конструктор по умолчанию
	~ToleranceObject(); // Деструктор по умолчанию


	GeomObjectApprox* objMath; // Геометрический объект, который описывает класс ToleranceObject
	int objID; // ID объекта ToleranceObject

	bool flagSelected = true; // Флаг определяет выбран ли объект в данный  момент в рабочем пространстве
	bool isVisible = true; // Видимость во вью

	PointGeometric PointPosition; // Точка определяет положение в пространстве
	virtual ToleranceObjectType getType(); // Тип объекта. 

protected:
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};


#endif /* __TOLERANCE_OBJECT_H__ */