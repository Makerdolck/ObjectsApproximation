// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//

#include "stdafx.h"
#include "try_OpenGL.h"

#include "try_OpenGLDoc.h"
#include "try_OpenGLView.h"

bool flagCubeColor_1 = false;
const GLfloat redColor[] = { 1.0,0.0,0.0,1.0 };
const GLfloat blueColor[] = { 0.0,0.0,1.0,1.0 };

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView

IMPLEMENT_DYNCREATE(CTry_OpenGLView, CView)

BEGIN_MESSAGE_MAP(CTry_OpenGLView, CView)
	//{{AFX_MSG_MAP(CTry_OpenGLView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView construction/destruction

CTry_OpenGLView::CTry_OpenGLView()
{
	m_z = 10.0f;

	wAngleY = -10.0f;		//10.0f;
	wAngleX = 15.0f;		//1.0f;
	wAngleZ = 0.0f;		//5.0f;

	wTransformX = 0;
	wTransformY = 0;

	fFarPlane = 500.0f;

	flagMiddleButtonDown = false;



		bg_r=0;
		bg_g=0;
		bg_b=0.2;
		LightAmbient_r=0.2;
		LightAmbient_g=0.2;
		LightAmbient_b=0.2;
		LightDiffuse_r=1;
		LightDiffuse_g=1;
		LightDiffuse_b=1;
		LightPosition_x=0.5;
		LightPosition_y=1;
		LightPosition_z=0.75;
		ss=1.5;
		m=0.00001;
		pan=false;
		rotate=false;
		ax=0;
		ay=0;
		ex=  vector(1, 0, 0);
		ey= vector(0, 1, 0);
		digits=4;
		f = new facet_obj();


		for (int idx = 0; idx < 4; idx++)
			s[idx] = 100;


		ww = glnWidth;
		hw = glnHeight;


		for (int idx = 0; idx < 4; idx++)
		{
			w[idx] = ww / s[idx];
			h[idx] = hw / s[idx];
			l[idx] = -w[idx] / 2;
			b[idx] = -h[idx] / 2;
			r[idx] = w[idx] / 2;
			t[idx] = h[idx] / 2;
		}

		//gl_panel = new TPanelEx(this);
		//gl_panel->Parent = this;

		//gl_panel->Left = Shape->Left + 1;
		//gl_panel->Top = Shape->Top + 1;
		//gl_panel->Width = Shape->Width - 2;
		//gl_panel->Height = Shape->Height - 2;

		//ww = gl_panel->Width;
		//hw = gl_panel->Height;


		//gl_panel->OnMouseMove = gl_panelMouseMove;
		//gl_panel->OnMouseDown = gl_panelMouseDown;
		//gl_panel->OnMouseUp = gl_panelMouseUp;
		//gl_panel->OnResize = gl_panelResize;
		//gl_panel->OnPaint = gl_panelPaint;

		//gl_panel->Anchors = Shape->Anchors;

		//gl_panel->BorderStyle = bsNone;

		//DoubleBuffered = true;
		//gl_panel->DoubleBuffered = true;
		//gl_panel->FullRepaint = false;

		//hwnd = gl_panel->Handle;
		//hdc = GetDC(hwnd);

		set_dc_pixel_format();
		
		//hglrc = wglCreateContext(hdc);
		//wglMakeCurrent(hdc, hglrc);
		set_gl_params();

		create_fonts();
		create_gl_fonts();

		f = new facet_obj;

		render();




}

CTry_OpenGLView::~CTry_OpenGLView()
{
}

BOOL CTry_OpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView diagnostics

#ifdef _DEBUG
void CTry_OpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CTry_OpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTry_OpenGLDoc* CTry_OpenGLView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTry_OpenGLDoc)));
	return (CTry_OpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

//--------------------------------------------------------------
//	----	Message Handlers		----	Initial Messages
//--------------------------------------------------------------

// ---																							// On Create
int CTry_OpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	hRC=SetUpOpenGL(m_hWnd);
	for(int i=0;i<4;i++) cube[i].x=i*2-3;
	rMyRotation=0;
	//SetTimer(1000,100,NULL);
	
	return 0;
}
// ---																							// On Destroy
void CTry_OpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//KillTimer(1000);

}

//--------------------------------------------------------------
//	----	Message Handlers		----	Drawing
//--------------------------------------------------------------

