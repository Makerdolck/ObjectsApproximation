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
#include <Tolerance/Tolerance.h>

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

	//Voronov
	ON_COMMAND(ID_TOLERANCE_LINE_SIZE, &CMainFrame::OnToleranceLineSize)
	ON_COMMAND(ID_TOLERANCE_DIAMETER_LINE, &CMainFrame::OnToleranceDiameterLine)
	ON_COMMAND(ID_TOLERANCE_AXIAL_LINE, &CMainFrame::OnToleranceAxialLine)

	ON_COMMAND(ID_TOLERANCE_FORM_STRAIGHTNESS, &CMainFrame::OnToleranceFormStraightness)
	ON_COMMAND(ID_TOLERANCE_FORM_FLATNESS, &CMainFrame::OnToleranceFormFlatness)
	ON_COMMAND(ID_TOLERANCE_FORM_ROUNDNESS, &CMainFrame::OnToleranceFormRoundess)
	ON_COMMAND(ID_TOLERANCE_FORM_CYLINDRICITY, &CMainFrame::OnToleranceFormCylindricity)

	ON_COMMAND(ID_TOLERANCE_ORIENTATION_PARALLELISM, &CMainFrame::OnToleranceOrientationParallelism)

	ON_COMMAND(ID_TOLERANCE_LOCATION_CONCENTRICITY, &CMainFrame::OnToleranceLocationConcentricity)
	ON_COMMAND(ID_TOLERANCE_LOCATION_COAXIALITY, &CMainFrame::OnToleranceLocationCoaxiality)

	ON_COMMAND(ID_TMP_BTN_TOLERANCE_ORIENTATION, &CMainFrame::OnToleranceCreateOrientation)
	ON_COMMAND(ID_TMP_BTN_TOLERANCE_LOCATION, &CMainFrame::OnToleranceCreateLocation)
	
	//-------------
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
	delete pTolerance; // Voronov
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
	pView->dlgMeasure = dlgMeasure;
	

	dlgMeasure->objectsArray			= &((CMFCKioDoc*)pView->GetDocument())->objectsArray;
	dlgMeasure->toleranceObjectsArray = &((CMFCKioDoc*)pView->GetDocument())->toleranceObjectsArray; // Voronov
	pDataTree->objectsArray				= &((CMFCKioDoc*)pView->GetDocument())->objectsArray;

	pTolerance = new Tolerance(dlgMeasure->toleranceObjectsArray);
	return TRUE;
}

