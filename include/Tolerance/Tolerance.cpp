#include "stdafx.h"
#include "Tolerance.h"




Tolerance::Tolerance(std::vector<ToleranceObject*>* toleranceObjectsArray)
{
	this->toleranceObjectsArray = toleranceObjectsArray;
}

Tolerance::~Tolerance()
{
}

double Tolerance::FormStraightness(LineSegmentApprox* line)
{
	LineGeometric lineGeom = LineGeometric(line->Vector);
	
	double min = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](0));
	double max = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](0));
	for (int i = 0; i < line->PointsForApprox.size(); i++) {
		//double distance = DistanceBetween(line->PointStart, line->PointEnd, line->PointsForApprox.operator[](i));
		double distance = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](i));

		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	//str.Format(L"Result: %g", max);
	double result = round(max, 4) - round(min, 4);
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, result);
	AfxMessageBox(str, MB_ICONWARNING | MB_OK);
	return max; // ����� �� ������ �� 2?
}

double Tolerance::FormFlatness(PlaneApprox* plane)
{
	double max = DistanceBetween(plane->PointProjection(plane->PointsForApprox.operator[](0)), plane->PointsForApprox.operator[](0));
	for (int i = 0; i < plane->PointsForApprox.size(); i++) {
		double result = DistanceBetween(plane->PointProjection(plane->PointsForApprox.operator[](i)), plane->PointsForApprox.operator[](i));
//		double result = distancePointPlane(plane->PointsForApprox.operator[](i), *plane);
		if (result > max) {
			max = result;
		}
	}

	CString str = L"";
	str.Format(L"Result: %g", max);
	AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return max;
}

double Tolerance::FormRoundness(CircleApprox* circle)
{
	double min = 0;
	double max = 0;
	PointGeometric centerPoint = centerByPoints(&circle->PointsForApprox[0], circle->PointsForApprox.size());
	min = DistanceBetween(centerPoint, circle->PointsForApprox.operator[](0));
	

	for (int i = 0; i < circle->PointsForApprox.size(); i++) {
		PointGeometric tmpPoint = circle->PointsForApprox.operator[](i);
		if (max < DistanceBetween(centerPoint, tmpPoint)) {
			max = DistanceBetween(centerPoint, tmpPoint);
		}

		if (min > DistanceBetween(centerPoint, tmpPoint)) {
			min = DistanceBetween(centerPoint, tmpPoint);
		}
	}
	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, abs(abs(max) - abs(min)));
	AfxMessageBox(str, MB_ICONWARNING | MB_OK);
	return abs(abs(max) - abs(min)); // ����� �� ������ �� 2?
}


double Tolerance::FormCylindricity(CylinderApprox* cylinder)
{
	PointGeometric bottomCenter = cylinder->PointBottomSurfaceCenter;
	PointGeometric topCenter = cylinder->PointTopSurfaceCenter;
	VectorGeometric axial = VectorGeometric(bottomCenter, topCenter, false);
	
	double max = axial.DistanceToPoint(cylinder->PointsForApprox.operator[](0), bottomCenter);
	double min = axial.DistanceToPoint(cylinder->PointsForApprox.operator[](0), bottomCenter);
	for (int i = 0; i < cylinder->PointsForApprox.size(); i++) {
		double distance = VectorGeometric(bottomCenter, topCenter, false).DistanceToPoint(cylinder->PointsForApprox.operator[](i), bottomCenter);
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, abs(max-min));
	AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return abs(max-min);
}


PointGeometric Tolerance::centerByPoints(PointGeometric* points, int arraySize)
{
	double sumX = 0, sumY = 0, sumZ = 0;

	for (int i = 0; i < arraySize; i++)
	{
		sumX += points[i].X;
		sumY += points[i].Y;
		sumZ += points[i].Z;
	}

	double x = sumX / (double)arraySize;
	double y = sumY / (double)arraySize;
	double z = sumZ / (double)arraySize;

	return PointGeometric(x, y, z);
}



