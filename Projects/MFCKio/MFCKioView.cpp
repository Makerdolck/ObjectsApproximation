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

// MFCKioView.cpp : implementation of the CMFCKioView class
//


#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCKio.h"
#endif

#include "MFCKioDoc.h"
#include "MFCKioView.h"

#include "DialogObjectProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCKioView

IMPLEMENT_DYNCREATE(CMFCKioView, COpenGLView)

BEGIN_MESSAGE_MAP(CMFCKioView, COpenGLView)
	ON_MESSAGE(WM_MATH_OBJECT_READY,	&CMFCKioView::OnNewObjectForVisualize)
	ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_AUTOMEASURING, &CMFCKioView::OnEditAutomeasuring)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCKioView construction/destruction

CMFCKioView::CMFCKioView()
{
	NameNumb_Point			= 0;
	NameNumb_LineSegment	= 0;
	NameNumb_Plane			= 0;
	NameNumb_Circle			= 0;
	NameNumb_Cylinder		= 0;
	NameNumb_Cone			= 0;
	NameNumb_Sphere			= 0;

	radiusOfAllObjects		= 1;

	pDataTree				= nullptr;
	dlgMeasure = nullptr;
}

CMFCKioView::~CMFCKioView()
{
	pDataTree = nullptr;
}

void CMFCKioView::OnInitialUpdate()
{
	COpenGLView::OnInitialUpdate();

	objectsArray = &((CMFCKioDoc*)GetDocument())->objectsArray;
}


// CMFCKioView drawing


void CMFCKioView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	if (dlgMeasure->objAp != nullptr)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}	
#endif
}


// CMFCKioView diagnostics

#ifdef _DEBUG
void CMFCKioView::AssertValid() const
{
	COpenGLView::AssertValid();
}

void CMFCKioView::Dump(CDumpContext& dc) const
{
	COpenGLView::Dump(dc);
}

CMFCKioDoc* CMFCKioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCKioDoc)));
	return (CMFCKioDoc*)m_pDocument;
}
#endif //_DEBUG



