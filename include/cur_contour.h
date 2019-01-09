////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контур в двумерном пространстве.
         \en Contour in two-dimensional space. \~ 
 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CUR_CONTOUR_H
#define __CUR_CONTOUR_H


#include <curve.h>
#include <cur_line_segment.h>
#include <templ_s_list.h>
#include <alg_base.h>


class  MATH_CLASS MbArc;
class             SimpleNameArray;
class             MbCurveIntoNurbsInfo;


//------------------------------------------------------------------------------
/** \brief \ru Контур в двумерном пространстве.
           \en Contour in two-dimensional space. \~ 
  \details \ru Контур представляет собой составную кривую, в которой начало каждого последующего сегмента стыкуется с концом предыдущего.
    Контур является замкнутым, если конец последнего сегмента стыкуется с началом первого сегмента.\n
    Если сегменты составной кривой стыкуются не гладко, то составная кривая будет иметь изломы. 
    В общем случае в местах стыковки сегментов производные составной кривой терпят разрыв по длине и направлению. \n
      Начальное значение параметра составной кривой равно нулю. 
    Параметрическая длина составной кривой равна сумме параметрических длин составляющих её сегментов. \n
    При вычислении радиуса-вектора составной кривой сначала определяется сегмент, 
    которому соответствует значение параметра составной кривой, и соответствующее значение собственного параметра этого сегмента. 
    Далее вычисляется радиус-вектор сегмента, который и будет радиусом-вектором составной кривой. \n
      В качестве сегментов составной кривой не используются другие составные кривые. 
    Если составную кривую нужно построить на основе других составных кривых, 
    то последние должны рассматриваться как совокупность составляющих их кривых, а не как единые кривые.\n 
    Двумерный контур используется для плоского моделирования, а также для описания двумерных связных областей, например, для описания области определения параметров поверхности.\n
           \en Contour is a composite curve in which the beginning of each subsequent segment is joined to the end of the previous one.
    Contour is closed if the end of last segment is joined to the beginning of the first segment.\n
    If the segments of a composite curve are not smoothly joined then the composite curve will have breaks. 
    In general case in places of joining segments derivatives of a composite curve have discontinuity along the length and direction. \n
      The initial value of the composite curve is equal to zero. 
    The parametric length of a composite curve is equal to the sum of the parametric lengths of components of its segments. \n
    When the calculation of the radius-vector of a composite curve segment is determined at first, 
    the value of composite curve parameter and the corresponding value of the own parameters of this segment corresponds to this segment. 
    Then computes the radius-vector of the segment which will be the radius-vector of the composite curve. \n
      Other composite curves are not used as segments of the composite curve. 
    If it is required to create a composite curve based on other composite curves, 
    then the latter must be regarded as a set of their curves, and not as single curves. \n 
    The two-dimensional contour is used for planar modeling and also for describing of two-dimensional areas, for example for determining of the domain of surface.\n \~
  \ingroup Curves_2D
*/
// ---
class MATH_CLASS MbContour : public MbCurve, public MbNestSyncItem {
protected :
  RPArray<MbCurve>        segments;     ///< \ru Множество сегментов контура. \en An array of contour segments. 
  bool                    closed;       ///< \ru Признак замкнутости кривой. \en An Attribute of curve closedness. 
  double                  paramLength;  ///< \ru Параметрическая длина контура. \en Parametric length of a contour. 
protected:
  mutable double          metricLength; ///< \ru Метрическая длина контура. \en Metric length of a contour. 
  mutable MbRect          rect;         ///< \ru Габаритный прямоугольник. \en Bounding box. 
  mutable c3d::DoublePair areaSign;     ///< \ru Площадь контура со знаком. \en Contour area with a sign. 

public :
  MbContour(); // \ru Пустой контур. \en Empty contour. 
  MbContour( const MbCurve & init, bool same );
  MbContour( const RPArray<MbCurve> & init, bool same );
  MbContour( const std::vector< SPtr<MbCurve> > & init, bool same );
protected :
  MbContour( const MbContour *, MbRegDuplicate * );
public :
  virtual ~MbContour();

public:
  VISITING_CLASS( MbContour ); 

  /** \ru \name Общие функции геометрического объекта.
      \en \name Common functions of a geometric object.
      \{ */

