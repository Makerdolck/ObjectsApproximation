// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//


#include "stdafx.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/GLUT.h>
#include <gl/Glaux.h>
#include "OpenGLView.h"
#include "gl/GL.h"
#define M_PI 3.1415926535897932384626433832795;


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

const GLfloat greenToleranceColor[] = { 0.0f,	1.0f,	0.0f,	1.0f };
//const GLfloat greenToleranceColor[] = { 0.0f,	0.0f,	0.0f,	1.0f };
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
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLView construction/destruction

COpenGLView::COpenGLView()
{
	fNearPlane	= 1.f;
	fFarPlane	= 10000.0f;

	flagMiddleButtonDown	= false;
	flagShiftPressed		= false;

	objectsArray = nullptr;
	toleranceObjectsArray = nullptr; // Voronov

	pointEyeLook.X = 0;
	pointEyeLook.Y = 0;
	pointEyeLook.Z = 100;

	pointAimLook.X = 0;
	pointAimLook.Y = 0;
	pointAimLook.Z = 0;
	
	vectorViewX = VectorGeometric(1.f, 0.f, 0.f);
	vectorViewY = VectorGeometric(0.f, 1.f, 0.f);
	vectorViewZ	= VectorGeometric(pointEyeLook, pointAimLook);

	distanceAimEye = pointAimLook.DistanceToPoint(pointEyeLook);

	flagRotateAxisX = false;
	flagRotateAxisY = false;

	rotx = 0;
	roty = 0;
	prev_rotx = 0;
	prev_roty = 0;

	MatrixIdentity(BoxTrans);

	kioFont = new KIOFont("seguisym.ttf", "arial.ttf", 32);
	//kioFont = new KIOFont("seguisym.ttf", "GOST_type_A.ttf", 32);
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
	
	hRC = SetUpOpenGL(m_hWnd);

	return 0;
}
// ---																							// On Destroy
void COpenGLView::OnDestroy()
{
	CView::OnDestroy();
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Drawing
//--------------------------------------------------------------

// ---																							// On Draw
void COpenGLView::OnDraw(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, hRC);
	//**Draw GL here!
	//glClearColor(0.0,0.0,0.0,0.0);

	glClearDepth(1.0f);                                  //Specifies the clear value for the depth buffer 
	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);             //Set Background Color
	//glColor3fv(redColor);
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
		MatrixCopy(BoxTrans, trans);

		MatrixRotateY(trans, roty - prev_roty);

		MatrixCopy(trans, BoxTrans);  //saving last transformation
		MatrixTranspose(trans);			//making row-major for openGL
	}
	
	if (flagRotateAxisX)
	{
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
    glnWidth = (GLint)cx;
    glnHeight = (GLint)cy;
    gldAspect = (GLfloat)glnWidth/(GLfloat)glnHeight;
	
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
	double			step		= 3.0f+fabs(((int)pointEyeLook.Z)/25);
	LineGeometric	lineOffset	= LineGeometric(pointAimLook, vectorViewZ * (-1));


	if (zDelta < 0)
		distanceAimEye += step;
	else
		distanceAimEye -= step;

	pointEyeLook = lineOffset.CreatePointOnDistance(distanceAimEye);


	//kioFont->SetFontSize(32); // Изменение размеров шрифта при масштабе

	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
// ---																							// On Mouse Move
void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	if (flagToleranceMove) {
		PointGeometric mouseNewInWorld;
		GetWorldCoord(point.x, point.y, 0, mouseNewInWorld);
		selectedToleranceObject->PointPosition = mouseNewInWorld;
		Invalidate(FALSE);
	}


	if (nFlags& MK_LBUTTON && flagShiftPressed)
	{
		if (point.x - prev_x != 0)
		{
			flagRotateAxisY = true;
			roty += (float)((point.x - prev_x) * 0.5);
		}
		else
			flagRotateAxisY = false;
		if (point.y - prev_y != 0)
		{
			flagRotateAxisX = true;
			rotx += (float)((point.y - prev_y) * 0.5);
		}
		else
			flagRotateAxisX = false;

		Invalidate(FALSE);		//The end
	}

	if (flagMiddleButtonDown)
	{

		PointGeometric mouseNewInWorld;
		PointGeometric mouseOldInWorld;

		GetWorldCoord(point.x, point.y, 0, mouseNewInWorld);
		GetWorldCoord(prev_x, prev_y, 0, mouseOldInWorld);
	
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
		

		if (prev_x - point.x < 0)
		{
			lineOffsetEye.Vector = vectorViewX * (-1);
			lineOffsetAim.Vector = vectorViewX * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;
		}
		if (prev_x - point.x > 0)
		{
			lineOffsetEye.Vector = vectorViewX;
			lineOffsetAim.Vector = vectorViewX;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetX);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetX);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;
		}

		if (prev_y - point.y < 0)
		{
			lineOffsetEye.Vector = vectorViewY;
			lineOffsetAim.Vector = vectorViewY;

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;
		}
		if (prev_y - point.y > 0)
		{
			lineOffsetEye.Vector = vectorViewY * (-1);
			lineOffsetAim.Vector = vectorViewY * (-1);

			pointEyeLook = lineOffsetEye.CreatePointOnDistance(stepOffsetY);
			pointAimLook = lineOffsetAim.CreatePointOnDistance(stepOffsetY);

			lineOffsetEye.Point = pointEyeLook;
			lineOffsetAim.Point = pointAimLook;
		}

		Invalidate(FALSE);
	}


	//mouse_x0 = point.x;
	//mouse_y0 = point.y;

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
	//mouse_x0 = point.x; 
	//mouse_y0 = point.y;

	prev_x = point.x;
	prev_y = point.y;

	//Invalidate(FALSE);
	
	if (flagShiftPressed)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	if (flagToleranceMove) {
		flagToleranceMove = false;
		if (selectedToleranceObject != nullptr) {
			selectedToleranceObject->flagSelected = false;
			selectedToleranceObject = nullptr;
			

		}
		return;
	}

	ObjectApprox *objApprox = GetObjectUnderMouse(point);
	ToleranceObject *toleranceObj = GetToleranceObjectUnderMouse(point);

	int a = 0;
	if (objApprox != nullptr)
	{
		objApprox->flagSelected = !objApprox->flagSelected;
	}
	else																
	{
		if (toleranceObj != nullptr) {
			startSelectObject(toleranceObj);
		}
		else {
			if (objectsArray != nullptr)
			{
				for (int i = 0; i < (int)objectsArray->size(); i++)
				{
					objApprox = objectsArray->operator[](i);

					objApprox->flagSelected = false;
				}
			}
			if (toleranceObjectsArray != nullptr)
			{
				for (int i = 0; i < (int)toleranceObjectsArray->size(); i++)
				{
					toleranceObjectsArray->operator[](i)->flagSelected = false;

				}
			}
			
		}
	}
	

	Invalidate(FALSE);
	//RedrawWindow();
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
	//mouse_x0 = point.x;  
	//mouse_y0 = point.y;

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
			if (selectBuf[1 + i * 4] < minz) 
			{ 
				n = i; 
				minz = selectBuf[1 + i * 4]; 
			}
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

