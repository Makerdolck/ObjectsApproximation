#include <stdafx.h>

#include "FormRoundnessToleranceObject.h"

FormRoundnessToleranceObject::FormRoundnessToleranceObject(CircleApprox* circle)
{
	objMath = circle;
}

ToleranceObject::ToleranceObjectType FormRoundnessToleranceObject::getType()
{
	return FORM_ROUDNESS;
}
