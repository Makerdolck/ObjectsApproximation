
// ExampleView.cpp : implementation of the CExampleView class
//

#include "stdafx.h"
#include "Example.h"

#include "ExampleDoc.h"
#include "ExampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExampleView

IMPLEMENT_DYNCREATE(CExampleView, CView)

BEGIN_MESSAGE_MAP(CExampleView, CView)
	//{{AFX_MSG_MAP(CExampleView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
//	ON_COMMAND(ID_MYCOMAND_DRAWPOINT, &CExampleView::OnMycomandDrawpoint)
//	ON_WM_PAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExampleView construction/destruction

CExampleView::CExampleView()
{
	//
}

CExampleView::~CExampleView()
{
	//
}

BOOL CExampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return COpenGLView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CExampleView diagnostics

#ifdef _DEBUG
void CExampleView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CExampleView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

CExampleDoc* CExampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExampleDoc)));
	return (CExampleDoc*)m_pDocument;
}
#endif //_DEBUG

//--------------------------------------------------------------
//	----	Message Handlers		----	Initial Messages
//--------------------------------------------------------------

// ---																							// On Create
int CExampleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//PIXELFORMATDESCRIPTOR pfd;
	//int iPixelFormat;
	//CDC* pDC;

	//if (CView::OnCreate(lpCreateStruct) == -1)
	//	return -1;

	//// TODO: Add your specialized creation code here
	//pDC = GetDC();
	//memset(&pfd, 0, sizeof(pfd));
	//pfd.nSize = sizeof(pfd);
	//pfd.nVersion = 1;
	//pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;//| PFD_DOUBLEBUFFER;
	//pfd.iPixelType = PFD_TYPE_RGBA;
	//pfd.iLayerType = PFD_MAIN_PLANE;
	//pfd.cDepthBits = 16;
	//iPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	//SetPixelFormat(pDC->m_hDC, iPixelFormat, &pfd);
	//m_hglrc = wglCreateContext(pDC->m_hDC);
	//wglMakeCurrent(pDC->m_hDC, m_hglrc);
	//ReleaseDC(pDC);


	//fNearPlane = 1.0f;
	//fAspect = 1.0f;
	//fFarPlane = 1000.0f;

	//m_z = (fNearPlane + fFarPlane) * 0.5f;

	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	//glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	//glEnable(GL_LIGHT1);
	////	glEnable(GL_LIGHTING);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	//glMatrixMode(GL_MODELVIEW);



	//glEnable(GL_DEPTH_TEST);

	COpenGLView::OnCreate(lpCreateStruct);

    return 0;
}
// ---																							// On Destroy
void CExampleView::OnDestroy() 
{
	COpenGLView::OnDestroy();
	
	// TODO: Add your message handler code here
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Drawing
//--------------------------------------------------------------

// ---																							// On Draw
void CExampleView::OnDraw(CDC* pDC)
{
	//CRect clientRect;

	CExampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	////// TODO: add draw code for native data here
	////GetClientRect(&clientRect);
	////glViewport(0, 0, clientRect.right, clientRect.bottom);

	////glClearColor(1, 1, 1, 0);			// ���� ����
	////glClear(GL_COLOR_BUFFER_BIT);      // ������� ������ �����

	////glEnable(GL_LINE_SMOOTH);

	//////glPointSize (4);					// ������ �����
	////glColor3f(0, 0, 0);				// ������� ���� ����������

	////glLineWidth(20);

	////glBegin(GL_LINE_LOOP);
	////glVertex2f(0, 0.5);
	////glVertex2f(0.5, -0.5);
	////glVertex2f(-0.5, -0.5);
	////glEnd();


	////glDisable(GL_LINE_SMOOTH);

	////glFlush();
	//////SwapBuffers(pDC->m_hDC);



	//glClearDepth(1.0f);                                  //Specifies the clear value for the depth buffer 
	//glClearColor(0.99f, 0.99f, 0.99f, 1.0f);             //Set Background Color
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Indicate the buffers to be cleared
	//glPushMatrix();                                      //push the current matrix stack

	////glFrustum(-1, 1, -1, 1, 3, 1000.0f);

	//glTranslatef(0.0f + wTransformX, 0.0f + wTransformY, -m_z);                      //move object far-near
	//glRotatef(wAngleX, 1.0f, 0.0f, 0.0f);                //rotate object    
	//glRotatef(wAngleY, 0.0f, 1.0f, 0.0f);                //around the axe
	//glRotatef(wAngleZ, 0.0f, 0.0f, 1.0f);                //specified

	//glColor3f(0.85f, 0.85f, 0.85f);				// ������� ���� ����������


	//////DrawOpenGL_Cube(10, 15, 0, 0);
	////DrawOpenGL_Cube(10, 0, 0, 0);



	///*DrawOpenGL_Cylinder();
	//DrawOpenGL_LineSegment();
	//DrawOpenGL_PlaneViaRectangle();
	//DrawOpenGL_Cone();
	//DrawOpenGL_Sphere();
	//DrawOpenGL_Circle();
	//DrawOpenGL_Point();*/


	//DrawOpenGL_SceneObjects(GL_RENDER);

	//////	---	---	---

	//glPopMatrix();                                        //Pop the current matrix stack 
	//glFinish();                                           //Blocks until all OpenGL execution is complete
	////SwapBuffers(wglGetCurrentDC());                       //Exchanges the front and back buffers 
}
// ---																							// On Size
void CExampleView::OnSize(UINT nType, int cx, int cy)
{
	COpenGLView::OnSize(nType, cx, cy);

	/*if (cy > 0)
	{
		glViewport(0, 0, cx, cy);

		if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glnWidth = (GLsizei)cx;
		glnHeight = (GLsizei)cy;
		fAspect = (GLdouble)cx / cy;

		gluPerspective(45.0f, (GLdouble)cx / cy, fNearPlane, fFarPlane);
		glMatrixMode(GL_MODELVIEW);
	}*/
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Standart Messages
//--------------------------------------------------------------

// ---																							// On Mouse Move
void CExampleView::OnMouseMove(UINT nFlags, CPoint point)
{
	////CRect rectClient;

	////GetClientRect(&rectClient);

	///*m_XMousePos = (float)(2 * point.x)						 / (float)(rectClient.right) - 1;
	//m_YMousePos = (float)(2 * (rectClient.bottom - point.y)) / (float)(rectClient.bottom) - 1;*/

	////InvalidateRect(NULL, FALSE);

	//if (nFlags & MK_LBUTTON)
	//{
	//	wAngleY += (point.x - mouse_x0);
	//	wAngleX += (point.y - mouse_y0);


	//	mouse_x0 = point.x;  mouse_y0 = point.y;

	//	Invalidate(FALSE);
	//}

	//double step = 2.0f;
	//if (flagMiddleButtonDown)
	//{
	//	if (mouse_x0 - point.x < 0)
	//		wTransformX += fabs(point.x - mouse_x0);
	//	if(mouse_x0 - point.x > 0)
	//		wTransformX -= fabs(point.x - mouse_x0);
	//	
	//	if (mouse_y0 - point.y < 0)
	//		wTransformY -= fabs(point.y - mouse_y0);
	//	if(mouse_y0 - point.y > 0)
	//		wTransformY += fabs(point.y - mouse_y0);

	//	mouse_x0 = point.x;
	//	mouse_y0 = point.y;

	//	Invalidate(FALSE);
	//}
	//
	////RedrawWindow();

	COpenGLView::OnMouseMove(nFlags, point);
}
// ---																							// On Left Button Down
void CExampleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//mouse_x0 = point.x;  mouse_y0 = point.y;

	////	---	---	---	---	---

	//GLuint selectBuf[BUFSIZE];
	//GLint hits;
	//GLint viewport[4];


	///*glSelectBuffer(512, selectBuf);
	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//	glRenderMode(GL_SELECT);

	//	glInitNames();
	//	glPushName(-1);

	//	glLoadIdentity();
	//	gluPickMatrix(point.x, viewport[3] - point.y, 2, 2, viewport);
	//	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	//	glMatrixMode(GL_MODELVIEW);
	//		DrawOpenGL_SceneObjects(GL_SELECT);
	//		hits = glRenderMode(GL_RENDER);
	//	glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);*/

	////glGetIntegerv(GL_VIEWPORT, viewport);
	////glSelectBuffer(BUFSIZE, selectBuf);

	////glRenderMode(GL_SELECT);				// Enter the SELECT render mode
	////glInitNames();
	////glPushName(-1);

	////glMatrixMode(GL_PROJECTION);
	////glPushMatrix();
	////glLoadIdentity();
	////gluPickMatrix((GLdouble)point.x, (GLdouble)(viewport[3] - point.y), 5.0, 5.0, viewport);
	////gluPerspective(30.0, gldAspect, 1.0, 20.0);
	////glMatrixMode(GL_MODELVIEW);


	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glSelectBuffer(BUFSIZE, selectBuf);

	//glRenderMode(GL_SELECT);				// Enter the SELECT render mode
	//glInitNames();
	//glPushName(-1);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//	//glRenderMode(GL_SELECT);
	//	glLoadIdentity();
	//	gluPickMatrix(point.x, viewport[3] - point.y, 2, 2, viewport);
	//	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	//	glMatrixMode(GL_MODELVIEW);
	//		DrawOpenGL_SceneObjects(GL_SELECT);
	//	glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);


	////glPopMatrix();
	////glFlush();
	////
	//
	//
	//hits = glRenderMode(GL_RENDER);

	/////*glMatrixMode(GL_PROJECTION);
	////glLoadIdentity();
	////glViewport(0, 0, glnWidth, glnHeight);
	////gluPerspective(30.0, gldAspect, 1.0, 20.0);*/

	////gluPerspective(45.0f, (GLdouble)cx / cy, fNearPlane, fFarPlane);

	//int a = 0;
	//if (hits)
	//{
	//	int n = 0; double minz = selectBuf[1];
	//	for (int i = 1; i < hits; i++)
	//	{
	//		if (selectBuf[1 + i * 4] < minz) { n = i; minz = selectBuf[1 + i * 4]; }
	//	}
	//	a = selectBuf[3 + n * 4];
	//	
	//}
	//
	//
	//RedrawWindow();
	
	COpenGLView::OnLButtonDown(nFlags, point);
}
// ---																							// On Mouse Wheel
BOOL CExampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//double step = 15.0f;

	//if (zDelta < 0)
	//	m_z += step;
	//else 
	//	m_z -= step;

	//Invalidate(FALSE);
	////RedrawWindow();

	return COpenGLView::OnMouseWheel(nFlags, zDelta, pt);
}
// ---																							// On Middle Button Down
void CExampleView::OnMButtonDown(UINT nFlags, CPoint point)
{
	//flagMiddleButtonDown = true;

	//mouse_x0 = point.x;  mouse_y0 = point.y;
	
	COpenGLView::OnMButtonDown(nFlags, point);
}
// ---																							// On Middle Button UP
void CExampleView::OnMButtonUp(UINT nFlags, CPoint point)
{
	//flagMiddleButtonDown = false;

	COpenGLView::OnMButtonUp(nFlags, point);
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Objects in Scene
void CExampleView::DrawOpenGL_SceneObjects(GLenum mode)
{

	if (mode == GL_SELECT)
	{
		glLoadName(105);
	}
	DrawOpenGL_Cube(10, 0, -1, 0);


}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cube
void CExampleView::DrawOpenGL_Cube(double param, double cx, double cy, double cz)
{
	param /= 2;

	//glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);

	glNormal3f(0, 0, 1);

	glVertex3f(-param+cx, param + cy, param + cz);

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
void CExampleView::DrawOpenGL_Circle()
{

	std::ifstream					file;
	CircleApprox					circle;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataSphere.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		circle.FindByPoints(&points[0], (int)points.size(), accuracy);


		glColor3d(0, 0.4f, 0.8f);
		glLineWidth(3);

		glBegin(GL_LINE_LOOP);
			int i;
			for (i = 0; i < circle.Mesh.points.size(); i++)
			{
				glVertex3f(circle.Mesh.points[i].X, circle.Mesh.points[i].Y, circle.Mesh.points[i].Z);
			}
			glNormal3f(circle.Mesh.vectorsNormal[0].X, circle.Mesh.vectorsNormal[0].Y, circle.Mesh.vectorsNormal[0].Z);

		glEnd();

		/*glColor3d(1, 0, 0);
		glPointSize(3);
		glBegin(GL_POINTS);
			glVertex3d(circle.Line.Point.X, circle.Line.Point.Y, circle.Line.Point.Z);
		glEnd();*/

	}

	glLineWidth(1);
	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cylinder
void CExampleView::DrawOpenGL_Cylinder()
{

	std::ifstream					file;
	CylinderApprox					cylinderApprox;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataCylinder.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		cylinderApprox.FindByPoints(&points[0], (int)points.size(), accuracy);


		glBegin(GL_TRIANGLES);
		int i;
		for (i = 2; i < cylinderApprox.Mesh.points.size(); i+=3)
		{
			glVertex3f(cylinderApprox.Mesh.points[i].X, cylinderApprox.Mesh.points[i].Y, cylinderApprox.Mesh.points[i].Z);
			glVertex3f(cylinderApprox.Mesh.points[i - 1].X, cylinderApprox.Mesh.points[i - 1].Y, cylinderApprox.Mesh.points[i - 1].Z);
			glVertex3f(cylinderApprox.Mesh.points[i - 2].X, cylinderApprox.Mesh.points[i - 2].Y, cylinderApprox.Mesh.points[i - 2].Z);
			
			glNormal3f(cylinderApprox.Mesh.vectorsNormal[i / 3].X, cylinderApprox.Mesh.vectorsNormal[i / 3].Y, cylinderApprox.Mesh.vectorsNormal[i / 3].Z);
		}
		glEnd();


		
		// Bottom center point
		/*glColor3d(1, 0, 0);
		glPointSize(5);
		glBegin(GL_POINTS);
			glVertex3d(cylinder.PointBottomSurface.X, cylinder.PointBottomSurface.Y, cylinder.PointBottomSurface.Z);
		glEnd();*/

		glColor3d(1, 0, 0);
		glBegin(GL_LINE_LOOP);
		for (i = 0; i < cylinderApprox.pointsTopCircleEdge_Copy.size(); i++)
		{
			glVertex3f(cylinderApprox.pointsTopCircleEdge_Copy[i].X, cylinderApprox.pointsTopCircleEdge_Copy[i].Y, cylinderApprox.pointsTopCircleEdge_Copy[i].Z);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		for (i = 0; i < cylinderApprox.pointsBottomCircleEdge_Copy.size(); i++)
		{
			glVertex3f(cylinderApprox.pointsBottomCircleEdge_Copy[i].X, cylinderApprox.pointsBottomCircleEdge_Copy[i].Y, cylinderApprox.pointsBottomCircleEdge_Copy[i].Z);
		}
		glEnd();
	}

	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Point
void CExampleView::DrawOpenGL_Point()
{

	std::ifstream					file;
	PointApprox						pointApprox;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataSphere.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		pointApprox.FindByPoints(&points[0], (int)points.size(), accuracy);

		glColor3d(1.0f, 0.05f, 0.01f);
		glPointSize(5);

		glBegin(GL_POINTS);
			glVertex3d(pointApprox.X + 20, pointApprox.Y + 20, pointApprox.Z + 20);
			glNormal3f(pointApprox.Line.Vector.X, pointApprox.Line.Vector.Y, pointApprox.Line.Vector.Z);
		glEnd();

	}

	glPointSize(1);
	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL LineSegment
void CExampleView::DrawOpenGL_LineSegment()
{
	std::ifstream					file;
	LineSegmentApprox				lineApprox;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataLine.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		lineApprox.FindByPoints(&points[0], (int)points.size(), accuracy);



		glColor3d(1, 0, 0);
		//glPointSize(5);
		glLineWidth(2);
		glBegin(GL_LINES);
			glVertex3d(lineApprox.PointStart.X, lineApprox.PointStart.Y, lineApprox.PointStart.Z);
			glVertex3d(lineApprox.PointEnd.X, lineApprox.PointEnd.Y, lineApprox.PointEnd.Z);
			glNormal3f(lineApprox.Vector.X, lineApprox.Vector.Y, lineApprox.Vector.Z);
		glEnd();
		
	}

	glLineWidth(1);
	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Plane (Rectangle)
void CExampleView::DrawOpenGL_PlaneViaRectangle()
{
	std::ifstream					file;
	RectangleApprox					planeApprox;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataPlane.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		planeApprox.FindByPoints(&points[0], (int)points.size(), accuracy);

		glBegin(GL_TRIANGLES);
		int i;
		for (i = 2; i < planeApprox.Mesh.points.size(); i += 3)
		{
			glVertex3f(planeApprox.Mesh.points[i].X,		planeApprox.Mesh.points[i].Y,		planeApprox.Mesh.points[i].Z);
			glVertex3f(planeApprox.Mesh.points[i - 1].X,	planeApprox.Mesh.points[i - 1].Y,	planeApprox.Mesh.points[i - 1].Z);
			glVertex3f(planeApprox.Mesh.points[i - 2].X,	planeApprox.Mesh.points[i - 2].Y,	planeApprox.Mesh.points[i - 2].Z);

			glNormal3f(planeApprox.Mesh.vectorsNormal[0].X, planeApprox.Mesh.vectorsNormal[0].Y, planeApprox.Mesh.vectorsNormal[0].Z);
		}
		glEnd();
	}

	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Cone
void CExampleView::DrawOpenGL_Cone()
{


	std::ifstream					file;
	std::vector <PointGeometric>	points;
	ConeApprox						coneApprox;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataCone.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		coneApprox.FindByPoints(&points[0], (int)points.size(), accuracy);


		file.close();
		points.clear();

		glColor3f(0.85f, 0.85f, 0.9f);				// текущий цвет примитивов

		glBegin(GL_TRIANGLES);
		int i;
		for (i = 2; i < coneApprox.Mesh.points.size(); i += 3)
		{
			glVertex3f(coneApprox.Mesh.points[i].X, coneApprox.Mesh.points[i].Y, coneApprox.Mesh.points[i].Z);
			glVertex3f(coneApprox.Mesh.points[i - 1].X, coneApprox.Mesh.points[i - 1].Y, coneApprox.Mesh.points[i - 1].Z);
			glVertex3f(coneApprox.Mesh.points[i - 2].X, coneApprox.Mesh.points[i - 2].Y, coneApprox.Mesh.points[i - 2].Z);

			glNormal3f(coneApprox.Mesh.vectorsNormal[i/3].X, coneApprox.Mesh.vectorsNormal[i / 3].Y, coneApprox.Mesh.vectorsNormal[i / 3].Z);
		}
		glEnd();
	


		//// Bottom center point
		//glColor3d(1, 0, 0);
		//glPointSize(5);
		//glBegin(GL_POINTS);
		//	glVertex3d(coneApprox.PointBottomSurface.X, coneApprox.PointBottomSurface.Y, coneApprox.PointBottomSurface.Z);
		//glEnd();

		glColor3d(1, 0, 0);
		glBegin(GL_LINE_LOOP);
		for (i = 0; i < coneApprox.pointsTopCircleEdge_Copy.size(); i++)
		{
			glVertex3f(coneApprox.pointsTopCircleEdge_Copy[i].X, coneApprox.pointsTopCircleEdge_Copy[i].Y, coneApprox.pointsTopCircleEdge_Copy[i].Z);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		for (i = 0; i < coneApprox.pointsBottomCircleEdge_Copy.size(); i++)
		{
			glVertex3f(coneApprox.pointsBottomCircleEdge_Copy[i].X, coneApprox.pointsBottomCircleEdge_Copy[i].Y, coneApprox.pointsBottomCircleEdge_Copy[i].Z);
		}
		glEnd();


	}
	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

//////////////////////////////////////////////////////////	---	---	---	---	---	---	---	---	---	// Draw OpenGL Sphere
void CExampleView::DrawOpenGL_Sphere()
{

	std::ifstream					file;
	SphereApprox					sphereApprox;
	std::vector <PointGeometric>	points;
	PointGeometric					point;
	double							accuracy = 0.00001;

	file.open("dataSphere.txt");
	if (file.is_open())
	{
		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		sphereApprox.FindByPoints(&points[0], (int)points.size(), accuracy);

		glColor3f(0.2f, 0.85f, 0.1f);
		glBegin(GL_TRIANGLES);
		int i;
		for (i = 2; i < sphereApprox.Mesh.points.size() /*&&i<=dd*/; i += 3)
		{
			//glColor3f((double)(i%10)/10, (double)(i % 10) /10, (double)(i % 10) /10);
			
			glVertex3f(sphereApprox.Mesh.points[i].X, sphereApprox.Mesh.points[i].Y, sphereApprox.Mesh.points[i].Z);
			glVertex3f(sphereApprox.Mesh.points[i - 1].X, sphereApprox.Mesh.points[i - 1].Y, sphereApprox.Mesh.points[i - 1].Z);
			glVertex3f(sphereApprox.Mesh.points[i - 2].X, sphereApprox.Mesh.points[i - 2].Y, sphereApprox.Mesh.points[i - 2].Z);

			glNormal3f(sphereApprox.Mesh.vectorsNormal[i / 3].X, sphereApprox.Mesh.vectorsNormal[i / 3].Y, sphereApprox.Mesh.vectorsNormal[i / 3].Z);
		}
		glEnd();

		/*glColor3d(1, 0, 0);
		glPointSize(3);
		glBegin(GL_POINTS);
			glVertex3d(circle.Line.Point.X, circle.Line.Point.Y, circle.Line.Point.Z);
		glEnd();*/

	}

	glColor3f(0.85f, 0.85f, 0.85f);				// текущий цвет примитивов

	file.close();
	points.clear();
}
///////////////////////////////////////////////////////

