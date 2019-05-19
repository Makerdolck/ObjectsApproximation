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

	// Допуски ориентации
	double OrientationParallelism(PlaneApprox* base, PlaneApprox* control); // Параллельность
	int OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control); // Перпендикулярность
	int OrientationAngularity(std::vector<ObjectApprox*>* objectsArray); // Наклон

	// Допуски месторасположения
	int LocationPosition(std::vector<ObjectApprox*>* objectsArray); // Позиционирование
	int LocationConcentricity(CircleApprox *circleA, CircleApprox *circleB); // Концентричность (для точек)
	int LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB); // Соосность (для осей)
	int LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // Симметричность

	// Допуски биения
	int RunOutCircular(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности
	int RunOutTotal(std::vector<ObjectApprox*>* objectsArray); // Форма заданной поверхности
	

	// Обработка и добавление объекта в список для отрисовки
	void DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawDiameterLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // Осевая линия
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // Допуски формы круглости

	void DrawOrientationParallelism(std::vector<ObjectApprox*>* objectsArray);

	void DrawLocationConcentricity(std::vector<ObjectApprox*>* objectsArray);
	void DrawLocationCoaxiality(std::vector<ObjectApprox*>* objectsArray);


private:

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	void addNewObject(ToleranceObject* obj);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double DistanceBetween(PlaneApprox plane, PointGeometric point);
	double round(double value, int num_after_point); // Округление до n цифры после запятой
	
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};

