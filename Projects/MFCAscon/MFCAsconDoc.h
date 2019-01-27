// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MFCAsconDoc.h : interface of the CMFCAsconDoc class
//


#pragma once


class CMFCAsconDoc : public CDocument
{
//	--- --- --- --- --- --- --- --- ---	// Properties
public:
	std::vector<ObjectApprox*>			objectsArray;

//	--- --- --- --- --- --- --- --- ---	// Methods
protected: // create from serialization only
	CMFCAsconDoc();
	DECLARE_DYNCREATE(CMFCAsconDoc)

// Attributes
public:
	//CWinThread			*pThread;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMFCAsconDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

// ---														// Scanning
//UINT __cdecl Thread_Scanning(LPVOID pParam);