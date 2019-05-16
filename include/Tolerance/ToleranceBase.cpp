#include "stdafx.h"
#include "ToleranceBase.h"


ToleranceBase::ToleranceBase()
{
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