void Tolerance::DrawSizeLine(std::vector<ObjectApprox*>* objectsArray)
{
	if (objectsArray == nullptr)
	{
		AfxMessageBox(L"objectsArray == nullptr", MB_ICONWARNING | MB_OK);
		return;
	}
	if (objectsArray->size() < 1) {
		AfxMessageBox(L"��� ��������", MB_ICONWARNING | MB_OK);
		return;
	}


	ObjectApprox* objApprox;

	PointApprox* point1 = new PointApprox();
	PointApprox* point2 = new PointApprox();

	RectangleApprox* planeA = new RectangleApprox();
	LineSegmentApprox* lineSegmentA = new LineSegmentApprox();

	ConeApprox* coneA = new ConeApprox();
	CylinderApprox* cylinderA = new CylinderApprox();
	SphereApprox* sphereA = new SphereApprox();
	TorusApprox* torusA = new TorusApprox();

	SizeLine* newSizeLine = nullptr;

	int countSelectedObject = 0;
	int objectNum = 1;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (!objApprox->flagReady || !objApprox->flagSelected)
			continue;

		
		if(countSelectedObject == 1){
			if (objApprox->objMath->GetName() == planeA->GetName()) {
				AfxMessageBox(L"���������� ������ ������ ��� ��������� (������������ ������)", MB_ICONWARNING | MB_OK);
			}
			else if (objApprox->objMath->GetName() == lineSegmentA->GetName()) {
				lineSegmentA = (LineSegmentApprox*)objApprox->objMath;
				newSizeLine = new SizeLine(lineSegmentA);
			}
			else if (objApprox->objMath->GetName() == coneA->GetName()) {
				coneA = (ConeApprox*)objApprox->objMath;
				newSizeLine = new SizeLine(coneA);

			}
			else if (objApprox->objMath->GetName() == cylinderA->GetName()) {
				cylinderA = (CylinderApprox*)objApprox->objMath;
				newSizeLine = new SizeLine(cylinderA);

			}
			else if (objApprox->objMath->GetName() == sphereA->GetName()) {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);

			}
			else if (objApprox->objMath->GetName() == torusA->GetName()) {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
			}
		}
		if (countSelectedObject == 2) {
			if (objApprox->objMath->GetName() == point1->GetName()) {
				if (objectNum == 1) {
					point1 = (PointApprox*)objApprox->objMath;
					objectNum++;
				}
				else{
					point2 = (PointApprox*)objApprox->objMath;
					newSizeLine = new SizeLine(point1, point2);
				}
			}
			else {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
			}
		}
	}

	if (newSizeLine != nullptr) {
		addNewObject(newSizeLine);
	}
}

void Tolerance::DrawAxialLine(std::vector<ObjectApprox*>* objectsArray)
{
	if (objectsArray == nullptr)
	{
		AfxMessageBox(L"objectsArray == nullptr", MB_ICONWARNING | MB_OK);
		return;
	}
	if (objectsArray->size() < 1) {
		AfxMessageBox(L"��� ��������", MB_ICONWARNING | MB_OK);
		return;
	}




	ObjectApprox* objApprox;

	CircleApprox* circleA = new CircleApprox();
	CylinderApprox* cylinderA = new CylinderApprox();
	ConeApprox* coneA = new ConeApprox();


	AxialLine* newAxialLine = nullptr;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName()) {
				circleA = (CircleApprox*)objApprox->objMath;
				newAxialLine = new AxialLine(circleA);
			}
			else if (objApprox->objMath->GetName() == cylinderA->GetName()) {
				cylinderA = (CylinderApprox*)objApprox->objMath;
				newAxialLine = new AxialLine(cylinderA);
			}
			else if (objApprox->objMath->GetName() == coneA->GetName()) {
				coneA = (ConeApprox*)objApprox->objMath;
				newAxialLine = new AxialLine(coneA);
			}
			else {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
			}
		}

	}

	if (newAxialLine != nullptr) {
		addNewObject(newAxialLine);
	}
}

void Tolerance::DrawFormRoundness(std::vector<ObjectApprox*>* objectsArray)
{
	if (objectsArray == nullptr)
	{
		AfxMessageBox(L"objectsArray == nullptr", MB_ICONWARNING | MB_OK);
		return;
	}
	if (objectsArray->size() < 1) {
		AfxMessageBox(L"��� ��������", MB_ICONWARNING | MB_OK);
		return;
	}



	ObjectApprox* objApprox;

	RectangleApprox* planeA = new RectangleApprox();
	LineSegmentApprox* lineSegmentA = new LineSegmentApprox();
	CircleApprox* circleA = new CircleApprox();

	ConeApprox* coneA = new ConeApprox();
	CylinderApprox* cylinderA = new CylinderApprox();
	SphereApprox* sphereA = new SphereApprox();
	TorusApprox* torusA = new TorusApprox();

	FormRoundnessToleranceObject* newTolerance = nullptr;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == circleA->GetName()) {
				circleA = (CircleApprox*)objApprox->objMath;
				newTolerance = new FormRoundnessToleranceObject(circleA);
				double result = FormRoundness(circleA);

			}
			else {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
			}

		}

	}

	if (newTolerance != nullptr) {
		addNewObject(newTolerance);

	}
}

void Tolerance::addNewObject(ToleranceObject* obj)
{
	toleranceObjectsArray->push_back(obj);
}

double Tolerance::DistanceBetween(PointGeometric point1, PointGeometric point2)
{
	return sqrt(pow(point2.X - point1.X, 2) + pow(point2.Y - point1.Y, 2) + pow(point2.Z - point1.Z, 2));
}

double Tolerance::DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point)
{
	double d = 0;
	VectorGeometric AB = VectorGeometric(A, B, false);
	VectorGeometric AC = VectorGeometric(A, point, false);
	VectorGeometric BC = AB * AC;
	
	double area = BC.length();
	d = area / AB.length();

	return d;
}

// ���������� value �� num_after_point ����� �������
double Tolerance::round(double value, int num_after_point) {
	int n = pow(10, num_after_point);
	int nValue = (int)(value * n);
	return  ((double)nValue)/n;
}
