////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Базовый класс для структур данных сетки (#MbMesh). Вершина. Полигон.
         \en Base class for mesh data structures (#MbMesh). Vertex. Polygon. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_PRIMITIVE_H
#define __MESH_PRIMITIVE_H


#include <mesh_float_point3d.h>
#include <mesh_polygon.h>
#include <templ_rp_array.h>
#include <attribute_container.h>
#include <mb_cube.h>
#include <mb_data.h>
#include <model_item.h>


class  MATH_CLASS MbCurve;
class  MATH_CLASS MbPolyline3D;


//------------------------------------------------------------------------------
/** \brief \ru Типы примитивов.
           \en Types of primitives. \~
  \details \ru Типы примитивов полигонального объекта.
           \en Get type of primitive of polygonal object. \~ 
  \ingroup Polygonal_Objects
*/
// ---
enum MbePrimitiveType {
  pt_Apex3D,           ///< \ru Апекс. \en Apex. 
  pt_Polygon3D,        ///< \ru Полигон. \en Polygon. 
  pt_Grid,             ///< \ru Триангуляция. \en Triangulation. 
};


//------------------------------------------------------------------------------
/** \brief \ru Примитив.
           \en Primitive. \~
  \details \ru Родительский класс элемента полигонального объекта служит для 
  аппроксимации геометрического объекта. Каждый экземпляр класса MbPrimitive несет имя и 
  указатель на исходный геометрический объект (если есть такой) и атрибуты.\n
  MbPrimitive является предком для триангуляции (#MbGrid), полигона (#MbPolygon3D) и апекса (#MbApex3D).
           \en Parent class for polygigonal elements is used to represent the approximated geometric object. 
  Each instance of MbPrimitive class has name and pointer to the source geometric object (if it exists) and attributes.\n
  MbPrimitive is ancestor for triangulation (#MbGrid), polygon (#MbPolygon3D), and apex (#MbApex3D). \~
  \ingroup Polygonal_Objects
*/ 
// ---
class MATH_CLASS MbPrimitive : public MbAttributeContainer, public MbRefItem {
protected:
  SimpleName        name;       ///< \ru Имя примитива. \en Name of primitive. 
  const MbRefItem * parentItem; ///< \ru Породивший объект (не владеем). \en Begetter object (don't own). 
  MbeRefType        type;       ///< \ru Тип примитива. \en Type of primitive.

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbPrimitive( const MbPrimitive & init );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPrimitive( const MbPrimitive & init, MbRegDuplicate * iReg );
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPrimitive();
public:
  /// \ru Деструктор \en Destructor 
  virtual ~MbPrimitive();

public:

  /** \ru \name Общие функции примитива.
      \en \name Common functions of primitive.
  \{ */  
  /// \ru Получить тип объекта. \en Get the object type. 
  virtual MbePrimitiveType  Type() const = 0;

  /** \brief \ru Создать копию примитива.
             \en Create a copy of primitive. \~
  \details \ru Создать копию примитива с использованием регистратора. 
    Регистратор используется для предотвращения многократного копирования примитива, 
    входящего в состав нескольких объектов копируемых одновременно.
    При копировании одиночного объекта или набора не связанных между собой объектов допустимо не использовать регистратор.
    Регистратор необходимо использовать, если надо последовательно копировать несколько взаимосвязанных объектов.
    Взаимосвязь представляет собой наличие в объектах ссылок на общие примитивы. 
    Тогда, при копировании без использования регистратора, можно получить набор копий, 
    содержащих ссылки на разные копии одного и того же вложенного примитива, что ведет к потере связи между копиями.
           \en Create a copy of primitive using the registrator. 
    Registrator is used for preventing multiple copying of primitive 
    contained in several simultaneously copied objects.
    It is allowed not to use the registrator while copying a single object or a set of disconnected objects.
    The registrator must be used to copy several correlated objects successively.
    Correlation is an existence of references to common primitives in objects. 
    Then, while copying without using the registrator, one can get a set of copies 
    which contains references to the different copies of a single included primitive, what leads to loss of connection between the copies. \~
  \param[in] iReg - \ru Регистратор.
                    \en Registrator. \~
  \return \ru Копия объекта.
          \en The object copy. \~
  */
  virtual MbPrimitive &     Duplicate( MbRegDuplicate * iReg = NULL ) const = 0; 

  /** \brief \ru Преобразовать примитив согласно матрице.
             \en Transform primitive according to the matrix. \~
  \details \ru Преобразовать примитив согласно матрице c использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих трансформации.
           \en Transform primitive according to the matrix using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to transformation. \~
  \param[in] matr - \ru Матрица преобразования.
                    \en A transformation matrix. \~
  */
  virtual void    Transform( const MbMatrix3D & matr ) = 0;

  /** \brief \ru Сдвинуть примитив вдоль вектора.
             \en Move primitive along a vector. \~
  \details \ru Сдвинуть примитив вдоль вектора с использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих сдвигу.
           \en Move primitive along the vector using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to moving. \~
  \param[in] to   - \ru Вектор сдвига.
                    \en Movement vector. \~
  */
  virtual void    Move( const MbVector3D & to ) = 0;

  /** \brief \ru Повернуть примитив вокруг оси на заданный угол.
             \en Rotate primitive about an axis by a given angle. \~
  \details \ru Повернуть примитив вокруг оси на заданный угол с использованием регистратора. 
    Регистратор служит для предотвращения многократного преобразования примитива,
    входящего в состав нескольких объектов, трансформируемых одновременно.
    Допустимо использовать функцию без регистратора, если надо преобразовать одиночный объект.
    Если надо преобразовать набор взаимосвязанных объектов, необходимо использовать регистратор для 
    предотвращения повторного преобразования вложенных примитивов, поскольку не исключена ситуация,
    когда несколько объектов из набора содержат ссылки на один или несколько общих примитивов, подлежащих повороту.
           \en Rotate primitive about an axis by a given angle using the registrator. 
    The registrator is used for preventing multiple transformation of primitive
    contained in several simultaneously transformed objects.
    The function can be used without the registrator to transform a single object.
    The registrator must be used to transform a set of interdependent objects to 
    prevent repeated transformation of the nested primitives, since it is not ruled out
    that several objects from the set contain references to one or several common primitives subject to rotation. \~
  \param[in] axis - \ru Ось поворота.
                    \en The rotation axis. \~
  \param[in] angle - \ru Угол поворота.
                     \en The rotation angle. \~
  */
  virtual void    Rotate( const MbAxis3D & axis, double angle ) = 0;

  // \ru Тип контейнера атрибутов. \en Type of attribute container. 
  virtual MbeImplicationType ImplicationType() const; 
  /// \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
  virtual void    AddYourGabaritTo( MbCube & r ) const = 0; 
  /// \ru Вычислить расстояние до точки. \en Calculate distance to point.  
  virtual double  DistanceToPoint( const MbCartPoint3D & pnt ) const = 0; 
  /// \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.  
  virtual double  DistanceToLine( const MbAxis3D & axis, double maxDistance, double & t ) const = 0; 
  /// \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    GetProperties( MbProperties & ) = 0;
  /// \ru Записать свойства объекта. \en Set properties of the object. 
  virtual void    SetProperties( const MbProperties & ) = 0;

          // \ru Выдать имя примитива. \en Get name of primitive. 
          SimpleName GetPrimitiveName() const { return name; } 
          // \ru Установить имя примитива. \en Set name of primitive. 
          void    SetPrimitiveName( SimpleName n ) { name = n; } 
          /// \ru Выдать породивший примитив объект. \en Get begetter object of primitive. 
  const   MbRefItem * GetItem() const { return parentItem; }
          /// \ru Установить породивший примитив объект. \en Set begetter object of primitive. 
          void    SetItem( const MbRefItem * g ) { parentItem = g; }
          /// \ru Дать тип объекта. \en Get type of object. 
          MbeRefType GetPrimitiveType() const { return type; } 
          /// \ru Установить тип объекта. \en Set type of object. 
          void    SetPrimitiveType( MbeRefType t ) { type = t; } 

          /// \ru Удовлетворяет ли примитив критериям поиска ближайшего объекта? \en Does the primitive satisfy the nearest object search criteria? 
          bool    NearestType( MbeSpaceType sType, MbeTopologyType tType, MbePlaneType pType ) const;

          /// \ru Получить пространственный объект, для которого построен примитив. \en Get spatial object for which the primitive is constructed. 
  const   MbSpaceItem * SpaceItem() const { return ((parentItem != NULL && parentItem->RefType() == rt_SpaceItem) ? (const MbSpaceItem *)parentItem : NULL); }
          /// \ru Получить двумерный объект, для которого построен примитив. \en Get two-dimensional object for which the primitive is constructed. 
  const   MbPlaneItem * PlaneItem() const { return ((parentItem != NULL && parentItem->RefType() == rt_PlaneItem) ? (const MbPlaneItem *)parentItem : NULL); }
          /// \ru Получить топологический объект, для которого построен примитив. \en Get the topological object for which the primitive is constructed. 
  const   MbTopItem * TopItem() const { return ((parentItem != NULL && parentItem->RefType() == rt_TopItem) ? (const MbTopItem *)parentItem : NULL); }
          /// \ru Получить объект геометрической модели, для которого построен примитив. \en Get geometric model object for which the primitive is constructed. 
  const   MbItem * Item() const { return ((parentItem != NULL && parentItem->RefType() == rt_Item) ? (const MbItem *)parentItem : NULL); }

          /// \ru Чтение примитива из потока. \en Reading of primitive from the stream.  
          void    PrimitiveRead ( reader & in  ); 
          /// \ru Запись примитива в поток. \en Writing of primitive to the stream.  
          void    PrimitiveWrite( writer & out ) const; 
  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
        void      operator = ( const MbPrimitive & init );
};


//------------------------------------------------------------------------------
/** \brief \ru Апекс (точка).
           \en Apex (point). \~
  \details \ru Апекс определяет положение точки, вершины или другого точечного объекта в пространстве.\n
           \en Apex defines position of a point, of a vertex or the other point-object in space.\n \~ 
  \ingroup Polygonal_Objects
*/ 
// ---
class MATH_CLASS MbApex3D : public MbPrimitive {
private :
  MbFloatPoint3D vertex; ///< \ru Положение апекса. \en Position of the apex. 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent a copying by default 
  MbApex3D( const MbApex3D & init ); 
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbApex3D( const MbApex3D & init, MbRegDuplicate * iReg );
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbApex3D() : MbPrimitive(), vertex() {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbApex3D( const MbFloatPoint3D & vert ) : MbPrimitive(), vertex( vert ) {}
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbApex3D( const MbCartPoint3D  & vert ) : MbPrimitive(), vertex( vert ) {}
  /// \ru Деструктор \en Destructor 
  virtual ~MbApex3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  virtual MbePrimitiveType Type() const; // \ru Тип объекта. \en A type of an object. 
  virtual MbApex3D & Duplicate( MbRegDuplicate * iReg = NULL ) const; // \ru Создать копию объекта. \en Create a copy of the object. 
  virtual void    Transform( const MbMatrix3D & matr );               // \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  virtual void    Move     ( const MbVector3D & to );                 // \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  virtual void    Rotate   ( const MbAxis3D & axis, double angle );   // \ru Повернуть вокруг оси на угол. \en Rotate about an axis by an angle. 
  virtual void    AddYourGabaritTo( MbCube & r ) const;               // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  virtual double  DistanceToPoint( const MbCartPoint3D & pnt ) const; // \ru Вычислить расстояние до точки. \en Calculate distance to the point.  
  virtual double  DistanceToLine( const MbAxis3D & axis, double maxDistance, double & t ) const;  // \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.  
  virtual void    GetProperties( MbProperties &properties );          // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties &properties );          // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \ru \name Функции апекса.
      \en \name Functions of the apex.
  \{ */  
          /// \ru Выдать точку. \en Get the point. 
  const   MbFloatPoint3D & GetPoint() const { return vertex; } 
          /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
          void    Init( const MbCartPoint3D  & vert );
          /// \ru Инициализировать апекс по точке. \en Initialize apex by a point.  
          void    Init( const MbFloatPoint3D & vert );
          /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
          void    GetPoint( MbCartPoint3D & p ) const { p.Init( vertex.x, vertex.y, vertex.z ); }
          /// \ru Инициализировать точку по апексу. \en Initialize point by an apex.  
          void    GetPoint( MbFloatPoint3D & p ) const { p = vertex; }
          /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
          bool    IsSame( const MbApex3D & init, double eps ) const;

  /** \} */
private:
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbApex3D & init );

  DECLARE_NEW_DELETE_CLASS( MbApex3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbApex3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX( MbApex3D, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX( MbApex3D, MATH_FUNC_EX );
}; // MbApex3D


//------------------------------------------------------------------------------
/** \brief \ru Полигон.
           \en Polygon. \~
  \details \ru Полигон представляет собой упорядоченное множество точек в пространстве, 
    последовательное соединение которых даёт ломаную линию, аппроксимирующую некоторый объект или часть объекта. \n
           \en Polygon is an ordered set of points in space, 
    sequential connection of these points produces polyline that approximates an object or part of an object. \n \~ 
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbPolygon3D : public MbPrimitive {
private :
  SArray<MbFloatPoint3D> points; ///< \ru Множество точек полигона. \en Array of points of a polygon. 
  MbStepData             stepData;    ///< \ru Параметры расчета полигона (стрелка прогиба или угол отклонения). \en Parameters of polygon calculation (sag or angle of deviation). 
  /** \brief \ru Габаритный куб объекта (не записывается в поток и не читается). 
             \en Bounding box of a polygon (not read from stream and not written to stream). \~
      \details \ru Габаритный куб объекта рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
               \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube         cube;   ///< \ru Габаритный куб полигона (не записывается в поток и не читается). \en . 

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию \en Declaration without implementation of the copy-constructor to prevent copying by default 
  MbPolygon3D( const MbPolygon3D & );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPolygon3D( const MbPolygon3D &, MbRegDuplicate * ); 
public :
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbPolygon3D();
  /// \ru Деструктор \en Destructor 
  virtual ~MbPolygon3D();

public:

  // \ru Общие функции примитива. \en Common functions of the primitive. 
  virtual MbePrimitiveType Type() const; // \ru Вернуть тип объекта \en Get the object type. 
  virtual MbPolygon3D &    Duplicate( MbRegDuplicate * iReg = NULL ) const; // \ru Создать копию объекта \en Create a copy of the object 
  virtual void    Transform( const MbMatrix3D & );             // \ru Преобразовать полигон согласно матрице \en Transform polygon according to the matrix 
  virtual void    Move     ( const MbVector3D & );             // \ru Сдвиг полигона \en Translation of the polygon. 
  virtual void    Rotate   ( const MbAxis3D &, double angle ); // \ru Поворот полигона вокруг оси \en Rotation of the polygon around an axis 
  virtual void    AddYourGabaritTo( MbCube & ) const;        // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  virtual double  DistanceToPoint( const MbCartPoint3D & ) const; // \ru Вычислить расстояние до точки. \en Calculate distance to the point.  
  virtual double  DistanceToLine( const MbAxis3D &, double maxDistance, double & t ) const;  // \ru Вычислить расстояние до оси. \en Calculate the distance to the axis.  
  virtual void    GetProperties( MbProperties & );   // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties & );   // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \ru \name Функции полигона.
      \en \name Functions of polygon.
  \{ */  
          /// \ru Выдать размер занимаемой памяти. \en Get the size of taken memory. 
          size_t  SizeOf() const; 
          /// \ru Зарезервировать место для полигона. \en Reserve memory for polygon. 
          void    Reserve( size_t cnt )     { points.Reserve( cnt ); }
          /// \ru Задать величину наращивания памяти. \en Set increment of memory allocation. 
          void    SetMaxDelta( size_t cnt ) { points.SetMaxDelta( (uint16)cnt ); }
          /// \ru Удалить лишнюю память. \en Free the unnecessary memory. 
          void    Adjust()                  { points.Adjust(); } 
          // \ru Очистить полигон удалив все точки. \en Clear the polygon by deleting all the points. 
          void    Flush();
          // \ru Выдать количество точек. \en Get count of points. 
          size_t  Count() const { return points.size(); } 
          // \ru Добавить точку в конец полигона. \en Add point to the end of the polygon. 
          void    AddPoint( const MbCartPoint3D  & dpnt )  { MbFloatPoint3D fpnt( dpnt.x, dpnt.y, dpnt.z ); points.push_back( fpnt ); cube |= dpnt; }
          // \ru Добавить точку в конец полигона. \en Add point to the end of the polygon. 
          void    AddPoint( const MbFloatPoint3D & fpnt )  { points.push_back( fpnt ); cube |= fpnt; }

          /// \ru Установить точку с номером. \en Set point by index. 
          template <class Point>
          void    SetPoint( size_t i, Point & pnt ) { points[i].x = (float)pnt.x; points[i].y = (float)pnt.y; points[i].z = (float)pnt.z; cube.SetEmpty(); }
          /// \ru Выдать точку по её номеру. \en Get point by its index. 
          template <class Point>
          void    GetPoint( size_t i, Point & pnt ) const { pnt.x = points[i].x; pnt.y = points[i].y; pnt.z = points[i].z; }

          /// \ru Выдать точку по её номеру. \en Get point by its index. 
  const   MbFloatPoint3D & GetPoint( size_t i ) const { return points[i]; }

          /// \ru Выдать все точки полигона. \en Get all the points of the polygon. 
          template<class Points>
          void    GetPoints( Points & pnts ) const
          {
            size_t cnt = points.size();
            pnts.clear();
            pnts.reserve( cnt );
            for ( size_t k = 0; k < cnt; k++ ) {
              pnts.push_back( points[k] );
            }
          }
          /**\ru Проверить, лежат ли точки полигона в одной плоскости c заданной точностью metricAccuracy.
              Если да, то инициализировать плоскость plane.
             \en Check whether all points of polygon lie on the same plane with the given metricAccuracy accuracy.
              if so, then initialize 'plane' plane. \~
          */
          bool    IsPlanar( MbPlacement3D & plane, double metricAccuracy = Math::metricRegion ) const;
          /// \ru Если точки полигона лежат в одной плоскости, то инициализировать plane и заполнить полигон poly. \en If points of polygon lie on the same plane, then initialize 'plane' and fill the 'poly' polygon. 
          bool    GetPlanePolygon( MbPlacement3D & plane, MbPolygon & poly ) const;
          /// \ru Проверить наличие точек в объекте. \en Check existence of points in object. 
          bool    IsComplete() const { return (points.size() > 0); }

          /// \ru Выдать параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Get the parameter of triangulation calculation. 
  const   MbStepData &  GetStepData() const { return stepData; }
          /// \ru Установить параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Set the parameter of triangulation calculation. 
          void          SetStepData( const MbStepData & stData ) { stepData = stData; }
          /// \ru Установить параметр расчета полигона (стрелку прогиба или угол отклонения). \en Set the parameter of polygon calculation (sag or angle of deviation). 
          void          SetStepBySag( double s ) { stepData.InitStepBySag( s ); }

          /// \ru Добавить к полигону полигон с удалением совпадающих точек стыка. \en Add a polygon to the polygon with removing the coincident points of joint. 
          void    AddPolygon( const MbPolygon3D & );
          /// \ru Добавить к полигону полигон с удалением совпадающих точек стыка. \en Add a polygon to the polygon with removing the coincident points of joint. 
          void    operator += ( const MbPolygon3D & );
          /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
          bool    IsSame( const MbPolygon3D &, double eps ) const;

          /// \ru Создать ломаную по полигону. \en Create a polyline on the base of the polygon. 
          MbPolyline3D * CreatePolyline() const;

  /** \} */

          /// \ru Выдать адрес начала массива. \en Get the address of the beginning of the array. 
  const   MbFloatPoint3D * GetAddr() const { return points.begin(); }

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbPolygon3D & );

  DECLARE_NEW_DELETE_CLASS( MbPolygon3D )
  DECLARE_NEW_DELETE_CLASS_EX( MbPolygon3D )
  KNOWN_OBJECTS_RW_REF_OPERATORS_EX( MbPolygon3D, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX( MbPolygon3D, MATH_FUNC_EX );
}; // MbPolygon3D


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона.
           \en Calculate squared distance from straight line to segment of polygon. \~
  \details \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона, заданного начальной и конечной точками. \n
    Возвращает значение параметра ближайшей точки на линии tRes и квадрат расстояния от этой точки до сегмента. \n
    Возвращает значение параметра ближайшей точки на линии tRes и квадрат расстояния от этой точки до сегмента. \n
           \en Calculate squared distance from straight line to segment of polygon defined by start and end points. \n
    Returns value of parameter of nearest point on tRes line and squared distance from this point to the segment. \n
    Returns value of parameter of nearest point on tRes line and squared distance from this point to the segment. \n \~
  \param[in] seg_base - \ru Начальная точка сегмента.
                        \en Start point of the segment. \~
  \param[in] seg_end - \ru Начальная точка сегмента.
                       \en Start point of the segment. \~
  \param[in] line - \ru Прямая линия, до которой вычисляется расстояние.
                    \en Straight line to calculate the distance to. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки на линии.
                     \en Value of the nearest point parameter on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToSegmentDistanceSquared( const MbFloatPoint3D & seg_base, 
                                               const MbFloatPoint3D & seg_end,
                                               const MbFloatAxis3D &  line,
                                                     float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона.
           \en Calculate squared distance from straight line to segment of polygon. \~
  \details \ru Вычислить квадрат расстояния от прямой линии до сегмента полигона, заданного начальной и конечной точками. \n
    Возвращает значение параметра ближайшей точки на линии tRes, вектор между ближайшими точками 
    и квадрат расстояния от этой точки до сегмента. \n
           \en Calculate squared distance from straight line to segment of polygon defined by start and end points. \n
    Returns the value of parameter of nearest point on the line, the vector between the nearest points 
    and the squared distance from this point to the segment. \n \~
  \param[in] seg_base - \ru Начальная точка сегмента.
                        \en Start point of the segment. \~
  \param[in] seg_end - \ru Начальная точка сегмента.
                       \en Start point of the segment. \~
  \param[in] line - \ru Прямая линия, до которой вычисляется расстояние.
                    \en Straight line to calculate the distance to. \~
  \param[out] vRes - \ru Вектор от ближайшей точки на линии до ближайшей точки на сегменте.
                     \en Vector from the nearest point on the line to the nearest point on the segment. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки на линии.
                     \en Value of the nearest point parameter on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToSegmentDistanceSquared( const MbFloatPoint3D &  seg_base, 
                                               const MbFloatPoint3D &  seg_end,
                                               const MbFloatAxis3D &   line,
                                                     MbFloatVector3D & vRes,
                                                     float &           tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &    poly,
                                            const MbFloatAxis3D &  line,
                                                  float            delta,
                                                  MbFloatPoint3D & nearestPoint,
                                                  float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &    poly,
                                            const MbFloatAxis3D &  line,
                                                  float            delta,
                                            const MbPlacement3D &  cutPlace,
                                                  MbFloatPoint3D & nearestPoint,
                                                  float &          tRes );

//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes и квадрат расстояния 
    от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on tRes line and the squared distance 
    from this point to a segment of the polygon. \n \~
  \param[in] poly - \ru Тестируемый полигон.
                    \en Polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[in] cutPlaces - \ru Отсекающая плоскости.
                         \en Cutting planes. \~
  \param[out] nearestPoint - \ru Ближайшая к лучу точка.
                             \en The nearest point of the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToPolyDistanceSquared( const MbPolygon3D &                poly,
                                            const MbFloatAxis3D &              line,
                                                  float                        delta,
                                            const std::vector<MbPlacement3D> & cutPlaces,
                                                  MbFloatPoint3D &             nearestPoint,
                                                  float &                      tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить расстояние от линии до точки.
           \en Calculate the distance from a line to a point. \~
  \details \ru Для корректного вычисления расстояния от линии до точки вектор направления линии должен быть нормализован. \n
           \en For correct calculating of the distance from a line to a point the line direction vector must be normalized. \n \~
  \param[in] line - \ru Линия.
                    \en Line. \~
  \param[in] to - \ru Точка.
                  \en Point. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Расстояние от точки до линии.
          \en The distance from a point to a line. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Double, class Point, class Vector, class Axis>
Double LineToPointDistance( const Axis &   line,
                            const Point &  to, 
                                  Double & tRes )
{
  PRECONDITION( ::fabs(line.GetAxisZ().Length() - 1.0) < METRIC_ACCURACY );

  Vector vect( line.GetOrigin(), to );
  tRes = vect * line.GetAxisZ();
  return ( line.GetAxisZ() | vect ).Length();
}

//------------------------------------------------------------------------------
/** \brief \ru Вычислить расстояние от линии до точки.
           \en Calculate the distance from a line to a point. \~
  \details \ru Для корректного вычисления расстояния от линии до точки вектор направления линии должен быть нормализован. \n
           \en For correct calculating of the distance from a line to a point the line direction vector must be normalized. \n \~
  \param[in] line - \ru Линия.
                    \en Line. \~
  \param[in] to - \ru Точка.
                  \en Point. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Расстояние от точки до линии.
          \en The distance from a point to a line. \~
  \ingroup Algorithms_3D
*/
// ---
template <class Double, class Point, class Vector, class Axis, class Placement>
Double LineToPointDistance( const Axis &      line,
                            const Point &     to,
                            const Placement & cutPlace,
                                  Double &    tRes )
{
  PRECONDITION( ::fabs(line.GetAxisZ().Length() - 1.0) < METRIC_ACCURACY );
  PRECONDITION( cutPlace.IsNormal() );

  if ( cutPlace.PointRelative( to, METRIC_EPSILON ) != iloc_InItem ) { // Под плоскостью или на плоскости (Below plane or on plane)
    Vector vect( line.GetOrigin(), to );
    tRes = vect * line.GetAxisZ();
    return ( line.GetAxisZ() | vect ).Length();
  }

  tRes = FLT_MAX;
  return FLT_MAX;
}


#endif // __MESH_PRIMITIVE_H
