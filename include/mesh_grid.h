////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Tриангуляция.
         \en Triangulation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MESH_GRID_H
#define __MESH_GRID_H


#include <mesh_primitive.h>
#include <templ_s_array.h>
#include <mesh_triangle.h>
#include <mb_data.h>


class  MATH_CLASS  MbRect;


//------------------------------------------------------------------------------
/** \brief \ru Триангуляция.
           \en Triangulation. \~
  \details \ru Триангуляция представляет собой набор треугольных и четырёхугольных пластин, 
    стыкующихся друг с другом по общим сторонам.\n
      В простейшем случае триангуляция состоит из набора точек и набора треугольников.
    Треугольник - это три номера из набора точек, определяющих вершины треугольника.
    Триангуляция используется для аппроксимации криволинейных поверхностей.
    В определенных случаях использование плоских пластин значительно упрощает работу с криволинейными поверхностями. 
    Триангуляция описывает геометрическую форму объектов с определённой степенью точности.\n
      Триангуляция используется для получения точечных изображений, вычисления 
    масс-инерционных характеристик, проверки столкновений объектов, проведения численных 
    экспериментов над моделями. Для одного и того же объекта триангуляция может 
    иметь разное наполнение данных, в зависимости от назначения.\n
           \en Triangulation represents a set of triangular and quadrangular plates 
    which are joined to each other by their common sides.\n
      In simple case triangulation consists of a set of points and a set of triangles.
    Triangle is represented as three numbers from the set of points defining vertices of triangle.
    Triangulation is used for approximation of curved surfaces.
    In certain cases use of flat plates significantly simplifies work 
    with curved surfaces. Triangulation describes geometric form of objects 
    with the specified accuracy.\n
      Triangulation is used for obtaining point-images, for calculating 
    mass-inertial properties, for collision detection, for numerical 
    experiments with models. For the same object triangulation can 
    have different data depending on its purpose.\n \~
      \par \ru Назначения триангуляции
      От назначения триангуляции зависит наполнение данных:\n
      Если триангуляция предназначена для визуализации геометрической формы, 
    то заполняются множества points и normals, 
    шаг движения вдоль кривых и поверхностей вычисляется по стрелке прогиба, работают функции Step, StepU, StepV.\n
      Если триангуляция предназначена для аппроксимации геометрической формы с привязкой к параметрам поверхности, 
    то заполняются множества params, points и normals,
    шаг движения вдоль кривых и поверхностей вычисляется по стрелке прогиба.\n
      Если триангуляция предназначена для вычисления инерционных характеристик, 
    то заполняется множество params, шаг движения вдоль кривых и поверхностей вычисляется по углу отклонения нормали, 
    работают функции DeviationStep, DeviationStepU, DeviationStepV.\n
      Если триангуляция предназначена для определения столкновений элементов модели,
    то заполняются множества params и points, шаг движения вдоль кривых и поверхностей 
    вычисляется по стрелке прогиба, работают функции Step, StepU, StepV.\n
      Если триангуляция предназначена для разбивки на элементы,
    то заполняются множества points и normals, шаг движения вдоль кривых и поверхностей 
    вычисляется с ограничением длины сторон треугольников, может быть добавлено ограничение по стрелке прогиба и углу отклонения нормали.
    (работают функции MetricStep, MetricStepU, MetricStepV).\n
           \en Purposes of triangulation
      Data depends on purposes of triangulation:\n
      If triangulation is intended for visualization of a geometric form, 
    then 'points' and 'normals' sets are filled (in case of planes 'normals' contains one 
    element), spacing of sample along the curves and the surfaces is calculated by stepData
    (Step, StepU, StepV functions work).\n
      If triangulation is intended for visualization of geometric form and 
    texture, then 'params', 'points' and 'normals' sets are filled, spacing of sample along 
    curves and surfaces is calculated by stepData.\n
      If triangulation is intended for calculation of mass-inertial properties, 
    then 'params' set is filled, spacing of sample along curves and surfaces 
    is calculated by angle of deviation (DeviationStep, DeviationStepU, 
    DeviationStepV functions work).\n
      If triangulation is intended for collision detection between elements of model,
    then 'params' and 'points' sets are filled, spacing of sample along curves and surfaces 
    is calculated by stepData (Step, StepU, StepV functions work).\n
      If triangulation is intended for splitting into elements,
    then 'normals' and 'points' sets are filled, spacing of sampling along curves and surfaces 
    is calculated by angle of deviation with limiting of length 
    (MetricStep, MetricStepU, MetricStepV functions work).\n \~
  \ingroup Polygonal_Objects
*/
// ---
class MATH_CLASS MbGrid : public MbPrimitive {
private:
  SArray<MbFloatPoint>      params;      ///< \ru Множество параметров - двумерных точек на параметрической области триангулируемой поверхности (может быть пустым). \en Set of parameters of two-dimensional points in parametric domain of surface being triangulated(can be empty). 
  SArray<MbFloatPoint3D>    points;      ///< \ru Множество контрольных точек триангуляции (согласовано с множеством параметров, если последнее не пустое, или пусто, если не пусто множество параметров). \en Set of control points of triangulation (synchronized with set of parameters if the last is not empty, or empty if the set of parameters isn't empty). 
  SArray<MbFloatVector3D>   normals;     ///< \ru Множество нормалей в контрольных точках согласовано с множеством контрольных точек. \en Set of normals at control points is synchronized with the set of control points. 
  SArray<MbTriangle>        triangles;   ///< \ru Индексное множество треугольных пластин содержит номера элементов множества params и/или множеств points и normals. \en Set of triangular plates contains numbers of elements of 'params' set and/or of 'points' and 'normals' sets. 
  SArray<MbQuadrangle>      quadrangles; ///< \ru Индексное множество четырёхугольных пластин содержит номера элементов множества params и/или множеств points и normals. \en Set of quadrangular plates contains numbers of elements of 'params' set and/or of 'points' and 'normals' sets. 
  PArray<MbGridPolygon>     polygons;    ///< \ru Индексное множество полигонов содержит номера элементов множества points и/или множества params. объект владеет элементами множества. \en Set of polygons contains numbers of elements of 'points' set and/or of 'params' set. Ownership of elements of set.
  MbStepData                stepData;    ///< \ru Параметр расчета триангуляции. \en Parameter of triangulation calculation. 
  /** \brief \ru Габаритный куб объекта. 
             \en Bounding box of object. \~
      \details \ru Габаритный куб объекта рассчитывается только при запросе габарита объекта. Габаритный куб в конструкторе объекта и после модификации объекта принимает неопределенное значение.
               \en Bounding box of object is calculated only at the request. Bounding box of object is undefined after object constructor and after object modifications \n \~
  */
  mutable MbCube            cube;

protected:
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en The copy constructor without implementation prevents from copying by default. 
  MbGrid( const MbGrid & init );
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbGrid( const MbGrid & init, MbRegDuplicate * iReg ); 
public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbGrid();
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbGrid();

public:

