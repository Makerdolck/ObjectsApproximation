// ExampleView.h : interface of the CExampleView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __EXAMPLE_VIEW_H__
#define __EXAMPLE_VIEW_H__


#include <fstream>

#include <ApproximationCore/_ALLincludesCore.h>

#include <OpenGLView.h>

class CExampleView : public COpenGLView
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

	int			dd;

	HGLRC		hRC;

	GLdouble	gldAspect;
	GLsizei		glnWidth, 
				glnHeight;

public:
	void	DrawOpenGL_SceneObjects(GLenum mode);

	void	DrawOpenGL_Cube(double param, double cx, double cy, double cz);
	void	DrawOpenGL_Circle();
	void	DrawOpenGL_Cylinder();
	void	DrawOpenGL_Point();
	void	DrawOpenGL_LineSegment();
	void	DrawOpenGL_PlaneViaRectangle();
	void	DrawOpenGL_Cone();
	void	DrawOpenGL_Sphere();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

};

#ifndef _DEBUG  // debug version in ExampleView.cpp
inline CExampleDoc* CExampleView::GetDocument()
   { return (CExampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXAMPLE_VIEW_H__)