// ---																					// Measure Point
void CMainFrame::OnButtonMeasurePoint()
{
	theApp.MTActive = theApp.MTContainer.point;

	dlgMeasure->DoModal();
}
// ---																					// Measure Line
void CMainFrame::OnButtonMeasureLine()
{
	theApp.MTActive = theApp.MTContainer.lineSegment;

	dlgMeasure->DoModal();
}
// ---																					// Measure Plane
void CMainFrame::OnButtonMeasurePlane()
{
	theApp.MTActive = theApp.MTContainer.plane;

	dlgMeasure->DoModal();
}
// ---																					// Measure Circle
void CMainFrame::OnButtonMeasureCircle()
{
	theApp.MTActive = theApp.MTContainer.circle;

	dlgMeasure->DoModal();
}
// ---																					// Measure Cylinder
void CMainFrame::OnButtonMeasureCylinder()
{
	theApp.MTActive = theApp.MTContainer.cylinder;

	dlgMeasure->DoModal();
}
// ---																					// Measure Cone
void CMainFrame::OnButtonMeasureCone()
{
	theApp.MTActive = theApp.MTContainer.cone;

	dlgMeasure->DoModal();
}
// ---																					// Measure Sphere
void CMainFrame::OnButtonMeasureSphere()
{
	theApp.MTActive = theApp.MTContainer.sphere;

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
	Intersection tmp;
	int message = 0;
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
	message = tmp.InTwoPoint(dlgMeasure->objectsArray, 1);
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

	message = tmp.InTwoPlane(dlgMeasure->objectsArray);
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
	message = tmp.InPlanePoint(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}
	message = tmp.InSpherePlane(dlgMeasure->objectsArray);
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
	Intersection tmp;
	int message = 0;

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
	Intersection tmp;
	int message = 0;
	message = tmp.InConeAndPlane(dlgMeasure->objectsArray);
	if (message == 0) {

		pView->RedrawWindow();
		return;
	}

	message = tmp.InSpherePlane(dlgMeasure->objectsArray);
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
	Intersection tmp;
	int message = 0;

	message = tmp.InCirclePointMiddle(dlgMeasure->objectsArray);
	if (message == 0) {
		pView->RedrawWindow();
		return;
	}
	message = tmp.InLinePointMiddle(dlgMeasure->objectsArray);
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


// Voronov 
void CMainFrame::OnToleranceLineSize()
{
	pTolerance->DrawSizeLine(dlgMeasure->objectsArray);
	pView->RedrawWindow();
}

void CMainFrame::OnToleranceDiameterLine()
{
	pTolerance->DrawDiameterLine(dlgMeasure->objectsArray);
	pView->RedrawWindow();
}

void CMainFrame::OnToleranceAxialLine()
{
	pTolerance->DrawAxialLine(dlgMeasure->objectsArray);
	pView->RedrawWindow();
}

void CMainFrame::OnToleranceFormStraightness()
{
	pTolerance->FormStraightness((LineSegmentApprox*)(dlgMeasure->objectsArray->operator[](0)->objMath)); // TODO: Исправить выбор объекта
	
}

void CMainFrame::OnToleranceFormFlatness()
{
	pTolerance->FormFlatness((PlaneApprox*)(dlgMeasure->objectsArray->operator[](0)->objMath)); // TODO: Исправить выбор объекта
	
}

void CMainFrame::OnToleranceFormRoundess()
{
	pTolerance->DrawFormRoundness(dlgMeasure->objectsArray);
	pView->RedrawWindow();
}

void CMainFrame::OnToleranceFormCylindricity()
{
	pTolerance->FormCylindricity((CylinderApprox*)(dlgMeasure->objectsArray->operator[](0)->objMath)); // TODO: Исправить выбор объекта

}

void CMainFrame::OnToleranceOrientationParallelism()
{
	pTolerance->DrawOrientationParallelism(dlgMeasure->objectsArray);

}

void CMainFrame::OnToleranceLocationConcentricity()
{
	pTolerance->DrawLocationConcentricity(dlgMeasure->objectsArray);

}

void CMainFrame::OnToleranceLocationCoaxiality()
{
	pTolerance->DrawLocationCoaxiality(dlgMeasure->objectsArray);

}



void CMainFrame::OnToleranceCreateOrientation()
{
	LineSegmentApprox* lineSegmentApprox1 = new LineSegmentApprox();
	LineSegmentApprox* lineSegmentApprox2 = new LineSegmentApprox();
	LineSegmentApprox* lineSegmentApprox3 = new LineSegmentApprox();

	std::ifstream					file;

	std::vector <PointGeometric>	points;

	PointGeometric					pointMy;

	double							accuracy = 0.00001;

	ObjectApprox* newObjectApproxC3D1 = new ObjectApprox();
	ObjectApprox* newObjectApproxC3D2 = new ObjectApprox();

	ObjectApprox* newObjectApproxC3D3 = new ObjectApprox();
	ObjectApprox* newObjectApproxC3D4 = new ObjectApprox();

	theApp.MTActive = theApp.MTContainer.lineSegment;
	file.open("dataLine2.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			lineSegmentApprox1->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		lineSegmentApprox1->FindByPoints(&lineSegmentApprox1->PointsForApprox[0], (int)lineSegmentApprox1->PointsForApprox.size(), accuracy);

		newObjectApproxC3D1->objMath = lineSegmentApprox1;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D1);
		dlgMeasure->objectsArray->back()->objID = 1;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"first_line";
		
	}
	file.close();
	file.open("dataLine3.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			lineSegmentApprox2->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		lineSegmentApprox2->FindByPoints(&lineSegmentApprox2->PointsForApprox[0], (int)lineSegmentApprox2->PointsForApprox.size(), accuracy);

		newObjectApproxC3D2->objMath = lineSegmentApprox2;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D2);
		dlgMeasure->objectsArray->back()->objID = 2;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"second_line";
		
	}
	file.close();
	
	theApp.MTActive = theApp.MTContainer.plane;
	RectangleApprox* rectangleApprox = new RectangleApprox();
	ObjectApprox* newObjectApproxC3D = new ObjectApprox();
	file.open("dataPlane2.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			rectangleApprox->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		rectangleApprox->FindByPoints(&rectangleApprox->PointsForApprox[0], (int)rectangleApprox->PointsForApprox.size(), accuracy);

		rectangleApprox->Triangulation();

		newObjectApproxC3D->objMath = rectangleApprox;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 3;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"first_plane";
		
	}
	file.close();

	newObjectApproxC3D = new ObjectApprox();
	rectangleApprox = new RectangleApprox();
	file.open("dataPlane3.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			rectangleApprox->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		rectangleApprox->FindByPoints(&rectangleApprox->PointsForApprox[0], (int)rectangleApprox->PointsForApprox.size(), accuracy);

		rectangleApprox->Triangulation();


		newObjectApproxC3D->objMath = rectangleApprox;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 4;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"second_plane";
		
	}

	file.close();

	newObjectApproxC3D = new ObjectApprox();
	rectangleApprox = new RectangleApprox();
	file.open("dataPlane4.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			rectangleApprox->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		rectangleApprox->FindByPoints(&rectangleApprox->PointsForApprox[0], (int)rectangleApprox->PointsForApprox.size(), accuracy);

		rectangleApprox->Triangulation();

	
		newObjectApproxC3D->objMath = rectangleApprox;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 5;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"third_plane";
		
	}
	file.close();

	pView->SendMessageW(WM_MATH_OBJECT_READY);

}