  virtual MbePlaneType  IsA() const; // \ru Тип элемента \en A type of element 
  virtual MbePlaneType  Type() const; // \ru Тип элемента \en A type of element 
  virtual MbPlaneItem & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Сделать копию элемента \en Create a copy of the element. 
  virtual bool        IsSimilar ( const MbPlaneItem & ) const;   // \ru Являются ли элементы подобными \en Whether the elements are similar. 
  virtual bool        SetEqual( const MbPlaneItem & );           // \ru Сделать элементы равными \en Make equal elements. 
  virtual	bool        IsSame( const MbPlaneItem & other, double accuracy = LENGTH_EPSILON ) const; // \ru Является ли кривая curve копией данной кривой ? \en Whether the curve "curve" is a copy of a given curve? 
  virtual void        Transform( const MbMatrix & matr, MbRegTransform * ireg = NULL, const MbSurface * newSurface = NULL ); // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix. 
  virtual void        Move( const MbVector & to, MbRegTransform * = NULL, const MbSurface * newSurface = NULL ); // \ru Сдвиг \en Translation. 
  virtual void        Rotate( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * = NULL, const MbSurface * newSurface = NULL ); // \ru Поворот \en Rotation. 

  virtual MbCurve *   Offset( double rad ) const; // \ru Смещение контура. \en Shift of a contour 
  virtual void        AddYourGabaritTo ( MbRect & ) const; // \ru Добавь свой габарит в прямой прям-к. \en Add bounding box into a straight box. 
  virtual void        CalculateGabarit ( MbRect & ) const; // \ru Определить габариты кривой. \en Determine the bounding box of the curve. 
  virtual void        CalculateLocalGabarit( const MbMatrix & into, MbRect & local ) const; // \ru Добавь в прям-к свой габарит с учетом матрицы \en Add bounding box into a box with consideration of the matrix. 
    const MbRect    & GetGabarit() const { if ( rect.IsEmpty() ) CalculateGabarit( rect ); return rect; }
    const MbRect    & GetCube() const { if ( rect.IsEmpty() ) CalculateGabarit( rect ); return rect; }
          void        SetDirtyGabarit() const { rect.SetEmpty(); }
          void        CopyGabarit( const MbContour & c ) { rect = c.rect; }
          bool        IsGabaritEmpty() const { return rect.IsEmpty(); }

  virtual double      DistanceToPoint( const MbCartPoint & ) const; // \ru Расстояние до точки \en Distance to a point. 

  virtual MbeState    Deformation( const MbRect &, const MbMatrix & );  // \ru Деформация \en Deformation. 
  virtual bool        IsInRectForDeform( const MbRect & r ) const;      // \ru Виден ли объект в заданном прямоугольнике для деформации \en Whether the object is visible in the specified rectangle for the deformation. 

  virtual void        Refresh(); // \ru Сбросить все временные данные \en Reset all temporary data. 
  virtual void        PrepareIntegralData( const bool forced ) const; // \ru Рассчитать временные (mutable) данные объекта. \en Calculate temporary (mutable) data of an object.
  // \ru Удалить часть контура между параметрами t1 и t2 \en Remove a part of the contour between t1 and t2 parameters. 
  virtual MbeState    DeletePart( double t1, double t2, MbCurve *& part2 );
  // \ru Оставить часть контура между параметрами t1 и t2 \en Save a part of the contour between t1 and t2 parameters. 
  virtual MbeState    TrimmPart( double t1, double t2, MbCurve *& part2 );
  virtual MbCurve *   Trimmed( double t1, double t2, int sense ) const;

  /** \} */
  /** \ru \name Функции описания области определения кривой.
      \en \name Functions for curve domain description.
      \{ */

  virtual double      GetTMin()    const; // \ru Вернуть минимальное значение параметра. \en Get the minimum value of parameter. 
  virtual double      GetTMax()    const; // \ru Вернуть максимальное значение параметра. \en Get the maximum value of parameter. 
  virtual bool        IsClosed()   const; // \ru Проверка замкнутости кривой. \en Check for curve closedness. 
  virtual bool        IsStraight() const; // \ru Признак прямолинейности кривой. \en An attribute of curve straightness. 
  virtual bool        IsSmoothConnected( double angleEps ) const; // \ru Являются ли стыки контура\кривой гладкими? \en Whether the joints of contour\curve are smooth. 
  /** \} */

