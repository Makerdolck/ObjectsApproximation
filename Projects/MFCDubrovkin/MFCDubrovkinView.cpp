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

// MFCDubrovkinView.cpp : implementation of the CMFCDubrovkinView class
//


#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCDubrovkin.h"
#endif

#include "MFCDubrovkinDoc.h"
#include "MFCDubrovkinView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDubrovkinView

IMPLEMENT_DYNCREATE(CMFCDubrovkinView, COpenGLView)

BEGIN_MESSAGE_MAP(CMFCDubrovkinView, COpenGLView)
	ON_MESSAGE(WM_MATH_OBJECT_READY,	&CMFCDubrovkinView::OnNewObjectForVisualize)

	ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CMFCDubrovkinView construction/destruction

CMFCDubrovkinView::CMFCDubrovkinView()
{
	NameNumb_Point			= 0;
	NameNumb_LineSegment	= 0;
	NameNumb_Plane			= 0;
	NameNumb_Circle			= 0;
	NameNumb_Cylinder		= 0;
	NameNumb_Cone			= 0;
	NameNumb_Sphere			= 0;
}

CMFCDubrovkinView::~CMFCDubrovkinView()
{
	pDataTree = nullptr;
}

void CMFCDubrovkinView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	objectsArray = &((CMFCDubrovkinDoc*)GetDocument())->objectsArray;
}


// CMFCDubrovkinView drawing


void CMFCDubrovkinView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCDubrovkinView diagnostics

#ifdef _DEBUG
void CMFCDubrovkinView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CMFCDubrovkinView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

CMFCDubrovkinDoc* CMFCDubrovkinView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDubrovkinDoc)));
	return (CMFCDubrovkinDoc*)m_pDocument;
}
#endif //_DEBUG



