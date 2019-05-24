#pragma once
  
#include <iostream>
#include "Tolerance.h"




class ToleranceFrame : public ToleranceObject
{
public:
	

	ToleranceFrame();
	ToleranceFrame(ObjectApprox *controlObject, int toleranceName, double toleranceValue);
	ToleranceFrame(ToleranceBase *base, ObjectApprox *controlObject, int toleranceName, double toleranceValue);
	~ToleranceFrame();

	double boxHeight = 3; // Высота прямоугольника
	double boxFirstSectionWidth = 3; // Ширина прямоугольника под значок допуска
	double boxSecondSectionWidth = 6; // Ширина прямоугольника под значение отклонения
	double boxThirdSectionWidth = 3; // Ширина прямоугольника под имя базы
	
	int toleranceName; // Название допуска
	double toleranceValue; // Значение отклонения

	ToleranceBase *Base = nullptr; // База
	PointGeometric PointStart; // Откуда идёт линия

	PointGeometric getCenterBage(); // Центр места под значок
	PointGeometric getCenterToleranceValue(); // Центр места под текст
	PointGeometric getCenterToleranceBaseName(); // Центр места под имя базы


	void changeBoxDirection();
	void setFrameSize(double firstSectionWidth, double secondSectionWidth, double thirdSectionWidth, double height);
	double Width();

	virtual ToleranceObjectType getType(); // Тип объекта

private:
	
};
