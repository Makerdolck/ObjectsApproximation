// DialogObjectProperties.cpp : implementation file
//

#include "stdafx.h"
#include "MFCAscon.h"
#include "DialogObjectProperties.h"
#include "afxdialogex.h"

#include <fstream>
#include <string>

// DialogObjectProperties dialog

IMPLEMENT_DYNAMIC(DialogObjectProperties, CDialog)

DialogObjectProperties::DialogObjectProperties(CWnd* pParent /*=nullptr*/, ObjectApprox *obj)
	: CDialog(IDD_DIALOG_OBJECT_PROPERTIES, pParent)
{
	object = obj;

	pCMM = theApp.pCMM;
}

DialogObjectProperties::~DialogObjectProperties()
{
}

void DialogObjectProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_VECTOR, Vector_text);
	DDX_Control(pDX, IDC_EDIT_VECT_X, Vector_x);
	DDX_Control(pDX, IDC_EDIT_VECT_Y, Vector_y);
	DDX_Control(pDX, IDC_EDIT_VECT_Z, Vector_z);
	DDX_Control(pDX, IDC_TEXT_WIDTH, Width_text);
	DDX_Control(pDX, IDC_EDIT_WIDTH_DIAMETER, Width_diameter);
	DDX_Control(pDX, IDC_TEXT_DIAMETER, Diameter_text);
	DDX_Control(pDX, IDC_TEXT_LENGTH, Length_text);
	DDX_Control(pDX, IDC_EDIT_LENGTH_HEIGHT, Length_height);
	DDX_Control(pDX, IDC_TEXT_HEIGHT, Height_text);
	DDX_Control(pDX, IDC_TEXT_ANGLE, Angle_text);
	DDX_Control(pDX, IDC_EDIT_ANGLE, Angle_size);
	DDX_Control(pDX, IDC_LIST_POINTS, ListOfPoints);
	DDX_Control(pDX, IDC_EDIT_COOR_X, vcCenterPoint_X);
	DDX_Control(pDX, IDC_EDIT_COOR_Y, vcCenterPoint_Y);
	DDX_Control(pDX, IDC_EDIT_COOR_Z, vcCenterPoint_Z);
	DDX_Control(pDX, IDC_BUTTON_SAVE_DATA, butSaveData);
}


BEGIN_MESSAGE_MAP(DialogObjectProperties, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &DialogObjectProperties::OnBnClickedSaveData)
END_MESSAGE_MAP()

// DialogObjectProperties message handlers





