#include "MainFrm.h"
#pragma once

// Диалоговое окно DialogToleranceSelectObjects

class DialogToleranceSelectObjects : public CDialog
{
	DECLARE_DYNAMIC(DialogToleranceSelectObjects)

public:
	

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
	std::vector<ToleranceObject*>* toleranceObjectsArray;

	ToleranceBase* base;
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
	ToleranceBase* getSelectedBase();
	BOOL OnInitDialog();
	void changeName();


public:
	afx_msg void OnBnClickedButtonSelectBaseObject();
	afx_msg void OnBnClickedButtonSelectControlObject();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnStnClickedTextBaseName();
	afx_msg void OnStnClickedTextInstruction();
};
