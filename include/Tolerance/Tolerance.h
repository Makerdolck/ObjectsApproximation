#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>
#include "includes.h"


// ISO 1101:2017 http://geo-dim-tol.ir/files/iso1101.pdf
// ГОСТ 53442-2015 https://internet-law.ru/gosts/gost/60536/

// Коды значков https://en.wikipedia.org/wiki/Geometric_dimensioning_and_tolerancing

// Отклонения и допуски формы (ГОСТ24462 - 83) https://studfiles.net/preview/5125915/page:26/
class Tolerance
{
public:
	Tolerance(std::vector<ToleranceObject*>* toleranceObjectsArray);
	~Tolerance();

	
	// Допуски формы
	double FormStraightness(LineSegmentApprox* line); // Прямолинейность
	double FormFlatness(PlaneApprox* plane); // Плоскостность
	double FormRoundness(CircleApprox *circle); // Круглость
	double FormCylindricity(CylinderApprox *cylinder); // Цилиндричность
	int FormLineProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданного профиля
	int FormSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности

	// Допуски ориентации
	int OrientationParallelism(std::vector<ObjectApprox*>* objectsArray); // Параллельность
	int OrientationPerpendicularity(std::vector<ObjectApprox*>* objectsArray); // Перпендикулярность
	int OrientationAngularity(std::vector<ObjectApprox*>* objectsArray); // Наклон
	int OrientationLineProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданного профиля
	int OrientationSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности

	// Допуски месторасположения
	int LocationPosition(std::vector<ObjectApprox*>* objectsArray); // Позиционирование
	int LocationConcentricity(std::vector<ObjectApprox*>* objectsArray); // Концентричность (для точек)
	int LocationCoaxiality(std::vector<ObjectApprox*>* objectsArray); // Соосность (для осей)
	int LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // Симметричность
	int LocationLineProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданного профиля
	int LocationSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности

	// Допуски биения
	int RunOutCircular(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности
	int RunOutTotal(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности
	

	// Обработка и добавление объекта в список для отрисовки
	void DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // Осевая линия
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // Допуски формы круглости

private:

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	void addNewObject(ToleranceObject* obj);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double round(double value, int num_after_point); // Округление до n цифры после запятой
	// Расстояние от точки до прямой
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};

