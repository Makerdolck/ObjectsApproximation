#include "stdafx.h"

#include "SizeLine.h"


SizeLine::SizeLine(ObjectApprox* obj1, ObjectApprox* obj2)
{
	this->PointStart = obj1->objMath->Line.Point;
	this->PointEnd = obj2->objMath->Line.Point;
	objMath = obj1->objMath;
	objMath2 = obj2->objMath;
}

SizeLine::SizeLine(PointApprox *point1, PointApprox *point2)
{
	this->PointStart = *point1;
	this->PointEnd = *point2;
}

SizeLine::SizeLine(LineSegmentApprox* lineSegment)
{
	
	objMath = lineSegment;
	this->PointStart = lineSegment->PointStart;
	this->PointEnd = lineSegment->PointEnd;

}

SizeLine::SizeLine(CylinderApprox* cylinder)
{
	PointPosition = PointGeometric(centerByPoints(new PointGeometric[2]{ PointStart, PointEnd }, 2));
	objMath = cylinder;
	this->PointStart = cylinder->PointBottomSurfaceCenter;
	this->PointEnd = cylinder->PointTopSurfaceCenter;
}

SizeLine::SizeLine(ConeApprox* cone)
{
	objMath = cone;
	this->PointStart = cone->PointBottomSurfaceCenter;
	this->PointEnd = cone->PointTopSurfaceCenter;
}



SizeLine::~SizeLine()
{
}


ToleranceObject::ToleranceObjectType SizeLine::getType()
{
	return SIZE_LINE;
}

double SizeLine::length()
{
	return sqrt(pow(PointEnd.X-PointStart.X, 2)+pow(PointEnd.Y-PointStart.Y, 2)+pow(PointEnd.Z-PointStart.Z, 2));
}

