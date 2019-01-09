#pragma once


// DialogSettings dialog

class DialogSettings : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	CmmApprox			*pCMM;

	double				AccuracyApprox;						

	int					PortNumber,
						MachineAccuracy;

//	--- --- --- --- --- --- --- --- ---	// Methods
	DECLARE_DYNAMIC(DialogSettings)

public:
	DialogSettings(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogSettings();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedButtonConnect();
	CButton buttonConnection;
	int m_vPortNumber;
	int m_vAccuracy;
	afx_msg void OnDestroy();
	int m_vMachineAccuracy;
};
