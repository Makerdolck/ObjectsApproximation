// CAutoDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCKio.h"
#include "CAutoDlg.h"
#include "afxdialogex.h"
#include <sstream>


// Диалоговое окно CAutoDlg

IMPLEMENT_DYNAMIC(CAutoDlg, CDialog)

CAutoDlg::CAutoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_AUTO, pParent)
	
	, maxPlanes(0)
	, maxPoints(0)
{

}

CAutoDlg::~CAutoDlg()
{
}

void CAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STEP, EdStep);
	DDX_Control(pDX, IDC_EDIT_PLANES, EdPl);
	DDX_Control(pDX, IDC_ST_STEP, StStep);
	DDX_Control(pDX, IDC_ST_PLANE, StPlane);
	DDX_Control(pDX, IDC_ST_TYPE, StType);

	DDX_Control(pDX, IDC_CHECK_PART, ChPart);
	DDX_Text(pDX, IDC_EDIT_PLANES, maxPlanes);
	DDX_Text(pDX, IDC_EDIT_STEP, maxPoints);
}


BEGIN_MESSAGE_MAP(CAutoDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_VAL, &CAutoDlg::OnBnClickedRadioVal)
	ON_BN_CLICKED(IDC_RADIO_GAP, &CAutoDlg::OnBnClickedRadioGap)
	ON_BN_CLICKED(IDOK, &CAutoDlg::OnBnClickedOk)
	
	ON_BN_CLICKED(IDC_CHECK_PART, &CAutoDlg::OnBnClickedCheckPart)
	ON_EN_KILLFOCUS(IDC_EDIT_PLANES, &CAutoDlg::OnKillfocusEditPlanes)
	ON_EN_KILLFOCUS(IDC_EDIT_STEP, &CAutoDlg::OnKillfocusEditStep)
END_MESSAGE_MAP()


// Обработчики сообщений CAutoDlg


void CAutoDlg::OnBnClickedRadioVal()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_bBoss = false;
}


void CAutoDlg::OnBnClickedRadioGap()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_bBoss = true;
}


BOOL CAutoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	

	CString t;

	t.Format(_T("%d"), m_bStep);
	EdStep.SetWindowTextW((LPCTSTR)t);
	t.Format(_T("%d"), m_bN);
	EdPl.SetWindowTextW((LPCTSTR)t);
	((CButton*)(GetDlgItem(IDC_RADIO_VAL)))->SetCheck(true);
	if (ObjType == 1)
	{
		ChPart.ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_VAL)))->ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_GAP)))->ShowWindow(false);
		StType.ShowWindow(false);
	}
	if (ObjType == 2)
	{
		
		//((CButton*)(GetDlgItem(IDC_RADIO_VAL)))->ShowWindow(false);
		//((CButton*)(GetDlgItem(IDC_RADIO_GAP)))->ShowWindow(false);
		StType.ShowWindow(false);
		StPlane.ShowWindow(false);
		EdPl.ShowWindow(false);
	}
	if (ObjType == 3)
	{
		ChPart.ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_VAL)))->ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_GAP)))->ShowWindow(false);
		StType.ShowWindow(false);
		StPlane.ShowWindow(false);
		EdPl.ShowWindow(false);
	}
	if (ObjType == 4)
	{
		ChPart.ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_VAL)))->ShowWindow(false);
		((CButton*)(GetDlgItem(IDC_RADIO_GAP)))->ShowWindow(false);
		StType.ShowWindow(false);
		StStep.ShowWindow(false);
		EdStep.ShowWindow(false);
		EdPl.ShowWindow(false);
		StPlane.ShowWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CAutoDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	
	CString sText;
	GetDlgItemText(IDC_EDIT_PLANES, sText);
	m_bN = _wtoi(sText);
	GetDlgItemText(IDC_EDIT_STEP, sText);
	m_bStep = _wtoi(sText);
	CDialog::OnOK();
}





void CAutoDlg::OnBnClickedCheckPart()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_bPart = !m_bPart;
}


void CAutoDlg::OnKillfocusEditPlanes()
{
	UpdateData(TRUE);
	CString t;
	if (maxPlanes < 2)
	{
		t.Format(_T("%d"), 2);
		EdPl.SetWindowTextW((LPCTSTR)t);
	}
	if (maxPlanes > maxH)
	{
		t.Format(_T("%d"), maxH);
		EdPl.SetWindowTextW((LPCTSTR)t);
	}
}

void CAutoDlg::OnKillfocusEditStep()
{
	UpdateData(TRUE);
	CString t;
	if (maxPoints < 2)
	{
		t.Format(_T("%d"), 2);
		EdStep.SetWindowTextW((LPCTSTR)t);
	}
	if (maxPoints > maxP)
	{
		t.Format(_T("%d"), maxP);
		EdStep.SetWindowTextW((LPCTSTR)t);
	}
}
