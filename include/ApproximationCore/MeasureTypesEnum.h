#pragma once

#ifndef __MEASURE_TYPES_ENUM_H__
#define __MEASURE_TYPES_ENUM_H__


class MeasureTypesEnum
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	enum MeasureTypes
	{
		point = 0,
		lineSegment,
		circle,
		plane,
		cylinder,
		cone,
		sphere
	};

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	MeasureTypesEnum();
	~MeasureTypesEnum();

};

#endif /* __MEASURE_TYPES_ENUM_H__ */ 