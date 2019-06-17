#include "stdafx.h"

#include "SizeLine.h"


SizeLine::SizeLine(ObjectApprox* obj1, ObjectApprox* obj2)
{
	this->PointStart = obj1->objMath->Line.Point;
	this->PointEnd = obj2->objMath->Line.Point;
	objMath = obj1->objMath;
	objMath2 = obj2->objMath;
	if (objMath->GetName() == CylinderApprox().GetName() && objMath2->GetName() == CylinderApprox().GetName()) {
		CylinderApprox* cylinder1 = (CylinderApprox*)objMath;
		CylinderApprox* cylinder2 = (CylinderApprox*)objMath2;
		VectorGeometric obj2Axial = VectorGeometric(cylinder2->PointBottomSurfaceCenter, cylinder2->PointTopSurfaceCenter, false);
		PointStart = cylinder1->Line.Point;
		PointEnd = obj2Axial.PointProjection(PointStart, cylinder2->PointBottomSurfaceCenter);
	}
	if (objMath->GetName() == RectangleApprox().GetName() && objMath2->GetName() == RectangleApprox().GetName()) {
		RectangleApprox* rec1 = (RectangleApprox*)objMath;
		RectangleApprox* rec2 = (RectangleApprox*)objMath2;
		PointStart = rec1->Line.Point;
		PointEnd = rec2->Plane.PointProjection(PointStart);
	}
	if (objMath->GetName() == LineSegmentApprox().GetName() && objMath2->GetName() == LineSegmentApprox().GetName()) {
		LineSegmentApprox* line1 = (LineSegmentApprox*)objMath;
		LineSegmentApprox* line2 = (LineSegmentApprox*)objMath2;
		PointStart = line1->Line.Point;
		PointEnd = line2->PointProjection(PointStart);
	}
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

