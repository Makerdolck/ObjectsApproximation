// try_OpenGLView.h : interface of the CTry_OpenGLView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __OPENGL_VIEW_H__
#define __OPENGL_VIEW_H__

#include <afxwin.h>
#include <fstream>
#include <ApproximationCore/_ALLincludesCore.h>

class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

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
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	GLdouble	gldAspect;
	GLsizei		glnWidth, 
				glnHeight;

	HGLRC		hRC;
	double		r;


	HGLRC		m_hglrc;

	bool		flagMiddleButtonDown;

	int			mouse_x0,
				mouse_y0,

				mouseX_old,
				mouseY_old;

	double		m_z;

	GLfloat		wAngleY,
				wAngleX,
				wAngleZ,

				fNearPlane,
				fFarPlane,
				fAspect,

				wTransformX,
				wTransformY,			

				angleLook;

	CRect       m_oldRect;

	int			dd;

	bool flagColor1, flagColor2, flagColor3, flagColor4, flagColor5, flagColor6, flagColor7;

	//FLOAT m_fLineWidth;
	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	VectorGeometric		vectorRotationX,
						vectorRotationY,
						vectorRotationZ;



protected:
	std::vector<ObjectApprox*>	*objectsArray;

	PointGeometric				centerOfAllObjects;

	PointGeometric				pointEyeLook,
								pointAimLook;

// Generated message map functions
protected:
	HGLRC	SetUpOpenGL(HWND hWnd);
	void	PaintScene(GLenum mode);
	//void	DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor);
	void	DrawOpenGL_Circle(GeomObjectApprox obj);
	void	DrawOpenGL_Point(PointApprox *obj);
	void	DrawOpenGL_LineSegment(LineSegmentApprox *obj);
	void	DrawOpenGL_PlaneViaRectangle(GeomObjectApprox obj);


	void	DrawOpenGL_ObjViaTriangles(GeomObjectApprox obj);


	//{{AFX_MSG(CTry_OpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in try_OpenGLView.cpp
inline CTry_OpenGLDoc* CTry_OpenGLView::GetDocument()
   { return (CTry_OpenGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OPENGL_VIEW_H__)
