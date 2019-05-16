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
	int FormLineProfile(std::vector<ObjectApprox*>* objectsArray); // ����� ��������� �������
	int FormSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������

	// ������� ����������
	int OrientationParallelism(std::vector<ObjectApprox*>* objectsArray); // ��������������
	int OrientationPerpendicularity(std::vector<ObjectApprox*>* objectsArray); // ������������������
	int OrientationAngularity(std::vector<ObjectApprox*>* objectsArray); // ������
	int OrientationLineProfile(std::vector<ObjectApprox*>* objectsArray); // ����� ��������� �������
	int OrientationSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������

	// ������� �����������������
	int LocationPosition(std::vector<ObjectApprox*>* objectsArray); // ����������������
	int LocationConcentricity(std::vector<ObjectApprox*>* objectsArray); // ��������������� (��� �����)
	int LocationCoaxiality(std::vector<ObjectApprox*>* objectsArray); // ��������� (��� ����)
	int LocationSymmetry(std::vector<ObjectApprox*>* objectsArray); // ��������������
	int LocationLineProfile(std::vector<ObjectApprox*>* objectsArray); // ����� ��������� �������
	int LocationSurfaceProfile(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������

	// ������� ������
	int RunOutCircular(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������
	int RunOutTotal(std::vector<ObjectApprox*>* objectsArray); // ����� �������� �����������
	

	// ��������� � ���������� ������� � ������ ��� ���������
	void DrawSizeLine(std::vector<ObjectApprox*>* objectsArray);
	void DrawAxialLine(std::vector<ObjectApprox*>* objectsArray); // ������ �����
	void DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray); // ������� ����� ���������

private:

	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	void addNewObject(ToleranceObject* obj);
	double DistanceBetween(PointGeometric point1, PointGeometric point2);
	double DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point);
	double round(double value, int num_after_point); // ���������� �� n ����� ����� �������
	// ���������� �� ����� �� ������
	PointGeometric centerByPoints(PointGeometric* points, int arraySize);
};

