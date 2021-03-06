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

// MFCKio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCKio.h"
#include "MainFrm.h"

#include "MFCKioDoc.h"
#include "MFCKioView.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCKioApp

BEGIN_MESSAGE_MAP(CMFCKioApp, CWinAppEx)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW,	 &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()




// CMFCKioApp construction

CMFCKioApp::CMFCKioApp()
{
	//// Memory Leaks detecting
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(20236);


	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MFCKio.AppID.Version1"));

	//	---	---	---	---	---	---	---	---	---	---	---
	pCMM					= new CmmApprox();
	nextObjectID			= 1;

	AccuracyApprox			= 0.00001f;
	CoordinatesAccuracy		= 5;
	CornersAccuracy			= 5;
	MTActive				= 0;
	TriangulationINAccuracy	= 0.1f;
}

// The one and only CMFCKioApp object
CMFCKioApp::~CMFCKioApp()
{
	//
}

CMFCKioApp theApp;


// CMFCKioApp initialization

BOOL CMFCKioApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("PolytechAndKIO"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
											IDR_VISIONTYPE,
											RUNTIME_CLASS(CMFCKioDoc),
											RUNTIME_CLASS(CMainFrame),       // main SDI frame window
											RUNTIME_CLASS(CMFCKioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	//	---	---	---	---	---	---	---	---	---	---	---

	CoordinatesAccuracy		= GetProfileIntW(L"Application", L"CoordinatesAccuracy", 5);
	CornersAccuracy			= GetProfileIntW(L"Application", L"CornersAccuracy", 5);
	AccuracyApprox			= (float)_wtof(AfxGetApp()->GetProfileStringW(L"Approx", L"Accuracy", L"0.00001"));
	TriangulationINAccuracy	= (float)_wtof(AfxGetApp()->GetProfileStringW(L"Approx", L"TriangulationINAccuracy", L"0.1"));
	pCMM->CompensationValue	= (float)_wtof(AfxGetApp()->GetProfileStringW(L"Digitizer", L"CompensationValue", L"3.998"));
	
	pCMM->Connect_();

	return TRUE;
}

int CMFCKioApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	//	---	---	---	---	---	---	---	---	---	---	---

	pCMM->Disconect_();

	CString str;
	
	WriteProfileInt(L"Application", L"CornersAccuracy", CornersAccuracy);
	WriteProfileInt(L"Application", L"CoordinatesAccuracy", CoordinatesAccuracy);

	str.Format(L"%f", AccuracyApprox);
	WriteProfileStringW(L"Approx", L"Accuracy", str);
	
	str.Format(L"%f", TriangulationINAccuracy);
	WriteProfileStringW(L"Approx", L"TriangulationINAccuracy", str);

	str.Format(L"%f", pCMM->CompensationValue);
	WriteProfileStringW(L"Digitizer", L"CompensationValue", str);

	delete pCMM;

	//	---	---	---	---	---	---	---	---	---	---	---

	return CWinAppEx::ExitInstance();
}

// CMFCKioApp message handlers



// CMFCKioApp customization load/save methods

void CMFCKioApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CMFCKioApp::LoadCustomState()
{
}

void CMFCKioApp::SaveCustomState()
{
}

// CMFCKioApp message handlers