// ---																							// On Draw
void CTry_OpenGLView::OnDraw(CDC* pDC)
{
	//
}
// ---																							// On Size
void CTry_OpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	HDC hDC = ::GetDC(this->m_hWnd);
	wglMakeCurrent(hDC,hRC);

	//**Write code for GL resize here!
	// get the new size of the client window
    // note that we size according to the height,
    // not the smaller of the height or width.
    glnWidth = (GLsizei)cx;
    glnHeight = (GLsizei)cy;
    gldAspect = (GLdouble)glnWidth/(GLdouble)glnHeight;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,glnWidth,glnHeight);
	gluPerspective(30.0, gldAspect, 1.0, fFarPlane);
	//**End code for GL resize!

	wglMakeCurrent( NULL, NULL );
    ::ReleaseDC( this->m_hWnd, hDC );
	
}
// ---																							// On Paint
void CTry_OpenGLView::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	//
	//// TODO: Add your message handler code here
	//wglMakeCurrent(dc.m_hDC,hRC);
	//
	////**Draw GL here!
	//glClearColor(0.0,1.0,0.0,0.0);
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	////glMatrixMode(GL_MODELVIEW);
	////glLoadIdentity();
	////gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	//gluLookAt(6.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	////glTranslatef(0.0f + wTransformX, 0.0f + wTransformY, -m_z);                      //move object far-near
	////glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);                //rotate object    
	////glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);                //around the axe
	////glRotatef(wAngleZ, 0.0f, 0.0f, 1.0f);                //specified



	/////////////////////////////////////////////////////////////////////////////

	//int idx = 3;

	//set_projection_matrix(xyz_pm_t);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	////glRotatef(-54.74, 1, 0, 0);
	////glRotatef(-135, 0, 0, 1);

	///*GLfloat LightAmbient[4] = { LightAmbient_r, LightAmbient_g, LightAmbient_b, 1 };
	//GLfloat LightDiffuse[4] = { LightDiffuse_r, LightDiffuse_g, LightDiffuse_b, 1 };
	//GLfloat LightPosition[4] = { LightPosition_x, LightPosition_y, LightPosition_z, 0 };
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	//glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glPushMatrix();
	//glScalef(m, m, m);
	//////
	//////
	//////
	//glPopMatrix();


	/////////////////////////////////////////////////////////////////////////////


	//PaintScreen(GL_RENDER);

	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);

	////**End code for draw GL!
	//
	//SwapBuffers(dc.m_hDC);
	//wglMakeCurrent(NULL,NULL);
	//
	//// Do not call CView::OnPaint() for painting messages








	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	PAINTSTRUCT ps;
	memset(&ps, 0, sizeof(ps));
	ps.hdc = hdc;
	ps.fErase = FALSE;
	ps.rcPaint = CRect(0, 0, ww, hw);

	//BeginPaint(hwnd, &ps);
	CPaintDC dc(this);
	wglMakeCurrent(dc.m_hDC, hRC);


	glClearColor(bg_r, bg_g, bg_b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////////////////////////////////////////////////////////////////////
	int idx = 3;
		set_projection_matrix(xyz_pm_t);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (idx == xyz_pm_t)
		{
			glRotatef(-54.74, 1, 0, 0);
			glRotatef(-135, 0, 0, 1);
		}


		GLfloat LightAmbient[4] = { LightAmbient_r, LightAmbient_g, LightAmbient_b, 1 };
		GLfloat LightDiffuse[4] = { LightDiffuse_r, LightDiffuse_g, LightDiffuse_b, 1 };
		GLfloat LightPosition[4] = { LightPosition_x, LightPosition_y, LightPosition_z, 0 };
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glPushMatrix();
		glScalef(m, m, m);


		glEnable(GL_LIGHTING);


		glColor3f(1, 1, 1);

		f->render();
		

		glDisable(GL_LIGHTING);


		glPopMatrix();

		DrawOpenGL_Cube(10, 0, 0, 0, true);
		render_basis();
		render_basis_info();



	////////////////////////////////////////////////////////////////////////////////

	SwapBuffers(dc.m_hDC);
	wglMakeCurrent(NULL, NULL);


}

//--------------------------------------------------------------
//	----	Message Handlers		----	Standart Messages
//--------------------------------------------------------------

