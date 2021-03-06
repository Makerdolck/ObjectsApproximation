// DialogMeasure.cpp : implementation file
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DialogMeasure.h"
#include "afxdialogex.h"
#include "Traekt.h"


// DialogMeasure dialog

IMPLEMENT_DYNAMIC(DialogMeasure, CDialog)

DialogMeasure::DialogMeasure(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MEASURE, pParent)
	, m_vObjName(_T(""))
{
	pCMM			= theApp.pCMM;
	NumStyle		= &theApp.MTActive;
	nextObjectID	= &theApp.nextObjectID;
	AccuracyApprox	= theApp.AccuracyApprox;

	pView					= nullptr;
	objectsArray			= nullptr;
	PointsScanned			= 0;
	PointsAmountForApprox	= 0;
	nPos_old				= 0;
	flagNewElement			= true;
}

DialogMeasure::~DialogMeasure()
{
	//
}

void DialogMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEASURE_NAME, MeasureName);
	DDX_Text(pDX, IDC_EDIT_OBJECT_NAME, m_vObjName);
	DDV_MaxChars(pDX, m_vObjName, 255);
	DDX_Control(pDX, IDC_COMBO_COMPENSATION_OPTION, m_vCompensationOption);
}


BEGIN_MESSAGE_MAP(DialogMeasure, CDialog)
	ON_MESSAGE(WM_POINT_APPROX_READY,			&DialogMeasure::OnNewPointForApprox)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POINT,		&DialogMeasure::OnBnClickedButtonDeletePoint)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POINTS,		&DialogMeasure::OnBnClickedButtonDeletePoints)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &DialogMeasure::OnBnClickedOk)
END_MESSAGE_MAP()


// DialogMeasure message handlers


// ---																					// On Init Dialog
BOOL DialogMeasure::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	//CFont font;
	//font.CreateFont(640, // 320 - same
	//	10, // int nWidth, 
	//	0, 0, 400, FALSE, FALSE,
	//	0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	//	DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,
	//	L"Times New Roman");

	//MeasureName.SetFont(&font, 1);

	switch (*NumStyle)
	{
	case theApp.MTpoint:													// point
		MeasureName.SetWindowTextW(L"Точка");

		m_vObjName.Format(L"Точка_%d", pView->NameNumb_Point);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		
		//GetDlgItem(IDC_BUTTON_DELETE_POINT)->EnableWindow(false);

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"1");
		PointsAmountForApprox = PointsAmount_point;

		//pSpin->EnableWindow(false);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

		m_vCompensationOption.SetCurSel(0);

		break;
	case theApp.MTlineSegment:											// line
		MeasureName.SetWindowTextW(L"Линия");
		
		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"2");
		PointsAmountForApprox = PointsAmount_lineSegment;

		m_vObjName.Format(L"Линия_%d", pView->NameNumb_LineSegment);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

		m_vCompensationOption.SetCurSel(0);

		break;
	case theApp.MTplane:													// plane
		MeasureName.SetWindowTextW(L"Плоскость");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsAmount_plane;

		m_vObjName.Format(L"Плоскость_%d", pView->NameNumb_Plane);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveX);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveY);
		m_vCompensationOption.AddString(CompensationOptionNames.PositiveZ);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeX);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeY);
		m_vCompensationOption.AddString(CompensationOptionNames.NegativeZ);

		m_vCompensationOption.SetCurSel(0);

		break;
	case theApp.MTcircle:													// circle
		MeasureName.SetWindowTextW(L"Окружность");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsAmount_circle;

		m_vObjName.Format(L"Окружность_%d", pView->NameNumb_Circle);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

		m_vCompensationOption.SetCurSel(0);

		break;	
	case theApp.MTcylinder:												// cylinder
		MeasureName.SetWindowTextW(L"Цилиндр");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"4");
		PointsAmountForApprox = PointsAmount_cylinder;

		m_vObjName.Format(L"Цилиндр_%d", pView->NameNumb_Cylinder);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//rbIsCylinder.ShowWindow(SW_SHOW);
		//rbIsHole.ShowWindow(SW_SHOW);

		//rbIsCylinder.SetCheck(1);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

		m_vCompensationOption.SetCurSel(0);

		break;
	case theApp.MTcone:													// cone
		MeasureName.SetWindowTextW(L"Конус");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"4");
		PointsAmountForApprox = PointsAmount_cone;

		m_vObjName.Format(L"Конус_%d", pView->NameNumb_Cone);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

		m_vCompensationOption.SetCurSel(0);

		break;
	case theApp.MTsphere:													// sphere
		MeasureName.SetWindowTextW(L"Сфера");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsAmount_sphere;

		m_vObjName.Format(L"Сфера_%d", pView->NameNumb_Sphere);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		//	---	Compensation

		m_vCompensationOption.AddString(CompensationOptionNames.No);
		m_vCompensationOption.AddString(CompensationOptionNames.Shaft);
		m_vCompensationOption.AddString(CompensationOptionNames.Hole);

		m_vCompensationOption.SetCurSel(0);

		break;
	default:
		break;
	}

	//	---	---	---	---	---	---	---	---	---	---	---

	PointsScanned = 0;

	_UpdatePointsDataField();

	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_POINT_NUMBER);
	pSpin->SetRange(0, SHORT_MAX);
	pSpin->SetPos((int)(PointsAmountForApprox));
	nPos_old = (int)PointsAmountForApprox;

	//	---	---	---	---	---	---	---	---	---	---	---
	
	pCMM->PointsForApprox.clear();

	flagNewElement	= true;
	

	SetTimer(0, 0, NULL);

	if (bAuto == true)
	{
		((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_POINT_NUMBER))->SetPos((int)(0));
		nPos_old = 0;
		PointsAmountForApprox = 0;
		_UpdatePointsDataField();		
		OnTraekt();
	}

	return TRUE; 
}

