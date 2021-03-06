// DialogCreate.cpp : implementation file
//

#include "stdafx.h"
#include "MFCAscon.h"
#include "DialogCreate.h"
#include "afxdialogex.h"


// DialogCreate dialog

IMPLEMENT_DYNAMIC(DialogCreate, CDialog)

DialogCreate::DialogCreate(CWnd* pParent /*=nullptr*/, ObjectApproxC3D *obj)
	: CDialog(IDD_DIALOG_CREATE, pParent)
{
	object = obj;
}

DialogCreate::~DialogCreate()
{
}

void DialogCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_VECTOR, Vector_text);
	DDX_Control(pDX, IDC_EDIT_VECT_X, Vector_x);
	DDX_Control(pDX, IDC_EDIT_VECT_Y, Vector_y);
	DDX_Control(pDX, IDC_EDIT_VECT_Z, Vector_z);
	DDX_Control(pDX, IDC_TEXT_WIDTH, Width_text);
	DDX_Control(pDX, IDC_EDIT_WIDTH_RADIUS, Width_radius);
	DDX_Control(pDX, IDC_TEXT_RADIUS, Radius_text);
	DDX_Control(pDX, IDC_TEXT_LENGTH, Length_text);
	DDX_Control(pDX, IDC_EDIT_LENGTH_HEIGHT, Length_height);
	DDX_Control(pDX, IDC_TEXT_HEIGHT, Height_text);
	DDX_Control(pDX, IDC_TEXT_ANGLE, Angle_text);
	DDX_Control(pDX, IDC_EDIT_ANGLE, Angle_size);
	DDX_Control(pDX, IDC_LIST_POINTS, ListOfPoints);
	DDX_Control(pDX, IDC_EDIT_COOR_X, vcCenterPoint_X);
	DDX_Control(pDX, IDC_EDIT_COOR_Y, vcCenterPoint_Y);
	DDX_Control(pDX, IDC_EDIT_COOR_Z, vcCenterPoint_Z);
}


BEGIN_MESSAGE_MAP(DialogCreate, CDialog)
END_MESSAGE_MAP()


// DialogCreate message handlers





BOOL DialogCreate::OnInitDialog()
{
	CDialog::OnInitDialog();

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
		Width_radius.ShowWindow(SW_HIDE);
		Radius_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		ListOfPoints.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LIST_BORDER)->ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Точка");

		delete pointA;
		pointA = (PointApprox*)object->objMath;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(pointA->X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(pointA->Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(pointA->Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());

	}
	//	---													// lineSegment
	if (objectName == lineSegmentA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Width_radius.ShowWindow(SW_HIDE);
		Radius_text.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		//Length_text.ShowWindow(SW_HIDE);	//
		//Length_height.ShowWindow(SW_HIDE);	//
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Линия");
		
		delete lineSegmentA;
		lineSegmentA = (LineSegmentApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Vector.X));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Vector.Y));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(lineSegmentA->Line.Vector.Z));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Length
		str = L"";
		str.append(std::to_wstring(lineSegmentA->PointStart.DistanceToPoint(lineSegmentA->PointEnd)));
		Length_height.SetWindowTextW(str.c_str());

	}
	//	---													// rectangle
	if (objectName == rectangleA->GetName())
	{
		Radius_text.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Плоскость");

		delete rectangleA;
		rectangleA = (RectangleApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Vector.X));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Vector.Y));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(rectangleA->Line.Vector.Z));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Height
		str = L"";
		str.append(std::to_wstring(rectangleA->Height));
		Length_height.SetWindowTextW(str.c_str());
		//	----								---- Width
		str = L"";
		str.append(std::to_wstring(rectangleA->Width));
		Width_radius.SetWindowTextW(str.c_str());

	}
	//	---													// circle
	if (objectName == circleA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Окружность");

		delete circleA;
		circleA = (CircleApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(circleA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(circleA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(circleA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(circleA->Line.Vector.X));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(circleA->Line.Vector.Y));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(circleA->Line.Vector.Z));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Radius
		str = L"";
		str.append(std::to_wstring(circleA->Radius));
		Width_radius.SetWindowTextW(str.c_str());

	}
	//	---													// cylinder
	if (objectName == cylinderA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Цилиндр");

		delete cylinderA;
		cylinderA = (CylinderApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Vector.X));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Vector.Y));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(cylinderA->Line.Vector.Z));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Radius
		str = L"";
		str.append(std::to_wstring(cylinderA->Radius));
		Width_radius.SetWindowTextW(str.c_str());
		//	----								---- Height
		str = L"";
		str.append(std::to_wstring(cylinderA->Height));
		Length_height.SetWindowTextW(str.c_str());

	}
	//	---													// cone
	if (objectName == coneA->GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Конус");

		delete coneA;
		coneA = (ConeApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(coneA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(coneA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(coneA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Center Vector
		str = L"";
		str.append(std::to_wstring(coneA->Line.Vector.X));
		Vector_x.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(coneA->Line.Vector.Y));
		Vector_y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(coneA->Line.Vector.Z));
		Vector_z.SetWindowTextW(str.c_str());
		//	----								---- Radius
		str = L"";
		str.append(std::to_wstring(coneA->Radius));
		Width_radius.SetWindowTextW(str.c_str());
		//	----								---- Height
		str = L"";
		str.append(std::to_wstring(coneA->Height));
		Length_height.SetWindowTextW(str.c_str());
		//	----								---- Angle
		str = L"";
		str.append(std::to_wstring(coneA->Angle));
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
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, L"Сфера");

		delete sphereA;
		sphereA = (SphereApprox*)object;
		//	----								---- Center
		str = L"";
		str.append(std::to_wstring(sphereA->Line.Point.X));
		vcCenterPoint_X.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(sphereA->Line.Point.Y));
		vcCenterPoint_Y.SetWindowTextW(str.c_str());
		str = L"";
		str.append(std::to_wstring(sphereA->Line.Point.Z));
		vcCenterPoint_Z.SetWindowTextW(str.c_str());
		//	----								---- Radius
		str = L"";
		str.append(std::to_wstring(sphereA->Radius));
		Width_radius.SetWindowTextW(str.c_str());

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

	for (int i = 0; i < object->objMath->PointsForApprox.size(); i++)
	{
		str = L"";
		str.append(std::to_wstring(i));
		nIndex = ListOfPoints.InsertItem(0, str.c_str());

		str = L"";
		str.append(std::to_wstring(object->objMath->PointsForApprox[i].X));
		ListOfPoints.SetItemText(nIndex, 1, str.c_str());
		str = L"";
		str.append(std::to_wstring(object->objMath->PointsForApprox[i].Y));
		ListOfPoints.SetItemText(nIndex, 2, str.c_str());
		str = L"";
		str.append(std::to_wstring(object->objMath->PointsForApprox[i].Z));
		ListOfPoints.SetItemText(nIndex, 3, str.c_str());
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}
