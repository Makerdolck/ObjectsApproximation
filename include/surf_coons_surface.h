////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Бикубическая поверхность Кунса на четырех кривых и их поперечных производных.
         \en Bicubic Coons surface on four curves and its transverse derivatives. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SURF_COONS_SURFACE_H
#define __SURF_COONS_SURFACE_H


#include <surface.h>


#define COONS_COUNT 4 ///< \ru Число кривых, используемых для построения поверхности Кунса \en Count of curves used to construct Coons surface. 


//------------------------------------------------------------------------------
/** \brief \ru Способ расчёта поверхности Кунса.
\en Type of calculation of Coons surface. \~
\details \ru Способ расчёта поверхности Кунса. \n
\en Type of calculation of Coons surface. \n \~
\ingroup Surfaces
*/ 
// ---
enum MbeCoonsSurfaceCalcType {
  mcsv_DefaultType = 0, ///< \ru Способ по умолчанию. \en Default type.
  mcsv_LoftSurfType,    ///< \ru Способ расчёта для построения поверхности по сечениям. \en Type for the construction of the lofted surface.
};


//------------------------------------------------------------------------------
/** \brief \ru Поверхность Кунса на четырех кривых.
           \en Coons surface on four curves. \~
  \details \ru Бикубическая поверхность Кунса определяется четырьмя кривыми и 
    производными поверхности на этих кривых в поперечном к кривым направлениях. 
    Поверхность проходит через определяющие её кривые и 
    имеет заданные производные на этих кривых в поперечном к кривым направлениях. \n
           \en Bicubic Coons surface is determined by four curves and 
    surface derivatives on these curves in transverse directions to curves. 
    Surface passes through its determining curves and 
    has specified derivatives on this curves in transverse directions to curves. \n \~
  \ingroup Surfaces
*/
// ---
class MATH_CLASS MbCoonsPatchSurface : public MbSurface {

//                                                      curve2V
//                                             t2min    curve2    t2max
//                                           P3 ______________________ P2
//                                       t3max |                      | t1max
//                                             |                      |
//                                             |                      |
//                                     curve3  |                      | curve1
//                                     curve3U |                      | curve1U
//     t0=t0min*(1-u)+t0max*u                  |                      |
//     t1=t1min*(1-v)+t1max*v                  |                      |
//     t2=t2min*(1-u)+t2max*u            t3min |______________________| t1min
//     t3=t3min*(1-v)+t3max*v                P0                        P1
//                                             t0min    curve0    t0max
//                                                      curve0V
// \ru Не переименовывать в MbCoonsSurface - хэш совпал с существующим объектом (BUG_60351). \en No renaming to MbCoonsSurface - hash was coincided with existing object (BUG_60351). 

private:
  MbCurve3D *   curve0;  ///< \ru Кривая  0. \en Curve 0. 
  MbCurve3D *   curve1;  ///< \ru Кривая  1. \en Curve 1. 
  MbCurve3D *   curve2;  ///< \ru Кривая  2. \en Curve 2. 
  MbCurve3D *   curve3;  ///< \ru Кривая  3. \en Curve 3. 
  MbCurve3D *   curve0V; ///< \ru Производная по v вдоль кривой  0. \en Derivative by v along curve 0. 
  MbCurve3D *   curve1U; ///< \ru Производная по u вдоль кривой  1. \en Derivative by u along curve 1. 
  MbCurve3D *   curve2V; ///< \ru Производная по v вдоль кривой  2. \en Derivative by v along curve 2. 
  MbCurve3D *   curve3U; ///< \ru Производная по u вдоль кривой  3. \en Derivative by u along curve 3. 
  MbCartPoint3D vertex[COONS_COUNT];   ///< \ru Вершины. \en Vertices. 
  MbCartPoint3D vertexU[COONS_COUNT];  ///< \ru Производная по u в вершинах. \en Derivative by u at vertices. 
  MbCartPoint3D vertexV[COONS_COUNT];  ///< \ru Производная по v в вершинах. \en Derivative by v at vertices. 
  MbCartPoint3D vertexUV[COONS_COUNT]; ///< \ru Производная по uv в вершинах. \en Derivative by uv at vertices. 
  double        t0min;   ///< \ru Минимальное значение параметра на кривой  0. \en Minimal value of parameter on curve 0. 
  double        t0max;   ///< \ru Максимальное значение параметра на кривой 0. \en Maximal value of parameter on curve 0. 
  double        t1min;   ///< \ru Минимальное значение параметра на кривой  1. \en Minimal value of parameter on curve 1. 
  double        t1max;   ///< \ru Максимальное значение параметра на кривой 1. \en Maximal value of parameter on curve 1. 
  double        t2min;   ///< \ru Минимальное значение параметра на кривой  2. \en Minimal value of parameter on curve 2. 
  double        t2max;   ///< \ru Максимальное значение параметра на кривой 2. \en Maximal value of parameter on curve 2. 
  double        t3min;   ///< \ru Минимальное значение параметра на кривой  3. \en Minimal value of parameter on curve 3. 
  double        t3max;   ///< \ru Максимальное значение параметра на кривой 3. \en Maximal value of parameter on curve 3. 
  double        lK0min;  ///< \ru Коэффициент длины трансверсальной производной при минимальном параметре на кривой 0.
  double        lK0max;  ///< \ru Коэффициент длины трансверсальной производной при максимальном параметре на кривой 0.
  double        lK1min;  ///< \ru Коэффициент длины трансверсальной производной при минимальном параметре на кривой 1.
  double        lK1max;  ///< \ru Коэффициент длины трансверсальной производной при максимальном параметре на кривой 1.
  double        lK2min;  ///< \ru Коэффициент длины трансверсальной производной при минимальном параметре на кривой 2.
  double        lK2max;  ///< \ru Коэффициент длины трансверсальной производной при максимальном параметре на кривой 2.
  double        lK3min;  ///< \ru Коэффициент длины трансверсальной производной при минимальном параметре на кривой 3.
  double        lK3max;  ///< \ru Коэффициент длины трансверсальной производной при максимальном параметре на кривой 3.
  bool          uclosed; ///< \ru Замкнутость по u. \en Closeness by u. 
  bool          vclosed; ///< \ru Замкнутость по v. \en Closeness by v. 

