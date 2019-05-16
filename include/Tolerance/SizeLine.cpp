#include "stdafx.h"

#include "SizeLine.h"


SizeLine::SizeLine()
{
	//objectApproxName = (char*)"sizeLine";
}


SizeLine::SizeLine(LineSegmentApprox* lineSegment)
{
	SizeLine();
	objMath = lineSegment;
	this->PointStart = lineSegment->PointStart;
	this->PointEnd = lineSegment->PointEnd;

}

SizeLine::SizeLine(CylinderApprox* cylinder)
{
	SizeLine();
	objMath = cylinder;
	
	/*PointGeometric bottomCenterPoint = centerByPoints(&cylinder->pointsBottomCircleEdge_Copy[0], cylinder->pointsBottomCircleEdge_Copy.size());
	PointGeometric topCenterPoint = centerByPoints(&cylinder->pointsTopCircleEdge_Copy[0], cylinder->pointsTopCircleEdge_Copy.size());
	this->PointStart = bottomCenterPoint;
	this->PointEnd = topCenterPoint;
	*/
	this->PointStart = cylinder->PointBottomSurfaceCenter;
	this->PointEnd = cylinder->PointTopSurfaceCenter;
	offset = cylinder->Radius;
}

SizeLine::SizeLine(ConeApprox* cone)
{
	SizeLine();
	objMath = cone;
//	PointGeometric bottomCenterPoint = centerByPoints(&cone->pointsBottomCircleEdge_Copy[0], cone->pointsBottomCircleEdge_Copy.size());
	//PointGeometric topCenterPoint = centerByPoints(&cone->pointsTopCircleEdge_Copy[0], cone->pointsTopCircleEdge_Copy.size());
//	this->PointStart = bottomCenterPoint;
	//this->PointEnd = topCenterPoint;
	this->PointStart = cone->PointBottomSurfaceCenter;
	this->PointEnd = cone->PointTopSurfaceCenter;
	offset = cone->Radius;

}



SizeLine::~SizeLine()
{
	
}

void SizeLine::paint()
{
	
}

ToleranceObject::ToleranceObjectType SizeLine::getType()
{
	return SIZE_LINE;
}

