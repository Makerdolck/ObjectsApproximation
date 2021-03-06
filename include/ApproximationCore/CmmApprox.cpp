#include "stdafx.h"

#include "CmmApprox.h"

// ---																										// Constructors
CmmApprox::CmmApprox()
{
	CompensationValue		= 0.f;
	flagConnected			= false;

	// --- Load Digitizer's Library
	hLibrary = LoadLibraryW(L"CMM.dll");
	
	if (hLibrary == NULL)
	{
		MessageBoxW(NULL, L"���������� CMM.dll �� �������.", L"������", MB_OK + MB_ICONERROR);
		return;
	}

	Init		= (INIT *)::GetProcAddress(hLibrary, "Init");
	GetPoint	= (GETPOINT *)::GetProcAddress(hLibrary, "GetPoint");
	DisConnect	= (DISCONNECT *)::GetProcAddress(hLibrary, "DisConnect");
	Move		= (MOVE*)::GetProcAddress(hLibrary, "Move");
	Home		= (HOME*)::GetProcAddress(hLibrary, "Home");
}

CmmApprox::~CmmApprox()
{
	PointsForApprox.clear();

	Disconect_();
}
// ---																										// Connect
bool CmmApprox::Connect_()
{
	if (hLibrary == NULL)
		return false;

	flagConnected = (*Init)();

	return flagConnected;
}
// ---																										// Get Point
void CmmApprox::GetPoint_(double *X, double *Y, double *Z, short *STATUS)
{
	if (!flagConnected || hLibrary == NULL)
		return;
	
	float	b	= 0;
	bool	r	= 0;
	double	w	= 0;

	(*GetPoint)(X, Y, Z, &b, r, r, r, STATUS, &w, &w, &w);
}
// ---																										// Disconnect
void CmmApprox::Disconect_()
{
	if (!flagConnected || hLibrary == NULL)
		return;

	(*DisConnect)();

	flagConnected = false;
}
void CmmApprox::Move_(double *X, double *Y, double *Z, BOOL bTouch)
{
	if (!flagConnected || hLibrary == NULL)
		return;

	(*Move)(X, Y, Z, bTouch);
}
void CmmApprox::Home_()
{
	if (!flagConnected || hLibrary == NULL)
		return;

	(*Home)();
}