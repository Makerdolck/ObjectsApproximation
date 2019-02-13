// DialogMeasure.cpp : implementation file
//

#include "stdafx.h"
#include "MFCDubrovkin.h"
#include "DialogMeasure.h"
#include "afxdialogex.h"


// DialogMeasure dialog

IMPLEMENT_DYNAMIC(DialogMeasure, CDialog)

DialogMeasure::DialogMeasure(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MEASURE, pParent)
	, m_vObjName(_T(""))
{
	pCMM			= theApp.pCMM;
	NumStyle		= &theApp.MTActive;
	nextObjectID	= &theApp.nextObjectID;
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
}


BEGIN_MESSAGE_MAP(DialogMeasure, CDialog)
	ON_MESSAGE(WM_POINT_APPROX_READY,			&DialogMeasure::OnNewPointForApprox)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POINT,		&DialogMeasure::OnBnClickedButtonDeletePoint)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_POINTS, &DialogMeasure::OnBnClickedButtonDeletePoints)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
//	ON_EN_UPDATE(IDC_EDIT_OBJECT_NAME, &DialogMeasure::OnEnUpdateEditObjectName)
END_MESSAGE_MAP()


// DialogMeasure message handlers


BOOL DialogMeasure::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	CFont font;
	font.CreateFont(640, // 320 - same
		0, // int nWidth, 
		0, 0, 400, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,
		L"Times New Roman");

	MeasureName.SetFont(&font, 1);

	switch (*NumStyle)
	{
	case theApp.MTContainer.point:													// point
		MeasureName.SetWindowTextW(L"Точка");

		m_vObjName.Format(L"Point_%d", pView->NameNumb_Point);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);
		
		//GetDlgItem(IDC_BUTTON_DELETE_POINT)->EnableWindow(false);

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"1");
		PointsAmountForApprox = PointsMinAmount.point;

		//pSpin->EnableWindow(false);
		break;
	case theApp.MTContainer.lineSegment:											// line
		MeasureName.SetWindowTextW(L"Линия");
		
		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"2");
		PointsAmountForApprox = PointsMinAmount.lineSegment;

		m_vObjName.Format(L"Line segment_%d", pView->NameNumb_LineSegment);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		break;
	case theApp.MTContainer.plane:													// plane
		MeasureName.SetWindowTextW(L"Плоскость");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsMinAmount.plane;

		m_vObjName.Format(L"Plane_%d", pView->NameNumb_Plane);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		break;
	case theApp.MTContainer.circle:													// circle
		MeasureName.SetWindowTextW(L"Окружность");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsMinAmount.circle;

		m_vObjName.Format(L"Circle_%d", pView->NameNumb_Circle);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		break;	
	case theApp.MTContainer.cylinder:												// cylinder
		MeasureName.SetWindowTextW(L"Цилиндр");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"4");
		PointsAmountForApprox = PointsMinAmount.cylinder;

		m_vObjName.Format(L"Cylinder_%d", pView->NameNumb_Cylinder);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		break;
	case theApp.MTContainer.cone:													// cone
		MeasureName.SetWindowTextW(L"Конус");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"4");
		PointsAmountForApprox = PointsMinAmount.cone;

		m_vObjName.Format(L"Cone_%d", pView->NameNumb_Cone);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

		break;
	case theApp.MTContainer.sphere:													// sphere
		MeasureName.SetWindowTextW(L"Сфера");

		//SetDlgItemText(IDC_EDIT_NUM_OF_POINTS, L"3");
		PointsAmountForApprox = PointsMinAmount.sphere;

		m_vObjName.Format(L"Sphere_%d", pView->NameNumb_Sphere);
		SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

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
	
	Accuracy		= &theApp.AccuracyApprox;
	
	SetTimer(0, 0, NULL);

	return TRUE; 
}


//--------------------------------------------------------------
//	----	Message Handlers
//--------------------------------------------------------------

