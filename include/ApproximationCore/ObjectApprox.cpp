#include "stdafx.h"

#include "ObjectApprox.h"


// ---																										// Constructors
ObjectApprox::ObjectApprox()
{
	flagReady		= false;
	flagSelected	= false;
	Name			= L"Name";
	objMath			= new GeomObjectApprox();
	objID			= 1;
}

ObjectApprox::~ObjectApprox()
{
	delete objMath;
}