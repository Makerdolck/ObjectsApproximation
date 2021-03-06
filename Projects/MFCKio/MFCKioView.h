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

// MFCKioView.h : interface of the CMFCKioView class
//
#pragma once
#ifndef __MFC_KIO_VIEW_H__
#define __MFC_KIO_VIEW_H__

#include "OpenGLView.h"
#include "MFCKioDoc.h"

#include "CAutoDlg.h"

class DialogMeasure;
#include "DialogMeasure.h"


class DataTreeView;
#include "DataTreeView.h"


class CMFCKioView : public COpenGLView
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	double						radiusOfAllObjects;

public:
	DataTreeView				*pDataTree;

	DialogMeasure				*dlgMeasure;

	int							NameNumb_Point,
								NameNumb_LineSegment,
								NameNumb_Plane,
								NameNumb_Circle,
								NameNumb_Cylinder,
								NameNumb_Cone,
								NameNumb_Sphere;

	//	--- --- --- --- --- --- --- --- ---	// Methods
protected: // create from serialization only
	CMFCKioView();
	DECLARE_DYNCREATE(CMFCKioView)

	// Attributes
public:
	CMFCKioDoc * GetDocument() const;

// Operations
public:
	bool		RemoveObject(ObjectApprox *object);
	void		RecalculateCenterOfAllObjects();

// Overrides
public:

protected:

// Implementation
public:
	virtual ~CMFCKioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	FocusingOn(ObjectApprox* obj = nullptr);

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNewObjectForVisualize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnEditAutomeasuring();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in MFCKioView.cpp
inline CMFCKioDoc* CMFCKioView::GetDocument() const
   { return reinterpret_cast<CMFCKioDoc*>(m_pDocument); }
#endif

#endif /* __MFC_KIO_VIEW_H__ */ 
