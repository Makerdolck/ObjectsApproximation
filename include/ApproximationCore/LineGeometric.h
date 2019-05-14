#pragma once

#ifndef __LINE_GEOMETRIC_H__
#define __LINE_GEOMETRIC_H__

#include "VectorGeometric.h"

class LineGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	PointGeometric	Point;
	VectorGeometric	Vector;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	LineGeometric();
	LineGeometric(VectorGeometric vector);
	LineGeometric(PointGeometric point, VectorGeometric vector);
	~LineGeometric();

	void			Normalize();

	PointGeometric	CreatePointOnDistance(double distance, bool flagPositiveDirection = true);
	PointGeometric	PointProjection(PointGeometric point);
	double			DistanceToPoint(PointGeometric point);

	PointGeometric  PointBetweenLineMiddle(PointGeometric point);
	int				LineBetweenLine(LineGeometric Line2, PointGeometric* pointout);
};

#endif /* __LINE_GEOMETRIC_H__ */