// DialogMachine.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDubrovkin.h"
#include "DialogMachine.h"
#include "afxdialogex.h"


// DialogMachine dialog

IMPLEMENT_DYNAMIC(DialogMachine, CDialog)

DialogMachine::DialogMachine(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MACHINE, pParent)
{
	pCMM = theApp.pCMM;
}

DialogMachine::~DialogMachine()
{
	//
}

BOOL DialogMachine::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (pCMM->flagConnected)
		buttonConnection.SetWindowTextW(L"Отключиться");

	return TRUE; 
}


void DialogMachine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, buttonConnection);
}


BEGIN_MESSAGE_MAP(DialogMachine, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &DialogMachine::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// DialogMachine message handlers


//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------
// ---																					// On Connect\Disconnect Digitizer
void DialogMachine::OnBnClickedButtonConnect()
{
	char * chr = new char[5];

	if (pCMM->flagConnected)
	{
		pCMM->Disconect_();

		buttonConnection.SetWindowTextW(L"Подключиться");
	}
	else
	{
		try
		{
			pCMM->flagConnected = pCMM->Connect_();
		}
		catch (.../*const std::exception&*/)
		{
			MessageBoxW(L"Problem with connecting ...", L"Error", MB_OK + MB_ICONERROR);
			return;
		}

		if (pCMM->flagConnected)
		{
			buttonConnection.SetWindowTextW(L"Отключиться");
		}
		else
		{
			MessageBoxW(L"Problem with connecting ...", L"Error", MB_OK + MB_ICONERROR);
		}
	}
}