//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																					// On Click OK and destroy dialog
void DialogMeasure::OnBnClickedOk()
{
	KillTimer(0);
	
	bool flagVisualize = true;

	switch (*NumStyle)
	{
	case theApp.MTpoint:						// point
		if (PointsScanned == 0)
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTlineSegment:				// lineSegment
		if ((PointsScanned == 0) || (PointsScanned == 1))
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTplane:						// rectangle
		if (PointsScanned >= 0 && PointsScanned < PointsAmount_plane)
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTcircle:						// circle
		if (PointsScanned >= 0 && PointsScanned < PointsAmount_circle)
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTcylinder:					// cylinder
		if (PointsScanned >= 0 && PointsScanned < PointsAmount_cylinder)
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTcone:						// cone
		if (PointsScanned >= 0 && PointsScanned < PointsAmount_cone)
		{
			flagVisualize = false;
		}
		break;
	case theApp.MTsphere:						// sphere
		if (PointsScanned >= 0 && PointsScanned < PointsAmount_sphere)
		{
			flagVisualize = false;
		}
		break;
	default:
		break;
	}


	if (flagVisualize)
	{
		_SendObjectToVisualize();
		PointsScanned = 0;

		if (bAuto)
			pView->RemoveObject(objAp);
	}
	else
	{
		if (!objectsArray->empty())
		{
			if (objectsArray->back()->flagReady == false)
				objectsArray->pop_back();
		}
	}

	bAuto = false;

	CDialog::OnOK();
}
// ---																					// On Destroy Dialog
void DialogMeasure::OnDestroy()
{
	KillTimer(0);	

	if (!objectsArray->empty())
	{
		if (objectsArray->back()->flagReady == false)
			objectsArray->pop_back();
	}

	bAuto = false;

	CDialog::OnDestroy();
}
// ---																					// On Delete Last Point
void DialogMeasure::OnBnClickedButtonDeletePoint()
{
	if (PointsScanned <= 0)
		return;

	objectsArray->back()->objMath;
	GeomObjectApprox *obj = objectsArray->back()->objMath;;

	obj->PointsForApprox.pop_back();

	PointsScanned--;

	_UpdatePointsDataField();
}
// ---																					// On Delete All Points
void DialogMeasure::OnBnClickedButtonDeletePoints()
{
	if (PointsScanned <= 0)
		return;

	GeomObjectApprox *obj = objectsArray->back()->objMath;

	obj->PointsForApprox.clear();

	PointsScanned = 0;

	_UpdatePointsDataField();
}
// ---																					// DBL Click for __Test__
void DialogMeasure::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	return;

	// TODO: Add your message handler code here and/or call default

	std::ifstream					file;

	std::vector <PointGeometric>	points;

	PointGeometric					pointMy;

	double							accuracy			= 0.00001;

	ObjectApprox*					newObjectApproxC3D	= new ObjectApprox();


	switch (*NumStyle)
	{
	case theApp.MTpoint:
	{
		PointApprox* pointApprox = new PointApprox();

		file.open("dataSphere.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				pointMy.X -= 10;
				pointMy.Y -= 10;
				pointMy.Z -= 10;
				pointApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			pointApprox->FindByPoints(&pointApprox->PointsForApprox[0], (int)pointApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = pointApprox;
		}

		break;
	}
	case theApp.MTlineSegment:
	{
		LineSegmentApprox* lineSegmentApprox = new LineSegmentApprox();

		file.open("dataLine.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				lineSegmentApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			lineSegmentApprox->FindByPoints(&lineSegmentApprox->PointsForApprox[0], (int)lineSegmentApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = lineSegmentApprox;
		}
		break;
	}
	case theApp.MTplane:
	{
		RectangleApprox* rectangleApprox = new RectangleApprox();

		file.open("dataPlane.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				rectangleApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			rectangleApprox->FindByPoints(&rectangleApprox->PointsForApprox[0], (int)rectangleApprox->PointsForApprox.size(), accuracy);

			rectangleApprox->Triangulation();

			newObjectApproxC3D->objMath = rectangleApprox;
		}

		break;
	}
	case theApp.MTcircle:
	{
		CircleApprox* circleApprox = new CircleApprox();

		file.open("dataSphere.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				pointMy.X += 10;
				pointMy.Y += 10;
				pointMy.Z += 10;
				circleApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			circleApprox->FindByPoints(&circleApprox->PointsForApprox[0], (int)circleApprox->PointsForApprox.size(), accuracy);
			circleApprox->Triangulation(theApp.TriangulationINAccuracy);
			newObjectApproxC3D->objMath = circleApprox;
		}
		break;
	}
	case theApp.MTcylinder:
	{
		CylinderApprox* cylinderApprox = new CylinderApprox();

		file.open("dataCylinder.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				cylinderApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			//cylinderApprox->IsHole = isHole;

			cylinderApprox->FindByPoints(&cylinderApprox->PointsForApprox[0], (int)cylinderApprox->PointsForApprox.size(), accuracy);

			cylinderApprox->Triangulation(theApp.TriangulationINAccuracy);

			newObjectApproxC3D->objMath = cylinderApprox;
		}
		break;
	}
	case theApp.MTcone:
	{
		ConeApprox* coneApprox = new ConeApprox();

		file.open("dataCone.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				coneApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			coneApprox->FindByPoints(&coneApprox->PointsForApprox[0], (int)coneApprox->PointsForApprox.size(), accuracy);

			//coneApprox->IsHole = true;

			coneApprox->Triangulation(theApp.TriangulationINAccuracy);

			newObjectApproxC3D->objMath = coneApprox;
		}

		break;
	}
	case theApp.MTsphere:
	{
		SphereApprox* sphereApprox = new SphereApprox();

		file.open("dataSphere.txt");
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				sphereApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			sphereApprox->FindByPoints(&sphereApprox->PointsForApprox[0], (int)sphereApprox->PointsForApprox.size(), accuracy);

			sphereApprox->Triangulation(theApp.TriangulationINAccuracy);

			newObjectApproxC3D->objMath = sphereApprox;
		}


		break;
	}
	default:
		break;
	}

	file.close();
	points.clear();

	objectsArray->push_back(newObjectApproxC3D);

	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;

	UpdateData(true);	// Update variable with object name

	objectsArray->back()->flagReady = true;
	objectsArray->back()->Name = m_vObjName.GetString();

	pView->SendMessageW(WM_MATH_OBJECT_READY);


	CDialog::OnLButtonDblClk(nFlags, point);
}
// ---																					// MiddleButton Click for __Test__
void DialogMeasure::OnMButtonDown(UINT nFlags, CPoint point)
{
	return;
	// TODO: Add your message handler code here and/or call default

	std::ifstream					file;

	std::vector <PointGeometric>	points;
	PointGeometric					pointMy;
	double							accuracy = 0.00001;
	ObjectApprox* newObjectApproxC3D = new ObjectApprox();

	std::wstring fileName;

	UpdateData(true);	// Update variable with object name

	fileName = m_vObjName.GetString();

	switch (*NumStyle)
	{
	case theApp.MTpoint:
	{
		PointApprox* pointApprox = new PointApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				pointMy.X -= 10;
				pointMy.Y -= 10;
				pointMy.Z -= 10;
				pointApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			pointApprox->FindByPoints(&pointApprox->PointsForApprox[0], (int)pointApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = pointApprox;
		}
		break;
	}
	case theApp.MTlineSegment:
	{
		LineSegmentApprox* lineSegmentApprox = new LineSegmentApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				lineSegmentApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			lineSegmentApprox->FindByPoints(&lineSegmentApprox->PointsForApprox[0], (int)lineSegmentApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = lineSegmentApprox;
		}
		break;
	}
	case theApp.MTplane:
	{
		RectangleApprox* rectangleApprox = new RectangleApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				rectangleApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			rectangleApprox->FindByPoints(&rectangleApprox->PointsForApprox[0], (int)rectangleApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = rectangleApprox;
		}

		break;
	}
	case theApp.MTcircle:
	{
		//CircleApprox* circleApprox = new CircleApprox();
		ObjectApprox* newObjectApprox	= nullptr;
		CircleApprox* circleApprox		= nullptr;

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				/*pointMy.X += 10;
				pointMy.Y += 10;
				pointMy.Z += 10;*/
				circleApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);



				/*newObjectApprox = new ObjectApprox();
				circleApprox = new CircleApprox();

				file >> circleApprox->Line.Point.X >> circleApprox->Line.Point.Y >> circleApprox->Line.Point.Z;
				file >> circleApprox->Line.Vector.X >> circleApprox->Line.Vector.Y >> circleApprox->Line.Vector.Z;
				file >> circleApprox->Radius;

				circleApprox->Triangulation();

				newObjectApprox->objMath = circleApprox;
				objectsArray->push_back(newObjectApprox);

				objectsArray->back()->objID = *nextObjectID;
				*nextObjectID += 1;

				objectsArray->back()->flagReady = true;
				objectsArray->back()->Name = m_vObjName.GetString();*/
			}

			circleApprox->FindByPoints(&circleApprox->PointsForApprox[0], (int)circleApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = circleApprox;
		}
		break;
	}
	case theApp.MTcylinder:
	{
		CylinderApprox* cylinderApprox = new CylinderApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				cylinderApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			cylinderApprox->FindByPoints(&cylinderApprox->PointsForApprox[0], (int)cylinderApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = cylinderApprox;
		}
		break;
	}
	case theApp.MTcone:
	{
		ConeApprox* coneApprox = new ConeApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				coneApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			coneApprox->FindByPoints(&coneApprox->PointsForApprox[0], (int)coneApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = coneApprox;
		}

		break;
	}
	case theApp.MTsphere:
	{
		SphereApprox* sphereApprox = new SphereApprox();

		file.open(fileName);
		if (file.is_open())
		{
			while (!file.eof())					// Reading File "data.txt"
			{
				file >> pointMy.X >> pointMy.Y >> pointMy.Z;
				sphereApprox->PointsForApprox.push_back(pointMy);
				//points.Add(point);
			}

			sphereApprox->FindByPoints(&sphereApprox->PointsForApprox[0], (int)sphereApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = sphereApprox;
		}


		break;
	}
	default:
		break;
	}

	file.close();
	points.clear();

	objectsArray->push_back(newObjectApproxC3D);

	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID += 1;


	objectsArray->back()->flagReady = true;
	objectsArray->back()->Name = m_vObjName.GetString();

	pView->SendMessageW(WM_MATH_OBJECT_READY);


	CDialog::OnMButtonDown(nFlags, point);
}

