#include "stdafx.h"
#include "Intersection.h"


Intersection::Intersection()
{
}


Intersection::~Intersection()
{
}

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
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	line1.Point = line.Point;
	line1.Vector = line.Vector;
	
	int colv = 0;
	colv = circle.LineIntersection(line1, &point1, &point2);

	if (colv == 2) 
	{
		ObjectApprox	*newObj		= new ObjectApprox();
		PointApprox		*pointRes1	= new PointApprox(),
						*pointRes2	= new PointApprox();


		*pointRes1 = point1;
		*pointRes2 = point2;
		
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

		//glPointSize(5);
		//glBegin(GL_POINTS);

		//glVertex3d((GLfloat)point1.X,
		//	(GLfloat)point1.Y,
		//	(GLfloat)point1.Z);
		//glVertex3d((GLfloat)point2.X,
		//	(GLfloat)point2.Y,
		//	(GLfloat)point2.Z);
		//glEnd();
		//glPointSize(1);
	}
	else if (colv == 1) 
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;


		//glPointSize(5);
		//glBegin(GL_POINTS);

		//glVertex3d((GLfloat)point1.X,
		//	(GLfloat)point1.Y,
		//	(GLfloat)point1.Z);

		//glEnd();
		//glPointSize(1);
	}
	else 
	{
		return (1);
	}
	//
	delete lineSegmentA;
	delete circleA;
	return (0);
}

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
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

		
	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;


	
	}
	else
	{
		return (1);
	}
	//
	//delete lineSegmentA;
	delete circleA;
	return (0);


}

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
	//
	
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	

	PointGeometric point1 = circle.PointIntersectionMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete circleA;
	delete pointA;
	return (0);


}

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
	//

	VectorGeometric     vector1, vector2, vector3;
	LineGeometric       line1, line2, line3;
	


	PointGeometric point1 = line.PointBetweenLine(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete lineA;
	delete pointA;
	return (0);


}

