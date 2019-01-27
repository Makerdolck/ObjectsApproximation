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

// MFCAsconView.h : interface of the CMFCAsconView class
//
#pragma once
#ifndef __MFC_ASCON_VIEW_H__
#define __MFC_ASCON_VIEW_H__

#include "OpenGLView.h"
#include "MFCAsconDoc.h"

class DataTreeView;
#include "DataTreeView.h"


class CMFCAsconView : public COpenGLView
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	

public:
	//std::vector<ObjectApprox*>	*objectsArray;

	DataTreeView				*pDataTree;

	int							NameNumb_Point,
								NameNumb_LineSegment,
								NameNumb_Plane,
								NameNumb_Circle,
								NameNumb_Cylinder,
								NameNumb_Cone,
								NameNumb_Sphere;

	//	--- --- --- --- --- --- --- --- ---	// Methods
protected: // create from serialization only
	CMFCAsconView();
	DECLARE_DYNCREATE(CMFCAsconView)

	// Attributes
public:
	CMFCAsconDoc * GetDocument() const;

// Operations
public:
	bool		RemoveObject(ObjectApprox *object/*std::wstring objectName*/);

// Overrides
public:

protected:

// Implementation
public:
	virtual ~CMFCAsconView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//afx_msg void OnFilePrintPreview();
	//afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNewObjectForVisualize(WPARAM wParam, LPARAM lParam);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in MFCAsconView.cpp
inline CMFCAsconDoc* CMFCAsconView::GetDocument() const
   { return reinterpret_cast<CMFCAsconDoc*>(m_pDocument); }
#endif

#endif /* __MFC_ASCON_VIEW_H__ */ 