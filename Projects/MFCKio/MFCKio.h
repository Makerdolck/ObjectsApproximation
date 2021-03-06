// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MFCKio.h : main header file for the MFCKio application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif



#include "resource.h"       // main symbols

#include <ApproximationCore/_ALLincludesCore.h>

#define		WM_POINT_APPROX_READY		WM_USER + 5			// Point for approximation is adding to array
#define		WM_MATH_OBJECT_READY		WM_USER + 6			// Math object is ready to visualization
#define		WM_FULL_OBJECT_READY		WM_USER + 7			// Full object is ready to be added to the Tree

// CMFCKioApp:
// See MFCKio.cpp for the implementation of this class
//

class CMFCKioApp : public CWinAppEx
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	enum MeasureTypesEnum
	{
		MTpoint = 0,
		MTlineSegment,
		MTcircle,
		MTplane,
		MTcylinder,
		MTcone,
		MTsphere
	};

	CmmApprox			*pCMM;
	
	int					MTActive,
						CoordinatesAccuracy,
						CornersAccuracy;

	long int			nextObjectID;

	float				AccuracyApprox,
						TriangulationINAccuracy;

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CMFCKioApp();
	~CMFCKioApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	//UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

//	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCKioApp theApp;
