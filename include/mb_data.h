////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Данные.
         \en Data. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MB_DATA_H
#define __MB_DATA_H


#include <mb_enum.h>
#include <io_define.h>
#include <mb_cart_point3d.h>
#include <mb_cart_point.h>


//------------------------------------------------------------------------------
/** \brief \ru Данные для вычисления шага.
           \en Data for step calculation. \~
  \details \ru Данные для вычисления шага при триангуляции поверхностей и граней. \n
           \en Data for step calculation during face triangulation. \n \~
  \ingroup Data_Structures
*/
// ---
class MATH_CLASS MbStepData {

private:
  /** \brief \ru Способ вычисления приращения параметра при движении по объекту. 
             \en The method of calculation of parameter increment by the object. \~
      \details \ru Способ вычисления приращения параметра при движении по кривой или поверхности.
        Для визуализации геометрической формы используется способ ist_SpaceStep. \n
        Для операций построения используется способ ist_DeviationStep. \n
        Для 3D принтеров используется способ ist_MetricStep и могут быть добавлены первые два. \n
        Для привязки объектов к параметрам поверхности используется способ ist_ParamStep,
        Для определения столкновений элементов модели используется способ ist_CollisionStep,
        Для вычисления инерционных характеристик используется способ ist_MipStep. 
               \en Methods of calculation of parameter increment by the object. \n \~
        Step by sag ist_SpaceStep is used for visualizations.
        Step by deviation angle ist_DeviationStep is used for calculation.
        Step by length ist_MetricStep is used for 3D printer (plus by sag and by deviation angle). \n
        Special step ist_ParamStep is used for linking with surface parameters.
        Special step ist_CollisionStep is used for collision detection of model elements.
        Special step ist_MipStep is used for calculation of inertial characteristics. \~
  */
  uint8  stepType;
  double sag;      ///< \ru Максимально допустимый прогиб кривой или поверхности в соседних точках на расстоянии шага. \en The maximum permissible sag of the curve or surface at adjacent points away step. \~
  double angle;    ///< \ru Максимально допустимое угловое отклонение касательных кривой или нормалей поверхности в соседних точках на расстоянии шага. \en The maximum angular deviation of the curve or surface normal in the neighboring points on the distance of a step. \~
  double length;   ///< \ru Максимально допустимое расстояние между соседними точками на расстоянии шага. \en The maximum distance between points a step away. \~
  size_t maxCount; ///< \ru Максимальное количество ячеек в строке и ряду триангуляционной сетки (если 0, то не задано). \en Maximum count of cell in row and column for triangulation grid (if 0, then unlimited). \~

public:

  /// \ru Конструктор с заданным типом шага. \en Constructor by step type. 
  MbStepData( MbeStepType t, double s );
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbStepData()
    : stepType( ist_SpaceStep    )
    , sag     ( Math::visualSag  )
    , angle   ( Math::deviateSag )
    , length  ( MAXIMON          )
    , maxCount( 0 )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbStepData( const MbStepData & other )
    : stepType( other.stepType )
    , sag     ( other.sag      )
    , angle   ( other.angle    )
    , length  ( other.length   )
    , maxCount( other.maxCount )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbStepData() {}

public:
  /// \ru Установить способ вычисления шага. \en Set the method of calculation of parameter increment by the object. \~
  void            SetStepType( MbeStepType t, bool add = true ) { if ( add ) { stepType |= t; } else { stepType = (uint8)t; } }
  /// \ru Установить максимально допустимый прогиб на расстоянии шага. \en Set the maximum permissible sag at adjacent points away step. \~
  void            SetSag     (  double s ) { sag = s; }
  /// \ru Установить максимально допустимое угловое отклонение в соседних точках. \en Set the maximum angular deviation in the neighboring points on the distance of a step. \~
  void            SetAngle   (  double a ) { angle = a; }
  /// \ru Установить максимально допустимое расстояние между соседними точками на расстоянии шага. \en Set the maximum distance between points a step away. \~
  void            SetLength  ( double l ) { length = l; }
  /// \ru Установить максимально допустимое количество ячеек в строке или ряду триангуляционной сетки. \en Set the maximum count of cell in row and column for triangulation grid. \~
  void            SetMaxCount( size_t c ) { maxCount = c; }
  
  /// \ru Дать максимально допустимый прогиб на расстоянии шага. \en Get the maximum permissible sag at adjacent points away step. \~
  double          GetSag     () const { return sag; }
  /// \ru Дать максимально допустимое угловое отклонение в соседних точках. \en Get the maximum angular deviation in the neighboring points on the distance of a step. \~
  double          GetAngle   () const { return angle; }
  /// \ru Дать максимально допустимое расстояние между соседними точками на расстоянии шага. \en Get the maximum distance between points a step away. \~
  double          GetLength  () const { return length; }
  /// \ru Дать максимально допустимое количество ячеек в строке или ряду триангуляционной сетки. \en Get the maximum count of cell in row and column for triangulation grid. \~
  size_t          GetMaxCount() const { return maxCount; }

