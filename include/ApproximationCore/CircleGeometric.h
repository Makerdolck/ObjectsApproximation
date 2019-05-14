#pragma once

#ifndef __CIRCLE_GEOMETRIC_H__
#define __CIRCLE_GEOMETRIC_H__

#include "LineGeometric.h"

class CircleGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	LineGeometric	Line;

	double			Radius;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CircleGeometric();
	CircleGeometric(LineGeometric line, double radius = 0);
	CircleGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3);
	~CircleGeometric();

	int				LineIntersection(LineGeometric line, PointGeometric* point1, PointGeometric* point2);
	int				CircleIntersection(CircleGeometric Circle2, PointGeometric* point1, PointGeometric* point2);
	PointGeometric	PointIntersectionMiddle(PointGeometric pointOut);
};

#endif /* __CIRCLE_GEOMETRIC_H__ */ 