void CTry_OpenGLView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	rMyRotation+=5;
	if(rMyRotation>=360) rMyRotation=0;
	RedrawWindow();
	
	CView::OnTimer(nIDEvent);
}
// ---																							// On Mouse Wheel
BOOL CTry_OpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	double step = 5.0f;
	int idx;
		idx = 3;


	if (zDelta < 0)
	{
		m_z += step;

		int xww = xw;
		int yww = yw;
		if (xww > ww) xww -= ww;
		if (yww > hw) yww -= hw;

		s[idx] *= ss;

		//восстанавливаем положение т-ки зумирования

		l[idx] = x[idx] - xww / s[idx];
		t[idx] = y[idx] + yww / s[idx];

		w[idx] = ww / s[idx];
		h[idx] = hw / s[idx];

		r[idx] = l[idx] + w[idx];
		b[idx] = t[idx] - h[idx];

	}
	else
	{
		m_z -= step;

		int xww = xw;
		int yww = yw;
		if (xww > ww) xww -= ww;
		if (yww > hw) yww -= hw;

		s[idx] /= ss;

		//восстанавливаем положение т-ки зумирования

		l[idx] = x[idx] - xww / s[idx];
		t[idx] = y[idx] + yww / s[idx];

		w[idx] = ww / s[idx];
		h[idx] = hw / s[idx];

		r[idx] = l[idx] + w[idx];
		b[idx] = t[idx] - h[idx];


	}


	RedrawWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