  MbeCoonsSurfaceCalcType calcType;  ///< \ru Версия реализации определяет способ расчёта поверхности. \en Version of implementation determines a type of calculation of surface.

public:
  /** \brief \ru Конструктор поверхности Кунса.
             \en Constructor of Coons surface. \~
      \details \ru Конструктор поверхности Кунса по набору кривых и производных вдоль кривых.
               \en Constructor of Coons surface by set of curves and derivatives along curves. \~
      \param[in] initCurve0 - \ru Кривая 0.
                              \en Curve 0. \~
      \param[in] initCurve1 - \ru Кривая 1.
                              \en Curve 1. \~
      \param[in] initCurve2 - \ru Кривая 2.
                              \en Curve 2. \~
      \param[in] initCurve3 - \ru Кривая 3.
                              \en Curve 3. \~
      \param[in] derVCurve0 - \ru Производная по v вдоль кривой  0.
                              \en Derivative by v along curve 0. \~
      \param[in] derUCurve1 - \ru Производная по u вдоль кривой  1.
                              \en Derivative by u along curve 1. \~
      \param[in] derVCurve2 - \ru Производная по v вдоль кривой  2.
                              \en Derivative by v along curve 2. \~
      \param[in] derUCurve3 - \ru Производная по u вдоль кривой  3.
                              \en Derivative by u along curve 3. \~
  */
  MbCoonsPatchSurface ( MbCurve3D & initCurve0, MbCurve3D & initCurve1, MbCurve3D & initCurve2, MbCurve3D & initCurve3,
                        MbCurve3D & derVCurve0, MbCurve3D & derUCurve1, MbCurve3D & derVCurve2, MbCurve3D & derUCurve3,
                        MbeCoonsSurfaceCalcType calcType = mcsv_DefaultType );
private:
  MbCoonsPatchSurface( const MbCoonsPatchSurface & ); // \ru Не реализовано. \en Not implemented. 
  MbCoonsPatchSurface( const MbCoonsPatchSurface &, MbRegDuplicate * ); ///< \ru Конструктор копирования. \en Copy-constructor. 
public:
  virtual ~MbCoonsPatchSurface( void );

public:
  VISITING_CLASS( MbCoonsPatchSurface );

  /// \ru Инициализация поверхности Кунса заданной поверхностью Кунса. \en Initialization of Coons surface by specified Coons surface. 
          void    Init( const MbCoonsPatchSurface & );

