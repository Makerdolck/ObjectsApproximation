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
ON_WM_RBUTTONDOWN()
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
	//gluLookAt(0.0, 0.0, 100.0, 
	//			0.0, 0.0,	0.0, 
	//			0.0, 1.0,	0.0);	//gluLookAt(6.0, 4.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


	/*gluLookAt(	xEyeLook,			1.0f,	zEyeLook,
				xEyeLook + xPointLook, 1.0f,	zEyeLook + zPointLook,
				0.0f,			1.0f,	0.0f);*/

	gluLookAt(	pointEyeLook.X,		pointEyeLook.Y,		pointEyeLook.Z,
				pointAimLook.X,		pointAimLook.Y,		pointAimLook.Z,
				vectorRotationY.X,	vectorRotationY.Y,	vectorRotationY.Z);
				//0.0f,		1.0f,		0.0f);




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

	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);




	//glTranslatef(0.0f + wTransformX, 0.0f + wTransformY, (GLfloat)-m_z);                    //move object far-near

	//glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);													//rotate object    
	//glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);													//around the axe
	//glRotatef(wAngleZ, 0.0f, 0.0f, 1.0f);													//specified
	
	//glTranslatef(0.0f + wTransformX, 0.0f + wTransformY, (GLfloat)+m_z);

	glTranslatef((GLfloat)centerOfAllObjects.X, (GLfloat)centerOfAllObjects.Y, (GLfloat)centerOfAllObjects.Z);
	/*glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);
	glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);*/

	//glRotatef(wAngleX, (GLfloat)vectorRotationX.X, (GLfloat)vectorRotationX.Y, (GLfloat)vectorRotationX.Z);
	//glRotatef(wAngleY, (GLfloat)vectorRotationY.X, (GLfloat)vectorRotationY.Y, (GLfloat)vectorRotationY.Z);


	glTranslatef((GLfloat )-centerOfAllObjects.X, (GLfloat)-centerOfAllObjects.Y, (GLfloat)-centerOfAllObjects.Z);


	PaintScene(GL_RENDER);



	//glBegin(GL_TRIANGLES);
	//
	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);

	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(-10, -10, 0);
	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(-10, 10, 0);
	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(10, -10, 0);

	//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);

	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(10, 10, 0);
	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(-10, 10, 0);
	//	glNormal3f(0.f, 0.f, 1.f);
	//	glVertex3f(10, -10, 0);

	//glEnd();








	glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHT1);
	//glDisable(GL_LIGHT2);
	//glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT4);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

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
	if (nFlags& MK_LBUTTON)
	{
		//double radiusOfLook = pointAimLook.DistanceToPoint(pointEyeLook);

		//wAngleY += (point.x - mouse_x0);
		//wAngleX += (point.y - mouse_y0);

		

		PointGeometric		pOld, pNew;
		pOld = PointGeometric(mouse_x0, mouse_y0);
		pNew = PointGeometric(point.x, point.y);

		VectorGeometric		vectorRotationX_Old = vectorRotationX,
							vectorRotationY_Old = vectorRotationY;

		LineGeometric		lineOffset;

		// ---																						// Changing		Y-vector
		if (pOld.Y < pNew.Y)
		{
			lineOffset = LineGeometric(pointEyeLook, vectorRotationY);

		}
		else
		{
			lineOffset = LineGeometric(pointEyeLook, vectorRotationY * (-1));

		}

		pointEyeLook = lineOffset.CreatePointOnDistance(fabs(pOld.Y - pNew.Y));

		vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		vectorRotationY = vectorRotationZ ^ vectorRotationX;
		
		if (vectorRotationY * vectorRotationY_Old < 0)
		{
			vectorRotationY = vectorRotationY * (-1);
		}

		// ---																						// Changing		X-vector
		if (pOld.X > pNew.X)
		{
			lineOffset = LineGeometric(pointEyeLook, vectorRotationX);

		}
		else
		{
			lineOffset = LineGeometric(pointEyeLook, vectorRotationX * (-1));

		}

		pointEyeLook = lineOffset.CreatePointOnDistance(fabs(pOld.X - pNew.X));

		vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		vectorRotationX = vectorRotationZ ^ vectorRotationY;

		if (vectorRotationX * vectorRotationX_Old < 0)
		{
			vectorRotationX = vectorRotationX * (-1);
		}

		// ---																						//distance between camera and Aim correction
		lineOffset = LineGeometric(pointAimLook, vectorRotationZ * (-1));
		
		pointEyeLook = lineOffset.CreatePointOnDistance(distanceAimEye);


		Invalidate(FALSE);
	}

	//GLfloat step = 0.5f + (GLfloat)fabs(((int)m_z) / 50);
	GLfloat stepX = (GLfloat)fabs(mouse_x0 - point.x)/ 250* distanceAimEye,
			stepY = (GLfloat)fabs(mouse_y0 - point.y)/ 250* distanceAimEye;

	if (flagMiddleButtonDown)
	{
		//if (mouse_x0 - point.x < 0)
		//	wTransformX += step;// fabs(((float)(point.x - mouse_x0)) / 2);
		//if (mouse_x0 - point.x > 0)
		//	wTransformX -= step;// fabs(((float)(point.x - mouse_x0)) / 2);

		//if (mouse_y0 - point.y < 0)
		//	wTransformY -= step;// fabs(((float)(point.y - mouse_y0)) / 2);
		//if (mouse_y0 - point.y > 0)
		//	wTransformY += step;// fabs(((float)(point.y - mouse_y0)) / 2);

		LineGeometric		lineOffsetEye,
							lineOffsetAim;

		lineOffsetEye.Point = pointEyeLook;
		lineOffsetAim.Point = pointAimLook;


		if (mouse_x0 - point.x < 0)
		{
			lineOffsetEye.Vector = vectorRotationX * (-1);
			lineOffsetAim.Vector = vectorRotationX * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			/*pointEyeLook.X-= step;
			pointAimLook.X -= step;*/
		}
		if (mouse_x0 - point.x > 0)
		{
			lineOffsetEye.Vector = vectorRotationX;
			lineOffsetAim.Vector = vectorRotationX;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			/*pointEyeLook.X += step;
			pointAimLook.X += step;*/
		}

		if (mouse_y0 - point.y < 0)
		{
			lineOffsetEye.Vector = vectorRotationY;
			lineOffsetAim.Vector = vectorRotationY;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			/*pointEyeLook.Y += step;
			pointAimLook.Y += step;*/
		}
		if (mouse_y0 - point.y > 0)
		{
			lineOffsetEye.Vector = vectorRotationY * (-1);
			lineOffsetAim.Vector = vectorRotationY * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;

			/*pointEyeLook.Y -= step;
			pointAimLook.Y -= step;*/
		}

		Invalidate(FALSE);
	}

	mouse_x0 = point.x;
	mouse_y0 = point.y;


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

	CView::OnLButtonDown(nFlags, point);
}
// ---																							// On Right Button Down
void COpenGLView::OnRButtonDown(UINT nFlags, CPoint point)
{

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


	CView::OnRButtonDown(nFlags, point);
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

////////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cube (Test)
//void COpenGLView::DrawOpenGL_Cube(double param, double cx, double cy, double cz, bool flagColor)
//{
//	/*glPushMatrix();
//
//	glTranslated(0, 0, 0);*/
//
//	param /= 2;
//
//	if (flagColor)
//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
//	else
//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);
//
//	//glDisable(GL_TEXTURE_2D);
//	glBegin(GL_QUAD_STRIP);
//
//	glNormal3f(0, 0, 1);
//
//	glVertex3f(-param + cx, param + cy, param + cz);
//
//	glVertex3f(-param + cx, -param + cy, param + cz);
//
//	glVertex3f(param + cx, param + cy, param + cz);
//
//	glVertex3f(param + cx, -param + cy, param + cz);
//
//	glVertex3f(param + cx, param + cy, -param + cz);
//	glNormal3f(1, 0, 0);
//	glVertex3f(param + cx, -param + cy, -param + cz);
//
//	glVertex3f(-param + cx, param + cy, -param + cz);
//	glNormal3f(0, 0, 1);
//	glVertex3f(-param + cx, -param + cy, -param + cz);
//
//	glVertex3f(-param + cx, param + cy, param + cz);
//	glNormal3f(1, 0, 0);
//	glVertex3f(-param + cx, -param + cy, param + cz);
//
//	glEnd();
//
//	//glPopMatrix();
//
//	return;
//
//	glBegin(GL_QUADS);
//
//	glVertex3f(-param + cx, param + cy, param + cz);
//
//	glVertex3f(param + cx, param + cy, param + cz);
//
//	glVertex3f(param + cx, param + cy, -param + cz);
//
//	glVertex3f(-param + cx, param + cy, -param + cz);
//	glEnd();
//
//	glBegin(GL_QUADS);
//	glVertex3f(-param + cx, -param + cy, param + cz);
//
//	glVertex3f(param + cx, -param + cy, param + cz);
//
//	glVertex3f(param + cx, -param + cy, -param + cz);
//
//	glVertex3f(-param + cx, -param + cy, -param + cz);
//	glEnd();
//
//}
/////////////////////////////////////////////////////////

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
	glPointSize(5);

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



	switch (nChar) {
	case  VK_RIGHT :
		/*angleLook -= 0.01f;
		xPointLook = sin(angleLook);
		zPointLook = -cos(angleLook);*/


		lineOffset = LineGeometric(pointEyeLook, vectorRotationX * (-1));


		pointEyeLook = lineOffset.CreatePointOnDistance(10);

		vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		vectorRotationX = vectorRotationZ ^ vectorRotationY;

		if (vectorRotationX* vectorRotationX_Old < 0)
		{
			vectorRotationX = vectorRotationX * (-1);
		}

		// ---																						//distance between camera and Aim correction
		lineOffset = LineGeometric(pointAimLook, vectorRotationZ * (-1));

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceAimEye);



		wAngleY += 10;
		/*vectorRotationX.X = cos(wAngleY * PI_Approx / 180.0f);
		vectorRotationX.Z = sin(wAngleY * PI_Approx / 180.0f);*/


		break;
	case VK_LEFT:
		/*angleLook += 0.01f;
		xPointLook = sin(angleLook);
		zPointLook = -cos(angleLook);*/

		wAngleY -= 10;
		/*vectorRotationX.X = cos(wAngleY * PI_Approx / 180.0f);
		vectorRotationX.Z = sin(wAngleY * PI_Approx / 180.0f);*/

		break;
	case VK_UP:
		/*xEyeLook += xPointLook * fraction;
		zEyeLook += zPointLook * fraction;*/

		wAngleX += 10;
		/*vectorRotationY.Y = cos(wAngleX * PI_Approx / 180.0f);
		vectorRotationY.Z = sin(wAngleX * PI_Approx / 180.0f);*/

		break;
	case VK_DOWN:
		/*xEyeLook -= xPointLook * fraction;
		zEyeLook -= zPointLook * fraction;*/

		lineOffset = LineGeometric(pointEyeLook, vectorRotationY);


		pointEyeLook = lineOffset.CreatePointOnDistance(10);

		vectorRotationZ = VectorGeometric(pointEyeLook, pointAimLook);						// Vector Z

		vectorRotationY = vectorRotationZ ^ vectorRotationX;

		if (vectorRotationY * vectorRotationY_Old < 0)
		{
			vectorRotationY = vectorRotationY * (-1);
		}

		lineOffset = LineGeometric(pointAimLook, vectorRotationZ * (-1));

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceAimEye);





		wAngleX -= 10;
	/*	vectorRotationY.Y = cos(wAngleX * PI_Approx / 180.0f);
		vectorRotationY.Z = sin(wAngleX * PI_Approx / 180.0f);*/

		break;
	}

	
	/*pointEyeLook.Z = pointAimLook.Z + radiusOfLook * sin(wAngleX * PI_Approx / 180.0f) * cos(wAngleY * PI_Approx / 180.0f);
	pointEyeLook.Y = pointAimLook.Y + radiusOfLook * sin(wAngleX * PI_Approx / 180.0f) * sin(wAngleY * PI_Approx / 180.0f);	
	pointEyeLook.X = pointAimLook.X + radiusOfLook * cos(wAngleX * PI_Approx / 180.0f);*/

	Invalidate(FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}





