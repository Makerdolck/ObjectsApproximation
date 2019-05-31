#include "stdafx.h"
#include "Tolerance.h"


#define M_PI 3.1415926535897932384626433832795;

Tolerance::Tolerance()
{
}

Tolerance::Tolerance(std::vector<ToleranceObject*>* toleranceObjectsArray)
{
	this->toleranceObjectsArray = toleranceObjectsArray;
}

Tolerance::~Tolerance()
{
}

double Tolerance::FormStraightness(LineSegmentApprox* line)
{
	//LineGeometric lineGeom = LineGeometric(line->Vector);
	
	//double min = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](0));
	//double max = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](0));	
	//double min = DistanceBetween(line->PointStart, line->PointEnd, line->PointsForApprox.operator[](0));
	double min = DistanceBetween(line->PointsForApprox.operator[](0), line->PointsForApprox.operator[](line->PointsForApprox.size()-1), line->PointsForApprox.operator[](1));
	double max = DistanceBetween(line->PointsForApprox.operator[](0), line->PointsForApprox.operator[](line->PointsForApprox.size()-1), line->PointsForApprox.operator[](1));
	for (int i = 1; i < line->PointsForApprox.size()-1; i++) {
		double distance = DistanceBetween(line->PointsForApprox.operator[](0), line->PointsForApprox.operator[](line->PointsForApprox.size() - 1), line->PointsForApprox.operator[](i));
		//double distance = lineGeom.DistanceToPoint(line->PointsForApprox.operator[](i));

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
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);
	return round(max,3);
}

double Tolerance::FormFlatness(PlaneApprox* plane)
{
	//double max = DistanceBetween(plane->PointProjection(plane->PointsForApprox.operator[](0)), plane->PointsForApprox.operator[](0));
	double min = DistanceBetween(*plane, plane->PointsForApprox.operator[](0));
	double max = min;
	for (int i = 0; i < plane->PointsForApprox.size(); i++) {
		//double result = DistanceBetween(plane->PointProjection(plane->PointsForApprox.operator[](i)), plane->PointsForApprox.operator[](i));
		double result = DistanceBetween(*plane, plane->PointsForApprox.operator[](i));
		if (result > max) {
			max = result;
		}
		if (result < min) {
			min = result;
		}
	}



	CString str = L"";
	//str.Format(L"Result: %g", max);
	str.Format(L"�������������. Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min), 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min), 3);
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
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min) / 2, 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);
	return round(fabs(max - min) / 2, 3);
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
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max-min)/2, 3));
//	AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min) / 2, 3);
}

double Tolerance::OrientationParallelism(PlaneApprox *base, PlaneApprox *control)
{

	double min = DistanceBetween(*base, control->PointsForApprox.operator[](0));
	double max = min;

	for (int i = 0; i < control->PointsForApprox.size(); i++) {
		double distance = DistanceBetween(*base, control->PointsForApprox.operator[](i));
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min) / 2, 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min) / 2, 3);
}

double Tolerance::OrientationParallelism(LineSegmentApprox *base, LineSegmentApprox*control)
{
	double min = DistanceBetween(base->PointStart, base->PointEnd, control->PointsForApprox.operator[](0));
	double max = min;

	for (int i = 0; i < control->PointsForApprox.size(); i++) {
		double distance = DistanceBetween(base->PointStart, base->PointEnd, control->PointsForApprox.operator[](i));
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min) / 2, 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min) / 2, 3);
}


