#pragma once

#ifndef WM_FULL_OBJECT_READY
#define		WM_FULL_OBJECT_READY		WM_USER + 7	
#endif // !WM_MATH_OBJECT_READY



#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>

#include "..//Projects/MFCKio/DataTreeView.h"
#include "..//Projects/MFCKio/MFCKioView.h"
#include "..//Projects/MFCKio/MFCKio.h"

class Intersection
{
public:
	DataTreeView*	pDataTree;
	CMFCKioView*	pView;

	long int*		nextObjectID;

public:
	Intersection(DataTreeView* dataTree, CMFCKioView* view, long int* id_pointer);
	~Intersection();
	// ---																										// Point
	int InTwoPoint(std::vector<ObjectApprox*> *objectsArray, int variant);
	int InLinePoint(std::vector<ObjectApprox*> *objectsArray);
	int InCirclePoint(std::vector<ObjectApprox*> *objectsArray);
	int InSpherePoint(std::vector<ObjectApprox*> *objectsArray);
	int InPlanePoint(std::vector<ObjectApprox*> *objectsArray); //проблема при повторном выборе
	int InCylinderPoint(std::vector<ObjectApprox*> *objectsArray);
	int InConePoint(std::vector<ObjectApprox*> *objectsArray);

	// ---																										// MiddlePoint
	int InLinePointMiddle(std::vector<ObjectApprox*> *objectsArray);
	int InCirclePointMiddle(std::vector<ObjectApprox*> *objectsArray);    
	int InPlanePointMiddle(std::vector<ObjectApprox*> *objectsArray);
	// ---																										// Line
	int InTwoLine(std::vector<ObjectApprox*> *objectsArray);
	int InCircleLine(std::vector<ObjectApprox*> *objectsArray);
	int InPlaneLine(std::vector<ObjectApprox*> *objectsArray);
	int InPlaneLineProjection(std::vector<ObjectApprox*> *objectsArray);
	int InSphereLine(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderLine(std::vector<ObjectApprox*> *objectsArray);
	int InConeLine(std::vector<ObjectApprox*> *objectsArray);

	// ---																										// Plane
	int InTwoPlane(std::vector<ObjectApprox*> *objectsArray);
	int InSpherePlane(std::vector<ObjectApprox*> *objectsArray);
	int InPlaneCircle(std::vector<ObjectApprox*> *objectsArray);
	int InConeAndPlane(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderPlane(std::vector<ObjectApprox*> *objectsArray);
	int InMiddleTwoPlane(std::vector<ObjectApprox*> *objectsArray);
	// ---																										// Circle
	int InTwoCircle(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderCircle(std::vector<ObjectApprox*> *objectsArray);
	int InSphereCircle(std::vector<ObjectApprox*> *objectsArray);
	int InConeCircle(std::vector<ObjectApprox*> *objectsArray);
	// ---																										// Atr
	int InSphereCylinder(std::vector<ObjectApprox*> *objectsArray);
	int InSphereCone(std::vector<ObjectApprox*> *objectsArray);
	int InTwoCylinder(std::vector<ObjectApprox*> *objectsArray);
	int InTwoCone(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderCone(std::vector<ObjectApprox*> *objectsArray);
	int InTwoSphere(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderConePlane(std::vector<ObjectApprox*> *objectsArray);
};

