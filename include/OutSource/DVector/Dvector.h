
//----------------------------------------------------------
// Класс CDVector
//----------------------------------------------------------

#include "math.h"

#ifndef __DVECTOR_H__
#define __DVECTOR_H__

//#define DPI  3.14159265359
#define DPI  3.1415926535897932384626433832795
#define AXIS_X	0
#define AXIS_Y	1
#define AXIS_Z	2


class CDVector
{
public:
	double x, y, z;

// Конструкторы класса CDVector
	CDVector();
	CDVector( double f );
	CDVector( double a, double b, double c );
	//CDVector( CVector vector );

// Функции класса CDVector
	void Set( double a, double b, double c );
	void SetNull();
	void SetMin( CDVector vector );
	void SetMax( CDVector vector );
	CDVector Min( CDVector vector );
	CDVector Max( CDVector vector );
	double Length();
	bool IsNull();
	void Normalize();
	int GetMainIndex();
	int GetMinorIndex();
	CDVector Vector2Plane( CDVector n );
	CDVector Node2Plane( CDVector p, CDVector n );
	double Angle(CDVector V);

// Операторы класса CDVector
	CDVector operator-();
	CDVector operator+( CDVector vector );
	CDVector operator-( CDVector vector );
	CDVector operator*( double val );
	CDVector operator/( double val );

	double operator*( CDVector vector );
	CDVector operator^( CDVector vector );

	void operator+=( CDVector vector );
	void operator-=( CDVector vector );
	void operator*=( double val );
	void operator/=( double val );

	bool operator==( CDVector vector );
	bool operator!=( CDVector vector );

	//void operator>>( CArchive* ar );
	//void operator<<( CArchive* ar );

	//operator CPoint();
	//operator tagPOINT();

	double operator[]( int nIndex ) const;
	double& operator[]( int nIndex );

	//operator CVector();
};

typedef class CDVector CDNode;

//----------------------------------------------------------
// Класс CDMatrix
//----------------------------------------------------------

class CDMatrix
{
public:
	CDVector X, Y, Z;					//	| X.x X.y X.z |
										//	| Y.x Y.y Y.z |
// Конструкторы класса CDMatrix			//	| Z.x Z.y Z.z |
	CDMatrix();
	CDMatrix( CDVector A, CDVector B, CDVector C );
	CDMatrix( double Angle, char Axis );
	CDMatrix( double Angle, int iAxis );	// "правильный" поворот

// Функции класса CDMatrix
public:
	void SetUnit();
	void Set( CDVector View );
	void Set( CDVector vX, CDVector vY );
	void Set( CDVector vX, CDVector vY, CDVector vZ );

// Операторы класса CDMatrix
	CDMatrix operator~();
	bool operator==( CDMatrix matrix );
	CDMatrix operator*( double value );
	CDVector operator*( CDVector vector );
	CDMatrix operator*( CDMatrix matrix );
	CDMatrix operator&( CDMatrix matrix );
	CDMatrix operator-( CDMatrix matrix );
	CDMatrix operator+( CDMatrix matrix );

	CDVector operator[]( int nIndex ) const;
	CDVector& operator[]( int nIndex );

	//void operator>>( CArchive* ar );
	//void operator<<( CArchive* ar );

	//operator CMatrix();
};

//----------------------------------------------------------
// Класс CDTransform
//----------------------------------------------------------
class CDTransform
{
public:
	CDVector T;	// перенос
	CDMatrix R;	// вращение
	CDVector C;	// центр вращения

// Конструкторы класса CDTransform
public:
	CDTransform();
	CDTransform( CDVector t, CDMatrix r, CDVector c );

	void SetUnit();
	void Set( CDVector View );
	void Set( CDVector View, CDVector Center );
	//void Serialize( CArchive& ar );
	void ChangeCenter( CDVector Center );

// Операторы класса CDTransform
	bool operator==( CDTransform transform );
	void operator+=( CDVector vector );
	void operator-=( CDVector vector );
	CDTransform operator+( CDVector vector );
	CDTransform operator-( CDVector vector );
	CDVector operator*( CDVector node );

	void operator*=( CDMatrix matrix );
	CDTransform operator*( CDTransform transform );
	CDTransform operator~();

	
};



//#include "DVector.inl"
#endif /* __LINE_GEOMETRIC_H__ */