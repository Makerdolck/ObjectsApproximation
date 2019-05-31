#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>
#include "includes.h"


// ISO 1101:2017 http://geo-dim-tol.ir/files/iso1101.pdf
// ���� 53442-2015 https://internet-law.ru/gosts/gost/60536/

// ���� ������� https://en.wikipedia.org/wiki/Geometric_dimensioning_and_tolerancing

// ���������� � ������� ����� (����24462 - 83) https://studfiles.net/preview/5125915/page:26/

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

	

	// ������� �����
	double FormStraightness(LineSegmentApprox* line); // ���������������
	double FormFlatness(PlaneApprox* plane); // �������������
	double FormRoundness(CircleApprox *circle); // ���������
	double FormCylindricity(CylinderApprox *cylinder); // ��������������

	// ������� ����������
	double OrientationParallelism(PlaneApprox* base, PlaneApprox* control);
	double OrientationParallelism(LineSegmentApprox* base, LineSegmentApprox* control);

	
	
	VectorGeometric getIntersectionVector(PlaneApprox Plane1, PlaneApprox Plane2);
	VectorGeometric rotatePlane(PlaneApprox* plane, VectorGeometric axis, double a);

	VectorGeometric rotatePlane(VectorGeometric* v, VectorGeometric axis, double a);

	// ��������������
	double OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control); // ������������������
	double OrientationAngularity(PlaneApprox* base, PlaneApprox* control, double angle); // ������
	//double OrientationAngularity(PlaneApprox* base, PlaneApprox* control, PlaneApprox* result);
	//double OrientationAngularity(PlaneGeometric* base, PlaneGeometric* control, PlaneGeometric* result);
	void rotatePlane(PlaneGeometric* plane, VectorGeometric axis, PlaneGeometric* result, double a);
	void rotatePlane(PlaneGeometric* plane, VectorGeometric* axis, PlaneGeometric* result, double a);
	//double OrientationAngularity(PlaneGeometric* base, PlaneGeometric* control);

	void rotatePlane(PlaneGeometric* plane, PlaneGeometric* result, double a);

	void rotatePlane(LineGeometric* axis, PlaneGeometric* plane, double angle);

	

	// ������� �����������������
	double LocationPosition(std::vector<ObjectApprox*>* objectsArray); // ����������������
	double LocationConcentricity(CircleApprox *circleA, CircleApprox *circleB); // ��������������� (��� �����)
	double LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB); // ��������� (��� ����)
	double LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // ��������������

	// ������� ������
	double RunOutFace(CylinderApprox* base, RectangleApprox* control); // ��������
	double RunOutFace(CylinderApprox* base, CircleApprox* control); // ��������
	double RunOutRadial(CylinderApprox* base, CylinderApprox* control); // ����������
	

	// ��������� � ���������� ������� � ������ ��� ���������
	SizeLine* DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	//void DrawToleranceFrame(std::vector<ObjectApprox*>* objectsArray);
	void DrawDiameterLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // ������ �����
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // ������� ����� ���������

	void DrawOrientationParallelism(std::vector<ObjectApprox*>* objectsArray);
	void DrawOrientationAngularity(std::vector<ObjectApprox*>* objectsArray);


	void DrawLocationConcentricity(std::vector<ObjectApprox*>* objectsArray);
	void DrawLocationCoaxiality(std::vector<ObjectApprox*>* objectsArray);


	void addNewObject(ToleranceObject* obj);
	double round(double value, int num_after_point); // ���������� �� n ����� ����� �������
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
