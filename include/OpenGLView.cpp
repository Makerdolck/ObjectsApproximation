// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//

#include "stdafx.h"

#include "OpenGLView.h"

bool flagCubeColor_1 = false;
const GLfloat redColor[] = { 1.0f,	0.0f,	0.0f,	1.0f };
const GLfloat blueColor[] = { 0.0f,	0.0f,	1.0f,	1.0f };

const GLfloat simpleElementsColor1[] = { 1.0f,	0.2f,	0.2f,	1.0f };
const GLfloat simpleElementsColor2[] = { 0.0f,	1.0f,	1.0f,	1.0f };

//GLfloat ambientLight[] = { 0.3f, 0.3f,0.3f, 1.0f };
//GLfloat diffuseLight[] = { 0.7f, 0.7f,0.7f, 1.0f };
//GLfloat lightPos[] = { -50.0f, 50.0f,100.0f, 1.0f };

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
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
ON_WM_KEYDOWN()
//ON_WM_RBUTTONDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView construction/destruction

COpenGLView::COpenGLView()
{
	m_z = 10.0f;

	//wAngleY = -10.0f;		//10.0f;
	//wAngleX = 15.0f;		//1.0f;
	
	wAngleY = 0.0f;		//10.0f;
	wAngleX = 0.0f;		//1.0f;
	wAngleZ = 0.0f;		//5.0f;

	wTransformX = 0;
	wTransformY = 0;

	fFarPlane = 10000.0f;

	flagMiddleButtonDown = false;


	flagColor1 = true;
	flagColor2 = true;
	flagColor3 = true;
	flagColor4 = true;
	flagColor5 = true;
	flagColor6 = true;
	flagColor7 = true;

	flagShiftPressed = false;

	objectsArray = nullptr;

	//m_fLineWidth = 0.05;
	// Colors
	m_ClearColorRed = 0.0f;
	m_ClearColorGreen = 0.0f;
	m_ClearColorBlue = 0.2f;


	pointEyeLook.X = 0;
	pointEyeLook.Y = 0;
	pointEyeLook.Z = 100;

	pointAimLook.X = 0;
	pointAimLook.Y = 0;
	pointAimLook.Z = 0;
	
	angleLook = 0;

	vectorRotationX = VectorGeometric(1.f, 0.f, 0.f);
	vectorRotationY = VectorGeometric(0.f, 1.f, 0.f);
	//vectorRotationZ = VectorGeometric(0.f, 0.f, 1.f);
	vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);

	distanceAimEye = pointAimLook.DistanceToPoint(pointEyeLook);
	offsetView_X = 0;
	offsetView_Y = 0;


	rotationStep = 45;
	rotationAngleX = 0;
	rotationAngleY = 0;

	flagRotateAxisX = false;
	flagRotateAxisY = false;

	coordinateDifferenceX = 0,
	coordinateDifferenceY = 0;

	MatrixIdentity(BoxTrans);
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
	GLfloat light0_direction[] = { 0.0,		0.0,	1.0,	0.0 },
			light1_direction[] = { 0.0,		1.0,	1.0,	0.0 },
			light2_direction[] = { 0.0,		-1.0,	1.0,	0.0 },
			light3_direction[] = { 0.0,		1.0,	0.0,	0.0 },
			light4_direction[] = { 0.0,		0.0,	-1.0,	0.0 };

	GLfloat light_diffuse[]			= { 1,		1,		1 },
			light_diffuseMiddle[]	= { 0.2f,	0.2f,	0.2f },
			light_diffuseDark[]		= { 0.0f,	0.0f,	0.0f };




	wglMakeCurrent(pDC->m_hDC, hRC);
	//**Draw GL here!
	//glClearColor(0.0,0.0,0.0,0.0);

	glClearDepth(1.0f);                                  //Specifies the clear value for the depth buffer 
	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);             //Set Background Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	pointEyeLook.X,		pointEyeLook.Y,		pointEyeLook.Z,
				pointAimLook.X,		pointAimLook.Y,		pointAimLook.Z,
				0.0f,		1.0f,		0.0f);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);

	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuseDark);
	//glLightfv(GL_LIGHT1, GL_POSITION, light1_direction);

	//glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuseDark);
	//glLightfv(GL_LIGHT2, GL_POSITION, light2_direction);

	//glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuseDark);
	//glLightfv(GL_LIGHT3, GL_POSITION, light3_direction);

	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_direction);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


	glTranslatef((GLfloat)centerOfAllObjects.X, (GLfloat)centerOfAllObjects.Y, (GLfloat)centerOfAllObjects.Z);
	

	float trans[16];
	
	MatrixCopy(BoxTrans, trans);
	if (flagRotateAxisY)
	{
		//coordinateDifferenceX
		MatrixCopy(BoxTrans, trans);

		MatrixRotateY(trans, roty - prev_roty);

		MatrixCopy(trans, BoxTrans);  //saving last transformation
		MatrixTranspose(trans); //making row-major for openGL
	}
	
	if (flagRotateAxisX)
	{
		//coordinateDifferenceX
		MatrixCopy(BoxTrans, trans);

		MatrixRotateX(trans, rotx - prev_rotx);

		MatrixCopy(trans, BoxTrans);  //saving last transformation
		MatrixTranspose(trans); //making row-major for openGL
	}

	glMultMatrixf(trans);
	prev_rotx = rotx;
	prev_roty = roty;


	glTranslatef((GLfloat )-centerOfAllObjects.X, (GLfloat)-centerOfAllObjects.Y, (GLfloat)-centerOfAllObjects.Z);


	PaintScene(GL_RENDER);


	//DrawOpenGL_Cube(10, 0, 0, 0, true);            // Draws a sphere with translation <100, 0, 0>


	glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHT2);
	//glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POINT_SMOOTH);
	//glPopMatrix();

	//**End code for draw GL!

	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
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

