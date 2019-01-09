#pragma once

#ifndef __LINE_SEGMENT_APPROX_H__
#define __LINE_SEGMENT_APPROX_H__

#include "GeomObjectApprox.h"


class LineSegmentApprox : public GeomObjectApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	PointGeometric	Point,
					PointStart,
					PointEnd;
	VectorGeometric Vector;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	LineSegmentApprox();
	~LineSegmentApprox();

	double			DistanceToPoint(PointGeometric point);
	PointGeometric	PointProjection(PointGeometric point);

	void			FindByPoints(PointGeometric *points, int arraySize, double accuracy);

private:
	double			FunctionApprox(PointGeometric *points, int arraySize);
};

#endif /* __LINE_SEGMENT_APPROX_H__ */ 