  /// \ru Указанный шаг задан. \en This step is set.
  bool            StepIs     ( MbeStepType sType ) const { return !!(stepType & sType); }

  /// \ru Задан шаг по максимальному прогибу. \en Step by maximum deflection defined. \~
  bool            SagIncluded() const { return !!(stepType & ist_ParamStep) || 
                                               !!(stepType & ist_SpaceStep) || 
                                               !!(stepType & ist_CollisionStep); }
  /// \ru Задан шаг по угловому отклонению. \en Step by angular deviation defined. \~
  bool            AngleIncluded() const { return !!(stepType & ist_DeviationStep) || 
                                                 !!(stepType & ist_MipStep); }
  /// \ru Задан шаг по максимальному расстоянию. \en Step by maximum distance defined. \~
  bool            LengthIncluded() const { return !!(stepType & ist_MetricStep); }

  /// \ru Установить данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void            Init( MbeStepType t, double s, double a, double l, size_t c = 0 )
  { 
    stepType  = (uint8)t;
    sag       = s;
    angle     = a;
    length    = l;
    maxCount  = c;
  }

  /// \ru Установить данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void            InitStepBySag( double s )
  { 
    stepType  = (uint8)ist_SpaceStep;
    sag       = ::fabs(s);
    angle     = Math::deviateSag;
    length    = MAXIMON;
    maxCount  = 0;
  }

  /// \ru Функция копирования данных. \en Copy function of data. 
  void            Init( const MbStepData & other )
  { 
    stepType = other.stepType;
    sag      = other.sag;
    angle    = other.angle;
    length   = other.length;
    maxCount = other.maxCount;
  }

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbStepData &    operator = ( const MbStepData & other )
  {
    stepType = other.stepType;
    sag      = other.sag;
    angle    = other.angle;
    length   = other.length;
    maxCount = other.maxCount;
    return *this;
  }

  /// \ru Сбросить данные для вычисления шага. \en Reset data for step calculation. 
  void            Reset()
  {
    stepType = (uint8)ist_SpaceStep;
    sag = Math::visualSag;
    angle = Math::deviateSag;
    length = MAXIMON;
    maxCount = 0;
  }

  /// \ru Функция сравнения. \en Equal function. 
  bool            IsEqual( const MbStepData & other, double epsilon ) const;
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool            IsEmpty( double epsilon ) const;

