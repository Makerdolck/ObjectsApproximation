// try_OpenGLView.cpp : implementation of the CTry_OpenGLView class
//

#include "stdafx.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/GLUT.h>
#include <gl/Glaux.h>
#include "OpenGLView.h"

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


	if (flagShiftPressed)
	{
		CView::OnLButtonDown(nFlags, point);
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
	if (nChar == VK_SHIFT)
		flagShiftPressed = true;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
		flagShiftPressed = false;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
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


