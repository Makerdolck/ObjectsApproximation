////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Двумерная NURBS кривая.
         \en Two-dimensional NURBS curve. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_NURBS_H
#define __CUR_NURBS_H


#include <templ_c_array.h>
#include <templ_array2.h>
#include <tool_multithreading.h>
#include <mb_homogeneous.h>
#include <mb_nurbs_function.h>
#include <mb_point_mating.h>
#include <cur_nurbs_vector.h> // BUG_22943, VASE2.SAT 
#include <cur_polycurve.h>
#include <alg_base.h>


class  MATH_CLASS MbLine;
class  MATH_CLASS MbBezier;
class  MATH_CLASS MbContour;


//------------------------------------------------------------------------------
/** \brief \ru NURBS кривая в двумерном пространстве.
           \en NURBS curve in two-dimensional space. \~
  \details \ru NURBS кривая определяется контрольными точками pointList, 
    весами контрольных точек weights, узловым вектором knots и порядком сплайна degree.\n
    Аббревиатура NURBS получена из первых букв словосочетания Non-Uniform Rational B-Spline.
    NURBS кривая не проходит через свои контрольные точки.
    Узловой вектор knots должен представлять собой неубывающую последовательность действительных чисел.
    Множества pointList и weights должны содержать одинаковое количество элементов. 
    Для не замкнутой кривой узловой вектор knots должен содержать количество элементов множества pointList плюс degree.
    Для замкнутой кривой кривой узловой вектор knots должен содержать количество элементов множества pointList плюс 2*degree-1. 
    Минимальное значение параметра сплайна равно значению элемента узлового вектора с индексом degree-1. 
    Максимальное значение параметра сплайна равно значению элемента узлового вектора с индексом, равным последнему элементу минус degree-1. 
    Расчет кривой в каждой своей точке производится на основе нормированных неоднородных В-сплайнов.\n
    Семейство В-сплайнов определяется заданной неубывающей последовательностью узловых параметров и заданным порядком B-сплайна.\n
           \en NURBS curve is defined by 'pointList' control points, 
    weights of control points ('weights'), knot vector ('knots') and order of spline ('degree').\n
    Abbreviation of NURBS is obtained from the first letters of the Non-Uniform Rational B-Spline phrase.
    NURBS curve doesn't pass through its control points.
    'knots' knot vector has to be not decreasing sequence of real numbers.
    'pointList' and 'weights' sets have to contain the same count of elements. 
    For not closed curve 'knots' knot vector has to contain the count of elements of 'pointList' set plus 'degree'.
    For closed curve 'knots' knot vector has to contain the count of elements of 'pointList' set plus 2*degree-1. 
    Minimal value of spline parameter is equal to value of element of knot vector with degree-1 index. 
    Maximal value of spline parameter is equal to value of element of knot vector with index, which is equal to index of last element minus degree-1. 
    Curve calculation at each point is performed by normalized non-uniform B-splines.\n
    Family of B-splines is defined by given crescent sequence of knot parameters and given order of B-spline.\n \~
  \ingroup Curves_2D
*/
// ---

class MATH_CLASS MbNurbs : public MbPolyCurve {
private:
  ptrdiff_t         degree;        ///< \ru Порядок В-сплайна (порядок = степень + 1). \en Order of B-spline (order = degree + 1). 
  ptrdiff_t         uppKnotsIndex; ///< \ru Последний индекс узлового вектора. \en Last index of knot vector. 
  SArray<double>    knots;         ///< \ru Узловой вектор. \en Knot vector. 
  SArray<double>    weights;       ///< \ru Множество весов контрольных точек. \en Set of weights of the control points. 
  MbeNurbsCurveForm form;          ///< \ru Форма кривой. \en Form of curve. 

private:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /** \brief \ru Вспомогательные данные. 
             \en Auxiliary data. \~
      \details \ru Вспомогательные данные служат для ускорения работы объекта.
               \en Auxiliary data are used for fast calculations. \n \~
    */
  // ---
  class MbNurbsAuxiliaryData : public AuxiliaryData {
  public:
    double            tCalc; ///< \ru Текущее значение параметра, по которому вычислены временные данные кривой. \en Current value of parameter the curve temporary data are calculated by. 
    double          * nd;    ///< \ru B - базис. \en B - basis. 
    MbHomogeneous   * h0;    ///< \ru Текущий сегмент. \en Current segment. 
    MbHomogeneous   * h1;    ///< \ru Текущий сегмент. \en Current segment. 
    MbHomogeneous   * h2;    ///< \ru Текущий сегмент. \en Current segment. 
    MbHomogeneous   * h3;    ///< \ru Текущий сегмент. \en Current segment. 
    double          * wc;               ///< \ru Насчитанные значения весов (может быть ноль). \en Calculated values of points (can be null). 
    MbVector          rc[cdt_CountDer]; ///< \ru Насчитанные значения точек. \en Calculated values of points. 
    ptrdiff_t         leftIndex;        ///< \ru Левый индекс узлового вектора. \en Left index of knot vector. 
    double          * m_left;
    double          * m_right;
  public:
    MbNurbsAuxiliaryData();
    MbNurbsAuxiliaryData( const MbNurbsAuxiliaryData & init );
    virtual ~MbNurbsAuxiliaryData();
    void FreeMemory();
    bool CatchMemory(  ptrdiff_t, bool );
  }; // MbNurbsAuxiliaryData 

  mutable CacheManager<MbNurbsAuxiliaryData> cache;

public://protected:
  DEPRECATE_DECLARE MbNurbs();
protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по порядку, точкам, параметрам и признаку замкнутости.
      При недопустимых параметрах initDegree и points поведение кривой не определено.\n
             \en Constructor by order, points, parameters and an attribute of closedness.
      If parameters initDegree and points is invalid then the curve behavior is undefined. \n \~
    \param[in] degree  - \ru Порядок сплайна. 
                         Должен быть больше единицы. Не должен превышать количество характерных точек.
                         \en A spline order. 
                         It must be greater than unity. It shouldn't exceed count of control points. \~
    \param[in] cls     - \ru Признак замкнутости.
                         \en A closedness attribute. \~
    \param[in] points  - \ru Набор характерных точек. 
                         Количество точек должно быть больше или равно двум.
                         \en Set of control points. 
                         Count of points must be greater than or equal to two. \~
    \param[in] weights - \ru Набор весов для характерных точек. 
                            Количество весов должно соответствовать количеству точек.
                            \en Set of weights for control points. 
                            Count of weights must be equal to count of points. \~
    \param[in] knots   - \ru Последовательность узловых параметров.
                         \en Sequence of knot parameters. \~ 
    
