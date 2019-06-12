#include "stdafx.h"
#include "ToleranceBase.h"


//std::vector <char> ToleranceBase::charStack { 'Z', 'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A' };
//std::vector <char> ToleranceBase::charStack { '�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�' };
std::vector <CString> ToleranceBase::charStack { L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�", L"�" };
//std::vector <char> ToleranceBase::charStack { '�', '�', '�' };


ToleranceBase::ToleranceBase()
{
}

ToleranceBase::ToleranceBase(ObjectApprox* obj)
{
	this->objMath = obj->objMath;

	if (obj->objMath->GetName() == CylinderApprox().GetName()) {
		CylinderApprox tmp = *((CylinderApprox*)obj->objMath);
		this->PointStart = tmp.PointsForApprox.operator[](0);
	}

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



ToleranceBase::ToleranceBase(ToleranceObject* obj)
{
	baseChar = charStack.back();
	charStack.pop_back();
	this->objMath = obj->objMath;
	
}




ToleranceBase::~ToleranceBase()
{
	charStack.push_back(baseChar);
	TRACE("Deleted ToleranceBase\n");
}

ToleranceObject::ToleranceObjectType ToleranceBase::getType()
{
	return ToleranceObjectType::BASE_LINE;
}


