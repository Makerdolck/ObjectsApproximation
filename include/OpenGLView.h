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
#include "OutSource/KIOFont.h"


class COpenGLView : public CView
{
protected: // create from serialization only
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:

// Implementation
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	std::vector<ObjectApprox*> *objectsArray;

	GLfloat		gldAspect,
				fNearPlane,
				fFarPlane;

	GLint		glnWidth, 
				glnHeight;

	HGLRC		hRC;


	

	//FLOAT m_fLineWidth;
	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	bool		flagMiddleButtonDown,
				flagShiftPressed,
				flagRotateAxisX,
				flagRotateAxisY;

	GLdouble	m_dModelViewMatrix[16];
	GLdouble	m_dProjectionMatrix[16];
	GLint		m_iViewport[4];


	   
protected:
	//std::vector<ObjectApprox*>	*objectsArray;
	std::vector<ToleranceObject*>* toleranceObjectsArray; // Voronov

	const GLfloat blackColor[3] = { 0.0f,	0.0f,	0.0f };
	const GLfloat redColor[3]	= { 1.0f,	0.0f,	0.0f };
	const GLfloat blueColor[3]	= { 0.0f,	0.0f,	1.0f };
	const GLfloat simpleElementsColor1[3] = { 1.0f,	0.2f,	0.2f};
	const GLfloat simpleElementsColor2[3] = { 0.0f,	1.0f,	1.0f};


	VectorGeometric		vectorViewX,
						vectorViewY,
						vectorViewZ;

	PointGeometric		centerOfAllObjects,
						pointEyeLook,
						pointAimLook;


	KIOFont *kioFont;

	double						
								offsetView_X,
								offsetView_Y;

	float				distanceAimEye,
						BoxTrans[16],
						rotx,
						roty,
						prev_rotx,
						prev_roty;


	int					prev_x, 
						prev_y;


	
	

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
	void DrawOpenGL_ToleranceBase(ToleranceBase* base);
	void DrawOpenGL_ToleranceFrame(ToleranceFrame* frame);
	void DrawOpenGL_AngleLine(AngleLine* obj);

	void DrawOpenGL_SizeLine(SizeLine* obj);
	void DrawOpenGL_DiameterLine(DiameterLine* obj);
	void DrawOpenGL_AxialLine(AxialLine* obj);
	double angle_point(PointGeometric a, PointGeometric b, PointGeometric c);
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
	
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);


public:
		// Voronov
		bool flagToleranceMove = false; // Voronov Флаг что двигается размерная линия
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
