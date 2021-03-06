// DialogObjectProperties.cpp : implementation file
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DialogObjectProperties.h"
#include "afxdialogex.h"

#include <fstream>
#include <string>

// DialogObjectProperties dialog

IMPLEMENT_DYNAMIC(DialogObjectProperties, CDialog)

DialogObjectProperties::DialogObjectProperties(CWnd* pParent /*=nullptr*/, ObjectApprox *obj)
	: CDialog(IDD_DIALOG_OBJECT_PROPERTIES, pParent)
{
	object	= obj;
	pView	= (CMFCKioView*)pParent;
	pCMM	= theApp.pCMM;
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
	DDX_Control(pDX, IDC_COMBO_COMPENSATION_OPTION, m_vCompensationOption);
}


BEGIN_MESSAGE_MAP(DialogObjectProperties, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &DialogObjectProperties::OnBnClickedSaveData)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPENSATION_OPTION, &DialogObjectProperties::OnSelchangeComboCompensationOption)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// DialogObjectProperties message handlers



BOOL DialogObjectProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (object == nullptr)
		return FALSE;

	UpdateDataFields();

	CString str;		// std::wstring 	str;

	// Scanned Points;
	
	ListOfPoints.InsertColumn(0, _T("N"), LVCFMT_LEFT, 30);
	ListOfPoints.InsertColumn(1, _T("X"), LVCFMT_LEFT, 100);
	ListOfPoints.InsertColumn(2, _T("Y"), LVCFMT_LEFT, 100);
	ListOfPoints.InsertColumn(3, _T("Z"), LVCFMT_LEFT, 100);

	int nIndex; 

	for (int i = 0; i < (int)object->objMath->PointsForApprox.size(); i++)
	{
		str.Format(L"%d", i);
		nIndex = ListOfPoints.InsertItem(0, str);

		str.Format(L"%g", RoundingOf(object->objMath->PointsForApprox[i].X, theApp.CoordinatesAccuracy));
		ListOfPoints.SetItemText(nIndex, 1, str);

		str.Format(L"%g", RoundingOf(object->objMath->PointsForApprox[i].Y, theApp.CoordinatesAccuracy));
		ListOfPoints.SetItemText(nIndex, 2, str);
		
		str.Format(L"%g", RoundingOf(object->objMath->PointsForApprox[i].Z, theApp.CoordinatesAccuracy));
		ListOfPoints.SetItemText(nIndex, 3, str);
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
	std::wstring str = object->Name;
	str.append(L".txt");

	std::ofstream file(str);

	if (!file.is_open())
		return;

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
// ---																					// On CompensationOption Changing
void DialogObjectProperties::OnSelchangeComboCompensationOption()
{
	CString selectedCompensationOption;

	m_vCompensationOption.GetLBText(m_vCompensationOption.GetCurSel(), selectedCompensationOption);

	if (wcscmp(object->CompensationOption, selectedCompensationOption) != 0)
	{
		object->UpdateCompensationOption((wchar_t*)selectedCompensationOption.GetString(), pCMM->CompensationValue / 2);

		UpdateDataFields();
	}
}
// ---																					// On Destroy dialog
void DialogObjectProperties::OnDestroy()
{
	object->objMath->Triangulation(theApp.TriangulationINAccuracy);

	pView->RecalculateCenterOfAllObjects();
	pView->Invalidate(FALSE);

	CDialog::OnDestroy();
}


//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

// ---																					// Update Data Fields
void DialogObjectProperties::UpdateDataFields()
{
	if (object == nullptr)
		return;
	
	CString	str;
	
	char*	objectName = object->objMath->GetName();

	m_vCompensationOption.ResetContent();

	//	---																		// point
	if (objectName == PointApprox().GetName())
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

		
		str.Format(L"Точка - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		PointApprox* pointA = (PointApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(pointA->X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);

		str.Format(L"%g", RoundingOf(pointA->Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);

		str.Format(L"%g", RoundingOf(pointA->Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

	}
	//	---																		// lineSegment
	if (objectName == LineSegmentApprox().GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);	//
		Length_height.ShowWindow(SW_HIDE);	//
		
		str.Format(L"Линия - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		LineSegmentApprox* lineSegmentA = (LineSegmentApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", lineSegmentA->Point.X, theApp.CoordinatesAccuracy);
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", lineSegmentA->Point.Y, theApp.CoordinatesAccuracy);
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", lineSegmentA->Point.Z, theApp.CoordinatesAccuracy);
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Center Vector
		str.Format(L"%g", RoundingOf(lineSegmentA->Vector.X, theApp.CoordinatesAccuracy));
		Vector_x.SetWindowTextW(str);

		str.Format(L"%g", RoundingOf(lineSegmentA->Vector.Y, theApp.CoordinatesAccuracy));
		Vector_y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(lineSegmentA->Vector.Z, theApp.CoordinatesAccuracy));
		Vector_z.SetWindowTextW(str);

		//	----								---- Length
		str.Format(L"%g", RoundingOf(lineSegmentA->PointStart.DistanceToPoint(lineSegmentA->PointEnd), theApp.CoordinatesAccuracy));
		Length_height.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

	}
	//	---																		// rectangle
	if (objectName == RectangleApprox().GetName())
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

		str.Format(L"Плоскость - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		RectangleApprox* rectangleA = (RectangleApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(rectangleA->Line.Point.X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(rectangleA->Line.Point.Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(rectangleA->Line.Point.Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Center Vector
		str.Format(L"%g", RoundingOf(rectangleA->Line.Vector.X, theApp.CoordinatesAccuracy));
		Vector_x.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(rectangleA->Line.Vector.Y, theApp.CoordinatesAccuracy));
		Vector_y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(rectangleA->Line.Vector.Z, theApp.CoordinatesAccuracy));
		Vector_z.SetWindowTextW(str);

		//	----								---- Height
		str.Format(L"%g", RoundingOf(rectangleA->Height, theApp.CoordinatesAccuracy));
		Length_height.SetWindowTextW(str);

		//	----								---- Width
		str.Format(L"%g", RoundingOf(rectangleA->Width, theApp.CoordinatesAccuracy));
		Width_diameter.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

	}
	//	---																		// circle
	if (objectName == CircleApprox().GetName())
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

		str.Format(L"Окружность - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		CircleApprox* circleA = (CircleApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(circleA->Line.Point.X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(circleA->Line.Point.Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(circleA->Line.Point.Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Center Vector
		str.Format(L"%g", RoundingOf(circleA->Line.Vector.X, theApp.CoordinatesAccuracy));
		Vector_x.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(circleA->Line.Vector.Y, theApp.CoordinatesAccuracy));
		Vector_y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(circleA->Line.Vector.Z, theApp.CoordinatesAccuracy));
		Vector_z.SetWindowTextW(str);

		//	----								---- Diameter
		str.Format(L"%g", RoundingOf(circleA->Radius * 2, theApp.CoordinatesAccuracy));
		Width_diameter.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

	}
	//	---																		// cylinder
	if (objectName == CylinderApprox().GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Angle_text.ShowWindow(SW_HIDE);
		Angle_size.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);

		str.Format(L"Цилиндр - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		CylinderApprox* cylinderA = (CylinderApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(cylinderA->Line.Point.X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(cylinderA->Line.Point.Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(cylinderA->Line.Point.Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Center Vector
		str.Format(L"%g", RoundingOf(cylinderA->Line.Vector.X, theApp.CoordinatesAccuracy));
		Vector_x.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(cylinderA->Line.Vector.Y, theApp.CoordinatesAccuracy));
		Vector_y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(cylinderA->Line.Vector.Z, theApp.CoordinatesAccuracy));
		Vector_z.SetWindowTextW(str);

		//	----								---- Diameter
		str.Format(L"%g", RoundingOf(cylinderA->Radius * 2, theApp.CoordinatesAccuracy));
		Width_diameter.SetWindowTextW(str);

		//	----								---- Height
		str.Format(L"%g", RoundingOf(cylinderA->Height, theApp.CoordinatesAccuracy));
		Length_height.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

	}
	//	---																		// cone
	if (objectName == ConeApprox().GetName())
	{
		Width_text.ShowWindow(SW_HIDE);
		Length_text.ShowWindow(SW_HIDE);
		Length_height.ShowWindow(SW_HIDE);
		Diameter_text.ShowWindow(SW_HIDE);
		Width_diameter.ShowWindow(SW_HIDE);
		Height_text.ShowWindow(SW_HIDE);
		
		str.Format(L"Конус - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		ConeApprox* coneA = (ConeApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(coneA->Line.Point.X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(coneA->Line.Point.Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(coneA->Line.Point.Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Center Vector
		str.Format(L"%g", RoundingOf(coneA->Line.Vector.X, theApp.CoordinatesAccuracy));
		Vector_x.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(coneA->Line.Vector.Y, theApp.CoordinatesAccuracy));
		Vector_y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(coneA->Line.Vector.Z, theApp.CoordinatesAccuracy));
		Vector_z.SetWindowTextW(str);

		//	----								---- Angle
		str.Format(L"%g", RoundingOf(coneA->Angle, theApp.CornersAccuracy));
		Angle_size.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

	}
	//	---																		// sphere
	if (objectName == SphereApprox().GetName())
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

		str.Format(L"Сфера - %s", object->Name.c_str());
		SetDlgItemTextW(IDC_DIALOG_OBJ_NAME, str);

		SphereApprox* sphereA = (SphereApprox*)object->objMath;

		//	----								---- Center
		str.Format(L"%g", RoundingOf(sphereA->Line.Point.X, theApp.CoordinatesAccuracy));
		vcCenterPoint_X.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(sphereA->Line.Point.Y, theApp.CoordinatesAccuracy));
		vcCenterPoint_Y.SetWindowTextW(str);
		
		str.Format(L"%g", RoundingOf(sphereA->Line.Point.Z, theApp.CoordinatesAccuracy));
		vcCenterPoint_Z.SetWindowTextW(str);

		//	----								---- Diameter
		str.Format(L"%g", RoundingOf(sphereA->Radius * 2, theApp.CoordinatesAccuracy));
		Width_diameter.SetWindowTextW(str);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

	}

	m_vCompensationOption.SetCurSel(m_vCompensationOption.FindStringExact(0, object->CompensationOption));	
}
