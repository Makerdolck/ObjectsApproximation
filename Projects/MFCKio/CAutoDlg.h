#pragma once
#include "afxwin.h"


// Диалоговое окно CAutoDlg

class CAutoDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoDlg)

public:
	CAutoDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CAutoDlg();
	bool m_bBoss= false;
	bool m_bPart = false;
	int m_bStep = 3;
	int m_bN = 2;
	int ObjType = 0;
	int maxP = 2;
	int maxH = 2;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AUTO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioVal();
	afx_msg void OnBnClickedRadioGap();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedCheckPart();
	CEdit EdStep;
	CEdit EdPl;
	CStatic StStep;
	CStatic StPlane;
	CStatic StType;
	
	CButton ChPart;
	int maxPlanes;
	afx_msg void OnKillfocusEditPlanes();
	int maxPoints;
	afx_msg void OnKillfocusEditStep();
};
