// try_OpenGLView.h : interface of the CTry_OpenGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRY_OPENGLVIEW_H__AD9F0474_7472_482D_8A1D_1DE2154A5366__INCLUDED_)
#define AFX_TRY_OPENGLVIEW_H__AD9F0474_7472_482D_8A1D_1DE2154A5366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyOpenGL.h"


#define RAD_IN_DEG 0.017453292519943295769

#include <OutSource/matrix.h>
#include <OutSource/vector.h>
#include <OutSource/facet_utils.h>


#include <string>


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
	double rMyRotation;


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


	/////////////////////////////////////////////////////////////////////////////////////

	HWND hwnd;
	HDC hdc;
	HGLRC hglrc;

	facet_obj* f;

	enum pm_t
	{
		xz_pm_t = 0,
		yz_pm_t,
		xy_pm_t,
		xyz_pm_t,
		norm_pm_t
	};


	GLclampf bg_r, bg_g, bg_b; //фон

	GLfloat LightAmbient_r, LightAmbient_g, LightAmbient_b;
	GLfloat LightDiffuse_r, LightDiffuse_g, LightDiffuse_b;
	GLfloat LightPosition_x, LightPosition_y, LightPosition_z;


	int ww, hw; //размеры окна
	float w[4], h[4]; //размеры модели

	float l[4], b[4], r[4], t[4]; //границы модели

	int xw, yw; //координаты курсора в пр-ве окна
	float x[4], y[4]; //координаты курсора в пр-ве модели

	int xwp, ywp; //координаты курсора в пр-ве окна при активации ф-ии панорамирования
	float lp, bp; //левый нижний угол модели при активации ф-ии панорамирования

	float s[4]; //к-т масштабирования
	float ss; //ступень масштабирования
	float m;

	bool pan;
	DWORD pan_t;

	bool rotate;

	float ax;
	float ay;

	float axp;
	float ayp;

	vector ex;
	vector ey;

	vector exp;
	vector eyp;

	//TPanelEx* gl_panel;

	int digits;

	HFONT font_h;
	GLuint font_list;
	int real_font_height;

	HFONT font_big_h;
	GLuint font_big_list;


	/////////////////////////////////////////////////////////////////////////////////////




// Generated message map functions
protected:
	void DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor);
	void set_projection_matrix(const pm_t type);

	void create_fonts(void);
	void destroy_fonts(void);

	void create_gl_fonts(void);
	void destroy_gl_fonts(void);

	void print(const GLuint lst, const float& x, const float& y/*, const AnsiString& s*/);

	void set_dc_pixel_format(void);
	void set_gl_params(void);

	//void set_projection_matrix(const pm_t type);

	void render(void);

	void render_basis
	(
		const GLclampf ex_r = 0.7, const GLclampf ex_g = 0, const GLclampf ex_b = 0,
		const GLclampf ey_r = 0, const GLclampf ey_g = 0.7, const GLclampf ey_b = 0,
		const GLclampf ez_r = 0.3, const GLclampf ez_g = 0.3, const GLclampf ez_b = 1,
		const bool invert_ey = false,
		const float m = 1
	);
	void render_basis_info
	(
		const GLclampf ex_r = 0.7, const GLclampf ex_g = 0, const GLclampf ex_b = 0,
		const GLclampf ey_r = 0, const GLclampf ey_g = 0.7, const GLclampf ey_b = 0,
		const GLclampf ez_r = 0.3, const GLclampf ez_g = 0.3, const GLclampf ez_b = 1,
		const bool invert_ey = false,
		const float m = 1
	);



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
