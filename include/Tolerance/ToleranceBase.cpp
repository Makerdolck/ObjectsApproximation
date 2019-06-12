#include "stdafx.h"
#include "ToleranceBase.h"


//std::vector <char> ToleranceBase::charStack { 'Z', 'Y', 'X', 'W', 'V', 'U', 'T', 'S', 'R', 'Q', 'P', 'O', 'N', 'M', 'L', 'K', 'J', 'I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A' };
//std::vector <char> ToleranceBase::charStack { 'ß','Þ','Ý','Ü','Û','Ú','Ù','Ø','×','Ö','Õ','Ô','Ó','Ò','Ñ','Ð','Ï','Î','Í','Ì','Ë','Ê','É','È','Ç','Æ','¨','Å','Ä','Ã','Â','Á','À' };
std::vector <CString> ToleranceBase::charStack { L"ß", L"Þ", L"Ý", L"Ü", L"Û", L"Ú", L"Ù", L"Ø", L"×", L"Ö", L"Õ", L"Ô", L"Ó", L"Ò", L"Ñ", L"Ð", L"Ï", L"Î", L"Í", L"Ì", L"Ë", L"Ê", L"É", L"È", L"Ç", L"Æ", L"¨", L"Å", L"Ä", L"Ã", L"Â", L"Á", L"À" };
//std::vector <char> ToleranceBase::charStack { 'Â', 'Á', 'À' };


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


