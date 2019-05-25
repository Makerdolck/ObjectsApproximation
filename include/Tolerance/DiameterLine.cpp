#include "stdafx.h"
#include "DiameterLine.h"



DiameterLine::DiameterLine(CircleApprox* circle)
{
	this->centerPoint = centerByPoints(&circle->PointsForApprox[0], circle->PointsForApprox.size());
	this->objMath = circle;
	this->diameter = circle->Radius * 2;
}

DiameterLine::DiameterLine(CylinderApprox* cylinder)
{
	this->centerPoint = cylinder->PointBottomSurfaceCenter;
	this->objMath = cylinder;
	this->diameter = cylinder->Radius * 2;
}


DiameterLine::~DiameterLine()
{
}

ToleranceObject::ToleranceObjectType DiameterLine::getType()
{
	return ToleranceObjectType::DIAMETER_LINE;
}
