#pragma once

#ifndef __CIRCLE_GEOMETRIC_H__
#define __CIRCLE_GEOMETRIC_H__

//#include "PointGeometric.h"
#include "LineGeometric.h"
//#include "..\\DVector\\Dvector.h"

class CircleGeometric
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	LineGeometric	Line;
	//PointGeometric	Point;

	double			Radius;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CircleGeometric();
	CircleGeometric(PointGeometric point1, PointGeometric point2, PointGeometric point3);
	~CircleGeometric();


};

#endif /* __CIRCLE_GEOMETRIC_H__ */ 