void CMainFrame::OnToleranceCreateLocation()
{
	CircleApprox* circle = new CircleApprox();

	CylinderApprox* cylinder = new CylinderApprox();
	

	std::ifstream					file;

	std::vector <PointGeometric>	points;

	PointGeometric					pointMy;

	double							accuracy = 0.00001;

	ObjectApprox* newObjectApproxC3D = new ObjectApprox();
	
	theApp.MTActive = theApp.MTContainer.cylinder;

	file.open("dataCylinder2.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			cylinder->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		//cylinderApprox->IsHole = isHole;

		cylinder->FindByPoints(&cylinder->PointsForApprox[0], (int)cylinder->PointsForApprox.size(), accuracy);

		cylinder->Triangulation(theApp.TriangulationINAccuracy);

		newObjectApproxC3D->objMath = cylinder;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 1;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Cylinder1";
		
	}
	file.close();
	
	cylinder = new CylinderApprox();
	newObjectApproxC3D = new ObjectApprox();
	
	file.open("dataCylinder3.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			cylinder->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		//cylinderApprox->IsHole = isHole;

		cylinder->FindByPoints(&cylinder->PointsForApprox[0], (int)cylinder->PointsForApprox.size(), accuracy);

		cylinder->Triangulation(theApp.TriangulationINAccuracy);
		newObjectApproxC3D->objMath = cylinder;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 2;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Cylinder2";
		
	}
	file.close();
	
	cylinder = new CylinderApprox();
	newObjectApproxC3D = new ObjectApprox();
	
	file.open("dataCylinder4.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			cylinder->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		//cylinderApprox->IsHole = isHole;

		cylinder->FindByPoints(&cylinder->PointsForApprox[0], (int)cylinder->PointsForApprox.size(), accuracy);

		cylinder->Triangulation(theApp.TriangulationINAccuracy);
		newObjectApproxC3D->objMath = cylinder;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 3;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Cylinder3";
		
	}
	file.close();
	
	
	circle = new CircleApprox();
	newObjectApproxC3D = new ObjectApprox();
	file.open("dataCircle1.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			pointMy.X += 10;
			pointMy.Y += 10;
			pointMy.Z += 10;
			circle->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		circle->FindByPoints(&circle->PointsForApprox[0], (int)circle->PointsForApprox.size(), accuracy);
		circle->Triangulation(theApp.TriangulationINAccuracy);
		newObjectApproxC3D->objMath = circle;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 4;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Circle1";
	}
	file.close();
	
	
	circle = new CircleApprox();
	newObjectApproxC3D = new ObjectApprox();
	file.open("dataCircle2.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			pointMy.X += 10;
			pointMy.Y += 10;
			pointMy.Z += 10;
			circle->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		circle->FindByPoints(&circle->PointsForApprox[0], (int)circle->PointsForApprox.size(), accuracy);
		circle->Triangulation(theApp.TriangulationINAccuracy);
		newObjectApproxC3D->objMath = circle;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 5;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Circle2";
	}
	file.close();
	
	
	circle = new CircleApprox();
	newObjectApproxC3D = new ObjectApprox();
	file.open("dataCircle3.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> pointMy.X >> pointMy.Y >> pointMy.Z;
			pointMy.X += 10;
			pointMy.Y += 10;
			pointMy.Z += 10;
			circle->PointsForApprox.push_back(pointMy);
			//points.Add(point);
		}

		circle->FindByPoints(&circle->PointsForApprox[0], (int)circle->PointsForApprox.size(), accuracy);
		circle->Triangulation(theApp.TriangulationINAccuracy);
		newObjectApproxC3D->objMath = circle;
		dlgMeasure->objectsArray->push_back(newObjectApproxC3D);
		dlgMeasure->objectsArray->back()->objID = 6;
		dlgMeasure->objectsArray->back()->flagReady = true;
		dlgMeasure->objectsArray->back()->Name = L"Circle3";
	}
	file.close();
	
	



	pView->SendMessageW(WM_MATH_OBJECT_READY);

}

