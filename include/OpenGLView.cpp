// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//


#include "stdafx.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/GLUT.h>
#include <gl/Glaux.h>
#include "OpenGLView.h"
#include "gl/GL.h"


GLdouble		m_dModelViewMatrix[16];
GLdouble		m_dProjectionMatrix[16];
GLint			m_iViewport[4];

GLint		viewport_test[4];
GLdouble	modelview_test[16];
GLdouble	projection_test[16];

bool flagCubeColor_1 = false;

const GLfloat blackColor[] = { 0.0f,	0.0f,	0.0f,	1.0f };

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
	
	wAngleY = 0;		//10.0f;
	wAngleX = 0;		//1.0f;
	wAngleZ = 0;		//5.0f;

	wTransformX = 0;
	wTransformY = 0;

	fNearPlane	= 1.f;
	fFarPlane	= 10000.0f;

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
	toleranceObjectsArray = nullptr; // Voronov

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
	myFont = new MyFont("arial.ttf", 16, 16);
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

	GLfloat light_diffuse[]			= { 1.f,		1.f,		1.f },
			light_diffuseMiddle[]	= { 0.2f,	0.2f,	0.2f },
			light_diffuseDark[]		= { 0.0f,	0.0f,	0.0f };


	wglMakeCurrent(pDC->m_hDC, hRC);
	//**Draw GL here!
	//glClearColor(0.0,0.0,0.0,0.0);

	glClearDepth(1.0f);                                  //Specifies the clear value for the depth buffer 
	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);             //Set Background Color
	glColor3fv(redColor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	pointEyeLook.X,		pointEyeLook.Y,		pointEyeLook.Z,
				pointAimLook.X,		pointAimLook.Y,		pointAimLook.Z,
				0.0f,		1.0f,		0.0f);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);


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

	glGetDoublev(GL_MODELVIEW_MATRIX, m_dModelViewMatrix);

	//DrawOpenGL_Cube(20, 0, 0, 0, false);

	PaintScene(GL_RENDER);


	//DrawOpenGL_Cube(10, 0, 0, 0, true);            // Draws a sphere with translation <100, 0, 0>


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
	gluPerspective(30.0, gldAspect, fNearPlane, fFarPlane);
	//**End code for GL resize!

	glGetDoublev(GL_PROJECTION_MATRIX, m_dProjectionMatrix);
	glGetIntegerv(GL_VIEWPORT, m_iViewport);


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
	
	if (flagToleranceMove) {
		PointGeometric mouseNewInWorld;
		PointGeometric mouseOldInWorld;

		GetWorldCoord(point.x, point.y, 0, mouseNewInWorld);
		
		GetWorldCoord(mouse_x0, mouse_y0, 0, mouseOldInWorld);
		selectedToleranceObject->PointPosition = mouseNewInWorld;
	}


	if (nFlags& MK_LBUTTON && flagShiftPressed)
	{
		if (point.x - prev_x != 0)
		{
			flagRotateAxisY = true;
			//coordinateDifferenceX = point.x - prev_x;
			roty += (float)((point.x - prev_x) * 0.5);
		}
		else
			flagRotateAxisY = false;
		//roty += (point.x - prev_x) * 0.5;
		if (point.y - prev_y != 0)
		{
			flagRotateAxisX = true;
			//coordinateDifferenceY = point.y - prev_y;
			rotx += (float)((point.y - prev_y) * 0.5);
		}
		else
			flagRotateAxisX = false;
		//rotx += (point.y - prev_y) * 0.5;

		Invalidate(FALSE);		//The end

	}

	if (flagMiddleButtonDown)
	{

		PointGeometric mouseNewInWorld;
		PointGeometric mouseOldInWorld;

		GetWorldCoord(point.x, point.y, 0, mouseNewInWorld);
		GetWorldCoord(mouse_x0, mouse_y0, 0, mouseOldInWorld);
	
		double	stepOffsetX,
				stepOffsetY;

		PointGeometric	point0_0(0, 0, 0),
						pointX_0(glnWidth, 0, 0),
						point0_Y(0, glnHeight, 0);

		GetWorldCoord(point0_0.X, point0_0.Y, 0, point0_0);
		GetWorldCoord(pointX_0.X, pointX_0.Y, 0, pointX_0);
		GetWorldCoord(point0_Y.X, point0_Y.Y, 0, point0_Y);

		VectorGeometric vectorX(point0_0, pointX_0),
						vectorY(point0_0, point0_Y);
			

		LineGeometric		lineOffsetEye,
							lineOffsetAim;

		LineGeometric lineTmp(point0_0, vectorX);
		stepOffsetX = lineTmp.PointProjection(mouseNewInWorld).DistanceToPoint(lineTmp.PointProjection(mouseOldInWorld));

		lineTmp.Vector	= vectorY;
		stepOffsetY		= lineTmp.PointProjection(mouseNewInWorld).DistanceToPoint(lineTmp.PointProjection(mouseOldInWorld));
		

		lineOffsetEye.Point = pointEyeLook;
		lineOffsetAim.Point = pointAimLook;
		

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

	if (flagToleranceMove) {
		flagToleranceMove = false;
		return;
	}
	
	
	ObjectApprox *objApprox = GetObjectUnderMouse(point);
	

	int a = 0;
	if (objApprox != nullptr)
	{
		objApprox->flagSelected = !objApprox->flagSelected;
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
	

	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

// ---																							// Get World Coordinate
void COpenGLView::GetWorldCoord(int ix, int iy, GLdouble fz, PointGeometric& coord)
{
	GLdouble x, y, z, winX, winY, winZ;

	winX = (GLdouble)ix;
	winY = (GLdouble)m_iViewport[3] - iy;

	fz += pointEyeLook.DistanceToPoint(pointAimLook);	//GetFocalLength()

	// Calculate the winZ coordinate:
	// Compensate for perspective view
	winZ = 0.5 + (((fFarPlane + fNearPlane) - (2 * fFarPlane * fNearPlane) / fz)) / (2 * (fFarPlane - fNearPlane));

	// Unproject the point
	gluUnProject(winX, winY, winZ,
		m_dModelViewMatrix,
		m_dProjectionMatrix,
		m_iViewport,
		&x, &y, &z);

	coord.X = x;
	coord.Y = y;
	coord.Z = z;
}
// ---																							// Get Screen Coordinate
void COpenGLView::GetScreenCoord(GLdouble wX, GLdouble wY, GLdouble wZ, PointGeometric& coord)
{
	gluProject(	wX, wY, wZ,
				m_dModelViewMatrix, m_dProjectionMatrix, m_iViewport,
				&coord.X, &coord.Y, &coord.Z);
	coord.Y = (GLdouble)m_iViewport[3] - coord.Y;
	coord.Z += pointEyeLook.DistanceToPoint(pointAimLook);	//GetFocalLength()
}

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Get Object Under Mouse
ObjectApprox *COpenGLView::GetObjectUnderMouse(CPoint point)
{
	mouse_x0 = point.x;  mouse_y0 = point.y;

	prev_x = point.x;
	prev_y = point.y;

	#define BUFSIZE 512

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
	gluPerspective(30.0, gldAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
	PaintScene(GL_SELECT);
	glPopMatrix();
	glFlush();

	hits = glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, glnWidth, glnHeight);
	gluPerspective(30.0, gldAspect, fNearPlane, fFarPlane);

	/*for(int i=0;i<hits;i++)
	{
		cube[selectBuf[3+i*4]].selected=!cube[selectBuf[3+i*4]].selected;
	}*/

	ObjectApprox* objApprox = nullptr;
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
					return objApprox;
				}
			}
		}
	}
	else
	{
		return nullptr;
	}

	wglMakeCurrent(NULL, NULL);
	::ReleaseDC(this->m_hWnd, hDC);

	return nullptr;
}
//////////////////////////////////////////////////////////

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

	// Voronov
	if (toleranceObjectsArray != nullptr) {

		ToleranceObject* toleranceObject;

		for (int i = 0; i < toleranceObjectsArray->size(); i++) {
			toleranceObject = toleranceObjectsArray->operator[](i);

			if (!toleranceObject->isVisible) {
				continue;
			}


			if (dynamic_cast<SizeLine*>(toleranceObject)) {
				DrawOpenGL_SizeLine((SizeLine*)toleranceObject);
			}
			else if (dynamic_cast<DiameterLine*> (toleranceObject)) {
				DrawOpenGL_DiameterLine((DiameterLine*)toleranceObject);
			}
			else if (dynamic_cast<AxialLine*> (toleranceObject)) {
				DrawOpenGL_AxialLine((AxialLine*)toleranceObject);
			}
			else if (dynamic_cast<FormRoundnessToleranceObject*>(toleranceObject)) {
				DrawOpenGL_Tolerance_Form_Roudness((FormRoundnessToleranceObject*)toleranceObject);
			}else if (dynamic_cast<ToleranceFrame*>(toleranceObject)) {
				DrawOpenGL_ToleranceFrame((ToleranceFrame*)toleranceObject);
			}

			Invalidate(false);
		}
	}



	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;


		
		//	---	---	---	---	---										// Set color

		if (objApprox->objMath->GetName() == cylinderA->GetName()	||
			objApprox->objMath->GetName() == coneA->GetName()		||
			objApprox->objMath->GetName() == sphereA->GetName()		||
			objApprox->objMath->GetName() == rectangleA->GetName())
		{
			if (objApprox->flagSelected)
				glColor3fv(redColor);//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redColor);
			else
				glColor3fv(blueColor);//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueColor);
		}
		else
		{
			if (objApprox->flagSelected)
				glColor3fv(simpleElementsColor1);//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, simpleElementsColor1);
			else
				glColor3fv(simpleElementsColor2);//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, simpleElementsColor2);
		}



		/*if (objApprox->flagSelected)
			glColor3f(0.2f, 0.85f, 0.1f);
		else
			glColor3f(0.8f, 0.35f, 0.8f);*/

		//	---	---	---	---	---										// Set name
		if (mode == GL_SELECT) glLoadName(objApprox->objID);

		//	---	---	---	---	---												// Draw via Triangles
		if (objApprox->objMath->GetName() == cylinderA->GetName()	||
			objApprox->objMath->GetName() == coneA->GetName()		||
			objApprox->objMath->GetName() == sphereA->GetName())
		{
			DrawOpenGL_ObjViaTriangles(*objApprox->objMath);
			
			if (objApprox->objMath->GetName() == cylinderA->GetName())
			{
				glColor3fv(blackColor);

				glLineWidth(4);
				DrawOpenGL_ByLineLoop(	((CylinderApprox*)(objApprox->objMath))->pointsTopCircleEdge, 
										((CylinderApprox*)(objApprox->objMath))->Line.Vector);
			
				DrawOpenGL_ByLineLoop(	((CylinderApprox*)(objApprox->objMath))->pointsBottomCircleEdge,
										((CylinderApprox*)(objApprox->objMath))->Line.Vector);
				glLineWidth(1);
			}
			if (objApprox->objMath->GetName() == coneA->GetName())
			{
				glColor3fv(blackColor);

				glLineWidth(4);
				DrawOpenGL_ByLineLoop(	((ConeApprox*)(objApprox->objMath))->pointsTopCircleEdge,
										((ConeApprox*)(objApprox->objMath))->Line.Vector);

				DrawOpenGL_ByLineLoop(	((ConeApprox*)(objApprox->objMath))->pointsBottomCircleEdge,
										((ConeApprox*)(objApprox->objMath))->Line.Vector);
				glLineWidth(1);
			}
		}
		//	---	---	---	---	---												// Draw Circle
		if (objApprox->objMath->GetName() == circleA->GetName())
		{
			DrawOpenGL_Circle(*objApprox->objMath);
		}
		//	---	---	---	---	---												// Draw Point
		if (objApprox->objMath->GetName() == pointA->GetName())
		{
			DrawOpenGL_Point((PointApprox*)objApprox->objMath);
		}
		//	---	---	---	---	---												// Draw Line Segment
		if (objApprox->objMath->GetName() == lineSegmentA->GetName())
		{
			DrawOpenGL_LineSegment((LineSegmentApprox*)objApprox->objMath);
		}
		//	---	---	---	---	---												// Draw PlaneViaRectangle
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