  */
  MbNurbs( ptrdiff_t degree, bool cls, const SArray<MbCartPoint> & points,
           const SArray<double> * weights = NULL, const SArray<double> * knots = NULL );
  MbNurbs( const MbNurbs & );
public :
  virtual ~MbNurbs();

public:
  /** \brief \ru Создать копию сплайна.
             \en Create copy of spline. \~
      \details \ru Создать копию сплайна.\n
               \en Create copy of spline.\n \~
  */
  static  MbNurbs * Create( const MbNurbs & );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
      \details \ru Создать сплайн и установить параметры сплайна.\n
               \en Create spline and set parameters of spline.\n \~
      \param[in] degree  - \ru Порядок сплайна.
                           \en A spline order. \~ 
      \param[in] points  - \ru Набор характерных точек.
                           \en Set of control points. \~
      \param[in] cls     - \ru Признак замкнутости.
                           \en A closedness attribute. \~
      \param[in] weights - \ru Набор весов для характерных точек.
                           \en Set of weights for control points. \~ 
  */
  static  MbNurbs * Create( ptrdiff_t degree, const SArray<MbCartPoint> & points, bool cls,
                            const SArray<double> * weights = NULL );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Создать сплайн и установить параметры сплайна.\n
             \en Create spline and set parameters of spline.\n \~
    \param[in] initDegree  - \ru Порядок сплайна.
                             \en A spline order. \~ 
    \param[in] initClosed  - \ru Признак замкнутости.
                              \en A closedness attribute. \~
    \param[in] initPoints  - \ru Набор характерных точек.
                             \en Set of control points. \~ 
    \param[in] initWeights - \ru Набор весов для характерных точек.
                             \en Set of weights for control points. \~
    \param[in] initKnots   - \ru Неубывающая последовательность весов.
                             \en Non-decreasing sequence of weights. \~
    \param[in] initForm    - \ru Тип построения.
                             \en Type of construction. \~
  */
  template <class PointsVector, class DoubleVector>
  static  MbNurbs * Create( ptrdiff_t initDegree, bool initClosed, const PointsVector & initPoints, 
                            const DoubleVector & initWeights, const DoubleVector & initKnots, 
                            MbeNurbsCurveForm initForm = ncf_Unspecified )
  {
#if defined (_MSC_VER) && !defined(ALL_WARNINGS)  // Set warnings level
#pragma warning(disable: 4996) // MbNurbs::MbNurbs': This item is deprecated for external using.
#endif
    MbNurbs * resNurbs = new MbNurbs;
#if defined (_MSC_VER) && !defined(ALL_WARNINGS)  // Set warnings level
#pragma warning(default: 4996) // MbNurbs::MbNurbs': This item is deprecated for external using.
#endif
    if ( !resNurbs->Init( initDegree, initClosed, initPoints, initWeights, initKnots, initForm ) )
      ::DeleteItem( resNurbs );
    return resNurbs;
  }

  /** \brief \ru Создать сплайн.
             \en Create spline. \~
      \details \ru Сплайн, проходящий через заданные точки при заданных параметрах.\n
        В случае замкнутости нужно передавать массив с совпадением первой и последней точек.\n
               \en Spline passing through given points at given parameters.\n
        In case of closedness it is necessary to pass the array with coincidence of the first and the last points.\n \~
      \param[in] degree  - \ru Порядок сплайна.
                           \en A spline order. \~
      \param[in] cls     - \ru Признак замкнутости.
                           \en A closedness attribute. \~
      \param[in] points  - \ru Набор точек, через которые проходит сплайн.
                           \en Set of points which the spline passes through. \~
      \param[in] params  - \ru Последовательность узловых параметров.
                           \en Sequence of knot parameters. \~
  */
  static  MbNurbs * CreateThrough( ptrdiff_t degree, bool cls, const SArray<MbCartPoint> & points,
                                   const SArray<double> & params );
  /** \brief \ru Заполнить NURBS по данным parasolid.
              \en Fill NURBS by parasolid data. \~
      \details \ru Заполнить NURBS по данным parasolid.\n
                \en Fill NURBS by parasolid data.\n \~
      \param[in] degree     - \ru Степень сплайна.
                              \en Order of spline. \~
      \param[in] closed     - \ru Признак замкнутости.
                              \en A closedness attribute. \~
      \param[in] rational   - \ru Является ли кривая рациональной. Если true - строится NURBS, false - кривая Безье.
                              \en Whether the curve is rational. if true then NURBS is created, false - Bezier curve. \~
      \param[in] count      - \ru Количество характерных точек.
                              \en Count of control points. \~
      \param[in] verts      - \ru Массив координат точек. Если сплайн рациональный, четвертая координата - вес точки.
                              \en An array of coordinates of points. If spline is rational, then the fourth coordinate - weight of point. \~ 
      \param[in] vertsCount - \ru Количество элементов в массиве verts.
                              \en Count of elements in 'verts' array. \~
      \param[in] mul        - \ru Массив с данными о кратности каждого узла.
                              \en Array with multiplicity of each knot. \~
      \param[in] mulCount   - \ru Количество элементов в массиве mul.
                              \en Count of elements in 'mul' array. \~
      \param[in] knots       - \ru Массив со значениями параметров в узлах. Каждое значение представлено один раз. 
                               Информация о кратности узла лежит в элементе массива mul с тем же номером.
                               \en Array with values of parameters at knots. Each value is presented only once. 
                               Information about knot multiplicity is in the element of 'mul' array with the same index. \~
      \param[in] knotsCount  - \ru Количество элементов в массиве knots.
                               \en Count of elements in 'knots' array. \~
      \param[in] scl         - \ru Коэффициент масштабирования.
                               \en A scale factor. \~
  */
  static  MbNurbs * CreateParasolid( ptrdiff_t degree, bool closed, bool rational, ptrdiff_t count,
                                     const CcArray<double> & verts, ptrdiff_t vertsCount,
                                     const CcArray<ptrdiff_t> & mul, ptrdiff_t mulCount,
                                     const CcArray<double> & knots, ptrdiff_t knotsCount,
                                     double scl );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
      \details \ru Создать сплайн и установить параметры сплайна.\n
               \en Create spline and set parameters of spline. \n \~
      \param[in] p1  - \ru Начальная точка, через которую проходит сплайн.
                       \en The initial point the spline passes through. \~
      \param[in] v1  - \ru Касательный вектор к кривой в начальной точке.
                       \en A tangent vector to the curve at the start point. \~
      \param[in] p2  - \ru Конечная точка, через которую проходит сплайн.
                       \en The final point the spline passes through. \~
      \param[in] v2  - \ru Касательный вектор к кривой в конечной точке.
                       \en A tangent vector to the curve at the end point. \~
  */
  static  MbNurbs * CreateCube( const MbCartPoint & p1, const MbVector & v1, const MbCartPoint & p2, const MbVector & v2 );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Конструктор сплайна, описывающего дугу конического сечения.\n
             \en Constructor of a spline describing the arc of a conic section. \n \~
    \param[in] points  - \ru Набор из четырех точек, через которое проходит сечение.
                         Первая и последняя точки определяют начало и конец дуги, соответственно.
                         \en A set of four points the section passes through.
                         The first and last points define the beginning and ending of the arc respectively. \~
  */
  static  MbNurbs * CreateArc( const SArray<MbCartPoint> & points );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Конструктор сплайна, описывающего дугу окружности.\n
             \en Constructor of a spline describing the arc of a circle. \n \~
    \param[in] a2 - \ru Половина угла раствора.
                    \en A half opening angle. \~
    \param[in] p1 - \ru Начальная точка дуги.
                    \en The starting point of the arc. \~
    \param[in] p2 - \ru Конечная точка дуги.
                    \en The end point of the arc. \~
  */
  static  MbNurbs * CreateArc( double a2, const MbCartPoint & p1, const MbCartPoint & p2 );
  /** \brief \ru Создать сплайн.
             \en Create spline. \~
    \details \ru Конструктор сплайна, описывающего волнистую линию.\n
             \en Constructor of a spline describing a wavy line. \n \~
    \param[in] p1      - \ru Начальная точка кривой.
                         \en Start point of curve. \~
    \param[in] p2      - \ru Конечная точка кривой.
                         \en End point of curve. \~
    \param[in] height  - \ru Высота гребешка волны.
                         \en Height of the wave. \~
    \param[in] periode - \ru Период волны.
                         \en Period of the wave. \~
  */
  static  MbNurbs * CreateWavyLine( const MbCartPoint & p1, const MbCartPoint & p2, double height, double periode );

public :
  VISITING_CLASS( MbNurbs ); 