//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																										// Left Button DBL Click
void CMFCKioView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ObjectApprox *object = GetObjectUnderMouse(point);

	if (object == nullptr)
	{
		COpenGLView::OnLButtonDblClk(nFlags, point);
		return;
	}

	object->flagSelected = false;
	Invalidate(FALSE);

	DialogObjectProperties dlg(this, object);
	dlg.DoModal();

	COpenGLView::OnLButtonDblClk(nFlags, point);
}
// ---																										// Remove Scene Segment
bool CMFCKioView::RemoveObject(ObjectApprox *object)
{
	bool flag = false;

	if (object != nullptr)
	{
		try
		{
			HTREEITEM groop = pDataTree->pCtrl->GetRootItem();
			HTREEITEM hChildItem;
			ObjectApprox* obj = nullptr;
			bool flagHasDeleted = false;

			while (groop != NULL && !flagHasDeleted)
			{
				hChildItem = pDataTree->pCtrl->GetChildItem(groop);
				while (hChildItem != NULL)
				{
					obj = (ObjectApprox*)pDataTree->pCtrl->GetItemData(hChildItem);
					if (obj->objID == object->objID)
					{
						pDataTree->pCtrl->DeleteItem(hChildItem);
						flagHasDeleted = true;
						break;
					}
					hChildItem = pDataTree->pCtrl->GetNextItem(hChildItem, TVGN_NEXT);
				}
				groop = pDataTree->pCtrl->GetNextItem(groop, TVGN_NEXT);
			}
			
			std::vector<ObjectApprox*>::iterator i = std::find(objectsArray->begin(), objectsArray->end(), object);
			objectsArray->erase(i);
			delete object;

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
LRESULT CMFCKioView::OnNewObjectForVisualize(WPARAM wParam, LPARAM lParam)
{
	ObjectApprox			*object;

	//if (objectsArray->back()->flagReady == true)
	//	object	= objectsArray->back();
	//else if(objectsArray->size() > 1)
	//	object  = objectsArray->operator[](objectsArray->size() - 2);

	object					= objectsArray->back();

	GeomObjectApprox		*objMath	= object->objMath;	

	char					*objectName		= objMath->GetName();
		
	//	---													// point
	if (objectName == PointApprox().GetName())
	{
		
		//pointA = (PointApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Point++;
	}
	//	---													// lineSegment
	if (objectName == LineSegmentApprox().GetName())
	{
		
		//lineSegmentA = (LineSegmentApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_LineSegment++;
	}
	//	---													// rectangle
	if (objectName == RectangleApprox().GetName())
	{
		
		//rectangleA = (RectangleApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Plane++;
	}
	//	---													// circle
	if (objectName == CircleApprox().GetName())
	{
		
		//circleA = (CircleApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Circle++;
	}
	//	---													// cylinder
	if (objectName == CylinderApprox().GetName())
	{
		
		//cylinderA = (CylinderApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Cylinder++;
	}
	//	---													// cone
	if (objectName == ConeApprox().GetName())
	{
		
		//coneA = (ConeApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Cone++;
	}
	//	---													// sphere
	if (objectName == SphereApprox().GetName())
	{
		
		//sphereA = (SphereApprox*)objMath;

		//	---	---	--- Naming
		NameNumb_Sphere++;
	}

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	RecalculateCenterOfAllObjects();

	FocusingOn();

	Invalidate(FALSE);
	UpdateWindow();

	return 0L;
}
// ---																							// Recalculate center of all objects
void CMFCKioView::RecalculateCenterOfAllObjects()
{
	double	x = 0, 
			y = 0, 
			z = 0;

	int arraySize = (int)objectsArray->size();

	GeomObjectApprox* obj = nullptr;

	for (int i = 0; i < arraySize; i++)
	{
		obj = objectsArray->operator[](i)->objMath;

		//	--- Center

		if (obj->GetName() == PointApprox().GetName())
		{
			x += ((PointApprox*)obj)->X;
			y += ((PointApprox*)obj)->Y;
			z += ((PointApprox*)obj)->Z;
		}
		else if (obj->GetName() == LineSegmentApprox().GetName())
		{
			x += ((LineSegmentApprox*)obj)->Point.X;
			y += ((LineSegmentApprox*)obj)->Point.Y;
			z += ((LineSegmentApprox*)obj)->Point.Z;
		}
		else
		{
			x += obj->Line.Point.X;
			y += obj->Line.Point.Y;
			z += obj->Line.Point.Z;
		}
		
	}

	radiusOfAllObjects = 1;

	if (arraySize != 0)
	{
		centerOfAllObjects.X = x / arraySize;
		centerOfAllObjects.Y = y / arraySize;
		centerOfAllObjects.Z = z / arraySize;

		for (int i = 0; i < arraySize; i++)
		{
			obj = objectsArray->operator[](i)->objMath;

			//	--- Radius

			double distance = 0;

			if (obj->GetName() == PointApprox().GetName())
			{
				distance = centerOfAllObjects.DistanceToPoint(*(PointApprox*)obj);
			}
			else if (obj->GetName() == LineSegmentApprox().GetName())
			{
				distance =	centerOfAllObjects.DistanceToPoint(((LineSegmentApprox*)obj)->Point) +
							((LineSegmentApprox*)obj)->Point.DistanceToPoint(((LineSegmentApprox*)obj)->PointEnd);
			}
			else if (obj->GetName() == RectangleApprox().GetName())
			{
				RectangleApprox* plane = (RectangleApprox*)obj;

				double	xParam = pow(plane->maxX - plane->minX, 2),
						yParam = pow(plane->maxY - plane->minY, 2);

				distance =	centerOfAllObjects.DistanceToPoint(obj->Line.Point) +
							sqrt(xParam + yParam) / 2;
			}
			else if (obj->GetName() == CircleApprox().GetName() ||
					 obj->GetName() == SphereApprox().GetName())
			{
				distance =	centerOfAllObjects.DistanceToPoint(obj->Line.Point) +
							obj->Radius;
			}
			else if (obj->GetName() == CylinderApprox().GetName() ||
					 obj->GetName() == ConeApprox().GetName())
			{
				double	xParam = pow(obj->Radius * 2, 2),
						yParam = pow(obj->Height, 2);

				distance =	centerOfAllObjects.DistanceToPoint(obj->Line.Point) +
							sqrt(xParam + yParam) / 2;
			}

			if (radiusOfAllObjects < distance)
				radiusOfAllObjects = distance;
		}
	}
	else
	{
		radiusOfAllObjects	= 1;
		centerOfAllObjects	= PointGeometric();
	}
}
// ---																							// Focusing on object OR scene center
void CMFCKioView::FocusingOn(ObjectApprox* obj)
{
	if (obj == nullptr)
	{
		double distanceOffset = pointAimLook.DistanceToPoint(centerOfAllObjects);

		LineGeometric lineOffset(pointEyeLook, VectorGeometric(pointAimLook, centerOfAllObjects));

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceOffset);
		pointAimLook = centerOfAllObjects;

		lineOffset = LineGeometric(pointAimLook, VectorGeometric(pointAimLook, pointEyeLook));

		distanceOffset = radiusOfAllObjects / tan(15.f * PI_Approx / 180.f);

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceOffset);


		distanceAimEye = (float)pointAimLook.DistanceToPoint(pointEyeLook);
	}
	else
	{
		GeomObjectApprox* object = obj->objMath;

		PointGeometric centerOfObject;

		if (object->GetName() == PointApprox().GetName())
			centerOfObject = PointGeometric(
									((PointApprox*)object)->X,
									((PointApprox*)object)->Y,
									((PointApprox*)object)->Z);
		else if (object->GetName() == LineSegmentApprox().GetName())
			centerOfObject = PointGeometric(
									((LineSegmentApprox*)object)->Point.X,
									((LineSegmentApprox*)object)->Point.Y,
									((LineSegmentApprox*)object)->Point.Z);
		else
			centerOfObject = object->Line.Point;

		double distanceOffset = pointAimLook.DistanceToPoint(centerOfObject);

		LineGeometric lineOffset(pointEyeLook, VectorGeometric(pointAimLook, centerOfObject));

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceOffset);
		pointAimLook = centerOfObject;

		lineOffset = LineGeometric(pointAimLook, VectorGeometric(pointAimLook, pointEyeLook));

		

		double distance = 1;

		if (object->GetName() == PointApprox().GetName())
		{
			distance = 1;
		}
		else if (object->GetName() == LineSegmentApprox().GetName())
		{
			distance = ((LineSegmentApprox*)object)->Point.DistanceToPoint(((LineSegmentApprox*)object)->PointEnd);
		}
		else if (object->GetName() == RectangleApprox().GetName())
		{
			RectangleApprox* plane = (RectangleApprox*)object;

			double	xParam = pow(plane->maxX - plane->minX, 2),
					yParam = pow(plane->maxY - plane->minY, 2);

			distance = sqrt(xParam + yParam) / 2;
		}
		else if (object->GetName() == CircleApprox().GetName() ||
				 object->GetName() == SphereApprox().GetName())
		{
			distance = object->Radius;
		}
		else if (object->GetName() == CylinderApprox().GetName() ||
				 object->GetName() == ConeApprox().GetName())
		{
			double	xParam = pow(object->Radius * 2, 2),
					yParam = pow(object->Height, 2);

			distance = sqrt(xParam + yParam) / 2;
		}

		distanceOffset = distance / tan(15.f * PI_Approx / 180.f);

		pointEyeLook = lineOffset.CreatePointOnDistance(distanceOffset);

		distanceAimEye = (float)pointAimLook.DistanceToPoint(pointEyeLook);
	}

	Invalidate(FALSE);
}



void CMFCKioView::OnEditAutomeasuring()
{
	if (dlgMeasure->objAp == nullptr)
	{
		return;
	}
	// TODO: добавьте свой код обработчика команд
	dlgMeasure->bAuto = true;
	//dlgMeasure->OnTraekt();
	//GetObjectUnderMouse(GetCursorPos);
	
	dlgMeasure->DoModal();
}


void CMFCKioView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	dlgMeasure->objAp = GetObjectUnderMouse(point);
	COpenGLView::OnRButtonDown(nFlags, point);
}
