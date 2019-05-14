#pragma once


#include <ApproximationCore/_ALLincludesCore.h>
#include <vector>

class Intersection
{
public:
	Intersection();
	~Intersection();

	int InCircleLine(std::vector<ObjectApprox*> *objectsArray);
	int InTwoCircle(std::vector<ObjectApprox*> *objectsArray);
	int InCirclePointMiddle(std::vector<ObjectApprox*> *objectsArray);
	int InLinePoint(std::vector<ObjectApprox*> *objectsArray);
	int InTwoPlane(std::vector<ObjectApprox*> *objectsArray);
	int InPlaneLine(std::vector<ObjectApprox*> *objectsArray);
	int InTwoLine(std::vector<ObjectApprox*> *objectsArray);
	int InSpherePlane(std::vector<ObjectApprox*> *objectsArray);
	int InPlaneCircle(std::vector<ObjectApprox*> *objectsArray);
	int InPlanePointMiddle(std::vector<ObjectApprox*> *objectsArray);
	int InLinePointMiddle(std::vector<ObjectApprox*> *objectsArray);
	int InPlanePoint(std::vector<ObjectApprox*> *objectsArray);
	int InCirclePoint(std::vector<ObjectApprox*> *objectsArray);
	int InSphereLine(std::vector<ObjectApprox*> *objectsArray);
	int InSphereCircle(std::vector<ObjectApprox*> *objectsArray);
	int InTwoPoint(std::vector<ObjectApprox*> *objectsArray, int variant);
	int InCylinderPlane(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderLine(std::vector<ObjectApprox*> *objectsArray);
	int InSpherePoint(std::vector<ObjectApprox*> *objectsArray);
	int InTwoCylinder(std::vector<ObjectApprox*> *objectsArray);
	int InTwoCone(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderCone(std::vector<ObjectApprox*> *objectsArray);
	int InCylinderCircle(std::vector<ObjectApprox*> *objectsArray);
	int InConeAndPlane(std::vector<ObjectApprox*> *objectsArray);

};