  /** \ru \name Функции инициализации NURBS-кривой.
      \en \name Functions of NURBS curve initialization.
      \{ */
          // \ru Приведенные ниже функции меняют степень degree, поэтому в них необходим вызов CatchMemory(); \en The following functions change the degree "degree" therefore they call CatchMemory(); 

          /// \ru Установить параметры сплайна по заданной NURBS-кривой. \en Set the spline parameters by a given NURBS curve. 
          void    Init( const MbNurbs & );
          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Установить параметры сплайна.\n
                       \en Set parameters of spline.\n \~
              \param[in] degree  - \ru Порядок сплайна.
                                   \en A spline order. \~ 
              \param[in] points  - \ru Набор характерных точек.
                                   \en Set of control points. \~
              \param[in] cls     - \ru Признак замкнутости.
                                   \en A closedness attribute. \~
              \param[in] weights - \ru Набор весов для характерных точек.
                                   \en Set of weights for control points. \~ 
          */
          bool    Init( ptrdiff_t degree, const SArray<MbCartPoint> & points, bool cls,
                        const SArray<double> * weights = NULL );
          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Установить параметры сплайна.\n
                       \en Set parameters of spline.\n \~
              \param[in] degree  - \ru Порядок сплайна.
                                   \en A spline order. \~ 
              \param[in] cls     - \ru Признак замкнутости.
                                   \en A closedness attribute. \~
              \param[in] points  - \ru Набор характерных точек.
                                   \en Set of control points. \~ 
              \param[in] weights - \ru Набор весов для характерных точек.
                                   \en Set of weights for control points. \~
              \param[in] knots   - \ru Неубывающая последовательность весов.
                                   \en Non-decreasing sequence of weights. \~
              \param[in] form    - \ru Тип построения.
                                   \en Type of construction. \~
          */
          template <class PointsVector, class DoubleVector>
          bool 	  Init( ptrdiff_t initDegree, bool initClosed, const PointsVector & initPoints, 
                        const DoubleVector & initWeights, const DoubleVector & initKnots, 
                        MbeNurbsCurveForm initForm = ncf_Unspecified )
          {
            if ( ::IsValidNurbsParamsExt( initDegree, initClosed, initPoints, &initWeights, &initKnots ) ) {
              Refresh(); // Must come first, since frees allocated memory

              degree = initDegree;
              closed = initClosed;
              pointList = initPoints;
              form = initForm;
              weights = initWeights;
              knots = initKnots;

              uppIndex = (ptrdiff_t)pointList.size() - 1;
              uppKnotsIndex = (ptrdiff_t)knots.size() - 1;

              SetClamped(); // сделать узловой вектор точно зажатым
              return true;
            }
            return false;
          }

          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Установить параметры сплайна.\n
                       \en Set parameters of spline.\n \~
              \param[in] degree  - \ru Порядок сплайна.
                                   \en A spline order. \~ 
              \param[in] cls     - \ru Признак замкнутости.
                                   \en A closedness attribute. \~
              \param[in] points  - \ru Набор характерных точек.
                                   \en Set of control points. \~ 
              \param[in] knots   - \ru Неубывающая последовательность узлов.
                                   \en Non-decreasing sequence of knots. \~
              \param[in] nPoints - \ru Количество характерных точек.
                                   \en Count of control points. \~
              \param[in] nKnots  - \ru Количество узлов.
                                   \en Count of knots. \~
          */
          void    Init( ptrdiff_t degree, bool cls, const CcArray<double> & points,
                        const CcArray<double> & knots, ptrdiff_t nPoints, ptrdiff_t nKnots );
          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Сплайн, проходящий через заданные точки при заданных параметрах.\n
                В случае замкнутости нужно передавать массив с совпадением первой и последней точек.\n
                       \en Spline passing through given points at given parameters.\n
                In case of closedness it is necessary to pass the array with coincidence of the first and the last points.\n \~
              \param[in] degree  - \ru Порядок сплайна.
                                   \en A spline order. \~
              \param[in] cls     - \ru Признак замкнутости.
                                   \en A closedness attribute. \~
              \param[in] points  - \ru Набор точек, через которые проходит сплайн.
                                   \en Set of points which the spline passes through. \~
              \param[in] params  - \ru Последовательность узловых параметров.
                                   \en Sequence of knot parameters. \~
          */
          bool    InitThrough( ptrdiff_t degree, bool cls, const SArray<MbCartPoint> & points,
                               const SArray<double> & params );
          
          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Инициализировать прямолинейный сплайн.\n
                       \en Initialize a straight spline. \n \~
              \param[in] t1  - \ru Начальный узел.
                               \en The initial knot. \~
              \param[in] p1  - \ru Начальная точка, через которую проходит сплайн.
                               \en The initial point the spline passes through. \~
              \param[in] t2  - \ru Конечный узел.
                               \en The final knot. \~
              \param[in] p2  - \ru Конечная точка, через которую проходит сплайн.
                               \en The final point the spline passes through. \~
          */
          bool    InitLine( double t1, const MbCartPoint & p1, double t2, const MbCartPoint & p2 );
          /** \brief \ru Инициализация.
                     \en Initialization. \~
              \details \ru Инициализировать кубическую кривую как сплайн.\n
                       \en Initialize a cubic curve as a spline. \n \~
              \param[in] p1  - \ru Начальная точка, через которую проходит сплайн.
                               \en The initial point the spline passes through. \~
              \param[in] v1  - \ru Касательный вектор к кривой в начальной точке.
                               \en A tangent vector to the curve at the start point. \~
              \param[in] p2  - \ru Конечная точка, через которую проходит сплайн.
                               \en The final point the spline passes through. \~
              \param[in] v2  - \ru Касательный вектор к кривой в конечной точке.
                               \en A tangent vector to the curve at the end point. \~
          */
          bool    InitCube( const MbCartPoint & p1, const MbVector & v1, const MbCartPoint & p2, const MbVector & v2 );

