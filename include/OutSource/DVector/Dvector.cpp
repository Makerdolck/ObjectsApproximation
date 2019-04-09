
#include "stdafx.h"
//#include "afxwin.h"
#include "DVector.h"
#include "math.h"
#include "minmax.h"

// ������������ ������ CDVector


CDVector::CDVector()
{ /* random filled */
}

CDVector::CDVector(double f)
{
	x = y = z = f;
}

CDVector::CDVector(double a, double b, double c)
{
	x = a; y = b; z = c;
}

// ������� ������ CDVector

void CDVector::Set(double a, double b, double c)
{
	x = a; y = b; z = c;
}

void CDVector::SetNull()
{
	x = 0.f; y = 0.f; z = 0.f;
}

void CDVector::SetMin(CDVector vector)
{
	x = min(x, vector.x); y = min(y, vector.y); z = min(z, vector.z);
}

void CDVector::SetMax(CDVector vector)
{
	x = max(x, vector.x); y = max(y, vector.y); z = max(z, vector.z);
}

CDVector CDVector::Min(CDVector vector)
{
	return CDVector(min(x, vector.x), min(y, vector.y), min(z, vector.z));
}

CDVector CDVector::Max(CDVector vector)
{
	return CDVector(max(x, vector.x), max(y, vector.y), max(z, vector.z));
}

double CDVector::Length()
{
	return sqrt(x*x + y*y + z*z);
}

bool CDVector::IsNull()
{
	return (x == 0.0&&y == 0.0&&z == 0.0);
}

void CDVector::Normalize()
{
	double d = this->Length(); if (d != 0.f) *this /= d;
}

// ��������� ������ CDVector

CDVector CDVector::operator-()
{
	return CDVector(-x, -y, -z);
}

CDVector CDVector::operator+(CDVector vector)
{
	return CDVector(x + vector.x, y + vector.y, z + vector.z);
}

CDVector CDVector::operator-(CDVector vector)
{
	return CDVector(x - vector.x, y - vector.y, z - vector.z);
}

CDVector CDVector::operator*(double val)
{
	return CDVector(x*val, y*val, z*val);
}

CDVector CDVector::operator/(double val)
{
	return CDVector(x / val, y / val, z / val);
}

double CDVector::operator*(CDVector vector)	// ��������� ������������
{
	return (x*vector.x + y*vector.y + z*vector.z);
}


CDVector CDVector::operator^(CDVector vector)	// ��������� ������������
{
	return CDVector(y*vector.z - z*vector.y,
		z*vector.x - x*vector.z,
		x*vector.y - y*vector.x);
}

void CDVector::operator+=(CDVector vector)
{
	x += vector.x; y += vector.y; z += vector.z;
}

void CDVector::operator-=(CDVector vector)
{
	x -= vector.x; y -= vector.y; z -= vector.z;
}

void CDVector::operator*=(double val)
{
	x *= val; y *= val; z *= val;
}

void CDVector::operator/=(double val)
{
	x /= val; y /= val; z /= val;
}

bool CDVector::operator==(CDVector vector)
{
	return (x == vector.x && y == vector.y && z == vector.z);
}

bool CDVector::operator!=(CDVector vector)
{
	return (x != vector.x || y != vector.y || z != vector.z);
}



double CDVector::operator[](int nIndex) const
{
	return *(&x + nIndex);
}

double& CDVector::operator[](int nIndex)
{
	return *(&x + nIndex);
}



//----------------------------------------------------------
// ����� CDMatrix
//----------------------------------------------------------

// ������������ ������ CDMatrix
CDMatrix::CDMatrix()
{ /* random filled */
}

CDMatrix::CDMatrix(CDVector A, CDVector B, CDVector C)
{
	X = A; Y = B; Z = C;
}

// ������� ������ CDMatrix

void CDMatrix::SetUnit()
{
	X.Set(1.f, 0.f, 0.f);
	Y.Set(0.f, 1.f, 0.f);
	Z.Set(0.f, 0.f, 1.f);
}

// ��������� ������ CDMatrix

CDMatrix CDMatrix::operator~()					// ���������������� �������
{
	return CDMatrix(CDVector(X.x, Y.x, Z.x), CDVector(X.y, Y.y, Z.y), CDVector(X.z, Y.z, Z.z));
}

