#include "stdafx.h"
#include <Traekt.h>
#include <ApproximationCore\_ALLincludesCore.h>
#include <iostream>
#include <algorithm>

PointGeometric TransToGlobal(PointGeometric	point,
	PointGeometric	pointCenterLocal,
	VectorGeometric	vectorX,
	VectorGeometric	vectorY,
	VectorGeometric vectorZ)
{
	VectorGeometric vectorR(PointGeometric(0, 0, 0), pointCenterLocal, false);

	VectorGeometric vectorPoint = vectorR + vectorX * point.X + vectorY * point.Y + vectorZ * point.Z;

	return PointGeometric(vectorPoint.X, vectorPoint.Y, vectorPoint.Z);
}
UINT TraektCylinder(LPVOID Struct)		//  StTr *Struct
{ 
	StTr* struc = ((StTr*)Struct);
	CylinderApprox CylinderTraekt = *(CylinderApprox *)struc->TrGeom;

	int step = struc->TrStep;
	int n = struc->TrPl;
	bool inside = struc->TrBoss;
	bool part = struc->TrPart;
	CmmApprox* pCMM = struc->TrCmm;
	CircleApprox CircleTraekt;
	LineGeometric LineTraekt;
	LineTraekt.Point = CylinderTraekt.Line.CreatePointOnDistance(CylinderTraekt.Height / 2, false);
	LineTraekt.Vector = CylinderTraekt.Line.Vector;

	int PtDist = 10;
	int ToolSize = 10;
	short sh = 0;
	
	PointGeometric PointMach;
	pCMM->GetPoint_(&PointMach.X, &PointMach.Y, &PointMach.Z, &sh);
	
	int MinN = 0;
	double MinDist;
	CircleTraekt.Radius = CylinderTraekt.Radius;
	CircleTraekt.Line.Vector = CylinderTraekt.Line.Vector;
	//if (inside)
	//{
	//	PointGeometric inFirst;
	//	inFirst = CylinderTraekt.Line.CreatePointOnDistance(CylinderTraekt.Height + ToolSize, false);
	//	//Move_(&xMach, &yMach, &inFirst.Z, false);
	//	ForP1->Move_(&inFirst.X, &inFirst.Y, &inFirst.Z, false);
	//}

	PlaneGeometric PlaneTr;
	LineGeometric LinePart;
	PointGeometric PointPart;
	std::vector<PointGeometric> VPart;
	double PartDist;
	int Fpoint;
	int Lpoint;
	VectorGeometric vectorZ, vectorY, vectorX;
	double p;
	double h;
	double angle1, anglesSum;
	double angle;
	int forPntCnt = 0;
	LineGeometric tmpLine;
	PointGeometric tmpPoint;
	PointGeometric tmpPoint1;
	CircleGeometric CircleTr;
	LineGeometric xVec;
	PointGeometric xP;
	VectorGeometric yCoord;


	CircleTraekt.Line.Point = LineTraekt.CreatePointOnDistance(0);
	if (part)
	{
		PlaneTr.Line.Point = CircleTraekt.Line.Point;
		PlaneTr.Line.Vector = CircleTraekt.Line.Vector;
		for (int i = 0; i < (int)CylinderTraekt.PointsForApprox.size(); i++)
		{
			PointPart = PlaneTr.PointProjection(CylinderTraekt.PointsForApprox[i]);
			LinePart.Point = CircleTraekt.Line.Point;
			LinePart.Vector = VectorGeometric(CircleTraekt.Line.Point, PointPart);
			VPart.push_back(LinePart.CreatePointOnDistance(CircleTraekt.Radius));
		}
		PartDist = -1;

		for (int i = 0; i < (int)VPart.size(); i++)
		{
			for (int j = 0; j < (int)VPart.size(); j++)
			{
				if (PartDist < VPart[i].DistanceToPoint(VPart[j]))
				{
					PartDist = VPart[i].DistanceToPoint(VPart[j]);
					Fpoint = i;
					Lpoint = j;
				}
			}
		}

		vectorX = VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint]);
		vectorZ = CircleTraekt.Line.Vector;
		vectorY = vectorX ^ vectorZ;
		if (VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint + 1])*vectorY < 0)
		{
			vectorY = vectorY * (-1);
		}

		p = (CircleTraekt.Radius + CircleTraekt.Radius + VPart[Fpoint].DistanceToPoint(VPart[Lpoint])) / 2;
		h = 2 * sqrt(p*(p - CircleTraekt.Radius)*(p - CircleTraekt.Radius)*(p - VPart[Fpoint].DistanceToPoint(VPart[Lpoint]))) / VPart[Fpoint].DistanceToPoint(VPart[Lpoint]);

		if (VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) >= 2 * CircleTraekt.Radius)
		{
			angle1 = 180.0;
		}
		else
		{
			angle1 = 2 * (asin(VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) / (2 * CircleTraekt.Radius))*180.0f / PI_Approx);
		}
		angle = angle1/ (step - 1);
		
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// Y component		

			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			/*fprintf(dots, "%lf %lf %lf\n", CircleTraekt.Mesh.points[i].X, CircleTraekt.Mesh.points[i].Y, CircleTraekt.Mesh.points[i].Z);
			i++;*/
			forPntCnt++;
		}
		/*fclose(dots);*/
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
	}
	else
	{
		angle = 360.0 / step;
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// Y component		
			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			forPntCnt++;
		}
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[0]);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
		if (PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i])<MinDist)
		{
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i]);
		MinN = i;
		}
		}
		if (MinN != 0)
		{
		CircleTraekt.Mesh.points.insert(CircleTraekt.Mesh.points.end(), &CircleTraekt.Mesh.points[0], &CircleTraekt.Mesh.points[MinN - 1]);
		CircleTraekt.Mesh.points.erase(CircleTraekt.Mesh.points.begin(), CircleTraekt.Mesh.points.begin() + MinN - 1);
		}
	}

	LineGeometric lineOffset;
	lineOffset.Vector = CircleTraekt.Line.Vector;

	for (double StepSum = 0; StepSum <= CylinderTraekt.Height; StepSum += CylinderTraekt.Height / (n - 1))//для вала
	{
		CircleTraekt.Line.Point = LineTraekt.CreatePointOnDistance(StepSum);
		tmpLine.Point = CircleTraekt.Line.Point;
		tmpLine.Vector = VectorGeometric(CircleTraekt.Line.Point, CircleTraekt.Mesh.points[0]);
		if (inside)
		{
			tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius - PtDist);
		}
		else
		{
			tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius + PtDist);
		}
		pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
		pCMM->Move_(&CircleTraekt.Mesh.points[0].X, &CircleTraekt.Mesh.points[0].Y, &CircleTraekt.Mesh.points[0].Z, true);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
			tmpLine.Vector = VectorGeometric(CircleTraekt.Line.Point, CircleTraekt.Mesh.points[i]);
			tmpPoint1 = tmpPoint;
			

			CircleTr.Line.Point = CircleTraekt.Line.Point;
			CircleTr.Radius = CircleTraekt.Radius;
			if (/*CircleTr.LineIntersection = 0 || */inside)
			{
				tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius - PtDist);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			else
			{
				tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius + PtDist);
				double xPre = CircleTraekt.Line.Point.DistanceToPoint(tmpPoint1);

				xVec.Point = CircleTraekt.Line.Point;
				xVec.Vector = VectorGeometric(CircleTraekt.Line.Point, tmpPoint1);
				xP = xVec.PointProjection(tmpPoint);
				double yNew = xP.DistanceToPoint(tmpPoint);
				PointGeometric subPoint(xPre, yNew, 0);

				yCoord = xVec.Vector^CircleTraekt.Line.Vector;
				if (yCoord*(VectorGeometric(tmpPoint1, tmpPoint)) < 0)
				{
					yCoord = yCoord * (-1);
				}
				subPoint = TransToGlobal(subPoint, CircleTraekt.Line.Point, xVec.Vector, yCoord, CircleTraekt.Line.Vector);
				pCMM->Move_(&subPoint.X, &subPoint.Y, &subPoint.Z, false);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			pCMM->Move_(&CircleTraekt.Mesh.points[i].X, &CircleTraekt.Mesh.points[i].Y, &CircleTraekt.Mesh.points[i].Z, true);
		}
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			lineOffset.Point = CircleTraekt.Mesh.points[i];
			CircleTraekt.Mesh.points[i] = lineOffset.CreatePointOnDistance(CylinderTraekt.Height / (n - 1));
		}
		if (part)
		{
			std::reverse(std::begin(CircleTraekt.Mesh.points), std::end(CircleTraekt.Mesh.points));
		}
		
	}
	delete struc;
	return 0;
}
UINT TraektCone(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	ConeApprox ConeTraekt = *(ConeApprox *)struc->TrGeom;
	int step = struc->TrStep;
	int n = struc->TrPl;
	bool inside = struc->TrBoss;
	bool part = struc->TrPart;
	CmmApprox* pCMM = struc->TrCmm;
	CircleApprox CircleTraekt;
	LineGeometric LineTraekt;
	LineTraekt.Point = ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2, false);
	LineTraekt.Vector = ConeTraekt.Line.Vector;

	int PtDist = 10;
	int ToolSize = 10;
	
	short sh = 0;
	PointGeometric PointMach;
	pCMM->GetPoint_(&PointMach.X, &PointMach.Y, &PointMach.Z, &sh);
	
	int MinN = 0;
	double MinDist;
	CircleTraekt.Radius = ConeTraekt.Radius;
	CircleTraekt.Line.Vector = ConeTraekt.Line.Vector;

	PlaneGeometric PlaneTr;
	LineGeometric LinePart;
	PointGeometric PointPart;
	std::vector<PointGeometric> VPart;
	double PartDist;
	int Fpoint;
	int Lpoint;
	VectorGeometric vectorZ, vectorY, vectorX;
	double p;
	double h;
	double angle1, anglesSum;
	double angle;
	int forPntCnt = 0;
	LineGeometric tmpLine;
	PointGeometric tmpPoint;
	PointGeometric tmpPoint1;
	CircleGeometric CircleTr;
	LineGeometric xVec;
	PointGeometric xP;
	VectorGeometric yCoord;

	CircleTraekt.Line.Point = LineTraekt.CreatePointOnDistance(0);
	if (part)
	{
		PlaneTr.Line.Point = CircleTraekt.Line.Point;
		PlaneTr.Line.Vector = CircleTraekt.Line.Vector;
		for (int i = 0; i < (int)ConeTraekt.PointsForApprox.size(); i++)
		{
			PointPart = PlaneTr.PointProjection(ConeTraekt.PointsForApprox[i]);
			LinePart.Point = CircleTraekt.Line.Point;
			LinePart.Vector = VectorGeometric(CircleTraekt.Line.Point, PointPart);
			VPart.push_back(LinePart.CreatePointOnDistance(CircleTraekt.Radius));
		}
		PartDist = -1;

		for (int i = 0; i < (int)VPart.size(); i++)
		{
			for (int j = 0; j < (int)VPart.size(); j++)
			{
				if (PartDist < VPart[i].DistanceToPoint(VPart[j]))
				{
					PartDist = VPart[i].DistanceToPoint(VPart[j]);
					Fpoint = i;
					Lpoint = j;
				}
			}
		}

		vectorX = VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint]);
		vectorZ = CircleTraekt.Line.Vector;
		vectorY = vectorX ^ vectorZ;
		if (VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint + 1])*vectorY < 0)
		{
			vectorY = vectorY * (-1);
		}

		p = (CircleTraekt.Radius + CircleTraekt.Radius + VPart[Fpoint].DistanceToPoint(VPart[Lpoint])) / 2;
		h = 2 * sqrt(p*(p - CircleTraekt.Radius)*(p - CircleTraekt.Radius)*(p - VPart[Fpoint].DistanceToPoint(VPart[Lpoint]))) / VPart[Fpoint].DistanceToPoint(VPart[Lpoint]);

		if (VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) >= 2 * CircleTraekt.Radius)
		{
			angle1 = 180.0;
		}
		else
		{
			angle1 = 2 * (asin(VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) / (2 * CircleTraekt.Radius))*180.0f / PI_Approx);//(asin(h / CircleTraekt.Radius) * 180.0 / PI_Approx);
		}
		
		angle = angle1/ (step - 1);

		/*FILE* dots;
		dots = fopen("D:\\dots.txt", "wt");
		int i = 0;*/
		
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// Y component		

			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			/*fprintf(dots, "%lf %lf %lf\n", CircleTraekt.Mesh.points[i].X, CircleTraekt.Mesh.points[i].Y, CircleTraekt.Mesh.points[i].Z);
			i++;*/
			forPntCnt++;
		}
		/*fclose(dots);*/

		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
	}
	else
	{
		angle = 360.0 / step;
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// Y component		
			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			forPntCnt++;
		}
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[0]);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
		if (PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i])<MinDist)
		{
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i]);
		MinN = i;
		}
		}
		if (MinN != 0)
		{
		CircleTraekt.Mesh.points.insert(CircleTraekt.Mesh.points.end(), &CircleTraekt.Mesh.points[0], &CircleTraekt.Mesh.points[MinN - 1]);
		CircleTraekt.Mesh.points.erase(CircleTraekt.Mesh.points.begin(), CircleTraekt.Mesh.points.begin() + MinN - 1);
		}
	}

	
	double forR = 0;
	int forRev = 1;
	PointGeometric poSurf;
	LineGeometric lnSurf;
	LineGeometric lnSurf1;
	//poSurf = ConeTraekt.Line.CreatePointOnDistance(sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)- ConeTraekt.Height / 2);
	PlaneGeometric plSurf;
	plSurf.Line.Point = CircleTraekt.Line.Point;
	plSurf.Line.Vector = CircleTraekt.Line.Vector;

	for (double StepSum = 0; StepSum <= ConeTraekt.Height; StepSum += ConeTraekt.Height / (n - 1))//для вала
	{
		poSurf = ConeTraekt.Line.CreatePointOnDistance(StepSum + sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f) - ConeTraekt.Height / 2);
		lnSurf1.Point = CircleTraekt.Mesh.points[0];
		lnSurf1.Vector = plSurf.Line.Vector;//VectorGeometric(CircleTraekt.Mesh.points[0], plSurf.PointProjection(CircleTraekt.Mesh.points[0]));
		lnSurf.Point = poSurf;
		lnSurf.Vector = VectorGeometric(poSurf, lnSurf1.PointProjection(poSurf));
		poSurf = lnSurf.CreatePointOnDistance(sqrt(ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f) - sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)*sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)));
		CircleTraekt.Line.Point = LineTraekt.CreatePointOnDistance(StepSum);
		tmpLine.Point = CircleTraekt.Mesh.points[0];
		tmpLine.Vector = VectorGeometric(ConeTraekt.Line.CreatePointOnDistance((ConeTraekt.Height / 2) - StepSum, false), poSurf);
		if (inside)
		{
			tmpPoint = tmpLine.CreatePointOnDistance(PtDist,false);
		}
		else
		{
			tmpPoint = tmpLine.CreatePointOnDistance(PtDist);
		}
		pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
		pCMM->Move_(&CircleTraekt.Mesh.points[0].X, &CircleTraekt.Mesh.points[0].Y, &CircleTraekt.Mesh.points[0].Z, true);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
			tmpPoint1 = tmpPoint;
			poSurf = ConeTraekt.Line.CreatePointOnDistance(sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f) - ConeTraekt.Height / 2);
			lnSurf1.Point = CircleTraekt.Mesh.points[i];
			lnSurf1.Vector = plSurf.Line.Vector;
			lnSurf.Point = poSurf;
			lnSurf.Vector = VectorGeometric(poSurf, lnSurf1.PointProjection(poSurf));
			poSurf = lnSurf.CreatePointOnDistance(sqrt(ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f) - sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)*sin(ConeTraekt.Angle* PI_Approx / 180.0f)*ConeTraekt.Radius*sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f)));
			tmpLine.Point = CircleTraekt.Mesh.points[i];
			tmpLine.Vector = VectorGeometric(ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2,false), poSurf);
			

			CircleTr.Line.Point = CircleTraekt.Line.Point;
			CircleTr.Radius = CircleTraekt.Radius;
			if (/*CircleTr.LineIntersection = 0 || */inside)
			{
				tmpPoint = tmpLine.CreatePointOnDistance(PtDist, false);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			else
			{
				tmpPoint = tmpLine.CreatePointOnDistance(PtDist);
				//ForP1->GetPoint_(&CurX, &CurY, &CurZ, &sh);
				PlaneGeometric plSurf1(tmpPoint1, tmpPoint, tmpLine.CreatePointOnDistance(2*PtDist));
				double xPre = plSurf1.PointProjection(ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2, false)).DistanceToPoint(tmpPoint1);

				xVec.Point = plSurf1.PointProjection(ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2, false));
				xVec.Vector = VectorGeometric(plSurf1.PointProjection(ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2, false)), tmpPoint1);
				xP = xVec.PointProjection(tmpPoint);
				double yNew = xP.DistanceToPoint(tmpPoint);
				PointGeometric subPoint(xPre, yNew, 0);
				if (plSurf1.Line.Vector * ConeTraekt.Line.Vector < 0)
				{
					plSurf1.Line.Vector = plSurf1.Line.Vector * (-1);
				}
				yCoord = xVec.Vector^plSurf1.Line.Vector;
				if (yCoord*(VectorGeometric(tmpPoint1, tmpPoint)) < 0)
				{
					yCoord = yCoord * (-1);
				}
				subPoint = TransToGlobal(subPoint, plSurf1.PointProjection(ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.Height / 2, false)), xVec.Vector, yCoord, plSurf1.Line.Vector);
				pCMM->Move_(&subPoint.X, &subPoint.Y, &subPoint.Z, false);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			pCMM->Move_(&CircleTraekt.Mesh.points[i].X, &CircleTraekt.Mesh.points[i].Y, &CircleTraekt.Mesh.points[i].Z, true);
		}
		LineGeometric PointPusher;
		forR = ConeTraekt.Height / (n - 1);
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			PointPusher.Point = CircleTraekt.Mesh.points[i];
			PointPusher.Vector = VectorGeometric(CircleTraekt.Mesh.points[i],
				ConeTraekt.Line.CreatePointOnDistance(ConeTraekt.HeightPhantom - ConeTraekt.Height / 2));
			CircleTraekt.Mesh.points[i] = PointPusher.CreatePointOnDistance(forR/sin((90 - ConeTraekt.Angle)* PI_Approx / 180.0f));//false?
		}
		if (part)
		{
			std::reverse(std::begin(CircleTraekt.Mesh.points), std::end(CircleTraekt.Mesh.points));
		}
	}
	delete struc;
	return 0;
}
UINT TraektSphere(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	SphereApprox SphereTraekt = *(SphereApprox *)struc->TrGeom;
	int step = struc->TrStep;
	int n = struc->TrPl;
	bool inside = struc->TrBoss;
	bool part = struc->TrPart;
	CmmApprox* pCMM = struc->TrCmm;
	CircleGeometric CirTr(SphereTraekt.PointsForApprox[0], SphereTraekt.PointsForApprox[1], SphereTraekt.PointsForApprox[2]);
	PlaneGeometric FirstPl(SphereTraekt.PointsForApprox[0], SphereTraekt.PointsForApprox[1], SphereTraekt.PointsForApprox[2]);
	CirTr.Line.Vector = VectorGeometric(FirstPl.PointProjection(SphereTraekt.PointsForApprox[3]),SphereTraekt.PointsForApprox[3]);
	
	CircleApprox CircleTraekt;
	
	PointGeometric PointMach;
	short sh = 0;
	
	pCMM->GetPoint_(&PointMach.X, &PointMach.Y, &PointMach.Z, &sh);


	int MinN = 0;
	double MinDist;

	LineGeometric LineTraekt;
	LineTraekt.Point = CirTr.Line.CreatePointOnDistance(CirTr.Radius, false);
	LineTraekt.Vector = CirTr.Line.Vector;
	std::vector<PointGeometric> VPart;
	std::vector<PointGeometric> VPart1;
	for (int i = 0 ; i< (int)SphereTraekt.PointsForApprox.size(); i++)
	{
		VPart.push_back(LineTraekt.PointProjection(SphereTraekt.PointsForApprox[i]));
	}
	double PartDist = -1, PartDist1;
	int Fpoint, Lpoint, Fpoint1, Lpoint1;
	for (int i = 0; i < (int)VPart.size(); i++)
	{
		for (int j = 0; j < (int)VPart.size(); j++)
		{
			if (PartDist < VPart[i].DistanceToPoint(VPart[j]))
			{
				PartDist = VPart[i].DistanceToPoint(VPart[j]);
				Fpoint = i;
				Lpoint = j;
			}
		}
	}
	
	
	CircleTraekt.Line.Point = VPart[Fpoint];
	CircleTraekt.Line.Vector = CirTr.Line.Vector;
	CircleTraekt.Radius = sqrt(SphereTraekt.Radius*SphereTraekt.Radius - VPart[Fpoint].DistanceToPoint(SphereTraekt.Line.Point)*VPart[Fpoint].DistanceToPoint(SphereTraekt.Line.Point));

	VectorGeometric vectorZ, vectorY, vectorX;
	double p;
	double h;
	double angle1, anglesSum;
	double /*angleF, angleL,*/ angle;
	int forPntCnt = 0;
	PlaneGeometric PlaneTr;
	LineGeometric LinePart;
	if (part)
	{
		/*for (int i = 0; i < SphereTraekt.PointsForApprox.size(); i++)
		{
			VPart1.push_back(LineTraekt.PointProjection(SphereTraekt.PointsForApprox[i]));
		}*/
		PlaneTr.Line.Point = SphereTraekt.Line.Point;
		PlaneTr.Line.Vector = CirTr.Line.Vector;
		for (int i = 0; i < (int)SphereTraekt.PointsForApprox.size(); i++)
		{
			LinePart.Point = PlaneTr.Line.Point;
			LinePart.Vector = VectorGeometric(PlaneTr.Line.Point, PlaneTr.PointProjection(SphereTraekt.PointsForApprox[i]));
			VPart1.push_back(LinePart.CreatePointOnDistance(SphereTraekt.Radius));
		}
		PartDist1 = -1;
		for (int i = 0; i < (int)VPart1.size(); i++)
		{
			for (int j = 0; j < (int)VPart1.size(); j++)
			{
				if (PartDist1 < VPart1[i].DistanceToPoint(VPart1[j]))
				{
					PartDist1 = VPart1[i].DistanceToPoint(VPart1[j]);
					Fpoint1 = i;
					Lpoint1 = j;
				}
			}
		}


		vectorX = VectorGeometric(CircleTraekt.Line.Point, VPart1[Fpoint1]);
		vectorZ = CircleTraekt.Line.Vector;
		vectorY = vectorX ^ vectorZ;
		if (VectorGeometric(CircleTraekt.Line.Point, VPart1[Fpoint1 + 1])*vectorY < 0)
		{
			vectorY = vectorY * (-1);
		}


		p = (CircleTraekt.Radius + CircleTraekt.Radius + VPart1[Fpoint1].DistanceToPoint(VPart1[Lpoint1])) / 2;
		h = 2 * sqrt(p*(p - CircleTraekt.Radius)*(p - CircleTraekt.Radius)*(p - VPart1[Fpoint1].DistanceToPoint(VPart1[Lpoint1]))) / VPart1[Fpoint1].DistanceToPoint(VPart1[Lpoint1]);

		if (VPart1[Fpoint1].DistanceToPoint(VPart1[Lpoint1]) >= 2 * CircleTraekt.Radius)
		{
			angle1 = 180.0;
		}
		else
		{
			angle1 = 2 * (asin(VPart1[Fpoint1].DistanceToPoint(VPart1[Lpoint1]) / (2 * CircleTraekt.Radius))*180.0f / PI_Approx);//(asin(h / CircleTraekt.Radius) * 180.0 / PI_Approx);
		}
		
		angle = angle1/(step-1);//(angleL - angleF)/step;

																						/*FILE* dots;
																						dots = fopen("D:\\dots.txt", "wt");
																						int i = 0;*/
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// Y component		

			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			/*fprintf(dots, "%lf %lf %lf\n", CircleTraekt.Mesh.points[i].X, CircleTraekt.Mesh.points[i].Y, CircleTraekt.Mesh.points[i].Z);
			i++;*/
			forPntCnt++;
		}
		/*fclose(dots);*/



		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
	}
	else
	{
		angle = 360.0 / step;
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// Y component		
			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			forPntCnt++;
		}
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[0]);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
			if (PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i])<MinDist)
			{
				MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i]);
				MinN = i;
			}
		}
		if (MinN != 0)
		{
			CircleTraekt.Mesh.points.insert(CircleTraekt.Mesh.points.end(), &CircleTraekt.Mesh.points[0], &CircleTraekt.Mesh.points[MinN - 1]);
			CircleTraekt.Mesh.points.erase(CircleTraekt.Mesh.points.begin(), CircleTraekt.Mesh.points.begin() + MinN - 1);
		}
	}
	LineGeometric tmpLine;
	PointGeometric tmpPoint;
	PointGeometric tmpPoint1;
	CircleGeometric CircleTr;
	LineGeometric xVec;
	PointGeometric xP;
	VectorGeometric yCoord;
	
	int PtDist = 10;
	int ToolSize = 10;
	double forR = 0;
	PointGeometric PointPart;
	int forRev = 1;
	LineGeometric PointPusher;
	PointGeometric ptmp;
	for (double StepSum = 0; StepSum <= PartDist; StepSum += PartDist / (n - 1))//для вала
	{
		CircleTraekt.Line.Point = LineTraekt.CreatePointOnDistance(StepSum);
		tmpLine.Point = SphereTraekt.Line.Point;
		tmpLine.Vector = VectorGeometric(SphereTraekt.Line.Point, CircleTraekt.Mesh.points[0]);
		if (inside)
		{
			tmpPoint = tmpLine.CreatePointOnDistance(SphereTraekt.Radius - PtDist);
		}
		else
		{
			tmpPoint = tmpLine.CreatePointOnDistance(SphereTraekt.Radius + PtDist);
		}
		pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
		pCMM->Move_(&CircleTraekt.Mesh.points[0].X, &CircleTraekt.Mesh.points[0].Y, &CircleTraekt.Mesh.points[0].Z, true);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
			tmpLine.Vector = VectorGeometric(SphereTraekt.Line.Point, CircleTraekt.Mesh.points[i]);
			tmpPoint1 = tmpPoint;
			

			CircleTr.Line.Point = CircleTraekt.Line.Point;
			CircleTr.Radius = CircleTraekt.Radius;
			if (/*CircleTr.LineIntersection = 0 || */inside)
			{
				tmpPoint = tmpLine.CreatePointOnDistance(SphereTraekt.Radius - PtDist);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			else
			{
				tmpPoint = tmpLine.CreatePointOnDistance(SphereTraekt.Radius + PtDist);
				//ForP1->GetPoint_(&CurX, &CurY, &CurZ, &sh);
				double xPre = SphereTraekt.Line.Point.DistanceToPoint(tmpPoint1);

				xVec.Point = SphereTraekt.Line.Point;
				xVec.Vector = VectorGeometric(SphereTraekt.Line.Point, tmpPoint1);
				xP = xVec.PointProjection(tmpPoint);
				double yNew = xP.DistanceToPoint(tmpPoint);
				PointGeometric subPoint(xPre, yNew, 0);
				PlaneGeometric subPl(SphereTraekt.Line.Point,tmpPoint1, tmpPoint);
				yCoord = xVec.Vector^subPl.Line.Vector;
				if (yCoord*(VectorGeometric(tmpPoint1, tmpPoint)) < 0)
				{
					yCoord = yCoord * (-1);
				}
				subPoint = TransToGlobal(subPoint, SphereTraekt.Line.Point, xVec.Vector, yCoord, subPl.Line.Vector);
				pCMM->Move_(&subPoint.X, &subPoint.Y, &subPoint.Z, false);
				pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			}
			pCMM->Move_(&CircleTraekt.Mesh.points[i].X, &CircleTraekt.Mesh.points[i].Y, &CircleTraekt.Mesh.points[i].Z, true);
		}
		forR = PartDist/(n-1);
		
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			PointPusher.Point = CircleTraekt.Line.CreatePointOnDistance(forR);
			PointPusher.Vector = VectorGeometric(CircleTraekt.Line.Point, CircleTraekt.Mesh.points[i]);
			CircleTraekt.Mesh.points[i] = PointPusher.CreatePointOnDistance(sqrt(SphereTraekt.Radius*SphereTraekt.Radius - (VPart[Fpoint].DistanceToPoint(SphereTraekt.Line.Point) + forRev*forR)*(VPart[Fpoint].DistanceToPoint(SphereTraekt.Line.Point) + forRev * forR)));//false?
		}
		if (part)
		{
			std::reverse(std::begin(CircleTraekt.Mesh.points), std::end(CircleTraekt.Mesh.points));
		}
	}
	delete struc;
	return 0;
}
UINT TraektPoint(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	PointApprox PointTraekt = *(PointApprox *)struc->TrGeom;
	CmmApprox* pCMM = struc->TrCmm;
	
	pCMM->Move_(&PointTraekt.X, &PointTraekt.Y, &PointTraekt.Z, true);
	delete struc;
	return 0;
}
UINT TraektLine(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	LineSegmentApprox LineTraekt = *(LineSegmentApprox *)struc->TrGeom;
	int step = struc->TrStep;
	CmmApprox* pCMM = struc->TrCmm;
	
	LineGeometric LineF;
	std::vector<PointGeometric> PointF;
	int i = 0;
	LineF.Point = LineTraekt.PointStart;
	LineF.Vector = LineTraekt.Line.Vector;
	LineTraekt.Height = LineTraekt.PointStart.DistanceToPoint(LineTraekt.PointEnd);
	for (double StepSum = 0;i<step; StepSum += LineTraekt.Height/(step-1))
	{
		PointF.push_back(LineF.CreatePointOnDistance(StepSum));
		i++;
	}
	for (i = 0;i<(int)PointF.size();i++)
	{
		pCMM->Move_(&PointF[i].X, &PointF[i].Y, &PointF[i].Z, true);
	}
	delete struc;
	return 0;
}
UINT TraektRectangle(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	RectangleApprox RectangleTraekt = *(RectangleApprox *)struc->TrGeom;
	int step = struc->TrStep;
	int n = struc->TrPl;
	CmmApprox* pCMM = struc->TrCmm;
	
	LineGeometric LineX, LineY;
	std::vector<PointGeometric> PointF;
	LineGeometric tmpLine;
	PointGeometric tmpPoint;
	int i = 0;
	int j = 0;
	double PtDist = 10;
	/*LineX.Vector = RectangleTraekt.Line.Vector^RectangleTraekt.VectorX;
	LineX.Point = RectangleTraekt.Line.CreatePointOnDistance(RectangleTraekt.Height / 2, false);
	LineX.Vector = RectangleTraekt.VectorX;
	LineX.Point = LineX.CreatePointOnDistance(RectangleTraekt.Width, false);
	LineY.Point = LineX.Point;
	LineY.Vector = RectangleTraekt.Line.Vector^RectangleTraekt.VectorX;*/

	if (RectangleTraekt.Line.Vector*VectorGeometric(0, 0, 1) < 0)
	{
		RectangleTraekt.Line.Vector = RectangleTraekt.Line.Vector*(-1);
	}
	LineX.Point = RectangleTraekt.Line.Point;
	LineX.Vector = RectangleTraekt.VectorY;
	LineX.Point = LineX.CreatePointOnDistance(RectangleTraekt.Height / 2, false);
	LineX.Vector = RectangleTraekt.VectorX;
	LineX.Point = LineX.CreatePointOnDistance(RectangleTraekt.Width / 2, false);
	
	
	LineY.Vector = RectangleTraekt.VectorY;
	LineY.Point = LineX.Point;

	


	for (double StepY = 0;j<n; StepY+= RectangleTraekt.Height/(n-1))
	{
		LineX.Point = LineY.CreatePointOnDistance(StepY, true);
		for (double StepSum = 0; i < step; StepSum += RectangleTraekt.Width/(step-1))
		{
			PointF.push_back(LineX.CreatePointOnDistance(StepSum));
			i++;
		}
		for (i = 0; i < (int)PointF.size(); i++)
		{
			tmpLine.Point = PointF[i];
			tmpLine.Vector = RectangleTraekt.Line.Vector;
			tmpPoint = tmpLine.CreatePointOnDistance(PtDist);
			pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
			pCMM->Move_(&PointF[i].X, &PointF[i].Y, &PointF[i].Z, true);
		}
		PointF.clear();
		i = 0;
		j++;
	}
	delete struc;
	return 0;
}
UINT TraektCircle(LPVOID Struct)
{
	StTr* struc = ((StTr*)Struct);
	CircleApprox CircleTraekt = *(CircleApprox *)struc->TrGeom;
	int step = struc->TrStep;
	bool part = struc->TrPart;
	CmmApprox* pCMM = struc->TrCmm;
	int PtDist = 10;
	int ToolSize = 10;

	PointGeometric PointMach;
	short sh = 0;
	pCMM->GetPoint_(&PointMach.X, &PointMach.Y, &PointMach.Z, &sh);
	
	int MinN = 0;
	double MinDist;

	PlaneGeometric PlaneTr;
	LineGeometric LinePart;
	PointGeometric PointPart;
	std::vector<PointGeometric> VPart;
	double PartDist;
	int Fpoint;
	int Lpoint;
	VectorGeometric vectorZ, vectorY, vectorX;
	double p;
	double h;
	double angle1, anglesSum;
	double /*angleF, angleL,*/ angle;
	int forPntCnt = 0;
	LineGeometric tmpLine;
	PointGeometric tmpPoint;
	PointGeometric tmpPoint1;
	CircleGeometric CircleTr;
	LineGeometric xVec;
	PointGeometric xP;
	VectorGeometric yCoord;

	if (part)
	{
		PlaneTr.Line.Point = CircleTraekt.Line.Point;
		PlaneTr.Line.Vector = CircleTraekt.Line.Vector;
		for (int i = 0; i < (int)CircleTraekt.PointsForApprox.size(); i++)
		{
			PointPart = PlaneTr.PointProjection(CircleTraekt.PointsForApprox[i]);
			LinePart.Point = CircleTraekt.Line.Point;
			LinePart.Vector = VectorGeometric(CircleTraekt.Line.Point, PointPart);
			VPart.push_back(LinePart.CreatePointOnDistance(CircleTraekt.Radius));
		}
		PartDist = -1;

		for (int i = 0; i < (int)VPart.size(); i++)
		{
			for (int j = 0; j < (int)VPart.size(); j++)
			{
				if (PartDist < VPart[i].DistanceToPoint(VPart[j]))
				{
					PartDist = VPart[i].DistanceToPoint(VPart[j]);
					Fpoint = i;
					Lpoint = j;
				}
			}
		}

		vectorX = VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint]);
		vectorZ = CircleTraekt.Line.Vector;
		vectorY = vectorX ^ vectorZ;
		if (VectorGeometric(CircleTraekt.Line.Point, VPart[Fpoint + 1])*vectorY < 0)
		{
			vectorY = vectorY * (-1);
		}


		p = (CircleTraekt.Radius + CircleTraekt.Radius + VPart[Fpoint].DistanceToPoint(VPart[Lpoint])) / 2;
		h = 2 * sqrt(p*(p - CircleTraekt.Radius)*(p - CircleTraekt.Radius)*(p - VPart[Fpoint].DistanceToPoint(VPart[Lpoint]))) / VPart[Fpoint].DistanceToPoint(VPart[Lpoint]);

		if (VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) >= 2 * CircleTraekt.Radius)
		{
			angle1 = 180.0;
		}
		else
		{
			angle1 = 180.0 - 2 * (asin(VPart[Fpoint].DistanceToPoint(VPart[Lpoint]) / (2 * CircleTraekt.Radius))*180.0f / PI_Approx);//(asin(h / CircleTraekt.Radius) * 180.0 / PI_Approx);
		}
		
		angle = angle1/step;//(angleL - angleF)/step;

																						/*FILE* dots;
																						dots = fopen("D:\\dots.txt", "wt");
																						int i = 0;*/
		
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f)*CircleTraekt.Radius;	// Y component		

			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			/*fprintf(dots, "%lf %lf %lf\n", CircleTraekt.Mesh.points[i].X, CircleTraekt.Mesh.points[i].Y, CircleTraekt.Mesh.points[i].Z);
			i++;*/
			forPntCnt++;
		}
		/*fclose(dots);*/



		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
	}
	else
	{
		angle = 360.0 / step;
		CircleTraekt.Mesh.points.clear();
		for (anglesSum = 0.0f; forPntCnt < step; anglesSum += angle)
		{
			p = cos(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// X component
			h = sin(anglesSum * PI_Approx / 180.0f) * CircleTraekt.Radius;	// Y component		
			CircleTraekt.Mesh.points.push_back(PointGeometric(p, h, 0));
			forPntCnt++;
		}
		for (int i = 0; i < (int)CircleTraekt.Mesh.points.size(); i++)
		{
			CircleTraekt.Mesh.points[i] = TransToGlobal(CircleTraekt.Mesh.points[i],
				CircleTraekt.Line.Point,
				vectorX,
				vectorY,
				vectorZ);
		}
		CircleTraekt.Mesh.vectorsNormal.push_back(CircleTraekt.Line.Vector);
		MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[0]);
		for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
		{
			if (PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i])<MinDist)
			{
				MinDist = PointMach.DistanceToPoint(CircleTraekt.Mesh.points[i]);
				MinN = i;
			}
		}
		if (MinN != 0)
		{
			CircleTraekt.Mesh.points.insert(CircleTraekt.Mesh.points.end(), &CircleTraekt.Mesh.points[0], &CircleTraekt.Mesh.points[MinN - 1]);
			CircleTraekt.Mesh.points.erase(CircleTraekt.Mesh.points.begin(), CircleTraekt.Mesh.points.begin() + MinN - 1);
		}
	}

	LineGeometric lineOffset;
	lineOffset.Vector = CircleTraekt.Line.Vector;

	
		
	tmpLine.Point = CircleTraekt.Line.Point;
	tmpLine.Vector = VectorGeometric(CircleTraekt.Line.Point, CircleTraekt.Mesh.points[0]);
	tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius + PtDist);
	pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
	pCMM->Move_(&CircleTraekt.Mesh.points[0].X, &CircleTraekt.Mesh.points[0].Y, &CircleTraekt.Mesh.points[0].Z, true);
	for (int i = 1; i<(int)CircleTraekt.Mesh.points.size(); i++)
	{
		tmpLine.Vector = VectorGeometric(CircleTraekt.Line.Point, CircleTraekt.Mesh.points[i]);
		tmpPoint1 = tmpPoint;
		tmpPoint = tmpLine.CreatePointOnDistance(CircleTraekt.Radius + PtDist);

		double xPre = CircleTraekt.Line.Point.DistanceToPoint(tmpPoint1);

		xVec.Point = CircleTraekt.Line.Point;
		xVec.Vector = VectorGeometric(CircleTraekt.Line.Point, tmpPoint1);
		xP = xVec.PointProjection(tmpPoint);
		double yNew = xP.DistanceToPoint(tmpPoint);
		PointGeometric subPoint(xPre, yNew, 0);

		yCoord = xVec.Vector^CircleTraekt.Line.Vector;
		if (yCoord*(VectorGeometric(tmpPoint1, tmpPoint)) < 0)
		{
			yCoord = yCoord * (-1);
		}
		subPoint = TransToGlobal(subPoint, CircleTraekt.Line.Point, xVec.Vector, yCoord, CircleTraekt.Line.Vector);
		pCMM->Move_(&subPoint.X, &subPoint.Y, &subPoint.Z, false);
		pCMM->Move_(&tmpPoint.X, &tmpPoint.Y, &tmpPoint.Z, false);
		pCMM->Move_(&CircleTraekt.Mesh.points[i].X, &CircleTraekt.Mesh.points[i].Y, &CircleTraekt.Mesh.points[i].Z, true);
	}
	//CircleTraekt.Mesh.points.clear();
	delete struc;
	return 0;
}


