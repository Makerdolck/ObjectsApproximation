#include "stdafx.h"

#include <conio.h>
#include <locale>

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

//#ifndef AFXTEMPL_H
//#define AFXTEMPL_H
//#include <afxtempl.h>
//#endif

#include <vector>

#include <fstream>
#include <iostream>
#include <ctime>


#include <ApproximationCore/_ALLincludesCore.h>


using namespace std; 


int main()
{
	cout.setf(ios::fixed);
	cout.precision(7);


	int enteredNumber = 0, enteredVariantMax = 0;

	double accuracy = 0.000001;

	do
	{

		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Cylinder" << endl;
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Cone" << endl;
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Torus" << endl;
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Sphere(Circle)" << endl;		// 4
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Plane" << endl;				// 5
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Line" << endl;				// 6
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Circle" << endl;				// 7
		enteredVariantMax++;
		std::cout << enteredVariantMax << ". " << "Rectangle" << endl;			// 8

		std::cin >> enteredNumber;

	} while (enteredNumber <= 0 || enteredNumber > enteredVariantMax);

	system("cls");

	ifstream file;	
	
	std::vector <PointGeometric> points;
	PointGeometric point;

	CylinderApprox		cylinder;
	ConeApprox			cone;
	TorusApprox			torus;
	SphereApprox		sphere;
	PlaneApprox			plane;
	LineSegmentApprox	line;
	CircleApprox		circle;
	RectangleApprox		rectangle;

	switch (enteredNumber) {
	case 1:

		file.open("dataCylinder.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
			//points.Add(point);
		}

		cylinder.FindByPoints(&points[0], (int)points.size(), accuracy);

		cout << "Center: \t" << cylinder.Line.Point.X	<< "\t" << cylinder.Line.Point.Y	<< "\t" << cylinder.Line.Point.Z	<< endl;
		cout << "Vector: \t" << cylinder.Line.Vector.X	<< "\t" << cylinder.Line.Vector.Y	<< "\t" << cylinder.Line.Vector.Z	<< endl;
		cout << "Diametr: \t" << cylinder.Radius*2 << endl;
		//cout << "Radius: \t" << cylinder.Radius			<< endl;
		//cout << "Height: \t" << cylinder.Height			<< endl << endl;;

		break;
	case 2:

		file.open("dataCone.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
		}

		cone.FindByPoints(&points[0], (int)points.size(), accuracy);

		cout << "Point:  \t" << cone.Line.Point.X << "\t" << cone.Line.Point.Y << "\t" << cone.Line.Point.Z << endl;
		cout << "Vector: \t"		<< cone.Line.Vector.X	<< "\t" << cone.Line.Vector.Y	<< "\t" << cone.Line.Vector.Z << endl;
		cout << "Angle:  \t"		<< cone.Angle			<< endl;
		//cout << "Height: \t"		<< cone.Height			<< endl;
		//cout << "Radius: \t"		<< cone.Radius			<< endl;
		//cout << "RadiusSmaller: \t" << cone.RadiusSmaller	<< endl;
		//cout << "HeightPhantom: \t" << cone.HeightPhantom	<< endl << endl;
		
		break;
	case 3:

		file.open("dataTorus.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
		}

		torus.FindByPoints(&points[0], (int)points.size(), accuracy);

		cout << "Vector: \t"		<< torus.Line.Vector.X	<< "\t"	<< torus.Line.Vector.Y << "\t"	<< torus.Line.Vector.Z << endl;
		cout << "Point:  \t"		<< torus.Line.Point.X	<< "\t"	<< torus.Line.Point.Y << "\t"	<< torus.Line.Point.Z << endl;
		cout << "radiusCircleM:\t"	<< torus.RadiusCircle	<< endl;
		cout << "Height: \t"		<< torus.Height			<< endl;
		cout << "Radius: \t"		<< torus.Radius			<< endl << endl;

		break;
	case 4:

		file.open("dataSphere.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
		}

		sphere.FindByPoints(&points[0], (int)points.size(), accuracy);

		cout << "Center: \t" << sphere.Line.Point.X << "\t" << sphere.Line.Point.Y << "\t" << sphere.Line.Point.Z << endl;
		cout << "Height: \t" << sphere.Height << endl;
		cout << "Radius: \t" << sphere.Radius << endl;

		break;
	case 5:

		file.open("dataPlane.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			//cout << point.X << " " << point.Y << " " << point.Z << endl;
			points.push_back(point);
		}

		plane.FindByPoints(&points[0], (int)points.size(), 0.000001);

		cout << "Vector: \t" << plane.Line.Vector.X << "\t" << plane.Line.Vector.Y << "\t" << plane.Line.Vector.Z << endl;
		cout << "Center: \t" << plane.Line.Point.X << "\t" << plane.Line.Point.Y << "\t" << plane.Line.Point.Z << endl;

		break;
	case 6:

		file.open("dataLine.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
		}
		
		line.FindByPoints(&points[0], (int)points.size(), 0.000001);

		cout << "Vector: \t"	<< line.Vector.X		<< "\t" << line.Vector.Y		<< "\t" << line.Vector.Z		<< endl;
		cout << "Center: \t"	<< line.Point.X			<< "\t" << line.Point.Y			<< "\t" << line.Point.Z			<< endl;
		cout << "Point_Start:\t"<< line.PointStart.X	<< "\t" << line.PointStart.Y	<< "\t" << line.PointStart.Z	<< endl;
		cout << "Point_End:\t"	<< line.PointEnd.X		<< "\t" << line.PointEnd.Y		<< "\t" << line.PointEnd.Z		<< endl;

		break;
	case 7:

		file.open("dataSphere.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			points.push_back(point);
		}

		circle.FindByPoints(&points[0], (int)points.size(), 0.000001);

		cout << "Vector: \t" << circle.Line.Vector.X	<< "\t" << circle.Line.Vector.Y << "\t" << circle.Line.Vector.Z << endl;
		cout << "Center: \t" << circle.Line.Point.X		<< "\t" << circle.Line.Point.Y	<< "\t" << circle.Line.Point.Z << endl;
		cout << "Radius: \t" << circle.Radius			<< endl << endl;

		break;
	case 8:

		file.open("dataPlane.txt");
		if (!file.is_open())
		{
			cout << "Problem with opening file...";
			_getch();
			return 1;
		}

		while (!file.eof())					// Reading File "data.txt"
		{
			file >> point.X >> point.Y >> point.Z;
			//cout << point.X << " " << point.Y << " " << point.Z << endl;
			points.push_back(point);
		}

		rectangle.FindByPoints(&points[0], (int)points.size(), 0.000001);

		cout << "Vector: \t" << rectangle.Line.Vector.X << "\t" << rectangle.Line.Vector.Y  << "\t" << rectangle.Line.Vector.Z<< endl;
		cout << "Center: \t" << rectangle.Line.Point.X  << "\t"  << rectangle.Line.Point.Y  << "\t" << rectangle.Line.Point.Z << endl;
		cout << "VectorX:\t" << rectangle.VectorX.X		<< "\t" << rectangle.VectorX.Y		<< "\t" << rectangle.VectorX.Z	  << endl;
		cout << "Width:  \t" << rectangle.Width			<< endl ;
		cout << "Height: \t" << rectangle.Height		<< endl << endl;

		break;
	default:
		return 1;
		break;
	}
	
	points.clear();
	
	_getch();
	return 0;
}