  /** \ru \name Общие функции геометрического объекта
      \en \name Common functions of a geometric object
      \{ */
  virtual MbeSpaceType  IsA() const; // \ru Тип элемента \en A type of element 
  virtual MbSpaceItem & Duplicate( MbRegDuplicate * = NULL )  const; // \ru Cделать копию элемента \en Make a copy of element 
  virtual bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const;   // \ru Является ли объект копией. \en Whether the object is a copy. 
  virtual bool    SetEqual( const MbSpaceItem & init );        // \ru Сделать равным \en Make equal 
  virtual bool    IsSimilar( const MbSpaceItem & init ) const; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  virtual void    Transform( const MbMatrix3D & matr, MbRegTransform * = NULL ); // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void    Move( const MbVector3D & to, MbRegTransform * = NULL );                 // \ru Сдвиг \en Translation 
  virtual void    Rotate( const MbAxis3D & axis, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate around an axis 

  virtual void    GetProperties( MbProperties & ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void    SetProperties( const MbProperties & ); // \ru Записать свойства объекта \en Set properties of the object 
  virtual void    GetBasisItems ( RPArray <MbSpaceItem> &s ); // \ru Дать базовые объекты \en Get the base objects 
  virtual void    GetBasisPoints( MbControlData3D & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData3D & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */

  /** \ru \name Функции описания области определения поверхности
      \en \name Functions for surface domain description
      \{ */
  virtual double  GetUMin() const;
  virtual double  GetVMin() const;
  virtual double  GetUMax() const;
  virtual double  GetVMax() const;
  virtual bool    IsUClosed() const; // \ru Замкнута ли поверхность по параметру u. \en Whether the surface is closed by parameter u. 
  virtual bool    IsVClosed() const; // \ru Замкнута ли поверхность по параметру v. \en Whether the surface is closed by parameter v. 
  virtual bool    GetPoleUMin() const;
  virtual bool    GetPoleUMax() const;
  virtual bool    GetPoleVMin() const;
  virtual bool    GetPoleVMax() const;
  virtual bool    IsPole( double u, double v ) const; // \ru Является ли точка особенной \en Whether the point is special 
  /** \} */

  /** \ru \name Функции для работы в области определения поверхности
            Функции PointOn, Derive... поверхностей корректируют параметры
            при выходе их за пределы прямоугольной области определения параметров.\n
      \en \name Functions for working at surface domain
            Functions PointOn, Derive... of surfaces correct parameters
            when they are out of bounds of rectangular domain of parameters.\n
      \{ */
  virtual void    PointOn  ( double & u, double & v, MbCartPoint3D & ) const; // \ru Точка на поверхности \en Point on the surface 
  virtual void    DeriveU  ( double & u, double & v, MbVector3D & ) const; // \ru Первая производная по u \en First derivative with respect to u 
  virtual void    DeriveV  ( double & u, double & v, MbVector3D & ) const; // \ru Первая производная по v \en First derivative with respect to v 
  virtual void    DeriveUU ( double & u, double & v, MbVector3D & ) const; // \ru Вторая производная по u \en Second derivative with respect to u 
  virtual void    DeriveVV ( double & u, double & v, MbVector3D & ) const; // \ru Вторая производная по v \en Second derivative with respect to v 
  virtual void    DeriveUV ( double & u, double & v, MbVector3D & ) const; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  virtual void    DeriveUUU( double & u, double & v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveUUV( double & u, double & v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveUVV( double & u, double & v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void    DeriveVVV( double & u, double & v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void    Normal   ( double & u, double & v, MbVector3D & ) const; // \ru Нормаль \en Normal 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения поверхности
            функции _PointOn, _Derive... поверхностей не корректируют
            параметры при выходе их за пределы прямоугольной области определения параметров.
      \en \name Functions for working inside and outside the surface's domain
            functions _PointOn, _Derive... of surfaces don't correct
            parameters when they are out of bounds of rectangular domain of parameters.
      \{ */
  virtual void   _PointOn  ( double  u, double  v, MbCartPoint3D & ) const; // \ru Точка на расширенной поверхности \en Point on the extended surface 
  virtual void   _DeriveU  ( double  u, double  v, MbVector3D & ) const; // \ru Первая производная по u \en First derivative with respect to u 
  virtual void   _DeriveV  ( double  u, double  v, MbVector3D & ) const; // \ru Первая производная по v \en First derivative with respect to v 
  virtual void   _DeriveUU ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная по u \en Second derivative with respect to u 
  virtual void   _DeriveVV ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная по v \en Second derivative with respect to v 
  virtual void   _DeriveUV ( double  u, double  v, MbVector3D & ) const; // \ru Вторая производная по uv \en Second derivative with respect to uv 
  virtual void   _DeriveUUU( double  u, double  v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void   _DeriveUUV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void   _DeriveUVV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void   _DeriveVVV( double  u, double  v, MbVector3D & ) const; // \ru Третья производная \en Third derivative 
  virtual void   _Normal   ( double  u, double  v, MbVector3D & ) const; // \ru Нормаль \en Normal 
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметров поверхности.
      \en \name Functions for get of the group of data inside and outside the surface's domain of parameters.
      \{ */
  virtual void    Explore( double & u, double & v, bool ext,
                           MbCartPoint3D & pnt, MbVector3D & uDer, MbVector3D & vDer,
                           MbVector3D * uuDer, MbVector3D * vvDer, MbVector3D * uvDer, MbVector3D * nor ) const;
  /** \} */

  /** \ru \name Функции движения по поверхности
      \en \name Functions of moving along the surface
      \{ */
  virtual size_t  GetUCount() const;
  virtual size_t  GetVCount() const;
  /** \} */

  /** \ru \name Общие функции поверхности
      \en \name Common functions of surface
      \{ */
  virtual MbCurve3D * CurveU( double v, MbRect1D * pRgn, bool bApprox = true ) const; // \ru Пространственная копия линии v = const \en Spatial copy of 'v = const'-line 
  virtual MbCurve3D * CurveV( double u, MbRect1D * pRgn, bool bApprox = true ) const; // \ru Пространственная копия линии u = const \en Spatial copy of 'u = const'-line 

  virtual void    ChangeCarrier( MbSpaceItem & item, MbSpaceItem & init ); // \ru Изменение носителя \en Changing of carrier 
  // \ru Существует ли полюс на границе параметрической области \en Whether there is pole on boundary of parametric region 

  virtual size_t  GetUMeshCount() const; // \ru Выдать количество полигонов по u \en Get the count of polygons by u 
  virtual size_t  GetVMeshCount() const; // \ru Выдать количество полигонов по v \en Get the count of polygons by v 

  /// \ru Получить кривую 0. \en Get curve 0. 
  const   MbCurve3D & GetCurve0() const { return *curve0; } 
  /// \ru Получить кривую 1. \en Get curve 1. 
  const   MbCurve3D & GetCurve1() const { return *curve1; } 
  /// \ru Получить кривую 2. \en Get curve 2. 
  const   MbCurve3D & GetCurve2() const { return *curve2; } 
  /// \ru Получить кривую 3. \en Get curve 3. 
  const   MbCurve3D & GetCurve3() const { return *curve3; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 0. \en Get derivative curve transversal to curve 0.
  const   MbCurve3D & GetDerCurve0() const { return *curve0V; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 1. \en Get derivative curve transversal to curve 1.
  const   MbCurve3D & GetDerCurve1() const { return *curve1U; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 2. \en Get derivative curve transversal to curve 2.
  const   MbCurve3D & GetDerCurve2() const { return *curve2V; } 
  /// \ru Получить кривую производной в трансверсальном направлении к кривой 3. \en Get derivative curve transversal to curve 3.
  const   MbCurve3D & GetDerCurve3() const { return *curve3U; } 
  /** \} */

private:
          void    operator = ( const MbCoonsPatchSurface & ); // \ru Не реализовано. \en Not implemented. 
          void    Setup();
          // \ru Определение местных координат. \en Determination of local coordinates. 
          void    CalculateCoordinate( double & u, double & v, bool ext,
                                       double & t0, double & t1, double & t2, double & t3 ) const;
          void    CalculatePoint ( double & u, double & v, bool ext,
                                   MbCartPoint3D * point, MbCartPoint3D * pointUV ) const;
          void    CalculateFirst ( double & u, double & v, bool ext,
                                   MbCartPoint3D * point, MbVector3D * first, 
                                   MbCartPoint3D * pointUV, MbVector3D * firstUV ) const;
          void    CalculateThird ( double & u, double & v, bool ext,
                                   MbCartPoint3D * point, MbVector3D * third,
                                   MbCartPoint3D * pointUV, MbVector3D * thirdUV ) const;
          void    CalculateExplore( double & u, double & v, bool ext,
                                    MbCartPoint3D * point, MbVector3D * first, MbVector3D * second, 
                                    MbCartPoint3D * pointUV, MbVector3D * firstUV, MbVector3D * secondUV ) const;
          void    Derivatives( double & u, double & v, bool ext, MbVector3D & uDer, MbVector3D & vDer ) const; // \ru Ппроизводные. \en Derivatives with respect to u and to v.
  inline  void    ParamPoint ( double w, double * t ) const;
  inline  void    ParamFirst ( double w, double * t ) const;
  inline  void    ParamSecond( double w, double * t ) const;
  inline  void    ParamThird ( double w, double * t ) const;
  inline  void    AddMatrix  ( double * uu, double * vv, MbVector3D & p ) const; // \ru Добавить матрицу поверхности. \en Add the matrix of the surface.

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCoonsPatchSurface )
}; // MbCoonsSurface


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра точки \en Determination of array of degrees of point parameter 
// ---
inline void MbCoonsPatchSurface::ParamPoint ( double w, double * t ) const {
  t[0] = 1.0 - 3.0 * w * w + 2.0 * w * w * w; //*/ 1.0 - 10.0 * w * w * w + 15.0 * w * w * w * w - 6.0 * w * w * w * w * w;
  t[1] =       3.0 * w * w - 2.0 * w * w * w; //*/       10.0 * w * w * w - 15.0 * w * w * w * w + 6.0 * w * w * w * w * w;
  t[2] = w   - 2.0 * w * w +       w * w * w; //*/ 0.0;
  t[3] =     -       w * w +       w * w * w; //*/ 0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра производной \en Determination of array of degrees of derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamFirst ( double w, double * t ) const {
  t[0] =     - 6.0 * w + 6.0 * w * w; //*/ -30.0 * w * w + 60.0 * w * w * w - 30.0 * w * w * w * w;
  t[1] =       6.0 * w - 6.0 * w * w; //*/  30.0 * w * w - 60.0 * w * w * w + 30.0 * w * w * w * w;
  t[2] = 1.0 - 4.0 * w + 3.0 * w * w; //*/ 0.0;
  t[3] =     - 2.0 * w + 3.0 * w * w; //*/ 0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра второй производной \en Determination of array of degrees of second derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamSecond( double w, double * t ) const {
  t[0] = - 6.0 + 12.0 * w; //*/ -60.0 * w + 180.0 * w * w - 120.0 * w * w * w;
  t[1] =   6.0 - 12.0 * w; //*/  60.0 * w - 180.0 * w * w + 120.0 * w * w * w;
  t[2] = - 4.0 + 6.0  * w; //*/  0.0;
  t[3] = - 2.0 + 6.0  * w; //*/  0.0;
}


//------------------------------------------------------------------------------
// \ru Определение массива степеней параметра третьей производной \en Determination of array of degrees of third derivative parameter 
// ---
inline void MbCoonsPatchSurface::ParamThird ( double /*w*/, double * t ) const {
  t[0] =  12.0; //*/ -60.0 + 360.0 * w - 360.0 * w * w;
  t[1] = -12.0; //*/  60.0 - 360.0 * w + 360.0 * w * w;
  t[2] =   6.0; //*/   0.0;
  t[3] =   6.0; //*/   0.0;
}


//------------------------------------------------------------------------------
// \ru Добавить матрицу поверхности. \en Add the matrix of the surface.
// ---
inline void MbCoonsPatchSurface::AddMatrix( double * uu, double * vv, MbVector3D & p ) const {
  p.Add( vertex[0],   -uu[0]*vv[0], vertex[1],   -uu[1]*vv[0], vertex[2],   -uu[1]*vv[1], vertex[3],   -uu[0]*vv[1] );
  p.Add( vertexU[0],  -uu[2]*vv[0], vertexU[1],  -uu[3]*vv[0], vertexU[2],  -uu[3]*vv[1], vertexU[3],  -uu[2]*vv[1] );
  p.Add( vertexV[0],  -uu[0]*vv[2], vertexV[1],  -uu[1]*vv[2], vertexV[2],  -uu[1]*vv[3], vertexV[3],  -uu[0]*vv[3] );
  p.Add( vertexUV[0], -uu[2]*vv[2], vertexUV[1], -uu[3]*vv[2], vertexUV[2], -uu[3]*vv[3], vertexUV[3], -uu[2]*vv[3] );
}


#endif // __SURF_COONS_SURFACE_H
