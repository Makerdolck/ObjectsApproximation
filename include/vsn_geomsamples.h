////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Примеры классов объектов, содержащих геометрические данные.
         \en Samples of classes that include geometric data. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GEOMSAMPLES_H
#define __VSN_GEOMSAMPLES_H

#include "vsn_geommeshobject.h"
#include "vsn_global.h"

class MbSolid;

VSN_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
/** \brief \ru Полилиния.
           \en Polyline. \~
  \details \ru Множество точек, которые соединяются между собой ломаной.
           \en Set of points connected by a polyline. \~
  \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomPolylineObject : public GeomObject 
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomPolylineObject();
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomPolylineObject( const GeomPolylineObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomPolylineObject();

public:
  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;
  /// \ru Определить, является ли объект пустым. \en Determine whether the object is empty.
  bool isEmpty() const;

public:
  /// \ru Добавить массив точек для построения полилинии и вернуть идентификатор этой группы точек. \en Add a vector of points of polyline.
  uint AddPolyline( const std::vector<GLfloat>& data );
  /// \ru Добавить массив точек для построения полилинии и вернуть идентификатор этой группы точек. \en Add a vector of points of polyline.
  uint AddPolyline( const std::vector<MbCartPoint3D>& points );
  /// \ru Добавить массив точек для построения полилинии и вернуть идентификатор этой группы точек. \en Add a vector of points of polyline.
  uint AddPolyline( const std::vector<MbFloatPoint3D>& pointsList );

public:
  /// \ru Очистить содержимое полигона и сделать его пустым. \en Clear the object.
  void Clear();
  /// \ru Оператор копирования. \en A copy operator.
  GeomPolylineObject& operator = ( const GeomPolylineObject& other );

protected:
  /// \ru Отрисовать полилинию. \en Draw a polyline.
  virtual void OGLDraw( const RenderState& renderState);
};


//------------------------------------------------------------------------------
/** \brief \ru Отрезок.
           \en Line segment. \~
  \details \ru Линия, ограниченную конечными точками.
           \en A line limited by points. \~
  \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomLineSegmentObject : public GeomPolylineObject 
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomLineSegmentObject();
  /// \ru Конструктор по точкам. \en Constructor by points.
  GeomLineSegmentObject( const MbCartPoint3D& first, const MbCartPoint3D& second );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomLineSegmentObject( const GeomLineSegmentObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomLineSegmentObject();

public:
  /// \ru Инициализация отрезка. \en Line segment initialization.
  void InitSegment( const MbCartPoint3D& first, const MbCartPoint3D& second );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;

public:
  /// \ru Оператор копирования. \en A copy operator.
  GeomLineSegmentObject& operator = ( const GeomLineSegmentObject& other );

private:
  /// \ru Создать каркас. \en Create a wireframe.
  void BuildWireframe();

private:
  MbCartPoint3D m_pntFirst;  // первая точка отрезка
  MbCartPoint3D m_pntSecond; // вторая точка отрезка
};


//------------------------------------------------------------------------------
/** \brief \ru Окружность.
           \en A circle. \~
  \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomCircleObject : public GeomPolylineObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomCircleObject();
  /// \ru Конструктор по радиусу. \en Constructor by radius.
  GeomCircleObject( double radius );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomCircleObject( const GeomCircleObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomCircleObject();

public:
  /// \ru Задать отклонение для расчета полигона. \en Set the deviation for the calculating of the polygon.
  void SetDeviation( double sag );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;

private:
  /// \ru Создать каркас. \en Create a wireframe.
  void BuildWireframe();

private:
  double m_radius; // радиус окружности
  double m_deviation; // коэффициент отклонения для расчета полигонов
};
  


//------------------------------------------------------------------------------
/** \brief \ru Плоский трёхмерный прямоугольник.
           \en Rectangle in space. \~
    \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomRectangleObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomRectangleObject();
  /// \ru Конструктор по параметрам. \en Constructor by parameters.
  GeomRectangleObject( double width, double height );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomRectangleObject( const GeomRectangleObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomRectangleObject();

public:
  /// \ru Выдать ширину прямоугольника. \en Get width.
  double GetWidth() const;
  /// \ru Задать ширину прямоугольника. \en Set width.
  void SetWidth( double width );

  /// \ru Выдать высоту прямоугольника. \en Get height.
  double GetHeight() const;
  /// \ru Задать высоту прямоугольника. \en Set height.
  void SetHeight( double height );

  /// \ru Задать размеры прямоугольника. \en Set rectangle parameters.
  void SetSize( double width, double height );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;
  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();

private:
  /// \ru Отрисовать прямоугольник с помощью OpenGL. \en Draw a rectangle with OpenGL.
  virtual void OGLDraw( const RenderState& );

private:
  /// \ru Создать полигональное тело. \en Create a mesh of rectangle.
  void BuildMeshRectangle();

private:
  double m_width;  /// ширина прямоугольника
  double m_height; /// высота прямоугольника
};


//------------------------------------------------------------------------------
/** \brief \ru Параллелепипед.
           \en A box. \~
  \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomBoxObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomBoxObject( double width, double length, double height );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomBoxObject( const GeomBoxObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomBoxObject();

public:
  /// \ru Выдать ширину параллелепипеда. \en Get width.
  double GetWidth() const;
  /// \ru Задать ширину параллелепипеда. \en Set width.
  void SetWidth( double width );

  /// \ru Выдать длину параллелепипеда. \en Get length.
  double GetLength() const;
  /// \ru Задать длину параллелепипеда. \en Set length.
  void SetLength( double length );

  /// \ru Выдать высоту параллелепипеда. \en Get height.
  double GetHeight() const;
  /// \ru Задать высоту параллелепипеда. \en Set height.
  void SetHeight( double height );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;
  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();

private:
  /// \ru Отрисовать параллелепипед с помощью OpenGL. \en Draw a box with OpenGL.
  virtual void OGLDraw( const RenderState& );

private:
  /// \ru Создать полигональное представление. \en Create a mesh of a box.
  void BuildMeshBox();
  /// \ru Создать каркасное представление. \en Create a wireframe of a box.
  void BuildWireFrameBox();

private:
  double m_width;   /// ширина параллелепипеда
  double m_length;  /// длина параллелепипеда
  double m_height;  /// высота параллелепипеда
};


//------------------------------------------------------------------------------
/** \brief \ru Цилиндр.
           \en A cylinder. \~
    \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomCylinderObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по радиусу, высоте и направлению оси Z. \en Сonstructor by the radius, the height and the direction of the axis Z.
  GeomCylinderObject( double radius, double height, int spaceStep = 70 );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomCylinderObject( const GeomCylinderObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomCylinderObject();

public:
  /// \ru Выдать высоту цилиндра. \en Get height.
  double GetHeight() const;
  /// \ru Задать высоту цилиндра. \en Set height.
  void SetHeight( double height );

  /// \ru Выдать радиус цилиндра. \en Get radius.
  double GetRadius() const;
  /// \ru Задать радиус цилиндра. \en Set radius.
  void SetRadius( double radius );

  /// \ru Выдать шаг сетки цилиндра. \en Get space step of the mesh.
  int GetSpaceStep() const;
  /// \ru Задать шаг сетки цилиндра. \en Set space step of the mesh.
  void SetSpaceStep( int spaceStep );

  /// \ru Определить, замкнут ли цилиндр. \en Determine whether the cylinder is closed.
  bool IsClosed() const;
  /// \ru Установить флаг замкнутости цилиндра. \en Set the flag of the closedness of the cylinder.
  void SetClose( bool clouse );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;
  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();

private:
  /// \ru Отрисовать цилиндр с помощью OpenGL. \en Draw a cylinder with OpenGL.
  virtual void OGLDraw( const RenderState& );

private:
  /// \ru Создать представления цилиндра. \en Create wireframe and mesh representations of the cylinder.
  void BuildMeshAndWire();

private:
  double m_radius; /// радиус цилиндра
  double m_height; /// высота цилиндра по оси Z
  int m_spaceStep; /// шаг сетки цилиндра 
  bool m_bIsClosed; /// признак, что цилиндр замкнутый
};


//------------------------------------------------------------------------------
/** \brief \ru Конус.
           \en A cone. \~
  \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomConeObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomConeObject( double radius, double height, int polyStep = 60 );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomConeObject( const GeomConeObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomConeObject();

public:
  /// \ru Выдать высоту конуса. \en Get height.
  double GetHeight() const;
  /// \ru Задать высоту конуса (должна быть больше нуля). \en Set the height (must be greater than zero).
  void SetHeight( double height );

  /// \ru Выдать радиус конуса. \en Get radius.
  double GetRadius() const;
  /// \ru Задать радиус конуса (должен быть больше нуля). \en Set the radius (must be greater than zero).
  void SetRadius( double radius );

  /// \ru Выдать шаг полигона. \en Get polygon step.
  int GetPolygonStep() const;
  /// \ru Задать шаг полигона (должен быть больше нуля). \en Set the polygon step (must be greater than zero).
  // устновить шаг поигона должен быть больше 0
  void SetPolygonStep( int polyStep );

  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;

  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();

private:
  /// \ru Отрисовать конус с помощью OpenGL. \en Draw a cone with OpenGL.
  virtual void OGLDraw( const RenderState& );

private:
  /// \ru Создать проволочное и сеточное представления конуса. \en Create wireframe and mesh representations of the cone.
  void BuildMeshAndWire();

private:
  double m_radius;   /// радиус конуса
  double m_height;   /// высота конуса (по Z вектору) 
  int    m_polyStep; /// шаг полигона
};


//------------------------------------------------------------------------------
/** \brief \ru Сфера.
           \en Sphere. \~
    \ingroup Vision_Geometry
*/
// ---
///////////////////////////////////////////////////////////////////////////////
//
// класс GeomSphereObject представляет 3D сферу
//
///////////////////////////////////////////////////////////////////////////////
class VSN_CLASS GeomSphereObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  GeomSphereObject( double radius );
  /// \ru Конструктор копирования. \en Copy-constructor.
  GeomSphereObject( const GeomSphereObject& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeomSphereObject();

public:
  /// \ru Задать радиус сферы. \en Set radius.
  void SetRadius( double rad );
  /// \ru Задать отклонение для расчета полигона. \en Set polygon deviation.
  void SetDeviation( double sag );

public:
  /// \ru Сделать копию объекта. \en Create a copy of the object.
  virtual GeomObject* Duplicate() const;
  /// \ru Выдать габаритный куб. \en Get a bounding box.
  virtual const MbCube& GetBoundingBox();

private:
  /// \ru Отрисовать сферу с помощью OpenGL. \en Draw a sphere with OpenGL.
  virtual void OGLDraw( const RenderState& );

private:
  /// \ru Создать сферическое тело. \en Create a sphere  solid.
  MbSolid* CreateSphere( double rad ) const;
  /// \ru Создать проволочное и сеточное представления сферы. \en Create wireframe and mesh representations of the sphere.
  void BuildBodySolid();

private:
  double m_radius;   /// радиус сферы
  double m_deviation; /// коэффициент отклонения для расчета полигонов
};
VSN_END_NAMESPACE


#endif /* __VSN_GEOMSAMPLES_H */