  /** \ru \name Функции для работы в области определения кривой. 
            Функции PointOn, FirstDer, SecondDer, ThirdDer,... корректируют параметр
            при выходе его за пределы области определения.
      \en \name Functions for working in the domain of a curve. 
            Functions: PointOn, FirstDer, SecondDer, ThirdDer,... correct parameter
            when it is outside domain.
      \{ */
  virtual void        PointOn  ( double & t, MbCartPoint & p ) const;  // \ru Точка на кривой \en The point on the curve 
  virtual void        FirstDer ( double & t, MbVector    & v ) const;  // \ru Первая производная \en First derivative 
  virtual void        SecondDer( double & t, MbVector    & v ) const;  // \ru Вторая производная \en Second derivative 
  virtual void        ThirdDer ( double & t, MbVector    & v ) const;  // \ru Третья производная \en Third derivative 
  /** \} */

  /** \ru \name Функции для работы внутри и вне области определения кривой. 
            Функции _PointOn, _FirstDer, _SecondDer, _ThirdDer,... не корректируют параметр
            при выходе за пределы области определения. При выходе за область определения незамкнутая
            кривая, в общем случае, продолжается по касательной, которую она имеет на соответствующем конце.
      \en \name Function for working inside and outside of the curve domain. 
            Function _PointOn, _FirstDer, _SecondDer, _ThirdDer,... do not correct a parameter
            when it is outside domain. If non-closed curve is outside of the domain 
            in the general case it continues along a tangent, which it has at the respective end.
      \{ */
  virtual void       _PointOn  ( double   t, MbCartPoint & p ) const;  // \ru Точка на кривой. \en The point on the curve. 
  virtual void       _FirstDer ( double   t, MbVector    & v ) const;  // \ru Первая производная. \en Tirst derivative. 
  virtual void       _SecondDer( double   t, MbVector    & v ) const;  // \ru Вторая производная. \en Second derivative. 
  virtual void       _ThirdDer ( double   t, MbVector    & v ) const;  // \ru Третья производная. \en Third derivative. 
  /** \} */

  /** \ru \name Функции доступа к группе данных для работы внутри и вне области определения параметра кривой.
      \en \name Functions for get of the group of data inside and outside the curve's domain of parameter.
      \{ */
  // \ru Вычислить значения точки и производных для заданного параметра. \en Calculate point and derivatives of object for given parameter. \~
  virtual void        Explore( double & t, bool ext,
                               MbCartPoint & pnt, MbVector & fir, MbVector * sec, MbVector * thir ) const;
  /** \} */

  /** \ru \name Функции движения по кривой
      \en \name Functions of the motion along the curve
    \{ */
  virtual double      Step( double t, double sag ) const; // \ru Вычисление шага аппроксимации \en Calculate step of approximation. 
  virtual double      DeviationStep( double t, double angle ) const; // \ru Вычисление шага аппроксимации с учетом угла отклонения \en Calculation of step approximation with consideration of the deviation angle. 
  /** \} */

  /** \ru \name Общие функции кривой
      \en \name Common functions of curve
      \{ */
  virtual bool        HasLength( double & length ) const;
  virtual double      GetMetricLength() const;   // \ru Метрическая длина контура \en Metric length of a contour. 
  virtual double      GetLengthEvaluation() const; // \ru Оценка метрической длины кривой \en Evaluation of the metric length of the curve.

  virtual double      CalculateMetricLength() const; // \ru Посчитать метрическую длину \en Calculate the metric length 
          double      GetParamLength() const { return paramLength; }
          double      CalculateParamLength(); // \ru Посчитать параметрическую длину \en Calculate the parametric length 
          double      GetArea( double sag = Math::deviateSag ) const
          { 
            sag = ::fabs(sag);
            if ( ::fabs( areaSign.first - sag ) > EXTENT_EPSILON )
              return CalculateArea( sag );
            return areaSign.second;
          }

  virtual MbNurbs *   NurbsCurve( const MbCurveIntoNurbsInfo & ) const;
  virtual	MbContour * NurbsContour() const;

          void        SetClosed(); // \ru Установить признак замкнутости контура \en Set the closedness attribute of contour 
          void        CheckClosed( double eps ); // \ru Установить признак замкнутости контура \en Set the closedness attribute of contour 
          void        InitClosed( bool c ) { closed = c; } // \ru Установить признак замкнутости контура \en Set the closedness attribute of contour 

