#include <windows.h>
#include "MyOpenGL.h"
////////////////////////////////////////////////////////////////
const GLdouble vertex[][3]={
	{0.0,0.0,0.0},
	{1.0,0.0,0.0},
	{1.0,1.0,0.0},
	{0.0,1.0,0.0},
	{0.0,0.0,1.0},
	{1.0,0.0,1.0},
	{1.0,1.0,1.0},
	{0.0,1.0,1.0}
};

const int face[][4]={
	{0,1,2,3},
	{1,5,6,2},
	{5,4,7,6},
	{4,0,3,7},
	{4,5,1,0},
	{3,2,6,7}
};

const GLfloat color[][3]={
	{1.0f,0.0f,0.0f},
	{0.0f,1.0f,0.0f},
	{0.0f,0.0f,1.0f},
	{1.0f,1.0f,0.0f},
	{1.0f,0.0f,1.0f},
	{0.0f,1.0f,1.0f}
};

const GLdouble normal[][3]={
	{0.0,0.0,-1.0},
	{1.0,0.0,0.0},
	{0.0,0.0,1.0},
	{-1.0,0.0,0.0},
	{0.0,-1.0,0.0},
	{0.0,1.0,0.0}
};

const GLfloat red[]={ 1.0,0.0,0.0,1.0};
const GLfloat blue[]={ 0.0,0.0,1.0,1.0};

////////////////////////////////////////////////////////////////
HGLRC SetUpOpenGL(HWND hWnd)
{    
	static PIXELFORMATDESCRIPTOR pfd = {
        sizeof (PIXELFORMATDESCRIPTOR), // struct size 
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
	
    hDC = ::GetDC( hWnd );
    nMyPixelFormatID = ChoosePixelFormat( hDC, &pfd );

    if (!nMyPixelFormatID)
	{
		MessageBox(hWnd,"May be you do not install OpenGL!","Error!",MB_ICONERROR|MB_OK);
		exit(1);
	}
    if (!SetPixelFormat( hDC, nMyPixelFormatID, &pfd ))
	{
		MessageBox(hWnd,"Can not set pixel format for OpenGL!","Error!",MB_ICONERROR|MB_OK);
		exit(1);
	}

	glCullFace(GL_BACK);

    hRC = wglCreateContext( hDC );
	::ReleaseDC( hWnd, hDC );

    return hRC;
}
////////////////////////////////////////////////////////////////
Cube::Cube()
{
	x=y=z=0;
	selected=false;
}

void Cube::Draw()
{
	int i,j;
	
	glPushMatrix();
	glTranslated(x,y,z);
	if (selected) glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,red);
	else glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,blue);
	
	glBegin(GL_QUADS);
	for(i=0;i<6;i++){
		glNormal3dv(normal[i]);
		for(j=0;j<4;j++){
			glVertex3dv(vertex[face[i][j]]);
		}
	}
	glEnd();
	glPopMatrix();
}
////////////////////////////////////////////////////////////////
