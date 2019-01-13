#pragma once

#ifndef __OBJECT_APPROX_C3D_H__
#define __OBJECT_APPROX_C3D_H__

#include "GeomObjectApprox.h"
#include <iostream>
//#include "..\\vsn_scenesegment.h"

class ObjectApproxC3D
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	GeomObjectApprox	*objMath;
	//SceneSegment		*objSS;

	std::wstring		Name;

	bool				flagReady;


//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	ObjectApproxC3D();
	~ObjectApproxC3D();
};

#endif /* __OBJECT_APPROX_C3D_H__ */