          // \ru Интерполяция. \en Interpolation. 

          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн второго порядка по точкам, параметрам и признаку замкнутости.
                       \en Create a planar spline of second-order by points, parameters and attribute of closedness. \~
          */
  static  MbNurbs * CreateNURBS2( const SArray<MbCartPoint> & points, const SArray<double> & params, bool cls );

          /// \ru Создать кубический NURBS по точкам, через которые он проходит, и параметрам сопряжения. \en Create cubic NURBS by parameters of conjugation and points which it passes through. 
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> &, MbeSplineParamType spType,
                                  const MbPntMatingData<MbVector> & begData, 
                                  const MbPntMatingData<MbVector> & endData );
          /// \ru Создать кубический NURBS по интерполяционным точкам и данным сопряжения в каждой точке. \en Create cubic NURBS by interpolation points and data of conjugation at each point. 
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> &, MbeSplineParamType spType,
                                  bool closed,
                                  RPArray< MbPntMatingData<MbVector> > & );
          /// \ru Создать кубический NURBS по интерполяционным точкам, их параметрам и данным сопряжения в каждой точке. \en Create cubic NURBS by interpolation points, parameters and data of conjugation at each point. 
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> &, const SArray<double> &,
                                  bool closed,
                                  RPArray< MbPntMatingData<MbVector> > & );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по точкам, признаку замкнутости и типу параметризации.\n
                           Сплайн проходит через точки. Используется граничное условие отсутствия узла.
                       \en Create a planar spline of fourth order by points, attribute of closedness and parametrization type.\n
                           NURBS passes through points. Used boundary condition of knot absence. \~
          */
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> &, bool cls, MbeSplineParamType spType,
                                  MbeSplineCreateType useInitThrough = sct_Version2 );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по точкам, параметрам и признаку замкнутости.\n
                           Используется граничное условие отсутствия узла.
                       \en Create a planar spline of fourth order by points, parameters and attribute of closedness.\n
                           Used boundary condition of knot absence. \~
          */
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> & points, const SArray<double> & params, bool cls,
                                  MbeSplineCreateType useInitThrough = sct_Version2 );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по весам, точкам, параметрам и признаку замкнутости.\n
                           Используется граничное условие отсутствия узла.
                       \en Create a planar spline of fourth order by weights, points, parameters and attribute of closedness.\n
                           Used boundary condition of knot absence. \~
          */
  static  MbNurbs * CreateNURBS4( const SArray<double> & weights, const SArray<MbCartPoint> & points,
                                  SArray<double> & params, bool cls );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по точкам, параметрам и признаку замкнутости
                с граничными условиями - заданными векторами первых или вторых производных.\n
                Имеет 2 кратных внутренних узла, принадлежит классу дифференцируемых ( но не дважды дифференцируемых ) функций.
                       \en Create a planar spline of fourth order by points, parameters and attribute of closedness
                with boundary conditions - given vectors of first or second derivatives.\n
                Has 2 multiple internal knots, belongs to class of differentiable (but not twice differentiable) functions. \~
                \param[in] bfstS  - \ru Если true, то начальное граничное условие - вектор первой производной, иначе - вектор второй производной.
                                    \en If true, then start boundary condition is the vector of the first derivative, otherwise - the vector of the second derivative. \~
                \param[in] bfstN  - \ru Если true, то конечное граничное условие - вектор первой производной, иначе - вектор второй производной.
                                    \en If true, then end boundary condition is the vector of first derivative, otherwise - vector of second derivative. \~
          */
  static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> & points, const SArray<double> & params, 
                                  const MbVector &, const MbVector &, bool cls,
                                  bool bfstS = true, bool bfstN = true );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по точкам, производным, параметрам и признаку замкнутости.\n
                           Имеет 2 кратных внутренних узла, принадлежит классу дифференцируемых ( но не дважды дифференцируемых ) функций.
                       \en Create a planar spline of fourth order by points, derivatives, parameters and attribute of closedness.\n
                           It has 2 multiple internal knots, belongs to the class of differentiable (but not twice differentiable) functions. \~
          */
 static  MbNurbs * CreateNURBS4( const SArray<MbCartPoint> & points, const SArray<MbVector> & vectors,
                                 const SArray<double> & params, bool cls );
          /** \brief \ru Интерполяция.
                     \en Interpolation. \~
              \details \ru Создать плоский сплайн четвертого порядка по составному сплайну Безье четвертого порядка.\n
                Внимание! Парамеризация отлична от параметризации исходной кривой Безье.
                       \en Create a planar spline of fourth order by composite Bezier spline of fourth order.\n
                If closedness is necessary - call UnClamped( bezier.IsClosed() ). \~
          */
  static  MbNurbs * CreateNURBS4( const MbBezier & );

          /// \ru Установить сопряжение на конце. \en Set conjugation at the end. 
          bool    AttachG( MbPntMatingData<MbVector> & connectData, bool beg );

          /// \ru Увеличить порядок кривой, не изменяя ее геометрическую форму и параметризацию. \en Increase order of curve without changing its geometric shape and parametrization. 
          bool    RaiseDegree    ( ptrdiff_t, double relEps = Math::paramEpsilon );
          /// \ru Уменьшить порядок кривой на 1, не изменяя ее геометрическую форму и параметризацию. \en Decrease order of curve by 1 without changing its geometric shape and parametrization. 
          bool    ReductionDegree( double relEps = Math::paramEpsilon );
          /// \ru Задать порядок сплайна. \en Set the spline order. 
          void 		SetDegree( ptrdiff_t newDegree );
          /// \ru Увеличить порядок  на 1. \en Increase the order by 1. 
          void    DegreeIncrease();
          /// \ru Установить тип формы. \en Set the type of shape. 
          void 		SetFormType( MbeNurbsCurveForm f ) { form = f; }
          /// \ru Точка на кратном узле. \en The point on a multiple knot. 
          bool    PointOnMultipleKnot( const MbCartPoint & point ) const;

  /** \} */
  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of geometric object.
      \{ */
  virtual MbePlaneType  IsA() const; // \ru Тип элемента. \en A type of element. 
  virtual MbPlaneItem & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Сделать копию элемента \en Create a copy of the element 
  virtual	bool    IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const; // \ru Является ли кривая other копией данной кривой? \en Whether the curve is duplicate of current curve. 
  virtual bool    SetEqual( const MbPlaneItem & );       // \ru Сделать элементы равными. \en Make elements equal.   
  virtual void    Transform( const MbMatrix & matr, MbRegTransform * ireg = NULL, const MbSurface * newSurface = NULL );  // \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
  virtual void    Move( const MbVector & to, MbRegTransform * = NULL, const MbSurface * newSurface = NULL ); // \ru Сдвиг. \en Translation. 
  virtual void    Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = NULL, const MbSurface * newSurface = NULL ); // \ru Поворот. \en Rotation. 

  virtual void    GetProperties( MbProperties & );  // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties & );  // \ru Записать свойства объекта. \en Set properties of the object. 
  virtual void    GetBasisPoints( MbControlData & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  
  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions describing the domain of a curve.
      \{ */
  virtual double  GetTMin()      const; // \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  virtual double  GetTMax()      const; // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  virtual bool    IsClosed()     const; // \ru Замкнутость кривой. \en A curve closedness. 
  virtual bool    IsDegenerate( double eps = Math::LengthEps ) const; // \ru Проверка вырожденности кривой. \en Check curve degeneracy. 
  virtual bool    IsPeriodic()   const; // \ru Периодичность замкнутой кривой. \en Periodicity of a closed curve. 

  /** \} */
  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the curve's domain. 
            Functions PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is out of domain bounds.
      \{ */
  virtual void    PointOn  ( double & t, MbCartPoint & pnt ) const; // \ru Точка на кривой. \en Point on the curve. 
  virtual void    FirstDer ( double & t, MbVector    & fd ) const; // \ru Первая производная. \en First derivative. 
  virtual void    SecondDer( double & t, MbVector    & sd ) const; // \ru Вторая производная. \en Second derivative. 
  virtual void    ThirdDer ( double & t, MbVector    & td ) const; // \ru Третья производная. \en Third derivative. 
  /** \} */
  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  virtual void    Explore( double & t, bool ext,
                           MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const;
  // \ru Вычислить значения производных для заданного параметра. \en Calculate derivatives of object for given parameter. \~
          void    Derivatives( double & t, bool ext, MbVector & fir, MbVector * sec, MbVector * thi ) const;
          
          // \ru Функции, продолжающие кривую не по касательной как _PointOn() и др., а по кривой. \en Functions which do not continue curve along the tangent as _PointOn (), etc., and along a curve. 
          /// \ru Точка на продолжении кривой. \en Point on the curve extension. 
          void    ExtPointOn  ( double t, MbCartPoint & pnt ) const;
          /// \ru Первая производная на продолжении кривой. \en The first derivative on the curve extension. 
          void    ExtFirstDer ( double t, MbVector    & fd ) const;
          /// \ru Вторая производная на продолжении кривой. \en The second derivative on the curve extension. 
          void    ExtSecondDer( double t, MbVector    & sd ) const;
          /// \ru Третья производная на продолжении кривой. \en The third derivative on the curve extension. 
          void    ExtThirdDer ( double t, MbVector    & td ) const;

  /** \} */
  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  // \ru Добавить габарит в прямоугольник. \en Add a bounding box to rectangle.
  virtual void    CalculateGabarit ( MbRect & ) const;
  virtual bool    IsStraight()   const; // \ru Прямолинейность кривой. \en Straightness of curve. 
  // \ru Посчитать метрическую длину \en Calculate the metric length 
  virtual double  CalculateMetricLength() const; 
  // \ru Сдвинуть параметр t на расстояние len. \en Move parameter t on the metric distance len. 
  virtual bool    DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                                 VERSION version = Math::DefaultMathVersion() ) const;
  // \ru Вычислить метрическую длину кривой.\en Calculate the metric length of a curve.
  virtual double  CalculateLength( double t1, double t2 ) const; 

  virtual size_t  GetCount() const;

  virtual MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const;
  virtual MbCurve *   NurbsCurve( const MbNurbsParameters & tParameters ) const; // \ru Построить NURBS копию кривой. \en Construct a NURBS copy of a curve. 
  virtual	MbContour * NurbsContour() const;

  virtual MbCurve *   Trimmed( double t1, double t2, int sense ) const;

  virtual void    Refresh(); // \ru Сбросить все временные данные. \en Reset all temporary data. 
  virtual void    Inverse( MbRegTransform * iReg = NULL ); // \ru Изменение направления кривой на противоположное. \en Set the opposite direction of curve. 

  // \ru Определить, является ли кривая репараметризованно такой же. \en Define whether a reparameterized curve is the same. 
  virtual bool    IsReparamSame( const MbCurve & curve, double & factor ) const;

  virtual double  Step         ( double t, double sag ) const; // \ru Вычисление шага аппроксимации с учетом радиуса кривизны. \en Calculation of the approximation step with consideration of the curvature radius. 
  virtual double  DeviationStep( double t, double angle ) const; // \ru Вычисление шага аппроксимации по угловой толерантности. \en Calculation of the approximation step by angular tolerance. 

  virtual void    CalculatePolygon( double sag, MbPolygon & ) const;

  virtual MbeState DeletePart( double t1, double t2, MbCurve *& part2 ); // \ru Удалить часть поликривой между параметрами t1 и t2. \en Remove a part of the polyline between t1 and t2 parameters. 
  virtual MbeState TrimmPart ( double t1, double t2, MbCurve *& part2 ); // \ru Оставить часть поликривой между параметрами t1 и t2. \en Save a part of the polyline between t1 and t2 parameters. 

  virtual MbeState Deformation( const MbRect &, const MbMatrix & );  // \ru Деформация. \en Deformation. 
  virtual bool    GoThroughPoint( MbCartPoint &p0 ); // \ru Прохождение сплайна через точку. \en Passing of spline through the point. 
  virtual void    TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const; // \ru Вычисление всех касательных к кривой из данной точки. \en Calculation of all tangents to the curve from a given point. 
  virtual void    OffsetCuspPoint( SArray<double> & tCusps, double dist ) const; // \ru Определение особых точек офсетной кривой. \en Determination of singular points of the offset curve. 

  virtual bool    GetCentre( MbCartPoint & ) const; // \ru Выдать центр кривой. \en Give the curve center. 
  virtual double  GetRadius() const; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  virtual bool    GetAxisPoint( MbCartPoint & p ) const; // \ru Выдать центр оси кривой. \en Give the curve axis center. 
  virtual bool    IsSimilarToCurve( const MbCurve & curve ) const; // \ru Подобные ли кривые для объединения (слива). \en Whether the curves to union (joining) are similar. 

          /// \ru Касание сплайна прямой. \en Touching the spline by line. 
          void    MakeTangentLine( MbLine * line );
          /// \ru Определить выпуклую оболочку сегмента кривой. \en Determine the convex hull of the curve segment. 
          void    ConvexHull( ptrdiff_t seg, MbCartPoint * p ) const; 

  /** \} */
  /** \ru \name Общие функции полигональной кривой.
      \en \name Common functions of polygonal curve.
      \{ */

  virtual void    GetRuleInterval( ptrdiff_t index, double & t1, double & t2 ) const; // \ru Выдать интервал влияния точки B-сплайна. \en Get range of influence of B-spline point. 

  virtual void    Rebuild(); // \ru Перестроить B-сплайн. \en Rebuild B-spline. 
  virtual void    SetClosed( bool cls ); // \ru Установить признак замкнутости. \en Set the closedness attribute. 

  virtual void    RemovePoint( ptrdiff_t index ); // \ru Удалить точку. \en Remove the point. 
  virtual void    RemovePoints(); // \ru Удалить все точки. \en Delete all points. 

  virtual void    AddPoint( const MbCartPoint & pnt ); // \ru Добавить точку в конец массива. \en Add point to the end of the array. 
  virtual void    InsertPoint( ptrdiff_t index, const MbCartPoint & pnt ); // \ru Вставить точку по индексу. \en Insert a point by index. 
  virtual void    InsertPoint( double t,  const MbCartPoint & pnt, double xEps, double yEps ); // \ru Вставить точку. \en Insert a point. 
  virtual void    ChangePoint( ptrdiff_t index, const MbCartPoint & pnt ); // \ru Заменить вершину pointList[index] на pnt. \en Replace the vertex pointList[index] with pnt. 
  virtual bool    ChangePointsValue( const SArray<MbCartPoint> & pntList ); // \ru Заменить точки новыми при сохранении остальных параметров. \en Replace points with new one and save the other parameters. 
  virtual bool    CheckParam ( double & t, ptrdiff_t & i0, ptrdiff_t & i1, double & t0, double & t1 ) const; // \ru Загнать параметр, получить локальный индексы и параметры. \en Move parameter, get local indices and parameters. 
  virtual double  GetParam( ptrdiff_t i ) const;
  virtual size_t  GetParamsCount() const; 

  /** \} */
  /** \ru \name Функции B-сплайна.
      \en \name Functions of B-spline.
      \{ */

          /// \ru Добавить точку с весом. \en Add a point with weight. 
          void    AddPoint( ptrdiff_t index, const MbCartPoint & pnt, double weight );
          /// \ru Добавить точку в конец массива. \en Add point to the end of the array. 
          void    AddPoint( const MbCartPoint & pnt, double weight );
          /// \ru Сделать контур из NURBS-кривой. \en Create a contour from the NURBS curve. 
          MbContour * CreateContour() const;
          /// \ru Выделить часть. \en Break a part. 
          MbNurbs *   Break( double t1, double t2 ) const;

          /// \ru Получить форму В-сплайна. \en Get form of B-spline. 
          MbeNurbsCurveForm  GetFormType() const { return form; } 

          /// \ru Выдать порядок сплайна. \en Get the spline order.       
          ptrdiff_t GetDegree()   const { return degree; }
          /// \ru Вернуть признак рациональности, но не регулярности кривой. \en Get attribute of rationality, but no regularity of curve. 
          bool    IsRational() const;

          size_t  GetWeightsCount() const { return weights.Count(); }
          void    GetWeights( SArray<double> & wts ) const { wts = weights; }
          double  GetWeight( size_t ind ) const { return weights[ind]; }
          double &  SetWeight( size_t ind )       { return weights[ind]; }

          size_t  GetKnotsCount()     const { return knots.Count(); }
          void    GetKnots( SArray<double> & knts ) const { knts = knots; }
          double  GetKnot  ( size_t ind ) const { return knots[ind]; }
          double &  SetKnot  ( size_t ind )       { return knots[ind]; }
          ptrdiff_t GetUppKnotsIndex()  const { return uppKnotsIndex; }

          // \ru BEG: для библиотеки (хорошо бы избавиться) \en BEG: for the library (it would be good to get rid of this) 
          /// \ru Добавить точку в конец массива. \en Add point to the end of the array. 
          void    LtAddPoint ( MbCartPoint & pnt, double weight );
          /// \ru Добавить хар точку в степенном представлении в конец массива. \en Add a control point with degree representation to the end of the array. 
          void    LtAddPowerPoint( MbCartPoint & pnt );
          /// \ru Добавить узел в конец узлового вектора. \en Add a knot to the end of knot vector. 
          void    LtAddKnot  ( double knot );
          /// \ru Задать порядок сплайна. \en Set the spline order. 
          void    LtSetDegree( ptrdiff_t newDegree );
          /// \ru Установить признак замкнутости. \en Set the closedness attribute. 
          void    LtSetClosed( bool cls );
          /// \ru Изменить степень, замкнутость и тип формы. \en Change degree, closedness and type of shape. 
          void 		LtSetData( ptrdiff_t d, bool c, MbeNurbsCurveForm f );
          /// \ru Перестроить сплайн после накачки из библиотеки. \en Rebuild the spline. 
          bool    LtRebuild();
          /// \ru Инициализация. \en Initialization. 
          void 	  LtInit();
          // \ru Преобразование кусочно степенной формы в NURBS-кривую. \en Convert a piecewise exponential form to a NURBS-curve. 
          void    LtInitPowerArc(); 
          bool    LtTrimmed( double t1, double t2, int sense = 1 );
          // \ru END: для библиотеки (хорошо бы избавиться) \en END: for the library (it would be good escape) 

          /// \ru Создать Bezier форму Nurbs. \en Create a Bezier shape of Nurbs. 
          void    Bezier( MbNurbs & bezierForm ) const;
          /// \ru Присоединить nurbs. \en Attach nurbs. 
          bool    Concatenate( MbNurbs & nurbs );

          /// \ru Задать вес для вершины. \en Set weight for control point. 
          void    SetWeight( ptrdiff_t pointNumber, double newWeight );

          /// \ru Получить кратность узла. \en Get the knot multiplicity. 
          ptrdiff_t KnotMultiplicity( ptrdiff_t knotIndex ) const;
          /// \ru Определение базисного узлового вектора. \en Determination of basis knot vector. 
          void    DefineKnotsVector();
          /// \ru Переопределение базисного узлового вектора из Close в Open. \en Redetermination of the basis knot vector from Close to Open. 
          bool    OpenKnotsVector();
          /// \ru Переопределение базисного узлового вектора из Open в Close. \en Redetermination of the basis knot vector from Open to Close. 
          bool    CloseKnotsVector();
          /// \ru Сдвинуть параметр замкнутого сплайна. \en Shift parameter of closed spline. 
          void    CyclicShift( ptrdiff_t interval );
          void    CyclicShift( double t );
          bool    BasicFunctions( double & t, ptrdiff_t k, CcArray<double> & values, ptrdiff_t & left, double & sum );                    

          void    CheckForm();
          /// \ru Преобразовать кривую в коническое сечение, если это возможно. \en Transform a curve into a conic section if it is possible. 
          MbCurve * ConvertToConic();
          /// \ru Установить область изменения параметра. \en Set the range of parameter. 
          void    SetLimitParam( double newTMin, double newTMax );

  // \ru Базовые операции над NURBS-кривой. \en Base operation with NURBS curve. 

          /// \ru Добавление нового узла; возвращает количество узлов, которые удалось вставить. \en Addition of a new knots; returns the number of knots which have been inserted. 
          ptrdiff_t InsertKnots( double & newKnot, ptrdiff_t multiplicity, double relEps );
          /// \ru Удалить кратный внутренний узел id, num раз; вернуть количество удалений, которое удалось сделать. \en Remove multiple internal 'id' knot 'num' times, return count of removals was successfully made. 
          ptrdiff_t RemoveKnot( ptrdiff_t id, ptrdiff_t num, double relEps = Math::paramEpsilon, double absEps = Math::lengthEpsilon );
          /// \ru Удалить все внутренние узлы, если это возможно. \en Remove all internal knots if it is possible. 
          void    RemoveAllKnots( double relEps = Math::paramEpsilon, double absEps = Math::lengthEpsilon );
          /// \ru Преобразовать данный nurbs в форму Безье, узловой вектор в зажатый. \en Convert this nurbs to Bezier form; knot vector to clamped. 
          bool    DecomposeCurve();

          /// \ru Преобразовать узловой вектор в зажатый (если кривая замкнута и clm = false) или разжатый (если кривая не замкнута и clm = true). \en Transform knot vector to clamped (if curve is closed and clm = false) or unclamped (if curve is open and clm = true). 
          bool    UnClamped( bool clm );
          /// \ru Добавить кривую в конец. \en Add curve to the end. 
          void    AddCurve ( MbNurbs &, bool bmerge = true );
          /// \ru Добавить кривые в конец. \en Add curves to the end. 
          void    AddCurves( RPArray<MbNurbs> & );

          /** \brief \ru Разбить кривую.
                     \en Split the curve. \~
              \details \ru Разбить недифференцируемую NURBS-кривую четвертой степени в трижды кратном внутреннем узле.\n
                Если внутренних трижды кратных узлов не существует, то в массив заносится копия кривой.\n
                Если bline = true, то проверить вырожденность в прямую, если прямая - преобразовать в прямую.
                       \en Split the non-differentiable NURBS-curve of fourth degree at internal knot with multiplicity of three.\n
                If there are no internal knots with multiplicity of three, then the array is filled with copy of curve.\n
                If bline = true, then check the curve for degeneration into a line, if it is a line then transform to a line. \~
          */
          bool    BreakC0NURBS4( RPArray<MbCurve> &, bool bline = true );
          /// \ru Разбить NURBS-кривую в местах, где кривая не дифференцируема. Параметризация не сохраняется. \en Split NURBS-curve at places where the curve is non-differentiable. Parametrization does not remain. 
          bool    BreakC0( RPArray<MbCurve> & );
          /// \ru Расширить незамкнутую NURBS-кривую по касательным. \en Extend open NURBS-curve by tangents. 
          bool    ExtendNurbs( double, double, bool bmerge = false );

  virtual void    ResetTCalc() const; // \ru Сбросить текущее значение параметра \en Reset the current value of the parameter 
  // \ru Расстояние до точки. \en The distance to a point. 
  virtual double  DistanceToPoint( const MbCartPoint & to ) const;
  // \ru Расстояние до точки, если оно меньше d. \en Distance to the point if it is less than d.
  virtual bool    DistanceToPointIfLess( const MbCartPoint & to, double & d ) const;
  // \ru Найти проекцию точки на кривую. \en Find the point projection to the curve. 
  virtual double  PointProjection( const MbCartPoint & pnt ) const;
  // \ru Найти проекцию точки на кривую. \en Find the point projection to the curve.
  virtual bool    NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                       double & t, bool ext, MbRect1D * tRange = NULL ) const;

  virtual void    GetStartPoint( MbCartPoint & ) const;
  virtual void    GetEndPoint  ( MbCartPoint & ) const;
          /** \brief \ru Замкнуть кривую.
                     \en Make curve closed. \~
              \details \ru Замкнуть фактически замкнутую кривую.\n
                То есть если первая и последняя точки кривой совпадают, но она реализована как незамкнутая,
                то одна из совпадающих точек убирается и кривая делается замкнутой.
                       \en Make actually closed curve closed.\n
                That is, if the first and the last points of curve are coincident, but curve implemented as open,
                then one of coincident points is taken away and curve becomes closed. \~
          */
          void    FixClosedNurbs();

  /** \} */

protected:
  virtual bool    CanChangeClosed() const; // \ru Можно ли поменять признак замкнутости. // ЯТ К6 \en Whether it is possible to change the attribute of closedness. // ЯТ К6 

private: // \ru Системные методы. \en System methods. 
          bool    CatchMemory( MbNurbsAuxiliaryData * cache  ) const; // \ru Выделить память. \en Allocate memory. 
          void    FreeMemory( MbNurbsAuxiliaryData * cache  )  const; // \ru Освободить память. \en Free memory. 
          void    VerifyParam( double & t ) const; // \ru Загнать параметр t в параметрическую область кривой. \en Parameter set in the curve region. 
          void    CalculateSegment( double & t, MbNurbsAuxiliaryData * cache  ) const; // \ru Рассчитать базисные функции и разностные формы на участке. \en Calculate the basis functions and differential forms on the region. 
          void    CalculateSpline( ptrdiff_t n, MbNurbsAuxiliaryData * cache  ) const;   // \ru Рассчитать точку NURBS-кривой или производную n-го порядка. \en Calculate point of NURBS-curve or n-th order derivative. 
          void    CalculateSplineWeight( double & t, ptrdiff_t n, MbNurbsAuxiliaryData * cache  ) const;
          bool    InitSegments( MbNurbsAuxiliaryData * cache  ) const;
          void    SetClamped();

          void    ResetCache(); // \ru Очистить кэш главного потока, сбросить остальные кэши. \en Clear main thread cache, reset other caches. 
          bool    NurbsPlus( MbNurbs & nurbs, double tin, double tax ) const;

          // \ru Расчет весовых функций и их первых производных. \en Calculation of weight functions and its first derivatives. 
          ptrdiff_t WeightFunctions( double & x, CcArray<double> & ) const;
          // \ru Вычисление шага аппроксимации в обе стороны. \en Calculation of approximation step in both directions. 
          //double  StepD( double & t, double sag, bool checkAngle = false, double angle = 0.0 ) const;
          double  StepD( double & t, double sag, bool checkAngle = false, double angle = 0.0, MbNurbsAuxiliaryData *cache = NULL ) const;
          // \ru Вычисление шага аппроксимации сплайна второго порядка. \en Calculation of approximation step of second order spline. 
          //double  PolylineStep( double t, bool half ) const;
          double  PolylineStep( double t, bool half, MbNurbsAuxiliaryData *cache  ) const;
          // \ru Уточнить проекцию \en Specify projection.
          double  SpecifyProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, double t, bool ext ) const;

          void    operator = ( const MbNurbs & ); // \ru Не реализовано. \en Not implemented. 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbNurbs )
};


