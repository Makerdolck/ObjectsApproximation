////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Поверхность на базе триангуляции.
         \en Surface based on triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_GRID_SURFACE_H
#define __SURF_GRID_SURFACE_H


#include <templ_p_array.h>
#include <surface.h>
#include <cur_polyline.h>
#include <mesh_triangle.h>
#include <mb_operation_result.h>
#include <vector>


class  MATH_CLASS  MbCurve3D;
class  MATH_CLASS  MbMesh;
class  MATH_CLASS  MbGrid;


//#define _QUADRANGLES_


//------------------------------------------------------------------------------
/** \brief \ru Поверхность на базе триангуляции.
           \en Surface based on triangulation. \~
  \details \ru Поверхность на базе триангуляции образована криволинейными треугольниками, 
    гладко стыкующимися между собой по общим сторонам.
    В общих вершинах стыкующиеся треугольники имеют общую нормаль.
    Сторону треугольников изменяются по кубическому закону. \n
           \en Surface based on triangulation is formed by curvilinear triangles 
    which are smoothly connected together through common edges.
    Connected triangles have common normal at common vertices.
    Edges of triangles are changed by cubic law. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbGridSurface : public MbSurface {
public:
  
  /// \ru Tреугольная пластина поверхности на сетке точек. \en Triangular plate. 
  // ---
  class MbTrivela {
  protected :
    size_t index1;  ///< \ru Номер первой вершины треугольника в массиве точек. \en The number of first vertex.
    size_t index2;  ///< \ru Номер второй вершины треугольника в массиве точек. \en The number of second vertex.
    size_t index3;  ///< \ru Номер третьей вершины треугольника в массиве точек. \en The number of third vertex.
  public :
    MbTrivela() : index1( 0 ), index2( 0 ), index3( 0 ) {}
    MbTrivela( size_t i1, size_t i2, size_t i3 ) : index1( i1 ), index2( i2 ), index3( i3 ) {}
    MbTrivela( const MbTrivela  & init ) : index1( init.index1 ), index2( init.index2 ), index3( init.index3 ) {}
    MbTrivela( const MbTriangle & init ) : index1( init.GetIndex(0) ), index2( init.GetIndex(1) ), index3( init.GetIndex(2) ) {}
  public :
    void        Init( size_t i1, size_t i2, size_t i3, bool orientation )
    {
      if ( orientation ) { index1 = i1; index2 = i2; index3 = i3; }
      else               { index1 = i1; index2 = i3; index3 = i2; }
    }
    void        GetTriangle( size_t & i1, size_t & i2, size_t & i3 ) const { i1 = index1; i2 = index2; i3 = index3; }
    MbTrivela & operator = ( const MbTrivela  & init ) { index1 = init.index1; index2 = init.index2; index3 = init.index3; return *this; }
    MbTrivela & operator = ( const MbTriangle & init ) { index1 = init.GetIndex(0); index2 = init.GetIndex(1); index3 = init.GetIndex(2); return *this; }
  }; // MbTrivela
#ifdef _QUADRANGLES_
  /// \ru Четырехугольная пластина поверхности на сетке точек. \en Quadranglar plate. 
  // ---
  class MbTetragon {
  protected :
    size_t index1;  ///< \ru Номер первой вершины четырехугольника в массиве точек. \en The number of first vertex.
    size_t index2;  ///< \ru Номер второй вершины четырехугольника в массиве точек. \en The number of second vertex.
    size_t index3;  ///< \ru Номер третьей вершины четырехугольника в массиве точек. \en The number of third vertex.
    size_t index4;  ///< \ru Номер четвертой вершины четырехугольника в массиве точек. \en The number of forth vertex.
  public :
    MbTetragon() : index1( 0 ), index2( 0 ), index3( 0 ), index4( 0 ) {}
    MbTetragon( size_t i1, size_t i2, size_t i3, size_t i4 ) : index1( i1 ), index2( i2 ), index3( i3 ), index4( i4 ) {}
    MbTetragon( const MbTetragon & init ) : index1( init.index1 ), index2( init.index2 ), index3( init.index3 ), index4( init.index4 ) {}
    MbTetragon( const MbQuadrangle & init ) : index1( init.GetIndex(0) ), index2( init.GetIndex(1) ), index3( init.GetIndex(2) ), index4( init.GetIndex(3) ) {}
  public :
    void        Init( size_t i1, size_t i2, size_t i3, size_t i4, bool orientation )
    {
      if ( orientation ) { index1 = i1; index2 = i2; index3 = i3; index4 = i4; }
      else               { index1 = i1; index2 = i4; index3 = i3; index4 = i2; }
    }
    void        GetQuadrangle( size_t & i1, size_t & i2, size_t & i3, size_t & i4 ) const { i1 = index1; i2 = index2; i3 = index3; i4 = index4; }
    MbTetragon & operator = ( const MbTetragon & init ) { index1 = init.index1; index2 = init.index2; index3 = init.index3; index4 = init.index4; return *this; }
    MbTetragon & operator = ( const MbQuadrangle & init ) { index1 = init.GetIndex(0); index2 = init.GetIndex(1); index3 = init.GetIndex(2); index4 = init.GetIndex(3); return *this; }
  }; // MbTetragon
#endif // _QUADRANGLES_
  /// \ru Окружение вершины треугольной пластины. \en Environment of triangular plate's vertex. 
  // ---
  class MbContent {
  protected :
    /// \ru Номера входящих  точек на параметрической области поверхности. \en Number of incoming vertices.
    std::vector<size_t> prev;
    /// \ru Номера выходящих точек на параметрической области поверхности. \en Number of outcoming vertices.
    std::vector<size_t> next;
  public :
    MbContent() : prev(), next() {}
    MbContent( const MbContent & init ) 
      : prev( init.prev ), next( init.next ) {}
    ~MbContent() {}
  public :

    void    AddPrev( size_t ind ) { prev.push_back( ind ); }
    void    AddNext( size_t ind ) { next.push_back( ind ); }
    size_t  GetPrevSize() const { return prev.size(); }
    size_t  GetNextSize() const { return next.size(); }
    size_t  GetPrev( size_t i ) const { return prev[i]; }
    size_t  GetNext( size_t i ) const { return next[i]; }
    bool    Disappearance( size_t & indPrev, size_t & indNext ); 

    MbContent & operator = ( const MbContent & init ) { 
      prev = init.prev; 
      next = init.next; 
      return *this; 
    }
  };

private:
  // \ru Согласованные между собой множества данных в вершинах. \en Sets of data at vertices matched each other.  
  std::vector<MbCartPoint>      params;      ///< \ru Множество точек на параметрической области поверхности. \en Set of points in parametric space of surface. 
  std::vector<MbCartPoint3D>    points;      ///< \ru Множество точек поверхности. \en Set of points of surface. 
  std::vector<MbVector3D>       normals;     ///< \ru Множество нормалей поверхности. \en Set of normals of surface. 
  std::vector<MbTrivela>        triangles;   ///< \ru Множество треугольников. \en Set of triangles. 
#ifdef _QUADRANGLES_
  std::vector<MbTetragon>       quadrangles; ///< \ru Множество четырехугольников. \en Set of quadrangles. 
#endif // _QUADRANGLES_
  // \ru Описание области параметров поверхности. \en Description of surface parameters region.  
  size_t                        uCount;      ///< \ru Количество разбиений области по u. \en Count of splittings of region by u. 
  size_t                        vCount;      ///< \ru Количество разбиений области по v. \en Count of splittings of region by v. 
  PArray< std::vector<size_t> > cell;        ///< \ru Сетка области параметров поверхности. \en Grid of surface parameters region. 
  std::vector<MbContent>        contents;    ///< \ru Множество номеров соседних точек. \en Set of numbers of neighboring points. 
  std::vector<MbPolyline *>     boundary;    ///< \ru Граничные кривые области параметров поверхности. \en Boundary curves of surface parameters region. 
  double                        umin;        ///< \ru Минимальное  значение параметра u. \en Minimal value of parameter u. 
  double                        vmin;        ///< \ru Минимальное  значение параметра v. \en Minimal value of parameter v. 
  double                        umax;        ///< \ru Максимальное значение параметра u. \en Maximal value of parameter u. 
  double                        vmax;        ///< \ru Максимальное значение параметра v. \en Maximal value of parameter v. 
  bool                          uclosed;     ///< \ru Признак замкнутости по параметру u. \en Attribute of closedness by parameter u. 
  bool                          vclosed;     ///< \ru Признак замкнутости по параметру v. \en Attribute of closedness by parameter v. 

private:
  /// \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbGridSurface( const MbGridSurface & init );
  MbGridSurface( const MbGridSurface & init, MbRegDuplicate * iReg );
protected:
  /// \ru Конструктор поверхности. \en Constructor of surface. 
  template <class Params, class Points, class Normals, class Triangles
#ifdef _QUADRANGLES_
    , class Quadrangles
#endif // _QUADRANGLES_
    , class Bounds>
  MbGridSurface ( const Params      & _params 
                , const Points      & _points
                , const Normals     & _normals 
                , const Triangles   & _triangles
#ifdef _QUADRANGLES_
                , const Quadrangles & _quadrangles 
#endif // _QUADRANGLES_
                , const Bounds      & _bounds)
    : MbSurface  ()
    , params     ()
    , points     ()
    , normals    ()
    , triangles  ()
#ifdef _QUADRANGLES_
    , quadrangles()
#endif // _QUADRANGLES_
    , uCount( 0 )
    , vCount( 0 )
    , cell( 0, 1, true)
    , contents ()
    , boundary ()
    , umin     ( 0.0 )
    , vmin     ( 0.0 )
    , umax     ( 0.0 )
    , vmax     ( 0.0 )
    , uclosed  ( false )
    , vclosed  ( false )
  {
    size_t k, cnt;

    cnt = _params.size();
    params.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      params.push_back( _params[k] );
    cnt = _points.size();
    points.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      points.push_back( _points[k] );

    if ( _normals.size() > 0 ) {
      size_t normalsLast = _normals.size() - 1;
      normals.reserve( cnt );
      for ( k = 0; k < cnt; k++ ) {
        size_t ind = std_min( k, normalsLast );
        normals.push_back( _normals[ind] );
      }
    }
    cnt = _triangles.size();
    triangles.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      triangles.push_back( _triangles[k] );

#ifdef _QUADRANGLES_
    cnt = _quadrangles.size();
    quadrangles.reserve( cnt );
    for ( k = 0; k < cnt; k++ )
      quadrangles.push_back( _quadrangles[k] );
#endif // _QUADRANGLES_

    cnt = _bounds.size();
    boundary.reserve(cnt);
    for (k = 0; k < cnt; k++)
    {
      _bounds[k]->AddRef();
      boundary.push_back(_bounds[k]);
    }

    Init( boundary.size() == 0 );
  }

public:
  virtual ~MbGridSurface();

public:
  VISITING_CLASS( MbGridSurface );

  /** \name Общие функции геометрического объекта 
      \{ */
  // \ru Общие функции геометрического объекта \en Common functions of a geometric object  
  virtual MbeSpaceType  IsA() const; // \ru Тип элемента \en A type of element 
  virtual MbSpaceItem & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Cделать копию элемента \en Make a copy of element 
  virtual bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const;
  virtual bool    SetEqual( const MbSpaceItem &init );        // \ru Сделать равным \en Make equal 
  virtual bool    IsSimilar( const MbSpaceItem &init ) const;         // \ru Являются ли объекты подобными \en Whether the objects are similar 
  virtual void    Transform( const MbMatrix3D &matr, MbRegTransform * = NULL ); // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void    Move( const MbVector3D &to, MbRegTransform * = NULL ); // \ru Сдвиг \en Translation 
  virtual void    Rotate( const MbAxis3D &axis, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate around an axis 

  virtual void    GetProperties( MbProperties &properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void    SetProperties( const MbProperties &properties ); // \ru Записать свойства объекта \en Set properties of the object 
  virtual void    GetBasisPoints( MbControlData3D & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData3D & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  virtual void    Refresh(); // \ru Сбросить все временные данные. \en Reset all temporary data. 
  /** \} */

  /** \name Функции описания области определения поверхности
      \{ */
  // \ru Функции описания области определения поверхности. \en Functions for surface domain description. 
  virtual double  GetUMin() const;
  virtual double  GetVMin() const;
  virtual double  GetUMax() const;
  virtual double  GetVMax() const;
  virtual bool    IsUClosed() const; // \ru Замкнута ли поверхность по параметру u. \en Whether the surface is closed by parameter u. 
  virtual bool    IsVClosed() const; // \ru Замкнута ли поверхность по параметру v. \en Whether the surface is closed by parameter v. 
  /** \} */
  
  /** \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \{ */
  // \ru Функции для работы в области определения поверхности. \en Functions for working at surface domain. 
  virtual void    PointOn  ( double & u, double & v, MbCartPoint3D & p ) const; // \ru Точка на поверхности \en Point on the surface 
  virtual void    DeriveU  ( double & u, double & v, MbVector3D & der ) const; // \ru Первая производная по u \en First derivative with respect to u 
  virtual void    DeriveV  ( double & u, double & v, MbVector3D & der ) const; // \ru Первая производная по v \en First derivative with respect to v 
  virtual void    DeriveUU ( double & u, double & v, MbVector3D & der ) const; // \ru Вторая производная по u \en Second derivative with respect to u 
  virtual void    DeriveVV ( double & u, double & v, MbVector3D & der ) const; // \ru Вторая производная по v \en Second derivative with respect to v 
  virtual void    DeriveUV ( double & u, double & v, MbVector3D & der ) const; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  virtual void    DeriveUUU( double & u, double & v, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveUUV( double & u, double & v, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveUVV( double & u, double & v, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveVVV( double & u, double & v, MbVector3D & der ) const; // \ru Третья производная \en Third derivative 
  /** \} */
  
  /** \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
            За пределами параметрической области поверхность продолжается по касательной.
      \{ */
  // \ru Функции для работы внутри и вне области определения поверхности. \en Functions for working inside and outside the surface's domain. 
  virtual void   _PointOn  ( double  u, double  v, MbCartPoint3D & ) const; // \ru Точка на расширенной поверхности. \en The point on the extended surface. 
  virtual void   _DeriveU  ( double  u, double  v, MbVector3D & ) const; // \ru Первая производная по u. \en The first derivative with respect to u. 
  virtual void   _DeriveV  ( double  u, double  v, MbVector3D & ) const; // \ru Первая производная по v. \en The first derivative with respect to v. 
  virtual void   _DeriveUU ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная по u. \en The second derivative with respect to u. 
  virtual void   _DeriveVV ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная по v. \en The second derivative with respect to v. 
  virtual void   _DeriveUV ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная. \en The second derivative. 
  virtual void   _DeriveUUU( double  u, double  v, MbVector3D & ) const; // \ru Третья производная. \en The third derivative. 
  virtual void   _DeriveUUV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная. \en The third derivative. 
  virtual void   _DeriveUVV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная. \en The third derivative. 
  virtual void   _DeriveVVV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная. \en The third derivative. 
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  virtual void    Explore( double & u, double & v, bool ext,
                           MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                           MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const;
  /** \} */
  // \ru Функции движения по поверхности \en Functions of moving along the surface 
  virtual double  StepU( double u, double v, double sag ) const; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны по U \en Calculation of the approximation step with consideration of the curvature radius by U 
  virtual double  StepV( double u, double v, double sag ) const; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны по V \en Calculation of the approximation step with consideration of the curvature radius by V 
  virtual double  DeviationStepU( double u, double v, double ang ) const; // \ru Вычисление шага параметра u по углу отклонения нормали \en Calculation of parameter u step by the angle of deviation of normal 
  virtual double  DeviationStepV( double u, double v, double ang ) const; // \ru Вычисление шага параметра v по углу отклонения нормали \en Calculation of parameter v step by the angle of deviation of normal 
  virtual size_t  GetUCount() const;
  virtual size_t  GetVCount() const;
  virtual size_t  GetUPairs( double v, SArray<double> & u ) const; // \ru Вычислить U-пары от V. \en Calculate U-pairs by V. 
  virtual size_t  GetVPairs( double u, SArray<double> & v ) const; // \ru Вычислить V-пары от U. \en Calculate V-pairs by U. 
  virtual void    CalculateWire( const MbStepData & stepData, MbMesh & mesh ) const; // \ru Рассчитать сетку. \en Calculate mesh. 
  virtual void    CalculateSurfaceWire( const MbStepData & stepData, size_t beg,  MbMesh & mesh, 
                                        size_t uMeshCount = WIRE_MAX, size_t vMeshCount = WIRE_MAX ) const;   // \ru Рассчитать сетку. \en Calculate mesh. 
  // \ru Аппроксимация поверхности треугольными пластинами. \en Approximation of a surface by triangular plates. 
  virtual void    CalculateSurfaceGrid( const MbStepData & stepData, bool sense, MbGrid & grid ) const;
  // \ru Пересчитать нормали в вершинах. \en Normals Calculation on vertex. 
  virtual void    Normalize();

          size_t  GetBoundariesCount() const { return boundary.size(); } // \ru Выдать количество граничных двумерных кривых. \en Get the two-dimensional boundary curves count. 
  virtual MbContour & MakeContour( bool sense ) const; // \ru Выдать граничных двумерный контур. \en Get the two-dimensional boundary contour. 
  virtual MbCurve & MakeSegment( size_t i, bool sense ) const; // \ru Дать граничную двумерную кривую. \en Get the two-dimensional boundary curve. 

          /// \ru Инициализация объекта по другому такому же. \en Initialization of a object by same other object.
          void    Init( const MbGridSurface & init );

          /// \ru Конструктор поверхности. \en Constructor of surface. 
  template <class Params, class Points, class Normals, class Triangles 
#ifdef _QUADRANGLES_
           , class Quadrangles
#endif // _QUADRANGLES_
           , class Bounds>

  static  MbGridSurface * Create( const Params    & _params 
                                , const Points    & _points
                                , const Normals   & _normals 
                                , const Triangles & _triangles
#ifdef _QUADRANGLES_
                                , const Quadrangles & _quadrangles
#endif // _QUADRANGLES_
                                , const Bounds    & _bounds )
  {
    MbGridSurface * surface = NULL;

    const size_t itemsCnt = _params.size();

    if ( (itemsCnt > 2) && (itemsCnt == _points.size()) && (_triangles.size() > 0 
#ifdef _QUADRANGLES_
      || _quadrangles.size() > 0
#endif // _QUADRANGLES_
      ) ) 
    {
      if ( (itemsCnt == _normals.size()) || (_normals.size() == 1) )
        surface = new MbGridSurface( _params, _points, _normals, _triangles
#ifdef _QUADRANGLES_
                                   , _quadrangles 
#endif // _QUADRANGLES_
                                   , _bounds );
    }
    return surface;
  }

private:
          /// \ru Инициализация. \en Initialization.
          void    Init( bool bound = true );
          // \ru Инициализация граничных кривых. \en Initialization of boundary curves. 
          void    MakeBoundary(); 
          // \ru Инициализация множества номеров соседних точек. \en Initialization of set of numbers of neighboring points. 
          void    MakeContents();
          // \ru Поиск ближайшего треугольника для инициализации данных ячейки. \en Search nearest triangle for initialization of data of cell. 
          void    FindNearest( size_t i, size_t j, std::vector<size_t> & indecies, 
                               double uDelta, double vDelta, double u, double v );
          // \ru Добавить ближайший треугольник в ячейку. \en Add nearest triangle to cell. 
          bool    AddNearest( size_t i, size_t j, size_t ind );
          // \ru Расстояние до треугольника. \en The distance to a triangle. 
          double  DistanceToTriangle( size_t ind, const double & u, const double & v, 
                                      double eps, MbCartPoint & p ) const;
          // \ru Проверка параметров. \en Check parameters. 
          void    CheckParam( double & u, double & v ) const;
          // \ru Найти индекс треугольника \en Find index of triangle 
          size_t  FindIndex( double & u, double & v ) const; 
          // \ru Вычислить временные данные найденного треугольника. \en Calculate temporary data of found triangle. 
          size_t  PrepareData( double & u, double & v,
                               MbCartPoint3D & vertex1, MbCartPoint3D & vertex2, MbCartPoint3D & vertex3, // \ru Вершины треугольнака. \en Vertices of triangle.
                               MbVector3D & normal1,    MbVector3D & normal2,    MbVector3D & normal3, // \ru Нормали в вершинах треугольнака.  \en Normals on vertices of triangle.
                               MbVector3D & vector1_2, MbVector3D & vector2_1,
                               MbVector3D & vector2_3, MbVector3D & vector3_2, 
                               MbVector3D & vector3_1, MbVector3D & vector1_3, 
                               double & aCalc, double & bCalc, double & cCalc,
                               double & da_du, double & da_dv,
                               double & db_du, double & db_dv,
                               double & dc_du, double & dc_dv ) const; 
          // \ru Вычислить точки. \en Calculate points. 
          void    CalculatePoint( const MbCartPoint3D & vertex1,
                                  const MbCartPoint3D & vertex2,
                                  const MbCartPoint3D & vertex3,
                                  const MbVector3D & normal1,
                                  const MbVector3D & normal2,
                                  const MbVector3D & normal3,
                                  const MbVector3D & vector1_2,
                                  const MbVector3D & vector2_1,
                                  const MbVector3D & vector2_3,
                                  const MbVector3D & vector3_2,
                                  const MbVector3D & vector3_1,
                                  const MbVector3D & vector1_3,
                                  const double & aCalc,
                                  const double & bCalc,
                                  const double & cCalc,
                                  bool smartCaolculation,
                                  MbCartPoint3D & p1, MbCartPoint3D & p2, MbCartPoint3D & p3 ) const;
          // \ru Вычислить производные. \en Calculate derivatives. 
          void    CalculateFirst( const MbCartPoint3D & vertex1,
                                  const MbCartPoint3D & vertex2,
                                  const MbCartPoint3D & vertex3,
                                  const MbVector3D & vector1_2,
                                  const MbVector3D & vector2_1,
                                  const MbVector3D & vector2_3,
                                  const MbVector3D & vector3_2,
                                  const MbVector3D & vector3_1,
                                  const MbVector3D & vector1_3,
                                  const double & aCalc,
                                  const double & bCalc,
                                  const double & cCalc,
                                  const MbCartPoint3D & p1, const MbCartPoint3D & p2, const MbCartPoint3D & p3,
                                  MbVector3D & aDer,  MbVector3D & bDer,  MbVector3D & cDer ) const;
          // \ru Вычислить производные. \en Calculate derivatives. 
          void    CalculateSecond( const MbCartPoint3D & vertex1,
                                   const MbCartPoint3D & vertex2,
                                   const MbCartPoint3D & vertex3,
                                   const MbVector3D & vector1_2,
                                   const MbVector3D & vector2_1,
                                   const MbVector3D & vector2_3,
                                   const MbVector3D & vector3_2,
                                   const MbVector3D & vector3_1,
                                   const MbVector3D & vector1_3,
                                   const double & aCalc,
                                   const double & bCalc,
                                   const double & cCalc,
                                   MbVector3D & aaDer, MbVector3D & bbDer, MbVector3D & ccDer,
                                   MbVector3D & abDer, MbVector3D & bcDer, MbVector3D & caDer ) const;
          // \ru Вычислить производные. \en Calculate derivatives. 
          void    CalculateThird( const MbCartPoint3D & vertex1,
                                  const MbCartPoint3D & vertex2,
                                  const MbCartPoint3D & vertex3,
                                  const MbVector3D & vector1_2,
                                  const MbVector3D & vector2_1,
                                  const MbVector3D & vector2_3,
                                  const MbVector3D & vector3_2,
                                  const MbVector3D & vector3_1,
                                  const MbVector3D & vector1_3,
                                  const double & aCalc,
                                  const double & bCalc,
                                  const double & cCalc,
                                  MbVector3D & aaaDer, MbVector3D & bbbDer, MbVector3D & cccDer,
                                  MbVector3D & aabDer, MbVector3D & aacDer, 
                                  MbVector3D & bbcDer, MbVector3D & bbaDer, 
                                  MbVector3D & ccaDer, MbVector3D & ccbDer ) const;
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void    operator = ( const MbGridSurface & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbGridSurface )
}; // MbGridSurface


#endif // __SURF_GRID_SURFACE_H