// Voronov

void COpenGLView::drawBitmapText(char* string, double x, double y, double z)
{
	char* c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{
		//glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
}


//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL SizeLine
void COpenGLView::DrawOpenGL_ToleranceFrame(ToleranceFrame* frame)
{
	
	PointGeometric pStart = frame->PointStart;
	PointGeometric pEnd = frame->PointPosition;

	glPointSize(5);
	glColor3d(0, 100, 0);

	glBegin(GL_LINES);
		glVertex3d(pStart.X, pStart.Y, pStart.Z);
		glVertex3d(pEnd.X, pEnd.Y, pEnd.Z);
	glEnd();

	// ���� ����� ��������� ������ �� ����� ������, �� ������ ����������� �����
	if (pEnd.X > pStart.X) {
		//frame->changeBoxDirection();
	}


	PointGeometric centerBage = frame->getCenterBage();
	PointGeometric centerToleranceValue = frame->getCenterToleranceValue();
	PointGeometric centerToleranceBase = frame->getCenterToleranceBaseName();

	// ������
	glBegin(GL_LINE_LOOP);
		glVertex3d(pEnd.X, pEnd.Y, pEnd.Z);
		glVertex3d(pEnd.X - frame->Width() , pEnd.Y, pEnd.Z); // ������ �����
		glVertex3d(pEnd.X - frame->Width(), pEnd.Y + frame->boxHeight, pEnd.Z);

		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth, pEnd.Y + frame->boxHeight, pEnd.Z); // ������ ����� ������ ������
		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth, pEnd.Y, pEnd.Z);
		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth, pEnd.Y + frame->boxHeight, pEnd.Z); // ����� ����� ������ ������ ������ ������ � ������ ������ ������ ������

		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth + frame->boxSecondSectionWidth, pEnd.Y + frame->boxHeight, pEnd.Z); // ������ ����� ������ ������
		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth + frame->boxSecondSectionWidth, pEnd.Y, pEnd.Z);
		glVertex3d(pEnd.X - frame->Width() + frame->boxFirstSectionWidth + frame->boxSecondSectionWidth, pEnd.Y + frame->boxHeight, pEnd.Z);
		glVertex3d(pEnd.X, pEnd.Y + frame->boxHeight, pEnd.Z);
	glEnd();

	glBegin(GL_POINTS);
		glVertex3d(centerBage.X, centerBage.Y, centerBage.Z);
		glVertex3d(centerToleranceValue.X, centerToleranceValue.Y, centerToleranceValue.Z);
		glVertex3d(centerToleranceBase.X, centerToleranceBase.Y, centerToleranceBase.Z);
	glEnd();

}

