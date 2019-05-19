#include "stdafx.h"
#include "DiameterLine.h"



DiameterLine::DiameterLine(CircleApprox* circle, bool isOutdoor = false)
{
	this->centerPoint = centerByPoints(&circle->PointsForApprox[0], circle->PointsForApprox.size());
	this->objMath = circle;
	this->isOutdoor = isOutdoor;
	this->diameter = circle->Radius * 2;
}

DiameterLine::DiameterLine(CylinderApprox* cylinder, bool isOutdoor)
{
	this->centerPoint = cylinder->PointBottomSurfaceCenter;
	this->objMath = cylinder;
	this->isOutdoor = isOutdoor;
	this->diameter = cylinder->Radius * 2;
}


DiameterLine::~DiameterLine()
{
}

ToleranceObject::ToleranceObjectType DiameterLine::getType()
{
	return ToleranceObjectType::DIAMETER_LINE;
}