// ---																							// On Mouse Move
void CTry_OpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	//if (nFlags& MK_LBUTTON)
	//{
	//	wAngleY += (point.x - mouse_x0);
	//	wAngleX += (point.y - mouse_y0);


	//	mouse_x0 = point.x;  mouse_y0 = point.y;

	//	RedrawWindow();
	//}

	//double step = 0.3f;
	//if (flagMiddleButtonDown)
	//{
	//	if (mouse_x0 - point.x < 0)
	//		wTransformX += step;// fabs(((float)(point.x - mouse_x0)) / 2);
	//	if (mouse_x0 - point.x > 0)
	//		wTransformX -= step;// fabs(((float)(point.x - mouse_x0)) / 2);

	//	if (mouse_y0 - point.y < 0)
	//		wTransformY -= step;// fabs(((float)(point.y - mouse_y0)) / 2);
	//	if (mouse_y0 - point.y > 0)
	//		wTransformY += step;// fabs(((float)(point.y - mouse_y0)) / 2);

	//	mouse_x0 = point.x;
	//	mouse_y0 = point.y;

	//	RedrawWindow();
	//}



	xw = point.x;
	yw = point.y;

	int idx;
	
	idx = 3;


	int xww = xw;
	int yww = yw;
	if (xww > ww) xww -= ww;
	if (yww > hw) yww -= hw;

	x[idx] = l[idx] + xww / s[idx];
	y[idx] = t[idx] - yww / s[idx];

	if (flagMiddleButtonDown)
	{
		l[idx] = lp + (xwp - xw) / s[idx];
		b[idx] = bp - (ywp - yw) / s[idx];
		r[idx] = l[idx] + w[idx];
		t[idx] = b[idx] + h[idx];
	}
	else if (nFlags & MK_LBUTTON)
	{
		ax = float(yw - ywp) / hw * 90;
		ay = float(xw - xwp) / ww * 90;

		ex = matrix::rotate(vector(0, 0, 0), ey, ay * RAD_IN_DEG) * exp;
		ex = ey ^ ex ^ ey;
		ex.normalize();

		ey = matrix::rotate(vector(0, 0, 0), ex, ax * RAD_IN_DEG) * eyp;
		ey = ex ^ ey ^ ex;
		ey.normalize();
	}



	RedrawWindow();


	CView::OnMouseMove(nFlags, point);
}
// ---																							// On Middle Button Down
void CTry_OpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = true;

	mouse_x0 = point.x;  mouse_y0 = point.y;


	///////////////

	int idx;

	idx = 3;


	lp = l[idx];
	bp = b[idx];
	xwp = xw;
	ywp = yw;

	//SetCapture(/*hwnd*/);

	pan = true;
	pan_t = GetTickCount();


	/////////////////


	CView::OnMButtonDown(nFlags, point);
}
// ---																							// On Middle Button UP
void CTry_OpenGLView::OnMButtonUp(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = false;

	CView::OnMButtonUp(nFlags, point);
}
// ---																							// On Left Button Down
void CTry_OpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mouse_x0 = point.x;  mouse_y0 = point.y;

	/////////////////////////////////
	int idx;
	idx = 3;

	lp = l[idx];
	bp = b[idx];
	xwp = xw;
	ywp = yw;

	exp = ex;
	eyp = ey;

	//SetCapture(/*hwnd*/);


	/////////////////////////////



	#define BUFSIZE 512

	// TODO: Add your message handler code here and/or call default
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	HDC hDC = ::GetDC(this->m_hWnd);
	wglMakeCurrent(hDC, hRC);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(BUFSIZE, selectBuf);

	glRenderMode(GL_SELECT);				// Enter the SELECT render mode
	glInitNames();
	glPushName(-1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix((GLdouble)point.x, (GLdouble)(viewport[3] - point.y), 5.0, 5.0, viewport);
	gluPerspective(30.0, gldAspect, 1.0, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
	PaintScreen(GL_SELECT);
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, glnWidth, glnHeight);
	gluPerspective(30.0, gldAspect, 1.0, fFarPlane);

	/*for(int i=0;i<hits;i++)
	{
		cube[selectBuf[3+i*4]].selected=!cube[selectBuf[3+i*4]].selected;
	}*/
	int a = 0;
	if (hits)
	{
		int n = 0; double minz = selectBuf[1];
		for (int i = 1; i < hits; i++)
		{
			if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
		}


		a = selectBuf[3 + n * 4];

		if (a == 105)	flagCubeColor_1 = !flagCubeColor_1;

		cube[selectBuf[3 + n * 4]].selected = !cube[selectBuf[3 + n * 4]].selected;
	}

	wglMakeCurrent(NULL, NULL);
	::ReleaseDC(this->m_hWnd, hDC);

	RedrawWindow();

	CView::OnLButtonDown(nFlags, point);
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

void CTry_OpenGLView::PaintScreen(GLenum mode)
{
	glPushMatrix();

	//glRotated(rMyRotation,0.0,1.0,0.0);

	for(int i=0;i<4;i++)
	{
		if (mode==GL_SELECT) glLoadName(i);
		cube[i].Draw();		
	}

	if (mode == GL_SELECT) glLoadName(105);
	DrawOpenGL_Cube(1, 0, -1, 0, flagCubeColor_1);

	glPopMatrix();
}

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cube
void CTry_OpenGLView::DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor)
{
	/*glPushMatrix();

	glTranslated(0, 0, 0);*/

	param /= 2;

	if (flagColor)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
	else
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);

	//glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);

	glNormal3f(0, 0, 1);

	glVertex3f(-param + cx, param + cy, param + cz);

	glVertex3f(-param + cx, -param + cy, param + cz);

	glVertex3f(param + cx, param + cy, param + cz);

	glVertex3f(param + cx, -param + cy, param + cz);

	glVertex3f(param + cx, param + cy, -param + cz);
	glNormal3f(1, 0, 0);
	glVertex3f(param + cx, -param + cy, -param + cz);

	glVertex3f(-param + cx, param + cy, -param + cz);
	glNormal3f(0, 0, -1);
	glVertex3f(-param + cx, -param + cy, -param + cz);

	glVertex3f(-param + cx, param + cy, param + cz);
	glNormal3f(-1, 0, 0);
	glVertex3f(-param + cx, -param + cy, param + cz);

	glEnd();

	//glPopMatrix();

	return;

	glBegin(GL_QUADS);

	glVertex3f(-param + cx, param + cy, param + cz);

	glVertex3f(param + cx, param + cy, param + cz);

	glVertex3f(param + cx, param + cy, -param + cz);

	glVertex3f(-param + cx, param + cy, -param + cz);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-param + cx, -param + cy, param + cz);

	glVertex3f(param + cx, -param + cy, param + cz);

	glVertex3f(param + cx, -param + cy, -param + cz);

	glVertex3f(-param + cx, -param + cy, -param + cz);
	glEnd();

}
///////////////////////////////////////////////////////


