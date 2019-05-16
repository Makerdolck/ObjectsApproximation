#pragma once

#ifndef __TOLERANCE_BASE_H__
#define __TOLERANCE_BASE_H__

#include <ApproximationCore/_ALLincludesCore.h>
#include "ToleranceObject.h"
#include <iostream>

class ToleranceBase : public ToleranceObject
{
public:
	ToleranceBase();
	~ToleranceBase();

	
	ToleranceObjectType getType();
	char getCharName();

private:
	char charName;
};

#endif