double Tolerance::OrientationAngularity(PlaneApprox* base, PlaneApprox* control, double angle)
{
	VectorGeometric rotatedPlane = rotatePlane(base, base->Line.Vector ^ control->Line.Vector, angle);


	double min = DistanceBetween(rotatedPlane, control->PointsForApprox.operator[](0));
	double max = min;

	for (int i = 1; i < control->PointsForApprox.size(); i++) {
		double distance = DistanceBetween(rotatedPlane, control->PointsForApprox.operator[](i));
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Angle: %g; Min: %g; Max: %g; Result: %g", AngleBetween(base->Line.Vector, control->Line.Vector), min, max, fabs(max - min));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);
	
	return fabs(max-min);
	
}


VectorGeometric Tolerance::getIntersectionVector(PlaneApprox Plane1, PlaneApprox Plane2) {
	double D1 = 0;
	double D2 = 0;
	D1 = 1 * (Plane1.Line.Vector.X * Plane1.Line.Point.X + Plane1.Line.Vector.Y * Plane1.Line.Point.Y + Plane1.Line.Vector.Z * Plane1.Line.Point.Z);
	D2 = 1 * (Plane2.Line.Vector.X * Plane2.Line.Point.X + Plane2.Line.Vector.Y * Plane2.Line.Point.Y + Plane2.Line.Vector.Z * Plane2.Line.Point.Z);

	VectorGeometric VectorPr;
	LineGeometric LineInter;
	VectorPr = Plane1.Line.Vector ^ Plane2.Line.Vector;
	LineInter.Vector = VectorGeometric(VectorPr.X, VectorPr.Y, VectorPr.Z, false);
	if ((VectorPr.X == 0) && (VectorPr.Y == 0) && (VectorPr.Z = 0)) {
		return VectorPr;
	}

	else {
		/*if (VectorPr.X != 0) {
			double del;
			del = Line.Vector.Y * Plane2.Line.Vector.Z - Line.Vector.Z*Plane2.Line.Vector.Y;
			LineInter.Point.Y = (D1*Plane2.Line.Vector.Z - Line.Vector.Z*D2) / del;
			LineInter.Point.Z = (Line.Vector.Y*D2 - Plane2.Line.Vector.Y*D1) / del;

		}*/
		double del;
		del = Plane1.Line.Vector.Y * Plane2.Line.Vector.Z - Plane1.Line.Vector.Z * Plane2.Line.Vector.Y;
		if (del != 0) {
			LineInter.Point.Y = (D1 * Plane2.Line.Vector.Z - Plane1.Line.Vector.Z * D2) / del;
			LineInter.Point.Z = (Plane1.Line.Vector.Y * D2 - Plane2.Line.Vector.Y * D1) / del;
		}
		if (del == 0) {
			del = Plane1.Line.Vector.X * Plane2.Line.Vector.Z - Plane1.Line.Vector.Z * Plane2.Line.Vector.X;
			LineInter.Point.X = (D1 * Plane2.Line.Vector.Z - Plane1.Line.Vector.Z * D2) / del;
			LineInter.Point.Z = (Plane1.Line.Vector.X * D2 - Plane2.Line.Vector.X * D1) / del;
		}

		//*line = LineInter;
		//return 1;
		//return LineInter;
		return LineInter.Vector;
	}
}

VectorGeometric Tolerance::rotatePlane(PlaneApprox* plane, VectorGeometric axis, double a) {
	
	axis.Normalize();
	double x = axis.X;
	double y = axis.Y;
	double z = axis.Z;
	a = a / 180 * M_PI;
	
	
	double matrix[3][3] = { 
		{cos(a) + (1 - cos(a)) * pow(x, 2), (1 - cos(a)) * x * y - sin(a) * z, (1 - cos(a)) * x * z + sin(a) * y},
		{(1 - cos(a)) * y * x + sin(a) * z, cos(a) + (1 - cos(a)) * pow(y, 2), (1 - cos(a)) * y * z - sin(a) * x},
		{(1 - cos(a)) * z * x - sin(a) * y, (1 - cos(a)) * z * y + sin(a) * x, cos(a) + (1 - cos(a)) * pow(z, 2)} 
	};


	RectangleApprox *test = (RectangleApprox*) plane;
	double vector[3] = { test->Plane.Line.Vector.X,test->Plane.Line.Vector.Y,test->Plane.Line.Vector.Z };
	double resultVector[3] = { 0,0,0 };
	for (int i = 0; i < 3; i++) {
		
		for (int j = 0; j < 3; j++) {
			resultVector[i] += (matrix[i][j] * vector[j]);
		}
		
	}
	VectorGeometric result = VectorGeometric(resultVector[0], resultVector[1], resultVector[2], false);

	return result;
	
}


VectorGeometric Tolerance::rotatePlane(VectorGeometric* v, VectorGeometric axis, double a) {
	
	double x = axis.X;
	double y = axis.Y;
	double z = axis.Z;
	a = a * 180 / M_PI;
	
	
	double matrix[3][3] = { 
		{cos(a) + (1 - cos(a)) * pow(x, 2), (1 - cos(a)) * x * y - sin(a) * z, (1 - cos(a)) * x * z + sin(a) * y},
		{(1 - cos(a)) * y * x + sin(a) * z, cos(a) + (1 - cos(a)) * pow(y, 2), (1 - cos(a)) * y * z - sin(a) * x},
		{(1 - cos(a)) * z * x - sin(a) * y, (1 - cos(a)) * z * y + sin(a) * x, cos(a) + (1 - cos(a)) * pow(z, 2)} 
	};



	double vector[3] = { v->X, v->Y, v->Z };
	double resultVector[3] = { 0,0,0 };
	for (int i = 0; i < 3; i++) {
		double tmp = 0;
		for (int j = 0; j < 3; j++) {
			tmp += matrix[i][j] * vector[j];
		}
		resultVector[i] = tmp;
	}
	VectorGeometric result = VectorGeometric(resultVector[0], resultVector[1], resultVector[2], false);

	
//	result.Line.Vector = 
	return result;
	
}

double Tolerance::OrientationPerpendicularity(PlaneApprox* base, PlaneApprox* control)
{
	double min = DistanceBetween(*base, control->PointsForApprox.operator[](0));
	double max = min;

	for (int i = 0; i < control->PointsForApprox.size(); i++) {
		double distance = DistanceBetween(*base, control->PointsForApprox.operator[](i));
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min) / 2, 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min) / 2, 3);
}