          /** \brief \ru Проверить замкнутость и непрерывность точек контура.
                     \en Check for closedness and continuity of contour points. \~
            \details \ru Проверить замкнутость и непрерывность точек контура.
              Проверяется совпадение первой и последней точки контура,
              совпадение последней точки каждого сегмента с первой точкой следующего сегмента.
              Равенство точек проверяется по умолчанию грубо - с точностью, равной 5 * PARAM_NEAR.
                     \en Check for closedness and continuity of contour points.
              Checking for coincidence of first and last points of the contour,
              coincidence of the last point of each segment with the first point of the next segment.
              Equality of points is checked roughly by default - with tolerance is equal to 5* PARAM_NEAR. \~
            \return \ru true, если контур замкнутый и непрерывный.
                    \en true, if contour is closed and continuous. \~
          */
          bool        IsClosedContinuousC0( double eps = 5.0 * PARAM_NEAR ) const;

          void        CloseByLineSeg( bool calcData ); // \ru Замкнуть контур отрезком \en Close the contour by segment 

  // \ru Посчитать метрическую длину разомкнутой кривой с заданной точностью \en Calculate the metric length of unclosed curve within the given tolerance 
  virtual double      CalculateLength( double t1, double t2 ) const;
  // \ru Сдвинуть параметр t на расстояние len \en Move parameter t on the distance len 
  virtual bool        DistanceAlong( double & t, double len, int curveDir, double eps = Math::LengthEps,
                                     VERSION version = Math::DefaultMathVersion() ) const;
          // \ru Cбросить переменные кэширования. \en Reset variables caching.
          void        Clear( bool calculateParamLength = true ) {
                        if ( calculateParamLength )
                          CalculateParamLength(); // \ru Параметрическая длина контура \en Parametric length of a contour 
                        metricLength = -1; // \ru Метрическая длина кривой \en Metric length of a curve 
                        rect.SetEmpty();
                        areaSign.first = -1.0;
                      }
          ptrdiff_t   FindSegment( double & t, double & tSeg ) const; // \ru Нахождение сегмента контура \en Finding of a contour segment 
          size_t      GetSegmentsCount() const { return segments.Count(); } // \ru Выдать количество сегментов контура \en Get the number of contour segments 
          // \ru Выдать сегмент контура по индексу \en Get contour segment by the index 
  const   MbCurve *   GetSegment( size_t ind ) const { return segments[ind]; }
          MbCurve *   SetSegment( size_t ind )       { return segments[ind]; }

          // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
          // \ru iloc_InItem = 1 - точка находится слева от контура, \en Iloc_InItem = 1 - point is located to the left of the contour,  
          // \ru iloc_OnItem = 0 - точка находится на контуре, \en Iloc_OnItem = 0 - point is located on the contour, 
          // \ru iloc_OutOfItem = -1 - точка находится справа от контура. \en Iloc_OutOfItem = -1 - point is located to the right of the contour. 
  virtual MbeItemLocation PointRelative( const MbCartPoint & pnt, double eps = Math::LengthEps ) const; 
          // \ru Положение точки относительно кривой. \en The point position relative to the curve. 
  virtual MbeLocation     PointLocation( const MbCartPoint & pnt, double eps = Math::LengthEps ) const;

  virtual double      PointProjection( const MbCartPoint & pnt ) const;  // \ru Проекция точки на кривую \en Point projection on the curve 
  virtual bool        NearPointProjection( const MbCartPoint & pnt, double xEpsilon, double yEpsilon, 
                                           double & t, bool ext, MbRect1D * tRange = NULL ) const; // \ru Проекция точки на кривую или её продолжение в области поиска проекции \en Point projection on the curve or its extension in the projection search area  
          double      DistanceToBorder( const MbCartPoint & pnt ) const; // \ru Параметрическое расстояние до ближайшей границы \en Parametric distance to the nearest boundary  

          // \ru Выделить часть контура \en Trim a part of the contour 
          void        Trimm( double t1, double t2 );

          // \ru Вычисление всех перпендикуляров к кривой из данной точки \en Calculation of all perpendiculars to the curve from a given point 
  virtual void        PerpendicularPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const;
          // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all tangents to the curve from a given point 

  virtual void        TangentPoint( const MbCartPoint & pnt, SArray<double> & tFind ) const;

  virtual void        IntersectHorizontal( double y, SArray<double> & ) const; // \ru Пересечение с горизонтальной прямой \en Intersection with the horizontal line 
  virtual void        IntersectVertical  ( double x, SArray<double> & ) const; // \ru Пересечение с вертикальной прямой \en Intersection with the vertical line 
  virtual void        SelfIntersect( SArray<MbCrossPoint> & ) const;           // \ru Самопересечение контура \en Self-intersection of the contour 
          bool        IsSelfIntersect( bool bStrict = true ) const;

