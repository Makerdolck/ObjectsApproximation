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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "DialogMeasure.h"
#include "DialogObjectProperties.h"
#include <Tolerance/Tolerance.h>

class CMainFrame : public CFrameWndEx
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	CWinThread				*pThread;

	CmmApprox				*pCMM;

	DialogMeasure			*dlgMeasure;

	CMFCKioView				*pView;

	DataTreeView			*pDataTree;

	Tolerance				*pTolerance;
//	--- --- --- --- --- --- --- --- ---	// Methods
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	CSplitterWnd m_wndSplitter;
// Attributes
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  
	CMFCRibbonBar				m_wndRibbonBar;// control bar embedded members
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages			m_PanelImages;
	CMFCRibbonStatusBar			m_wndStatusBar;

// Generated message map functions
protected:
	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnZoomToExtents();
	afx_msg void OnButtonMeasurePoint();
	afx_msg void OnButtonMeasureLine();
	afx_msg void OnButtonMeasurePlane();
	afx_msg void OnButtonMeasureCircle();
	afx_msg void OnButtonMeasureCylinder();
	afx_msg void OnButtonMeasureCone();
	afx_msg void OnButtonMeasureSphere();
	afx_msg void OnButtonSettings();
	afx_msg void OnButtonMachine();
	afx_msg void OnButtonIntersectionPoint();
	afx_msg void OnButtonIntersectionLine();
	afx_msg void OnButtonIntersectionCircle();
	afx_msg void OnButtonIntersectionMeddlePoint();

	// Voronov
	afx_msg void OnToleranceLineSize();
	afx_msg void OnToleranceDiameterLine();
	afx_msg void OnToleranceAxialLine();
	afx_msg void OnToleranceBase();
	afx_msg void OnToleranceFormStraightness();
	afx_msg void OnToleranceFormFlatness();
	afx_msg void OnToleranceFormRoundess();
	afx_msg void OnToleranceFormCylindricity();

	afx_msg void OnToleranceOrientationParallelism();
	afx_msg void OnToleranceOrientationAngularity();
	afx_msg void OnToleranceOrientationPerpendicularity();


	afx_msg void OnToleranceCreateOrientation();
	afx_msg
		void OnAddObjectTest(CString filename, MeasureTypesEnum::MeasureTypes type);
	void OnVideo1();
	void OnVideo2();
	void OnVideo3();
	void OnVideo4();
	void OnVideo5();
	void OnVideo6();
	void OnVideo7();
	void OnVideo8();
	void OnToleranceCreateLocation();
	afx_msg void OnToleranceLocationConcentricity();
	afx_msg void OnToleranceLocationCoaxiality();
	afx_msg void OnToleranceLocationPosition();
	afx_msg void OnToleranceLocationSymmetry();



	afx_msg void OnToleranceRunOutRadial(); // Радиальное
	afx_msg void OnToleranceRunOutFace(); // Торцевое
};


// ---														// Scanning
UINT __cdecl Thread_Scanning(LPVOID pParam);