// DialogToleranceSelectObjects.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DialogToleranceSelectObjects.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdio.h>
// Диалоговое окно DialogToleranceSelectObjects

IMPLEMENT_DYNAMIC(DialogToleranceSelectObjects, CDialog)


DialogToleranceSelectObjects::DialogToleranceSelectObjects(CWnd* pParent, TOLERANCE_NAME toleranceName)
	: CDialog(IDD_DIALOG_TOLERANCE_OBJECT_SELECT, pParent)
{
	parent = (CMainFrame *) pParent;
	this->toleranceName = toleranceName;
	this->objectsArray = parent->dlgMeasure->objectsArray;
	this->toleranceObjectsArray = parent->dlgMeasure->toleranceObjectsArray;
	unselectAllObjects();
	parent->pView->isToleranceAction = true;
	base = nullptr;
	control = nullptr;
}


BOOL DialogToleranceSelectObjects::OnInitDialog() {

	changeName();
	((CButton*)GetDlgItem(IDOK))->EnableWindow(false);
	return TRUE;
}


DialogToleranceSelectObjects::~DialogToleranceSelectObjects()
{
	unselectAllObjects();
	parent->pView->isToleranceAction = false;
}

void DialogToleranceSelectObjects::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void DialogToleranceSelectObjects::unselectAllObjects()
{
	if (objectsArray != nullptr)
	{
		for (int i = 0; i < (int)objectsArray->size(); i++)
		{
			objectsArray->operator[](i)->flagSelected = false;
		}
	}
	if (toleranceObjectsArray != nullptr)
	{
		for (int i = 0; i < (int)toleranceObjectsArray->size(); i++)
		{
			toleranceObjectsArray->operator[](i)->flagSelected = false;
		}
	}
	parent->pView->RedrawWindow();
}

ObjectApprox* DialogToleranceSelectObjects::getSelectedObject()
{
	if (objectsArray != nullptr)
	{
		for (int i = 0; i < (int)objectsArray->size(); i++)
		{
			if (objectsArray->operator[](i)->flagSelected) {
				return objectsArray->operator[](i);
			}
		}
	}
	return nullptr;
}

ToleranceBase* DialogToleranceSelectObjects::getSelectedBase()
{
	if (toleranceObjectsArray != nullptr)
	{
		for (int i = 0; i < (int)toleranceObjectsArray->size(); i++)
		{
			if (toleranceObjectsArray->operator[](i)->flagSelected) {
				if (dynamic_cast<ToleranceBase*>(toleranceObjectsArray->operator[](i))) {
					return (ToleranceBase*)toleranceObjectsArray->operator[](i);
				}

			}
		}
	}
	return nullptr;
}



void DialogToleranceSelectObjects::changeName()
{
	LPCTSTR name = L"Не разработано";
	switch (toleranceName) {
	case LOCATION_COAXIALITY:
		name = L"Соосность";
		break;
	case LOCATION_CONCENTRICITY:
		name = L"Концентричность";
		break;
	case ORIENTATION_ANGULARITY:
		name = L"Наклон";
		((CStatic*)GetDlgItem(IDC_TEXT_ANGULARITY))->ShowWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_ANGULARITY))->ShowWindow(TRUE);
		break;
	case ORIENTATION_PERPENDICULARITY:
		name = L"Перпендикулярность";
		break;
	case ORIENTATION_PARALLELISM:
		name = L"Параллельность";
		break;
	case RUNOUT_FACE:
		name = L"Торцевое биение";
		break;
	case RUNOUT_RADIAL:
		name = L"Радиальное биение";
		break;
	case LOCATION_SYMMETRY:
		name = L"Симметричность";
		break;
	case LOCATION_POSITION:
		name = L"Позиционирование";
		break;

	default:
		name = L"Не разработано";
	}

	((CStatic*)GetDlgItem(IDC_TEXT_INSTRUCTION))->SetWindowTextW(name);
}




