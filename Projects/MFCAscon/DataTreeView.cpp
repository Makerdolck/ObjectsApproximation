/****************************** Module Header ******************************\
* Module Name:  MyEditView.cpp
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

#include "stdafx.h"
#include "MFCAscon.h"
#include "DataTreeView.h"

#include "DialogObjectProperties.h"


// CMyEditView

IMPLEMENT_DYNCREATE(DataTreeView, CTreeView)

DataTreeView::DataTreeView()
{
	pCtrl = &GetTreeCtrl();
}

DataTreeView::~DataTreeView()
{
}

BEGIN_MESSAGE_MAP(DataTreeView, CTreeView)
	ON_MESSAGE(WM_FULL_OBJECT_READY,	&DataTreeView::OnAddNewObjToTree)

	ON_NOTIFY_REFLECT(TVN_SELCHANGED,	&DataTreeView::OnSelChanged)
	ON_WM_KEYUP()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &DataTreeView::OnNMDblclk)
END_MESSAGE_MAP()


// CMyEditView diagnostics

#ifdef _DEBUG
void DataTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void DataTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																										// On Initial Update
void DataTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	pCtrl->ModifyStyle(0, TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT); //TVS_CHECKBOXES

	TIR_Points			= pCtrl->InsertItem(L"Точки");
	pCtrl->SetItemData(TIR_Points,		(DWORD)nullptr);

	TIR_LineSegments	= pCtrl->InsertItem(L"Лини");
	pCtrl->SetItemData(TIR_LineSegments,(DWORD)nullptr);

	TIR_Planes			= pCtrl->InsertItem(L"Плоскости");
	pCtrl->SetItemData(TIR_Planes,		(DWORD)nullptr);

	TIR_Circles			= pCtrl->InsertItem(L"Окружности");
	pCtrl->SetItemData(TIR_Circles,		(DWORD)nullptr);

	TIR_Cylinders		= pCtrl->InsertItem(L"Цилиндры");
	pCtrl->SetItemData(TIR_Cylinders,	(DWORD)nullptr);

	TIR_Cones			= pCtrl->InsertItem(L"Конусы");
	pCtrl->SetItemData(TIR_Cones,		(DWORD)nullptr);

	TIR_Spheres			= pCtrl->InsertItem(L"Сферы");
	pCtrl->SetItemData(TIR_Spheres,		(DWORD)nullptr);
}
// ---																										// On Change Selection
void DataTreeView::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	focused_TI = pNMTreeView->itemNew.hItem;

	*pResult = 0;
}
// ---																										// On Push any keybord Button
void DataTreeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
	{		
		if (pView->RemoveObject((ObjectApprox*)pCtrl->GetItemData(focused_TI)))
			pCtrl->DeleteItem(focused_TI);
	}
	if (nChar == 'F')		// 'F' is pressed
	{
		pView->FocusedOn((ObjectApprox*)pCtrl->GetItemData(focused_TI));
			
	}

	CTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
}
// ---																										// On Look Object Information
void DataTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	if ((ObjectApprox*)pCtrl->GetItemData(focused_TI) != nullptr)
	{
		DialogObjectProperties dlg(pView, (ObjectApprox*)pCtrl->GetItemData(focused_TI));
		dlg.DoModal();
	}
	
	*pResult = 0;
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

// ---																					// On New Object for Visualize is coming
LRESULT DataTreeView::OnAddNewObjToTree(WPARAM wParam, LPARAM lParam)
{
	ObjectApprox			*object;

	if (objectsArray->back()->flagReady == true)
		object = objectsArray->back();
	else if (objectsArray->size() > 1)
		object = objectsArray->operator[](objectsArray->size() - 2);

	HTREEITEM			treeItem;


	switch (theApp.MTActive)
	{
	case theApp.MTContainer.point:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Points);

		pCtrl->Expand(TIR_Points, TVE_EXPAND);

		break;
	case theApp.MTContainer.lineSegment:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_LineSegments);

		pCtrl->Expand(TIR_LineSegments, TVE_EXPAND);

		break;
	case theApp.MTContainer.plane:
	
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Planes);

		pCtrl->Expand(TIR_Planes, TVE_EXPAND);

		break;
	case theApp.MTContainer.circle:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Circles);

		pCtrl->Expand(TIR_Circles, TVE_EXPAND);

		break;
	case theApp.MTContainer.cylinder:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Cylinders);

		pCtrl->Expand(TIR_Cylinders, TVE_EXPAND);

		break;
	case theApp.MTContainer.cone:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Cones);

		pCtrl->Expand(TIR_Cones, TVE_EXPAND);

		break;
	case theApp.MTContainer.sphere:
		
		treeItem = pCtrl->InsertItem(object->Name.c_str(), TIR_Spheres);

		pCtrl->Expand(TIR_Spheres, TVE_EXPAND);

		break;
	default:
		break;
	}

	//pCtrl->Expand(TIR_Points, TVE_EXPAND);
	//pCtrl->Expand(TIR_Points, TVE_COLLAPSE);

	pCtrl->SetItemData(treeItem, (DWORD64)object);

	pCtrl->Invalidate();
	
	//pCtrl->SetItemState(treeItem, TVIS_SELECTED, TVIS_SELECTED);

	return 0L;
}