bool CDMatrix::operator==(CDMatrix matrix)
{
	return (X == matrix.X && Y == matrix.Y && Z == matrix.Z);
}

CDMatrix CDMatrix::operator*(double value)		// ������������ ������� �� �����
{
	return CDMatrix(X*value, Y*value, Z*value);
}

CDVector CDMatrix::operator*(CDVector vector)	//  ������������ ������� �� ������
{
	return CDVector(X*vector, Y*vector, Z*vector);
}

CDVector CDMatrix::operator[](int nIndex) const
{
	return *((CDVector *)(&X + nIndex));
}

CDVector& CDMatrix::operator[](int nIndex)
{
	return *((CDVector *)(&X + nIndex));
}

/*void CDMatrix::operator>>(CArchive* ar)
{
	X >> ar; Y >> ar; Z >> ar;
}*/

/*void CDMatrix::operator<<(CArchive* ar)
{
	X << ar; Y << ar; Z << ar;
}
*/





int CDVector::GetMainIndex()
{
	int i = 0;
	float f = (float)fabs(x);
	if (fabs(y) > f) { i = 1; f = (float)fabs(y); }
	if (fabs(z) > f) i = 2;
	return i;
}

int CDVector::GetMinorIndex()
{
	int i = 0;
	float f = (float)fabs(x);
	if (fabs(y) < f) { i = 1; f = (float)fabs(y); }
	if (fabs(z) < f) i = 2;
	return i;
}

// �������� ������� �� ���������
CDVector CDVector::Vector2Plane(CDVector n)
{
	return *this - n * (*this * n);
}

// �������� ����� �� ���������
CDVector CDVector::Node2Plane(CDNode p, CDVector n)
{
	return *this - n * ((*this - p) * n);
}
double CDVector::Angle(CDVector V)
{
	return(acos(*this*V / (this->Length()*V.Length())));
	/*/double sin = (*this^V).Length()/(this->Length()*V.Length());
	double alfa = acos(cos);
	if(alfa>PI/2) alfa = PI - alfa;
	if(cos<0&&sin>0) alfa +=PI/2;
	if(cos<0&&sin<0) alfa +=PI;
	if(cos>0&&sin>0) alfa +=3*PI/2;
	return (alfa);
	*/
}

//----------------------------------------------------------
// ����� CDMatrix
//----------------------------------------------------------

// ������������ ������ CDMatrix

CDMatrix::CDMatrix(double Angle, char Axis)
{
	double sn = sin(Angle);
	double cs = cos(Angle);

	switch (Axis)
	{
	case 'x':
		X.Set(1.f, 0.f, 0.f);
		Y.Set(0.f, cs, sn);
		Z.Set(0.f, -sn, cs);
		break;
	case 'y':
		X.Set(cs, 0.f, -sn);
		Y.Set(0.f, 1.f, 0.f);
		Z.Set(sn, 0.f, cs);
		break;
	case 'z':
		X.Set(cs, sn, 0.f);
		Y.Set(-sn, cs, 0.f);
		Z.Set(0.f, 0.f, 1.f);
	}
}

// �������� ���������� ������� ��������
CDMatrix::CDMatrix(double Angle, int iAxis)
{
	double sn = sin(Angle);
	double cs = cos(Angle);

	switch (iAxis)
	{
	case AXIS_X:
		X.Set(1.f, 0.f, 0.f);
		Y.Set(0.f, cs, -sn);
		Z.Set(0.f, sn, cs);
		break;
	case AXIS_Y:
		X.Set(cs, 0.f, sn);
		Y.Set(0.f, 1.f, 0.f);
		Z.Set(-sn, 0.f, cs);
		break;
	case AXIS_Z:
		X.Set(cs, -sn, 0.f);
		Y.Set(sn, cs, 0.f);
		Z.Set(0.f, 0.f, 1.f);
	}
}


// ������� ������ CDMatrix

// �������� ������� ���� �� ������� ����
void CDMatrix::Set(CDVector View)
{
	Z = View;
	double d = Z.Length();

	if (d != 0.0) Z /= (-d);

	X.x = -Z.y;
	X.y = Z.x;
	X.z = 0.0f;

	d = X.Length();

	if (d != 0.0)
		X /= d;
	else
		X.x = 1.0f;

	Y = Z ^ X;
}

