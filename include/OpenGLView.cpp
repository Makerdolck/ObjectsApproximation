// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//

#include "stdafx.h"

#include "OpenGLView.h"

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

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
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

COpenGLView::COpenGLView()
{
	m_z = 10.0f;

	wAngleY = -10.0f;		//10.0f;
	wAngleX = 15.0f;		//1.0f;
	wAngleZ = 0.0f;		//5.0f;

	wTransformX = 0;
	wTransformY = 0;

	fFarPlane = 500.0f;

	flagMiddleButtonDown = false;


	flagColor1 = true;
	flagColor2= true; 
	flagColor3= true; 
	flagColor4= true; 
	flagColor5= true; 
	flagColor6= true; 
	flagColor7 = true;

	objectsArray = nullptr;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

//--------------------------------------------------------------
//	----	Message Handlers		----	Initial Messages
//--------------------------------------------------------------

// ---																							// On Create
int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	hRC=SetUpOpenGL(m_hWnd);
	///*for(int i=0;i<4;i++) cube[i].x=i*2-3;
	//r=0;*/
	//SetTimer(1000,100,NULL);
	
	return 0;
}
// ---																							// On Destroy
void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	//KillTimer(1000);

}

//--------------------------------------------------------------
//	----	Message Handlers		----	Drawing
//--------------------------------------------------------------

// ---																							// On Draw
void COpenGLView::OnDraw(CDC* pDC)
{
	//
}
// ---																							// On Size
void COpenGLView::OnSize(UINT nType, int cx, int cy) 
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
void COpenGLView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	wglMakeCurrent(dc.m_hDC,hRC);
	
	//**Draw GL here!
	//glClearColor(0.0,0.0,0.0,0.0);

	glClearDepth(1.0f);                                  //Specifies the clear value for the depth buffer 
	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);             //Set Background Color
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