// ---																					// On Destroy Dialog
void DialogMeasure::OnDestroy()
{
	KillTimer(0);
	
	bool flagVisualize = true;

	if (PointsAmountForApprox == 0 && objectsArray->size() > 0)
	{
		switch (*NumStyle)
		{
		case theApp.MTContainer.point:						// point
															//
			break;
		case theApp.MTContainer.lineSegment:				// lineSegment
			if (PointsScanned == 1)
			{
				flagVisualize = false;
			}
			break;
		case theApp.MTContainer.plane:					// rectangle
			if (PointsScanned >= 1 && PointsScanned < PointsMinAmount.plane)
			{
				flagVisualize = false;
			}
			break;
		case theApp.MTContainer.circle:						// circle
			if (PointsScanned >= 1 && PointsScanned < PointsMinAmount.circle)
			{
				flagVisualize = false;
			}
			break;
		case theApp.MTContainer.cylinder:					// cylinder
			if (PointsScanned >= 1 && PointsScanned < PointsMinAmount.cylinder)
			{
				flagVisualize = false;
			}
			break;
		case theApp.MTContainer.cone:						// cone
			if (PointsScanned >= 1 && PointsScanned < PointsMinAmount.cone)
			{
				flagVisualize = false;
			}
			break;
		case theApp.MTContainer.sphere:						// sphere
			if (PointsScanned >= 1 && PointsScanned < PointsMinAmount.sphere)
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
		}
			
		return;
	}

	if (PointsScanned != 0 && PointsScanned == PointsAmountForApprox)
	{
		_SendObjectToVisualize();
		PointsScanned = 0;

		return;
	}

	if (PointsScanned != 0 && objectsArray->size()>0)
		objectsArray->pop_back();

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
	// TODO: Add your message handler code here and/or call default

	std::ifstream					file;

	std::vector <PointGeometric>	points;
	PointGeometric					pointMy;
	double							accuracy = 0.00001;
	ObjectApprox* newObjectApproxC3D = new ObjectApprox();




	switch (*NumStyle)
	{
	case theApp.MTContainer.point:
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
	case theApp.MTContainer.lineSegment:
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
	case theApp.MTContainer.plane:
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

			newObjectApproxC3D->objMath = rectangleApprox;
		}

		break;
	}
	case theApp.MTContainer.circle:
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

			newObjectApproxC3D->objMath = circleApprox;
		}
		break;
	}
	case theApp.MTContainer.cylinder:
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

			cylinderApprox->FindByPoints(&cylinderApprox->PointsForApprox[0], (int)cylinderApprox->PointsForApprox.size(), accuracy);

			newObjectApproxC3D->objMath = cylinderApprox;
		}
		break;
	}
	case theApp.MTContainer.cone:
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

			newObjectApproxC3D->objMath = coneApprox;
		}

		break;
	}
	case theApp.MTContainer.sphere:
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
	case theApp.MTContainer.point:
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
	case theApp.MTContainer.lineSegment:
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
	case theApp.MTContainer.plane:
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
	case theApp.MTContainer.circle:
	{
		CircleApprox* circleApprox = new CircleApprox();

		file.open(fileName);
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

			newObjectApproxC3D->objMath = circleApprox;
		}
		break;
	}
	case theApp.MTContainer.cylinder:
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
	case theApp.MTContainer.cone:
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
	case theApp.MTContainer.sphere:
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

