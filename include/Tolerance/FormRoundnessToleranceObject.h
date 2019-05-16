#pragma once


#ifndef __FORM_ROUDNESS_TOLERANCE_OBJECT_H__
#define __FORM_ROUDNESS_TOLERANCE_OBJECT_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"



class FormRoundnessToleranceObject : public ToleranceObject
{
public:
	FormRoundnessToleranceObject();
	FormRoundnessToleranceObject(CircleApprox* circle);
	~FormRoundnessToleranceObject();

	
	ToleranceObjectType getType();

};


#endif