// ---																							// On Mouse Wheel
BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags& MK_LBUTTON)
	{
		wAngleY += (point.x - mouse_x0);
		wAngleX += (point.y - mouse_y0);


		mouse_x0 = point.x;  mouse_y0 = point.y;

		RedrawWindow();
	}

	double step = 1.0f;
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
void COpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = true;

	mouse_x0 = point.x;  mouse_y0 = point.y;

	CView::OnMButtonDown(nFlags, point);
}
// ---																							// On Middle Button UP
void COpenGLView::OnMButtonUp(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = false;

	CView::OnMButtonUp(nFlags, point);
}
// ---																							// On Left Button Down
void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
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
	
	ObjectApprox* objApprox;
	int a = 0;
	if (hits)
	{
		int n = 0; double minz = selectBuf[1];
		for (int i = 1; i < hits; i++)
		{
			if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
		}


		a = selectBuf[3 + n * 4];

		//if (a == 105)	flagCubeColor_1 = !flagCubeColor_1;

		//cube[selectBuf[3 + n * 4]].selected = !cube[selectBuf[3 + n * 4]].selected;

		if (objectsArray != nullptr)
		{
			for (int i = 0; i < (int)objectsArray->size(); i++)
			{
				objApprox = objectsArray->operator[](i);

				if (objApprox->objID == a)
				{
					objApprox->flagSelected = !objApprox->flagSelected;
				}
			}
		}
	}

	wglMakeCurrent(NULL, NULL);
	::ReleaseDC(this->m_hWnd, hDC);

	RedrawWindow();

	CView::OnLButtonDown(nFlags, point);
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Set Up OpenGL Settings
HGLRC COpenGLView::SetUpOpenGL(HWND hWnd)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), // struct size 
		1,                              // Version number
		PFD_DRAW_TO_WINDOW |    // Flags, draw to a window,
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,          // RGBA pixel values
		24,                     // 24-bit color
		0, 0, 0,                // RGB bits & shift sizes.
		0, 0, 0,                // Don't care about them
		0, 0,                   // No alpha buffer info
		0, 0, 0, 0, 0,          // No accumulation buffer
		32,                     // 32-bit depth buffer
		0,                      // No stencil buffer
		0,                      // No auxiliary buffers
		PFD_MAIN_PLANE,         // Layer type
		0,                      // Reserved (must be 0)
		0,                      // No layer mask
		0,                      // No visible mask
		0                       // No damage mask
	};

	int nMyPixelFormatID;
	HDC hDC;
	HGLRC hRC;

	hDC = ::GetDC(hWnd);
	nMyPixelFormatID = ChoosePixelFormat(hDC, &pfd);

	if (!nMyPixelFormatID)
	{
		//MessageBox(/*hWnd,*/ "May be you do not install OpenGL!", "Error!", MB_ICONERROR | MB_OK);
		
		exit(1);
	}
	if (!SetPixelFormat(hDC, nMyPixelFormatID, &pfd))
	{
		//MessageBox(/*hWnd,*/ "Can not set pixel format for OpenGL!", "Error!", MB_ICONERROR | MB_OK);
		
		exit(1);
	}

	glCullFace(GL_BACK);

	hRC = wglCreateContext(hDC);
	::ReleaseDC(hWnd, hDC);

	return hRC;
}
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Paint Screen
void COpenGLView::PaintScreen(GLenum mode)
{
	glPushMatrix();

	//glRotated(r,0.0,1.0,0.0);

	//if (mode == GL_SELECT) glLoadName(105);
	//DrawOpenGL_Cube(1, 0, -1, 0, flagCubeColor_1);


	if (objectsArray == nullptr)
	{
		glPopMatrix();
		return;
	}


	ObjectApprox		*objApprox;

	PointApprox			*pointA = new PointApprox();
	LineSegmentApprox	*lineSegmentA = new LineSegmentApprox();
	RectangleApprox		*rectangleA = new RectangleApprox();
	CircleApprox		*circleA = new CircleApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();
	ConeApprox			*coneA = new ConeApprox();
	SphereApprox		*sphereA = new SphereApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;


		//	---	---	---	---	---										// Set color
		if (objApprox->flagSelected)
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
		else
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);

		if (objApprox->flagSelected)
			glColor3f(0.2f, 0.85f, 0.1f);
		else
			glColor3f(0.8f, 0.35f, 0.8f);

		//	---	---	---	---	---										// Set name
		if (mode == GL_SELECT) glLoadName(objApprox->objID);

		//	---	---	---	---	---											// Draw via Triangles
		if (objApprox->objMath->GetName() == rectangleA->GetName()	||
			objApprox->objMath->GetName() == cylinderA->GetName()	||
			objApprox->objMath->GetName() == coneA->GetName()		||
			objApprox->objMath->GetName() == sphereA->GetName())
		{
			DrawOpenGL_ObjViaTriangles(*objApprox->objMath);
		}
		//	---	---	---	---	---											// Draw Circle
		if (objApprox->objMath->GetName() == circleA->GetName())
		{
			DrawOpenGL_Circle(*objApprox->objMath);
		}
		//	---	---	---	---	---											// Draw Point
		if (objApprox->objMath->GetName() == pointA->GetName())
		{
			DrawOpenGL_Point((PointApprox*)objApprox->objMath);
		}
		//	---	---	---	---	---											// Draw Line Segment
		if (objApprox->objMath->GetName() == lineSegmentA->GetName())
		{
			DrawOpenGL_LineSegment((LineSegmentApprox*)objApprox->objMath);
		}

	}

	glPopMatrix();
}
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cube (Test)
void COpenGLView::DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor)
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
	glNormal3f(0, 0, 1);
	glVertex3f(-param + cx, -param + cy, -param + cz);

	glVertex3f(-param + cx, param + cy, param + cz);
	glNormal3f(1, 0, 0);
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

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Circle
void COpenGLView::DrawOpenGL_Circle(GeomObjectApprox obj)
{
	glLineWidth(3);

	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < obj.Mesh.points.size(); i++)
	{
		glVertex3f(obj.Mesh.points[i].X, obj.Mesh.points[i].Y, obj.Mesh.points[i].Z);
	}
	glNormal3f(obj.Mesh.vectorsNormal[0].X, obj.Mesh.vectorsNormal[0].Y, obj.Mesh.vectorsNormal[0].Z);

	glEnd();

	glLineWidth(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Point
void COpenGLView::DrawOpenGL_Point(PointApprox *obj)
{
	glPointSize(5);

	glBegin(GL_POINTS);
	glVertex3d(obj->X + 20, obj->Y + 20, obj->Z + 20);
	glNormal3f(obj->Line.Vector.X, obj->Line.Vector.Y, obj->Line.Vector.Z);
	glEnd();

	glPointSize(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL LineSegment
void COpenGLView::DrawOpenGL_LineSegment(LineSegmentApprox *obj)
{
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex3d(obj->PointStart.X, obj->PointStart.Y, obj->PointStart.Z);
		glVertex3d(obj->PointEnd.X, obj->PointEnd.Y, obj->PointEnd.Z);
		glNormal3f(obj->Vector.X, obj->Vector.Y, obj->Vector.Z);
	glEnd();


	glLineWidth(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Object via Triangles
void COpenGLView::DrawOpenGL_ObjViaTriangles(GeomObjectApprox obj)
{
	glBegin(GL_TRIANGLES);
	for (int i = 2; i < obj.Mesh.points.size() /*&&i<=dd*/; i += 3)
	{
		//glColor3f((double)(i%10)/10, (double)(i % 10) /10, (double)(i % 10) /10);

		glVertex3f(obj.Mesh.points[i].X, obj.Mesh.points[i].Y, obj.Mesh.points[i].Z);
		glVertex3f(obj.Mesh.points[i - 1].X, obj.Mesh.points[i - 1].Y, obj.Mesh.points[i - 1].Z);
		glVertex3f(obj.Mesh.points[i - 2].X, obj.Mesh.points[i - 2].Y, obj.Mesh.points[i - 2].Z);

		glNormal3f(obj.Mesh.vectorsNormal[i / 3].X, obj.Mesh.vectorsNormal[i / 3].Y, obj.Mesh.vectorsNormal[i / 3].Z);
	}
	glEnd();
}
///////////////////////////////////////////////////////





