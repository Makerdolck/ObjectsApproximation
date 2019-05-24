// DialogToleranceSelectObjects.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCKio.h"
#include "DialogToleranceSelectObjects.h"
#include "afxdialogex.h"


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
		break;
	case ORIENTATION_PARALLELISM:
		name = L"Параллельность";
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
				if (base->objMath->GetName() == RectangleApprox().GetName() && control->objMath->GetName() == RectangleApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath);
				}
				else if (base->objMath->GetName() == LineSegmentApprox().GetName() && control->objMath->GetName() == LineSegmentApprox().GetName()) {
					result = parent->pTolerance->OrientationParallelism((LineSegmentApprox*)base->objMath, (LineSegmentApprox*)control->objMath);
				}
				tmpBase = new ToleranceBase(base);
				if (result != NULL) {
					frame = new ToleranceFrame(tmpBase, control, toleranceName, result);
				}
				
				break;

		case ORIENTATION_ANGULARITY:
			//tmpBase = new ToleranceBase(base);
			
			//parent->pTolerance->OrientationAngularity((PlaneGeometric*)base, (PlaneGeometric*)control, result);
			parent->pTolerance->OrientationAngularity((PlaneApprox*)base->objMath, (PlaneApprox*)control->objMath);
				//tmpBase = new ToleranceBase(base);
				//frame = new ToleranceFrame(tmpBase, control, toleranceName, 10);
				
				break;

			default:
				AfxMessageBox(L"Для данного допуска еще не разработано");
		}

		if (frame != nullptr) {
			parent->pTolerance->addNewObject(frame);
			parent->pView->selectedToleranceObject = nullptr;
			parent->pView->flagToleranceMove = true;
			parent->pView->selectedToleranceObject = frame;
			parent->RedrawWindow();
		}
		
	}
	else {
		AfxMessageBox(L"Не выбрана база или контрольный объект");
	}
	
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
