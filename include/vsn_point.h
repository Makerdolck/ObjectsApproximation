///////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс Point определяет точку на плоскости и использует целочисленную точность.
         \en Point class defines point on the plane using integer precision. \~
*/
///////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_POINT_H
#define __VSN_POINT_H

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

/* Point */
//----------------------------------------------------------------------------
//
/** \brief \ru Класс Point определяет точку на плоскости и использует целочисленную точность.
           \en Point class defines point on the plane using integer precision. \~
    \details \ru Точка задается координатами х и у, которые можно получить с помощью функций GetX() и GetY(). 
                 Функция IsNull() возвращает истину, если обе координаты х и у равны 0. Координаты могут быть установлены или изменены 
                 с помощью функций SetX() и SetY() или в качестве альтернативы функциями InitX() и InitY(), которые возвращают ссылки на координаты. \n
             \en Point is defined with X- and Y-coordinates that can be returned with GetX() and GetY() functions.
                 IsNull() function returns True, if both X- and Y-coordinates are 0. The coordinates can be set or changed
                 with SetX() and SetY() functions, or alternatively InitX() and InitY() functions that return coordinate references. \n \~
    \ingroup Vision_Base
*/
// ---
class VSN_CLASS Point
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  Point();
  /// \ru Специальный конструктор для инициализации координат x и y. \en Special constructor for initialization X- and Y-coordinates.
  Point(int x, int y);

public:
  /// \ru Вернуть true, если обе координаты х и у равны 0, в противном случае возвращает false. \en True if both X- and Y-coordinates are 0.
  bool IsNull() const;

  /// \ru Вернуть координату х. \en Returns X-coordinate.
  int GetX() const;
  /// \ru Установить координату х. \en Sets X-coordinate.
  void SetX(int x);

  /// \ru Вернуть координату y. \en Returns Y-coordinate.
  int GetY() const;
  /// \ru Установить координату y. \en Sets Y-coordinate.
  void SetY(int y);

  /// \ru Инициализация координаты х. \en Initializes X-coordinate.
  int& InitX();
  /// \ru Инициализация координаты y. \en Initializes Y-coordinate.
  int& InitY();

  /// \ru Добавить указанную точку в эту точку и вернуть ссылку на эту точку. \en Adds the specified point to the given point and returns point reference.
  Point& operator += (const Point& p);
  /// \ru Вычесть указанную точку из этой точки и вернуть ссылку на эту точку. \en Subtracts the specified point from the given point and returns point reference.
  Point& operator -= (const Point& p);
  /// \ru Вернуть true, если указанная точка и эта точка равны, в противном случае возвращает false. \en True if the specified point and the given point are equal.
  bool operator == (const Point& p);
  /// \ru Вернуть true если указанная точка и эта точка не равны, в противном случае возвращает false. \en True if the specified point and the given point are not equal.
  bool operator != (const Point& p);
  const Point operator + (const Point& p);
  const Point operator - (const Point& p);

private:
  int m_xPnt; ///< \ru Координата x точки. \en Point X-coordinate. 
  int m_yPnt; ///< \ru Координата y точки. \en Point Y-coordinate. 
};


inline Point::Point() 
  : m_xPnt(0)
  , m_yPnt(0) 
{
}

inline Point::Point(int x, int y) 
  : m_xPnt(x)
  , m_yPnt(y) 
{
}

inline bool Point::IsNull() const
{ 
  return m_xPnt == 0 && m_yPnt == 0; 
}

inline int Point::GetX() const
{ 
  return m_xPnt; 
}

inline int Point::GetY() const
{ 
  return m_yPnt; 
}

inline void Point::SetX(int x)
{ 
  m_xPnt = x; 
}

inline void Point::SetY(int y)
{ 
  m_yPnt = y;
}

inline int& Point::InitX()
{ 
  return m_xPnt; 
}

inline int& Point::InitY()
{ 
  return m_yPnt; 
}

inline Point& Point::operator += (const Point& p)
{ 
  m_xPnt += p.m_xPnt; 
  m_yPnt += p.m_yPnt; 
  return *this; 
}

inline Point &Point::operator -= (const Point& p)
{ 
  m_xPnt -= p.m_xPnt; 
  m_yPnt -= p.m_yPnt; 
  return *this; 
}

inline bool Point::operator == ( const Point& p )
{ 
  return m_xPnt == p.m_xPnt && m_yPnt == p.m_yPnt; 
}

inline bool Point::operator != (const Point& p)
{ 
  return m_xPnt != p.m_xPnt || m_yPnt != p.m_yPnt; 
}

inline const Point Point::operator + (const Point& p)
{ 
  return Point(m_xPnt + p.m_xPnt, m_yPnt + p.m_yPnt); 
}

inline const Point Point::operator - (const Point& p)
{ 
  return Point(m_xPnt - p.m_xPnt, m_yPnt - p.m_yPnt); 
}

VSN_END_NAMESPACE

#endif /*__VSN_POINT_H*/