          // \ru Функции находятся в файле equcntr.cpp \en Functions are in the file equcntr.cpp 
          // \ru Скругление двух соседних элементов с информацией об удалении \en Fillet of two neighboring elements with information about removal 
          bool        FilletTwoSegments( ptrdiff_t/*OV_x64 */ & index, double rad, bool & del1, bool & del2 );
          bool        FilletTwoSegments( ptrdiff_t & index, double rad );  // \ru Скругление двух соседних элементов \en Fillet of two neighboring elements 
          // \ru Вставка фаски между двумя соседними элементами с информацией об удалении \en Insertion of chamfer between two neighboring elements with information about removal 
          bool        ChamferTwoSegments( ptrdiff_t & index, double len, double angle,
                                          bool type, bool firstSeg, bool & del1, bool & del2 );
          // \ru Вставка фаски между двумя соседними элементами \en Insertion of chamfer between two neighboring elements 
          bool        ChamferTwoSegments( ptrdiff_t & index, double len, double angle,
                                          bool type, bool firstSeg = true );
          bool        Fillet( double rad ); // \ru Скругление контрура \en Fillet of contour 
          bool        Chamfer( double len, double angle, bool type ); // \ru Вставка фаски \en Insertion of the chamfer 
          MbeState    RemoveFilletOrChamfer( const MbCartPoint & pnt );  // \ru Удалить скругление или фаску контура \en Remove fillet or contour chamfer 
          // \ru Разбить контур на непересекающиеся сегменты \en Split contour into non-overlapping segments 
          bool        InsertCrossPoints();
          // \ru Разбиение сегментов контура в точках пересечения \en Splitting of contour segments at the points of intersection 
          void        BreakSegment( ptrdiff_t & index, ptrdiff_t firtsIdx,
                                    SArray<MbCrossPoint> & cross, bool firstCurve = true );

          bool        CheckConnection( double eps = Math::LengthEps ) const;  // \ru Проверка непрерывности контура \en Check for contour continuity 
          bool        CheckConnection( double xEps, double yEps ) const;  // \ru Проверка непрерывности контура \en Check for contour continuity 

          // \ru Скругление двух соседних элементов дугой нулевого радиуса \en Rounding two neighboring elements by arc of zero radius 
          void        FilletTwoSegmentsZero( ptrdiff_t & index, int defaultSense, bool fullInsert );
          // \ru Скругление контрура дугой нулевого радиуса \en Rounding contour by arc of zero radius 
          void        FilletZero( int defaultSense, bool fullInsert = false );
          // \ru Вставка фаски между двумя соседними элементами для построения эквидистанты \en Insertion of chamfer between two neighboring elements for construction of the offset 
          void        ChamferTwoSegmentsZero( ptrdiff_t & index, double rad );
          void        ChamferZero( double rad ); // \ru Вставка фаски для построения эквидистанты \en Insertion of chamfer for construction of the offset 
          // \ru Удаление вырожденных сегментов контура \en Removal of degenerate contour segments 
          void        DeleteDegenerateSegments( double radius, MbCurve * curve, bool mode );

          /** \brief \ru Построение эквидистанты к контуру.
                     \en Construction of offset to contour. \~
            \details \ru Построение эквидистанты к контуру справа и слева.
              Имя каждого эквидистантного контура совпадает с именем исходного.
                     \en Construction of offset to contour of the left and right.
              A name of every offset contour matches with the name of the initial one. \~
            \param[in]  radLeft  - \ru Радиус эквидистанты слева по направлению.
                                   \en The equidistance radius on the left by direction. \~
            \param[in]  radRight - \ru Радиус эквидистанты справа по направлению.
                                   \en The equidistance radius on the right by direction. \~
            \param[in]  side     - \ru Признак, с какой стороны строить:\n
                                   0 - слева по направлению,\n
                                   1 - справа по направлению,\n
                                   2 - с двух сторон.
                                   \en Attribute defining the side to construct:\n
                                   0 - on the left by direction,\n
                                   1 - on the right by direction,\n
                                   2 - on the both sides. \~
            \param[in]  mode     - \ru Cпособ обхода углов:\n
                                   true - дугой,
                                   false - срезом.
                                   \en The way of traverse of angles:\n
                                   true - by arc,
                                   false - by section. \~
            \param[out] equLeft  - \ru Массив контуров слева.
                                   \en The array of contours on the left side. \~
            \param[out] equRight - \ru Массив контуров справа.
                                   \en The array of contours on the right side. \~
          */
          void        Equid( double radLeft, double radRight, int side, bool mode,
                             PArray<MbCurve> & equLeft, PArray<MbCurve> & equRight );

