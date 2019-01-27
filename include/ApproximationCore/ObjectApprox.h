#pragma once

#ifndef __OBJECT_APPROX_C3D_H__
#define __OBJECT_APPROX_C3D_H__

#include "GeomObjectApprox.h"
#include <iostream>

class ObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	GeomObjectApprox	*objMath;

	std::wstring		Name;

	bool				flagReady,
						flagSelected;

	long int			objID;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ObjectApprox();
	~ObjectApprox();
};

#endif /* __OBJECT_APPROX_C3D_H__ */