//---------------------------------------------------------------------------
void CTry_OpenGLView::set_projection_matrix(const pm_t type)
{


		
	glViewport(0, 0, glnWidth, glnHeight);


	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (type == norm_pm_t)
	{
		glOrtho(0, glnWidth, 0, glnHeight, -1000, 1000);
	}
	else
	{
		glOrtho(l[type], r[type], b[type], t[type], -1000, 1000);
	}
	if (type == xz_pm_t)
	{
		gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, 1);
	}
	else if (type == xy_pm_t)
	{
		gluLookAt(0, 0, 1, 0, 0, 0, 0, -1, 0);
	}
	else if (type == yz_pm_t)
	{
		gluLookAt(1, 0, 0, 0, 0, 0, 0, 0, 1);
	}
	else if (type == xyz_pm_t)
	{
		vector eye = ex ^ ey;
		gluLookAt(eye.x, eye.y, eye.z, 0, 0, 0, ey.x, ey.y, ey.z);
	}
}
// -------------------------------------------------------------------------- -
void CTry_OpenGLView::create_fonts(void)
{
	LOGFONT logfont;

	logfont.lfHeight = -8;
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(logfont.lfFaceName, "Fixedsys");

	font_h = CreateFontIndirect(&logfont);

	real_font_height = -8;

	logfont.lfHeight = -18;
	strcpy(logfont.lfFaceName, "Arial");

	font_big_h = CreateFontIndirect(&logfont);
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::destroy_fonts(void)
{
	DeleteObject(font_h);
	DeleteObject(font_big_h);
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::create_gl_fonts(void)
{
	SelectObject(hdc, font_h);
	font_list = glGenLists(256);
	wglUseFontBitmaps(hdc, 0, 256, font_list);

	SelectObject(hdc, font_big_h);
	font_big_list = glGenLists(256);
	wglUseFontBitmaps(hdc, 0, 256, font_big_list);
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::destroy_gl_fonts(void)
{
	glDeleteLists(font_list, 1);
	glDeleteLists(font_big_list, 1);
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::print(const GLuint lst, const float& x, const float& y/*, const AnsiString& s*/)
{
	glRasterPos2f(x, y);
	glListBase(lst);
	//glCallLists(s.Length(), GL_UNSIGNED_BYTE, s.c_str());
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::set_dc_pixel_format(void)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		0,
		0,
		0, 0, 0
	};
	int pixel_format = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixel_format, &pfd);
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::set_gl_params(void)
{
	glEnable(GL_DEPTH_TEST);

	glClearDepth(1000);

	glPointSize(10);
	glLineWidth(1);

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_NORMALIZE);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void CTry_OpenGLView::render_basis
(
	const GLclampf ex_r, const GLclampf ex_g, const GLclampf ex_b,
	const GLclampf ey_r, const GLclampf ey_g, const GLclampf ey_b,
	const GLclampf ez_r, const GLclampf ez_g, const GLclampf ez_b,
	const bool invert_ey,
	const float m
)
{
	////
	glColor3f(ex_r, ex_g, ex_b);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(m, 0, 0);
	glEnd();

	glColor3f(ey_r, ey_g, ey_b);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, invert_ey ? -m : m, 0);
	glEnd();

	glColor3f(ez_r, ez_g, ez_b);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, m);
	glEnd();
	
}
//---------------------------------------------------------------------------
void CTry_OpenGLView::render_basis_info
(
	const GLclampf ex_r, const GLclampf ex_g, const GLclampf ex_b,
	const GLclampf ey_r, const GLclampf ey_g, const GLclampf ey_b,
	const GLclampf ez_r, const GLclampf ez_g, const GLclampf ez_b,
	const bool invert_ey,
	const float m
)
{
	glPushMatrix();

	glDisable(GL_DEPTH_TEST);

	glColor3f(ex_r, ex_g, ex_b);
	print(font_big_list, 1.2 * m, 0/*, "X"*/);

	glColor3f(ey_r, ey_g, ey_b);
	print(font_big_list, 0, 1.2 * (invert_ey ? -m : m)/*, "Y"*/);

	glTranslatef(0, 0, 1.2 * m);
	glColor3f(ez_r, ez_g, ez_b);
	print(font_big_list, 0, 0/*, "Z"*/);

	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void CTry_OpenGLView::render(void)
{
	try
	{
		RedrawWindow();
	}
	catch (const std::exception&)
	{

	}
	
}
//---------------------------------------------------------------------------
