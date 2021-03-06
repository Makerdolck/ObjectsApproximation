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

class CMainFrame : public CFrameWndEx
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	CWinThread				*pThread;

	CmmApprox				*pCMM;

	DialogMeasure			*dlgMeasure;

	CMFCKioView				*pView;

	DataTreeView			*pDataTree;

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
	afx_msg void OnButtonIntersectionPlane();
};


// ---														// Scanning
UINT __cdecl Thread_Scanning(LPVOID pParam);