BEGIN_MESSAGE_MAP(DialogToleranceSelectObjects, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_BASE_OBJECT, &DialogToleranceSelectObjects::OnBnClickedButtonSelectBaseObject)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_CONTROL_OBJECT, &DialogToleranceSelectObjects::OnBnClickedButtonSelectControlObject)
	ON_BN_CLICKED(IDOK, &DialogToleranceSelectObjects::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DialogToleranceSelectObjects::OnBnClickedCancel)
ON_STN_CLICKED(IDC_TEXT_INSTRUCTION, &DialogToleranceSelectObjects::OnStnClickedTextInstruction)
END_MESSAGE_MAP()


// Обработчики сообщений DialogToleranceSelectObjects


void DialogToleranceSelectObjects::OnBnClickedButtonSelectBaseObject()
{
	base = nullptr;
	parent->pView->isToleranceAction = true;
	base = getSelectedBase();
	if (base != nullptr) {
		((CStatic*)GetDlgItem(IDC_TEXT_BASE_NAME))->SetWindowTextW(base->baseChar);
		((CStatic*)GetDlgItem(IDC_TEXT_BASE_NAME))->EnableWindow(true);
		if (control != nullptr) {
			((CButton*)GetDlgItem(IDOK))->EnableWindow(true);
		}
	}
	else {
		AfxMessageBox(L"Необходимо выбрать базу, а не объект!");
	}
	unselectAllObjects();
}


void DialogToleranceSelectObjects::OnBnClickedButtonSelectControlObject()
{
	control = nullptr;
	
	parent->pView->isToleranceAction = true;
	control = getSelectedObject();
	if (control != nullptr) {
		((CStatic*)GetDlgItem(IDC_TEXT_CONTROL_NAME))->SetWindowTextW(control->Name.c_str());
		((CStatic*)GetDlgItem(IDC_TEXT_CONTROL_NAME))->EnableWindow(true);
		if (base != nullptr) {
			((CStatic*)GetDlgItem(IDOK))->EnableWindow(true);
		}
	}
	unselectAllObjects();
}