//------------------------------------------------------------------------------
// \ru Добавить точку в конец массива. \en Add point to the end of the array. 
// ---
inline void MbNurbs::AddPoint( const MbCartPoint & pnt, double weight )
{
  pointList.Add( pnt );
  weights.Add( weight );
  form = ncf_Unspecified;
  Rebuild();
}


//------------------------------------------------------------------------------
// \ru Для библиотеки. \en For library. 
// \ru Добавить точку в конец массива. \en Add point to the end of the array. 
// ---
inline void MbNurbs::LtAddPoint( MbCartPoint & pnt, double weight )
{
  pointList.Add( pnt );
  weights.Add( weight );
}


//-------------------------------------------------------------------------------
// \ru Для библиотеки. \en For library. 
// \ru Добавить характерную точку в степенном представлении в конец массива. \en Add a control point with degree representation to the end of the array. 
// ---
inline void MbNurbs::LtAddPowerPoint( MbCartPoint &pnt ) {
  pointList.Add( pnt );
}


//------------------------------------------------------------------------------
// \ru Для библиотеки. \en For library. 
// \ru Добавить узел в конец узлового вектора. \en Add a knot to the end of knot vector. 
// ---
inline void MbNurbs::LtAddKnot( double knot ) {
  knots.Add( knot );
}