  /** \ru \name Общие функции примитива.
      \en \name Common functions of primitive.
      \{ */
  virtual MbePrimitiveType Type() const; // \ru Тип объекта. \en A type of an object. 
  virtual MbGrid & Duplicate( MbRegDuplicate * iReg = NULL ) const; // \ru Создать копию объекта. \en Create a copy of the object. 
  virtual void    Transform( const MbMatrix3D & matr );             // \ru Преобразовать сетку согласно матрице. \en Transform mesh according to the matrix. 
  virtual void    Move     ( const MbVector3D & to );               // \ru Сдвиг сетки. \en Move mesh. 
  virtual void    Rotate   ( const MbAxis3D & axis, double angle ); // \ru Поворот сетки вокруг оси. \en Rotation of mesh about an axis. 
  virtual void    AddYourGabaritTo( MbCube & r ) const;             // \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object.  
  virtual double  DistanceToPoint( const MbCartPoint3D & pnt ) const; // \ru Вычислить расстояние до точки. \en Calculate distance to point.  
  virtual double  DistanceToLine( const MbAxis3D & axis, double maxDistance, double & t ) const;  // \ru Вычислить расстояние до оси. \en Calculate the distance to an axis.  
  virtual void    GetProperties( MbProperties & );        // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties & );  // \ru Записать свойства объекта. \en Set properties of the object. 

  /** \} */
  /** \ru \name Функции триангуляции.
      \en \name Functions of triangulation.
      \{ */  
          /// \ru Проверить наличие данных. \en Check data availability. 
          bool    IsComplete() const { return (triangles.Count() > 0) || (quadrangles.Count() > 0) || (polygons.Count() > 0); } 
          /// \ru Выдать количество параметров. \en Get the number of parameters. 
          size_t  ParamsCount()  const { return params.Count();  } 
          /// \ru Выдать количество точек. \en Get the number of points. 
          size_t  PointsCount()  const { return points.Count();  }
          /// \ru Выдать количество нормалей. \en Get the number of normals. 
          size_t  NormalsCount() const { return normals.Count(); }
          /// \ru Выдать количество параметров минус 1 (максимальный индекс). \en Get the number of parameters minus one (maximal index). 
          ptrdiff_t ParamsMaxIndex()  const { return params.MaxIndex();  }
          /// \ru Выдать количество точек минус 1 (максимальный индекс). \en Get the number of points minus one (maximal index). 
          ptrdiff_t PointsMaxIndex()  const { return points.MaxIndex();  }
          /// \ru Выдать количество нормалей минус 1 (максимальный индекс). \en Get the number of normals minus one (maximal index). 
          ptrdiff_t NormalsMaxIndex() const { return normals.MaxIndex(); }

          /// \ru Выдать общее количество всех (связанных и несвязанных) треугольников. \en Get total number of all triangles (adjacent and non-adjacent). 
          size_t  AllTrianglesCount() const;
          /// \ru Выдать общее количество всех (связанных и несвязанных) четырёхугольников. \en Get total number of all quadrangles (adjacent and non-adjacent). 
          size_t  AllQuadranglesCount() const;
          /// \ru Выдать количество треугольников. \en Get the number of triangles. 
          size_t  TrianglesCount() const { return triangles.Count(); }
          /// \ru Выдать количество четырёхугольников. \en Get the number of quadrangles. 
          size_t  QuadranglesCount() const { return quadrangles.Count(); }
          /// \ru Выдать количество полигонов. \en Get the number of polygons. 
          size_t  PolygonsCount() const { return polygons.Count(); }