int Intersection::InTwoPlane(std::vector<ObjectApprox*> *objectsArray)
{
	RectangleApprox plane1;
	RectangleApprox plane2;


	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	RectangleApprox			*planeA = new RectangleApprox();
	

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


	if (Selection1 == true || Selection2 == true )
	{
		delete planeA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	LineGeometric		line;

	int colv = 0;
	colv = plane1.PlaneIntersectionPlane(plane2, &line);
	if (colv == 1)
		return(1);

	//ObjectApprox	*newObj = new ObjectApprox();
	//PointApprox		*pointRes1 = new PointApprox(),
	//	*pointRes2 = new PointApprox();
	
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
		for (int i = 0; i < (int)point.size() - 1;i++ ) {
			for (int j = i+1; j < (int)point.size(); j++) {
				dist = point[i].DistanceToPoint(point[j]);
				if (maxdist < dist) {
					maxdist = point[i].DistanceToPoint(point[j]);
					point1 = point[i];
					point2 = point[j];
				}
			}
			
		}
		

			ObjectApprox	*newObj = new ObjectApprox();
			LineSegmentApprox	*lineRes = new LineSegmentApprox();


			lineRes->PointStart = point1;
			lineRes->PointEnd = point2;

			objectsArray->push_back(newObj);
			objectsArray->back()->objMath = (GeomObjectApprox*)lineRes;
			objectsArray->back()->flagReady = true;
			/*newObj = new ObjectApprox();
			objectsArray->push_back(newObj);
			objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
			objectsArray->back()->flagReady = true;*/

	


	delete planeA;
	return (0);


}

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


	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName())
				plane = *(RectangleApprox*)objApprox->objMath;
			if (objApprox->objMath->GetName() == lineSegmentA->GetName())
				line = *(LineSegmentApprox*)objApprox->objMath;
		}
	}


	if ( (line.PointStart == line.PointEnd))
	{
		delete lineSegmentA;
		delete planeA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	line1.Point = line.Point;
	line1.Vector = line.Vector;

	int colv = 0;
	colv = plane.PlaneBetweenLine(line1, &point1);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
	}

	delete lineSegmentA;
	delete planeA;
	return (0);
}

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
	//
	PointGeometric		point1, point2,  pointout;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       Line2;
	Line2.Point = line2.Point;
	Line2.Vector = line2.Vector;

	int colv = 0;
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
		

	}
	
	else
	{
		return (1);
	}
	//
	//delete lineSegmentA;
	delete lineA;
	return (0);




}
//int Intersection::InPlaneLine(std::vector<ObjectApprox*> *objectsArray)
//{
//	RectangleApprox		plane;
//	LineSegmentApprox	line;
//
//	if (objectsArray == nullptr)
//	{
//		return (0);
//	}
//
//
//	ObjectApprox		*objApprox;
//
//	LineSegmentApprox	*lineSegmentA = new LineSegmentApprox();
//	RectangleApprox		*planeA = new RectangleApprox();
//
//
//	for (int i = 0; i < (int)objectsArray->size(); i++)
//	{
//		objApprox = objectsArray->operator[](i);
//
//		if (objApprox->flagReady == false)
//			continue;
//
//		if (objApprox->flagSelected)
//		{
//			if (objApprox->objMath->GetName() == planeA->GetName())
//				plane = *(RectangleApprox*)objApprox->objMath;
//			if (objApprox->objMath->GetName() == lineSegmentA->GetName())
//				line = *(LineSegmentApprox*)objApprox->objMath;
//		}
//	}
//
//
//	if ( (line.PointStart == line.PointEnd))
//	{
//		delete lineSegmentA;
//		delete planeA;
//		return 1;
//	}
//	//
//	PointGeometric		point1, point2, point3, point4;
//	VectorGeometric     vector1, vector2, vector3;
//	CircleGeometric		Circle, Circle2;
//	LineGeometric       line1, line2, line3;
//	line1.Point = line.Point;
//	line1.Vector = line.Vector;
//
//	int colv = 0;
//	colv = plane.PlaneBetweenLine(line1);
//	if (colv == 0) {
//		ObjectApprox	*newObj = new ObjectApprox();
//		PointApprox		*pointRes1 = new PointApprox();
//
//		*pointRes1 = point1;
//
//		objectsArray->push_back(newObj);
//		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
//		objectsArray->back()->flagReady = true;
//	}
//
//	delete lineSegmentA;
//	delete planeA;
//	return (0);
//}
//int Intersection::InTwoLine(std::vector<ObjectApprox*> *objectsArray) {
//	LineSegmentApprox	line;
//	LineSegmentApprox	line2;
//	
//	if (objectsArray == nullptr)
//	{
//		return (0);
//	}
//
//
//	ObjectApprox		*objApprox;
//
//	LineSegmentApprox		*lineA = new LineSegmentApprox();
//
//
//
//	for (int i = 0; i < (int)objectsArray->size(); i++)
//	{
//		objApprox = objectsArray->operator[](i);
//
//		if (objApprox->flagReady == false)
//			continue;
//
//		if (objApprox->flagSelected)
//		{
//			if (objApprox->objMath->GetName() == lineA->GetName() && (line.PointStart == line.PointEnd)) {
//				line = *(LineSegmentApprox*)objApprox->objMath;
//				continue;
//			}
//			if (objApprox->objMath->GetName() == lineA->GetName())
//				line2 = *(LineSegmentApprox*)objApprox->objMath;
//		}
//	}
//
//
//	if ((line.PointStart == line.PointEnd) || (line2.PointStart == line2.PointEnd))
//	{
//
//		delete lineA;
//		return 1;
//	}
//	//
//	PointGeometric		point1, point2,  pointout;
//	VectorGeometric     vector1, vector2, vector3;
//	CircleGeometric		Circle, Circle2;
//	LineGeometric       Line2;
//	Line2.Point = line.Point;
//	Line2.Vector = line.Vector;
//
//	int colv = 0;
//	colv = line.LineBetweenLine(Line2, &pointout);
//
//	if (colv == 0)
//	{
//		ObjectApprox	*newObj = new ObjectApprox();
//		PointApprox		*pointRes1 = new PointApprox(),
//			*pointRes2 = new PointApprox();
//
//
//		*pointRes1 = point1;
//		*pointRes2 = point2;
//
//		objectsArray->push_back(newObj);
//		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
//		objectsArray->back()->flagReady = true;
//		newObj = new ObjectApprox();
//		objectsArray->push_back(newObj);
//		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
//		objectsArray->back()->flagReady = true;
//
//
//	}
//	
//	else
//	{
//		return (1);
//	}
//	//
//	//delete lineSegmentA;
//	delete lineA;
//	return (0);
//
//}

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


	if ((sphere.Radius == 0)&& (Selection1 = true))
	{
		delete sphereA;
		delete planeA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	/*line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv = 0;
	colv = sphere.IntersectionSphereAndPlane(plane, &Circle);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox		*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;

	}
	else {
		return(1);
	}

	delete sphereA;
	delete planeA;
	return (0);
}

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


	if ((circle.Radius == 0) && (Selection1 = true))
	{
		delete circleA;
		delete planeA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	/*line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv = 0;
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
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;
	
	}
	else if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;

	}
	else
	{
		return (1);
	}

	delete circleA;
	delete planeA;
	return (0);
}

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

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName())
				plane = *(RectangleApprox*)objApprox->objMath;


			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}

	}


	if ( Selection == true)
	{
		delete planeA;
		delete pointA;
		return 1;
	}
	//

	VectorGeometric     vector1, vector2, vector3;
	LineGeometric       line1, line2, line3;



	PointGeometric point1 = plane.PointBetweenPlaneMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete planeA;
	delete pointA;
	return (0);

}

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
	//

	VectorGeometric     vector1, vector2, vector3;
	LineGeometric       line1, line2, line3;



	PointGeometric point1 = line.PointBetweenLineMiddle(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete lineA;
	delete pointA;
	return (0);


}

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

	bool Selection = true;
	for (int i = 0; i < (int)objectsArray->size(); i++)
	{
		objApprox = objectsArray->operator[](i);

		if (objApprox->flagReady == false)
			continue;

		if (objApprox->flagSelected)
		{
			if (objApprox->objMath->GetName() == planeA->GetName())
				plane = *(RectangleApprox*)objApprox->objMath;


			if (objApprox->objMath->GetName() == pointA->GetName()) {
				point = *(PointApprox*)objApprox->objMath;
				Selection = false;
			}
		}

	}


	if (Selection == true)
	{
		delete planeA;
		delete pointA;
		return 1;
	}
	//

	VectorGeometric     vector1, vector2, vector3;
	LineGeometric       line1, line2, line3;



	PointGeometric point1 = plane.PointBetweenPlane(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete planeA;
	delete pointA;
	return (0);

}

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

	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;


	PointGeometric point1 = circle.PointIntersection(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete circleA;
	delete pointA;
	return (0);
}

int Intersection::InSphereLine(std::vector<ObjectApprox*> *objectsArray)
{
	LineSegmentApprox	line;
	SphereApprox		sphere;

	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	LineSegmentApprox		*lineA = new LineSegmentApprox();
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


	if ((sphere.Radius == 0) && (line.PointStart == line.PointEnd))
	{
		delete sphereA;
		delete lineA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	line1.Point = line.Point;
	line1.Vector = line.Vector;

	int colv = 0;
	colv = sphere.IntersectionSphereAndLine(line, &point1,&point2);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();


		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

	}
	else {
		return(1);
	}

	delete sphereA;
	delete lineA;
	return (0);
}

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
	//
	PointGeometric		point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	LineGeometric		line;

	
	
	

	if (variant == 1) {
		point3 = point1.PointBetweenPointMiddle(point2);


		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point3;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		
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

	}

	delete pointA;
	return (0);


}
////////////////////////////
int Intersection::InCylinderLine(std::vector<ObjectApprox*> *objectsArray) {
	LineSegmentApprox	line;
	CylinderApprox		cylinder;

	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	LineSegmentApprox		*lineA = new LineSegmentApprox();
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


	if ((cylinder.Radius == 0) && (line.PointStart == line.PointEnd))
	{
		delete cylinderA;
		delete lineA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	line1.Point = line.Point;
	line1.Vector = line.Vector;
	line1.Normalize();
	int colv = 0;
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
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

	}
	if (colv == 1) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
	}
	else {
		return(1);
	}

	delete cylinderA;
	delete lineA;
	return (0);
}


/////////////////////////////
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


	if ((cylinder.Radius == 0) && (Selection1 = true))
	{
		delete cylinderA;
		delete planeA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;
	/*line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv = 0;
	colv = cylinder.IntersectionCylinderAndPlane(plane, &Circle);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox		*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;

	}
	else {
		return(1);
	}

	delete cylinderA;
	delete planeA;
	return (0);
}
///////////////
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

	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;


	PointGeometric point1 = sphere.PointIntersection(point);

	ObjectApprox	*newObj = new ObjectApprox();
	PointApprox		*pointRes1 = new PointApprox();

	*pointRes1 = point1;

	objectsArray->push_back(newObj);
	objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
	objectsArray->back()->flagReady = true;

	delete sphereA;
	delete pointA;
	return (0);

}

////////////////////
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
	colv = cylinder.IntersectionTwoCylinder(cylinder2,&point1);

	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;
		
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
	}

	else
	{
		return (1);
	}

	delete cylinderA;
	return (0);

}
///////
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
	}

	else
	{
		return (1);
	}

	delete coneA;
	return (0);
}

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

	if (colv == 0)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
	}

	else
	{
		return (1);
	}

	delete cylinderA;
	delete coneA;
	return (0);
}
////////////////////////////////
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

	PointGeometric		point1,point2;

	int colv = 0;
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
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

	}
	if (colv == 1)
	{
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox();

		*pointRes1 = point1;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
	}

	else
	{
		return (1);
	}

	delete cylinderA;
	delete circleA;
	return (0);
}
//////////////////////////////
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


	if ((sphere.Radius == 0) && (Selection1 = true))
	{
		delete sphereA;
		delete circleA;
		return 1;
	}
	//
	PointGeometric		point1, point2, point3, point4;
	VectorGeometric     vector1, vector2, vector3;
	CircleGeometric		Circle, Circle2;
	LineGeometric       line1, line2, line3;

	int colv = 0;
	colv = sphere.IntersectionSphereAndCircle(circle, &point1, &point2);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		PointApprox		*pointRes1 = new PointApprox(),
			*pointRes2 = new PointApprox();


		*pointRes1 = point1;
		*pointRes2 = point2;

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes1;
		objectsArray->back()->flagReady = true;
		newObj = new ObjectApprox();
		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)pointRes2;
		objectsArray->back()->flagReady = true;

	}
	else {
		return(1);
	}

	delete sphereA;
	delete circleA;
	return (0);
}
int Intersection::InConeAndPlane(std::vector<ObjectApprox*> *objectsArray) {
	RectangleApprox		plane;
	ConeApprox		cone;

	if (objectsArray == nullptr)
	{
		return (0);
	}


	ObjectApprox		*objApprox;

	RectangleApprox		*planeA = new RectangleApprox();
	ConeApprox		*coneA = new ConeApprox();

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


	if ((cone.Radius == 0) && (Selection1 = true))
	{
		delete coneA;
		delete planeA;
		return 1;
	}
	//
	CircleGeometric		Circle;
	/*line1.Point = line.Point;
	line1.Vector = line.Vector;*/

	int colv;
	colv = cone.IntersectionConeAndPlane(plane, &Circle);
	if (colv == 0) {
		ObjectApprox	*newObj = new ObjectApprox();
		CircleApprox		*CircleRes = new CircleApprox();

		CircleRes->Line.Point = Circle.Line.Point;
		CircleRes->Line.Vector = Circle.Line.Vector;
		CircleRes->Radius = Circle.Radius;
		CircleRes->Triangulation();

		objectsArray->push_back(newObj);
		objectsArray->back()->objMath = (GeomObjectApprox*)CircleRes;
		objectsArray->back()->flagReady = true;

	}
	else {
		return(1);
	}

	delete coneA;
	delete planeA;
	return (0);

}