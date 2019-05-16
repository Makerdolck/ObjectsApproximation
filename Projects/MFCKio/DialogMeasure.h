#pragma once
#include "afxwin.h"

class CMFCKioView;
#include "MFCKioView.h"

// DialogMeasure dialog

class DialogMeasure : public CDialog
{
	DECLARE_DYNAMIC(DialogMeasure)
//	--- --- --- --- --- --- --- --- ---	// Enums
private:
	class PointsAmountEnum
	{
	public:
		enum PointsAmount
		{
			point		= 1,
			lineSegment = 2,
			plane		= 3,
			circle		= 3,
			cylinder	= 6,
			cone		= 6,
			sphere		= 3
		};
	};
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	CmmApprox						*pCMM;

	int								*NumStyle,
									PointsAmountForApprox, 
									PointsScanned;

	long int						*nextObjectID;

	PointsAmountEnum				PointsMinAmount;

	CompensationOptionNamesEnum		CompensationOptionNames;

	bool							flagNewElement;

	UINT							nPos_old;

public:

	float							AccuracyApprox;
	bool bAuto = false;
	ObjectApprox* objAp;
	CMFCKioView						*pView;

	std::vector<ObjectApprox*>		*objectsArray;
	std::vector<ToleranceObject*> *toleranceObjectsArray; // Voronov

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	void _UpdatePointsDataField();
	void _SendObjectToVisualize();
	void OnTraekt();

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	DialogMeasure(CWnd* pParent);   // standard constructor
	virtual ~DialogMeasure();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MEASURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CStatic MeasureName;
public:
	afx_msg LRESULT OnNewPointForApprox(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_vObjName;
	afx_msg void OnBnClickedButtonDeletePoint();
	afx_msg void OnBnClickedButtonDeletePoints();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnEnUpdateEditObjectName();
	CComboBox m_vCompensationOption;
	afx_msg void OnBnClickedOk();
};
// rrr