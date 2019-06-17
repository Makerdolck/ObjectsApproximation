#include "stdafx.h"
#include "ToleranceBase.h"

std::vector <CString> ToleranceBase::charStack { L"ß", L"Þ", L"Ý", L"Ü", L"Û", L"Ú", L"Ù", L"Ø", L"×", L"Ö", L"Õ", L"Ô", L"Ó", L"Ò", L"Ñ", L"Ð", L"Ï", L"Î", L"Í", L"Ì", L"Ë", L"Ê", L"É", L"È", L"Ç", L"Æ", L"¨", L"Å", L"Ä", L"Ã", L"Â", L"Á", L"À" };



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


