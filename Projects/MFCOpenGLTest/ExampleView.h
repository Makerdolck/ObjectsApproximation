// ExampleView.h : interface of the CExampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMPLEVIEW_H__F10F716D_2B31_11D4_90AE_0010B53C9D0C__INCLUDED_)
#define AFX_EXAMPLEVIEW_H__F10F716D_2B31_11D4_90AE_0010B53C9D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>

#include <ApproximationCore/_ALLincludesCore.h>

class CExampleView : public CView
{
protected: // create from serialization only
	CExampleView();
	DECLARE_DYNCREATE(CExampleView)

// Attributes
public:
	CExampleDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExampleView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CExampleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HGLRC m_hglrc;

	bool		flagMiddleButtonDown;

	int			mouse_x0, 
				mouse_y0,
		
				mouseX_old,
				mouseY_old;

	float		m_z;

	GLfloat		wAngleY,
				wAngleX,
				wAngleZ,

				fNearPlane, 
				fFarPlane,
				fAspect,
				
				wTransformX,
				wTransformY;

	CRect       m_oldRect;

	int			dd = 2;

	//CDC* pDC;
public:
	void DrawOpenGL_Cube(double param, double cx, double cy, double cz);
	void DrawOpenGL_Circle();
	void DrawOpenGL_Cylinder();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMycomandDrawpoint();
};

#ifndef _DEBUG  // debug version in ExampleView.cpp
inline CExampleDoc* CExampleView::GetDocument()
   { return (CExampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMPLEVIEW_H__F10F716D_2B31_11D4_90AE_0010B53C9D0C__INCLUDED_)
