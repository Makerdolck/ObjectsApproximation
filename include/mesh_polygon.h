////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерный полигон.
         \en Two-dimensional polygon. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_POLYGON_H
#define __MESH_POLYGON_H


#include <templ_s_array.h>
#include <io_tape.h>
#include <mesh_float_point.h>
#include <mb_rect.h>


#define NO_LENGTH -1.0


class  MATH_CLASS  MbPolyline;


//------------------------------------------------------------------------------
/** \brief \ru Двумерный полигон.
           \en Two-dimensional polygon. \~
  \details \ru Двумерный полигон представляет собой упорядоченное множество точек в 
  двумерном пространстве, последовательное соединение которых даёт ломаную линию, 
  аппроксимирующую некоторый двумерный объект. \n
           \en Two-dimensional polygon is an ordered set of points in 
  two-dimensional space, sequential connection of them produces a polyline 
  that approximate a two-dimensional object. \n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbPolygon {
private:
  SArray<MbFloatPoint> points;  ///< \ru Множество точек полигона. \en Array of points of a polygon. 
  double               sag;     ///< \ru Стрелка прогиба, с которой расчитан полигон. \en Sag used for calculation of a polygon. 
  mutable MbRect       rect;    ///< \ru Габарит полигона (не записываeтся в поток и не читаeтся). \en Bounding box of polygon (not read from stream and not written to stream). 
  mutable double       length;  ///< \ru Длина полигона   (не записываeтся в поток и не читаeтся). \en Length of a polygon (not read from stream and not written to stream). 

public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPolygon();
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbPolygon( const MbPolygon & );
  /// \ru Деструктор. \en Destructor. 
  ~MbPolygon();

public:
  /// \ru Обеспечить резерв памяти под additionalSpace элементов. \en Reserve memory for additionalSpace elements. 
  void    Reserve( size_t additionalSpace ); 
  /// \ru Установить максимальное из приращений. \en Set the maximum increment. 
  void    SetMaxDelta( uint16 delta ); 
  /// \ru Удалить лишнюю память. \en Free the unnecessary memory. 
  void    Adjust(); 
  /// \ru Вернуть количество точек. \en Get count of points. 
  size_t  Count() const { return points.Count(); } 

  /// \ru HardFlush очистить полигон (освободить всю память). \en HardFlush clear the polygon (free all the memory). 
  void    HardFlushPoints();   
  /// \ru Flush очистить полигон \en Flush clear polygon 
  void    SimpleFlushPoints(); 

  /// \ru Добавить новую точку. \en Add a new point. 
  void    AddPoint   ( double x, double y );  
  /// \ru Добавить новую точку. \en Add a new point. 
  void    AddPoint   ( const MbCartPoint & ); 
  /// \ru Добавить полигон. \en Add a polygon. 
  void    AddPolygon ( const MbPolygon & );
  /// \ru Удалить точку. \en Remove the point. 
  bool    RemovePoint( size_t index );

  /// \ru Выдать очередную точку. \en Get the next point. 
  bool    GetPoint ( size_t i, MbCartPoint &  ) const;
  /// \ru Выдать очередную точку. \en Get the next point. 
  bool    GetPoint ( size_t i, MbFloatPoint & ) const;
  /// \ru Изменить точку. \en Change a point. 
  bool    SetPoint ( size_t i, const MbCartPoint &  pnt );
  /// \ru Изменить точку. \en Change a point. 
  bool    SetPoint ( size_t i, const MbFloatPoint & pnt );
  /// \ru Выдать очередную точку. \en Get the next point. 
  bool    GetCoords( size_t i, double & x, double & y ) const;
  /// \ru Заполнить контейнер. \en Fill the container. 
  void    GetPoints( SArray<MbCartPoint>  & pnts ) const;
  /// \ru Заполнить контейнер. \en Fill the container. 
  void    GetPoints( SArray<MbFloatPoint> & pnts ) const;
  /// \ru Выдать точку. \en Get point. 
  const MbFloatPoint & GetPoint( size_t i ) const { return points[i]; } 

  /// \ru Сдвинyть полигон. \en Move the polygon. 
  void    Move  ( const MbVector & ); 
  /// \ru Повернуть полигон вокруг точки. \en Rotate a polygon about a point. 
  void    Rotate( const MbCartPoint &, const MbDirection & ); 
  /// \ru Преобразовать полигон согласно матрице. \en Transform a polygon according to the matrix. 
  void    Transform( const MbMatrix & matr ); 
  /// \ru Инверсия направления. \en Inverse the direction. 
  void    Inverse();

  /// \ru Вернуть габарит. \en Get bounding box. 
  const MbRect & GetRect() const;
  /// \ru Вернуть длину полигона. \en Get length of the polygon. 
  double  GetLength() const;
  /// \ru Получить стрелку прогиба. \en Get sag. 
  double  GetSag() const     { return sag; }
  /// \ru Установить стрелку прогиба. \en Set sag. 
  void    SetSag( double s ) { sag = s;    }
  /// \ru Является ли полигон выпуклым. \en Whether the polygon is convex. 
  bool    IsConvex() const;
  /// \ru Площадь полигона. \en Area of the polygon. 
  double  Area() const;

  void    operator = ( const MbPolygon & );

  // \ru Создать ломаную на основе полигона. \en Create a polyline from the polygon. 
  MbPolyline *    ConvertToPolyline() const; 

  /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const MbFloatPoint * GetAddr() const { return points.GetAddr(); }

private:
  // \ru Рассчитать габарит. \en Calculate bounding box. 
  void    CalculateRect() const;
  // \ru Рассчитать длину полигона. \en Calculate length of the polygon. 
  void    CalculateLength() const;
  // \ru Сбросить временные данные. \en Reset temporary data. 
  void    ResetMutable() const;

  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX( MbPolygon, MATH_FUNC_EX )
  DECLARE_NEW_DELETE_CLASS( MbPolygon )
  DECLARE_NEW_DELETE_CLASS_EX( MbPolygon )
};


//------------------------------------------------------------------------------
// \ru Вернуть габарит. \en Get bounding box. 
// ---
inline const MbRect & MbPolygon::GetRect() const
{
  if ( rect.IsEmpty() )
    CalculateRect();
  return rect;
}


//------------------------------------------------------------------------------
// \ru Вернуть длину полигона. \en Get length of the polygon. 
// ---
inline double MbPolygon::GetLength() const
{
  if ( length < 0.0 )
    CalculateLength();
  return length;
}


//------------------------------------------------------------------------------
// \ru Сбросить временные данные. \en Reset temporary data. 
// ---
inline void MbPolygon::ResetMutable() const
{
  rect.SetEmpty();
  length = NO_LENGTH;
}


//------------------------------------------------------------------------------
// \ru Выдать очередную точку. \en Get the next point. 
// ---
inline bool MbPolygon::GetPoint( size_t i, MbCartPoint & pnt ) const
{
  if ( i < points.Count() ) {
    pnt.Init( points[i].x, points[i].y );
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Выдать очередную точку. \en Get the next point. 
// ---
inline bool MbPolygon::GetPoint( size_t i, MbFloatPoint & pnt ) const
{
  if ( i < points.Count() ) {
    pnt.x = points[i].x;
    pnt.y = points[i].y;
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить очередную точку. \en Set the next point. 
// ---
inline bool MbPolygon::SetPoint( size_t i, const MbCartPoint & pnt )
{
  if ( i < points.Count() ) {
    MbFloatPoint & p = points[i];
    p.x = ::D2F( pnt.x );
    p.y = ::D2F( pnt.y );
    ResetMutable();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Установить очередную точку. \en Set the next point. 
// ---
inline bool MbPolygon::SetPoint( size_t i, const MbFloatPoint & pnt )
{
  if ( i < points.Count() ) {
    MbFloatPoint & p = points[i];
    p.x = pnt.x;
    p.y = pnt.y;
    ResetMutable();
    return true;
  }
  return false;
}


//------------------------------------------------------------------------------
// \ru Выдать очередную точку. \en Get the next point. 
// ---
inline bool MbPolygon::GetCoords( size_t i, double & x, double & y ) const
{
  if ( i < points.Count() ) {
    MbCartPoint p( points[i] );
    x = p.x;
    y = p.y;
    return true;
  }
  return false;
}


#undef NO_LENGTH


#endif

