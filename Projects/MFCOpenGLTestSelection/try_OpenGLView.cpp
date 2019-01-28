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
	r=0;
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
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	wglMakeCurrent(dc.m_hDC,hRC);
	
	//**Draw GL here!
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	//gluLookAt(6.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glTranslatef(0.0f + wTransformX, 0.0f + wTransformY, -m_z);                      //move object far-near
	glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);                //rotate object    
	glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);                //around the axe
	glRotatef(wAngleZ, 0.0f, 0.0f, 1.0f);                //specified

	PaintScreen(GL_RENDER);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//**End code for draw GL!
	
	SwapBuffers(dc.m_hDC);
	wglMakeCurrent(NULL,NULL);
	
	// Do not call CView::OnPaint() for painting messages
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Standart Messages
//--------------------------------------------------------------

void CTry_OpenGLView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	r+=5;
	if(r>=360) r=0;
	RedrawWindow();
	
	CView::OnTimer(nIDEvent);
}
// ---																							// On Mouse Wheel
BOOL CTry_OpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	double step = 5.0f;

	if (zDelta < 0)
		m_z += step;
	else
		m_z -= step;


	RedrawWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
// ---																							// On Mouse Move
void CTry_OpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags& MK_LBUTTON)
	{
		wAngleY += (point.x - mouse_x0);
		wAngleX += (point.y - mouse_y0);


		mouse_x0 = point.x;  mouse_y0 = point.y;

		RedrawWindow();
	}

	double step = 0.3f;
	if (flagMiddleButtonDown)
	{
		if (mouse_x0 - point.x < 0)
			wTransformX += step;// fabs(((float)(point.x - mouse_x0)) / 2);
		if (mouse_x0 - point.x > 0)
			wTransformX -= step;// fabs(((float)(point.x - mouse_x0)) / 2);

		if (mouse_y0 - point.y < 0)
			wTransformY -= step;// fabs(((float)(point.y - mouse_y0)) / 2);
		if (mouse_y0 - point.y > 0)
			wTransformY += step;// fabs(((float)(point.y - mouse_y0)) / 2);

		mouse_x0 = point.x;
		mouse_y0 = point.y;

		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}
// ---																							// On Middle Button Down
void CTry_OpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = true;

	mouse_x0 = point.x;  mouse_y0 = point.y;

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

	//glRotated(r,0.0,1.0,0.0);

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