//--------------------------------------------------------------
//	----	Message Handlers		----	Standart Messages
//--------------------------------------------------------------

// ---																							// On Mouse Wheel
BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	double step = 3.0f+fabs(((int)pointEyeLook.Z)/25);
	LineGeometric		lineOffset = LineGeometric(pointAimLook, vectorRotationZ * (-1));

	/*if (zDelta < 0)
		m_z += step;
	else
		m_z -= step;*/



	if (zDelta < 0)
	{
		distanceAimEye += step;
		//pointEyeLook.Z += step;
	}	
	else
	{
		distanceAimEye -= step;
		//pointEyeLook.Z -= step;
	}

	pointEyeLook = lineOffset.CreatePointOnDistance(distanceAimEye);


	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
// ---																							// On Mouse Move
void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags& MK_LBUTTON && flagShiftPressed)
	{
		if (point.x - prev_x != 0)
		{
			flagRotateAxisY = true;
			//coordinateDifferenceX = point.x - prev_x;
			roty += (point.x - prev_x) * 0.5;
		}
		else
			flagRotateAxisY = false;
		//roty += (point.x - prev_x) * 0.5;
		if (point.y - prev_y != 0)
		{
			flagRotateAxisX = true;
			//coordinateDifferenceY = point.y - prev_y;
			rotx += (point.y - prev_y) * 0.5;
		}
		else
			flagRotateAxisX = false;
		//rotx += (point.y - prev_y) * 0.5;

		Invalidate(FALSE);		//The end

	}

	if (flagMiddleButtonDown)
	{
		LineGeometric		lineOffsetEye,
							lineOffsetAim;

		lineOffsetEye.Point = pointEyeLook;
		lineOffsetAim.Point = pointAimLook;

		PointGeometric		pOld, pNew;
		pOld = PointGeometric(mouse_x0, mouse_y0);
		pNew = PointGeometric(point.x, point.y);


		double	stepOffsetX = distanceAimEye * fabs(pOld.X - pNew.X) / (glnWidth / 2.f),
				stepOffsetY = distanceAimEye * fabs(pOld.Y - pNew.Y) / (glnHeight / 2.f);


		if (mouse_x0 - point.x < 0)
		{
			lineOffsetEye.Vector = vectorRotationX * (-1);
			lineOffsetAim.Vector = vectorRotationX * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			offsetView_X -= stepOffsetX;
		}
		if (mouse_x0 - point.x > 0)
		{
			lineOffsetEye.Vector = vectorRotationX;
			lineOffsetAim.Vector = vectorRotationX;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			offsetView_X = stepOffsetX;
		}

		if (mouse_y0 - point.y < 0)
		{
			lineOffsetEye.Vector = vectorRotationY;
			lineOffsetAim.Vector = vectorRotationY;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			offsetView_Y += stepOffsetY;
		}
		if (mouse_y0 - point.y > 0)
		{
			lineOffsetEye.Vector = vectorRotationY * (-1);
			lineOffsetAim.Vector = vectorRotationY * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			offsetView_Y -= stepOffsetY;
		}

		Invalidate(FALSE);
	}

	mouse_x0 = point.x;
	mouse_y0 = point.y;

	prev_x = point.x;
	prev_y = point.y;


	CView::OnMouseMove(nFlags, point);
}
// ---																							// On Middle Button Down
void COpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	flagMiddleButtonDown = true;

	//mouse_x0 = point.x;  mouse_y0 = point.y;

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

	prev_x = point.x;
	prev_y = point.y;


	if (flagShiftPressed)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}

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
	PaintScene(GL_SELECT);
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

	ObjectApprox * objApprox;
	int a = 0;
	if (hits)
	{
		int n = 0; double minz = selectBuf[1];
		for (int i = 1; i < hits; i++)
		{
			if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
		}

		a = selectBuf[3 + n * 4];

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
	else																// Make all elements not selected
	{
		if (objectsArray != nullptr)
		{
			for (int i = 0; i < (int)objectsArray->size(); i++)
			{
				objApprox = objectsArray->operator[](i);

				objApprox->flagSelected = false;

			}
		}
	}

	wglMakeCurrent(NULL, NULL);
	::ReleaseDC(this->m_hWnd, hDC);

	Invalidate(FALSE);


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

	//glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	

	hRC = wglCreateContext(hDC);
	::ReleaseDC(hWnd, hDC);

	return hRC;
}
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Paint Scene
void COpenGLView::PaintScene(GLenum mode)
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

	PointApprox			*pointA			= new PointApprox();
	LineSegmentApprox	*lineSegmentA	= new LineSegmentApprox();
	RectangleApprox		*rectangleA		= new RectangleApprox();
	CircleApprox		*circleA		= new CircleApprox();
	CylinderApprox		*cylinderA		= new CylinderApprox();
	ConeApprox			*coneA			= new ConeApprox();
	SphereApprox		*sphereA		= new SphereApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;


		
		//	---	---	---	---	---									// Set color

		if (objApprox->objMath->GetName() == cylinderA->GetName()	||
			objApprox->objMath->GetName() == coneA->GetName()		||
			objApprox->objMath->GetName() == sphereA->GetName()		||
			objApprox->objMath->GetName() == rectangleA->GetName())
		{
			if (objApprox->flagSelected)
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
			else
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);
		}
		else
		{
			if (objApprox->flagSelected)
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, simpleElementsColor1);
			else
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, simpleElementsColor2);
		}



		/*if (objApprox->flagSelected)
			glColor3f(0.2f, 0.85f, 0.1f);
		else
			glColor3f(0.8f, 0.35f, 0.8f);*/

		//	---	---	---	---	---										// Set name
		if (mode == GL_SELECT) glLoadName(objApprox->objID);

		//	---	---	---	---	---											// Draw via Triangles
		if (objApprox->objMath->GetName() == cylinderA->GetName()	||
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
		//	---	---	---	---	---											// Draw PlaneViaRectangle
		if (objApprox->objMath->GetName() == rectangleA->GetName())
		{
			DrawOpenGL_PlaneViaRectangle(*objApprox->objMath);
		}

	}	

	delete pointA;
	delete lineSegmentA;
	delete rectangleA;
	delete circleA;
	delete cylinderA;
	delete coneA;
	delete sphereA;

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
		
		for (int i = 0; i < (int)obj.Mesh.points.size(); i++)
		{
			glNormal3f((GLfloat)obj.Mesh.vectorsNormal[0].X, (GLfloat)obj.Mesh.vectorsNormal[0].Y, (GLfloat)obj.Mesh.vectorsNormal[0].Z);
			glVertex3f(	(GLfloat)obj.Mesh.points[i].X /*- centerOfAllObjects.X*/, 
						(GLfloat)obj.Mesh.points[i].Y /*- centerOfAllObjects.Y*/, 
						(GLfloat)obj.Mesh.points[i].Z /*- centerOfAllObjects.Z*/);
		}		

	glEnd();

	glLineWidth(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Point
void COpenGLView::DrawOpenGL_Point(PointApprox *obj)
{
	glPointSize(10);

	glBegin(GL_POINTS);

		//glNormal3f((GLfloat)0/*obj->Line.Vector.X*/, (GLfloat)0/*obj->Line.Vector.Y*/, (GLfloat)1/*obj->Line.Vector.Z*/);
		glVertex3d(	(GLfloat)obj->X /*- centerOfAllObjects.X*/,	
					(GLfloat)obj->Y /*- centerOfAllObjects.Y*/, 	
					(GLfloat)obj->Z /*- centerOfAllObjects.Z*/);
		
	glEnd();

	glPointSize(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL LineSegment
void COpenGLView::DrawOpenGL_LineSegment(LineSegmentApprox *obj)
{
	glLineWidth(3);
	glBegin(GL_LINES);
		glNormal3f((GLfloat)obj->Vector.X, (GLfloat)obj->Vector.Y, (GLfloat)obj->Vector.Z);
		glVertex3d(	(GLfloat)obj->PointStart.X /*- centerOfAllObjects.X*/,	
					(GLfloat)obj->PointStart.Y /*- centerOfAllObjects.Y*/,
					(GLfloat)obj->PointStart.Z /*- centerOfAllObjects.Z*/);

		glNormal3f((GLfloat)obj->Vector.X, (GLfloat)obj->Vector.Y, (GLfloat)obj->Vector.Z);
		glVertex3d(	(GLfloat)obj->PointEnd.X /*- centerOfAllObjects.X*/,
					(GLfloat)obj->PointEnd.Y /*- centerOfAllObjects.Y*/,	
					(GLfloat)obj->PointEnd.Z /*- centerOfAllObjects.Z*/);
	glEnd();


	glLineWidth(1);
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Plane
void COpenGLView::DrawOpenGL_PlaneViaRectangle(GeomObjectApprox obj)
{
	glBegin(GL_TRIANGLES);
	for (int i = 2; i < (int)obj.Mesh.points.size(); i += 3)
	{
		//glColor3f((double)(i%10)/10, (double)(i % 10) /10, (double)(i % 10) /10);
		
		//if (i % 2 == 0)
			//glNormal3f((GLfloat)obj.Line.Vector.X,(GLfloat)obj.Line.Vector.Y, (GLfloat)obj.Line.Vector.Z);
		/*if (i%2==1)
			glNormal3f((-1)*(GLfloat)obj.Line.Vector.X, (-1) * (GLfloat)obj.Line.Vector.Y, (-1) * (GLfloat)obj.Line.Vector.Z);*/

		glNormal3f((GLfloat)obj.Line.Vector.X, (GLfloat)obj.Line.Vector.Y, (GLfloat)obj.Line.Vector.Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i].X /*- centerOfAllObjects.X*/,		
					(GLfloat)obj.Mesh.points[i].Y /*- centerOfAllObjects.Y*/,		
					(GLfloat)obj.Mesh.points[i].Z /*- centerOfAllObjects.Z*/);

		glNormal3f((GLfloat)obj.Line.Vector.X, (GLfloat)obj.Line.Vector.Y, (GLfloat)obj.Line.Vector.Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i - 1].X /*- centerOfAllObjects.X*/,
					(GLfloat)obj.Mesh.points[i - 1].Y /*- centerOfAllObjects.Y*/,	
					(GLfloat)obj.Mesh.points[i - 1].Z /*- centerOfAllObjects.Z*/);

		glNormal3f((GLfloat)obj.Line.Vector.X, (GLfloat)obj.Line.Vector.Y, (GLfloat)obj.Line.Vector.Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i - 2].X /*- centerOfAllObjects.X*/,	
					(GLfloat)obj.Mesh.points[i - 2].Y /*- centerOfAllObjects.Y*/,
					(GLfloat)obj.Mesh.points[i - 2].Z /*- centerOfAllObjects.Z*/);

		
	}
	glEnd();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Object via Triangles
void COpenGLView::DrawOpenGL_ObjViaTriangles(GeomObjectApprox obj)
{
	glBegin(GL_TRIANGLES);
	for (int i = 2; i < (int)obj.Mesh.points.size() /*&&i<=dd*/; i += 3)
	{
		//glColor3f((double)(i%10)/10, (double)(i % 10) /10, (double)(i % 10) /10);

		glNormal3f((GLfloat)obj.Mesh.vectorsNormal[i / 3].X, (GLfloat)obj.Mesh.vectorsNormal[i / 3].Y, (GLfloat)(GLfloat)obj.Mesh.vectorsNormal[i / 3].Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i].X /*- centerOfAllObjects.X*/,		
					(GLfloat)obj.Mesh.points[i].Y /*- centerOfAllObjects.Y*/,
					(GLfloat)obj.Mesh.points[i].Z /*- centerOfAllObjects.Z*/);
		
		glNormal3f((GLfloat)obj.Mesh.vectorsNormal[i / 3].X, (GLfloat)obj.Mesh.vectorsNormal[i / 3].Y, (GLfloat)(GLfloat)obj.Mesh.vectorsNormal[i / 3].Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i - 1].X /*- centerOfAllObjects.X*/,
					(GLfloat)obj.Mesh.points[i - 1].Y /*- centerOfAllObjects.Y*/,	
					(GLfloat)obj.Mesh.points[i - 1].Z /*- centerOfAllObjects.Z*/);
		
		glNormal3f((GLfloat)obj.Mesh.vectorsNormal[i / 3].X, (GLfloat)obj.Mesh.vectorsNormal[i / 3].Y, (GLfloat)(GLfloat)obj.Mesh.vectorsNormal[i / 3].Z);
		glVertex3f(	(GLfloat)obj.Mesh.points[i - 2].X /*- centerOfAllObjects.X*/,	
					(GLfloat)obj.Mesh.points[i - 2].Y /*- centerOfAllObjects.Y*/,	
					(GLfloat)obj.Mesh.points[i - 2].Z /*- centerOfAllObjects.Z*/);

	}
	glEnd();
}
///////////////////////////////////////////////////////