BOOL DialogObjectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (object == nullptr)
		return FALSE;

	//NumStyle = theApp.MTActive;

	std::wstring 	str;

	PointApprox				*pointA			= new PointApprox();
	LineSegmentApprox		*lineSegmentA	= new LineSegmentApprox();
	RectangleApprox			*rectangleA		= new RectangleApprox();
	CircleApprox			*circleA		= new CircleApprox();
	CylinderApprox			*cylinderA		= new CylinderApprox();
	ConeApprox				*coneA			= new ConeApprox();
	SphereApprox			*sphereA		= new SphereApprox();


	char		*objectName = object->objMath->GetName();

	
	//	---													// point
	if (objectName == pointA->GetName())
	{
		Vector_text.ShowWindow(SW_HIDE);
		Vector_x.ShowWindow(SW_HIDE);
		Vector_y.ShowWindow(SW_HIDE);
		Vector_z.ShowWindow(SW_HIDE);
		Width_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		ListOfPoints.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LIST_BORDER)->ShowWindow(SW_HIDE);
		str = L"Точка - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete pointA;
		pointA = (PointApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(RoundingOf(pointA->X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(RoundingOf(pointA->Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(RoundingOf(pointA->Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());

	}
	//	---													// lineSegment
	if (objectName == lineSegmentA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);	//
		Length_height.ShowWindow(SW_HIDE);	//
		str = L"Линия - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());
		
		delete lineSegmentA;
		lineSegmentA = (LineSegmentApprox*)object->objMath;
		//	----								---- Center
		str = L""; 
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Vector.X, pCMM->MachineAccuracy)));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Vector.Y, pCMM->MachineAccuracy)));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->Line.Vector.Z, pCMM->MachineAccuracy)));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Length
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(lineSegmentA->PointStart.DistanceToPoint(lineSegmentA->PointEnd), pCMM->MachineAccuracy)));
		Length_height.SetWindowTextW(str.c_str());

	}
	//	---													// rectangle
	if (objectName == rectangleA->GetName())
	{
		Diameter_text.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		Width_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		str = L"Плоскость - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete rectangleA;
		rectangleA = (RectangleApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Vector.X, pCMM->MachineAccuracy)));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Vector.Y, pCMM->MachineAccuracy)));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Line.Vector.Z, pCMM->MachineAccuracy)));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Height
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Height, pCMM->MachineAccuracy)));
		Length_height.SetWindowTextW(str.c_str());
		//	----								---- Width
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(rectangleA->Width, pCMM->MachineAccuracy)));
		Width_diameter.SetWindowTextW(str.c_str());

	}
	//	---													// circle
	if (objectName == circleA->GetName())
	{
		Vector_text.ShowWindow(SW_HIDE);
		Vector_x.ShowWindow(SW_HIDE);
		Vector_y.ShowWindow(SW_HIDE);
		Vector_z.ShowWindow(SW_HIDE);
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		str = L"Окружность - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete circleA;
		circleA = (CircleApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Vector.X, pCMM->MachineAccuracy)));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Vector.Y, pCMM->MachineAccuracy)));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Line.Vector.Z, pCMM->MachineAccuracy)));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Diameter
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(circleA->Radius * 2, pCMM->MachineAccuracy)));
		Width_diameter.SetWindowTextW(str.c_str());

	}
	//	---													// cylinder
	if (objectName == cylinderA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		str = L"Цилиндр - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete cylinderA;
		cylinderA = (CylinderApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Vector.X, pCMM->MachineAccuracy)));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Vector.Y, pCMM->MachineAccuracy)));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Line.Vector.Z, pCMM->MachineAccuracy)));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Diameter
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Radius * 2, pCMM->MachineAccuracy)));
		Width_diameter.SetWindowTextW(str.c_str());
		//	----								---- Height
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(cylinderA->Height, pCMM->MachineAccuracy)));
		Length_height.SetWindowTextW(str.c_str());

	}
	//	---													// cone
	if (objectName == coneA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		str = L"Конус - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete coneA;
		coneA = (ConeApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Vector.X, pCMM->MachineAccuracy)));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Vector.Y, pCMM->MachineAccuracy)));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Line.Vector.Z, pCMM->MachineAccuracy)));
		Vector_z.SetWindowTextW(str.c_str());
		////	----								---- Diameter
		//str = L"";
		//str.append(std::to_wstring(RoundingOf(, pCMM->MachineAccuracy)coneA->Radius*2));
		//Width_diameter.SetWindowTextW(str.c_str());
		////	----								---- Height
		//str = L"";
		//str.append(std::to_wstring(RoundingOf(, pCMM->MachineAccuracy)coneA->Height));
		//Length_height.SetWindowTextW(str.c_str());
		//	----								---- Angle
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(coneA->Angle, pCMM->MachineAccuracy)));
		Angle_size.SetWindowTextW(str.c_str());
	}
	//	---													// sphere
	if (objectName == sphereA->GetName())
	{
		Vector_text.ShowWindow(SW_HIDE);
		Vector_x.ShowWindow(SW_HIDE);
		Vector_y.ShowWindow(SW_HIDE);
		Vector_z.ShowWindow(SW_HIDE);
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		str = L"Сфера - ";
		str.append(object->Name);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str.c_str());

		delete sphereA;
		sphereA = (SphereApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(sphereA->Line.Point.X, pCMM->MachineAccuracy)));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(sphereA->Line.Point.Y, pCMM->MachineAccuracy)));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(sphereA->Line.Point.Z, pCMM->MachineAccuracy)));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Diameter
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(sphereA->Radius * 2, pCMM->MachineAccuracy)));
		Width_diameter.SetWindowTextW(str.c_str());

	}

	//TODO Build Points;

	ListOfPoints.InsertColumn(0, _T("N"), LVCFMT_LEFT, 30);
	ListOfPoints.InsertColumn(1, _T("X"), LVCFMT_LEFT, 100);
	ListOfPoints.InsertColumn(2, _T("Y"), LVCFMT_LEFT, 100);
	ListOfPoints.InsertColumn(3, _T("Z"), LVCFMT_LEFT, 100);

	int nIndex; 
	//nIndex = ListOfPoints.InsertItem(0, _T("1"));
	//ListOfPoints.SetItemText(nIndex, 1, _T("1"));
	//ListOfPoints.SetItemText(nIndex, 2, _T("2"));
	//ListOfPoints.SetItemText(nIndex, 3, _T("3"));

	//nIndex = ListOfPoints.InsertItem(1, _T("2"));
	//ListOfPoints.SetItemText(nIndex, 1, _T("4"));
	//ListOfPoints.SetItemText(nIndex, 2, _T("5"));
	//ListOfPoints.SetItemText(nIndex, 3, _T("6"));

	//nIndex = ListOfPoints.InsertItem(2, _T("3"));
	//ListOfPoints.SetItemText(nIndex, 1, _T("7"));
	//ListOfPoints.SetItemText(nIndex, 2, _T("8"));
	//ListOfPoints.SetItemText(nIndex, 3, _T("9"));

	for (int i = 0; i < (int)object->objMath->PointsForApprox.size(); i++)
	{
		str = L"";
		str.append(std::to_wstring(i));
		nIndex = ListOfPoints.InsertItem(0, str.c_str());

		str = L"";
		str.append(std::to_wstring(
			RoundingOf(object->objMath->PointsForApprox[i].X, pCMM->MachineAccuracy)));
		ListOfPoints.SetItemText(nIndex, 1, str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(object->objMath->PointsForApprox[i].Y, pCMM->MachineAccuracy)));
		ListOfPoints.SetItemText(nIndex, 2, str.c_str());
		str = L"";
		str.append(std::to_wstring(
			RoundingOf(object->objMath->PointsForApprox[i].Z, pCMM->MachineAccuracy)));
		ListOfPoints.SetItemText(nIndex, 3, str.c_str());
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}
//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																					// On Save Data To File
void DialogObjectProperties::OnBnClickedSaveData()
{
	//char		*objectName = object->objMath->GetName();

	std::ofstream file("data.txt");

	for (int i = 0; i < (int)object->objMath->PointsForApprox.size(); i++)
	{
		if (i != 0)
			file << "\n";
		file << object->objMath->PointsForApprox[i].X << "\t";
		file << object->objMath->PointsForApprox[i].Y << "\t";
		file << object->objMath->PointsForApprox[i].Z ;
	}

	file.close();
}