          // \ru Построение новых контуров из эквидистанты \en Construction of new contours from equidistance 
          void        CreateNewContours( RPArray<MbCurve> & contours );

          // \ru Вычисление площади контура, если контур замкнут \en Calculation of contour area if contour is closed  
          double      CalculateArea( double sag = Math::deviateSag ) const;
          // \ru Определение направления обхода контура, если контур замкнут \en Determination of traverse direction if contour is closed 
          int         GetSense() const;
          // \ru Установить направление обхода контура \en Set the traverse direction of the contour 
          void        SetSense( int sense );

          // \ru Изменить направление обхода контура \en Change the traverse direction of the contour 
  virtual void        Inverse( MbRegTransform * iReg = NULL );
  virtual size_t      GetCount() const; // \ru Количество разбиений для прохода в операциях \en The number of partitions for passage in the operations  
          // \ru Выдать характерную точку ограниченной кривой если она ближе чем dmax \en Get characteristic point of bounded curve if it is closer than dmax 
  virtual bool        DistanceToPointIfLess( const MbCartPoint & toP, double & d ) const; // \ru Расстояние до точки, если оно меньше d \en Distance to the point if it is less than d 
  virtual bool        GetSpecificPoint( const MbCartPoint & from, double & dmax, MbCartPoint & pnt ) const;
          // \ru Выдать среднюю точку сегмента контура \en Get a mid-point of the contour segment 
          bool        GetSegmentMiddlePoint( const MbCartPoint & from, MbCartPoint & midPoint ) const;
          // \ru Выдать линейный сегмент контура \en Get the linear segment of contour  
          bool        GetLinearSegment( const MbCartPoint & from, double maxDist, MbCartPoint & p1, MbCartPoint & p2, double & d ) const;
          // \ru Выдать дуговой сегмент контура \en Get the arc segment of contour 
          MbArc *     GetArcSegment( const MbCartPoint & from, double maxDist, double & d ) const;
          // \ru Выдать длину сегмента контура \en Get the contour segment length 
          bool        GetSegmentLength( const MbCartPoint & from, double & length ) const;

  virtual bool        GetWeightCentre( MbCartPoint & ) const; // \ru Выдать центр тяжести контура \en Get gravity center of contour 
  virtual bool        GetCentre      ( MbCartPoint & ) const; // \ru Выдать центр кривой \en Get the center of curve 
          // \ru Найти ближайший к точке узел контура \en Find the nearest node of contour to point 
          ptrdiff_t   FindNearestNode( const MbCartPoint & to ) const;
          // \ru Найти ближайший к точке сегмент контура \en Find the nearest segment of contour to point 
          ptrdiff_t   FindNearestSegment( const MbCartPoint & to ) const;
          // \ru Определение особых точек офсетной кривой \en Determination of singular points of the offset curve 
  virtual void        OffsetCuspPoint( SArray<double> & tCusps, double dist ) const;
  virtual double      GetRadius() const; // \ru Дать физический радиус объекта или ноль, если это невозможно. \en Get the physical radius of the object or null if it impossible.
  virtual bool        GetAxisPoint( MbCartPoint & p ) const; // \ru Выдать центр оси кривой. \en Give the curve axis center. 
          void        CombineNurbsSegments(); // \ru Объединить NURBS кривые в контуре \en Unite NURBS curves into the contour 

