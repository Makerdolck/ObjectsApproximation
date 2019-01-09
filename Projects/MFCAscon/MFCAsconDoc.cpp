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

// MFCAsconDoc.cpp : implementation of the CMFCAsconDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCAscon.h"
#endif

#include "MFCAsconDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCAsconDoc

IMPLEMENT_DYNCREATE(CMFCAsconDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCAsconDoc, CDocument)
END_MESSAGE_MAP()


// CMFCAsconDoc construction/destruction

CMFCAsconDoc::CMFCAsconDoc()
{
	//
}

CMFCAsconDoc::~CMFCAsconDoc()
{
	objectsArray.clear();
}

BOOL CMFCAsconDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMFCAsconDoc serialization

void CMFCAsconDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCAsconDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCAsconDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCAsconDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCAsconDoc diagnostics

#ifdef _DEBUG
void CMFCAsconDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCAsconDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCAsconDoc commands

////--------------------------------------------------------------
////	----	Thread		----	Scaning
////--------------------------------------------------------------
//
//UINT __cdecl Thread_Scanning(LPVOID pParam)
//{
//	CmmApprox*	m_pCMM	= (CmmApprox*)pParam;
//
//	short		stat	= 0;
//	CmmApprox	tmpCMM	= *m_pCMM;
//
//	PointApprox			*pointA;
//	LineSegmentApprox	*lineSegmentA;
//	RectangleApprox		*rectangleA;
//	CircleApprox		*circleA;
//	CylinderApprox		*cylinderA;
//	ConeApprox			*coneA;
//	SphereApprox		*sphereA;
//
//	
//	while (true)
//	{
//		if (!m_pCMM->flagScanning)
//		{
//			if (m_pCMM->PointsAmount == 0)
//			{
//				if (m_pCMM->PointsForApprox.size() > 0)
//				{
//					switch (m_pCMM->MeasureType)
//					{
//					case tmpCMM.MTContainer.point:						// point
//
//						pointA = new PointApprox();
//						*pointA = m_pCMM->PointCurrent;
//						m_pCMM->ObjectsForApprox.push_back(pointA);
//
//						break;
//					case tmpCMM.MTContainer.lineSegment:				// lineSegment
//
//						lineSegmentA = new LineSegmentApprox();
//						lineSegmentA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(lineSegmentA);
//
//						break;
//					case tmpCMM.MTContainer.rectangle:					// rectangle
//
//						rectangleA = new RectangleApprox();
//						rectangleA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(rectangleA);
//
//						break;
//					case tmpCMM.MTContainer.circle:						// circle
//
//						circleA = new CircleApprox();
//						circleA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(circleA);
//
//						break;
//					case tmpCMM.MTContainer.cylinder:					// cylinder
//
//						cylinderA = new CylinderApprox();
//						cylinderA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(cylinderA);
//
//						break;
//					case tmpCMM.MTContainer.cone:						// cone
//
//						coneA = new ConeApprox();
//						coneA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(coneA);
//
//						break;
//					case tmpCMM.MTContainer.sphere:						// sphere
//
//						sphereA = new SphereApprox();
//						sphereA->PointsForApprox = m_pCMM->PointsForApprox;
//						m_pCMM->ObjectsForApprox.push_back(sphereA);
//
//						break;
//					default:
//						break;
//					}
//
//					m_pCMM->PointsForApprox.clear();
//				}
//			}
//
//			Sleep(1);
//			continue;
//		}
//
//		(*m_pCMM->GetPoint)(&m_pCMM->PointCurrent.X, &m_pCMM->PointCurrent.Y, &m_pCMM->PointCurrent.Z, &stat);
//
//
//		if (stat == 0)
//		{
//			Sleep(1);
//			continue;
//		}
//
//
//		m_pCMM->PointsForApprox.push_back(m_pCMM->PointCurrent);		// Add point to array
//
//		if (m_pCMM->PointsForApprox.size() >= m_pCMM->PointsAmount && m_pCMM->PointsAmount != 0)
//		{
//			switch (m_pCMM->MeasureType)
//			{
//			case tmpCMM.MTContainer.point:						// point
//
//				pointA = new PointApprox();
//				*pointA = m_pCMM->PointCurrent;
//				m_pCMM->ObjectsForApprox.push_back(pointA);
//
//				break;
//			case tmpCMM.MTContainer.lineSegment:				// lineSegment
//
//				lineSegmentA = new LineSegmentApprox();
//				lineSegmentA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(lineSegmentA);
//								
//				break;
//			case tmpCMM.MTContainer.rectangle:					// rectangle
//				
//				rectangleA = new RectangleApprox();
//				rectangleA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(rectangleA);
//
//				break;
//			case tmpCMM.MTContainer.circle:						// circle
//				
//				circleA = new CircleApprox();
//				circleA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(circleA);
//
//				break;
//			case tmpCMM.MTContainer.cylinder:					// cylinder
//				
//				cylinderA = new CylinderApprox();
//				cylinderA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(cylinderA);
//
//				break;
//			case tmpCMM.MTContainer.cone:						// cone
//				
//				coneA = new ConeApprox();
//				coneA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(coneA);
//
//				break;
//			case tmpCMM.MTContainer.sphere:						// sphere
//				
//				sphereA = new SphereApprox();
//				sphereA->PointsForApprox = m_pCMM->PointsForApprox;
//				m_pCMM->ObjectsForApprox.push_back(sphereA);
//
//				break;
//			default:
//				break;
//			}
//
//			m_pCMM->PointsForApprox.clear();
//		}
//
//		
//
//		Sleep(1);
//	}
//
//	return 0;
//}