// try_OpenGLView.h : interface of the CTry_OpenGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRY_OPENGLVIEW_H__AD9F0474_7472_482D_8A1D_1DE2154A5366__INCLUDED_)
#define AFX_TRY_OPENGLVIEW_H__AD9F0474_7472_482D_8A1D_1DE2154A5366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyOpenGL.h"

class CTry_OpenGLView : public CView
{
protected: // create from serialization only
	void PaintScreen(GLenum mode);
	CTry_OpenGLView();
	DECLARE_DYNCREATE(CTry_OpenGLView)

// Attributes
public:
	CTry_OpenGLDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTry_OpenGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTry_OpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


private:
	GLdouble gldAspect;
	GLsizei glnWidth, glnHeight;
	HGLRC hRC;
	Cube cube[4];
	double r;


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

	int			dd;



// Generated message map functions
protected:
	void DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor);


	//{{AFX_MSG(CTry_OpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in try_OpenGLView.cpp
inline CTry_OpenGLDoc* CTry_OpenGLView::GetDocument()
   { return (CTry_OpenGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRY_OPENGLVIEW_H__AD9F0474_7472_482D_8A1D_1DE2154A5366__INCLUDED_)
