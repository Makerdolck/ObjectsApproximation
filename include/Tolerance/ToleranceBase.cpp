#include "stdafx.h"
#include "ToleranceBase.h"



ToleranceBase::ToleranceBase(ObjectApprox* obj)
{
	this->objMath = obj->objMath;
	this->baseName = obj->Name.c_str();
}

ToleranceBase::~ToleranceBase()
{
}

ToleranceObject::ToleranceObjectType ToleranceBase::getType()
{
	return ToleranceObjectType::BASE_LINE;
}


