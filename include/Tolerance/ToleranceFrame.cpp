#include "stdafx.h"

#include "ToleranceFrame.h"


ToleranceFrame::ToleranceFrame()
{
}

ToleranceFrame::ToleranceFrame(ObjectApprox* controlObject, int toleranceName, double toleranceValue)
{
	this->objMath = controlObject->objMath;
	this->toleranceName = toleranceName;
	this->toleranceValue = toleranceValue;

	if (controlObject->objMath->GetName() == LineSegmentApprox().GetName()) {
		LineSegmentApprox tmp = *((LineSegmentApprox*)controlObject->objMath);
		this->PointStart = (tmp.PointStart + tmp.PointEnd)/2;
	}else if (controlObject->objMath->GetName() == CircleApprox().GetName()) {
		CircleApprox tmp = *((CircleApprox*)controlObject->objMath);
		this->PointStart = tmp.PointsForApprox.operator[](0);
	}else if (controlObject->objMath->GetName() == PlaneApprox().GetName()) {
		PlaneApprox tmp = *((PlaneApprox*)controlObject->objMath);
		this->PointStart = tmp.Line.Point;
	}else if (controlObject->objMath->GetName() == CylinderApprox().GetName()) {
		CylinderApprox tmp = *((CylinderApprox*)controlObject->objMath);
		this->PointStart = tmp.PointsForApprox.operator[](0);
	}else if (controlObject->objMath->GetName() == RectangleApprox().GetName()) {
		PlaneApprox tmp = *((PlaneApprox*)controlObject->objMath);
		this->PointStart = tmp.Line.Point;
	}

}


ToleranceFrame::ToleranceFrame(ToleranceBase* base, ObjectApprox* controlObject, int toleranceName, double toleranceValue)
{
	this->Base = base;
	this->objMath = controlObject->objMath;
	this->toleranceName = toleranceName;
	this->toleranceValue = toleranceValue;

	if (controlObject->objMath->GetName() == CylinderApprox().GetName()) {
		this->PointStart = ((CylinderApprox*)controlObject->objMath)->PointTopSurfaceCenter;
	}
	else if (controlObject->objMath->GetName() == CircleApprox().GetName()) {
		CircleApprox tmp = *((CircleApprox*)controlObject->objMath);
		this->PointStart = tmp.PointsForApprox.operator[](0);
	}else if (controlObject->objMath->GetName() == RectangleApprox().GetName()) {
		PlaneApprox tmp = *((PlaneApprox*)controlObject->objMath);
		this->PointStart = tmp.Line.Point;
	}else if (controlObject->objMath->GetName() == LineSegmentApprox().GetName()) {
		LineSegmentApprox tmp = *((LineSegmentApprox*)controlObject->objMath);
		this->PointStart = (tmp.PointStart - tmp.PointEnd) / 2;
	}
	
}


ToleranceFrame::~ToleranceFrame()
{
}

PointGeometric ToleranceFrame::getCenterBage()
{
	return PointGeometric(PointPosition.X - Width() + boxFirstSectionWidth / 2, PointPosition.Y + boxHeight / 2, PointPosition.Z);
}

PointGeometric ToleranceFrame::getCenterToleranceValue()
{
	return PointGeometric(PointPosition.X - boxFirstSectionWidth - boxSecondSectionWidth / 2, PointPosition.Y + boxHeight / 2, PointPosition.Z);
}

PointGeometric ToleranceFrame::getCenterToleranceBaseName()
{
	return PointGeometric(PointPosition.X - boxThirdSectionWidth / 2, PointPosition.Y + boxHeight / 2, PointPosition.Z);
}

void ToleranceFrame::setFrameSize(double firstSectionWidth, double secondSectionWidth, double thirdSectionWidth, double height)
{
	this->boxFirstSectionWidth = firstSectionWidth;
	this->boxSecondSectionWidth = secondSectionWidth;
	this->boxThirdSectionWidth = thirdSectionWidth;
	this->boxHeight = height;
}

double ToleranceFrame::Width()
{
	return boxFirstSectionWidth + boxSecondSectionWidth + boxThirdSectionWidth;;
}

ToleranceObject::ToleranceObjectType ToleranceFrame::getType()
{
	return ToleranceObjectType::TOLERANCE_FRAME;
}

void ToleranceFrame::changeBoxDirection()
{
	boxFirstSectionWidth = -boxFirstSectionWidth;
	boxSecondSectionWidth = -boxSecondSectionWidth;
	boxThirdSectionWidth = -boxThirdSectionWidth;
}
