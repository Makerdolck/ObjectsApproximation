////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Усекающие элементы оболочки.
         \en Truncating elements of a shell. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SPLIT_DATA_H
#define __CR_SPLIT_DATA_H


#include <templ_rp_array.h>
#include <templ_s_array.h>
#include <math_define.h>
#include <mb_placement3d.h>
#include <reference_item.h>


class                reader;
class                writer;
class   MATH_CLASS   MbProperties;
class   MATH_CLASS   MbCartPoint3D;
class   MATH_CLASS   MbAxis3D;
class   MATH_CLASS   MbSpaceItem;
class   MATH_CLASS   MbCurve;
class   MATH_CLASS   MbContour;
class   MATH_CLASS   MbCurve3D;
class   MATH_CLASS   MbSurfaceIntersectionCurve;
class   MATH_CLASS   MbSurface;
class   MATH_CLASS   MbFaceShell;
class   MATH_CLASS   MbSNameMaker;
class   MATH_CLASS   MbCreator;
class   MATH_CLASS   MbSolid;
struct  MATH_CLASS   MbControlData3D;
class                MbRegDuplicate;
class                MbRegTransform;
enum                 MbeSenseValue;
enum                 MbeCopyMode;


//------------------------------------------------------------------------------
/** \brief \ru Усекающие элементы.
           \en Truncating elements. \~
  \details \ru Усекающие элементы используются для разделения граней на части и усечения оболдочек. 
    Усечение может выполняться двумерными кривыми, расположенными в пласкости XY локальной системы координат,
    трёхмерными кривыми, поверхностями и оболочками. 
    Усекающие элементы используются в строителе усеченной оболочки MbTruncatedShell и 
    строителе оболочки с разбиением граней MbSplitShell. \n
           \en Truncating elements are used for splitting faces into parts and truncation of shells. 
    Truncating can be performed by two-dimensional curves located in the XY plane of the local coordinate system,
    by three-dimensional curves, surfaces and shells. 
    Truncating elements are used in the creator of truncated shell MbTruncatedShell and 
    in the creator of shell with face splitting MbSplitShell. \n \~
  \ingroup Build_Parameters
*/
// ---
class MATH_CLASS MbSplitData : public MbRefItem {
  /// \ru Типы усекающих объектов. \en Truncating objects types. 
  enum MbeSplitItemsType {
    sit_NoItems  = 0, ///< \ru Нет объектов. \en No objects. 
    sit_Curves2d = 1, ///< \ru Двумерные кривые в локальной системе координат. \en Two-dimensional curves in the local coordinate system.  
    sit_Curves3d = 2, ///< \ru Трехмерные кривые. \en Three-dimensional curves. 
    sit_Surfaces = 3, ///< \ru Поверхности. \en Surfaces. 
    sit_Creators = 4, ///< \ru Строители тела. \en Solid creators.
  };

private:
  RPArray<MbContour> curves2d;   ///< \ru Двумерные кривые. \en Two-dimensional curves. 
  MbPlacement3D      place;      ///< \ru Локальная система координат двумерных кривых. \en Local coordinate system of two-dimensional curves. 
  MbeSenseValue      sense;      ///< \ru Направление выдавливания двумерных кривых. \en Extrusion direction of two-dimensional curves. 
  RPArray<MbCurve3D> curves3d;   ///< \ru Пространственные кривые. \en Spatial curves. 
  RPArray<MbSurface> surfaces;   ///< \ru Поверхности. \en Surfaces. 
  RPArray<MbCreator> creators;   ///< \ru Строители оболочки. \en Shell creators. 
  MbFaceShell      * solidShell; ///< \ru Оболочка. \en A shell. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbSplitData();
  /// \ru Конструктор по двумерным кривым в локальной системе координат. \en Constructor by two-dimensional curves in the local coordinate system. 
  MbSplitData( const MbPlacement3D & pl, const MbeSenseValue zdir, const RPArray<MbContour> & items, bool same );
  /// \ru Конструктор по пространственным кривым. \en Constructor by spatial curves. 
  MbSplitData( const RPArray<MbCurve3D> & items, bool same );
  /// \ru Конструктор по поверхностям. \en Constructor by surfaces. 
  MbSplitData( const RPArray<MbSurface> & items, bool same );
  /// \ru Конструктор по телу. \en Constructor by a solid. 
  MbSplitData( const MbSolid & solid, bool same, bool keepShell );
  /// \ru Конструктор копирования с регистратором копирования. \en Copy constructor with registrator of copying. 
  MbSplitData( const MbSplitData &, bool same, MbRegDuplicate * iReg );
  /// \ru Деструктор. \en Destructor. 
 ~MbSplitData();

public:
        /// \ru Инициализировать по двумерным кривым в локальной системе координат. \en Initialize by two-dimensional curves in the local coordinate system. 
        bool          Init( const MbPlacement3D & pl, MbeSenseValue zdir, const RPArray<MbContour> & items, bool same );
        /// \ru Инициализировать по пространственным кривым. \en Initialize by spatial curves. 
        bool          Init( const RPArray<MbCurve3D> & items, bool same );
        /// \ru Инициализировать по поверхностям. \en Initialize by surfaces. 
        bool          Init( const RPArray<MbSurface> & items, bool same );
        /// \ru Инициализировать по телу. \en Initialize by a solid. 
        bool          Init( const MbSolid & solid, bool same, bool keepShell );
        /// \ru Сделать равным. \en Make equal. 
        bool          SetEqual ( const MbSplitData & );
        /// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
        bool          IsSimilar( const MbSplitData & ) const;
        /// \ru Преобразовать элемент согласно матрице. \en Transform element according to the matrix. 
        void          Transform( const MbMatrix3D &, MbRegTransform * = NULL );
        /// \ru Сдвинуть по вектору. \en Shift by  a vector. 
        void          Move     ( const MbVector3D &, MbRegTransform * = NULL );
        /// \ru Повернуть вокруг оси. \en Rotate about an axis. 
        void          Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = NULL );
        /// \ru Отсутствуют ли объекты? \en Are the objects absent? 
        bool          IsEmpty() const { 
                        return ( (curves2d.Count() < 1) && 
                                 (curves3d.Count() < 1) && 
                                 (surfaces.Count() < 1) && 
                                 (creators.Count() < 1 && (solidShell == NULL)) ); }
        /// \ru Являются ли объекты равными? \en Determine whether an object is equal? 
        bool          IsSame( const MbSplitData & other, double accuracy ) const; 

  /** \ru \name Доступ к эскизу.
      \en \name Access to a sketch.
      \{ */
        /// \ru Выдать количество двумерных кривых. \en Get number of two-dimensional curves. 
        size_t          GetSketchCurvesCount() const { return curves2d.size(); }
        /// \ru Получить локальную систему координат двумерных кривых. \en Get the local coordinate system of two-dimensional curves. 
  const MbPlacement3D & GetSketchPlace() const { return place; }
        /// \ru Получить локальную систему координат двумерных кривых. \en Get the local coordinate system of two-dimensional curves. 
        MbPlacement3D & SetSketchPlace()       { return place; }
        /// \ru Выдать направление выдавливания двумерных кривых. \en Get extrusion direction of two-dimensional curves. 
        MbeSenseValue   GetSketchSense() const { return sense; }
        /// \ru Выдать направление выдавливания двумерных кривых. \en Get extrusion direction of two-dimensional curves. 
        MbeSenseValue & SetSketchSense()       { return sense; }
        /// \ru Установить направление выдавливания двумерных кривых. \en Set extrusion direction of two-dimensional curves. 
        void            SetSketchSense( MbeSenseValue zdir ) { sense = zdir; }
        /// \ru Получить двумерную кривую по индексу. \en Get two-dimensional curve by index. 
  const MbContour   *   GetSketchCurve( size_t k ) const { return ((k < curves2d.size()) ? curves2d[k] : NULL ); }
        /// \ru Получить двумерную кривую по индексу. \en Get two-dimensional curve by index. 
        MbContour   *   SetSketchCurve( size_t k )       { return ((k < curves2d.size()) ? curves2d[k] : NULL ); }
        /// \ru Получить все двумерные кривые. \en Get all two-dimensional curves. 
        void            GetSketchCurves( RPArray<MbContour> & curvs ) const { curvs.AddArray( curves2d ); }
        /// \ru Удалить двумерную кривую по индексу. \en Delete two-dimensional curve by index. 
        bool            DeleteSketchCurve( size_t k );

  /** \} */
  /** \ru \name Доступ к пространственным кривым.
      \en \name Access to spatial curves.
      \{ */
        /// \ru Выдать количество пространственных кривых. \en Get number of spatial curves. 
        size_t        GetSpaceCurvesCount() const { return curves3d.size(); }
        /// \ru Получить пространственную кривую по индексу. \en Get a spatial curve by index. 
  const MbCurve3D   * GetSpaceCurve( size_t k ) const { return ((k < curves3d.size()) ? curves3d[k] : NULL ); }
        /// \ru Получить пространственную кривую по индексу. \en Get a spatial curve by index. 
        MbCurve3D   * SetSpaceCurve( size_t k )       { return ((k < curves3d.size()) ? curves3d[k] : NULL ); }
        /// \ru Получить все пространственные кривые. \en Get all spatial curves. 
        void          GetSpaceCurves( RPArray<MbCurve3D> & curvs ) const { curvs.AddArray( curves3d ); }
        /// \ru Установить пространственную кривую по индексу. \en Set spatial curve by index. 
        bool          SetSpaceCurve( const MbCurve3D & curve, size_t k );

  /** \} */
  /** \ru \name Доступ к поверхностям.
      \en \name Access to surfaces.
      \{ */
        /// \ru Выдать количество поверхностей. \en Get number of surfaces. 
        size_t        GetSurfacesCount() const { return surfaces.size(); }
        /// \ru Получить поверхность по индексу. \en Get a surface by index. 
  const MbSurface   * GetSurface( size_t k ) const { return ((k < surfaces.size()) ? surfaces[k] : NULL ); }
        /// \ru Получить поверхность по индексу. \en Get a surface by index. 
        MbSurface   * SetSurface( size_t k )       { return ((k < surfaces.size()) ? surfaces[k] : NULL ); }
        /// \ru Получить все поверхности. \en Get all surfaces. 
        void          GetSurfaces( RPArray<MbSurface> & surfs ) const { surfs.AddArray( surfaces ); }
        /// \ru Установить поверхность по индексу. \en Set a surface by index. 
        bool          SetSurface( const MbSurface & surface, size_t k );

  /** \} */
  /** \ru \name Доступ к строителям.
      \en \name Access to creators.
      \{ */
        /// \ru Выдать количество строителей тела. \en Get number of solid creators. 
        size_t        GetCreatorsCount() const { return creators.size(); }
        /// \ru Получить строитель по индексу. \en Get constructor by index. 
  const MbCreator   * GetCreator( size_t k ) const { return ((k < creators.size()) ? creators[k] : NULL ); }
        /// \ru Получить строитель по индексу. \en Get constructor by index. 
        MbCreator   * SetCreator( size_t k )       { return ((k < creators.size()) ? creators[k] : NULL ); }
        /// \ru Получить все строители. \en Get all creators. 
        void          GetCreators( RPArray<MbCreator> & crs ) const { crs.AddArray( creators ); }
        /// \ru Получить хранимую оболочку. \en Get stored shell. 
  const MbFaceShell * GetSolidShell() const { return solidShell; }
        /// \ru Создать оболочку по строителям (solidShell остается нетронутой). \en Create a shell by creators (solidShell remains unchanged). 
        MbFaceShell * CreateShell( MbeCopyMode copyMode );
        /// \ru Создать оболочку по строителям. \en Create a shell by creators (solidShell remains unchanged). 
        bool          UpdateShell( MbeCopyMode copyMode );
        /// \ru Удалить данные. \en Delete data. 
        void          DeleteItems();
        /// \ru Прочитать данные. \en Read data. 
        void          ReadItems ( reader & );
        /// \ru Записать данные. \en Write data. 
        void          WriteItems( writer & ) const;
        /// \ru Выдать свойства объекта. \en Get properties of the object. 
        void          GetProperties( MbProperties & properties );
        /// \ru Записать свойства объекта. \en Set properties of the object. 
        void          SetProperties( const MbProperties & properties );
        /// \ru Дать базовые объекты. \en Get the base objects. 
        void          GetBasisItems ( RPArray<MbSpaceItem> & );
        /// \ru Выдать контрольные точки объекта. \en Get control points of object.
        void          GetBasisPoints( MbControlData3D & ) const;
        /// \ru Изменить объект по контрольным точкам. \en Change the object by control points.
        void          SetBasisPoints( const MbControlData3D & );
  /** \} */
OBVIOUS_PRIVATE_COPY( MbSplitData )
};


#endif // __CR_SPLIT_DATA_H
