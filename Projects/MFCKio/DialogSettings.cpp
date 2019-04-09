// DialogSettings.cpp : implementation file
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DialogSettings.h"
#include "afxdialogex.h"


// DialogSettings dialog

IMPLEMENT_DYNAMIC(DialogSettings, CDialog)

DialogSettings::DialogSettings(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SETTINGS, pParent)
	, m_vPortNumber(0)
	, m_vAccuracy(0)
	, m_vMachineAccuracy(0)
	, m_vCompensationValue(0)
{
	pCMM				= theApp.pCMM;
	AccuracyApprox		= theApp.AccuracyApprox;
	CompensationValue	= pCMM->CompensationValue;
}

DialogSettings::~DialogSettings()
{
}

BOOL DialogSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str = _T("");

	str.Format(L"%d", pCMM->PortNumber);
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->SetWindowTextW(str);

	str.Format(L"%d", (int)fabs(log10(AccuracyApprox)));
	GetDlgItem(IDC_EDIT_MEASURE_ACC)->SetWindowTextW(str);
	
	str.Format(L"%d", pCMM->MachineAccuracy);
	((CEdit*)GetDlgItem(IDC_EDIT_MACHINE_ACC))->SetWindowTextW(str);

	str.Format(L"%g", pCMM->CompensationValue);
	((CEdit*)GetDlgItem(IDC_EDIT_COMPENSATION_VALUE))->SetWindowTextW(str);

	if(pCMM->flagConnected)
		((CEdit*)GetDlgItem(IDC_EDIT_PORT))->SetReadOnly(true);

	return TRUE;
}


void DialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_vPortNumber);
	DDV_MinMaxInt(pDX, m_vPortNumber, 0, 100);
	DDX_Text(pDX, IDC_EDIT_MEASURE_ACC, m_vAccuracy);
	DDV_MinMaxInt(pDX, m_vAccuracy, 1, 100);
	DDX_Text(pDX, IDC_EDIT_MACHINE_ACC, m_vMachineAccuracy);
	DDV_MinMaxInt(pDX, m_vMachineAccuracy, 0, 20);
	DDX_Text(pDX, IDC_EDIT_COMPENSATION_VALUE, m_vCompensationValue);
}


BEGIN_MESSAGE_MAP(DialogSettings, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON_CONNECT, &DialogSettings::OnBnClickedButtonConnect)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

////--------------------------------------------------------------
////	----	Message Handlers
////--------------------------------------------------------------

// ---																					// On Destroy
void DialogSettings::OnDestroy()
{
	CDialog::OnDestroy();
	UpdateData(TRUE);
	AccuracyApprox	= (float)pow(10, (-1)*(int)m_vAccuracy);
	PortNumber		= m_vPortNumber;
	MachineAccuracy = m_vMachineAccuracy;

	if (m_vCompensationValue < 0)
		m_vCompensationValue = 0;
	CompensationValue = m_vCompensationValue;
}
