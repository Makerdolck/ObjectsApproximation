#pragma once

#ifndef __TOLERANCE_BASE_H__
#define __TOLERANCE_BASE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"


class ToleranceBase : public ToleranceObject
{
public:
	ToleranceBase(ObjectApprox* obj);
	~ToleranceBase();

	CString baseName;
	
	ToleranceObjectType getType();
	

};

#endif