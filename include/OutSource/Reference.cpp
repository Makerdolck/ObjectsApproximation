#include "stdafx.h"
//#include "Vector.h"
#include "DVector\DVector.h"
#include "Reference.h"


//----------------------------------------------------------
// ����� CReference
//----------------------------------------------------------

// ������������ ������ CReference
CReference::CReference():CDTransform()
{

}

void CReference::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_Name;
	}
	else
	{
		ar >> m_Name;
	}

	//CDTransform::Serialize( ar );
}