ToleranceObject* COpenGLView::GetToleranceObjectUnderMouse(CPoint point)
{


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


	ToleranceObject * obj = nullptr;
	int a = 0;
	if (hits)
	{
		int n = 0; double minz = selectBuf[1];
		for (int i = 1; i < hits; i++)
		{
			if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
		}

		a = selectBuf[3 + n * 4];
		if (toleranceObjectsArray != nullptr)
		{
			for (int i = 0; i < (int)toleranceObjectsArray->size(); i++)
			{
				obj = toleranceObjectsArray->operator[](i);

				if (obj->objID == a)
				{
					
					return obj;
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

	if (toleranceObjectsArray != nullptr) {

		ToleranceObject* toleranceObject;

		for (int i = 0; i < toleranceObjectsArray->size(); i++) {
			toleranceObject = toleranceObjectsArray->operator[](i);

			if (!toleranceObject->isVisible) {
				continue;
			}


			if (toleranceObject->flagSelected) {
				glColor3fv(redColor);
			}
			else {
				glColor3fv(greenToleranceColor);
			}

			if (mode == GL_SELECT) {
				glLoadName(toleranceObject->objID);
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
			else if (dynamic_cast<ToleranceFrame*>(toleranceObject)) {
				DrawOpenGL_ToleranceFrame((ToleranceFrame*)toleranceObject);
			}
			else if (dynamic_cast<ToleranceBase*>(toleranceObject)) {
				DrawOpenGL_ToleranceBase((ToleranceBase*)toleranceObject);
			}
			/*else if (dynamic_cast<AngleLine*>(toleranceObject)) {
				DrawOpenGL_AngleLine((AngleLine*)toleranceObject);
			}*/
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
//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL SizeLine
void COpenGLView::DrawOpenGL_ToleranceBase(ToleranceBase* base)
{
	PointGeometric pStart = base->PointStart;
	PointGeometric pEnd = base->PointPosition;
	VectorGeometric ABNorm;

	if (base->toleranceObject != nullptr) {
		if (dynamic_cast<AxialLine*>(base->toleranceObject)) {
			PointGeometric pAxialStart = ((AxialLine*)base->toleranceObject)->startPoint;
			PointGeometric pAxialEnd = ((AxialLine*)base->toleranceObject)->endPoint;
			VectorGeometric AB = VectorGeometric(pAxialStart, pAxialEnd, false);
			PointGeometric projectionPoint = AB.PointProjection(base->PointPosition, pStart);

			double axialLineLength = pAxialStart.DistanceToPoint(pAxialEnd);
			if (pAxialStart.DistanceToPoint(projectionPoint) + pAxialEnd.DistanceToPoint(projectionPoint) <= axialLineLength+ axialLineLength*0.01) {
				pStart = projectionPoint;
			}
			else {
				PointGeometric tmpEnd = pAxialStart;
				if (pAxialStart.DistanceToPoint(projectionPoint) < pAxialEnd.DistanceToPoint(projectionPoint)) {
					tmpEnd = pAxialStart;
					pStart = projectionPoint;
				}
				else {
					tmpEnd = pAxialEnd;
					pStart = projectionPoint;
				}
				glEnable(GL_LINE_STIPPLE); // разрешаем рисовать прерывистую линию
				glLineStipple(2, 58364); // Паттерн: ---- * ---- * ----
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex3d(pStart.X, pStart.Y, pStart.Z);
				glVertex3d(tmpEnd.X, tmpEnd.Y, tmpEnd.Z);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
			}
			AB.Normalize();
			ABNorm = AB;
		}
	}
	else {
		if (base->objMath->GetName() == LineSegmentApprox().GetName()) {
			PointGeometric pLineStart = ((LineSegmentApprox*)base->objMath)->PointStart;
			PointGeometric pLineEnd = ((LineSegmentApprox*)base->objMath)->PointEnd;
			VectorGeometric AB = VectorGeometric(pLineStart, pLineEnd, false);
			ABNorm = AB;
			ABNorm.Normalize();

			PointGeometric projectionPoint1 = AB.PointProjection(base->PointPosition, pLineStart); // Вдоль оси
			

			double lineLength = pLineStart.DistanceToPoint(pLineEnd);
			if (pLineStart.DistanceToPoint(projectionPoint1) + pLineEnd.DistanceToPoint(projectionPoint1) <= lineLength + lineLength * 0.01) {
				pStart = projectionPoint1;
			}
			else {
				PointGeometric tmpEnd = pLineStart;
				if (pLineStart.DistanceToPoint(projectionPoint1) < pLineEnd.DistanceToPoint(projectionPoint1)) {
					tmpEnd = pLineStart;
					pStart = projectionPoint1;
				}
				else {
					tmpEnd = pLineEnd;
					pStart = projectionPoint1;
				}
				
				glLineWidth(2);
				glBegin(GL_LINES);
				glVertex3d(pStart.X, pStart.Y, pStart.Z);
				glVertex3d(tmpEnd.X, tmpEnd.Y, tmpEnd.Z);
				glEnd();
				
			}
		}
		else if (base->objMath->GetName() == CylinderApprox().GetName()) {
			CylinderApprox *tmpCylinder = ((CylinderApprox*)base->objMath);
			PointGeometric pAxialStart = tmpCylinder->PointTopSurfaceCenter;
			PointGeometric pAxialEnd = tmpCylinder->PointBottomSurfaceCenter;
			VectorGeometric AB = VectorGeometric(pAxialStart, pAxialEnd, false);
			ABNorm = AB;
			ABNorm.Normalize();

			PointGeometric projectionPoint1 = AB.PointProjection(base->PointPosition, pAxialStart); // Вдоль оси
			PointGeometric projectionPoint2 = AB.PointProjection(pAxialStart, base->PointPosition); // Вокруг оси
			VectorGeometric AprojPoint2 = VectorGeometric(pAxialStart, projectionPoint2, true);

			PointGeometric radiusPoint = pAxialStart + AprojPoint2 * tmpCylinder->Radius; // Точка на окружности
			VectorGeometric radiusVector = VectorGeometric(pAxialStart, radiusPoint, true); // Вектор радиуса

			glPointSize(2);
			glBegin(GL_POINTS);
			glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
			glEnd();

			pStart = projectionPoint1 + radiusVector * tmpCylinder->Radius; // Настоящая точка начала

			// Линия от цилиндра (точка радиуса) до точки настоящего начала
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex3d(pStart.X, pStart.Y, pStart.Z);
			glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
			glEnd();

		}
		else if (base->objMath->GetName() == RectangleApprox().GetName()) {
			ABNorm = ((RectangleApprox*)base->objMath)->VectorX;
			ABNorm.Normalize();
		}else if (base->objMath->GetName() == CircleApprox().GetName()) {
			CircleApprox circle = *((CircleApprox*)base->objMath);
			ABNorm = circle.Line.Vector;
			ABNorm.Normalize();
			pStart = circle.Line.Point;
			PlaneGeometric circlePlane = PlaneGeometric(pStart, circle.Line.Vector);
			PointGeometric pEndProj = circlePlane.PointProjection(pEnd);
			pEnd = pEndProj;
			VectorGeometric radiusVector = VectorGeometric(pStart, pEndProj, true);
			ABNorm = ABNorm ^ radiusVector;
			pStart = pStart + radiusVector * circle.Radius;
		}


	}

	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex3d(pStart.X, pStart.Y, pStart.Z);
		glVertex3d(pEnd.X, pEnd.Y, pEnd.Z);
	glEnd();

	VectorGeometric lineVector = VectorGeometric(pStart, pEnd, true);
	

	double triangle_height = 0.5;
	double triangle_width = triangle_height;

	// Треугольник
	PointGeometric rightPoint = pStart + ABNorm * triangle_width;
	PointGeometric leftPoint = pStart - ABNorm * triangle_width;
	PointGeometric topPoint = pStart + lineVector * triangle_height;
	glBegin(GL_TRIANGLES);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
		glVertex3d(topPoint.X, topPoint.Y, topPoint.Z);
	glEnd();

	
	// Буква базы
	glPushMatrix();
	glRasterPos3f(pEnd.X, pEnd.Y, pEnd.Z);
	kioFont->RussianFont->Render(base->baseChar);
	glPopMatrix();


	
	float modelview[16];
	int i, j;
	// save the current modelview matrix
	glPushMatrix();
	// get the current modelview matrix
	glTranslated(pEnd.X, pEnd.Y, pEnd.Z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	// undo all rotations
	// beware all scaling is lost as well
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			if (i == j)
				modelview[i * 4 + j] = 1.0;
			else
				modelview[i * 4 + j] = 0.0;
		}
	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
	
	// Рамка допуска
	double boxWidth = 1 * distanceAimEye / 55;
	double boxHeight = 1.1 * distanceAimEye / 55;
	glBegin(GL_LINE_LOOP);
		glVertex2d(0, 0);
		glVertex2d(boxWidth, 0);
		glVertex2d(boxWidth, boxHeight);
		glVertex2d(0, boxHeight);
		glVertex2d(0, 0);
	glEnd();
	glPopMatrix();
}

void COpenGLView::DrawOpenGL_ToleranceFrame(ToleranceFrame* frame)
{
	PointGeometric pStart = frame->PointStart;
	PointGeometric pEnd = frame->PointPosition;
	VectorGeometric ABNorm;
	
	if (frame->objMath->GetName() == LineSegmentApprox().GetName()) {
		ABNorm = VectorGeometric(((LineSegmentApprox*)frame->objMath)->PointStart, ((LineSegmentApprox*)frame->objMath)->PointEnd, true);
	}
	else if (frame->objMath->GetName() == CylinderApprox().GetName() || frame->objMath->GetName() == ConeApprox().GetName()) {
		PointGeometric pAxialStart = ((CylinderApprox*)frame->objMath)->PointTopSurfaceCenter;
		PointGeometric pAxialEnd = ((CylinderApprox*)frame->objMath)->PointBottomSurfaceCenter;

		if (frame->objMath->GetName() == ConeApprox().GetName()) {
			pAxialStart = ((ConeApprox*)frame->objMath)->PointTopSurfaceCenter;
			pAxialEnd = ((ConeApprox*)frame->objMath)->PointBottomSurfaceCenter;
		}

		VectorGeometric AB = VectorGeometric(pAxialStart, pAxialEnd, false);
		ABNorm = VectorGeometric(pAxialStart, pAxialEnd, true);

		PointGeometric projectionPoint1 = AB.PointProjection(frame->PointPosition, pAxialStart); // Вдоль оси
		PointGeometric projectionPoint2 = AB.PointProjection(pAxialStart, frame->PointPosition); // Вокруг оси
		VectorGeometric AprojPoint2 = VectorGeometric(pAxialStart, projectionPoint2, true);

		PointGeometric radiusPoint = pAxialStart + AprojPoint2 * ((CylinderApprox*)frame->objMath)->Radius; // Точка на окружности
		VectorGeometric radiusVector = VectorGeometric(pAxialStart, radiusPoint, true); // Вектор радиуса
		
		glPointSize(2);
		glBegin(GL_POINTS);
		glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
		glEnd();
		
		pStart = projectionPoint1 + radiusVector * ((CylinderApprox*)frame->objMath)->Radius; // Настоящая точка начала

		// Линия от цилиндра (точка радиуса) до точки настоящего начала
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3d(pStart.X, pStart.Y, pStart.Z);
		glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
		glEnd();
		
	}else if (frame->objMath->GetName() == RectangleApprox().GetName()) {
		ABNorm = ((RectangleApprox*)frame->objMath)->VectorX;
		ABNorm.Normalize();
	}else if (frame->objMath->GetName() == CircleApprox().GetName()) {
		CircleApprox circle = *((CircleApprox*)frame->objMath);
		ABNorm = circle.Line.Vector;
		ABNorm.Normalize();
		pStart = circle.Line.Point;
		PlaneGeometric circlePlane = PlaneGeometric(pStart, circle.Line.Vector);
		PointGeometric pEndProj = circlePlane.PointProjection(pEnd);
		pEnd = pEndProj;
		VectorGeometric radiusVector = VectorGeometric(pStart, pEndProj, true);
		ABNorm = ABNorm ^ radiusVector;
		pStart = pStart + radiusVector * circle.Radius;
	}

	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex3d(pStart.X, pStart.Y, pStart.Z);
		glVertex3d(pEnd.X, pEnd.Y, pEnd.Z);
	glEnd();
	VectorGeometric lineVector = VectorGeometric(pStart, pEnd, true);

	// Треугольник
	double triangle_height = 0.6;
	double triangle_width = 0.3;
	PointGeometric rightPoint = pStart + ABNorm * triangle_width + lineVector * triangle_height;
	PointGeometric leftPoint = pStart - ABNorm * triangle_width + lineVector * triangle_height;
	PointGeometric topPoint = pStart;
	glBegin(GL_TRIANGLES);
	glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
	glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
	glVertex3d(topPoint.X, topPoint.Y, topPoint.Z);
	glEnd();

	float modelview[16];
	int i, j;
	glPushMatrix();
	glTranslated(pEnd.X, pEnd.Y, pEnd.Z);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			if (i == j)
				modelview[i * 4 + j] = 1.0;
			else
				modelview[i * 4 + j] = 0.0;
		}
	glLoadMatrixf(modelview);
	
	CString bageStr = frame->toleranceBage;
	CString frameString = L"";
	if (frame->Base != nullptr) {
		frameString.Format(L"%s %g %c", bageStr, frame->toleranceValue, frame->Base->baseChar);
	}
	else {
		frameString.Format(L"%s %g", bageStr, frame->toleranceValue);
	}
	double boxWidth = ((double)frameString.GetLength() / 100) * distanceAimEye;
	frameString.Format(L"%s %g", bageStr, frame->toleranceValue);


	double bageWidth = distanceAimEye / 60;
	double boxHeight = distanceAimEye / 60;

	glBegin(GL_LINE_LOOP);
		glVertex2d(0, 0);
		glVertex2d(boxWidth, 0);
		glVertex2d(boxWidth, boxHeight);
		glVertex2d(0, boxHeight);
		glVertex2d(0, 0);
	glEnd();
	frameString.Format(L"%s", bageStr);
	glRasterPos3f(0, 0, 0);
	kioFont->BadgeFont->Render(frameString);
	
	glBegin(GL_LINES);
		glVertex2d(bageWidth, 0);
		glVertex2d(bageWidth, boxHeight);
	glEnd();
	frameString.Format(L"%g", frame->toleranceValue);
	glRasterPos3f(bageWidth, 0,0);
	kioFont->RussianFont->Render(frameString);

	if(frame->Base != nullptr){
		glBegin(GL_LINES);
			glVertex2d(boxWidth-bageWidth, 0);
			glVertex2d(boxWidth-bageWidth, boxHeight);
		glEnd();
		glPushMatrix();
		glRasterPos3f(boxWidth - bageWidth, 0,0);
		kioFont->RussianFont->Render(frame->Base->baseChar);
	}
	glPopMatrix();
}

void COpenGLView::DrawOpenGL_DiameterLine(DiameterLine* obj)
{
	double triangle_height = 0.7;
	double triangle_widht = triangle_height;

	VectorGeometric n = obj->objMath->Line.Vector;
	PointGeometric centerPoint = obj->centerPoint - (n * 0.01);
	
	VectorGeometric p = VectorGeometric(obj->PointPosition, centerPoint);
	p = Tolerance().rotatePlane(&p, n, 90);
	VectorGeometric perp = n ^ p;
	perp.Normalize();
	n.Normalize();

	PointGeometric leftPoint = centerPoint + perp * (obj->diameter / 2);
	PointGeometric rightPoint = centerPoint - perp * (obj->diameter / 2);
	glLineWidth(2);


	
	glPointSize(8);
	// Центральная точка
	glBegin(GL_POINTS);
		glVertex3d(centerPoint.X, centerPoint.Y, centerPoint.Z);
	glEnd();

	
	// Линия диаметра (внутри окружности)
	glBegin(GL_LINES);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
	glEnd();

	VectorGeometric g = VectorGeometric(obj->objMath->PointsForApprox.operator[](1), centerPoint);
	VectorGeometric perpTrinagle = n ^ perp;
	perpTrinagle.Normalize();

	PointGeometric leftTopPoint = leftPoint + perp * triangle_height + perpTrinagle * triangle_widht;
	PointGeometric leftBottomPoint = leftPoint + perp * triangle_height - perpTrinagle * triangle_widht;
	// Левый треугольник
	glBegin(GL_TRIANGLES);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
		glVertex3d(leftTopPoint.X, leftTopPoint.Y, leftTopPoint.Z);
		glVertex3d(leftBottomPoint.X, leftBottomPoint.Y, leftBottomPoint.Z);
	glEnd();

	PointGeometric rightTopPoint = rightPoint - perp * triangle_height + perpTrinagle * triangle_widht;
	PointGeometric rightBottomPoint = rightPoint - perp * triangle_height - perpTrinagle * triangle_widht;
	// Правый треугольник
	glBegin(GL_TRIANGLES);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
		glVertex3d(rightTopPoint.X, rightTopPoint.Y, rightTopPoint.Z);
		glVertex3d(rightBottomPoint.X, rightBottomPoint.Y, rightBottomPoint.Z);
	glEnd();

	PointGeometric leftEarPoint = leftPoint  + (perp * 6);
	PointGeometric rightEarPoint = rightPoint  - (perp * 6);
	// Левое "Ушко"
	glBegin(GL_LINES);
		glVertex3d(leftEarPoint.X, leftEarPoint.Y, leftEarPoint.Z);
		glVertex3d(leftPoint.X, leftPoint.Y, leftPoint.Z);
	glEnd();
	
	// Правое "Ушко"
	glBegin(GL_LINES);
		glVertex3d(rightEarPoint.X, rightEarPoint.Y, rightEarPoint.Z);
		glVertex3d(rightPoint.X, rightPoint.Y, rightPoint.Z);
	glEnd();
		
	// Линия от правого "ушка" до точки фиксации
	glBegin(GL_LINES);
		glVertex3d(rightEarPoint.X, rightEarPoint.Y, rightEarPoint.Z);
		glVertex3d(obj->PointPosition.X, obj->PointPosition.Y, obj->PointPosition.Z);
	glEnd();

	CString sizeValue = L"";
	sizeValue.Format(L" \u00D8 %g мм", Tolerance().round(obj->diameter, 2));
	glPushMatrix();

	glRasterPos3f(obj->PointPosition.X, obj->PointPosition.Y, obj->PointPosition.Z);
	kioFont->RussianFont->Render(sizeValue);
	glPopMatrix();
}

void COpenGLView::DrawOpenGL_AngleLine(AngleLine* obj)
{
	glLineWidth(1);
	ConeApprox* cone = (ConeApprox*)obj->objMath;
	PointGeometric pEnd = obj->PointPosition;
	PointGeometric pointOnAB = cone->PointBottomSurfaceCenter;
	VectorGeometric AB = VectorGeometric(cone->PointBottomSurfaceCenter, cone->PointTopSurfaceCenter, false);
	VectorGeometric ABNorm = AB;
	ABNorm.Normalize();

	PointGeometric coneTopPoint = pointOnAB + ABNorm * cone->HeightPhantom;

	PointGeometric projectionPoint1 = AB.PointProjection(obj->PointPosition, pointOnAB); // Вдоль оси
	double coneHeight = pointOnAB.DistanceToPoint(cone->PointTopSurfaceCenter);
	if (projectionPoint1.DistanceToPoint(pointOnAB) + projectionPoint1.DistanceToPoint(cone->PointTopSurfaceCenter) <= coneHeight + 0.01) {
		projectionPoint1 = projectionPoint1;
	}
	else if (projectionPoint1.DistanceToPoint(pointOnAB) < projectionPoint1.DistanceToPoint(cone->PointTopSurfaceCenter)) {
		projectionPoint1 = pointOnAB; }
	else { projectionPoint1 = cone->PointTopSurfaceCenter; }

	PointGeometric projectionPoint2 = AB.PointProjection(pointOnAB, obj->PointPosition); // Вокруг оси
	VectorGeometric AprojVec2 = VectorGeometric(pointOnAB, projectionPoint2, true);

	double projToPhantomDistance = projectionPoint1.DistanceToPoint(coneTopPoint);
	double angleInRad = cone->Angle / 180 * M_PI;
	double radiusInProj = projToPhantomDistance * tan(angleInRad);
	PointGeometric radiusPoint = projectionPoint1 + AprojVec2 * radiusInProj; // Точка на окружности

	//От точки начала до точки остановки 
	glBegin(GL_LINES);
	glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
	glVertex3d(pEnd.X, pEnd.Y, pEnd.Z);
	glEnd();

	// Треугольник
	double triangleWidth = 0.5;
	double triangleHeight = triangleWidth;
	VectorGeometric lineVec = VectorGeometric(radiusPoint, pEnd, true);
	PointGeometric leftTrianglePoint = radiusPoint + ABNorm * triangleWidth + lineVec * triangleHeight;
	PointGeometric rightTrianglePoint = radiusPoint - ABNorm * triangleWidth + lineVec * triangleHeight;
	
	glBegin(GL_TRIANGLES);
	glVertex3d(leftTrianglePoint.X, leftTrianglePoint.Y, leftTrianglePoint.Z);
	glVertex3d(radiusPoint.X, radiusPoint.Y, radiusPoint.Z);
	glVertex3d(rightTrianglePoint.X, rightTrianglePoint.Y, rightTrianglePoint.Z);

	glEnd();
	

	// Текст
	CString sizeValue = L"";
	sizeValue.Format(L"\u299F %g\u00B0", Tolerance().round(((ConeApprox*)obj->objMath)->Angle, 2));
	glPushMatrix();
	glRasterPos3f(pEnd.X, pEnd.Y, pEnd.Z);
	kioFont->BadgeFont->Render(sizeValue);
	glPopMatrix();
}

void COpenGLView::DrawOpenGL_SizeLine(SizeLine* obj)
{
	glLineWidth(1);

	
	PointGeometric pMouseEnd = obj->PointPosition;

	VectorGeometric AB = VectorGeometric(obj->PointStart, obj->PointEnd, false);
	VectorGeometric ABNormalized = VectorGeometric(obj->PointStart, obj->PointEnd, true);
	PointGeometric projectionPoint1 = AB.PointProjection(pMouseEnd, obj->PointStart); // Вдоль оси
	if (obj->objMath->GetName() == CylinderApprox().GetName() && obj->objMath2->GetName() == CylinderApprox().GetName()) {
		CylinderApprox* cylinder = (CylinderApprox*)obj->objMath;
		AB = VectorGeometric(cylinder->PointBottomSurfaceCenter, cylinder->PointTopSurfaceCenter, false);
		ABNormalized = AB;
		ABNormalized.Normalize();
	 projectionPoint1 = AB.PointProjection(obj->PointStart, pMouseEnd ); // Вдоль оси
	}



	VectorGeometric ABPerp = VectorGeometric(projectionPoint1, pMouseEnd, true); // Перпендикуляр вектору между объектами в сторону курсора мыши

	// Высота параллельных прямых
	double height = pMouseEnd.DistanceToPoint(projectionPoint1);

	// Параллельные прямые
	PointGeometric leftLineTop = obj->PointStart + ABPerp * height;
	PointGeometric rightLineTop = obj->PointEnd + ABPerp * height;
	glBegin(GL_LINES);
		glVertex3d(obj->PointStart.X, obj->PointStart.Y, obj->PointStart.Z);
		glVertex3d(leftLineTop.X , leftLineTop.Y, leftLineTop.Z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3d(obj->PointEnd.X, obj->PointEnd.Y, obj->PointEnd.Z);
		glVertex3d(rightLineTop.X, rightLineTop.Y, rightLineTop.Z);
	glEnd();

	// Ушки
	double dh = 0.3;
	PointGeometric leftLineTopDh = obj->PointStart + ABPerp * (height + dh);
	PointGeometric rightLineTopDh = obj->PointEnd + ABPerp * (height + dh);
	glBegin(GL_LINES);
		glVertex3d(leftLineTop.X, leftLineTop.Y, leftLineTop.Z);
		glVertex3d(leftLineTopDh.X, leftLineTopDh.Y, leftLineTopDh.Z);
	glEnd();

	glBegin(GL_LINES);
		glVertex3d(rightLineTop.X, rightLineTop.Y, rightLineTop.Z);
		glVertex3d(rightLineTopDh.X, rightLineTopDh.Y, rightLineTopDh.Z);
	glEnd();

	// Линия между параллельными прямыми
	glBegin(GL_LINES);
		glVertex3d(leftLineTop.X, leftLineTop.Y, leftLineTop.Z);
		glVertex3d(rightLineTop.X, rightLineTop.Y, rightLineTop.Z);
	glEnd();


	// Треугольники
	double lineWidth = leftLineTop.DistanceToPoint(rightLineTop);
	double triangle_width = 0.025 * lineWidth;
	double triangle_height = triangle_width;
	// Левый
		PointGeometric leftTriangleTop = leftLineTop + ABNormalized * triangle_height + ABPerp * triangle_width;
		PointGeometric leftTriangleBottom= leftLineTop + ABNormalized * triangle_height - ABPerp * triangle_width;
		glBegin(GL_TRIANGLES);
			glVertex3d(leftTriangleTop.X, leftTriangleTop.Y, leftTriangleTop.Z);
			glVertex3d(leftLineTop.X, leftLineTop.Y, leftLineTop.Z);
			glVertex3d(leftTriangleBottom.X, leftTriangleBottom.Y, leftTriangleBottom.Z);
		glEnd();

	// Правый
		PointGeometric rightTriangleTop = rightLineTop - ABNormalized * triangle_height + ABPerp * triangle_width;
		PointGeometric rightTriangleBottom = rightLineTop - ABNormalized * triangle_height - ABPerp * triangle_width;
		glBegin(GL_TRIANGLES);
			glVertex3d(rightTriangleTop.X, rightTriangleTop.Y, rightTriangleTop.Z);
			glVertex3d(rightLineTop.X, rightLineTop.Y, rightLineTop.Z);
			glVertex3d(rightTriangleBottom.X, rightTriangleBottom.Y, rightTriangleBottom.Z);
		glEnd();


	// Текст
	CString sizeValue = L"10.5 мм";
	//CString sizeValue = L"";
	//sizeValue.Format(L"%g мм", Tolerance().round(obj->length(), 2));
	PointGeometric centerLine = ((leftLineTop + rightLineTop) / 2) + (ABPerp * 2.5) + ABNormalized * 3;
	glPushMatrix();
	glRasterPos3f(centerLine.X, centerLine.Y, centerLine.Z);
	kioFont->RussianFont->Render(sizeValue);
	glPopMatrix();

}

void COpenGLView::DrawOpenGL_AxialLine(AxialLine * obj)
{
	glLineWidth(2);
	glEnable(GL_LINE_STIPPLE); // разрешаем рисовать прерывистую линию
	glLineStipple(2, 58364); // Паттерн: ---- * ---- * ----
	glBegin(GL_LINES);
		glVertex3d(obj->startPoint.X, obj->startPoint.Y, obj->startPoint.Z);
		glVertex3d(obj->endPoint.X, obj->endPoint.Y, obj->endPoint.Z);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
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

	/*
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 0; i < obj.PointsForApprox.size(); i++) {
		PointGeometric point = obj.PointsForApprox.operator[](i);
		glVertex3d(point.X, point.Y, point.Z);
	}
	glEnd();

	CircleApprox* circle = (CircleApprox*)& obj;
	PointGeometric pmin;
	PointGeometric pmax;
	PointGeometric pCenter = circle->Line.Point;
	VectorGeometric vecNorm = circle->Line.Vector;
	vecNorm.Normalize();
	PointGeometric pTop = pCenter + vecNorm * 10;
	VectorGeometric AB = vecNorm;
	AB.Normalize();
	double ABLength = pCenter.DistanceToPoint(pTop);
	double min = 1000;
	double max = -1000;

	for (int i = 0; i < circle->PointsForApprox.size(); i++) {
		PointGeometric pointProjection = AB.PointProjection(circle->PointsForApprox.operator[](i), pCenter);
		glBegin(GL_POINTS);
			glVertex3d(pointProjection.X, pointProjection.Y, pointProjection.Z);
		glEnd();

		glBegin(GL_LINES);

			glVertex3d(pointProjection.X, pointProjection.Y, pointProjection.Z);
			glVertex3d(circle->PointsForApprox.operator[](i).X, circle->PointsForApprox.operator[](i).Y, circle->PointsForApprox.operator[](i).Z);

		glEnd();


		double distanceToCenter = pointProjection.DistanceToPoint(circle->PointsForApprox.operator[](i));

		CString sizeValue = L"";
		sizeValue.Format(L"%g", distanceToCenter);
		glPushMatrix();

		glRasterPos3f(circle->PointsForApprox.operator[](i).X, circle->PointsForApprox.operator[](i).Y, circle->PointsForApprox.operator[](i).Z);
		kioFont->RussianFont->Render(sizeValue);
		glPopMatrix();

		TRACE("distance: %g\n", distanceToCenter);
		
			if (distanceToCenter > max) {
				max = distanceToCenter;
				pmax = circle->PointsForApprox.operator[](i);
			}else if (distanceToCenter < min) {
				min = distanceToCenter;
				pmin = circle->PointsForApprox.operator[](i);
			}
	}

	glPointSize(15);
	glBegin(GL_POINTS);
		glVertex3d(pCenter.X, pCenter.Y, pCenter.Z);
		glVertex3d(pmax.X, pmax.Y, pmax.Z);
		glVertex3d(pmin.X, pmin.Y, pmin.Z);
	glEnd();
	*/

	
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


	/*
	glPointSize(1);
	glBegin(GL_POINTS);
	for (int i = 0; i < obj->PointsForApprox.size(); i++) {
		PointGeometric point = obj->PointsForApprox.operator[](i);
		glVertex3d(point.X, point.Y, point.Z);
	}
	glEnd();

	PointGeometric pStart = obj->PointsForApprox.operator[](0);
	PointGeometric pEnd = obj->PointsForApprox.operator[](obj->PointsForApprox.size()-1);
	PointGeometric pMax;
	
	double max = 0;
	for (int i = 1; i < obj->PointsForApprox.size() - 1; i++) {
		double distance = Tolerance().DistanceBetween(pStart, pEnd, obj->PointsForApprox.operator[](i));

		CString sizeValue = L"";
		sizeValue.Format(L"%g", distance);
		glPushMatrix();

		glRasterPos3f(obj->PointsForApprox.operator[](i).X, obj->PointsForApprox.operator[](i).Y, obj->PointsForApprox.operator[](i).Z);
		kioFont->RussianFont->Render(sizeValue);
		glPopMatrix();

		if (distance > max) {
			max = distance;
			pMax = obj->PointsForApprox.operator[](i);
			TRACE("distance: %g\n", distance);
		}
	}

	glPointSize(15);
	glBegin(GL_POINTS);

		glVertex3d(pMax.X, pMax.Y, pMax.Z);
	
	glEnd();*/

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

	/*
	glPointSize(2);
	glBegin(GL_POINTS);
	for (int i = 0; i < obj.PointsForApprox.size(); i++) {
		PointGeometric point = obj.PointsForApprox.operator[](i);
		glVertex3d(point.X, point.Y, point.Z);
	}
	glEnd();

	PlaneApprox *plane = (PlaneApprox*)& obj;
	PointGeometric pmin;
	PointGeometric pmax;

	PointGeometric pCenter = plane->Line.Point;
	VectorGeometric vecNorm = plane->Line.Vector;
	vecNorm.Normalize();
	PointGeometric pTop = pCenter + vecNorm * 10;
	VectorGeometric AB = VectorGeometric(pCenter, pTop, false);
	double ABLength = pCenter.DistanceToPoint(pTop);
	double min = -1000;
	double max = -1000;

	for (int i = 0; i < plane->PointsForApprox.size(); i++) {
		PointGeometric pointProjection = AB.PointProjection(plane->PointsForApprox.operator[](i), pCenter);
		glBegin(GL_POINTS);
			glVertex3d(pointProjection.X, pointProjection.Y, pointProjection.Z);
		glEnd();

		glBegin(GL_LINES);
		
			glVertex3d(pointProjection.X, pointProjection.Y, pointProjection.Z);
			glVertex3d(plane->PointsForApprox.operator[](i).X, plane->PointsForApprox.operator[](i).Y, plane->PointsForApprox.operator[](i).Z);
		
		glEnd();


		double distanceToCenter = pointProjection.DistanceToPoint(pCenter);

		CString sizeValue = L"";
		sizeValue.Format(L"%g", distanceToCenter);
		glPushMatrix();

		glRasterPos3f(plane->PointsForApprox.operator[](i).X, plane->PointsForApprox.operator[](i).Y, plane->PointsForApprox.operator[](i).Z);
		kioFont->RussianFont->Render(sizeValue);
		glPopMatrix();

		TRACE("distance: %g\n", distanceToCenter);
		double distanceToTop = pointProjection.DistanceToPoint(pTop);
		if (distanceToCenter + distanceToTop <= ABLength) {
			if (distanceToCenter > max) {
				max = distanceToCenter;
				pmax = plane->PointsForApprox.operator[](i);
			}
		}
		else {
			if (distanceToCenter > min) {
				min = distanceToCenter;
				pmin = plane->PointsForApprox.operator[](i);
			}
		}
	}

	glPointSize(7);
	glBegin(GL_POINTS);

		glVertex3d(pmax.X, pmax.Y, pmax.Z);
		glVertex3d(pmin.X, pmin.Y, pmin.Z);

	glEnd();*/

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
	if (nChar == VK_SHIFT)
		flagShiftPressed = true;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		flagShiftPressed = false;


	if (nChar == VK_DELETE)
	{
		if (flagToleranceMove) {
			if (selectedToleranceObject != nullptr) {
				if (dynamic_cast<AxialLine*>(selectedToleranceObject)) {

					for (int j = 0; j < toleranceObjectsArray->size(); j++) {
						ToleranceObject* tmpBase = toleranceObjectsArray->operator[](j);
						if (dynamic_cast<ToleranceBase*>(tmpBase)) {
							std::vector<ToleranceObject*>::iterator k = std::find(toleranceObjectsArray->begin(), toleranceObjectsArray->end(), tmpBase);
							delete (ToleranceBase*)tmpBase;
							toleranceObjectsArray->erase(k);
						}
					}
				}
				
				std::vector<ToleranceObject*>::iterator j = std::find(toleranceObjectsArray->begin(), toleranceObjectsArray->end(), selectedToleranceObject);
				if (dynamic_cast<ToleranceBase*>(selectedToleranceObject)) {
					delete (ToleranceBase*)selectedToleranceObject;
				}
				else {
					delete selectedToleranceObject;
				}
				toleranceObjectsArray->erase(j);
				selectedToleranceObject = nullptr;
				flagToleranceMove = false;
			}
				
		}
		Invalidate(FALSE);
		
	}


	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void COpenGLView::startSelectObject(ToleranceObject* selectedObject)
{
		selectedToleranceObject = nullptr;
		flagToleranceMove = true;
		selectedObject->flagSelected = true;
		selectedToleranceObject = selectedObject;
}


//ObjectApprox* COpenGLView::GetObjectUnderMouse(CPoint point)
//{
//	...
//
//	hits = glRenderMode(GL_RENDER);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glViewport(0, 0, glnWidth, glnHeight);
//	gluPerspective(30.0, gldAspect, fNearPlane, fFarPlane);
//
//	ObjectApprox* objApprox = nullptr;
//	int a = 0;
//	if (hits)
//	{
//		int n = 0; double minz = selectBuf[1];
//		for (int i = 1; i < hits; i++)
//		{
//			if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
//		}
//
//		a = selectBuf[3 + n * 4];
//
//		if (objectsArray != nullptr)
//		{
//			for (int i = 0; i < (int)objectsArray->size(); i++)
//			{
//				objApprox = objectsArray->operator[](i);
//
//				if (objApprox->objID == a)
//				{
//					return objApprox;
//				}
//			}
//		}
//	}
//
//	return nullptr;
//}



