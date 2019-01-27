// try_OpenGLDoc.h : interface of the CTry_OpenGLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRY_OPENGLDOC_H__80F4EF0D_B2B8_4C6C_B553_316A5A4CC3C9__INCLUDED_)
#define AFX_TRY_OPENGLDOC_H__80F4EF0D_B2B8_4C6C_B553_316A5A4CC3C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTry_OpenGLDoc : public CDocument
{
protected: // create from serialization only
	CTry_OpenGLDoc();
	DECLARE_DYNCREATE(CTry_OpenGLDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTry_OpenGLDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTry_OpenGLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTry_OpenGLDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRY_OPENGLDOC_H__80F4EF0D_B2B8_4C6C_B553_316A5A4CC3C9__INCLUDED_)
