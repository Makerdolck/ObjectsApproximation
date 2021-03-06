#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DialogCreate dialog

class DialogCreate : public CDialog
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	ObjectApproxC3D		*object;
	
//	--- --- --- --- --- --- --- --- ---	// Methods

	DECLARE_DYNAMIC(DialogCreate)

public:
	DialogCreate(CWnd* pParent, ObjectApproxC3D *obj = nullptr);   // standard constructor
	virtual ~DialogCreate();

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
	CEdit Vector_x;
	CEdit Vector_y;
	CEdit Vector_z;
	CStatic Width_text;
	CEdit Width_radius;
	CStatic Radius_text;
	CStatic Length_text;
	CEdit Length_height;
	CStatic Height_text;
	CStatic Angle_text;
	CEdit Angle_size;
public:
	CListCtrl ListOfPoints;
	CEdit vcCenterPoint_X;
	CEdit vcCenterPoint_Y;
	CEdit vcCenterPoint_Z;
};