//------------------------------------------------------------------------------
// \ru Для библиотеки. \en For library. 
// \ru Задать порядок сплайна. \en Set the spline order. 
// ---
inline void MbNurbs::LtSetDegree( ptrdiff_t newDegree )
{
  if ( newDegree >= 2 )
    if ( degree != newDegree && form == ncf_Unspecified )
      degree = newDegree;
}


//------------------------------------------------------------------------------
// \ru Для библиотеки. \en For library. 
// \ru Установить признак замкнутости. \en Set the closedness attribute. 
// ---
inline void MbNurbs::LtSetClosed( bool cls ) {
  if ( closed != cls && form == ncf_Unspecified )
    closed = cls;
}


//------------------------------------------------------------------------------
// \ru Функция вычисления пространственного шага, используется в 3D nurbs и MbSplineSurface. \en Calculation function of the spatial step, it is used in 3D nurbs and MbSplineSurface. 
// ---
int SurStep( bool checkAngle,               // \ru Флаг оценки по угловому отклонению. \en Flag of evaluation by angular deviation. 
             double & t,                    // \ru Текущий параметр. \en The current parameter. 
             double & step,                 // \ru Текущий шаг. \en The current step. 
             const SArray<double> & knots,  // \ru Узловой вектор. \en Knot vector. 
             ptrdiff_t & left,// \ru Индекс узла первой ненулевой базисной функции. \en The knot index of the first non-zero basis function. 
             ptrdiff_t k,     // \ru Минимальный  используемый индекс узлового вектора. \en Minimal used index of knot vector. 
             ptrdiff_t idMax, // \ru Максимальный используемый индекс узлового вектора. \en Maximal used index of knot vector. 
             bool bplus,    // \ru Флаг движения в сторону возрастания параметра. \en Flag of the movement in the direction of increasing parameter. 
             double & al,   // \ru Угловая оценка. \en Angular evaluation. 
             double & qd,   // \ru Квадратный корень оценки смещения. \en Square root of shift evaluation. 
             double df,     // \ru Длина первой производной (если bfirst - true). \en Length of the first derivative (if bfirst - true). 
             double ds,     // \ru Длина второй производной. \en Length of the second derivative. 
             double qds,    // \ru Квадратный корень длины второй производной. \en Square root of second derivative length. 
             double dt,     // \ru Длина третьей производной. \en Length of the third derivative. 
             double epsAL,  // \ru Угловая погрешность. \en Angular tolerance. 
             double epsQD );// \ru Метрическая погрешность. \en The metric tolerance. 


