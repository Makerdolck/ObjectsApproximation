#pragma once


// DialogMachine dialog

class DialogMachine : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	CmmApprox * pCMM;

//	--- --- --- --- --- --- --- --- ---	// Methods
	DECLARE_DYNAMIC(DialogMachine)

public:
	DialogMachine(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogMachine();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MACHINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();
	CButton buttonConnection;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonHome();
};