void COpenGLView::DrawOpenGL_SizeLine(SizeLine* obj)
{
	
	PointGeometric pStart = ((CylinderApprox*)(obj->objMath))->PointTopSurfaceCenter;
	PointGeometric pEnd = obj->PointPosition;

	glPointSize(5);
	glColor3d(0, 255, 0);

	if (flagToleranceMove && selectedToleranceObject == obj) {
		//obj->PointPosition.Y = obj->PointPosition.Y;
		TRACE("X = %g; Y = %g; Z = %g\n", obj->PointPosition.X, obj->PointPosition.Y, obj->PointPosition.Z);
		TRACE("X = %g; Y = %g; Z = %g\n", pStart.X, pStart.Y, pStart.Z);
		TRACE("------------------------\n");
	}


	
	

	glBegin(GL_LINES);
		glVertex3d(pStart.X, pStart.Y, pStart.Z);
		glVertex3d(obj->PointPosition.X, obj->PointPosition.Y, obj->PointPosition.Z);
	glEnd();


	
	double boxHeight = 3;

	double boxFirstSectionWidth = 3;
	double boxSecondSectionWidth = 6;
	double boxThirdSectionWidth = 3;
	
	if (obj->PointPosition.X > pStart.X) {
		boxFirstSectionWidth = -boxFirstSectionWidth;
		boxSecondSectionWidth = -boxSecondSectionWidth;
		boxThirdSectionWidth = -boxThirdSectionWidth;
	}

	double boxWidth = boxFirstSectionWidth + boxSecondSectionWidth + boxThirdSectionWidth;



	PointGeometric centerBage = PointGeometric(obj->PointPosition.X - boxWidth + boxFirstSectionWidth / 2, obj->PointPosition.Y + boxHeight / 2, obj->PointPosition.Z); // ����� �������� ��� ������ �������
	PointGeometric centerToleranceValue = PointGeometric(obj->PointPosition.X - boxWidth + boxFirstSectionWidth  + boxSecondSectionWidth / 2, obj->PointPosition.Y + boxHeight / 2, obj->PointPosition.Z); // ����� �������� ��� �������� �������
	PointGeometric centerToleranceBase = PointGeometric(obj->PointPosition.X - boxThirdSectionWidth / 2, obj->PointPosition.Y + boxHeight / 2, obj->PointPosition.Z); // ����� �������� ��� �������� ���� �������

	// ������
	glBegin(GL_LINE_LOOP);
		glVertex3d(obj->PointPosition.X, obj->PointPosition.Y, obj->PointPosition.Z);
		glVertex3d(obj->PointPosition.X-boxWidth, obj->PointPosition.Y, obj->PointPosition.Z); // ������ �����
		glVertex3d(obj->PointPosition.X-boxWidth, obj->PointPosition.Y+ boxHeight, obj->PointPosition.Z);

		glVertex3d(obj->PointPosition.X-boxWidth+boxFirstSectionWidth, obj->PointPosition.Y+ boxHeight, obj->PointPosition.Z); // ������ ����� ������ ������
		glVertex3d(obj->PointPosition.X-boxWidth+boxFirstSectionWidth, obj->PointPosition.Y, obj->PointPosition.Z);
		glVertex3d(obj->PointPosition.X - boxWidth + boxFirstSectionWidth, obj->PointPosition.Y + boxHeight, obj->PointPosition.Z); // ����� ����� ������ ������ ������ ������ � ������ ������ ������ ������

		glVertex3d(obj->PointPosition.X - boxWidth + boxFirstSectionWidth + boxSecondSectionWidth, obj->PointPosition.Y + boxHeight, obj->PointPosition.Z); // ������ ����� ������ ������
		glVertex3d(obj->PointPosition.X - boxWidth + boxFirstSectionWidth + boxSecondSectionWidth, obj->PointPosition.Y, obj->PointPosition.Z);
		glVertex3d(obj->PointPosition.X - boxWidth + boxFirstSectionWidth + boxSecondSectionWidth, obj->PointPosition.Y + boxHeight, obj->PointPosition.Z);
		glVertex3d(obj->PointPosition.X, obj->PointPosition.Y + boxHeight, obj->PointPosition.Z);
	glEnd();

	glBegin(GL_POINTS);
		glVertex3d(centerBage.X, centerBage.Y, centerBage.Z);
		glVertex3d(centerToleranceValue.X, centerToleranceValue.Y, centerToleranceValue.Z);
		glVertex3d(centerToleranceBase.X, centerToleranceBase.Y, centerToleranceBase.Z);
	glEnd();

	
	//
	/*double x1 = obj->PointStart.X;
	double y1 = obj->PointStart.Y;
	double z1 = obj->PointStart.Z;

	double x2 = obj->PointEnd.X;
	double y2 = obj->PointEnd.Y;
	double z2 = obj->PointEnd.Z;
	double length = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	//glPushMatrix();
	//glTranslated(x1, y1, z1);
	glLineWidth(2);
	glColor3d(0, 255, 0);

	double offset = 15 + obj->offset; // ���������� �� ������� �� ��������� �����. obj->offset - ����������� ������� �������.
	double dx = 0.3; // ������ "����"

	double triangle_width = 0.3;
	double triangle_height = 0.3;

	if(flagToleranceMove && selectedToleranceObject == obj){
		obj->PointPosition.Y = obj->PointPosition.Y ;
		
		
		double tmpY = ((y1 + obj->PointPosition.Y * (x1 - x2) / length));
		
		if ((tmpY < 0 && obj->PointPosition.Y > 0) || (tmpY > 0 && obj->PointPosition.Y < 0)) {
			obj->PointPosition.Y = obj->PointPosition.Y * -1;
		}
		TRACE("y = %g; tmpY = %g\n", obj->PointPosition.Y, tmpY);
	}

	

	double x1n = x1 + obj->offset * (y2 - y1) / length;
	double y1n = y1 + obj->offset * (x1 - x2) / length;
	double x2n = x2 + obj->offset * (y2 - y1) / length;
	double y2n = y2 + obj->offset * (x1 - x2) / length;

	// ���������� ��������� �����
	//double x1p = x1 + offset * (y2 - y1) / length;
	//double y1p = y1 + offset * (x1 - x2) / length;
	double x1p = x1 + obj->PointPosition.Y * (y2 - y1) / length;
	double y1p = y1 + obj->PointPosition.Y * (x1 - x2) / length;
	PointGeometric p1 = PointGeometric(x1p, y1p, z1);
	//p1.Y = p1.Y + obj->PointStop.Y;

	//double x2p = x2 + offset * (y2 - y1) / length;
	//double y2p = y2 + offset * (x1 - x2) / length;
	double x2p = x2 + obj->PointPosition.Y * (y2 - y1) / length;
	double y2p = y2 + obj->PointPosition.Y * (x1 - x2) / length;
	PointGeometric p2 = PointGeometric(x2p, y2p, z2);
	//p2 = p2 + obj->PointStop;
	//p2.Y = p2.Y + obj->PointStop.Y;

	VectorGeometric perpSize = VectorGeometric(PointGeometric(x1n, y1n, z1), p1, true);
	//drawBitmapText("TEST", x1p, y1p, z1);
	//---------
		
		VectorGeometric vec_norm = VectorGeometric(p2, p1, true);
		PointGeometric vec_h1_top = p1 +(perpSize* triangle_width) - (vec_norm * triangle_height);
		PointGeometric vec_h1_bottom = p1 - (perpSize * triangle_width) - (vec_norm * triangle_height);
	
		// �����������
		glBegin(GL_TRIANGLES);
			glVertex3d(p1.X, p1.Y, p1.Z);
			glVertex3d(vec_h1_top.X, vec_h1_top.Y, p1.Z);
			glVertex3d(vec_h1_bottom.X, vec_h1_bottom.Y, p1.Z);
		glEnd();
		
		PointGeometric vec_h2_top = p2 + (perpSize * triangle_width) + (vec_norm * triangle_height);
		PointGeometric vec_h2_bottom = p2 - (perpSize * triangle_width) + (vec_norm * triangle_height);

		glBegin(GL_TRIANGLES);
			glVertex3d(p2.X, p2.Y, p2.Z);
			glVertex3d(vec_h2_top.X, vec_h2_top.Y, p2.Z);
			glVertex3d(vec_h2_bottom.X, vec_h2_bottom.Y, p2.Z);
		glEnd();
	// ��������� �����
	glBegin(GL_LINES);
		glVertex3d(p1.X, p1.Y, p1.Z);
		glVertex3d(p2.X, p2.Y, p2.Z);
	glEnd();


	// ���������� "����"
	double x1o = x1 + (offset + dx) * (y2 - y1) / length;
	double y1o = y1 + (offset + dx) * (x1 - x2) / length;

	double x2o = x2 + (offset + dx) * (y2 - y1) / length;
	double y2o = y2 + (offset + dx) * (x1 - x2) / length;

	glDisable(GL_DEPTH_TEST);
	//glColor3f(0.0f, 0.0f, 0.0f);
	//myFont->Print(x1o, y1o, z1, L"Test");
	//myFont->Print(-140, 80, L"Test");
	glEnable(GL_DEPTH_TEST);
	//-----
	// ����� ����� �� ��������� ����� �� ��������� ����� + ������
	glBegin(GL_LINES);
		glVertex3d(x1n, y1n, z1);
		//glVertex3d(x1o, y1o, z1);
		glVertex3d(p1.X, p1.Y, p1.Z);
	glEnd();

	// ������ ����� �� ��������� ����� �� ��������� ����� + ������
	glBegin(GL_LINES);
		glVertex3d(x2n, y2n, z2);
		//glVertex3d(x2o, y2o, z2);
		glVertex3d(p2.X, p2.Y, p2.Z);
	glEnd();

	glDisable(GL_LINE_STIPPLE);


	glLineWidth(1);*/
	//glPopMatrix();
}