//------------------------------------------------------------------------------
// \ru Функция вычисления углового шага, используется в 3D nurbs и MbSplineSurface. \en Calculation function of the angular step, it is used in 3D nurbs and MbSplineSurface. 
// ---
int SurAngularStep( double & t, double & step, const SArray<double> & knots, 
                    ptrdiff_t & left, ptrdiff_t k, ptrdiff_t idMax,
                    bool bplus, double & alf, double & alt, double df, double ds, double dt,
                    double epsAF, double epsAT );


//------------------------------------------------------------------------------
// Проверить корректность узлов замкнутой Nurbs - кривой
// Используется для кривых, импортированных из формата Parasolid
// ---
bool AreClosedNurbsKnotsCorrect( ptrdiff_t degree, ptrdiff_t uppIndex, const SArray<double> & knots );


//------------------------------------------------------------------------------
// Оценка области влияния точки с сопряжениями
// ---
bool GetMatingRuleInterval( const MbNurbs & nurbs, const SArray<MbCartPoint> & pointsOnNurbs, size_t pointInd, double & t1, double & t2 );


//------------------------------------------------------------------------------
// \ru Является ли сплайн прямолинейным \en Whether the spline is straight 
// ---
template <class Nurbs, class Point, class Vector>
bool IsStraightNurbs( const Nurbs & nurbs, double mEps = METRIC_EPSILON )
{
  bool isStraight = false;

  if ( !nurbs.IsClosed() ) {
    SArray<double> wts( 0, 1 );
    nurbs.GetWeights( wts );
    size_t wtsCnt = wts.size();

    isStraight = true;
    if ( wtsCnt > 1 ) {
      double wt0 = wts[0];
      for ( size_t k = 1; k < wtsCnt; k++ ) {
        double wt = wts[k];
        if ( ::fabs(wt0 - wt) > EXTENT_EQUAL ) {
          isStraight = false;
          break;
        }
      }
    }
    if ( isStraight ) {
      isStraight = false;
      SArray<Point> pnts( 0, 1 );
      nurbs.GetPointList( pnts );
      if ( ::ArePointsOnLine<Point, Vector>( pnts, mEps ) )
        isStraight = true;
    }
  }

  return isStraight;
}


#endif // __CUR_NURBS_H