PointGeometric COpenGLView::GetOGLPos(int x, int y)       //Получение координат по клику возвращает вектор
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	VectorGeometric posnear;
	VectorGeometric posfar;
	
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	for (int i = 0; i < 16; i++)
	{
		if (modelview[i] < 0.00001 && modelview[i]>0)modelview[i] = 0;
		if (projection[i] < 0.00001 && projection[i]>0)projection[i] = 0;
	}

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	//gluUnProject(winX, winY, (-1)*winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	// точка на ближайшей отсекающей плоскости
	gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &posnear.X, &posnear.Y, &posnear.Z);
	// точка на дальней отсекающей плоскости
	gluUnProject(winX, winY, -1.0, modelview, projection, viewport, &posfar.X, &posfar.Y, &posfar.Z);
	VectorGeometric mouseRay = posfar - posnear; // вектор под мышой
	VectorGeometric n = VectorGeometric(pointEyeLook, pointAimLook); //VectorGeometric(0.0, 0.0, 1.0);//нормаль к плоскости x=0;y=0;z=1;
	float t = (n * (-mouseRay)) / (n * mouseRay);
	VectorGeometric pos = posnear + mouseRay * t; // собственно точка пересечения

	return PointGeometric(pos.X, pos.Y, pos.Z);;
	//return PointGeometric(posX, posY, posZ);
}