double COpenGLView::angle_point(PointGeometric a, PointGeometric b, PointGeometric c)
{
	double x1 = a.X - b.X, x2 = c.X - b.X;
	double y1 = a.Y - b.Y, y2 = c.Y - b.Y;
	double z1 = a.Z - b.Z, z2 = c.Z - b.Z;
	double d1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
	double d2 = sqrt(x2 * x2 + y2 * y2 + z2 * z2);

	return acos((x1 * x2 + y1 * y2 + z1 * z1) / (d1 * d2));
}


void COpenGLView::DrawOpenGL_DiameterLine(DiameterLine* obj)
{
	double triangle_height = 0.3;
	double triangle_widht = 0.3;

	VectorGeometric n = obj->objMath->Line.Vector;
	PointGeometric centerPoint = obj->centerPoint - (n * 0.01);
	VectorGeometric p = VectorGeometric(obj->objMath->PointsForApprox.operator[](0), centerPoint);
	VectorGeometric perp = n ^ p;
	perp.Normalize();
	n.Normalize();

	PointGeometric leftPoint = centerPoint + perp * (obj->diameter / 2);
	PointGeometric rightPoint = centerPoint - perp * (obj->diameter / 2);
	glLineWidth(2);
	glColor3d(0, 255, 0);


	glPointSize(8);
	glBegin(GL_POINTS);
		glVertex3d(centerPoint.X, centerPoint.Y, centerPoint.Z);
	glEnd();

	
	
	glBegin(GL_LINES);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
	glEnd();

	VectorGeometric g = VectorGeometric(obj->objMath->PointsForApprox.operator[](1), centerPoint);
	VectorGeometric perpTrinagle = n ^ perp;
	perpTrinagle.Normalize();

	PointGeometric leftTopPoint = leftPoint - perp * triangle_height + perpTrinagle * triangle_widht;
	PointGeometric leftBottomPoint = leftPoint - perp * triangle_height - perpTrinagle * triangle_widht;
	// ������ �����������
	glBegin(GL_TRIANGLES);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
		glVertex3d(leftTopPoint.X, leftTopPoint.Y, leftTopPoint.Z);
		glVertex3d(leftBottomPoint.X, leftBottomPoint.Y, leftBottomPoint.Z);
	glEnd();

	PointGeometric rightTopPoint = rightPoint + perp * triangle_height + perpTrinagle * triangle_widht;
	PointGeometric rightBottomPoint = rightPoint + perp * triangle_height - perpTrinagle * triangle_widht;
	// ������ �����������
	glBegin(GL_TRIANGLES);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
		glVertex3d(rightTopPoint.X, rightTopPoint.Y, rightTopPoint.Z);
		glVertex3d(rightBottomPoint.X, rightBottomPoint.Y, rightBottomPoint.Z);
	glEnd();





	/*
	glPushMatrix();       // ��������� ������� ����������
	//double angle = angle_point(PointGeometric(0, 0, 0), obj->centerPoint, obj->objMath->PointsForApprox.operator[](0));
	glTranslated(obj->centerPoint.X, obj->centerPoint.Y, obj->centerPoint.Z);  // ���������� �� ��� Z
	//VectorGeometric ve = VectorGeometric(obj->centerPoint, PointGeometric(0, 0, 0), true);
	//glRotated(angle, 0, 1, 0);
	
	glLineWidth(2);
	glColor3d(0, 255, 0);

	double triangle_height = 0.3;
	double triangle_widht = 0.3;
	
	
	glPointSize(8);
	glBegin(GL_POINTS);
		//glVertex3d(obj->centerPoint.X, obj->centerPoint.Y, obj->centerPoint.Z);
		glVertex3d(0, 0, 0);
		//glVertex3d(obj->diameter, 0, 0);
		//glVertex3d(0, obj->diameter, 0);
		
	glEnd();

	if(!obj->isOutdoor){
		// ������������� �����
		glBegin(GL_LINES);
			glVertex3d(obj->diameter / 2, 0, 0);
			glVertex3d(-obj->diameter / 2, 0, 0);
		glEnd();

		// ������ �����������
		glBegin(GL_TRIANGLES);
			glVertex3d(obj->diameter / 2, 0, 0);
			glVertex3d(obj->diameter / 2 - triangle_height, triangle_widht, 0);
			glVertex3d(obj->diameter / 2 - triangle_height, - triangle_widht, 0);
		glEnd();

		// ����� �����������
		glBegin(GL_TRIANGLES);
			glVertex3d(0 - obj->diameter / 2, 0, 0);
			glVertex3d(0 - obj->diameter / 2 + triangle_height, 0 + triangle_widht, 0);
			glVertex3d(0- obj->diameter / 2 + triangle_height, 0- triangle_widht, 0);
		glEnd();


	}
	else {
		// ������������� �����
		glBegin(GL_LINES);
			glVertex3d(obj->centerPoint.X + obj->diameter, obj->centerPoint.Y, obj->centerPoint.Z);
			glVertex3d(obj->centerPoint.X - obj->diameter, obj->centerPoint.Y, obj->centerPoint.Z);
		glEnd();

		// ������ �����������
		glBegin(GL_TRIANGLES);
		glVertex3d(obj->centerPoint.X + obj->diameter / 2, obj->centerPoint.Y, obj->centerPoint.Z);
		glVertex3d(obj->centerPoint.X + obj->diameter / 2 + triangle_height, obj->centerPoint.Y + triangle_widht, obj->centerPoint.Z);
		glVertex3d(obj->centerPoint.X + obj->diameter / 2 + triangle_height, obj->centerPoint.Y - triangle_widht, obj->centerPoint.Z);
		glEnd();

		// ����� �����������
		glBegin(GL_TRIANGLES);
		glVertex3d(obj->centerPoint.X - obj->diameter / 2, obj->centerPoint.Y, obj->centerPoint.Z);
		glVertex3d(obj->centerPoint.X - obj->diameter / 2 - triangle_height, obj->centerPoint.Y + triangle_widht, obj->centerPoint.Z);
		glVertex3d(obj->centerPoint.X - obj->diameter / 2 - triangle_height, obj->centerPoint.Y - triangle_widht, obj->centerPoint.Z);
		glEnd();
	}
	
	glPopMatrix();  // ������������ � ������ ������� ���������
	*/
}

