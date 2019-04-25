#pragma once

#ifndef __OBJECT_APPROX_C3D_H__
#define __OBJECT_APPROX_C3D_H__

#include "GeomObjectApprox.h"
#include "CompensationOptionNamesEnum.h"
#include <iostream>

class ObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	GeomObjectApprox			*objMath;

	std::wstring				Name;

	bool						flagReady,
								flagSelected;

	long unsigned int			objID;

	CompensationOptionNamesEnum	CompensationOption;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ObjectApprox();
	~ObjectApprox();

	void	UpdateCompensationOption(wchar_t* option, float value);
};

#endif /* __OBJECT_APPROX_C3D_H__ */