// �������� ������� ���� �� ���� �������� X � Y
void CDMatrix::Set(CDVector vX, CDVector vY)
{
	X = vX;
	Y = vY;
	Z = X^Y;
	X.Normalize();
	Y.Normalize();
	Z.Normalize();
}

// �������� ������� ���� �� ���� ��������
void CDMatrix::Set(CDVector vX, CDVector vY, CDVector vZ)
{
	X = vX;
	Y = vY;
	Z = vZ;
}

// ��������� ������ CDMatrix

CDMatrix CDMatrix::operator*(CDMatrix matrix)	//  ������������ ������
{
	matrix = ~matrix;
	return CDMatrix(CDVector(X*matrix.X, X*matrix.Y, X*matrix.Z),
		CDVector(Y*matrix.X, Y*matrix.Y, Y*matrix.Z),
		CDVector(Z*matrix.X, Z*matrix.Y, Z*matrix.Z));
}

CDMatrix CDMatrix::operator&(CDMatrix matrix)	//  ������������ ������
{
	return CDMatrix(CDVector(X*matrix.X, X*matrix.Y, X*matrix.Z),
		CDVector(Y*matrix.X, Y*matrix.Y, Y*matrix.Z),
		CDVector(Z*matrix.X, Z*matrix.Y, Z*matrix.Z));
}

CDMatrix CDMatrix::operator-(CDMatrix matrix)	//  ��������� ������
{
	return CDMatrix(X - matrix.X, Y - matrix.Y, Z - matrix.Z);
}

CDMatrix CDMatrix::operator+(CDMatrix matrix)	//  �������� ������
{
	return CDMatrix(X + matrix.X, Y + matrix.Y, Z + matrix.Z);
}

//----------------------------------------------------------
// ����� CDTransform
//----------------------------------------------------------

// ������������ ������ CDTransform
CDTransform::CDTransform()
{
	T.SetNull();		// ������������� ��������������
	R.SetUnit();
	C.SetNull();
}

CDTransform::CDTransform(CDVector t, CDMatrix r, CDVector c)
{
	T = t; R = r; C = c;
}

void CDTransform::SetUnit()
{
	T.SetNull();		// ������������� ��������������
	R.SetUnit();
	C.SetNull();
}

void CDTransform::Set(CDVector View)
{
	R.Set(View);
}

void CDTransform::Set(CDVector View, CDVector Center)
{
	R.Set(View);
	C = Center;
}

void CDTransform::ChangeCenter(CDVector Center)
{
	T += R * (Center - C) + C - Center;
	C = Center;
}

// ���������� � ������ ������� � ���� ������
/*void CDTransform::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar.Write(this, sizeof(CDTransform));
	}
	else
	{
		ar.Read(this, sizeof(CDTransform));
	}
}*/

// ��������� ������ CDTransform

bool CDTransform::operator==(CDTransform transform)
{
	return (T == transform.T && R == transform.R && C == transform.C);
}

void CDTransform::operator+=(CDVector vector)
{
	T += vector;
}

void CDTransform::operator-=(CDVector vector)
{
	T -= vector;
}

CDTransform CDTransform::operator+(CDVector vector)
{
	return CDTransform(T + vector, R, C);
}

CDTransform CDTransform::operator-(CDVector vector)
{
	return CDTransform(T - vector, R, C);
}

CDNode CDTransform::operator*(CDNode node)
{
	return R*(node - C) + C + T;
}
void CDTransform::operator*=(CDMatrix matrix)
{
	R = R * matrix;
}

//	������������� ��������������
//	N" = tr2 * tr1 * N
//	N" = tr * N
CDTransform CDTransform::operator*(CDTransform tr1)
{
	CDTransform& tr2 = *this;
	return CDTransform(tr2.R*(tr1.T + tr1.C - tr2.C) + tr2.C + tr2.T - tr1.C, tr2.R*tr1.R, tr1.C);
}


CDTransform CDTransform::operator~()
{
	return CDTransform(-(~R*T), ~R, C);
}