void COpenGLView::DrawOpenGL_AxialLine(AxialLine * obj)
{
	glLineWidth(2);
	glColor3d(0, 255, 0);

	VectorGeometric normalizedVector = VectorGeometric(obj->dirVector);
	normalizedVector.Normalize();

	VectorGeometric start = obj->dirVector - (normalizedVector * 5) - (normalizedVector * obj->offset);
	VectorGeometric end = obj->dirVector + (normalizedVector * 3) + (normalizedVector * obj->offset);

	//start = start + obj->centerPoint;
	//end = end + obj->centerPoint;

	start.X += obj->centerPoint.X;
	start.Y += obj->centerPoint.Y;
	start.Z += obj->centerPoint.Z;

	end.X += obj->centerPoint.X;
	end.Y += obj->centerPoint.Y;
	end.Z += obj->centerPoint.Z;

	glEnable(GL_LINE_STIPPLE); // ��������� ��������
							// ����������� �����
	glLineStipple(2, 58364); // ������� 
	glBegin(GL_LINES);
	
	glVertex3d(start.X, start.Y, start.Z);
	glVertex3d(end.X, end.Y, end.Z);
	glEnd();

	glDisable(GL_LINE_STIPPLE);

}

void COpenGLView::DrawOpenGL_Tolerance_Form_Roudness(FormRoundnessToleranceObject * obj)
{
	CircleApprox* circle = (CircleApprox*)obj->objMath;
	glLineWidth(2);
	glColor3d(255, 0, 255);
	glBegin(GL_LINE_LOOP);
	
	for (int i = 0; i < circle->PointsForApprox.size(); i++) {
		PointGeometric tmp = circle->PointsForApprox.operator[](i);
		glVertex3d(tmp.X, tmp.Y, tmp.Z);
	}
	glEnd();


}
/////////////////////////////////////////////


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

	glVertex3f((float)(-param + cx), (float)(param + cy), (float)(param + cz));

	glVertex3f((float)(-param + cx), (float)(-param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(-param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(param + cy), (float)(-param + cz));
	glNormal3f(1, 0, 0);
	glVertex3f((float)(param + cx), (float)(-param + cy), (float)(-param + cz));

	glVertex3f((float)(-param + cx), (float)(param + cy), (float)(-param + cz));
	glNormal3f(0, 0, 1);
	glVertex3f((float)(-param + cx), (float)(-param + cy), (float)(-param + cz));

	glVertex3f((float)(-param + cx), (float)(param + cy), (float)(param + cz));
	glNormal3f(1, 0, 0);
	glVertex3f((float)(-param + cx), (float)(-param + cy), (float)(param + cz));

	glEnd();

	//glPopMatrix();

	return;

	glBegin(GL_QUADS);

	glVertex3f((float)(-param + cx), (float)(param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(param + cy), (float)(-param + cz));

	glVertex3f((float)(-param + cx), (float)(param + cy), (float)(-param + cz));
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f((float)(-param + cx), (float)(-param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(-param + cy), (float)(param + cz));

	glVertex3f((float)(param + cx), (float)(-param + cy), (float)(-param + cz));

	glVertex3f((float)(-param + cx), (float)(-param + cy), (float)(-param + cz));
	glEnd();

}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL By-Line-Loop
void COpenGLView::DrawOpenGL_ByLineLoop(std::vector<PointGeometric> points, VectorGeometric vector)
{
	glBegin(GL_LINE_LOOP);
		for (int i = 0; i < (int)points.size(); i++)
		{
			glNormal3f((GLfloat)vector.X, (GLfloat)vector.Y, (GLfloat)vector.Z);
			glVertex3f(	(GLfloat)points[i].X,
						(GLfloat)points[i].Y ,
						(GLfloat)points[i].Z);
		}
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



//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// On Key Down for __Test__ (AND for Shift checking)
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

