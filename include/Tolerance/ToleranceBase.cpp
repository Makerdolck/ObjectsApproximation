#include "stdafx.h"
#include "ToleranceBase.h"

std::vector <CString> ToleranceBase::charStack { L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�" };



ToleranceBase::ToleranceBase()
{
}



ToleranceBase::ToleranceBase(CylinderApprox* obj)
{
	this->objMath = obj;
	PointStart = obj->PointsForApprox.operator[](0);
	baseChar = charStack.back();
	charStack.pop_back();
}

ToleranceBase::ToleranceBase(LineSegmentApprox* obj)
{
	this->objMath = obj;
	baseChar = charStack.back();
	charStack.pop_back();
}



ToleranceBase::ToleranceBase(PlaneApprox* obj)
{
	this->objMath = obj;
	PointStart = obj->Line.Point;
	baseChar = charStack.back();
	charStack.pop_back();
}

ToleranceBase::ToleranceBase(CircleApprox* obj)
{
	this->objMath = obj;
	PointStart = obj->PointsForApprox.operator[](0);
	baseChar = charStack.back();
	charStack.pop_back();
}

ToleranceBase::ToleranceBase(AxialLine* obj)
{
	baseChar = charStack.back();
	charStack.pop_back();

	this->toleranceObject = obj;
	this->objMath = obj->objMath;
	PointStart = obj->startPoint;
}


ToleranceBase::~ToleranceBase()
{
	charStack.push_back(baseChar);
}

ToleranceObject::ToleranceObjectType ToleranceBase::getType()
{
	return ToleranceObjectType::BASE_LINE;
}


