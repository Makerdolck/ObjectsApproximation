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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DataTreeView.h"
#include "DialogSettings.h"
#include "DialogMachine.h"

#include <Intersection.h>

#ifndef CIRCLE_APPROX_H
#define CIRCLE_APPROX_H

#include "MFCKioView.h"

#endif

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7,			&CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7,	&CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_ZOOM_TO_EXTENTS,				&CMainFrame::OnZoomToExtents)
	ON_COMMAND(ID_BUTTON_MEASURE_POINT,			&CMainFrame::OnButtonMeasurePoint)
	ON_COMMAND(ID_BUTTON_MEASURE_LINE,			&CMainFrame::OnButtonMeasureLine)
	ON_COMMAND(ID_BUTTON_MEASURE_RECTANGLE,		&CMainFrame::OnButtonMeasurePlane)
	ON_COMMAND(ID_BUTTON_MEASURE_CIRCLE,		&CMainFrame::OnButtonMeasureCircle)
	ON_COMMAND(ID_BUTTON_MEASURE_CYLINDER,		&CMainFrame::OnButtonMeasureCylinder)
	ON_COMMAND(ID_BUTTON_MEASURE_CONE,			&CMainFrame::OnButtonMeasureCone)
	ON_COMMAND(ID_BUTTON_MEASURE_SPHERE,		&CMainFrame::OnButtonMeasureSphere)
	ON_COMMAND(ID_BUTTON_SETTINGS,				&CMainFrame::OnButtonSettings)
	ON_COMMAND(ID_BUTTON_MACHINE,				&CMainFrame::OnButtonMachine)
	ON_COMMAND(ID_BUTTON_INTERSECTION_POINT, &CMainFrame::OnButtonIntersectionPoint)
	ON_COMMAND(ID_BUTTON_INTERSECTION_LINE, &CMainFrame::OnButtonIntersectionLine)
	ON_COMMAND(ID_BUTTON_INTERSECTION_CIRCLE, &CMainFrame::OnButtonIntersectionCircle)
	ON_COMMAND(ID_BUTTON_INTERSECTION_MEDDLE_POINT, &CMainFrame::OnButtonIntersectionMeddlePoint)
	ON_COMMAND(ID_BUTTON_INTERSECTION_PLANE, &CMainFrame::OnButtonIntersectionPlane)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	//theApp.m_nAppLook	= theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	//	---	---	---	---	---	---	---	---	---	---	---
	
	pCMM				= theApp.pCMM;

	pDataTree			= nullptr;
	pView				= nullptr;
	
	dlgMeasure			= new DialogMeasure(this);
	pThread				= AfxBeginThread(Thread_Scanning, this);
}

CMainFrame::~CMainFrame()
{
	//flagCloseThread = false;
	//TerminateThread(pThread->m_hThread, 0);
	//Sleep(1000 * 2);
	TerminateThread(pThread->m_hThread, 0);
	delete pThread;
	delete dlgMeasure;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);
	
	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	
	CDockablePane* pTabbedBar = nullptr;

	// set the visual manager and style based on persisted value
	//OnApplicationLook(theApp.m_nAppLook);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	//theApp.m_nAppLook = id;

	/*switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}*/


	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	//theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	//pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

void CMainFrame::OnZoomToExtents()
{
	pView->FocusingOn();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(DataTreeView), CSize(200, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMFCKioView), CSize(10, 10), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	//	---		---		---		---		---

	pDataTree	= (DataTreeView*)m_wndSplitter.GetPane(0, 0);
	pView		= (CMFCKioView*)m_wndSplitter.GetPane(0, 1);
	
	
	dlgMeasure->pView			= pView;
	pView->pDataTree			= pDataTree;
	pDataTree->pView			= pView;
	pView->dlgMeasure			= dlgMeasure;
	

	dlgMeasure->objectsArray			= &((CMFCKioDoc*)pView->GetDocument())->objectsArray;
	pDataTree->objectsArray				= &((CMFCKioDoc*)pView->GetDocument())->objectsArray;

	return TRUE;
}

