#pragma once
#include "ToleranceObject.h"
class AngleLine :
	public ToleranceObject
{
public:
	AngleLine(ConeApprox* cone);
	~AngleLine();
};

