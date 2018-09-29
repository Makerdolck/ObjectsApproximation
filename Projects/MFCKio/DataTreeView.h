/****************************** Module Header ******************************\
* Module Name:  MyEditView.h
* Project:      MFCSDIAppln
* Copyright (c) Microsoft Corporation.
*
* This is an EditView.
*
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL
* All other rights reserved.
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/
#pragma once
#ifndef __DATA_TREE_VIEW_H__
#define __DATA_TREE_VIEW_H__

class CMFCKioView;
#include "MFCKioView.h"

// CMyEditView view

class DataTreeView : public CTreeView
{
	DECLARE_DYNCREATE(DataTreeView)

protected:
	DataTreeView();           // protected constructor used by dynamic creation
	
public:
	virtual ~DataTreeView();

public:								// Tree Item Root
	HTREEITEM		TIR_Points,
					TIR_LineSegments,
					TIR_Planes,
					TIR_Circles,
					TIR_Cylinders,
					TIR_Cones,
					TIR_Spheres;

	HTREEITEM		focused_TI;

	CTreeCtrl		*pCtrl;

	CMFCKioView	*pView;

	std::vector<ObjectApprox*>	*objectsArray;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnAddNewObjToTree(WPARAM wParam, LPARAM lParam);

	virtual void OnInitialUpdate();
	afx_msg void OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


#endif /* __DATA_TREE_VIEW_H__ */ 