#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CMFCKioView;
#include "MFCKioView.h"

// DialogObjectProperties dialog

class DialogObjectProperties : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	ObjectApprox				*object;
	
	CmmApprox					*pCMM;

	CMFCKioView					*pView;

	CompensationOptionNamesEnum	CompensationOptionNames;

//	--- --- --- --- --- --- --- --- ---	// Methods

	DECLARE_DYNAMIC(DialogObjectProperties)

public:
	DialogObjectProperties(CWnd* pParent, ObjectApprox *obj = nullptr);   // standard constructor
	virtual ~DialogObjectProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJECT_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
private:
	CStatic		Vector_text;
	CEdit		Vector_x;
	CEdit		Vector_y;
	CEdit		Vector_z;
	CStatic		Width_text;
	CEdit		Width_diameter;
	CStatic		Diameter_text;
	CStatic		Length_text;
	CEdit		Length_height;
	CStatic		Height_text;
	CStatic		Angle_text;
	CEdit		Angle_size;
	CButton		butSaveData;
	CListCtrl	ListOfPoints;
	CEdit		vcCenterPoint_X;
	CEdit		vcCenterPoint_Y;
	CEdit		vcCenterPoint_Z;

	afx_msg void OnBnClickedSaveData();
private:
	void UpdateDataFields();
	CComboBox m_vCompensationOption;
public:
//	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeComboCompensationOption();
	afx_msg void OnDestroy();
};
