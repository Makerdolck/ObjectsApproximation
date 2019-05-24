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

	double boxHeight = 3; // ������ ��������������
	double boxFirstSectionWidth = 3; // ������ �������������� ��� ������ �������
	double boxSecondSectionWidth = 6; // ������ �������������� ��� �������� ����������
	double boxThirdSectionWidth = 3; // ������ �������������� ��� ��� ����
	
	int toleranceName; // �������� �������
	double toleranceValue; // �������� ����������

	ToleranceBase *Base = nullptr; // ����
	PointGeometric PointStart; // ������ ��� �����

	PointGeometric getCenterBage(); // ����� ����� ��� ������
	PointGeometric getCenterToleranceValue(); // ����� ����� ��� �����
	PointGeometric getCenterToleranceBaseName(); // ����� ����� ��� ��� ����


	void changeBoxDirection();
	void setFrameSize(double firstSectionWidth, double secondSectionWidth, double thirdSectionWidth, double height);
	double Width();

	virtual ToleranceObjectType getType(); // ��� �������

private:
	
};
