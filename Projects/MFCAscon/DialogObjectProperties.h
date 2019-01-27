#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DialogObjectProperties dialog

class DialogObjectProperties : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	ObjectApprox		*object;
	
	CmmApprox			*pCMM;
//	--- --- --- --- --- --- --- --- ---	// Methods

	DECLARE_DYNAMIC(DialogObjectProperties)

public:
	DialogObjectProperties(CWnd* pParent, ObjectApprox *obj = nullptr);   // standard constructor
	virtual ~DialogObjectProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int NumStyle;
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
private:
	CStatic Vector_text;
	CEdit	Vector_x;
	CEdit	Vector_y;
	CEdit	Vector_z;
	CStatic Width_text;
	CEdit	Width_diameter;
	CStatic Diameter_text;
	CStatic Length_text;
	CEdit	Length_height;
	CStatic Height_text;
	CStatic Angle_text;
	CEdit	Angle_size;
	CButton	butSaveData;
public:
	CListCtrl	ListOfPoints;
	CEdit		vcCenterPoint_X;
	CEdit		vcCenterPoint_Y;
	CEdit		vcCenterPoint_Z;

	afx_msg void OnBnClickedSaveData();
};