// ---																					// Show Current Coordinates
void DialogMeasure::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	CString str = _T("");

	// X
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.X, pCMM->MachineAccuracy));
	SetDlgItemText(IDC_EDIT_X, str);
	// Y
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.Y, pCMM->MachineAccuracy));
	SetDlgItemText(IDC_EDIT_Y, str);
	// Z
	str.Format(L"%g", RoundingOf(pCMM->PointCurrent.Z, pCMM->MachineAccuracy));
	SetDlgItemText(IDC_EDIT_Z, str);
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
		case theApp.MTContainer.point:						// point
															//
			break;
		case theApp.MTContainer.lineSegment:				// lineSegment
			if (nPos == 1)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.lineSegment;
				else
					nPos = 0;
			}
			break;
		case theApp.MTContainer.plane:					// rectangle
			if (nPos >= 1 && nPos < PointsMinAmount.plane)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.plane;
				else
					nPos = 0;
			}
			break;
		case theApp.MTContainer.circle:						// circle
			if (nPos >= 1 && nPos < PointsMinAmount.circle)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.circle;
				else
					nPos = 0;
			}
			break;
		case theApp.MTContainer.cylinder:					// cylinder
			if (nPos >= 1 && nPos < PointsMinAmount.cylinder)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.cylinder;
				else
					nPos = 0;
			}
			break;
		case theApp.MTContainer.cone:						// cone
			if (nPos >= 1 && nPos < PointsMinAmount.cone)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.cone;
				else
					nPos = 0;
			}
			break;
		case theApp.MTContainer.sphere:						// sphere
			if (nPos >= 1 && nPos < PointsMinAmount.sphere)
			{
				if (nPos_old < nPos)
					nPos = PointsMinAmount.sphere;
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

	switch (*NumStyle)
	{
	case theApp.MTContainer.point:													// point

		break;
	case theApp.MTContainer.lineSegment:											// line

		break;
	case theApp.MTContainer.plane:													// plane

		break;
	case theApp.MTContainer.circle:													// circle

		break;
	case theApp.MTContainer.cylinder:												// cylinder

		break;
	case theApp.MTContainer.cone:													// cone

		break;
	case theApp.MTContainer.sphere:													// sphere

		break;
	default:
		return 0L;
	}


	if ((PointsScanned >= PointsAmountForApprox) && PointsAmountForApprox != 0)
	{
		_SendObjectToVisualize();

		switch (*NumStyle)
		{
		case theApp.MTContainer.point:													// point
			m_vObjName.Format(L"Point_%d", pView->NameNumb_Point+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.lineSegment:											// line
			m_vObjName.Format(L"Line segment_%d", pView->NameNumb_LineSegment+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.plane:													// plane
			m_vObjName.Format(L"Plane_%d", pView->NameNumb_Plane+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.circle:													// circle
			m_vObjName.Format(L"Circle_%d", pView->NameNumb_Circle+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.cylinder:												// cylinder
			m_vObjName.Format(L"Cylinder_%d", pView->NameNumb_Cylinder+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.cone:													// cone
			m_vObjName.Format(L"Cone_%d", pView->NameNumb_Cone+1);
			SetDlgItemText(IDC_EDIT_OBJECT_NAME, m_vObjName);

			break;
		case theApp.MTContainer.sphere:													// sphere
			m_vObjName.Format(L"Sphere_%d", pView->NameNumb_Sphere+1);
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
		case theApp.MTContainer.point:
			obj = new PointApprox();
			break;
		case theApp.MTContainer.lineSegment:
			obj = new LineSegmentApprox();
			break;
		case theApp.MTContainer.plane:
			obj = new RectangleApprox();
			break;
		case theApp.MTContainer.circle:
			obj = new CircleApprox();
			break;
		case theApp.MTContainer.cylinder:
			obj = new CylinderApprox();
			break;
		case theApp.MTContainer.cone:
			obj = new ConeApprox();
			break;
		case theApp.MTContainer.sphere:
			obj = new SphereApprox();
			break;
		default:
			break;
		}

		flagNewElement			= false;
		PointsScanned			= 0;

		ObjectApprox *newObjectApproxC3D = new ObjectApprox();
		newObjectApproxC3D->objMath = obj;
		objectsArray->push_back(newObjectApproxC3D);
		//objectsArray->back().objMath = obj;
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
	GeomObjectApprox *obj = objectsArray->back()->objMath;

	switch (*NumStyle)
	{
	case theApp.MTContainer.point:
		((PointApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.lineSegment:
		((LineSegmentApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.plane:
		((RectangleApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.circle:
		((CircleApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.cylinder:
		((CylinderApprox*)obj)->	FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.cone:
		((ConeApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	case theApp.MTContainer.sphere:
		((SphereApprox*)obj)->		FindByPoints(&obj->PointsForApprox[0],	(int)obj->PointsForApprox.size(), *Accuracy);
		break;
	default:
		break;
	}

	objectsArray->back()->objID = *nextObjectID;
	*nextObjectID +=1;

	UpdateData(true);	// Update variable with object name

	objectsArray->back()->flagReady	= true;
	objectsArray->back()->Name		= m_vObjName.GetString();
	//pView->PostMessageW(WM_MATH_OBJECT_READY);
	//pView->OnNewObjectForVisualize();
	pView->SendMessageW(WM_MATH_OBJECT_READY);

	PointsScanned = 0;

	_UpdatePointsDataField();
}



