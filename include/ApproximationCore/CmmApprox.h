#pragma once

#ifndef __CMM_APPROX_H__
#define __CMM_APPROX_H__

#include <windows.h>
#include <minwindef.h>

#include <vector>

#include "PointGeometric.h"

typedef BOOL(INIT)();
typedef BOOL* (GETPOINT)(double *x, double *y, double *z, float *b, bool &button1, bool &button2, bool &button3, short *stat, double *x_prev, double *y_prev, double *z_prev);
typedef BOOL* (DISCONNECT)();
typedef BOOL* (MOVE)(double *X, double *Y, double *Z, BOOL bTouch);
typedef BOOL* (HOME)();

class CmmApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	HINSTANCE						hLibrary;								// Digitizer Library

	INIT							*Init;
	GETPOINT						*GetPoint;
	DISCONNECT						*DisConnect;
	MOVE							*Move;
	HOME							*Home;

public:
	PointGeometric					PointCurrent;
	std::vector<PointGeometric>		PointsForApprox;
	
	float							CompensationValue;

	bool							flagConnected;


//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CmmApprox();
	~CmmApprox();

	bool			Connect_();
	void			GetPoint_(double *X, double *Y, double *Z, short *STATUS);
	void			Disconect_();
	void			Move_(double *X, double *Y, double *Z, BOOL bTouch);
	void			Home_();
};

#endif /* __CMM_APPROX_H__ */ 