//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																										// Left Button DBL Click
void CMFCDubrovkinView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//SceneSegment* pSceneSegment = GetSceneContent()->GetRootSegment();

	//SceneSegment* pZXSegment = new SceneSegment(new GeometryRep3D(new GeomRectangleObject(10, 10)));
	//SceneSegment* pXYSegment = new SceneSegment(new GeometryRep3D(new GeomRectangleObject(10, 12)));
	//SceneSegment* pYZSegment = new SceneSegment(new GeometryRep3D(new GeomRectangleObject(10, 10)));


	//MbMatrix3D marta(MbPlacement3D(MbCartPoint3D(0, 0, 0)));

	//marta.Rotate(AXIS_0X, PI_Approx / 2);

	//pZXSegment->CreateRelativeMatrix(marta);

	//pSceneSegment->AddSegment(pXYSegment);
	//pSceneSegment->AddSegment(pZXSegment);

	//marta.Rotate(AXIS_0X, -PI_Approx / 2);
	//marta.Rotate(AXIS_0Y, PI_Approx / 2);
	//pYZSegment->CreateRelativeMatrix(marta);

	//pSceneSegment->AddSegment(pYZSegment);

	//---
	//SceneSegment* pPlaneSegment1 = new SceneSegment(new GeometryRep3D(new GeomLineSegmentObject(
	//	MbCartPoint3D(0, 0, 0), MbCartPoint3D(10, 10, 10))));

	//pSceneSegment->AddSegment(pPlaneSegment1);

	//---
	/*int k = 5;
	pPlaneSegment1 = new SceneSegment(new GeometryRep3D(new GeomBoxObject(k,k,k)));

	pSceneSegment->AddSegment(pPlaneSegment1);

	ObjectApprox *obj = new ObjectApprox();

	obj->Name = L"DDR";
	obj->objSS = pPlaneSegment1;

	PointApprox *pointR = new PointApprox();
	pointR->X = 5;
	pointR->Y = 9;
	pointR->Z = 5;

	obj->objMath = pointR;
	
	objectsArray->push_back(*obj);


	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);*/

	Invalidate();
	UpdateWindow();

	COpenGLView::OnLButtonDblClk(nFlags, point);
}
// ---																										// Remove Scene Segment
bool CMFCDubrovkinView::RemoveObject(ObjectApprox *object /*std::wstring objectName*/)
{
	//SceneSegment* pRootSS = GetSceneContent()->GetRootSegment();

	/*std::list <C3DVision::SceneSegment*> sList = pRootSS->GetSubSegmets();

	for (SceneSegment* pSElem : sList)
	{
		if (objectName == pSElem->GetName())
		{
			pRootSS->RemoveSegment(pSElem);
			delete pSElem;
			Invalidate();
			return true;
		}
	}*/	

	bool flag = false;

	if (object != nullptr)
	{
		try
		{
			std::vector<ObjectApprox*>::iterator i = std::find(objectsArray->begin(), objectsArray->end(), object);
			objectsArray->erase(i);

			flag = true;
		}
		catch(...)
		{

		}
		
		RecalculateCenterOfAllObjects();

		Invalidate(FALSE);
		UpdateWindow();
		return flag;
	}


	return false;
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

// ---																							// On New Object for Visualize is coming
LRESULT CMFCDubrovkinView::OnNewObjectForVisualize(WPARAM wParam, LPARAM lParam)
{
	ObjectApprox			*object;

	//if (objectsArray->back()->flagReady == true)
	//	object	= objectsArray->back();
	//else if(objectsArray->size() > 1)
	//	object  = objectsArray->operator[](objectsArray->size() - 2);

	object					= objectsArray->back();

	GeomObjectApprox		*objMath	= object->objMath;	

	PointApprox				*pointA			= new PointApprox();
	LineSegmentApprox		*lineSegmentA	= new LineSegmentApprox();
	RectangleApprox			*rectangleA		= new RectangleApprox();
	CircleApprox			*circleA		= new CircleApprox();
	CylinderApprox			*cylinderA		= new CylinderApprox();
	ConeApprox				*coneA			= new ConeApprox();
	SphereApprox			*sphereA		= new SphereApprox();

	char					*objectName		= objMath->GetName();
		
	//	---													// point
	if (objectName == pointA->GetName())
	{
		
		//pointA = (PointApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Point++;
	}
	//	---													// lineSegment
	if (objectName == lineSegmentA->GetName())
	{
		
		//lineSegmentA = (LineSegmentApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_LineSegment++;
	}
	//	---													// rectangle
	if (objectName == rectangleA->GetName())
	{
		
		//rectangleA = (RectangleApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Plane++;
	}
	//	---													// circle
	if (objectName == circleA->GetName())
	{
		
		//circleA = (CircleApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Circle++;
	}
	//	---													// cylinder
	if (objectName == cylinderA->GetName())
	{
		
		//cylinderA = (CylinderApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Cylinder++;
	}
	//	---													// cone
	if (objectName == coneA->GetName())
	{
		
		//coneA = (ConeApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Cone++;
	}
	//	---													// sphere
	if (objectName == sphereA->GetName())
	{
		
		//sphereA = (SphereApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Sphere++;
	}

	delete pointA;
	delete lineSegmentA;
	delete rectangleA;
	delete circleA;
	delete cylinderA;
	delete coneA;
	delete sphereA;
	


	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	RecalculateCenterOfAllObjects();

	FocusedOn();


	Invalidate(FALSE);
	UpdateWindow();

	return 0L;
}
// ---																							// Recalculate center of all objects
void CMFCDubrovkinView::RecalculateCenterOfAllObjects()
{
	double	x = 0, 
			y = 0, 
			z = 0;

	int arraySize = (int)objectsArray->size();

	for (int i = 0; i < arraySize; i++)
	{
		x += objectsArray->operator[](i)->objMath->Line.Point.X;
		y += objectsArray->operator[](i)->objMath->Line.Point.Y;
		z += objectsArray->operator[](i)->objMath->Line.Point.Z;
	}

	if (arraySize != 0)
	{
		centerOfAllObjects.X = x / arraySize;
		centerOfAllObjects.Y = y / arraySize;
		centerOfAllObjects.Z = z / arraySize;
	}
	else
		centerOfAllObjects = PointGeometric();
}
// ---																							// Focused on object OR scene center
void CMFCDubrovkinView::FocusedOn(ObjectApprox* obj)
{

	if (obj == nullptr || obj == NULL)
	{
		pointAimLook.X = centerOfAllObjects.X;
		pointAimLook.Y = centerOfAllObjects.Y;
		pointAimLook.Z = centerOfAllObjects.Z;

		pointEyeLook.X = centerOfAllObjects.X;
		pointEyeLook.Y = centerOfAllObjects.Y;
		pointEyeLook.Z = centerOfAllObjects.Z + 300;

		distanceAimEye = pointAimLook.DistanceToPoint(pointEyeLook);
		offsetView_X = 0;
		offsetView_Y = 0;
	}
	else
	{
		pointAimLook.X = obj->objMath->Line.Point.X;
		pointAimLook.Y = obj->objMath->Line.Point.Y;
		pointAimLook.Z = obj->objMath->Line.Point.Z;

		pointEyeLook.X = obj->objMath->Line.Point.X;
		pointEyeLook.Y = obj->objMath->Line.Point.Y;
		pointEyeLook.Z = obj->objMath->Line.Point.Z + 300;

		distanceAimEye = pointAimLook.DistanceToPoint(pointEyeLook);
		offsetView_X = 0;
		offsetView_Y = 0;
	}

	Invalidate(FALSE);
}