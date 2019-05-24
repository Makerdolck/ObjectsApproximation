#include "stdafx.h"
#include "ToleranceBase.h"



ToleranceBase::ToleranceBase(ObjectApprox* obj)
{
	this->objMath = obj->objMath;
}

ToleranceBase::~ToleranceBase()
{
}

ToleranceObject::ToleranceObjectType ToleranceBase::getType()
{
	return ToleranceObjectType::BASE_LINE;
}

char ToleranceBase::getCharName()
{
	return charName;
}
