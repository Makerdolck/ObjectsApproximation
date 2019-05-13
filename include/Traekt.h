#pragma once
#ifndef __TRAEKT_H__
#define __TRAEKT_H__

#include "ApproximationCore\GeomObjectApprox.h"
//class CmmApprox;
#include "ApproximationCore\CmmApprox.h"

struct StTr
{
	GeomObjectApprox* TrGeom;
	int TrStep;
	int TrPl;
	bool TrBoss;
	bool TrPart;
	CmmApprox* TrCmm;
};

UINT TraektCylinder(LPVOID Struct);
UINT TraektCone(LPVOID Struct);
UINT TraektSphere(LPVOID Struct);
UINT TraektPoint(LPVOID Struct);
UINT TraektLine(LPVOID Struct);
UINT TraektRectangle(LPVOID Struct);
UINT TraektCircle(LPVOID Struct);
#endif /* __TRAEKT_H__ */ 