double Tolerance::LocationConcentricity(CircleApprox* circleA, CircleApprox* circleB)
{

	PointGeometric projCenterPoint = circleA->Line.Vector.PointProjection(centerByPoints(&circleA->PointsForApprox[0], circleA->PointsForApprox.size()), centerByPoints(&circleB->PointsForApprox[0], circleB->PointsForApprox.size()));
	double distance = DistanceBetween(centerByPoints(&circleA->PointsForApprox[0], circleA->PointsForApprox.size()), projCenterPoint);
	
	CString str = L"";
	str.Format(L"Result: %g", distance);
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);


	return round(distance, 3);
}

double Tolerance::LocationCoaxiality(CylinderApprox* cylinderA, CylinderApprox* cylinderB)
{

	double bottomDistance = DistanceBetween(cylinderA->PointBottomSurfaceCenter, cylinderA->PointTopSurfaceCenter, cylinderB->PointBottomSurfaceCenter);
	double topDistance = DistanceBetween(cylinderA->PointBottomSurfaceCenter, cylinderA->PointTopSurfaceCenter, cylinderB->PointTopSurfaceCenter);
	
	double max = bottomDistance;
	if (topDistance > bottomDistance) {
		max = topDistance;
	}
	
	CString str = L"";
	str.Format(L"Top: %g; Bottom: %g; Result: %g", topDistance, bottomDistance, round(max, 3));
	//AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(max, 3);
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



double Tolerance::RunOutFace(CylinderApprox* base, RectangleApprox* control)
{
	VectorGeometric normal = VectorGeometric(base->PointBottomSurfaceCenter, base->PointTopSurfaceCenter, false);

	double min = DistanceBetween(normal, control->PointsForApprox.operator[](0));
	double max = min;

	for (int i = 1; i < control->PointsForApprox.size(); i++) {
		double distance = DistanceBetween(normal, control->PointsForApprox.operator[](i));

		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	return round(fabs(max-min),3);
}

double Tolerance::RunOutFace(CylinderApprox* base, CircleApprox* control)
{
	return 0.0;
}

double Tolerance::RunOutRadial(CylinderApprox* base, CylinderApprox* control)
{

	PointGeometric bottomCenter = base->PointBottomSurfaceCenter;
	PointGeometric topCenter = base->PointTopSurfaceCenter;
	VectorGeometric axial = VectorGeometric(bottomCenter, topCenter, false);

	double max = axial.DistanceToPoint(control->PointsForApprox.operator[](0), bottomCenter);
	double min = axial.DistanceToPoint(control->PointsForApprox.operator[](0), bottomCenter);
	for (int i = 0; i < control->PointsForApprox.size(); i++) {
		double distance = axial.DistanceToPoint(control->PointsForApprox.operator[](i), bottomCenter);
		if (distance > max) {
			max = distance;
		}
		if (min > distance) {
			min = distance;
		}
	}

	CString str = L"";
	str.Format(L"Min: %g; Max: %g; Result: %g", min, max, round(fabs(max - min) / 2, 3));
	//	AfxMessageBox(str, MB_ICONWARNING | MB_OK);

	return round(fabs(max - min) / 2, 3);
}

SizeLine* Tolerance::DrawSizeLine(std::vector<ObjectApprox*>* objectsArray)
{
	if (objectsArray == nullptr)
	{
		AfxMessageBox(L"objectsArray == nullptr", MB_ICONWARNING | MB_OK);
		return nullptr;
	}
	if (objectsArray->size() < 1) {
		AfxMessageBox(L"��� ��������", MB_ICONWARNING | MB_OK);
		return nullptr;
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

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}
	if (countSelectedObject == 0) {
		AfxMessageBox(L"�� ������ �� ���� ������", MB_ICONWARNING | MB_OK);
		return nullptr;
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
		}else if (countSelectedObject == 2) {
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
				return nullptr;
			}
		}
		else {
			AfxMessageBox(L"������� ������� ����� ��������", MB_ICONWARNING | MB_OK);
			return nullptr;
		}
	}

	if (newSizeLine != nullptr) {
		addNewObject(newSizeLine);
		return newSizeLine;
	}
	return nullptr;
}

