#include <minmax.h>

//----------------------------------------------------------
// Класс CDVector
//----------------------------------------------------------

// Конструкторы класса CDVector

inline CDVector::CDVector()
{ /* random filled */ }

inline CDVector::CDVector( double f )
{ x=y=z=f;}

inline CDVector::CDVector( double a, double b, double c )
{ x=a; y=b; z=c;}

//inline CDVector::CDVector( CVector vector )
//{ x=vector.x; y=vector.y; z=vector.z;}

// Функции класса CDVector

inline void CDVector::Set( double a, double b, double c )
{ x=a; y=b; z=c;}

inline void CDVector::SetNull()
{ x=0.f; y=0.f; z=0.f;}

inline void CDVector::SetMin( CDVector vector )
{ x=min(x,vector.x); y=min(y,vector.y); z=min(z,vector.z);}

inline void CDVector::SetMax( CDVector vector )
{ x=max(x,vector.x); y=max(y,vector.y); z=max(z,vector.z);}

inline CDVector CDVector::Min( CDVector vector )
{ return CDVector( min(x,vector.x), min(y,vector.y), min(z,vector.z));}

inline CDVector CDVector::Max( CDVector vector )
{ return CDVector( max(x,vector.x), max(y,vector.y), max(z,vector.z));}

inline double CDVector::Length()
{ return sqrt(x*x+y*y+z*z);}

inline bool CDVector::IsNull()
{ return (x==0.0&&y==0.0&&z==0.0);}

inline void CDVector::Normalize()
{ double d=this->Length(); if( d!=0.f ) *this/=d;}

// Операторы класса CDVector

inline CDVector CDVector::operator-()
{ return CDVector( -x, -y, -z );}

inline CDVector CDVector::operator+( CDVector vector )
{ return CDVector( x+vector.x, y+vector.y, z+vector.z );}

inline CDVector CDVector::operator-( CDVector vector )
{ return CDVector( x-vector.x, y-vector.y, z-vector.z );}

inline CDVector CDVector::operator*( double val )
{ return CDVector( x*val, y*val, z*val );}

inline CDVector CDVector::operator/( double val )
{ return CDVector( x/val, y/val, z/val );}

inline double CDVector::operator*( CDVector vector )	// скалярное произведение
{ return ( x*vector.x + y*vector.y + z*vector.z );}


inline CDVector CDVector::operator^( CDVector vector )	// векторное произведение
{
	return CDVector( y*vector.z-z*vector.y,
					z*vector.x-x*vector.z,
					x*vector.y-y*vector.x );
}

inline void CDVector::operator+=( CDVector vector )
{ x+=vector.x; y+=vector.y; z+=vector.z;}

inline void CDVector::operator-=( CDVector vector )
{ x-=vector.x; y-=vector.y; z-=vector.z;}

inline void CDVector::operator*=( double val )
{ x*=val; y*=val; z*=val;}

inline void CDVector::operator/=( double val )
{ x/=val; y/=val; z/=val;}

inline bool CDVector::operator==( CDVector vector )
{ return ( x == vector.x && y == vector.y && z == vector.z );}

inline bool CDVector::operator!=( CDVector vector )
{ return ( x != vector.x || y != vector.y || z != vector.z );}

//inline void CDVector::operator>>( CArchive* ar )
//{ *ar << x; *ar << y; *ar << z;}

//inline void CDVector::operator<<( CArchive* ar )
//{ *ar >> x; *ar >> y; *ar >> z;}

//inline CDVector::operator CPoint()
//{ return CPoint( (int)x, (int)y );}
//
//inline CDVector::operator tagPOINT()
//{ return CPoint( (int)x, (int)y );};

inline double CDVector::operator[]( int nIndex ) const
{ return *(&x + nIndex);}

inline double& CDVector::operator[]( int nIndex )
{ return *(&x + nIndex);}

//inline CDVector::operator CVector()
//{ return CVector( (float)x, (float)y, (float)z );}

//----------------------------------------------------------
// Класс CDMatrix
//----------------------------------------------------------

// Конструкторы класса CDMatrix
inline CDMatrix::CDMatrix()
{ /* random filled */ }

inline CDMatrix::CDMatrix( CDVector A, CDVector B, CDVector C )
{ X=A; Y=B; Z=C;}

// Функции класса CDMatrix

inline void CDMatrix::SetUnit()
{
	X.Set( 1.f, 0.f, 0.f );
	Y.Set( 0.f, 1.f, 0.f );
	Z.Set( 0.f, 0.f, 1.f );
}

// Операторы класса CDMatrix

inline CDMatrix CDMatrix::operator~()					// транспонирование матрицы
{
	return CDMatrix( CDVector(X.x,Y.x,Z.x),  CDVector(X.y,Y.y,Z.y),  CDVector(X.z,Y.z,Z.z));
}

inline bool CDMatrix::operator==( CDMatrix matrix )
{
	return ( X==matrix.X && Y==matrix.Y && Z==matrix.Z );
}

inline CDMatrix CDMatrix::operator*( double value )		// произведение матрицы на число
{
	return CDMatrix( X*value, Y*value, Z*value );
}

inline CDVector CDMatrix::operator*( CDVector vector )	//  произведение матрицы на вектор
{
	return CDVector( X*vector, Y*vector, Z*vector );
}

inline CDVector CDMatrix::operator[]( int nIndex ) const
{ return *((CDVector *)(&X + nIndex));}

inline CDVector& CDMatrix::operator[]( int nIndex )
{ return *((CDVector *)(&X + nIndex));}

//inline void CDMatrix::operator>>( CArchive* ar )
//{ X >> ar; Y >> ar; Z >> ar;}
//
//inline void CDMatrix::operator<<( CArchive* ar )
//{ X << ar; Y << ar; Z << ar;}
//
//inline CDMatrix::operator CMatrix()
//{ return CMatrix( (CVector)X, (CVector)Y, (CVector)Z );}