  KNOWN_OBJECTS_RW_REF_OPERATORS_EX( MbStepData, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса. \en For working with references and objects of the class. 
};


//------------------------------------------------------------------------------
/** \brief \ru Способ построения полигонального объекта.
           \en Way for polygonal object constructing. \~
  \details \ru Данные построения полигонального объекта и триангуляции поверхностей и граней. \n
           \en Way for polygonal object constructing or face triangulation. \n \~
  \ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbFormNote {

private:
  bool wire; ///< \ru Флаг построения полигонного объекта. \en Flag construction landfill facility. \~
  bool grid; ///< \ru Флаг построения полигонального объекта. \en Flag constructing polygonal object. \~
  bool seam; ///< \ru Флаг не игнорировать шовные ребра. \en Flag for not ignore the seam edges. \~

public:

  /// \ru Пустой конструктор. \en Empty constructor. 
  MbFormNote()
    : wire( false )
    , grid( true )
    , seam( true )
  {}
  /// \ru Конструктор с заданным типом шага. \en Constructor by step type. 
  MbFormNote( bool w, bool g, bool s = true )
    : wire( w )
    , grid( g )
    , seam( s )
  {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbFormNote( const MbFormNote & other )
    : wire( other.wire )
    , grid( other.grid )
    , seam( other.seam )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbFormNote() {}

public:
  /// \ru Установить флаг построения полигонного объекта. \en Set flag construction landfill facility. \~
  void            SetWire( bool w ) { wire = w; }
  /// \ru Установить флаг построения полигонального объекта. \en Set flag constructing polygonal object. \~
  void            SetGrid( bool g ) { grid = g; }
  /// \ru Установить флаг шовных ребер. \en Set flag for seam edges. \~
  void            SetSeam( bool s ) { seam = s; }

  /// \ru Дать флаг построения полигонного объекта. \en Get flag construction landfill facility. \~
  bool            Wire() const { return wire;}
  /// \ru Дать флаг построения полигонального объекта. \en Get flag constructing polygonal object. \~
  bool            Grid() const { return grid; }
  /// \ru Дать флаг шовных ребер. \en Get flag for seam edges. \~
  bool            Seam() const { return seam; }

  /// \ru Установить Данные для вычисления шага при триангуляции. \en Set data for step calculation during triangulation. 
  void            Init( bool w, bool g, bool s ) { 
    wire = w;
    grid = g;
    seam = s;
  }

  /// \ru Функция копирования данных. \en Copy function of data. 
  void            Init( const MbFormNote & other ) { 
    wire = other.wire;
    grid = other.grid;
    seam = other.seam;
  }

  /// \ru Оператор присваивания. \en Assignment operator. 
  MbFormNote &    operator = ( const MbFormNote & other ) {
    wire = other.wire;
    grid = other.grid;
    seam = other.seam;
    return *this;
  }

  /// \ru Функция сравнения. \en Equal function. 
  bool            IsEqual( const MbFormNote & other ) const;

};


//------------------------------------------------------------------------------
/** \brief \ru Данные для управления двумерными объектами.
\en The data for two-dimensional object control. \~
\details \ru Данные содержат контрольные точки двумерныx объектов. \n
\en The data consist of two-dimensional control points for object. \n \~
\ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbControlData {

private:
  SArray<MbCartPoint> total; ///< \ru Точки, перемещаемые вместе. \en Points conveyed along. \~
  SArray<MbCartPoint> share; ///< \ru Точки, перемещаемые по отдельности. \en Points transported separately. \~ 
  mutable size_t totalIndex; ///< \ru Индекс текущей точки total. \en The index of current point total. \~
  mutable size_t shareIndex; ///< \ru Индекс текущей точки share. \en The index of current point share. \~

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbControlData() : total( 0, 1 ), share(0, 1), totalIndex( 0 ), shareIndex(0) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbControlData( const MbControlData & other )
    : total( other.total )
    , share( other.share )
    , totalIndex( other.totalIndex )
    , shareIndex( other.shareIndex )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbControlData() {}

public:
  /// \ru Зарезервировать память. \en Size reserve. \~
  void            ReserveTotal( size_t c ) { total.Reserve( c ); }
  /// \ru Зарезервировать память. \en Size reserve. \~
  void            ReserveShare( size_t c ) { share.Reserve( c ); }
  /// \ru Добывить точку. \en Add a point conveyed along. \~
  void            AddTotal( const MbCartPoint & p ) { total.Add(p); }
  /// \ru Добывить точку. \en Add a point. \~
  void            AddShare( const MbCartPoint & p ) { share.Add(p); }
  /// \ru Выдать количество точек. \en Get points count conveyed along. \~
  size_t          TotalCount() const { return total.Count(); }
  /// \ru Выдать количество точек. \en Get points count. \~
  size_t          ShareCount() const { return share.Count(); }
  /// \ru Обнулить индексы. \en Reset index.
  void            ResetIndex() const { totalIndex = 0; shareIndex = 0; }
  /// \ru Выдать очередную точку. \en Get current point for totalIndex++.
  bool            GetTotal( MbCartPoint & p ) const;
  /// \ru Выдать очередную точку. \en Get current point for shareIndex++.
  bool            GetShare( MbCartPoint & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool            GetTotal( size_t i, MbCartPoint & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index.
  bool            GetShare( size_t i, MbCartPoint & p ) const;
  /// \ru Выдать общее точек. \en Get all points count. \~
  size_t          Count() const { return total.Count() + share.Count(); }
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool            GetPoint( size_t i, MbCartPoint & p ) const;
  /// \ru Установить точку по индексу. \en Set point by index conveyed along.
  bool            SetPoint( size_t i, MbCartPoint & p );
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint> & SetTotalPoints() { return total; }
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint> & SetSharePoints() { return share; }
  /// \ru Освободить память. \en Free memory.
  void            HardFlush() { total.HardFlush(); share.HardFlush(); totalIndex = 0; shareIndex = 0; }

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void            Transform( const MbMatrix & matrix );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void            Move( const MbVector & to );
  /// \ru Повернуть вокруг точки. \en Rotate around a point. 
  void            Rotate( const MbCartPoint & point, double angle );

  /// \ru Дать точку по индексу. \en Set point by index.
  MbCartPoint   & operator []( size_t i ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  MbControlData & operator = ( const MbControlData & other ) {
    total = other.total;
    share = other.share;
    totalIndex = other.totalIndex;
    shareIndex = other.shareIndex;
    return *this;
  }
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool            IsEmpty() const { return ( total.Count() == 0 && share.Count() == 0 ); }
}; // MbControlData


//------------------------------------------------------------------------------
/** \brief \ru Данные для управления трехмерными объектами.
\en The data for three-dimentional object control. \~
\details \ru Данные содержат контрольные точки трехмерныx объектов. \n
\en The data consist of three-dimentional control points for object. \n \~
\ingroup Data_Structures
*/
// ---
struct MATH_CLASS MbControlData3D {

private:
  SArray<MbCartPoint3D> total; ///< \ru Точки, перемещаемые вместе. \en Points conveyed along. \~
  SArray<MbCartPoint3D> share; ///< \ru Точки, перемещаемые по отдельности. \en Points transported separately. \~ 
  mutable size_t totalIndex;   ///< \ru Индекс текущей точки total. \en The index of current point total. \~
  mutable size_t shareIndex;   ///< \ru Индекс текущей точки share. \en The index of current point share. \~

public:
  /// \ru Пустой конструктор. \en Empty constructor. 
  MbControlData3D() : total( 0, 1 ), share(0, 1), totalIndex( 0 ), shareIndex(0) {}
  /// \ru Конструктор копирования. \en Copy-constructor. 
  MbControlData3D( const MbControlData3D & other )
    : total( other.total )
    , share( other.share )
    , totalIndex( other.totalIndex )
    , shareIndex( other.shareIndex )
  {}
  /// \ru Деструктор. \en Destructor. 
  ~MbControlData3D() {}

public:
  /// \ru Зарезервировать память. \en Size reserve. \~
  void            ReserveTotal( size_t c ) { total.Reserve( c ); }
  /// \ru Зарезервировать память. \en Size reserve. \~
  void            ReserveShare( size_t c ) { share.Reserve( c ); }
  /// \ru Добывить точку. \en Add a point conveyed along. \~
  void            AddTotal( const MbCartPoint3D & p ) { total.Add(p); }
  /// \ru Добывить точку. \en Add a point. \~
  void            AddShare( const MbCartPoint3D & p ) { share.Add(p); }
  /// \ru Выдать количество точек. \en Get points count conveyed along. \~
  size_t          TotalCount() const { return total.Count(); }
  /// \ru Выдать количество точек. \en Get points count. \~
  size_t          ShareCount() const { return share.Count(); }
  /// \ru Обнулить индексы. \en Reset index.
  void            ResetIndex() const { totalIndex = 0; shareIndex = 0; }
  /// \ru Выдать очередную точку. \en Get current point for totalIndex++.
  bool            GetTotal( MbCartPoint3D & p ) const;
  /// \ru Выдать очередную точку. \en Get current point for shareIndex++.
  bool            GetShare( MbCartPoint3D & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool            GetTotal( size_t i, MbCartPoint3D & p ) const;
  /// \ru Выдать точку по индексу. \en Get point by index.
  bool            GetShare( size_t i, MbCartPoint3D & p ) const;
  /// \ru Выдать общее точек. \en Get all points count. \~
  size_t          Count() const { return total.Count() + share.Count(); }
  /// \ru Выдать точку по индексу. \en Get point by index conveyed along.
  bool            GetPoint( size_t i, MbCartPoint3D & p ) const;
  /// \ru Установить точку по индексу. \en Set point by index conveyed along.
  bool            SetPoint( size_t i, MbCartPoint3D & p );
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint3D> & SetTotalPoints() { return total; }
  /// \ru Выдать все точки. \en Get points.
  SArray<MbCartPoint3D> & SetSharePoints() { return share; }
  /// \ru Освободить память. \en Free memory.
  void            HardFlush() { total.HardFlush(); share.HardFlush(); totalIndex = 0; shareIndex = 0; }

  /// \ru Преобразовать согласно матрице. \en Transform according to the matrix. 
  void            Transform( const MbMatrix3D & matrix );
  /// \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  void            Move( const MbVector3D & to );
  /// \ru Повернуть вокруг оси. \en Rotate around an axis. 
  void            Rotate( const MbAxis3D & axis, double angle );

  /// \ru Дать точку по индексу. \en Set point by index.
  MbCartPoint3D & operator []( size_t i ) const;
  /// \ru Оператор присваивания. \en Assignment operator. 
  MbControlData3D & operator = ( const MbControlData3D & other ) {
    total = other.total;
    share = other.share;
    totalIndex = other.totalIndex;
    shareIndex = other.shareIndex;
    return *this;
  }
  /// \ru Вырожденный ли объект? \en Is empty? 
  bool            IsEmpty() const { return ( total.Count() == 0 && share.Count() == 0 ); }
}; // MbControlData3D


#endif // __MB_DATA_H
