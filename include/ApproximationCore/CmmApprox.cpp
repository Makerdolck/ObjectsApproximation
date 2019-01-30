#include "stdafx.h"

#include "CmmApprox.h"

#include <string>

// ---																										// Constructors
CmmApprox::CmmApprox()
{
	PortNumber		= 0;
	MachineAccuracy = 3;
	flagConnected	= false;

	// --- Load Digitizer Library
	hLibrary = LoadLibraryW(L"CMM.dll");
	
	if (hLibrary == NULL)
	{
		MessageBoxW(NULL, L"The Cmm.dll library is not found", L"Error", MB_OK + MB_ICONERROR);
		return;
	}

	Init		= (INIT *)::GetProcAddress(hLibrary, "Init");
	GetPoint	= (GETPOINT *)::GetProcAddress(hLibrary, "GetPoint");
	DisConnect	= (DISCONNECT *)::GetProcAddress(hLibrary, "DisConnect");
}

CmmApprox::~CmmApprox()
{
	Disconect_();

	PointsForApprox.clear();
}
// ---																										// Connect
bool CmmApprox::Connect_()
{
	if (hLibrary == NULL)
		return false;
	/*std::wstring PortStr = L"COM";
	PortStr.append(std::to_wstring(PortNumber));

	flagConnected = (*Init)((char*)PortStr.c_str());*/
	//flagConnected = (*Init)((char*)"UCC.ini");

	//char* connectString;
	//connectString = "C:\\Program Files\\Renishaw\\UCC\\Machine\\ucc\\ucc.ini";
	//flagConnected = (*Init)(connectString);

	name_t filename = "C:\\Program Files\\Renishaw\\UCC\\Machine\\ucc\\ucc.ini";
	flagConnected = (*Init)(filename);

	//flagConnected = (*Init)((char*)"C:\\Program Files\\Renishaw\\UCC\\Machine\\ucc\\ucc.ini"); 
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
}