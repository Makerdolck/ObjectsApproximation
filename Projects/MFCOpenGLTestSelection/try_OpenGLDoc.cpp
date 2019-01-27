// try_OpenGLDoc.cpp : implementation of the CTry_OpenGLDoc class
//

#include "stdafx.h"
#include "try_OpenGL.h"

#include "try_OpenGLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLDoc

IMPLEMENT_DYNCREATE(CTry_OpenGLDoc, CDocument)

BEGIN_MESSAGE_MAP(CTry_OpenGLDoc, CDocument)
	//{{AFX_MSG_MAP(CTry_OpenGLDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLDoc construction/destruction

CTry_OpenGLDoc::CTry_OpenGLDoc()
{
	// TODO: add one-time construction code here

}

CTry_OpenGLDoc::~CTry_OpenGLDoc()
{
}

BOOL CTry_OpenGLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLDoc serialization

void CTry_OpenGLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLDoc diagnostics

#ifdef _DEBUG
void CTry_OpenGLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTry_OpenGLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTry_OpenGLDoc commands
