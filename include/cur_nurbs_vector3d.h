////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Nurbs-вектор.
         \en Nurbs-vecor. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_NURBS_VECTOR3D_H
#define __CUR_NURBS_VECTOR3D_H

#include <mb_cart_point3d.h>
#include <reference_item.h>


//------------------------------------------------------------------------------
/** \brief \ru Nurbs-вектор.
           \en Nurbs-vecor. \~
  \details \ru Трёхмерный Nurbs-вектор. \n
           \en Three-Dimensional Nurbs-vecor. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbNURBSVector {
public:
  double * x;
  double * y;
  double * z;
  double * w;

public:
  MbNURBSVector() : x( NULL ), y( NULL ), z( NULL ), w( NULL ) {}
  ~MbNURBSVector(); // \ru освободить память \en free memory 

public:
  bool CatchMemory( ptrdiff_t count, bool bWeight ); // \ru выделить память \en allocate memory 
  void Init( ptrdiff_t i, const MbCartPoint3D & ip, double iw );
  void Init( ptrdiff_t i, double ipx, double ipy, double ipz, double iw );
  void SetZero( ptrdiff_t i );
  void Set( ptrdiff_t i, const MbNURBSVector & p,  ptrdiff_t ip );
  void Dec( ptrdiff_t i, const MbNURBSVector & p1, ptrdiff_t ip1, const MbNURBSVector & p2, ptrdiff_t ip2, double kk );
  void Set( ptrdiff_t i, const MbNURBSVector & p,  ptrdiff_t ip, double kk );

private:
  void FreeMemory(); // \ru освободить память \en free memory 
  MbNURBSVector( const MbNURBSVector & ); // \ru не реализовано \en not implemented 
  void operator = ( const MbNURBSVector & ); // \ru не реализовано \en not implemented 
};


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::Init( ptrdiff_t i, const MbCartPoint3D & ip, double iw )
{
  if ( w != NULL ) {
    x[i] = ( ip.x * iw );
    y[i] = ( ip.y * iw );
    z[i] = ( ip.z * iw );
    w[i] = iw;
  }
  else {
    x[i] = ip.x;
    y[i] = ip.y;
    z[i] = ip.z;
  }
}


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::Init( ptrdiff_t i, double ipx, double ipy, double ipz, double iw )
{
  if ( w != NULL ) {
    x[i] = ( ipx * iw );
    y[i] = ( ipy * iw );
    z[i] = ( ipz * iw );
    w[i] = iw;
  }
  else {
    x[i] = ipx;
    y[i] = ipy;
    z[i] = ipz;
  }
}


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::SetZero( ptrdiff_t i ) {
  x[i] = 0.0;
  y[i] = 0.0;
  z[i] = 0.0;
  if ( w != NULL )
    w[i] = 0.0;
}


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::Set( ptrdiff_t i, const MbNURBSVector & p, ptrdiff_t ip ) {
  x[i] = p.x[ip];
  y[i] = p.y[ip];
  z[i] = p.z[ip];
  if ( w != NULL )
    w[i] = p.w[ip];
}


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::Dec( ptrdiff_t i, 
                                const MbNURBSVector & p1, ptrdiff_t ip1, 
                                const MbNURBSVector & p2, ptrdiff_t ip2, 
                                double kk )
{
  x[i] = ( (p2.x[ip2] - p1.x[ip1]) * kk );
  y[i] = ( (p2.y[ip2] - p1.y[ip1]) * kk );
  z[i] = ( (p2.z[ip2] - p1.z[ip1]) * kk );
  if ( w != NULL )
    w[i] = ( (p2.w[ip2] - p1.w[ip1]) * kk );
}


//------------------------------------------------------------------------------
//
// ---
inline void MbNURBSVector::Set( ptrdiff_t i, const MbNURBSVector & p, ptrdiff_t ip, double kk ) {
  x[i] = ( p.x[ip] * kk );
  y[i] = ( p.y[ip] * kk );
  z[i] = ( p.z[ip] * kk );
  if ( w != NULL )
    w[i] = ( p.w[ip] * kk );
}


#endif // __CUR_NURBS_VECTOR3D_H
