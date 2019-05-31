#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>
#include "includes.h"


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
	double OrientationParallelism(PlaneApprox* base, PlaneApprox* control);
	double OrientationParallelism(LineSegmentApprox* base, LineSegmentApprox* control);

	
	
	VectorGeometric getIntersectionVector(PlaneApprox Plane1, PlaneApprox Plane2);
	VectorGeometric rotatePlane(PlaneApprox* plane, VectorGeometric axis, double a);

	VectorGeometric rotatePlane(VectorGeometric* v, VectorGeometric axis, double a);

	// Параллельность
	double OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control); // Перпендикулярность
	double OrientationAngularity(PlaneApprox* base, PlaneApprox* control, double angle); // Наклон
	//double OrientationAngularity(PlaneApprox* base, PlaneApprox* control, PlaneApprox* result);
	//double OrientationAngularity(PlaneGeometric* base, PlaneGeometric* control, PlaneGeometric* result);
	void rotatePlane(PlaneGeometric* plane, VectorGeometric axis, PlaneGeometric* result, double a);
	void rotatePlane(PlaneGeometric* plane, VectorGeometric* axis, PlaneGeometric* result, double a);
	//double OrientationAngularity(PlaneGeometric* base, PlaneGeometric* control);

	void rotatePlane(PlaneGeometric* plane, PlaneGeometric* result, double a);

	void rotatePlane(LineGeometric* axis, PlaneGeometric* plane, double angle);

	

	// Допуски месторасположения
	double LocationPosition(std::vector<ObjectApprox*>* objectsArray); // Позиционирование
	double LocationConcentricity(CircleApprox *circleA, CircleApprox *circleB); // Концентричность (для точек)
	double LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB); // Соосность (для осей)
	double LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // Симметричность

	// Допуски биения
	double RunOutFace(CylinderApprox* base, RectangleApprox* control); // Торцевое
	double RunOutFace(CylinderApprox* base, CircleApprox* control); // Торцевое
	double RunOutRadial(CylinderApprox* base, CylinderApprox* control); // Радиальное
	

	// Обработка и добавление объекта в список для отрисовки
	SizeLine* DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	//void DrawToleranceFrame(std::vector<ObjectApprox*>* objectsArray);
	void DrawDiameterLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // Осевая линия
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // Допуски формы круглости

	void DrawOrientationParallelism(std::vector<ObjectApprox*>* objectsArray);
	void DrawOrientationAngularity(std::vector<ObjectApprox*>* objectsArray);


	void DrawLocationConcentricity(std::vector<ObjectApprox*>* objectsArray);
	void DrawLocationCoaxiality(std::vector<ObjectApprox*>* objectsArray);


	void addNewObject(ToleranceObject* obj);
	double round(double value, int num_after_point); // Округление до n цифры после запятой
private:

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	double AngleBetween(PlaneApprox plane1, PlaneApprox plane2);
	double AngleBetween(VectorGeometric n1, VectorGeometric n2);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double DistanceBetween(PlaneApprox plane, PointGeometric point);
	double DistanceBetween(VectorGeometric planeNormal, PointGeometric point);
	
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};