          /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
          void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D, const MbVector3D & n3D ) { params.Add( MbFloatPoint(p2D) ); points.Add( MbFloatPoint3D(p3D) ); normals.Add( MbFloatVector3D(n3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
          void    AddPoint ( const MbCartPoint   & p2D, const MbFloatPoint3D & p3D, const MbVector3D & n3D ) { params.Add( MbFloatPoint(p2D) ); points.Add(p3D); normals.Add( MbFloatVector3D(n3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, point and normal of triangulated surface at point to triangulation. 
          void    AddPoint ( const MbCartPoint   & p2D, const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) { params.Add( MbFloatPoint(p2D) ); points.Add(p3D); normals.Add(n3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
          void    AddPoint ( const MbCartPoint   & p2D, const MbCartPoint3D & p3D ) { params.Add( MbFloatPoint(p2D) ); points.Add( MbFloatPoint3D(p3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
          void    AddPoint ( const MbCartPoint   & p2D, const MbFloatPoint3D & p3D ) { params.Add( MbFloatPoint(p2D) ); points.Add(p3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
          void    AddPoint ( const MbCartPoint3D & p3D, const MbVector3D & n3D ) { points.Add( MbFloatPoint3D(p3D) ); normals.Add( MbFloatVector3D(n3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
          void    AddPoint ( const MbFloatPoint3D & p3D, const MbVector3D & n3D ) { points.Add(p3D); normals.Add( MbFloatVector3D(n3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
          void    AddPoint ( const MbCartPoint3D & p3D ) { points.Add( MbFloatPoint3D(p3D) ); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
          void    AddParam ( const MbCartPoint   & p2D ) { params.Add( MbFloatPoint(p2D) ); }
          /// \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
          void    AddNormal( const MbVector3D    & n3D ) { normals.Add( MbFloatVector3D( n3D ) ); }

          /// \ru Добавить в триангуляцию параметры, точку и нормаль триангулируемой поверхности в точке. \en Add parameters, a point and a normal of triangulated surface at the point to triangulation. 
          void    AddPoint( const MbFloatPoint   & p2D, const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) { params.Add(p2D); points.Add(p3D); normals.Add(n3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры и точку. \en Add parameters and a point to triangulation. 
          void    AddPoint( const MbFloatPoint   & p2D, const MbFloatPoint3D & p3D ) { params.Add(p2D); points.Add(p3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию точку и нормаль в точке. \en Add a point and normal at the point to triangulation. 
          void    AddPoint( const MbFloatPoint3D & p3D, const MbFloatVector3D & n3D ) { points.Add(p3D); normals.Add(n3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию точку. \en Add a point to triangulation. 
          void    AddPoint( const MbFloatPoint3D & p3D ) { points.Add(p3D); cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation. 
          void    AddParam( const MbFloatPoint   & p2D ) { params.Add(p2D); }
          /// \ru Добавить в триангуляцию нормаль. \en Add a normal to triangulation. 
          void    AddNormal( const MbFloatVector3D & n3D ) { normals.Add(n3D); }

          /// \ru Добавить в триангуляцию точки. \en Add points to triangulation. 
          template <class PointsVector>
          void    AddPoints ( const PointsVector & pnts )  { points  += pnts; cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию нормали. \en Add normals to triangulation. 
          template <class NormalsVector>
          void    AddNormals( const NormalsVector & nrms ) { normals += nrms; cube.SetEmpty(); }
          /// \ru Добавить в триангуляцию нормали, заданные в виде точек. \en Add normals given as points to triangulation. 
          void    AddNormals( const SArray<MbFloatPoint3D> & nrms );
          /// \ru Добавить в триангуляцию параметры триангулируемой поверхности. \en Add parameters of triangulated surface to triangulation.
          template <class ParamsVector>
          void    AddParams( const ParamsVector & prms ) { params += prms; }

  
          /// \ru Выдать точку по её номеру. \en Get point by its index. 
          void    GetPoint ( size_t i, MbCartPoint3D  & p ) const { points[i].GetCartPoint(p); }
          /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
          void    GetNormal( size_t i, MbVector3D     & n ) const { (normals.Count() == 1) ? normals[0].GetVector(n) : normals[i].GetVector(n); }
          /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
          void    GetParam ( size_t i, MbCartPoint    & p ) const { params[i].GetCartPoint(p); }

          /// \ru Выдать точку по её номеру. \en Get point by its index. 
          void    GetPoint ( size_t i, MbFloatPoint3D  & p ) const { p = points[i]; }
          /// \ru Выдать нормаль по её номеру. \en Get normal by its index. 
          void    GetNormal( size_t i, MbFloatVector3D & n) const { (normals.Count() == 1) ? n = normals[0] : n = normals[i]; }
          /// \ru Выдать параметр по его номеру. \en Get parameter by its index. 
          void    GetParam ( size_t i, MbFloatPoint    & p ) const { p = params[i]; }

          /// \ru Выдать точку с заданным номером. \en Get point by the given index. 
  const   MbFloatPoint3D  & GetPoint ( size_t i ) const { return points[i]; }
          /// \ru Выдать нормаль с заданным номером. \en Get normal by the given index. 
  const   MbFloatVector3D & GetNormal( size_t i ) const { return ( (normals.Count() == 1) ? normals[0] : normals[i] ); }
          /// \ru Выдать параметр с заданным номером. \en Get parameter by the given index. 
  const   MbFloatPoint    & GetParam ( size_t i ) const { return params[i]; }

          /// \ru Установить точку с заданным номером. \en Set point by the given index. 
          void    SetPoint ( size_t i, const MbCartPoint3D & p ) { points[i] = p; cube.SetEmpty(); }
          /// \ru Установить нормаль с заданным номером. \en Set normal by the given index. 
          void    SetNormal( size_t i, const MbVector3D    & n ) { normals[i].Init( MbFloatVector3D(n) ); }
          /// \ru Установить параметр с заданным номером. \en Set parameter by the given index. 
          void    SetParam ( size_t i, const MbCartPoint   & p ) { params[i] = p; }

          /// \ru Удалить параметры поверхности с заданным номером. \en Delete parameters of surface by the given index. 
          void    ParamRemove ( size_t i ) { if ( i < params.Count() )  params.RemoveInd( i );  }
          /// \ru Удалить точку с заданным номером. \en Delete point by the given index. 
          void    PointRemove ( size_t i ) { if ( i < points.Count() )  points.RemoveInd( i ); cube.SetEmpty(); }
          /// \ru Удалить точки. \en Delete points. 
          void    PointsRemove()           { points.HardFlush(); cube.SetEmpty(); }
          /// \ru Удалить нормаль с заданным номером. \en Delete normal by the given index. 
          void    NormalRemove( size_t i ) { if ( i < normals.Count() ) normals.RemoveInd( i ); }

          /// \ru Добавить треугольник. \en Add a triangle. 
          void    AddTriangle  ( const MbTriangle & triangle )   { triangles.Add( triangle ); }
          /// \ru Добавить треугольник с заданными номерами вершин. \en Add a triangle by the given indices of vertices 
          void    AddTriangle  ( uint j0, uint j1, uint j2, bool o ) { MbTriangle * t = triangles.Add(); t->Init(j0,j1,j2,o); }
          /// \ru Добавить треугольники. \en Add triangles. 
          void    AddTriangles ( const SArray<MbTriangle> & trngs ) { triangles += trngs; }
          /// \ru Добавить треугольники. \en Add triangles. 
          void    AddTriangles ( const std::vector<MbTriangle> & trngs )
          { 
            size_t addCnt = trngs.size();
            triangles.Reserve( addCnt );
            for ( size_t k = 0; k < addCnt; k++ )
              triangles.Add( trngs[k] );
          }

          /// \ru Добавить четырёхугольник. \en Add a quadrangle. 
          void    AddQuadrangle( const MbQuadrangle & quadrangle ) { quadrangles.Add( quadrangle ); }
          /// \ru Добавить четырёхугольник с заданными номерами вершин. \en Add a quadrangle by the given indices of vertices. 
          void    AddQuadrangle( uint j0, uint j1, uint j2, uint j3, bool o ) { MbQuadrangle * t = quadrangles.Add(); t->Init(j0,j1,j2,j3,o); }

          /// \ru Добавить полигон. \en Add a polygon. 
          void    AddPolygon( MbGridPolygon & poly ) { polygons.Add( &poly ); }
          /// \ru Собрать внешние полигоны. \en Collect outer polygons. 
          void    CollectEdges( std::vector<c3d::UintPair> & edges ) const;

          /// \ru Выдать индексы точек в массиве points для i-го треугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th triangle (adjacent or non-adjacent). 
          bool    GetAllTrianglePointIndex  ( size_t i, uint & ind0, uint & ind1, uint & ind2 ) const; 
          /// \ru Выдать индексы точек в массиве points для i-го четырехугольника (связанного или несвязанного). \en Get indices of points in 'points' array for i-th quadrangle (adjacent or non-adjacent). 
          bool    GetAllQuadranglePointIndex( size_t i, uint & ind0, uint & ind1, uint & ind2, uint & ind3 ) const; 

          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th triangle in general numbering (with strips). 
          bool    GetAllTriangleParams   ( size_t i, MbCartPoint &r0, MbCartPoint &r1, MbCartPoint &r2 ) const;
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
          bool    GetAllTrianglePoints   ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2 ) const;
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th triangle in general numbering (with strips). 
          bool    GetAllTrianglePoints   ( size_t i, MbFloatPoint3D &p0, MbFloatPoint3D &p1, MbFloatPoint3D &p2 ) const;
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
          bool    GetAllTriangleNormals  ( size_t i, MbFloatPoint3D &n0, MbFloatPoint3D &n1, MbFloatPoint3D &n2 ) const;
          /// \ru Выдать для треугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th triangle in general numbering (with strips). 
          bool    GetAllTriangleNormals  ( size_t i, MbFloatVector3D &n0, MbFloatVector3D &n1, MbFloatVector3D &n2 ) const;

          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) параметры поверхности. \en Get parameters of surface for i-th quadrangle in general numbering (with strips). 
          bool    GetAllQuadrangleParams ( size_t i, MbCartPoint &r0, MbCartPoint &r1, MbCartPoint &r2, MbCartPoint &r3 ) const;
          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetAllQuadranglePoints ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2, MbCartPoint3D &p3 ) const;
          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) точки вершин. \en Get points of vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetAllQuadranglePoints ( size_t i, MbFloatPoint3D &p0, MbFloatPoint3D &p1, MbFloatPoint3D &p2, MbFloatPoint3D &n3 ) const;
          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetAllQuadrangleNormals( size_t i, MbFloatPoint3D &n0, MbFloatPoint3D &n1, MbFloatPoint3D &n2, MbFloatPoint3D &n3 ) const;
          /// \ru Выдать для четырёхугольника с номером i в общей нумерации (с полосами) нормали в вершинах. \en Get normals at vertices for i-th quadrangle in general numbering (with strips). 
          bool    GetAllQuadrangleNormals( size_t i, MbFloatVector3D &n0, MbFloatVector3D &n1, MbFloatVector3D &n2, MbFloatVector3D &n3 ) const;

          /// \ru Выдать для треугольника с номером i параметры поверхности. \en Get parameters of surface for i-th triangle. 
          bool    GetTriangleParams   ( size_t i, MbCartPoint &r0, MbCartPoint &r1, MbCartPoint &r2 ) const;
          /// \ru Выдать для треугольника с номером i точки вершин. \en Get points of vertices for i-th triangle. 
          bool    GetTrianglePoints   ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2 ) const;
          /// \ru Выдать для треугольника с номером i точки вершин. \en Get points of vertices for i-th triangle. 
          bool    GetTrianglePoints   ( size_t i, MbFloatPoint3D &p0, MbFloatPoint3D &p1, MbFloatPoint3D &p2 ) const;
          /// \ru Выдать для треугольника с номером i нормали в вершинах. \en Get normals at vertices for i-th triangle. 
          bool    GetTriangleNormals  ( size_t i, MbFloatVector3D &n0, MbFloatVector3D &n1, MbFloatVector3D &n2 ) const;

          /// \ru Выдать для четырёхугольника с номером i параметры поверхности. \en Get parameters of surface for i-th quadrangle. 
          bool    GetQuadrangleParams ( size_t i, MbCartPoint &r0, MbCartPoint &r1, MbCartPoint &r2, MbCartPoint &r3 ) const;
          /// \ru Выдать для четырёхугольника с номером i точки вершин. \en Get points of vertices for i-th quadrangle. 
          bool    GetQuadranglePoints ( size_t i, MbCartPoint3D &p0, MbCartPoint3D &p1, MbCartPoint3D &p2, MbCartPoint3D &p3 ) const;
          /// \ru Выдать для четырёхугольника с номером i точки вершин. \en Get points of vertices for i-th quadrangle. 
          bool    GetQuadranglePoints ( size_t i, MbFloatPoint3D &p0, MbFloatPoint3D &p1, MbFloatPoint3D &p2, MbFloatPoint3D &p3 ) const;
          /// \ru Выдать для четырёхугольника с номером i нормали в вершинах. \en Get normals at vertices for i-th quadrangle. 
          bool    GetQuadrangleNormals( size_t i, MbFloatVector3D &n0, MbFloatVector3D &n1, MbFloatVector3D &n2, MbFloatVector3D &n3 ) const;

          /// \ru Выдать первую нормаль для плоской триангуляции, если количество точек больше количества нормалей (только для плоской триангуляции). \en Get first normal for flat triangulation if count of points is greater than count of normals (only for planar triangulation). 
          bool    GetSingleNormal   ( MbFloatVector3D & ) const;
          /// \ru Если количество точек больше количества нормалей, то добавить недостающие нормали (только для плоской триангуляции). \en If count of points is greater than count of normals, then add missing normals (only for planar triangulation). 
          void    SynchronizNormals ();

          /// \ru Выдать треугольник с номером i. \en Get i-th triangle. 
  const   MbTriangle   &  GetTriangle  ( size_t i ) const { return triangles[i];   }
          /// \ru Выдать четырёхугольник с номером i. \en Get i-th quadrangle. 
  const   MbQuadrangle &  GetQuadrangle( size_t i ) const { return quadrangles[i]; }
          /// \ru Выдать полигон с номером i. \en Get i-th polygon. 
  const   MbGridPolygon & GetPolygon   ( size_t i ) const { return *polygons[i];   }

          /// \ru Выдать для треугольника с номером i номера точек вершин. \en Get indices of vertex points for i-th triangle. 
          bool    GetTriangleIndex ( size_t i, uint & i0, uint & i1, uint & i2 ) const {
                    if ( i < triangles.Count() )
                      return triangles[i].GetTriangle( i0, i1, i2 );
                    return false;
                  }
          /// \ru Выдать для четырёхугольника с номером i номера точек вершин. \en Get indices of points of vertices for i-th quadrangle. 
          bool    GetQuadrangleIndex ( size_t i, uint & i0, uint & i1, uint & i2, uint & i3 ) const {
                    if ( i < quadrangles.Count() )
                      return quadrangles[i].GetQuadrangle( i0, i1, i2, i3 );
                    return false;
                  }
          /// \ru Выдать контейнер параметров. \en Get the container of parameters. 
  const   SArray<MbFloatPoint>    & GetParams()  const { return params;  }
          /// \ru Выдать контейнер точек. \en Get the container of points. 
  const   SArray<MbFloatPoint3D>  & GetPoints()  const { return points;  }
          /// \ru Выдать контейнер нормалей. \en Get the container of normals. 
  const   SArray<MbFloatVector3D> & GetNormals() const { return normals; }

          /// \ru Выдать контейнер треугольников. \en Get the container of triangles. 
  const   SArray<MbTriangle>   & GetTriangles()  const { return triangles; }
          /// \ru Выдать контейнер четырёхугольников. \en Get the container of quadrangles. 
  const   SArray<MbQuadrangle> & GetQuadrangles() const { return quadrangles; }

          /// \ru Удалить треугольник по его индексу. \en Delete triangle by its index. 
          void    RemoveTriangle  ( size_t k ) { if (k < triangles.Count()) triangles.RemoveInd( k ); }
          /// \ru Удалить четырёхугольник по его индексу. \en Delete quadrangle by its index. 
          void    RemoveQuadrangle( size_t k ) { if (k < quadrangles.Count()) quadrangles.RemoveInd( k ); }
          /// \ru Удалить полигон по его индексу. \en Delete polygone by its index. 
          void    RemovePolygon   ( size_t k ) { if (k < polygons.Count()) polygons.RemoveInd( k ); }

          /// \ru Преобразовать четырёхугольники в треугольники. \en Convert quadrangles to triangles. 
          void    ConvertQuadranglesToTriangles();
          /// \ru Преобразовать все объекты в треугольники и уравнять число точек и нормалей. \en Convert all objects to triangles and equalize count of points and count of normals. 
          void    ConvertAllToTriangles();
          /// \ru Удалить лишние точки. \en Remove redundant points. 
          bool    RemoveRedundantPoints( bool deleteNormals );

          /// \ru Удалить параметры поверхности. \en Delete parameters of surface. 
          void    ParamsDelete() { params.Flush(); } 

          /// \ru Удалить все треугольники. \en Delete all triangles. 
          void    TrianglesDelete()   { triangles.Flush();   }
          /// \ru Удалить все четырёхугольники. \en Delete all quadrangles. 
          void    QuadranglesDelete() { quadrangles.Flush(); }
          /// \ru Удалить все полигоны. \en Delete all polygons. 
          void    PolygonsDelete()    { polygons.Flush();    }

          /// \ru Расширить присланный габаритный прямоугольник так, чтобы он включал в себя проекцию данного объекта на глобальную плоскость XY. \en Extend given bounding box so that it enclose projection of this object to the global XY-plane. 
          void    AddRect( MbRect & rect ) const; 
          /// \ru Расширить присланный габаритный куб так, чтобы он включал в себя данный объект. \en Extend given bounding box so that it encloses the given object. 
          void    AddCube( MbCube & r ) const; 

          /// \ru Определить, пересекается ли проекция на глобальную плоскость XY треугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of triangle with a given index to the global XY-plane intersects the given rectangle. 
          bool    TriangleIntersectRect( size_t i, MbRect & rect ) const { return (i<triangles.Count()) ? triangles[i].IntersectRect( rect, points ) : false; }
          /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY треугольника с заданным номером. \en Determine bounding box of the projection of triangle with given index to the global XY-plane. 
          void    TriangleGetGabRect   ( size_t i, MbRect & rect ) const { if (i<triangles.Count()) triangles[i].GetGabRect( rect, points ); }

          /// \ru Определить, пересекается ли проекция на глобальную плоскость XY четырёхугольника с заданным номером с присланным прямоугольником. \en Determine whether the projection of quadrangle with given index to the global XY-plane intersects the given rectangle. 
          bool    QuadrangleIntersectRect( size_t i, MbRect & rect ) const { return (i<quadrangles.Count()) ? quadrangles[i].IntersectRect( rect, points ) : false; }
          /// \ru Рассчитать габаритный прямоугольник проекции на глобальную плоскость XY четырёхугольника с заданным номером. \en Determine bounding box of the projection of quadrangle with given index to the global XY-plane. 
          void    QuadrangleGetGabRect   ( size_t i, MbRect & rect ) const { if (i<quadrangles.Count()) quadrangles[i].GetGabRect( rect, points ); }

          /** 
            \brief \ru Определить положение объекта относительно плоскости.
                   \en Define the object position relative to the plane. \~
            \details \ru Определить положение объекта относительно плоскости XY локальной системы координат.
                     \en Determine the object position relative to the XY-plane of a local coordinate system. \~
            \param[in] pl - \ru Локальная система координат, задающая плоскость.
                            \en A local coordinate system which defines a plane. \~
            \param[in] eps - \ru Метрическая точность.
                             \en A metric tolerance. \~
            \param[in] onlyInItem - \ru Интересует только положение объекта над плоскостью XY локальной системы координат.
                                    \en Whether the object position relative to the XY-plane of a local coordinate system is interested only. \~
            \return \ru iloc_OnItem - объект пересекает плоскость XY локальной системы координат,\n
                    iloc_InItem - объект расположен над плоскостью XY локальной системы координат,\n
                    iloc_OutOfItem - объект расположен под плоскостью XY локальной системы координат.
                    \en Iloc_OnItem - object intersects the XY plane of a local coordinate system,\n
                    iloc_InItem - object is located over the XY plane of a local coordinate system,\n
                    iloc_OutOfItem - object is located under the XY plane of a local coordinate system. \~
          */
          MbeItemLocation GetLocation( const MbPlacement3D & pl, double eps, bool onlyInItem = false ) const; 

          /** 
            \brief \ru Определить положение объекта относительно трубы.
                   \en Determine the object position relative to the tube. \~
            \details \ru Определить, расположен ли объект внутри трубы прямоугольного сечения, 
              заданного прямоугольником в плоскости XY локальной системы координат.
                     \en Determine whether the object is inside the tube of rectangular section 
              given by a rectangle in the XY plane of a local coordinate system. \~
            \param[in] place - \ru Локальная система координат, в в плоскости XY которой лежит сечение трубы.
                               \en A local coordinate system in the XY plane of which a tube section is located. \~
            \param[in] rect - \ru Прямоугольник, задающая сечение трубы.
                              \en A rectangle which defines a tube section. \~
            \param[in] eps - \ru Метрическая точность.
                             \en A metric tolerance. \~
            \param[in] onlyInItem - \ru Интересует только положение объекта внутри трубы.
                                    \en Whether the object position relative to the tube is interested only. \~
            \return \ru true, если объект расположен внутри трубы.
                    \en True if the object is inside the tube. \~
          */
          bool    InsideLocation( const MbPlacement3D & place, MbRect & rect, double eps ) const; 

          /// \ru Преобразовать триангуляцию так, чтобы её параллельная проекция выглядела как центральная проекция, наблюдаемая из заданной точки vista. \en Transform triangulation so that its parallel projection looks as the central projection observed from the given 'vista' point. 
          void    SetVista   ( const MbCartPoint3D & vista ); 
          /// \ru Отменить преобразование триангуляцию для центральной проекции, наблюдаемой из заданной точки vista. \en Undo the transformation of triangulation for central projection observed from given 'vista' point. 
          void    DeleteVista( const MbCartPoint3D & vista ); 

          /// \ru Зарезервировать память для контейнера параметров. \en Reserve memory for container of elements. 
          bool    ParamsReserve     ( size_t n ) { params.Reserve( n ); return ((n < 1) || (params.begin() != NULL)); }
          /// \ru Зарезервировать память для контейнера точек. \en Reserve memory for container of points. 
          bool    PointsReserve     ( size_t n ) { points.Reserve( n ); return ((n < 1) || (points.begin() != NULL)); }
          /// \ru Зарезервировать память для контейнера нормалей. \en Reserve memory for container of normals. 
          bool    NormalsReserve    ( size_t n ) { normals.Reserve( n ); return ((n < 1) || (normals.begin() != NULL)); }
          /// \ru Зарезервировать память для контейнера треугольников. \en Reserve memory for container of triangles. 
          bool    TrianglesReserve  ( size_t n ) { triangles.Reserve( n ); return ((n < 1) || (triangles.begin() != NULL)); }
          /// \ru Зарезервировать память для контейнера четырёхугольников. \en Reserve memory for container of quadrangles. 
          bool    QuadranglesReserve( size_t n ) { quadrangles.Reserve( n ); return ((n < 1) || (quadrangles.begin() != NULL)); }
          /// \ru Зарезервировать память для контейнера полигонов. \en Reserve memory for container of polygons. 
          bool    PolygonsReserve      ( size_t n ) { polygons.Reserve( n ); return ((n < 1) || (polygons.begin() != NULL)); }

          /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
          bool    ReserveParamsPoints( size_t n ) {
            params.Reserve( n );
            points.Reserve( n );
            return ( (n < 1) || ((params.begin() != NULL) && (points.begin() != NULL)) );
          }
          /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
          bool    ReservePointsNormals( size_t n ) {
            points.Reserve( n );
            normals.Reserve( n );
            return ( (n < 1) || ((points.begin() != NULL) && (normals.begin() != NULL)) );
          }
          /// \ru Зарезервировать память для контейнеров. \en Reserve memory for some containers. 
          bool    ReserveParamsPointsNormals( size_t n ) {
            params.Reserve( n );
            points.Reserve( n );
            normals.Reserve( n );
            return ( (n < 1) || ((params.begin() != NULL) && (points.begin() != NULL) && (normals.begin() != NULL)) );
          }

          /// \ru Установить приращение памяти для контейнера параметров. \en Set increment of memory for container of parameters. 
          void    ParamsSetMaxDelta     ( uint16 size ) { params.SetMaxDelta( size ); }
          /// \ru Установить приращение памяти для контейнера точек. \en Set increment of memory for container of points. 
          void    PointsSetMaxDelta     ( uint16 size ) { points.SetMaxDelta( size ); }
          /// \ru Установить приращение памяти для контейнера нормалей. \en Set increment of memory for container of normals. 
          void    NormalsSetMaxDelta    ( uint16 size ) { normals.SetMaxDelta( size ); }
          /// \ru Установить приращение памяти для контейнера треугольников. \en Set increment of memory for container of triangles. 
          void    TrianglesSetMaxDelta  ( uint16 size ) { triangles.SetMaxDelta(size ); }
          /// \ru Установить приращение памяти для контейнера четырёхугольников. \en Set increment of memory for container of quadrangles. 
          void    QuadranglesSetMaxDelta( uint16 size ) { quadrangles.SetMaxDelta( size ); }
          /// \ru Установить приращение памяти для контейнера полигонов. \en Set increment of memory for container of polygons. 
          void    PolygonsSetMaxDelta   ( uint16 size ) { polygons.SetMaxDelta( size ); }

          /// \ru Выдать параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Get the parameter of triangulation calculation. 
  const   MbStepData &  GetStepData() const { return stepData; }
          /// \ru Установить параметр расчета триангуляции (стрелку прогиба или угол отклонения). \en Set the parameter of triangulation calculation. 
          void          SetStepData( const MbStepData & stData ) { stepData = stData; }

          /// \ru Удалить всю триангуляцию без освобождения памяти, занятую контейнерами. \en Delete all triangulation without freeing the memory occupied by containers. 
          void    Flush()     { params.Flush(); points.Flush(); normals.Flush(); 
                                triangles.Flush(); quadrangles.Flush(); polygons.Flush();    
                                cube.SetEmpty(); }
          /// \ru Удалить всю триангуляцию и освободить память. \en Delete all triangulation and free the memory. 
          void    HardFlush() { params.HardFlush(); points.HardFlush(); normals.HardFlush(); 
                                triangles.HardFlush(); quadrangles.HardFlush(); polygons.HardFlush();
                                cube.SetEmpty(); }
          /// \ru Освободить лишнюю память. \en Free the unnecessary memory. 
          void    Adjust()    { params.Adjust(); points.Adjust(); normals.Adjust(); 
                                triangles.Adjust(); quadrangles.Adjust(); polygons.Adjust(); }
          /// \ru Инвертировать последовательность вершин треугольников и четырехугольников. \en Reverse the sequence of vertices of triangles and quadrilaterals. 
          void    Reverse();
          /// \ru Выдать размер триангуляции в байтах. \en Get size of triangulation in bytes. 
          size_t  SizeOf() const;
          /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
          bool    IsSame( const MbGrid & init, double eps ) const;
          /// \ru Вернуть габаритный куб. \en Return bounding box. 
  const   MbCube & Cube() const { return cube; }

  /** \} */
private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation, to prevent an assignment by default. 
          void    operator = ( const MbGrid & ); 

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX( MbGrid, MATH_FUNC_EX );
  KNOWN_OBJECTS_RW_PTR_OPERATORS_EX( MbGrid, MATH_FUNC_EX );
}; // MbGrid


//------------------------------------------------------------------------------
/** \brief \ru Построить триангуляции в форме трубы заданного радиуса вокруг полигона.
           \en Construct triangulations in the form of a tube of given radius around the polygon. \~
  \details \ru Построить триангуляции в форме трубы заданного радиуса вокруг полигона. \n
           \en Construct triangulations in the form of a tube of given radius around the polygon. \n \~
  \param[in] poly - \ru Полигон.
                    \en A polygon. \~
  \param[in] radius - \ru Радиус трубы.
                      \en Radius of a tube. \~
  \param[in] sag - \ru Максимальное отклонение триангуляции от окружности (стрелка прогиба).
                   \en Maximum deviation of triangulation from a circle (sag). \~
  \param[out] grid - \ru Триангуляция - результат построения.
                     \en Triangulation - the result. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (bool) PolygonTubeGrid( const MbPolygon3D & poly, 
                                  double radius, 
                                  double sag, 
                                  MbGrid & grid );


//------------------------------------------------------------------------------
/** \brief \ru Преобразовать двумерный полигон в триангуляцию.
           \en Convert two-dimensional polygon to triangulation. \~
  \details \ru Преобразовать двумерный полигон в триангуляцию. \n
           \en Convert two-dimensional polygon to triangulation. \n \~
  \param[in] poly - \ru Двумерный полигон.
                    \en A two-dimensional polygon. \~
  \param[out] grid - \ru Построенная триангуляция.
                     \en Constructed triangulation. \~
  \ingroup Triangulation
*/
// ---
MATH_FUNC (bool) PolygonToGrid( const MbPolygon & poly, MbGrid & grid );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и треугольника.
           \en Find the intersection of a line with a triangle. \~
  \details \ru Найти пересечение прямой линии и треугольника, заданного своими вершинами. 
    Если прямая линия пересекает треугольник, то определяется значение параметра прямой линии для точки пересечения. \n
           \en Find the intersection of a line with a triangle defined by its vertices. 
    If a straight line crosses the triangle, then the value of parameter of a straight line at the intersection point is determined. \n \~
  \param[in] tri_v0 - \ru Первая вершина треугольника.
                      \en The first vertex of the triangle. \~
  \param[in] tri_v1 - \ru Вторая вершина треугольника.
                      \en The second vertex of the triangle. \~
  \param[in] tri_v2 - \ru Третья вершина треугольника.
                      \en The third vertex of the triangle. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с треугольником.
                    \en A straight line to calculate the intersection of triangle with. \~
  \param[in] epsilon - \ru Пространственная погрешность.
                       \en A spatial tolerance. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения на линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Пересекает ли прямая треугольник (true - В случае пересечения линии и треугольника).
          \en Whether the line intersects the triangle (true in case of line and triangle intersection). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(bool) LineTriangleIntersect( const MbFloatPoint3D & tri_v0,
                                       const MbFloatPoint3D & tri_v1, 
                                       const MbFloatPoint3D & tri_v2,
                                       const MbFloatAxis3D &  line,
                                             float            epsilon,
                                             MbFloatPoint3D & crossPnt,
                                             float &          tRes );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &         grid, 
                                    const MbFloatAxis3D &  line,
                                          MbFloatPoint3D & crossPnt,
                                          float &          tRes );

//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[in] cutPlace - \ru Отсекающая плоскость.
                        \en Cutting plane. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &         grid, 
                                    const MbFloatAxis3D &  line,
                                    const MbPlacement3D &  cutPlace,
                                          MbFloatPoint3D & crossPnt,
                                          float &          tRes );

//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[in] cutPlaces - \ru Отсекающая плоскости.
                         \en Cutting planes. \~
  \param[out] crossPnt - \ru Точка пересечения.
                         \en The intersection point. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid &                     grid, 
                                    const MbFloatAxis3D &              line,
                                    const std::vector<MbPlacement3D> & cutPlaces,
                                          MbFloatPoint3D &             crossPnt,
                                          float &                      tRes );


//------------------------------------------------------------------------------
/** \brief \ru Найти пересечение прямой линии и триангуляции.
           \en Find the intersection of a straight line with the triangulation. \~
  \details \ru Для всех треугольников определяется пересечение с прямой линии и вычисляется минимальное значение 
    параметра точки пересечения на секущей прямой линии. \n
           \en For all the triangles the intersection with the straight line is determined and the minimum value of 
    the intersection point parameter on the secant straight line is calculated. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] line - \ru Прямая линия, для которой вычисляется пересечение с триангуляцией.
                    \en Straight line to calculate the intersection of triangulation with. \~
  \param[out] tRes - \ru Параметр точки пересечения линии.
                     \en Parameter of the intersection point on the line. \~
  \return \ru Найдено ли пересечение (true - В случае успеха).
          \en Whether the intersection is found (true if success). \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) LineGridIntersect( const MbGrid        & grid, 
                                    const MbFloatAxis3D & line,
                                          float         & tRes );


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
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] edgeInd - \ru Индекс тестируемого полигона.
                       \en Index of polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] delta - \ru Радиус окрестности вокруг линии.
                     \en Neighborhood radius around the line. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToGridEdgeDistanceSquared( const MbGrid        & grid,
                                                      size_t          edgeInd,
                                                const MbFloatAxis3D & line,
                                                      float           delta,
                                                      float         & tRes );


//------------------------------------------------------------------------------
/** \brief \ru Вычислить квадрат расстояния от линии до полигона.
           \en Calculate squared distance from a line to a polygon. \~
  \details \ru При вычислении квадрата расстояния от линии до полигона проверяется расстояние от каждого 
    сегмента полигона до первого попадания в окрестность delta.
    Возвращается значение параметра ближайшей точки на линии tRes, вектор между ближайшими точками и квадрат 
    расстояния от этой точки до сегмента полигона. \n
           \en During calculation of squared distance from a line to a polygon the distance from each 
    segment of the polygon is checked until the first getting to 'delta' neighborhood.
    Returns the value of the nearest point parameter on the line, the vector between the nearest points 
    and the squared distance from this point to a segment of the polygon. \n \~
  \param[in] grid - \ru Триангуляция.
                    \en Triangulation. \~
  \param[in] edgeInd - \ru Индекс тестируемого полигона.
                       \en Index of polygon to check. \~
  \param[in] line - \ru Линия, до которой вычисляется расстояние.
                    \en Line to calculate the distance to. \~
  \param[in] vDelta - \ru Габарит окрестности вокруг линии.
                     \en The dimensions of the area around the line. \~
  \param[out] vRes - \ru Вектор от ближайшей точки на линии до ближайшей точки на полигоне.
                     \en Vector from the nearest point on the line to the nearest point on the polygon. \~
  \param[out] tRes - \ru Значение параметра ближайшей точки линии.
                     \en The value of parameter of the nearest point on the line. \~
  \return \ru Квадрат расстояния ближайшей точки до линии.
          \en Squared distance between the nearest point and the line. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC(float) LineToGridEdgeDistanceSquared( const MbGrid          & grid,
                                                      size_t            edgeInd,
                                                const MbFloatAxis3D   & line,
                                                const MbFloatVector3D & vDelta,
                                                      MbFloatVector3D & vRes,
                                                      float           & tRes );


#endif // __MESH_GRID_H