  virtual void        GetProperties( MbProperties & properties );  // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void        SetProperties( const MbProperties & properties );  // \ru Записать свойства объекта \en Set properties of the object 
  virtual void        GetBasisPoints( MbControlData & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void        SetBasisPoints( const MbControlData & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \} */
  /** \ru \name Функции работы с сегментами контура
      \en \name Function for working with segments of contour
      \{ */

          bool        Init( List<MbCurve> & curves );

          void        Init( const MbContour & other );
          template <class PointsVector>
          bool        Init( const PointsVector & points ); // \ru Инициализация по массиву точек (замкнутый контур). \en Initialization by array of points (closed contour).
          bool        InitAsRectangle( const MbCartPoint * points ); // \ru Инициализация как прямоугольника ( приходит 4 точки ) \en Initialization as rectangle (4 points are given).
          bool        InitByRectangle( const MbRect & ); // \ru Инициализация по прямоугольнику габарита. \en Initialization by rectangle of bounding box.

          bool        AddSegment             ( MbCurve * ); // \ru Добавить сегмент в контур \en Add a segment to the contour 
          bool        AddSegmentOrDeleteCurve( MbCurve * ); // \ru Добавить кривую как сегмент или удалить ее \en Add a curve as segment or remove its 
          MbCurve *   AddSegment( const MbCurve * pBasis, double t1, double t2, int sense = 1 );
          bool        AddAtSegment   ( MbCurve * newSegment, size_t index ); // \ru Вставить сегмент перед сегментом контура с индексом index \en Insert a segment before the contour segment with the index "index" 
          bool        AddAfterSegment( MbCurve * newSegment, size_t index ); // \ru Вставить сегмент после сегмента контура с индексом index \en Insert a segment after the contour segment with the index "index" 
          // \ru Функция добавления новой кривой с управляемой проверкой \en Function for addition of a new curve with checking 
          bool        AddCurveWithRuledCheck( MbCurve & newCur, double absEps, bool toEndOnly = false, bool checkSame = true,
                                              VERSION version = Math::DefaultMathVersion() );

          void        DeleteSegments();          // \ru Удалить все сегменты в контуре \en Remove all segments from contour 
          void        DeleteSegment( size_t ind ); // \ru Удалить сегмент в контуре \en Remove a segment from contour 
          void        SetDegenerated(); // \ru Сделать контур пустым \en Make a contour empty 
          void        DetachSegments();          // \ru Отцепить все сегменты от контура без удаления \en Detach all segments from the contour without removing 
          MbCurve *   DetachSegment( size_t ind ); // \ru Отцепить сегмент от контура и вернуть его \en Detach a segment from contour return it 
          void        SetSegment( MbCurve & newSegment, size_t ind ); // \ru Заменить сегмент в контуре \en Replace a segment in the contour 
          void        SegmentsAdd( MbCurve & newSegment, bool calculateParamLength = true ); // \ru Добавить сегмент в контур без проверки \en Add a segment to the contour without checking 
          void        SegmentsInsert( size_t ind, MbCurve & newSegment ); // \ru Вставить сегмент в контур перед индексом без проверки \en Insert a segment into contour before an index without checking 
          void        SegmentsRemove( size_t ind ); // \ru Удалить сегмент без проверки \en Remove a segment without checking 
          void        SegmentsDetach( size_t ind ); // \ru Отцепить сегмент без проверки \en Detach a segment without checking 
          void        Calculate( bool calcArea = false ); // \ru Рассчитать параметры:  rect, paramLength, metricLength, closed \en Calculate parameters:  rect, paramLength, metricLength, closed 
          // \ru Управление распределением памяти в массиве segments \en Control of memory allocation in the array "segments" 
          void        SegmentsReserve( size_t additionalSpace ) { segments.Reserve( additionalSpace ); } // \ru Зарезервировать место под столько элементов \en Reserve memory for this number of elements 
          void        SegmentsAdjust ()                         { segments.Adjust();                   } // \ru Удалить лишнюю память \en Free the unnecessary memory 

          void        GetSegments( RPArray<MbCurve> & segms ) const { segms.AddArray( (RPArray<MbCurve> &)segments ); }

          void        SetMetricLength( double len ) const { metricLength = len; }
          /// \ru Установить начальную (конечную) точку для замкнутого контура \en Set the start (end) point for closed contour 
          bool        SetBegEndPoint( double t );
          /// \ru Заменить сегменты контуры и сегменты полилинии \en Replace segments of contour and segments of polyline 
          void        ReplaceContoursAndPolylines();
          void        GetPolygon( double sag, SArray<MbCartPoint> & poly, double eps ) const; // \ru Дать точки полигона \en Get points of polygon 

          bool        IsAnyCurvilinear() const; // \ru Есть ли в контуре криволинейный сегмент \en Whether the contour has a curved segment 
          bool        IsSameSegments( const MbContour & cntr ) const; // \ru Содержат ли контура идентичные сегменты \en Whether contours contains identical segments 
          bool        GetBegSegmentPoint( size_t i, MbCartPoint & p ) const; // \ru Дать начальную точку i-го сегмента \en Get the start point of i-th segment 
          bool        GetEndSegmentPoint( size_t i, MbCartPoint & p ) const; // \ru Дать конечную  точку i-го сегмента \en Get the end point of i-th segment 

          /** \brief \ru Нормаль по параметру, учитывая стыки сегментов.
                     \en Normal by parameter with consideration of segments joints \~
            \details \ru Нормаль по параметру, учитывая стыки сегментов.\n
                     \en Normal by parameter with consideration of segments joints \n \~
            \param[in] t - \ru Параметр на контуре
                           \en Parameter on the contour \~
            \param[out] norm - \ru Единичный вектор нормали, если не попали на стык сегментов\n
              если попали на стык сегментов - вектор, направленный, 
              как сумма двух нормалей на сегментах в точке стыка, 
              с длиной, равной 1, деленной на синус половинного угла между сегментами
                               \en Unit vector of normal if not hit on the joint of segments \n
              if we got on the joint of segments then the vector is directed, 
              as the sum of two normals on segments in joint, 
              with length which is equal to 1 divided by the sine of half angle between the segments \~
            \return \ru true, если попали на стык сегментов
                    \en true, if got on the joint of segments \~
          */
          bool        CornerNormal( double t, MbVector & norm ) const;

          /** \brief \ru Параметры стыков сегментов.
                     \en Parameters of segments joints. \~
            \details \ru Параметры стыков сегментов кроме минимального
              и максимального параметров контура.
                     \en Parameters of segments joints without minimal
              and maximal contour parameter. \~
            \param[out] params - \ru Набор параметров.
                                 \en Set of parameters. \~
          */
          void        GetCornerParams( SArray<double> & params ) const;
           // \ru Вычисление всех касательных к кривой из данной точки \en Calculation of all tangents to the curve from a given point 

          /** \brief \ru Вычисление двух касательных (для параметров стыков).
                     \en Calculation of two tangents (for parameters of joints). \~
            \details \ru Вычисление двух касательных для параметра стыка по соответствующим сегментам.
                         Если параметр не стыковой, то касательные равны.
                     \en Calculation of two tangents for parameter of segments joint corresponding to the segments.
                         If parameter is not one of parameters of segments joints tangents are equal.
              and maximal contour parameter. \~
            \param[in] t      - \ru Параметр.
                                \en A parameter. \~
            \param[out] tan1  - \ru Первая касательная.
                                \en First tangent. \~
            \param[out] tan2  - \ru Вторая касательная.
                                \en Second tangent. \~
          */
          bool        GetTwoTangents( double t, MbVector & tan1, MbVector & tan2 ) const;

  /** \} */
  /** \ru \name Функции работы с именами контура.
      \en \name Functions for working with names of contours.
      \{ */

          /** \brief \ru Дать имена сегментов.
                     \en Get names of segments. \~
            \details \ru Дать имена сегментов контура.\n
                     \en Get names of contour segments. \n \~
            \param[out] names - \ru Имена сегментов.
                                \en Names of segments \~
          */
          void        GetSegmentsNames(       SimpleNameArray & names ) const;

          /** \brief \ru Установить имена сегментов.
                     \en Set names of segments. \~
            \details \ru Установить имена сегментов контура по массиву имен.\n
                     \en Set names of contour segments by array of names. \n \~
            \param[in] names - \ru Набор имен.
                               \en A set of names. \~
          */
          void        SetSegmentsNames( const SimpleNameArray & names );
  /** \} */

private:
          ptrdiff_t   _FindSegment( double & t, double & tSeg ) const; // \ru Нахождение сегмента контура \en Finding of a contour segment 
          MbContour & operator = ( const MbContour & initContour );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbContour )
}; // MbContour


//------------------------------------------------------------------------------
// инициализация по массиву точек ( замкнутый контур )
// ---
template <class PointsVector>
bool MbContour::Init( const PointsVector & points )
{
  size_t count = points.size();

  if ( count > 1 ) {
    DeleteSegments();    
    segments.reserve( count );
    for ( size_t i = 0; i < count; i++ ) {
      MbLineSegment * seg = new MbLineSegment( points[i], points[ (i + 1) % count ] );
      segments.Add( seg );
      seg->AddRef();
    }
    closed = true;
    Clear();
    return true;
  }
  return false;
}


#endif // __CUR_CONTOUR_H
