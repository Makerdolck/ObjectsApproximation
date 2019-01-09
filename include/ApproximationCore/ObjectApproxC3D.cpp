#include "stdafx.h"

#include "ObjectApproxC3D.h"


// ---																										// Constructors
ObjectApproxC3D::ObjectApproxC3D()
{
	flagReady	= false;
	Name		= L"Name";
	objMath		= new GeomObjectApprox();
	objSS		= new SceneSegment();
}

ObjectApproxC3D::~ObjectApproxC3D()
{
	delete objMath;
}