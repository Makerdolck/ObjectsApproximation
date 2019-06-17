#include "stdafx.h"
#include "AxialLine.h"







AxialLine::AxialLine(CylinderApprox* obj)
{
	objMath = obj;
	VectorGeometric normalizedVector = VectorGeometric(obj->Line.Vector);
	normalizedVector.Normalize();
	startPoint = obj->PointBottomSurfaceCenter - obj->Line.Vector - (normalizedVector * 2) - (normalizedVector * obj->Height);
	endPoint = obj->PointTopSurfaceCenter + obj->Line.Vector + (normalizedVector * 2) + (normalizedVector * obj->Height);
}

AxialLine::AxialLine(ConeApprox* obj)
{
	objMath = obj;
	VectorGeometric normalizedVector = VectorGeometric(obj->Line.Vector);
	normalizedVector.Normalize();
	startPoint = obj->PointBottomSurfaceCenter - obj->Line.Vector - (normalizedVector * 5) - (normalizedVector * obj->Height);
	endPoint = obj->PointTopSurfaceCenter + obj->Line.Vector + (normalizedVector * 3) + (normalizedVector * obj->Height);
}


AxialLine::~AxialLine()
{
}

ToleranceObject::ToleranceObjectType AxialLine::getType()
{
	return ToleranceObjectType::AXIAL_LINE;
}