//--------------------------------------------------------------
//	----	Message Handlers	---		Timer
//--------------------------------------------------------------

// ---																					// Timer (Show Current Coordinates)
void DialogMeasure::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	CString str = _T("");

	// X
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.X, theApp.CoordinatesAccuracy));
	SetDlgItemText(IDC_EDIT_X, str);
	// Y
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.Y, theApp.CoordinatesAccuracy));
	SetDlgItemText(IDC_EDIT_Y, str);
	// Z
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.Z, theApp.CoordinatesAccuracy));
	SetDlgItemText(IDC_EDIT_Z, str);

	//	---	CylinderHole status updating

	//if ((*NumStyle != theApp.MTcylinder) || (objectsArray->size() == 0))
	//	return ;

	//((CylinderApprox*)objectsArray->back()->objMath)->IsHole = isHole;
	
}

//--------------------------------------------------------------
//	----	Message Handlers	- Messages for Data Changing
//--------------------------------------------------------------

// ---																					// On Change Num of Points
void DialogMeasure::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode == SB_ENDSCROLL) 
		return;
	
	if (pScrollBar->GetDlgCtrlID() == IDC_SPIN_POINT_NUMBER) 
	{
		//UpdateData(TRUE);

		/*CString strValue;
		strValue.Format(L"%d / %d", PointsScanned, nPos);
		((CSpinButtonCtrl*)pScrollBar)->GetBuddy()->SetWindowText(strValue);*/

		if ((int)nPos < PointsScanned)
		{
			if (nPos_old < nPos)
				nPos = PointsScanned;
			else
				nPos = 0;
		}

		//	---	---	---	---	---	---	---	---	---	---	---

		switch (*NumStyle)
		{
		case theApp.MTpoint:						// point
			break;
		case theApp.MTlineSegment:				// lineSegment
			if (nPos == 1)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_lineSegment;
				else
					nPos = 0;
			}
			break;
		case theApp.MTplane:					// rectangle
			if (nPos >= 1 && nPos < PointsAmount_plane)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_plane;
				else
					nPos = 0;
			}
			break;
		case theApp.MTcircle:						// circle
			if (nPos >= 1 && nPos < PointsAmount_circle)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_circle;
				else
					nPos = 0;
			}
			break;
		case theApp.MTcylinder:					// cylinder
			if (nPos >= 1 && nPos < PointsAmount_cylinder)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_cylinder;
				else
					nPos = 0;
			}
			break;
		case theApp.MTcone:						// cone
			if (nPos >= 1 && nPos < PointsAmount_cone)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_cone;
				else
					nPos = 0;
			}
			break;
		case theApp.MTsphere:						// sphere
			if (nPos >= 1 && nPos < PointsAmount_sphere)
			{
				if (nPos_old < nPos)
					nPos = PointsAmount_sphere;
				else
					nPos = 0;
			}
			break;
		default:
			break;
		}

		nPos_old				= nPos;
		PointsAmountForApprox	= nPos;

		((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_POINT_NUMBER))->SetPos((int)(nPos));


		_UpdatePointsDataField();
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//--------------------------------------------------------------
//	----	Message Handlers		----	Custom Messages
//--------------------------------------------------------------

