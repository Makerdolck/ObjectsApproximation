#include "stdafx.h"

#include "TorusApprox.h"


// ---																										// Constructors
TorusApprox::TorusApprox() { objectApproxName = (char*)"torus"; RadiusCircle = 0; }

TorusApprox::~TorusApprox(){}
// ---																										// --- APPROXIMATION ---
double TorusApprox::FunctionApprox(PointGeometric *points, int arraySize)
{
	double sum = 0;

	PointGeometric circleCenter;
	VectorGeometric* vector = nullptr;

	for (int i = 0; i < arraySize; i++)
	{
		circleCenter = Line.Vector.PointProjection(Line.Point, points[i]);
		delete vector;
		vector = new VectorGeometric(Line.Point, circleCenter);
		//vector->Normalize();

		circleCenter.X = Line.Point.X + Radius * vector->X;
		circleCenter.Y = Line.Point.Y + Radius * vector->Y;
		circleCenter.Z = Line.Point.Z + Radius * vector->Z;

		sum += pow(
			circleCenter.DistanceToPoint(points[i]) - RadiusCircle
			, 2);
	}

	delete vector;
	return sum;
}

void TorusApprox::FindByPoints(PointGeometric *points, int arraySize, double accuracy)
{
	srand((unsigned int)time(NULL));

	std::vector <PointGeometric> pointsForCircleCenters;
	PlaneGeometric* plane = nullptr;
	std::vector <SphereApprox> spheres;
	SphereApprox sphere;

	int count = 0;

	for (int i = 0; i<arraySize; i++)
	{

		if (count < 3)
		{
			pointsForCircleCenters.push_back(points[i]);
			if (count == 2)
			{
				delete	plane;
				plane = new PlaneGeometric(pointsForCircleCenters[0], pointsForCircleCenters[1], pointsForCircleCenters[2]);
				//plane->Normalization();
			}
		}
		else
		{
			plane->Line.Point = pointsForCircleCenters[0];
			if (points[i].DistanceToPoint(plane->PointProjection(points[i])) < 0.1)
			{
				pointsForCircleCenters.push_back(points[i]);
			}
			else
			{
				sphere.FindByPoints(&pointsForCircleCenters[0], (int)pointsForCircleCenters.size(), accuracy);
				spheres.push_back(sphere);

				pointsForCircleCenters.clear();

				pointsForCircleCenters.push_back(points[i]);

				count = 0;
			}

		}
		count++;
	}

	sphere.FindByPoints(&pointsForCircleCenters[0], (int)pointsForCircleCenters.size(), accuracy);
	spheres.push_back(sphere);

	CenterByPoints(points, arraySize);				// Finde center point

	for (int i = 0; i < (int)spheres.size(); i++)
	{
		RadiusCircle += spheres[i].Radius;
	}
	RadiusCircle /= spheres.size();					// model radius in cross-section


	for (int i = 0; i < (int)spheres.size(); i++)
	{
		Radius += spheres[i].Line.Point.DistanceToPoint(Line.Point);
	}
	Radius /= spheres.size();						// model radius 


	int random = rand() % spheres.size();
	int randomOld;

	VectorGeometric *vector1 = nullptr,
					*vector2 = nullptr;

	delete vector1;
	vector1 = new VectorGeometric(Line.Point, points[random]);

	do
	{
		randomOld = random;
		random = rand() % spheres.size();
	} while (random == randomOld);

	delete vector2;
	vector2 = new VectorGeometric(Line.Point, points[random]);

	Line.Vector = *vector1 ^ *vector2;

	Line.Vector.Normalize();						// Vector Normalization

											///////////		Start Approximation

	double	globalDeviation = 0,
		globalDeviationOld = 0;


	double	*vectorMCoordinate1, *vectorMCoordinate2;

	// Find Max vector Element
	if ((fabs(Line.Vector.X) > fabs(Line.Vector.Y)) && (fabs(Line.Vector.X) > fabs(Line.Vector.Z)))
	{
		//Line.Vector.X;
		vectorMCoordinate1 = &Line.Vector.Y;
		vectorMCoordinate2 = &Line.Vector.Z;
	}
	else
	{
		if ((fabs(Line.Vector.Y) > fabs(Line.Vector.X)) && (fabs(Line.Vector.Y) > fabs(Line.Vector.Z)))
		{
			//Line.Vector.Y;
			vectorMCoordinate1 = &Line.Vector.X;
			vectorMCoordinate2 = &Line.Vector.Z;
		}
		else
		{
			//Line.Vector.Z;
			vectorMCoordinate1 = &Line.Vector.Y;
			vectorMCoordinate2 = &Line.Vector.X;
		}
	}

	globalDeviation = FunctionApprox(points, arraySize);

	do {
		globalDeviationOld = globalDeviation;

		////////////	Vector

		//Approximation(points, accuracy, &Line.Vector.X);	// Changing X - vector
		//Approximation(points, accuracy, &Line.Vector.Y);	// Changing Y - vector
		//Approximation(points, accuracy, &Line.Vector.Z);	// Changing Z - vector

		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate1);	// Changing 1 - vector
		Approximation(points, arraySize, accuracy, &Line.Vector, vectorMCoordinate2);	// Changing 2 - vector


		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.X);		// Changing X - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Y);		// Changing Y - center
		Approximation(points, arraySize, accuracy, &Line.Vector, &Line.Point.Z);		// Changing Z - center

		Approximation(points, arraySize, accuracy, &Line.Vector, &RadiusCircle);		// cross-section Radius 

		Approximation(points, arraySize, accuracy, &Line.Vector, &Radius);	// Radius

																					///////

		globalDeviation = FunctionApprox(points, arraySize);

	} while (fabs(globalDeviation - globalDeviationOld) > accuracy);

	Height = RadiusCircle * 2;

	delete	plane;
	delete	vector1;
	delete	vector2;
	pointsForCircleCenters.clear();
	spheres.clear();
}