void Tolerance::DrawDiameterLine(std::vector<ObjectApprox*>* objectsArray) {
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
	ConeApprox* coneA = new ConeApprox();
	CylinderApprox* cylinderA = new CylinderApprox();
	SphereApprox* sphereA = new SphereApprox();
	TorusApprox* torusA = new TorusApprox();

	DiameterLine* newDiameterLine = nullptr;

	int countSelectedObject = 0; // ���������� ��������� ��������
	int objectNum = 1; // ����� �������

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}
	if (countSelectedObject == 0) {
		AfxMessageBox(L"�� ������ �� ���� ������", MB_ICONWARNING | MB_OK);
		return;
	}
	if (countSelectedObject > 1) {
		AfxMessageBox(L"���������� ������� ������ ���� ������", MB_ICONWARNING | MB_OK);
		return;
	}


	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (!objApprox->flagReady || !objApprox->flagSelected)
			continue;

		if (objApprox->objMath->GetName() == circleA->GetName()) {
			circleA = (CircleApprox*)objApprox->objMath;
			newDiameterLine = new DiameterLine(circleA);
		}
		else if (objApprox->objMath->GetName() == coneA->GetName()) {
			AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
		}
		else if (objApprox->objMath->GetName() == cylinderA->GetName()) {
			cylinderA = (CylinderApprox*)objApprox->objMath;
			newDiameterLine = new DiameterLine(cylinderA);
		}
		else if (objApprox->objMath->GetName() == sphereA->GetName()) {
			AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
		}
		else if (objApprox->objMath->GetName() == torusA->GetName()) {
			AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
		}
		else {
			AfxMessageBox(L"������������ ������", MB_ICONWARNING | MB_OK);
		}
	}

	if (newDiameterLine != nullptr) {
		addNewObject(newDiameterLine);
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

	AxialLine* newAxialLine = nullptr;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == CircleApprox().GetName()) {
				newAxialLine = new AxialLine((CircleApprox*)objApprox->objMath);
			}
			else if (objApprox->objMath->GetName() == CylinderApprox().GetName()) {
				newAxialLine = new AxialLine((CylinderApprox*)objApprox->objMath);
			}
			else if (objApprox->objMath->GetName() == ConeApprox().GetName()) {
				newAxialLine = new AxialLine((ConeApprox*)objApprox->objMath);
			}
			else {
				AfxMessageBox(L"��� �� �����������", MB_ICONWARNING | MB_OK);
			}
		}

	}

	if (newAxialLine != nullptr) {
		addNewObject(newAxialLine);
		newAxialLine->flagSelected = false;
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


void Tolerance::DrawOrientationParallelism(std::vector<ObjectApprox*>* objectsArray)
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

	PlaneApprox* planeBase = new PlaneApprox();
	PlaneApprox* planeControl = new PlaneApprox();


	//FormRoundnessToleranceObject* newTolerance = nullptr;

	int countSelectedObject = 0;
	int objectNum = 1;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}
	if (countSelectedObject == 0) {
		AfxMessageBox(L"�� ������ �� ���� ������", MB_ICONWARNING | MB_OK);
		return;
	}

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (!objApprox->flagReady || !objApprox->flagSelected)
			continue;


		if (countSelectedObject == 2) {
			
			if (objectNum == 1) {
				
					planeBase = (PlaneApprox*)objApprox->objMath;
					objectNum++;
					continue;
			}
			else {
				
					planeControl = (PlaneApprox*)objApprox->objMath;
					OrientationParallelism(planeBase, planeControl);
					break;
				
			}
		}
		else {
			AfxMessageBox(L"������� ������� ����� ��������", MB_ICONWARNING | MB_OK);
			return;
		}
	}


}




