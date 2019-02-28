//----------------------------------------------------------
// Класс CReference
//----------------------------------------------------------


#pragma once

#ifndef __REFERENCE_H__
#define __REFERENCE_H__

#include "DVector/Dvector.h"
#include "afx.h"

class CReference : public CDTransform
{
public:
	CString m_Name;
	CReference();

	virtual void Serialize(CArchive& ar);
};
#endif /* __LINE_GEOMETRIC_H__ */