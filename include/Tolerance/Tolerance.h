#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>
#include "includes.h"


// ISO 1101:2017 http://geo-dim-tol.ir/files/iso1101.pdf
// ���� 53442-2015 https://internet-law.ru/gosts/gost/60536/

// ���� ������� https://en.wikipedia.org/wiki/Geometric_dimensioning_and_tolerancing

// ���������� � ������� ����� (����24462 - 83) https://studfiles.net/preview/5125915/page:26/
class Tolerance
{
public:
	Tolerance(std::vector<ToleranceObject*>* toleranceObjectsArray);
	~Tolerance();

	
	// ������� �����
	double FormStraightness(LineSegmentApprox* line); // ���������������
	double FormFlatness(PlaneApprox* plane); // �������������
	double FormRoundness(CircleApprox *circle); // ���������
	double FormCylindricity(CylinderApprox *cylinder); // ��������������

	// ������� ����������
	double OrientationParallelism(PlaneApprox* base, PlaneApprox* control); // ��������������
	int OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control); // ������������������
	int OrientationAngularity(std::vector<ObjectApprox*>* objectsArray); // ������

	// ������� �����������������
	int LocationPosition(std::vector<ObjectApprox*>* objectsArray); // ����������������
	int LocationConcentricity(CircleApprox *circleA, CircleApprox *circleB); // ��������������� (��� �����)
	int LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB); // ��������� (��� ����)
	int LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // ��������������

	// ������� ������
	int RunOutCircular(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������
	int RunOutTotal(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������
	

	// ��������� � ���������� ������� � ������ ��� ���������
	void DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawDiameterLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // ������ �����
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // ������� ����� ���������

	void DrawOrientationParallelism(std::vector<ObjectApprox*>* objectsArray);

	void DrawLocationConcentricity(std::vector<ObjectApprox*>* objectsArray);
	void DrawLocationCoaxiality(std::vector<ObjectApprox*>* objectsArray);


private:

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	void addNewObject(ToleranceObject* obj);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double DistanceBetween(PlaneApprox plane, PointGeometric point);
	double round(double value, int num_after_point); // ���������� �� n ����� ����� �������
	
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};

