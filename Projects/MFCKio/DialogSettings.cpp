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
	, m_vCompensationValue(0)
	, m_vCoordinatesAccuracy(0)
	, m_vCornersAccuracy(0)
	, m_vTriangulationINAccuracy(0)
{
	pCMM					= theApp.pCMM;

	CoordinatesAccuracy		= theApp.CoordinatesAccuracy;
	CornersAccuracy			= theApp.CornersAccuracy;
	TriangulationINAccuracy = theApp.TriangulationINAccuracy;
	CompensationValue		= pCMM->CompensationValue;
}

DialogSettings::~DialogSettings()
{
}

BOOL DialogSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str = _T("");
	
	str.Format(L"%d", CoordinatesAccuracy);
	((CEdit*)GetDlgItem(IDC_EDIT_COORDINATES_ACC))->SetWindowTextW(str);

	str.Format(L"%d", CornersAccuracy);
	((CEdit*)GetDlgItem(IDC_EDIT_CORNERS_ACC))->SetWindowTextW(str);

	str.Format(L"%g", pCMM->CompensationValue);
	((CEdit*)GetDlgItem(IDC_EDIT_COMPENSATION_VALUE))->SetWindowTextW(str);

	str.Format(L"%g", TriangulationINAccuracy);
	((CEdit*)GetDlgItem(IDC_EDIT_TRIANGULATION_INACCURACY))->SetWindowTextW(str);

	return TRUE;
}

void DialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMPENSATION_VALUE, m_vCompensationValue);
	DDX_Text(pDX, IDC_EDIT_COORDINATES_ACC, m_vCoordinatesAccuracy);
	DDV_MinMaxInt(pDX, m_vCoordinatesAccuracy, 1, 7);
	DDX_Text(pDX, IDC_EDIT_CORNERS_ACC, m_vCornersAccuracy);
	DDV_MinMaxInt(pDX, m_vCornersAccuracy, 1, 7);
	DDX_Text(pDX, IDC_EDIT_TRIANGULATION_INACCURACY, m_vTriangulationINAccuracy);
}


BEGIN_MESSAGE_MAP(DialogSettings, CDialog)
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

	CoordinatesAccuracy		= m_vCoordinatesAccuracy;
	CornersAccuracy			= m_vCornersAccuracy;

	if (m_vCompensationValue < 0)
		m_vCompensationValue = 0;
	if (m_vTriangulationINAccuracy < 0)
		m_vTriangulationINAccuracy = 1;

	TriangulationINAccuracy = m_vTriangulationINAccuracy;
	CompensationValue		= m_vCompensationValue;
}

