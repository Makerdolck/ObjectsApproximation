#include "MainFrm.h"
#pragma once

// Диалоговое окно DialogToleranceSelectObjects

class DialogToleranceSelectObjects : public CDialog
{
	DECLARE_DYNAMIC(DialogToleranceSelectObjects)

public:
	enum TOLERANCE_NAME {

		ORIENTATION_PARALLELISM,
		ORIENTATION_PERPENDICULARITY,
		ORIENTATION_ANGULARITY,

		LOCATION_POSITION,
		LOCATION_CONCENTRICITY,
		LOCATION_COAXIALITY,
		LOCATION_SYMMETRY,

		RUNOUT_FACE,
		RUNOUT_RADIAL
	};

	DialogToleranceSelectObjects(CWnd* pParent, TOLERANCE_NAME toleranceName);   // стандартный конструктор

	//DialogToleranceSelectObjects(CMainFrame *parent);
	virtual ~DialogToleranceSelectObjects();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TOLERANCE_OBJECT_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()


public:

	

	TOLERANCE_NAME toleranceName;

	std::vector<ObjectApprox*>* objectsArray;

	ObjectApprox* base;
	ObjectApprox* control;

	CStatic instruction;
	CStatic *baseNameText;
	CStatic *controlNameText;
	CButton selectBase;
	CButton selectControl;


private:
	CMainFrame* parent;
	void unselectAllObjects();
	ObjectApprox* getSelectedObject();
	BOOL OnInitDialog();
	void changeName();


public:
	afx_msg void OnBnClickedButtonSelectBaseObject();
	afx_msg void OnBnClickedButtonSelectControlObject();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnStnClickedTextBaseName();
};