void Tolerance::DrawLocationConcentricity(std::vector<ObjectApprox*>* objectsArray)
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
	CircleApprox* circleB = new CircleApprox();


	//FormRoundnessToleranceObject* newTolerance = nullptr;

	int countSelectedObject = 0;
	int objectNum = 1;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}
	if (countSelectedObject == 0) {
		AfxMessageBox(L"�� ������ �� ���� ������", MB_ICONWARNING | MB_OK);
		return;
	}

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (!objApprox->flagReady || !objApprox->flagSelected)
			continue;


		if (countSelectedObject == 2) {
			
			if (objectNum == 1) {
				
					circleA = (CircleApprox*)objApprox->objMath;
					objectNum++;
					continue;
			}
			else {
				
					circleB = (CircleApprox*)objApprox->objMath;
					LocationConcentricity(circleA, circleB);
					break;
				
			}
		}
		else {
			AfxMessageBox(L"������� ������� ����� ��������", MB_ICONWARNING | MB_OK);
			return;
		}
	}


}

void Tolerance::DrawLocationCoaxiality(std::vector<ObjectApprox*>* objectsArray)
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

	CylinderApprox* cylinderA = new CylinderApprox();
	CylinderApprox* cylinderB = new CylinderApprox();


	//FormRoundnessToleranceObject* newTolerance = nullptr;

	int countSelectedObject = 0;
	int objectNum = 1;

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagSelected) {
			countSelectedObject++;
		}
	}
	if (countSelectedObject == 0) {
		AfxMessageBox(L"�� ������ �� ���� ������", MB_ICONWARNING | MB_OK);
		return;
	}

	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (!objApprox->flagReady || !objApprox->flagSelected)
			continue;


		if (countSelectedObject == 2) {
			
			if (objectNum == 1) {
				
				cylinderA = (CylinderApprox*)objApprox->objMath;
				objectNum++;
				continue;
			}
			else {
				cylinderB = (CylinderApprox*)objApprox->objMath;
				LocationCoaxiality(cylinderA, cylinderB);
				break;
			}
		}
		else {
			AfxMessageBox(L"������� ������� ����� ��������", MB_ICONWARNING | MB_OK);
			return;
		}
	}


}




void Tolerance::addNewObject(ToleranceObject* obj)
{
	toleranceObjectsArray->push_back(obj);
}

double Tolerance::AngleBetween(PlaneApprox plane1, PlaneApprox plane2)
{
	VectorGeometric n1 = plane1.Line.Vector;
	VectorGeometric n2 = plane2.Line.Vector;
	return AngleBetween(n1, n2);
}

double Tolerance::AngleBetween(VectorGeometric n1, VectorGeometric n2)
{
	double angleCos = 0;
	//angleCos = (n1.X * n2.X + n1.Y * n2.Y + n1.Z * n2.Z) / (n1.length() * n2.length());
	angleCos = n1 * n2 / (n1.length() * n2.length());
	return acos(angleCos) * 180 / M_PI;
}



double Tolerance::DistanceBetween(PointGeometric point1, PointGeometric point2)
{
	return sqrt(pow(point2.X - point1.X, 2) + pow(point2.Y - point1.Y, 2) + pow(point2.Z - point1.Z, 2));
}

double Tolerance::DistanceBetween(PointGeometric A, PointGeometric B, PointGeometric point)
{
	VectorGeometric AB = VectorGeometric(A, B, false);
	VectorGeometric AC = VectorGeometric(A, point, false);
	VectorGeometric res = (AC ^ AB);
	return res.length() / AB.length();
}





double Tolerance::DistanceBetween(PlaneApprox plane, PointGeometric point)
{
	PointGeometric centerPoint = plane.Line.Point;
	VectorGeometric N = plane.Line.Vector;
	PointGeometric tmp = PointGeometric(0.00, 25.00048, 89.997153);
	double d = -(N * plane.PointsForApprox.operator[](1));
	//double d = -(N * plane.Line.Point);
	//double d = -(N * tmp);
	//double d = 0;
	double result = (N.X * point.X + N.Y * point.Y + N.Z * point.Z + d) / N.length();
	return round(result, 3);

}

double Tolerance::DistanceBetween(VectorGeometric planeNormal, PointGeometric point)
{
	
	VectorGeometric N = planeNormal;

	//double d = -(N * plane.PointsForApprox.operator[](0));

	double d = 0;
	double result = (N.X * point.X + N.Y * point.Y + N.Z * point.Z + d) / N.length();
	return round(result, 3);

}





// ���������� value �� num_after_point ����� �������
double Tolerance::round(double value, int num_after_point) {
	int n = pow(10, num_after_point);
	int nValue = (int)(value * n);
	return  ((double)nValue)/n;
}