#pragma once


// DialogSettings dialog

class DialogSettings : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	CmmApprox			*pCMM;

	float				CompensationValue,
						TriangulationINAccuracy;

	int					CoordinatesAccuracy,
						CornersAccuracy;

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
	afx_msg void OnDestroy();
	float m_vCompensationValue;
	int m_vCoordinatesAccuracy;
	int m_vCornersAccuracy;
	float m_vTriangulationINAccuracy;
};


