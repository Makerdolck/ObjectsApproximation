#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>
#include "includes.h"

#define M_PI 3.1415926535897932384626433832795;

// ISO 1101:2017 http://geo-dim-tol.ir/files/iso1101.pdf
// ГОСТ 53442-2015 https://internet-law.ru/gosts/gost/60536/

// Коды значков https://en.wikipedia.org/wiki/Geometric_dimensioning_and_tolerancing

// Отклонения и допуски формы (ГОСТ24462 - 83) https://studfiles.net/preview/5125915/page:26/

enum TOLERANCE_NAME {
	FORM_STRAIGHTNESS,
	FORM_FLATNESS,
	FORM_ROUNDNESS,
	FORM_CYLINDRICITY,

	ORIENTATION_PARALLELISM,
	ORIENTATION_PERPENDICULARITY,
	ORIENTATION_ANGULARITY,

	LOCATION_POSITION,
	LOCATION_CONCENTRICITY,
	LOCATION_COAXIALITY,
	LOCATION_SYMMETRY,

	RUNOUT_FACE,
	RUNOUT_RADIAL
};

class Tolerance
{
public:
	Tolerance();
	Tolerance(std::vector<ToleranceObject*>* toleranceObjectsArray);
	~Tolerance();

	// Допуски формы
	double FormStraightness(LineSegmentApprox* line); // Прямолинейность
	double FormFlatness(PlaneApprox* plane); // Плоскостность
	double FormRoundness(CircleApprox *circle); // Круглость
	double FormCylindricity(CylinderApprox *cylinder); // Цилиндричность

	
	// Допуски ориентации
	double OrientationParallelism(PlaneApprox* base, PlaneApprox* control);// Параллельность
	double OrientationParallelism(LineSegmentApprox* base, LineSegmentApprox* control);// Параллельность
	double OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control); // Перпендикулярность
	double OrientationAngularity(PlaneApprox* base, PlaneApprox* control, double angle); // Наклон

	// Допуски месторасположения
	double LocationPosition(PlaneApprox* base1, double l1, PlaneApprox* base2, double l2, CircleApprox* circle); // Позиционирование
	double LocationConcentricity(CircleApprox *circleA, CircleApprox *circleB); // Концентричность (для точек)
	double LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB); // Соосность (для осей)
	double LocationCoaxiality(CylinderApprox* cylinderA, ConeApprox* coneB); // Соосность (для осей)
	double LocationCoaxiality(ConeApprox* coneA, CylinderApprox* cylinderB); // Соосность (для осей)
	double LocationCoaxiality(ConeApprox* coneA, ConeApprox* coneB);// Соосность (для осей)
	double LocationSymmetry(PlaneApprox* base, PlaneApprox* control1, PlaneApprox* control2); // Симметричность
	double LocationSymmetry(LineSegmentApprox* base, LineSegmentApprox* control1, LineSegmentApprox* control2); // Симметричность

	// Допуски биения
	double RunOutFace(CylinderApprox* base, PlaneApprox* control); // Торцевое
	double RunOutFace(CylinderApprox* base, CircleApprox* control); // Торцевое
	double RunOutRadial(CylinderApprox* base, CylinderApprox* control); // Радиальное
	

	// Обработка и добавление объекта в список для отрисовки
	void addNewObject(ToleranceObject* obj);
	double round(double value, int num_after_point); // Округление до n цифры после запятой
	VectorGeometric rotatePlane(PlaneApprox* plane, VectorGeometric axis, double a);
	VectorGeometric rotatePlane(VectorGeometric* v, VectorGeometric axis, double a);

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov
	double AngleBetween(PlaneApprox plane1, PlaneApprox plane2);
	double AngleBetween(VectorGeometric n1, VectorGeometric n2);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double DistanceBetween(PlaneApprox plane, PointGeometric point);
	double DistanceBetween(VectorGeometric planeNormal, PointGeometric point);
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};
