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

	int toleranceName; // ID �������� �������
	double toleranceValue; // �������� ����������

	ToleranceBase *Base = nullptr; // ����
	PointGeometric PointStart; // ������ ��� �����

	/*double boxHeight = 3; // ������ ��������������
	double boxFirstSectionWidth = 3; // ������ �������������� ��� ������ �������
	double boxSecondSectionWidth = 6; // ������ �������������� ��� �������� ����������
	double boxThirdSectionWidth = 3; // ������ �������������� ��� ��� ����
	*/

	/*
	PointGeometric getCenterBage(); // ����� ����� ��� ������
	PointGeometric getCenterToleranceValue(); // ����� ����� ��� �����
	PointGeometric getCenterToleranceBaseName(); // ����� ����� ��� ��� ����


	void changeBoxDirection();
	void setBoxPosition(int position);
	void setFrameSize(double firstSectionWidth, double secondSectionWidth, double thirdSectionWidth, double height);
	double Width();

	virtual ToleranceObjectType getType(); // ��� �������
	*/
};
