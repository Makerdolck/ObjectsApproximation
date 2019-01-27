// try_OpenGL.h : main header file for the TRY_OPENGL application
//

#if !defined(AFX_TRY_OPENGL_H__0E81469C_B7BF_430B_B7DF_A8FEAB4D0E48__INCLUDED_)
#define AFX_TRY_OPENGL_H__0E81469C_B7BF_430B_B7DF_A8FEAB4D0E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLApp:
// See try_OpenGL.cpp for the implementation of this class
//

class CTry_OpenGLApp : public CWinApp
{
public:
	CTry_OpenGLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTry_OpenGLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTry_OpenGLApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRY_OPENGL_H__0E81469C_B7BF_430B_B7DF_A8FEAB4D0E48__INCLUDED_)
