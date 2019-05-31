#include "stdafx.h"
#include "AxialLine.h"


AxialLine::AxialLine()
{
}

AxialLine::AxialLine(CircleApprox* obj)
{
	objMath = obj;
	centerPoint = centerByPoints(&obj->PointsForApprox[0], obj->PointsForApprox.size());
	dirVector = obj->Line.Vector;
	offset = 0;
	VectorGeometric normalizedVector = VectorGeometric(dirVector);
	normalizedVector.Normalize();
	startPoint = centerPoint - dirVector - (normalizedVector * 5) - (normalizedVector * offset);
	endPoint = centerPoint + dirVector + (normalizedVector * 3) + (normalizedVector * offset);
}

AxialLine::AxialLine(CylinderApprox* obj)
{
	objMath = obj;
	startPoint = obj->PointBottomSurfaceCenter;
	endPoint = obj->PointTopSurfaceCenter;

	centerPoint = (obj->PointBottomSurfaceCenter + obj->PointTopSurfaceCenter) / 2;
	dirVector = obj->Line.Vector;
	offset = obj->Height;

	VectorGeometric normalizedVector = VectorGeometric(dirVector);
	normalizedVector.Normalize();
	startPoint = obj->PointBottomSurfaceCenter - dirVector - (normalizedVector * 2) - (normalizedVector * offset);
	endPoint = obj->PointTopSurfaceCenter + dirVector + (normalizedVector * 2) + (normalizedVector * offset);
}

AxialLine::AxialLine(ConeApprox* obj)
{
	objMath = obj;
	centerPoint = (obj->PointBottomSurfaceCenter + obj->PointTopSurfaceCenter) / 2;
	dirVector = obj->Line.Vector;
	offset = obj->Height;

	VectorGeometric normalizedVector = VectorGeometric(dirVector);
	normalizedVector.Normalize();
	startPoint = obj->PointBottomSurfaceCenter - dirVector - (normalizedVector * 5) - (normalizedVector * offset);
	endPoint = obj->PointTopSurfaceCenter + dirVector + (normalizedVector * 3) + (normalizedVector * offset);
}


AxialLine::~AxialLine()
{
}

ToleranceObject::ToleranceObjectType AxialLine::getType()
{
	return ToleranceObjectType::AXIAL_LINE;
}