// ---																					// Measure Point
void CMainFrame::OnButtonMeasurePoint()
{
	theApp.MTActive = theApp.MTpoint;

	dlgMeasure->DoModal();
}
// ---																					// Measure Line
void CMainFrame::OnButtonMeasureLine()
{
	theApp.MTActive = theApp.MTlineSegment;

	dlgMeasure->DoModal();
}
// ---																					// Measure Plane
void CMainFrame::OnButtonMeasurePlane()
{
	theApp.MTActive = theApp.MTplane;

	dlgMeasure->DoModal();
}
// ---																					// Measure Circle
void CMainFrame::OnButtonMeasureCircle()
{
	theApp.MTActive = theApp.MTcircle;

	dlgMeasure->DoModal();
}
// ---																					// Measure Cylinder
void CMainFrame::OnButtonMeasureCylinder()
{
	theApp.MTActive = theApp.MTcylinder;

	dlgMeasure->DoModal();
}
// ---																					// Measure Cone
void CMainFrame::OnButtonMeasureCone()
{
	theApp.MTActive = theApp.MTcone;

	dlgMeasure->DoModal();
}
// ---																					// Measure Sphere
void CMainFrame::OnButtonMeasureSphere()
{
	theApp.MTActive = theApp.MTsphere;

	dlgMeasure->DoModal();
}
// ---																					// Open Settings
void CMainFrame::OnButtonSettings()
{
	DialogSettings dlg;

	if (dlg.DoModal() == IDOK)
	{
		pCMM->CompensationValue			= dlg.CompensationValue;
		theApp.CoordinatesAccuracy		= dlg.CoordinatesAccuracy;
		theApp.CornersAccuracy			= dlg.CornersAccuracy;

		if (theApp.TriangulationINAccuracy != dlg.TriangulationINAccuracy)
		{
			std::vector<ObjectApprox*>* objects = &((CMFCKioDoc*)pView->GetDocument())->objectsArray;
			
			theApp.TriangulationINAccuracy = dlg.TriangulationINAccuracy;

			for (int i = 0; i < (int)objects->size(); i++)
			{
				objects->operator[](i)->objMath->Triangulation(theApp.TriangulationINAccuracy);
			}

			pView->Invalidate(FALSE);
		}		
	}
}
// ---																					// Open Machine
void CMainFrame::OnButtonMachine()
{
	DialogMachine dlg;

	dlg.DoModal();
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Intersection
//--------------------------------------------------------------

// ---																					// Intersection Point
void CMainFrame::OnButtonIntersectionPoint()
{
	Intersection tmp(pDataTree, pView, &theApp.nextObjectID);
	int message = 0;
	message = tmp.InPlanePoint(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InTwoPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InSpherePlane(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InPlaneLine(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InPlaneCircle(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InCylinderCircle(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoCylinder(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InSpherePoint(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoLine(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InCylinderLine(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InSphereCircle(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InSphereLine(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InCircleLine(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoCircle(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InCirclePoint(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}
	message = tmp.InLinePoint(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}
	message = tmp.InCylinderPoint(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InConePoint(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}
	message = tmp.InConeCircle(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InSphereCylinder(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InSphereCone(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoCone(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InConeLine(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InCylinderCone(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}


	if (message == 1)
	{
		AfxMessageBox(L"Нет пересечения", MB_ICONWARNING | MB_OK);
		//MessageBoxW(NULL, L"Пересечение", MB_ICONWARNING | MB_OK);
	}
}
// ---																					// Intersection Line
void CMainFrame::OnButtonIntersectionLine()
{
	Intersection tmp(pDataTree, pView, &theApp.nextObjectID);
	int message = 0;
	message = tmp.InPlaneLineProjection(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InTwoPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InTwoPoint(dlgMeasure->objectsArray, 2);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	if (message == 1)
	{
		AfxMessageBox(L"Нет пересечения", MB_ICONWARNING | MB_OK);
		//MessageBoxW(NULL, L"Пересечение", MB_ICONWARNING | MB_OK);
	}
}
// ---																					// Intersection Circle
void CMainFrame::OnButtonIntersectionCircle()
{
	Intersection tmp(pDataTree, pView, &theApp.nextObjectID);
	int message = 0;

	message = tmp.InSpherePlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoSphere(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InConeAndPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InCylinderPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}


	if (message == 1)
	{
		AfxMessageBox(L"Нет пересечения", MB_ICONWARNING | MB_OK);
		//MessageBoxW(NULL, L"Пересечение", MB_ICONWARNING | MB_OK);
	}
}
// ---																					// Intersection Middle Point
void CMainFrame::OnButtonIntersectionMeddlePoint()
{
	Intersection tmp(pDataTree, pView, &theApp.nextObjectID);
	int message = 0;
	message = tmp.InLinePointMiddle(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InTwoPoint(dlgMeasure->objectsArray, 1);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InCirclePointMiddle(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}

	message = tmp.InPlanePointMiddle(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}


	if (message == 1)
	{
		AfxMessageBox(L"Нет пересечения", MB_ICONWARNING | MB_OK);
		//MessageBoxW(NULL, L"Пересечение", MB_ICONWARNING | MB_OK);
	}
}
// ---																					// Intersection Middle Plane
void CMainFrame::OnButtonIntersectionPlane()
{
	Intersection tmp(pDataTree, pView, &theApp.nextObjectID);
	int message = 0;
	message = tmp.InMiddleTwoPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InCylinderConePlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	if (message == 1)
	{
		AfxMessageBox(L"Нет пересечения", MB_ICONWARNING | MB_OK);
		//MessageBoxW(NULL, L"Пересечение", MB_ICONWARNING | MB_OK);
	}
}

//--------------------------------------------------------------
//	----	Thread		----	Scaning
//--------------------------------------------------------------

UINT __cdecl Thread_Scanning(LPVOID pMFrame)
{
	CmmApprox*		pCMM		= (CmmApprox*)((CMainFrame *)pMFrame)->pCMM;

	DialogMeasure*	dlgMeasure	= (DialogMeasure*)((CMainFrame *)pMFrame)->dlgMeasure;

	short	stat	= 0;
	float	b		= 0;
	bool	r		= 0;
	double	w		= 0;


	while (true)
	{
		if (!pCMM->flagConnected || dlgMeasure->m_hWnd == NULL)
		{
			Sleep(1);
			continue;
		}

		pCMM->GetPoint_(&pCMM->PointCurrent.X, &pCMM->PointCurrent.Y, &pCMM->PointCurrent.Z, &stat);
		
		if (stat == 0)
		{
			Sleep(1);
			continue;
		}

		pCMM->PointsForApprox.push_back(pCMM->PointCurrent);		// Add point to array

		dlgMeasure->PostMessageW(WM_POINT_APPROX_READY);

		Sleep(1);
	}

	return 0;
}
