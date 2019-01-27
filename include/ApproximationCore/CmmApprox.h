#pragma once

#ifndef __CMM_APPROX_H__
#define __CMM_APPROX_H__

#include <windows.h>
#include <minwindef.h>

#include <vector>

#include "PointGeometric.h"

typedef BOOL(INIT)(char[5]/*, char **/);
//typedef BOOL* (GETPOINT)(double*, double*, double*, short*);
typedef BOOL* (GETPOINT)(double *x, double *y, double *z, float *b, bool &button1, bool &button2, bool &button3, short *stat, double *x_prev, double *y_prev, double *z_prev);
typedef BOOL* (DISCONNECT)();

class CmmApprox
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	HINSTANCE						hLibrary;								// Digitizer Library

	INIT							*Init;
	GETPOINT						*GetPoint;
	DISCONNECT						*DisConnect;

public:
	PointGeometric					PointCurrent;
	std::vector<PointGeometric>		PointsForApprox;
	
	int								PortNumber,
									MachineAccuracy;

	bool							flagConnected;


//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CmmApprox();
	~CmmApprox();

	bool			Connect_();
	void			GetPoint_(double *X, double *Y, double *Z, short *STATUS);
	void			Disconect_();
};

#endif /* __CMM_APPROX_H__ */ 