//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// On Key Down for __Test__
void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	float fraction = 0.1f;

	double radiusOfLook = pointAimLook.DistanceToPoint(pointEyeLook);

	LineGeometric		lineOffset;
	VectorGeometric		vectorRotationX_Old = vectorRotationX,
						vectorRotationY_Old = vectorRotationY;

	if (nChar == VK_SHIFT)
	{
		flagShiftPressed = true;
	}



	//VectorGeometric		vectorRotationX_Old = vectorRotationX,
	//					vectorRotationY_Old = vectorRotationY;

	//double	stepOffsetX = distanceAimEye * sqrt(2) * fabs(pOld.X - pNew.X) / (glnWidth / 8.f),
	//		stepOffsetY = distanceAimEye * sqrt(2) * fabs(pOld.Y - pNew.Y) / (glnHeight / 8.f);

	switch (nChar) {
	case  VK_RIGHT :

		//lineOffset = LineGeometric(pointEyeLook, vectorRotationY);

		//pointEyeLook = lineOffset.CreatePointOnDistance(stepOffsetY);

		//vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		//vectorRotationY = vectorRotationZ ^ vectorRotationX;

		//if (vectorRotationY* vectorRotationY_Old < 0)
		//{
		//	vectorRotationY = vectorRotationY * (-1);
		//}


		rotationAngleY = (-1) * rotationStep;
		//rotationAngleY = rotationStep;
		wAngleY += rotationStep;
		break;
	case VK_LEFT:

		//lineOffset = LineGeometric(pointEyeLook, vectorRotationY * (-1));

		//pointEyeLook = lineOffset.CreatePointOnDistance(stepOffsetY);

		//vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		//vectorRotationY = vectorRotationZ ^ vectorRotationX;

		//if (vectorRotationY* vectorRotationY_Old < 0)
		//{
		//	vectorRotationY = vectorRotationY * (-1);
		//}


		rotationAngleY = rotationStep;
		//rotationAngleY = (-1)*rotationStep;
		wAngleY -= rotationStep;
		break;
	case VK_UP:
		

		rotationAngleX =  rotationStep;
		//rotationAngleX = (-1) * rotationStep;
		wAngleX -= rotationStep;
		break;
	case VK_DOWN:
	



		rotationAngleX = (-1) * rotationStep;
		//rotationAngleX = rotationStep;
		wAngleX += rotationStep;
		break;
	}


	Invalidate(FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void COpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
	{
		flagShiftPressed = false;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
