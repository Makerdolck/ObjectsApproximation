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
}

AxialLine::AxialLine(CylinderApprox* obj)
{
	objMath = obj;
	//centerPoint = (centerByPoints(&obj->pointsBottomCircleEdge_Copy[0], obj->pointsBottomCircleEdge_Copy.size()) + 
	//	centerByPoints(&obj->pointsTopCircleEdge_Copy[0], obj->pointsTopCircleEdge_Copy.size())) / 2;
	centerPoint = (obj->PointBottomSurfaceCenter + obj->PointTopSurfaceCenter) / 2;
	dirVector = obj->Line.Vector;
	offset = obj->Height;
}

AxialLine::AxialLine(ConeApprox* obj)
{
	objMath = obj;
	/*centerPoint = (centerByPoints(&obj->pointsBottomCircleEdge_Copy[0], obj->pointsBottomCircleEdge_Copy.size()) +
		centerByPoints(&obj->pointsTopCircleEdge_Copy[0], obj->pointsTopCircleEdge_Copy.size())) / 2;
		*/
	centerPoint = (obj->PointBottomSurfaceCenter + obj->PointTopSurfaceCenter) / 2;
	dirVector = obj->Line.Vector;
	offset = obj->Height;
}


AxialLine::~AxialLine()
{
}

ToleranceObject::ToleranceObjectType AxialLine::getType()
{
	return ToleranceObjectType::AXIAL_LINE;
}