// ---																					// On New Point for Approx is coming
LRESULT DialogMeasure::OnNewPointForApprox(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	if ((PointsScanned >= PointsAmountForApprox) && PointsAmountForApprox != 0)
	{
		_SendObjectToVisualize();

		switch (*NumStyle)
		{
		case theApp.MTpoint:													// point
			m_vObjName.Format(L"Точка_%d", pView->NameNumb_Point);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTlineSegment:											// line
			m_vObjName.Format(L"Линия_%d", pView->NameNumb_LineSegment);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTplane:													// plane
			m_vObjName.Format(L"Плоскость_%d", pView->NameNumb_Plane);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTcircle:													// circle
			m_vObjName.Format(L"Окружность_%d", pView->NameNumb_Circle);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTcylinder:												// cylinder
			m_vObjName.Format(L"Цилиндр_%d", pView->NameNumb_Cylinder);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTcone:													// cone
			m_vObjName.Format(L"Конус_%d", pView->NameNumb_Cone);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTsphere:													// sphere
			m_vObjName.Format(L"Сфера_%d", pView->NameNumb_Sphere);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		default:
			break;
		}

		flagNewElement = true;
	}

	//	---	---	---	---	---	---	---	---	---	---	---
	
	GeomObjectApprox *obj = nullptr;
	

	if (flagNewElement)
	{
		switch (*NumStyle)
		{
		case theApp.MTpoint:
			obj = new PointApprox();
			break;
		case theApp.MTlineSegment:
			obj = new LineSegmentApprox();
			break;
		case theApp.MTplane:
			obj = new RectangleApprox();
			break;
		case theApp.MTcircle:
			obj = new CircleApprox();
			break;
		case theApp.MTcylinder:
			obj = new CylinderApprox();
			break;
		case theApp.MTcone:
			obj = new ConeApprox();
			break;
		case theApp.MTsphere:
			obj = new SphereApprox();
			break;
		default:
			break;
		}

		flagNewElement			= false;
		PointsScanned			= 0;

		ObjectApprox *newObjectApprox = new ObjectApprox();
		newObjectApprox->objMath = obj;
		objectsArray->push_back(newObjectApprox);
	}
	else
		obj = objectsArray->back()->objMath;

	//	---	---	---	---	---	---	---	---	---	---	---

	PointGeometric point = pCMM->PointsForApprox.back();
	obj->PointsForApprox.push_back(point);
	pCMM->PointsForApprox.pop_back();

	//	---	---	---	---	---	---	---	---	---	---	---

	PointsScanned++;

	_UpdatePointsDataField();	 

	return 0L;
}
// ---																					// _ Update Points Data Field
void DialogMeasure::_UpdatePointsDataField()
{
	CString	str;
	str.Format(L"%d / %d", PointsScanned, PointsAmountForApprox);
	SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, str);
}
// ---																					// _ Send Object To Visualize 
void DialogMeasure::_SendObjectToVisualize()
{
	UpdateData(true);	// Update variables

	GeomObjectApprox *obj = objectsArray->back()->objMath;

	VectorGeometric tmpVector;

	CString selectedCompensationOption;

	m_vCompensationOption.GetLBText(m_vCompensationOption.GetCurSel(), selectedCompensationOption);

	((ObjectApprox*)objectsArray->back())->UpdateCompensationOption((wchar_t*)selectedCompensationOption.GetString(), pCMM->CompensationValue / 2);

	switch (*NumStyle)
	{
	case theApp.MTpoint:
		((PointApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);
		
		if (selectedCompensationOption == CompensationOptionNames.PositiveX)					// X +
			((PointApprox*)obj)->X += pCMM->CompensationValue / 2;
		else if (selectedCompensationOption == CompensationOptionNames.PositiveY)				// Y +
			((PointApprox*)obj)->Y += pCMM->CompensationValue / 2;
		else if (selectedCompensationOption == CompensationOptionNames.PositiveZ)				// Z +
			((PointApprox*)obj)->Z += pCMM->CompensationValue / 2;
		else if (selectedCompensationOption == CompensationOptionNames.NegativeX)				// X -
			((PointApprox*)obj)->X -= pCMM->CompensationValue / 2;
		else if (selectedCompensationOption == CompensationOptionNames.NegativeY)				// Y -
			((PointApprox*)obj)->Y -= pCMM->CompensationValue / 2;
		else if (selectedCompensationOption == CompensationOptionNames.NegativeZ)				// Z -
			((PointApprox*)obj)->Z -= pCMM->CompensationValue / 2;

		break;
	case theApp.MTlineSegment:
		((LineSegmentApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);
		
		if (selectedCompensationOption == CompensationOptionNames.PositiveX)					// X +
		{
			((LineSegmentApprox*)obj)->Point.X		+= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.X += pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.X	+= pCMM->CompensationValue / 2;
		}
		else if (selectedCompensationOption == CompensationOptionNames.PositiveY)				// Y +
		{
			((LineSegmentApprox*)obj)->Point.Y		+= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.Y += pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.Y	+= pCMM->CompensationValue / 2;
		}
		else if (selectedCompensationOption == CompensationOptionNames.PositiveZ)				// Z +
		{
			((LineSegmentApprox*)obj)->Point.Z		+= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.Z += pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.Z	+= pCMM->CompensationValue / 2;
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeX)				// X -
		{
			((LineSegmentApprox*)obj)->Point.X		-= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.X -= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.X	-= pCMM->CompensationValue / 2;
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeY)				// Y -
		{
			((LineSegmentApprox*)obj)->Point.Y		-= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.Y -= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.Y	-= pCMM->CompensationValue / 2;
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeZ)				// Z -
		{
			((LineSegmentApprox*)obj)->Point.Z		-= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointStart.Z -= pCMM->CompensationValue / 2;
			((LineSegmentApprox*)obj)->PointEnd.Z	-= pCMM->CompensationValue / 2;
		}
		
		break;
	case theApp.MTplane:
		((RectangleApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);

		if (selectedCompensationOption == CompensationOptionNames.PositiveX)					// X +
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
		}
		else if (selectedCompensationOption == CompensationOptionNames.PositiveY)				// Y +
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
		}
		else if (selectedCompensationOption == CompensationOptionNames.PositiveZ)				// Z +
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeX)				// X -
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeY)				// Y -
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
		}
		else if (selectedCompensationOption == CompensationOptionNames.NegativeZ)				// Z -
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if ((tmpVector * ((RectangleApprox*)obj)->Line.Vector) > 0)
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2, false);
			else
				((RectangleApprox*)obj)->Line.Point = ((RectangleApprox*)obj)->Line.CreatePointOnDistance(pCMM->CompensationValue / 2);
		}

		((RectangleApprox*)obj)->Triangulation(theApp.TriangulationINAccuracy);

		break;
	case theApp.MTcircle:
		((CircleApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);

		if (selectedCompensationOption == CompensationOptionNames.Hole)
			((CircleApprox*)obj)->Radius += pCMM->CompensationValue;
		else if (selectedCompensationOption == CompensationOptionNames.Shaft)
			((CircleApprox*)obj)->Radius -= pCMM->CompensationValue;

		((CircleApprox*)obj)->Triangulation(theApp.TriangulationINAccuracy);

		break;
	case theApp.MTcylinder:
		((CylinderApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);
		
		if (selectedCompensationOption == CompensationOptionNames.Hole)
		{
			((CylinderApprox*)obj)->IsHole = true;
			((CylinderApprox*)obj)->Radius += pCMM->CompensationValue;
		}
		else if (selectedCompensationOption == CompensationOptionNames.Shaft)
		{
			((CylinderApprox*)obj)->IsHole = false;
			((CylinderApprox*)obj)->Radius -= pCMM->CompensationValue;
		}
		
		((CylinderApprox*)obj)->Triangulation(theApp.TriangulationINAccuracy);

		break;
	case theApp.MTcone:
		((ConeApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);
		
		if (selectedCompensationOption == CompensationOptionNames.Hole)
		{
			((ConeApprox*)obj)->IsHole = true;
			((ConeApprox*)obj)->Radius			+= pCMM->CompensationValue;
			((ConeApprox*)obj)->RadiusSmaller	+= pCMM->CompensationValue;
		}
		else if (selectedCompensationOption == CompensationOptionNames.Shaft)
		{
			((ConeApprox*)obj)->IsHole = false;
			((ConeApprox*)obj)->Radius			-= pCMM->CompensationValue;
			((ConeApprox*)obj)->RadiusSmaller	-= pCMM->CompensationValue;
		}

		((ConeApprox*)obj)->Triangulation(theApp.TriangulationINAccuracy);
		
		break;
	case theApp.MTsphere:
		((SphereApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), AccuracyApprox);
		
		if (selectedCompensationOption == CompensationOptionNames.Hole)
		{
			((SphereApprox*)obj)->IsHole = true;
			((SphereApprox*)obj)->Radius += pCMM->CompensationValue;
		}
		else if (selectedCompensationOption == CompensationOptionNames.Shaft)
		{
			((SphereApprox*)obj)->IsHole = false;
			((SphereApprox*)obj)->Radius -= pCMM->CompensationValue;
		}
		
		((SphereApprox*)obj)->Triangulation(theApp.TriangulationINAccuracy);

		break;
	default:
		break;
	}

	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID +=1;

	

	objectsArray->back()->flagReady	= true;
	objectsArray->back()->Name		= m_vObjName.GetString();
	//pView->PostMessageW(WM_MATH_OBJECT_READY);
	//pView->OnNewObjectForVisualize();
	pView->SendMessageW(WM_MATH_OBJECT_READY);

	PointsScanned = 0;

	_UpdatePointsDataField();
}
// ---																					// Auto Measuring
void DialogMeasure::OnTraekt()
{
	GeomObjectApprox* objMath;
	char* objectName;
	StTr* a = (StTr*)malloc(sizeof(StTr));
	CWinThread* TraektTh;

	CAutoDlg dlg;


	objMath = objAp->objMath;
	objectName = objMath->GetName();
	dlg.maxH = (int)objMath->Height;
	if (objectName == CylinderApprox().GetName())
	{
		CylinderApprox* cylinderA;
		//theApp.MTActive = theApp.MTcylinder;
		*NumStyle = theApp.MTcylinder;
		MeasureName.SetWindowTextW(L"Цилиндр");
		m_vObjName.Format(L"Цилиндр_%d", pView->NameNumb_Cylinder);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.maxP = (int)((360.f / (asin(0.5 / objMath->Radius) * 2 * 180.f / PI_Approx)) - 1);
		if (dlg.DoModal() == IDOK)
		{
			//delete cylinderA;
			cylinderA = (CylinderApprox*)objMath;
			//CmmApprox Cyl;

			//dlgMeasure->OnTraekt();
			//dlgMeasure->DoModal();

			a->TrGeom = cylinderA;
			a->TrStep = dlg.m_bStep;
			a->TrPl = dlg.m_bN;
			a->TrBoss = dlg.m_bBoss;
			a->TrPart = dlg.m_bPart;
			a->TrCmm = pCMM;
			//TraektTh = AfxBeginThread(Cyl.TraektCylinder, a);
			TraektTh = AfxBeginThread(TraektCylinder, a);

		}
		else
			OnCancel();
	}
	if (objectName == ConeApprox().GetName())
	{
		ConeApprox* coneA;
		//theApp.MTActive = theApp.MTcone;
		*NumStyle = theApp.MTcone;
		MeasureName.SetWindowTextW(L"Конус");
		m_vObjName.Format(L"Конус_%d", pView->NameNumb_Cone);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.maxP = (int)((360.f / (asin(0.5 / objMath->Radius) * 2 * 180.f / PI_Approx)) - 1);
		if (dlg.DoModal() == IDOK)
		{
			//delete coneA;
			coneA = (ConeApprox*)objMath;
			//CmmApprox Con;

			a->TrGeom = coneA;
			a->TrStep = dlg.m_bStep;
			a->TrPl = dlg.m_bN;
			a->TrBoss = dlg.m_bBoss;
			a->TrPart = dlg.m_bPart;
			a->TrCmm = pCMM;
			//dlgMeasure->OnTraekt();
			//dlgMeasure->DoModal();
			//TraektTh = AfxBeginThread(&(Con.TraektCone), a);
			TraektTh = AfxBeginThread(TraektCone, a);

		}
		else
			OnCancel();
	}
	if (objectName == SphereApprox().GetName())
	{
		SphereApprox* sphereA;
		//	theApp.MTActive = theApp.MTsphere;
		*NumStyle = theApp.MTsphere;
		MeasureName.SetWindowTextW(L"Сфера");
		m_vObjName.Format(L"Сфера_%d", pView->NameNumb_Sphere);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.maxP = (int)((360.f / (asin(0.5 / objMath->Radius) * 2 * 180.f / PI_Approx)) - 1);
		if (dlg.DoModal() == IDOK)
		{
			//delete sphereA;
			sphereA = (SphereApprox*)objMath;
			//CmmApprox Sp;

			a->TrGeom = sphereA;
			a->TrStep = dlg.m_bStep;
			a->TrPl = dlg.m_bN;
			a->TrBoss = dlg.m_bBoss;
			a->TrPart = dlg.m_bPart;
			a->TrCmm = pCMM;
			//dlgMeasure->OnTraekt();
			//dlgMeasure->DoModal();
			//TraektTh = AfxBeginThread(&(Sp.TraektSphere), a);
			TraektTh = AfxBeginThread(TraektSphere, a);

		}
		else
			OnCancel();
	}
	if (objectName == PointApprox().GetName())
	{
		PointApprox* pointA;
		//	theApp.MTActive = theApp.MTpoint;
		*NumStyle = theApp.MTpoint;
		MeasureName.SetWindowTextW(L"Точка");

		m_vObjName.Format(L"Точка_%d", pView->NameNumb_Point);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.ObjType = 4;
		/*		if (dlg.DoModal() == IDOK)
				{*/
				//delete pointA;
		pointA = (PointApprox*)objMath;
		//CmmApprox Poi;

		a->TrGeom = pointA;
		a->TrCmm = pCMM;
		//dlgMeasure->OnTraekt();
		//dlgMeasure->DoModal();
		//TraektTh = AfxBeginThread(&(Poi.TraektPoint), a);
		TraektTh = AfxBeginThread(TraektPoint, a);

		/*		}
				else
					OnCancel();*/
	}
	if (objectName == LineSegmentApprox().GetName())
	{
		LineSegmentApprox* lineSegmentA;
		//theApp.MTActive = theApp.MTlineSegment;
		*NumStyle = theApp.MTlineSegment;
		MeasureName.SetWindowTextW(L"Линия");
		m_vObjName.Format(L"Линия_%d", pView->NameNumb_LineSegment);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.ObjType = 3;
		dlg.maxP = (int)(objMath->Height);
		if (dlg.DoModal() == IDOK)
		{
			//delete lineSegmentA;
			lineSegmentA = (LineSegmentApprox*)objMath;
			//CmmApprox Lin;

			a->TrGeom = lineSegmentA;
			a->TrStep = dlg.m_bStep;
			a->TrCmm = pCMM;
			//dlgMeasure->OnTraekt();
			//dlgMeasure->DoModal();
			//TraektTh = AfxBeginThread(&(Lin.TraektLine), a);
			TraektTh = AfxBeginThread(TraektLine, a);

		}
		else
			OnCancel();
	}
	if (objectName == RectangleApprox().GetName())
	{
		RectangleApprox* rectangleA;
		//theApp.MTActive = theApp.MTplane;
		*NumStyle = theApp.MTplane;
		MeasureName.SetWindowTextW(L"Плоскость");
		m_vObjName.Format(L"Плоскость_%d", pView->NameNumb_Plane);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.ObjType = 1;
		dlg.maxP = (int)(objMath->Height);
		if (dlg.DoModal() == IDOK)
		{
			//delete rectangleA;
			rectangleA = (RectangleApprox*)objMath;
			//CmmApprox Rec;

			a->TrGeom = rectangleA;
			a->TrStep = dlg.m_bStep;
			a->TrPl = dlg.m_bN;
			a->TrCmm = pCMM;
			//dlgMeasure->DoModal();
			//TraektTh = AfxBeginThread(&(Rec.TraektRectangle), a);
			TraektTh = AfxBeginThread(TraektRectangle, a);

		}
		else
			OnCancel();
	}
	if (objectName == CircleApprox().GetName())
	{
		CircleApprox* circleA;
		//theApp.MTActive = theApp.MTcircle;
		*NumStyle = theApp.MTcircle;
		MeasureName.SetWindowTextW(L"Окружность");
		m_vObjName.Format(L"Окружность_%d", pView->NameNumb_Circle);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		dlg.ObjType = 2;
		dlg.maxP = (int)((360.f / (asin(0.5 / objMath->Radius) * 2 * 180.f / PI_Approx)) - 1);
		if (dlg.DoModal() == IDOK)
		{
			//delete circleA;
			circleA = (CircleApprox*)objMath;
			//CmmApprox Cir;

			a->TrGeom = circleA;
			a->TrStep = dlg.m_bStep;
			a->TrPart = dlg.m_bPart;
			a->TrCmm = pCMM;
			a->TrBoss = dlg.m_bBoss;
			//dlgMeasure->DoModal();
			//TraektTh = AfxBeginThread(&(Cir.TraektCircle), a);
			TraektTh = AfxBeginThread(TraektCircle, a);

		}
		else
			OnCancel();
	}



	/*delete rectangleA;
	delete circleA;
	delete lineSegmentA;
	delete pointA;
	delete sphereA;
	delete coneA;
	delete cylinderA;*/
}
