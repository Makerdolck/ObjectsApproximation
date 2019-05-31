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


/*DialogToleranceSelectObjects::DialogToleranceSelectObjects(CMainFrame* parent)
{

}*/

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
//	ON_STN_CLICKED(IDC_TEXT_BASE_NAME, &DialogToleranceSelectObjects::OnStnClickedTextBaseName)
ON_STN_CLICKED(IDC_TEXT_INSTRUCTION, &DialogToleranceSelectObjects::OnStnClickedTextInstruction)
END_MESSAGE_MAP()


// Обработчики сообщений DialogToleranceSelectObjects


void DialogToleranceSelectObjects::OnBnClickedButtonSelectBaseObject()
{
	base = nullptr;

	parent->pView->isToleranceAction = true;
	base = getSelectedObject();
	if (base != nullptr) {
		((CStatic*)GetDlgItem(IDC_TEXT_BASE_NAME))->SetWindowTextW(base->Name.c_str());
		((CStatic*)GetDlgItem(IDC_TEXT_BASE_NAME))->EnableWindow(true);
	}
	
	unselectAllObjects();
	if (base != nullptr && control != nullptr) {
		((CButton*)GetDlgItem(IDOK))->EnableWindow(true);
	}
}


void DialogToleranceSelectObjects::OnBnClickedButtonSelectControlObject()
{
	control = nullptr;
	
	parent->pView->isToleranceAction = true;
	control = getSelectedObject();
	if (base != nullptr) {
		((CStatic*)GetDlgItem(IDC_TEXT_CONTROL_NAME))->SetWindowTextW(control->Name.c_str());
		((CStatic*)GetDlgItem(IDC_TEXT_CONTROL_NAME))->EnableWindow(true);
	}
	
	unselectAllObjects();
	if (base != nullptr && control != nullptr) {
		((CStatic*)GetDlgItem(IDOK))->EnableWindow(true);
	}
}


void DialogToleranceSelectObjects::OnBnClickedOk()
{
	
	unselectAllObjects();
	parent->pView->isToleranceAction = false;
	if (base != nullptr && control != nullptr) {
		if (base->Name == control->Name) {
			AfxMessageBox(L"В качестве базы и контрольного объекта выбран один и тот же объект");
			return;
		}
		double angle = 0;
		CString sWindowText;
		ToleranceBase* tmpBase = nullptr;
		ToleranceFrame* frame = nullptr;
		double result = NULL;
		switch (toleranceName) {
			case LOCATION_CONCENTRICITY:
				result = parent->pTolerance->LocationConcentricity((CircleApprox*)base->objMath, (CircleApprox*)control->objMath);
				tmpBase = new ToleranceBase(base);
				frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				break;

			case LOCATION_COAXIALITY:
				result = parent->pTolerance->LocationCoaxiality((CylinderApprox*)base->objMath, (CylinderApprox*)control->objMath);
				tmpBase = new ToleranceBase(base);
				frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				break;

			case ORIENTATION_PARALLELISM:
				result = NULL;
				tmpBase = new ToleranceBase(base);
				if (base->objMath->GetName() == RectangleApprox().GetName() && control->objMath->GetName() == RectangleApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == LineSegmentApprox().GetName() && control->objMath->GetName() == LineSegmentApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((LineSegmentApprox*)base->objMath, (LineSegmentApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				break;

			case ORIENTATION_ANGULARITY:
				tmpBase = new ToleranceBase(base);
			
				((CEdit*)GetDlgItem(IDC_EDIT_ANGULARITY))->GetWindowTextW(sWindowText);
				
				//angle = atof((const char*)(LPCTSTR)(sWindowText));
				angle = atof((const char*)sWindowText.operator LPCTSTR());
				angle = wcstod(sWindowText, nullptr);
				result = parent->pTolerance->OrientationAngularity((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath, angle);
				frame = new ToleranceFrame(tmpBase, control, toleranceName, result);

				break;

			case ORIENTATION_PERPENDICULARITY:
				tmpBase = new ToleranceBase(base);
				result = parent->pTolerance->OrientationAngularity((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath, 90);
				//tmpBase = new ToleranceBase(base);
				frame = new ToleranceFrame(tmpBase, control, toleranceName, result);

				break;

			case RUNOUT_RADIAL:
				result = NULL;
				tmpBase = new ToleranceBase(base);
				if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == CylinderApprox().GetName()) {
					result = parent->pTolerance->RunOutRadial((CylinderApprox*)base->objMath, (CylinderApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == LineSegmentApprox().GetName() && control->objMath->GetName() == LineSegmentApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((LineSegmentApprox*)base->objMath, (LineSegmentApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				break;

			case RUNOUT_FACE:
				result = NULL;
				tmpBase = new ToleranceBase(base);
				if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == RectangleApprox().GetName()) {
					result = parent->pTolerance->RunOutFace((CylinderApprox*)base->objMath, (RectangleApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				else if (base->objMath->GetName() == CylinderApprox().GetName() && control->objMath->GetName() == CircleApprox().GetName()) {
					result = parent->pTolerance->RunOutFace((CylinderApprox*)base->objMath, (CircleApprox*)control->objMath);
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
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
