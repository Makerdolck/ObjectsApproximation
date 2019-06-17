#include "stdafx.h"
#include "Intersection.h"


Intersection::Intersection(DataTreeView* dataTree, CMFCKioView* view, long int* id_pointer)
{
	pDataTree		= dataTree;
	pView			= view;
	nextObjectID	= id_pointer;
}


Intersection::~Intersection()
{
}


// ---																										// Point

// ---																										// PointIntersectionPoint
int Intersection::InTwoPoint(std::vector<ObjectApprox*> *objectsArray, int variant)
{
	PointApprox			point1;
	PointApprox			point2;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	PointApprox			*pointA = new PointApprox();

	bool Selection1 = true;
	bool Selection2 = true;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == pointA->GetName() && Selection1 == true) {
				point1 = *(PointApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point2 = *(PointApprox*)objApprox->objMath;
				Selection2 = false;
			}
		}
	}

	if (Selection1 == true || Selection2 == true)
	{
		delete pointA;
		return 1;
	}

	PointGeometric		point3, point4;

	if (variant == 1) {
		point3 = point1.PointBetweenPointMiddle(point2);

		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point3;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_twoPoints";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	if (variant == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		LineSegmentApprox	*lineRes = new LineSegmentApprox();
		PointGeometric  pointST(point1.X, point1.Y, point1.Z);
		PointGeometric  pointEnd(point2.X, point2.Y, point2.Z);

		lineRes->PointStart = pointST;
		lineRes->PointEnd = pointEnd;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)lineRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Line_twoPoints";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	delete pointA;
	return (0);
}
// ---																										// PointIntersectionLine
int Intersection::InLinePoint(std::vector<ObjectApprox*> *objectsArray)
{
	LineSegmentApprox	line;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineA = new LineSegmentApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((line.PointStart == line.PointEnd) || Selection == true)
	{
		delete lineA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = line.PointBetweenLine(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	//	---	---	---	---
	objectsArray->back()->Name = L"Point_LinePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---

	delete lineA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionCircle
int Intersection::InCirclePoint(std::vector<ObjectApprox*> *objectsArray)
{
	CircleApprox		circle;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CircleApprox		*circleA = new CircleApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName())
				circle = *(CircleApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((circle.Radius == 0) || Selection == true)
	{
		delete circleA;
		delete pointA;
		return 1;
	}
	PointGeometric point1 = circle.PointIntersection(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"Point_CirclePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete circleA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionPlane 
int Intersection::InPlanePoint(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox		plane;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;

	RectangleApprox		*planeA = new RectangleApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection1 = true;
	bool Selection2 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
 				Selection2 = false;
			}
		}
	}

	if (Selection1 == true || Selection2 == true)
	{
		delete planeA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = plane.PointBetweenPlane(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"Point_PlanePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete planeA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionSphere
int Intersection::InSpherePoint(std::vector<ObjectApprox*> *objectsArray) {
	SphereApprox		sphere;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	SphereApprox		*sphereA = new SphereApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == sphereA->GetName())
				sphere = *(SphereApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((sphere.Radius == 0) || Selection == true)
	{
		delete sphereA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = sphere.IntersectionSphereAndPoint(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"Point_SpherePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete sphereA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionCylinder
int Intersection::InCylinderPoint(std::vector<ObjectApprox*> *objectsArray) {
	CylinderApprox		cylinder;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CylinderApprox		*cylinderA = new CylinderApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((cylinder.Radius == 0) || Selection == true)
	{
		delete cylinderA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = cylinder.IntersectionCylinderAndPoint(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"Point_CylinderPoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete cylinderA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionCone
int Intersection::InConePoint(std::vector<ObjectApprox*> *objectsArray) {
	ConeApprox			cone;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	ConeApprox			*coneA = new ConeApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone = *(ConeApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((cone.Radius == 0) || Selection == true)
	{
		delete coneA;
		delete pointA;
		return 1;
	}

	PointGeometric point1;
	int colv;
	colv = cone.IntersectionConeAndPoint(point, &point1);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_ConePoint";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return 1;
	}
	delete coneA;
	delete pointA;
	return (0);
}
// ---																										// PointMiddle

// ---																										// PointIntersectionLine
int Intersection::InLinePointMiddle(std::vector<ObjectApprox*> *objectsArray)
{
	LineSegmentApprox	line;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;

	LineSegmentApprox	*lineA = new LineSegmentApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((line.PointStart == line.PointEnd) || Selection == true)
	{
		delete lineA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = line.PointBetweenLineMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"MPoint_LinePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete lineA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionCircle
int Intersection::InCirclePointMiddle(std::vector<ObjectApprox*> *objectsArray)
{
	CircleApprox		circle;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CircleApprox		*circleA = new CircleApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName())
				circle = *(CircleApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}
	}

	if ((circle.Radius == 0) || Selection == true)
	{
		delete circleA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = circle.PointIntersectionMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"MPoint_CirclePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete circleA;
	delete pointA;
	return (0);
}
// ---																										// PointIntersectionPlane 
int Intersection::InPlanePointMiddle(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox		plane;
	PointApprox			point;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();
	PointApprox			*pointA = new PointApprox();

	bool Selection1 = true;
	bool Selection2 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection2 = false;
			}
		}
	}

	if (Selection1 == true || Selection2 == true)
	{
		delete planeA;
		delete pointA;
		return 1;
	}

	PointGeometric point1 = plane.PointBetweenPlaneMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"MPoint_PlanePoint";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---
	delete planeA;
	delete pointA;
	return (0);
}


// ---																										// Line

// ---																										// LineIntersectionLine
int Intersection::InTwoLine(std::vector<ObjectApprox*> *objectsArray) {
	LineSegmentApprox	line;
	LineSegmentApprox	line2;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox		*lineA = new LineSegmentApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName() && (line.PointStart == line.PointEnd)) {
				line = *(LineSegmentApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == lineA->GetName())
				line2 = *(LineSegmentApprox*)objApprox->objMath;
		}
	}

	if ((line.PointStart == line.PointEnd) || (line2.PointStart == line2.PointEnd))
	{
		delete lineA;
		return 1;
	}

	PointGeometric		pointout;
	LineGeometric       Line2;
	Line2.Point = line2.Point;
	Line2.Vector = line2.Vector;

	int colv;
	colv = line.LineBetweenLine(Line2, &pointout);

	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = pointout;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_InTwoLine";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = pointout;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_IntercrossLine";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	else
	{
		return (1);
	}

	delete lineA;
	return (0);
}
// ---																										// LineIntersectionPlane 
int Intersection::InPlaneLine(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox		plane;
	LineSegmentApprox	line;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineSegmentA = new LineSegmentApprox();
	RectangleApprox		*planeA = new RectangleApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == lineSegmentA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
		}
	}

	if ((line.PointStart == line.PointEnd)|| Selection1 == true) 
	{
		delete lineSegmentA;
		delete planeA;
		return 1;
	}
	PointGeometric		point1;
	LineGeometric       line1;
	line1.Point = line.Point;
	line1.Vector = line.Vector;

	int colv;
	colv = plane.PlaneBetweenLine(line1, &point1);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_LinePlane";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return (1);
	}

	delete lineSegmentA;
	delete planeA;
	return (0);
}
// ---																										// LineIntersectionCircle
int Intersection::InCircleLine(std::vector<ObjectApprox*> *objectsArray)
{
	CircleApprox		circle;
	LineSegmentApprox	line;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineSegmentA = new LineSegmentApprox();
	CircleApprox		*circleA = new CircleApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName())
				circle = *(CircleApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == lineSegmentA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
		}
	}

	if ((circle.Radius == 0) || (line.PointStart == line.PointEnd))
	{
		delete lineSegmentA;
		delete circleA;
		return 1;
	}

	PointGeometric		point1, point2;
	LineGeometric       line1;
	line1.Point = line.Point;
	line1.Vector = line.Vector;

	int colv;
	colv = circle.LineIntersectionCircle(line1, &point1, &point2);

	if (colv == 2)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;	
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_LineCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else
	{
		return (1);
	}

	delete lineSegmentA;
	delete circleA;
	return (0);
}
// ---																										// LineIntersectionSphere  
int Intersection::InSphereLine(std::vector<ObjectApprox*> *objectsArray)
{
	LineSegmentApprox	line;
	SphereApprox		sphere;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineA = new LineSegmentApprox();
	SphereApprox		*sphereA = new SphereApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == sphereA->GetName())
				sphere = *(SphereApprox*)objApprox->objMath;
		}
	}

	if ((sphere.Radius == 0) || (line.PointStart == line.PointEnd))
	{
		delete sphereA;
		delete lineA;
		return 1;
	}

	PointGeometric		point1, point2;
	VectorGeometric     vector1, vector2, vector3;
	LineGeometric       line1;
	line1.Point = line.Point;
	line1.Vector = line.Vector;

	int colv;
	colv = sphere.IntersectionSphereAndLine(line, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_LineSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}

	delete sphereA;
	delete lineA;
	return (0);
}
// ---																										// LineIntersectionCylinder 
int Intersection::InCylinderLine(std::vector<ObjectApprox*> *objectsArray) {
	LineSegmentApprox	line;
	CylinderApprox		cylinder;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineA = new LineSegmentApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}

	if ((cylinder.Radius == 0) || (line.PointStart == line.PointEnd))
	{
		delete cylinderA;
		delete lineA;
		return 1;
	}
	PointGeometric		point1, point2;
	LineGeometric       line1;
	line1.Point = line.Point;
	line1.Vector = line.Vector;
	line1.Normalize();
	int colv;
	colv = cylinder.IntersectionCylinderAndLine(line, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_LineCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 1) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}
	delete cylinderA;
	delete lineA;
	return (0);
}
// -- -																										// LineIntersectionCone 
int Intersection::InConeLine(std::vector<ObjectApprox*> *objectsArray) {
	LineSegmentApprox	line;
	ConeApprox			cone;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineA = new LineSegmentApprox();
	ConeApprox			*coneA = new ConeApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == lineA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone = *(ConeApprox*)objApprox->objMath;
		}
	}

	if ((cone.Radius == 0) || (line.PointStart == line.PointEnd))
	{
		delete coneA;
		delete lineA;
		return 1;
	}
	PointGeometric		point1, point2;
	LineGeometric       line1;
	line1.Point = line.Point;
	line1.Vector = line.Vector;
	line1.Normalize();
	int colv;
	colv = cone.IntersectionConeAndLine(line, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_LineCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 1) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_LineCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}
	delete coneA;
	delete lineA;
	return (0);
}
// -- -																										// LineProjectionPlane
int Intersection::InPlaneLineProjection(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox		plane;
	LineSegmentApprox	line;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	LineSegmentApprox	*lineSegmentA = new LineSegmentApprox();
	RectangleApprox		*planeA = new RectangleApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == lineSegmentA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
		}
	}

	if ((line.PointStart == line.PointEnd) || Selection1 == true)
	{
		delete lineSegmentA;
		delete planeA;
		return 1;
	}
	PointGeometric		point1, point2;
	LineGeometric       line1;
	/*line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv;
	colv = plane.PlaneProjectionLine(line, &point1, &point2); //
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		LineSegmentApprox	*lineRes = new LineSegmentApprox();

		lineRes->PointStart = point1;
		lineRes->PointEnd = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)lineRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"LineProjection_PlaneLine";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return (1);
	}

	delete lineSegmentA;
	delete planeA;
	return (0);
}
// ---																										// Plane

// ---																										// PlaneIntersectionPlane  
int Intersection::InTwoPlane(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox plane1;
	RectangleApprox plane2;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();

	bool Selection1 = true;
	bool Selection2 = true;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane1 = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == planeA->GetName()) {
				plane2 = *(RectangleApprox*)objApprox->objMath;
				Selection2 = false;
			}
		}
	}

	if (Selection1 == true || Selection2 == true)
	{
		delete planeA;
		return 1;
	}

	PointGeometric		point1, point2;
	LineGeometric		line;

	int colv;
	colv = plane1.PlaneIntersectionPlane(plane2, &line);
	if (colv == 1)
		return(1);
	else {
		line.Normalize();
		std::vector<PointGeometric> point;
		point.push_back(plane1.Mesh.points[0]);
		point.push_back(plane1.Mesh.points[1]);
		point.push_back(plane1.Mesh.points[2]);
		point.push_back(plane1.Mesh.points[3]);
		point.push_back(plane2.Mesh.points[0]);
		point.push_back(plane2.Mesh.points[1]);
		point.push_back(plane2.Mesh.points[2]);
		point.push_back(plane2.Mesh.points[3]);

		for (int i = 0; i < (int)point.size(); i++) {
			point[i] = line.PointProjection(point[i]);
		}
		double maxdist = 0;
		double dist = 0;
		for (int i = 0; i < (int)point.size() - 1; i++) {
			for (int j = i + 1; j < (int)point.size(); j++) {
				dist = point[i].DistanceToPoint(point[j]);
				if (maxdist < dist) {
					maxdist = point[i].DistanceToPoint(point[j]);
					point1 = point[i];
					point2 = point[j];
				}
			}

		}

		ObjectApprox		*newObj = new ObjectApprox();
		LineSegmentApprox	*lineRes = new LineSegmentApprox();

		lineRes->PointStart = point1;
		lineRes->PointEnd = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)lineRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Line_twoPlane";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	delete planeA;
	return (0);
}
// ---																										// PlaneIntersectionCircle 
int Intersection::InPlaneCircle(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox		plane;
	CircleApprox		circle;

	if (objectsArray == nullptr)
	{
		return (0);
	}
	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();
	CircleApprox		*circleA = new CircleApprox();
	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == circleA->GetName())
				circle = *(CircleApprox*)objApprox->objMath;
		}
	}

	if ((circle.Radius == 0) || (Selection1 == true))
	{
		delete circleA;
		delete planeA;
		return 1;
	}
	PointGeometric		point1, point2;

	int colv;
	colv = plane.PlaneIntersectionCircle(circle, &point1, &point2);
	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_PlaneCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_PlaneCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_PlaneCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else
	{
		return (1);
	}

	delete circleA;
	delete planeA;
	return (0);
}
// ---																										// PlaneIntersectionSphere 
int Intersection::InSpherePlane(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox		plane;
	SphereApprox		sphere;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();
	SphereApprox		*sphereA = new SphereApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == sphereA->GetName())
				sphere = *(SphereApprox*)objApprox->objMath;
		}
	}

	if ((sphere.Radius == 0) || (Selection1 == true))
	{
		delete sphereA;
		delete planeA;
		return 1;
	}

	CircleGeometric		Circle;
	PointGeometric      pointOut;

	int colv;
	colv = sphere.IntersectionSphereAndPlane(plane, &Circle, &pointOut);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox	*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;
		//-- - -- - -- - -- -
			objectsArray->back()->Name = L"Circle_PlaneSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();
		*pointRes1 = pointOut;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;

		//	---	---	---	---
		objectsArray->back()->Name = L"Point_PlaneSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}

	delete planeA;
	delete sphereA;
	return (0);
}
// ---																										// PlaneIntersectionCylinder 
int Intersection::InCylinderPlane(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox		plane;
	CylinderApprox		cylinder;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}

	if ((cylinder.Radius == 0) || (Selection1 == true))
	{
		delete cylinderA;
		delete planeA;
		return 1;
	}

	CircleGeometric		Circle;

	int colv = 0;
	colv = cylinder.IntersectionCylinderAndPlane(plane, &Circle);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox	*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_PlaneCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}

	delete cylinderA;
	delete planeA;
	return (0);
}
// ---																										// PlaneIntersectionCone 
int Intersection::InConeAndPlane(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox	plane;
	ConeApprox		cone;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();
	ConeApprox			*coneA = new ConeApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone = *(ConeApprox*)objApprox->objMath;
		}
	}

	if ((cone.Radius == 0) || (Selection1 == true))
	{
		delete coneA;
		delete planeA;
		return 1;
	}

	PointGeometric		point;
	CircleGeometric		Circle;

	int colv;
	colv = cone.IntersectionConeAndPlane(plane, &Circle, &point);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox	*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Circle_PlaneCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_PlaneCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	else {	return(1); }

	delete coneA;
	delete planeA;
	return (0);
}
// ---																										// MiddlePlaneTwoPlnae 
int Intersection::InMiddleTwoPlane(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox plane1;
	RectangleApprox plane2;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	RectangleApprox		*planeA = new RectangleApprox();

	bool Selection1 = true;
	bool Selection2 = true;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName() && Selection1 == true) {
				plane1 = *(RectangleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == planeA->GetName()) {
				plane2 = *(RectangleApprox*)objApprox->objMath;
				Selection2 = false;
			}
		}
	}

	if (Selection1 == true || Selection2 == true)
	{
		delete planeA;
		return 1;
	}

	PlaneGeometric		planeRes;

	
	planeRes = plane1.MiddlePlane(plane2);
	PlaneApprox planeBetween;
	planeBetween.Line = planeRes.Line;
		////
	PlaneGeometric tmpPlane = planeRes.Line;
	VectorGeometric VectorX, VectorY, VectorZ;
	VectorX = VectorGeometric(planeRes.Line.Point, PointGeometric(planeRes.Line.Point.X + 10, planeRes.Line.Point.Y + 10, planeRes.Line.Point.Z + 10));
	VectorX = tmpPlane.VectorProjection(VectorX);
	VectorZ = planeRes.Line.Vector;
	VectorY = VectorX ^ VectorZ;
	PointGeometric	tmpPoint;
	tmpPoint = planeRes.Line.Point;


	PointGeometric		point1(-(20), 20, 0),
		point2(20, -(20), 0),
		point3(-(20), -(30), 0),
		point4(20, 30, 0);

	PointGeometric pointUnique1 = TransferPointToNewCoordinateSystem(point1,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointUnique2 = TransferPointToNewCoordinateSystem(point2,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointRepeat1 = TransferPointToNewCoordinateSystem(point3,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointRepeat2 = TransferPointToNewCoordinateSystem(point4,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	ObjectApprox	*newObj = new ObjectApprox();
	RectangleApprox		*planeRes1 = new RectangleApprox();
	planeRes1->Line.Vector = planeBetween.Line.Vector;
	planeRes1->Mesh.vectorsNormal.push_back(planeBetween.Line.Vector);
	planeRes1->Mesh.points.push_back(pointUnique1);
	planeRes1->Mesh.points.push_back(pointRepeat1);
	planeRes1->Mesh.points.push_back(pointRepeat2);
	planeRes1->Mesh.points.push_back(pointUnique2);
	planeRes1->Mesh.points.push_back(pointRepeat1);
	planeRes1->Mesh.points.push_back(pointRepeat2);

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)planeRes1;
	objectsArray->back()->flagReady = true;

		//	---	---	---	---
		objectsArray->back()->Name = L"Plane_twoPlane";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	delete planeA;
	return (0);
}
// ---																										// Circle

// ---																										// CircleIntersectionCircle  
int Intersection::InTwoCircle(std::vector<ObjectApprox*> *objectsArray)
{
	CircleApprox		circle;
	CircleApprox		circle2;
	

	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	CircleApprox		*circleA = new CircleApprox();
	


	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName() && circle.Radius == 0) {
				circle = *(CircleApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == circleA->GetName())
				circle2 = *(CircleApprox*)objApprox->objMath;
		}
	}


	if ((circle.Radius == 0) || (circle2.Radius == 0))
	{
		
		delete circleA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	/*LineGeometric       line1, line2, line3;
	line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv = 0;
	colv = circle.CircleIntersection(circle2, &point1, &point2);

	if (colv == 2)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();


		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;

		objectsArray->back()->Name = L"Point1_twoCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();

		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

		objectsArray->back()->Name = L"Point2_twoCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();

		
	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_twoCircle";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else
	{
		return (1);
	}

	delete circleA;
	return (0);


}
// ---																										// CircleIntersectionCylinder  тест
int Intersection::InCylinderCircle(std::vector<ObjectApprox*> *objectsArray) {
	CircleApprox		circle;
	CylinderApprox		cylinder;
	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CircleApprox		*circleA = new CircleApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName() && circle.Radius == 0) {
				circle = *(CircleApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}

	if ((circle.Radius == 0) || (cylinder.Radius == 0))
	{
		delete circleA;
		delete cylinderA;
		return 1;
	}

	PointGeometric		point1, point2;

	int colv;
	colv = cylinder.IntersectionCylinderAndCircle(circle, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_CircleCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_CircleCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_CircleCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	else
	{
		return (1);
	}

	delete cylinderA;
	delete circleA;
	return (0);
}
// ---																										// CircleIntersectionSphere
int Intersection::InSphereCircle(std::vector<ObjectApprox*> *objectsArray)
{
	CircleApprox		circle;
	SphereApprox		sphere;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CircleApprox		*circleA = new CircleApprox();
	SphereApprox		*sphereA = new SphereApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName() && Selection1 == true) {
				circle = *(CircleApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == sphereA->GetName())
				sphere = *(SphereApprox*)objApprox->objMath;
		}
	}

	if ((sphere.Radius == 0) || (Selection1 == true))
	{
		delete sphereA;
		delete circleA;
		return 1;
	}

	PointGeometric		point1, point2;

	int colv;
	colv = sphere.IntersectionSphereAndCircle(circle, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_CircleSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_CircleSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 1) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_CircleSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else {
		return(1);
	}

	delete sphereA;
	delete circleA;
	return (0);
}
// ---																										// CircleIntersectionCone     тест
int Intersection::InConeCircle(std::vector<ObjectApprox*> *objectsArray) {
	ConeApprox		cone;
	CircleApprox	circle;
	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	CircleApprox		*circleA = new CircleApprox();
	ConeApprox			*coneA = new ConeApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName() && circle.Radius == 0) {
				circle = *(CircleApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone = *(ConeApprox*)objApprox->objMath;
		}
	}

	if ((circle.Radius == 0) || (cone.Radius == 0))
	{
		delete circleA;
		delete coneA;
		return 1;
	}

	PointGeometric		point1, point2;

	int colv;
	colv = cone.IntersectionConeAndCircle(circle, &point1, &point2);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();

		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point1_CircleCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point2_CircleCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_CircleCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	else
	{
		return (1);
	}

	delete coneA;
	delete circleA;
	return (0);
}

// ---																										// Atr

// ---																										// TwoSphere
int Intersection::InTwoSphere(std::vector<ObjectApprox*> *objectsArray) {
	SphereApprox		sphere;
	SphereApprox		sphere2;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;
	SphereApprox		*sphereA = new SphereApprox();

	bool Selection1 = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);
		if (objApprox->flagReady == false)
			continue;
		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == sphereA->GetName() && Selection1 == true) {
				sphere = *(SphereApprox*)objApprox->objMath;
				Selection1 = false;
				continue;
			}
			if (objApprox->objMath->GetName() == sphereA->GetName())
				sphere2 = *(SphereApprox*)objApprox->objMath;
		}
	}

	if ((sphere2.Radius == 0) || (Selection1 == true))
	{
		delete sphereA;
		return 1;
	}
	CircleGeometric		Circle;
	PointGeometric		pointTouch;
	int colv;
	colv = sphere.IntersectionTwoSphere(sphere2, &Circle, &pointTouch);
	if (colv == 2) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox	*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Circle_twoSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = pointTouch;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_twoSphere";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	}

	else {
		return(1);
	}

	delete sphereA;
	return (0);
}
// ---																										// SphereCylinder
int Intersection::InSphereCylinder(std::vector<ObjectApprox*> *objectsArray) {
	SphereApprox		sphere;
	CylinderApprox		cylinder;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;

	SphereApprox		*sphereA = new SphereApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();



	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == sphereA->GetName() && sphere.Radius == 0) {
				sphere = *(SphereApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}


	if ((sphere.Radius == 0) || (cylinder.Radius == 0))
	{
		delete sphereA;
		delete cylinderA;
		return 1;
	}

	PointGeometric		point = sphere.IntersectionSphereCylinder(cylinder);

		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_SphereCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---

	delete cylinderA;
	delete sphereA;
	return (0);
}
// ---																										// SphereCone
int Intersection::InSphereCone(std::vector<ObjectApprox*> *objectsArray) {
	SphereApprox		sphere;
	ConeApprox			cone;

	if (objectsArray == nullptr)
	{
		return (0);
	}

	ObjectApprox		*objApprox;

	SphereApprox		*sphereA = new SphereApprox();
	ConeApprox		*coneA = new ConeApprox();

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == sphereA->GetName() && sphere.Radius == 0) {
				sphere = *(SphereApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone = *(ConeApprox*)objApprox->objMath;
		}
	}


	if ((sphere.Radius == 0) || (cone.Radius == 0))
	{
		delete sphereA;
		delete coneA;
		return 1;
	}

	PointGeometric		point = sphere.IntersectionSphereCone(cone);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;
	//	---	---	---	---
	objectsArray->back()->Name = L"Point_SphereCone";
	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	pView->RecalculateCenterOfAllObjects();
	//	---	---	---	---


	delete coneA;
	delete sphereA;
	return (0);
}
// ---																										// TwoCylinder
int Intersection::InTwoCylinder(std::vector<ObjectApprox*> *objectsArray) {
	CylinderApprox		cylinder;
	CylinderApprox		cylinder2;


	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	CylinderApprox		*cylinderA = new CylinderApprox();



	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == cylinderA->GetName() && cylinder.Radius == 0) {
				cylinder = *(CylinderApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder2 = *(CylinderApprox*)objApprox->objMath;
		}
	}


	if ((cylinder.Radius == 0) || (cylinder2.Radius == 0))
	{

		delete cylinderA;
		return 1;
	}

	PointGeometric		point1;

	int colv = 0;
	colv = cylinder.IntersectionTwoCylinder(cylinder2, &point1);

	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_twoCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 2)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"IncreesPoint_twoCylinder";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}

	else
	{
		return (1);
	}

	delete cylinderA;
	return (0);

}
// ---																										// CylinderCone
int Intersection::InCylinderCone(std::vector<ObjectApprox*> *objectsArray) {
	ConeApprox		cone;
	CylinderApprox		cylinder;


	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	ConeApprox		*coneA = new ConeApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();



	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == coneA->GetName() && cone.Radius == 0) {
				cone = *(ConeApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}


	if ((cone.Radius == 0) || (cylinder.Radius == 0))
	{
		delete coneA;
		delete cylinderA;
		return 1;
	}

	PointGeometric		point1;

	int colv = 0;
	colv = cylinder.IntersectionCylinderCone(cone, &point1);

	if (colv == 0 || colv == 2)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_CylinderCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	//else if (colv == 2)
	//{
	//	ObjectApprox	*newObj = new ObjectApprox();
	//	PointApprox		*pointRes1 = new PointApprox();

	//	*pointRes1 = point1;

	//	objectsArray->push_back(newObj);
	//	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	//	objectsArray->back()->flagReady = true;
	//	//	---	---	---	---
	//	objectsArray->back()->Name = L"IncreesPoint_CylinderCone";
	//	objectsArray->back()->objID = *nextObjectID;
	//	*nextObjectID += 1;

	//	pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

	//	pView->RecalculateCenterOfAllObjects();
	//	//	---	---	---	---
	//}
	else
	{
		return (1);
	}

	delete cylinderA;
	delete coneA;
	return (0);
}
// ---																										// TwoCone
int Intersection::InTwoCone(std::vector<ObjectApprox*> *objectsArray) {
	ConeApprox		cone;
	ConeApprox		cone2;


	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	ConeApprox		*coneA = new ConeApprox();



	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == coneA->GetName() && cone.Radius == 0) {
				cone = *(ConeApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == coneA->GetName())
				cone2 = *(ConeApprox*)objApprox->objMath;
		}
	}


	if ((cone.Radius == 0) || (cone.Radius == 0))
	{

		delete coneA;
		return 1;
	}

	PointGeometric		point1;

	int colv = 0;
	colv = cone.IntersectionTwoCone(cone2, &point1);

	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Point_twoCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else if (colv == 2)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"IncreesPoint_twoCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();
		//	---	---	---	---
	}
	else
	{
		return (1);
	}

	delete coneA;
	return (0);
}
// ---																										// MiddlePlaneConeACylinder
int Intersection::InCylinderConePlane(std::vector<ObjectApprox*> *objectsArray) {
	ConeApprox			cone;
	CylinderApprox		cylinder;


	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	ConeApprox			*coneA = new ConeApprox();
	CylinderApprox		*cylinderA = new CylinderApprox();


	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == coneA->GetName() && cone.Radius == 0) {
				cone = *(ConeApprox*)objApprox->objMath;
				continue;
			}
			if (objApprox->objMath->GetName() == cylinderA->GetName())
				cylinder = *(CylinderApprox*)objApprox->objMath;
		}
	}


	if ((cone.Radius == 0) || (cylinder.Radius == 0))
	{
		delete coneA;
		delete cylinderA;
		return 1;
	}

	PlaneGeometric		plane;
	
	plane = cylinder.MiddlePlaneCylinderAndCone(cone);
	
	PlaneApprox planeBetween;
	planeBetween.Line = plane.Line;
	////
	PlaneGeometric tmpPlane = plane.Line;
	VectorGeometric VectorX, VectorY, VectorZ;
	VectorX = VectorGeometric(plane.Line.Point, PointGeometric(plane.Line.Point.X + 10, plane.Line.Point.Y + 10, plane.Line.Point.Z + 10));
	VectorX = tmpPlane.VectorProjection(VectorX);
	VectorZ = plane.Line.Vector;
	VectorY = VectorX ^ VectorZ;
	PointGeometric	tmpPoint;
	tmpPoint = plane.Line.Point;


	PointGeometric		point1(-(20), 20, 0),
		point2(20, -(20), 0),
		point3(-(20), -(20), 0),
		point4(20, 20, 0);

	PointGeometric pointUnique1 = TransferPointToNewCoordinateSystem(point1,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointUnique2 = TransferPointToNewCoordinateSystem(point2,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointRepeat1 = TransferPointToNewCoordinateSystem(point3,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	PointGeometric pointRepeat2 = TransferPointToNewCoordinateSystem(point4,
		tmpPoint,
		VectorX,
		VectorY,
		VectorZ);
	ObjectApprox	*newObj = new ObjectApprox();
	RectangleApprox		*planeRes1 = new RectangleApprox();
	planeRes1->Line.Vector = planeBetween.Line.Vector;
	planeRes1->Mesh.vectorsNormal.push_back(planeBetween.Line.Vector);
	planeRes1->Mesh.points.push_back(pointUnique1);
	planeRes1->Mesh.points.push_back(pointRepeat1);
	planeRes1->Mesh.points.push_back(pointRepeat2);
	planeRes1->Mesh.points.push_back(pointUnique2);
	planeRes1->Mesh.points.push_back(pointRepeat1);
	planeRes1->Mesh.points.push_back(pointRepeat2);

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)planeRes1;
	objectsArray->back()->flagReady = true;
		//	---	---	---	---
		objectsArray->back()->Name = L"Plane_CylinderCone";
		objectsArray->back()->objID = *nextObjectID;
		*nextObjectID += 1;

		theApp.MTActive = theApp.MTplane;
		pDataTree->PostMessageW(WM_FULL_OBJECT_READY);

		pView->RecalculateCenterOfAllObjects();

	delete cylinderA;
	delete coneA;
	return (0);

}