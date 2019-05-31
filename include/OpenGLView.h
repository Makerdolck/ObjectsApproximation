// try_OpenGLView.h : interface of the CTry_OpenGLView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __OPENGL_VIEW_H__
#define __OPENGL_VIEW_H__

#include <afxwin.h>
#include <fstream>
#include <ApproximationCore/_ALLincludesCore.h>

#include <OutSource/Matrix.h>
#include "Tolerance/includes.h"
#include "OutSource/MyFont.h"


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

protected:
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

	GLsizei		wAngleY,
				wAngleX,
				wAngleZ;

	GLfloat		fNearPlane,
				fFarPlane,
				fAspect,

				wTransformX,
				wTransformY,			

				angleLook;

	CRect       m_oldRect;

	int			dd,
				rotationStep,
				rotationAngleX,
				rotationAngleY;

	bool	flagColor1, flagColor2, flagColor3, flagColor4, flagColor5, flagColor6, flagColor7,
			flagShiftPressed,
			flagRotateAxisX,
			flagRotateAxisY;

	

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
	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	PointGeometric				centerOfAllObjects;

	PointGeometric				pointEyeLook,
								pointAimLook;

	MyFont *myFont;

	double						distanceAimEye,
								offsetView_X,
								offsetView_Y;

	float						BoxTrans[16];
	float						rotx = 0,
								roty = 0,
								prev_rotx = 0,
								prev_roty = 0;
	int							prev_x, prev_y;

	float						coordinateDifferenceX,
								coordinateDifferenceY;

	
	

// Generated message map functions
protected:
	ObjectApprox	*GetObjectUnderMouse(CPoint point);
	ToleranceObject	*GetToleranceObjectUnderMouse(CPoint point);

	HGLRC			SetUpOpenGL(HWND hWnd);
	void			PaintScene(GLenum mode);
	void			DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor);
	void			DrawOpenGL_ByLineLoop(std::vector<PointGeometric> points, VectorGeometric vector);
	void			DrawOpenGL_Circle(GeomObjectApprox obj);
	void			DrawOpenGL_Point(PointApprox *obj);
	void			DrawOpenGL_LineSegment(LineSegmentApprox *obj);
	void			DrawOpenGL_PlaneViaRectangle(GeomObjectApprox obj);

	void			GetWorldCoord(int ix, int iy, GLdouble fz, PointGeometric& coord);
	void			GetScreenCoord(GLdouble wX, GLdouble wY, GLdouble wZ, PointGeometric& coord);

	void			DrawOpenGL_ObjViaTriangles(GeomObjectApprox obj);


	// Voronov
	void DrawOpenGL_ToleranceFrame(ToleranceFrame* frame);
	void DrawOpenGL_SizeLine(SizeLine* obj);
	double angle_point(PointGeometric a, PointGeometric b, PointGeometric c);
	void DrawOpenGL_DiameterLine(DiameterLine* obj);
	void DrawOpenGL_AxialLine(AxialLine* obj);
	void DrawOpenGL_Tolerance_Form_Roudness(FormRoundnessToleranceObject* obj);
	//-----------------------

	//{{AFX_MSG(CTry_OpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool flagToleranceMove = false; // Voronov Флаг что двигается размерная линия
	
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);


public:
		// Voronov
		bool isToleranceAction = false; // Происходит ли выбор связанный с выбором объекта для измерения допуска или нет
		ToleranceObject* selectedToleranceObject = nullptr; // Voronov 
		void startSelectObject(ToleranceObject* selectedObject); // Маркирует isToleranceAction значением true, selectedToleranceObject = selectedObject, selectedObject->flagSelected = true
};

#ifndef _DEBUG  // debug version in try_OpenGLView.cpp
inline CTry_OpenGLDoc* CTry_OpenGLView::GetDocument()
   { return (CTry_OpenGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__OPENGL_VIEW_H__)