void DialogToleranceSelectObjects::OnBnClickedOk()
{
	
	unselectAllObjects();
	parent->pView->isToleranceAction = false;
	if (base != nullptr && control != nullptr) {
		if (base->toleranceObject != nullptr) {
			if (base->toleranceObject->objMath == control->objMath || base->objMath == control->objMath) {
				AfxMessageBox(L"В качестве базы и контрольного объекта выбран один и тот же объект");
			}
		}else if (base->objMath == control->objMath) {
			AfxMessageBox(L"В качестве базы и контрольного объекта выбран один и тот же объект");
			return;
		}
		double angle = 0;
		CString sWindowText;
		ToleranceFrame* frame = nullptr;
		double result = NULL;
		switch (toleranceName) {
			case LOCATION_CONCENTRICITY:
				result = parent->pTolerance->LocationConcentricity((CircleApprox*)base->objMath, (CircleApprox*)control->objMath);
				
				frame = new ToleranceFrame(base, control, toleranceName, result);
				break;

			case LOCATION_COAXIALITY:
				if (base->objMath->GetName() == CylinderApprox().GetName()) {
					if (control->objMath->GetName() == CylinderApprox().GetName()) {
						result = parent->pTolerance->LocationCoaxiality((CylinderApprox*)base->objMath, (CylinderApprox*)control->objMath);
					}
					else if (control->objMath->GetName() == ConeApprox().GetName()) {
						result = parent->pTolerance->LocationCoaxiality((CylinderApprox*)base->objMath, (ConeApprox*)control->objMath);
					}
				}
				else if (base->objMath->GetName() == ConeApprox().GetName()) {
					if (control->objMath->GetName() == CylinderApprox().GetName()) {
						result = parent->pTolerance->LocationCoaxiality((ConeApprox*)base->objMath, (CylinderApprox*)control->objMath);
					}
					else if (control->objMath->GetName() == ConeApprox().GetName()) {
						result = parent->pTolerance->LocationCoaxiality((ConeApprox*)base->objMath, (ConeApprox*)control->objMath);
					}
				}
				frame = new ToleranceFrame(base, control, toleranceName, result);
				break;

			case ORIENTATION_PARALLELISM:
				result = NULL;
				
				if (base->objMath->GetName() == RectangleApprox().GetName() && control->objMath->GetName() == RectangleApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == LineSegmentApprox().GetName() && control->objMath->GetName() == LineSegmentApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((LineSegmentApprox*)base->objMath, (LineSegmentApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				break;

			case ORIENTATION_ANGULARITY:
				((CEdit*)GetDlgItem(IDC_EDIT_ANGULARITY))->GetWindowTextW(sWindowText);
				
				//angle = atof((const char*)(LPCTSTR)(sWindowText));
				angle = atof((const char*)sWindowText.operator LPCTSTR());
				angle = wcstod(sWindowText, nullptr);
				result = parent->pTolerance->OrientationAngularity((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath, angle);
				frame = new ToleranceFrame(base, control, toleranceName, result);

				break;

			case ORIENTATION_PERPENDICULARITY:
				
				result = parent->pTolerance->OrientationPerpendicularity((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath);
				frame = new ToleranceFrame(base, control, toleranceName, result);

				break;

			case RUNOUT_RADIAL:
				result = NULL;
				if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == CylinderApprox().GetName()) {
					result = parent->pTolerance->RunOutRadial((CylinderApprox*)base->objMath, (CylinderApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == LineSegmentApprox().GetName() && control->objMath->GetName() == LineSegmentApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((LineSegmentApprox*)base->objMath, (LineSegmentApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				break;

			case RUNOUT_FACE:
				result = NULL;
				if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == RectangleApprox().GetName()) {
					result = parent->pTolerance->RunOutFace((CylinderApprox*)base->objMath, (PlaneApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == CircleApprox().GetName()) {
					result = parent->pTolerance->RunOutFace((CylinderApprox*)base->objMath, (CircleApprox*)control->objMath);
					frame = new ToleranceFrame(base, control, toleranceName, result);
				}
				break;

			

			default:
				AfxMessageBox(L"Для данного допуска еще не разработано");
		}

		if (frame != nullptr) {
			for (int j = 0; j < parent->dlgMeasure->toleranceObjectsArray->size(); j++) {
				if (dynamic_cast<ToleranceFrame*>(parent->dlgMeasure->toleranceObjectsArray->operator[](j))) {
					ToleranceFrame* existFrame = (ToleranceFrame*)parent->dlgMeasure->toleranceObjectsArray->operator[](j);
					// Проверка что такая рамка уже существует. Если существует, то начинаем ее перемещать
					if (existFrame->toleranceName == toleranceName && existFrame->Base->objMath == base->objMath && existFrame->objMath == control->objMath) {
						parent->pView->startSelectObject(existFrame);
						return;
					}
				}
			}
			parent->pTolerance->addNewObject(frame);
			parent->pView->startSelectObject(frame);
			parent->RedrawWindow();
		}
		
	}
	else {
		AfxMessageBox(L"Не выбрана база или контрольный объект");
	}
	
	this->CloseWindow();
	CDialog::OnOK();
}


void DialogToleranceSelectObjects::OnBnClickedCancel()
{

	unselectAllObjects();
	parent->pView->isToleranceAction = false;
	CDialog::OnCancel();
}


//void DialogToleranceSelectObjects::OnStnClickedTextBaseName()
//{
//	// TODO: добавьте свой код обработчика уведомлений
//}


void DialogToleranceSelectObjects::OnStnClickedTextInstruction()
{
	// TODO: добавьте свой код обработчика уведомлений
}
