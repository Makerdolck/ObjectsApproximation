////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель разрезанного тела.
         \en Cut solid constructor.
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_CUTTING_SOLID_H
#define __CR_CUTTING_SOLID_H


#include <creator.h>
#include <mb_placement3d.h>


class  MATH_CLASS MbSurface;
class  MATH_CLASS MbContour;
struct MATH_CLASS MbMergingFlags;


//------------------------------------------------------------------------------
/** \brief \ru Строитель разрезанного тела.
           \en Cut solid constructor. \~
  \details \ru Строитель тела, разрезанного поверхностью или набором граней, полученного выдавливанием плоского контура.\n
           \en Constructor of a solid cut by a surface or a set of faces obtained by extrusion of a planar contour.\n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbCuttingSolid : public MbCreator {
protected :
  MbSurface *   surface;    ///< \ru Режущая поверхность. \en Cutting surface. 
  MbContour *   contour;    ///< \ru Режущий контур (вместо поверхности). \en Cutting contour (instead of surface). 
  MbPlacement3D place;      ///< \ru Местная система координат контура. \en Local coordinate system of the contour. 
  MbVector3D    direction;  ///< \ru Направление и длина выдавливания контура. \en Direction and distance of the contour extrusion. 
  int           part;       ///< \ru Оставляемая часть (если part>0, то оставляем часть тела со стороны нормали поверхности). \en A part to be kept (if part>0, then keep a part of  solid from the side of surface normal). 
  bool          closed;     ///< \ru Замкнутоcть оболочки разрезаемого объекта. \en Closedness of the shell of the object being cut.
  bool          mergeFaces; ///< \ru Сливать подобные грани (true). \en Whether to merge similar faces (true). 
  bool          mergeEdges; ///< \ru Сливать подобные ребра (true). \en Whether to merge similar edges (true). 
  double        buildSag;   ///< \ru Угловое отклонение при движении по кривым и поверхностям. \en Angular deviation while moving along curves and surfaces. 

public :
  MbCuttingSolid( const MbSurface & surface, bool sameSurface, int part, 
                  bool closed, const MbMergingFlags & flags, const MbSNameMaker & n );
  MbCuttingSolid( const MbPlacement3D & place, const MbContour & contour, const MbVector3D & direction, int part, 
                  bool closed, const MbMergingFlags & flags, const MbSNameMaker & n );
private :
  MbCuttingSolid( const MbCuttingSolid &, MbRegDuplicate * ireg ); 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbCuttingSolid( const MbCuttingSolid & ); 
public :
  virtual ~MbCuttingSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  virtual MbeCreatorType  IsA() const; // \ru Тип элемента \en A type of element 
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * = NULL ); // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void        Move( const MbVector3D & to, MbRegTransform * = NULL );                 // \ru Сдвиг \en Translation 
  virtual void        Rotate( const MbAxis3D & axis, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate around an axis 
  virtual MbCreator & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Сделать копию \en Create a copy 

  virtual MbePrompt   GetPropertyName(); // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  virtual void        GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void        SetProperties( const MbProperties & properties ); // \ru Записать свойства объекта \en Set properties of the object 
  virtual void        GetBasisItems ( RPArray<MbSpaceItem> & s ); // \ru Дать базовые объекты \en Get the base objects 
  virtual void        GetBasisPoints( MbControlData3D & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void        SetBasisPoints( const MbControlData3D & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  virtual bool        IsSame( const MbCreator &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool        IsSimilar ( const MbCreator & ) const; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  virtual bool        SetEqual( const MbCreator & );       // \ru Сделать равным \en Make equal 

  // \ru Общие функции твердого тела \en Common functions of solid 

  virtual bool        CreateShell( MbFaceShell *& shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = NULL ); // \ru Построение \en Construction 

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void    operator = ( const MbCuttingSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbCuttingSolid )
}; // MbCuttingSolid


//------------------------------------------------------------------------------
/** \brief \ru Отрезать от оболочки некоторую её часть.
           \en Cut a part from a shell. \~
  \details \ru Для указанной оболочки построить оболочку без чати граней, отрезанных от неё указанной поверхностью. \n
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For a given shell create a shell without a part of faces cut from it by the given surface. \n
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in]  sameShell      - \ru Способ копирования граней исходной оболочки.
                               \en Method of copying the source shell faces. \~
  \param[in]  surface        - \ru Режущая поверхность.
                               \en Cutting plane. \~
  \param[in]  sameSurface    - \ru Использовать исходную поверхность (true) или её копию (false).
                               \en Use the source surface (true) or its copy (false). \~
  \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, -1).
                               \en A part of the source shell to be kept (+1, -1). \~
  \param[in]  closed         - \ru Построить замкнутую оболочку.
                               \en Create a closed shell. \~
  \param[in]  flags          - \ru Флаги слияния элементов оболочки.
                               \en Control flags of shell items merging. \~
  \param[in]  operNames      - \ru Именователь операции.
                               \en An object defining a name of the operation. \~
  \param[out] res            - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreatePart(       MbFaceShell *    solid, 
                                          MbeCopyMode      sameShell, 
                                    const MbSurface &      surface, 
                                          bool             sameSurface,
                                          int              part, 
                                          bool             closed, 
                                    const MbMergingFlags & flags,
                                    const MbSNameMaker &   operNames, 
                                          MbResultType &   res, 
                                          MbFaceShell *&   shell );


//------------------------------------------------------------------------------
/** \brief \ru Отрезать от оболочки некоторую её часть.
           \en Cut a part from a shell. \~
  \details \ru Для указанной оболочки построить оболочку без чати граней, отрезанных от неё набором граней, 
    полученных выдовливанием плоского контура. \n
    Одновременно с построением оболочки функция создаёт её строитель. \n
           \en For the given shell create a shell without a part of faces cut from it by a set of faces 
    obtained by extrusion of a planar contour. \n
    The function simultaneously constructs the shell and creates its constructor. \n \~
  \param[in]  solid          - \ru Исходная оболочка.
                               \en The source shell. \~
  \param[in]  sameShell      - \ru Способ копирования граней исходной оболочки.
                               \en Method of copying the source shell faces. \~
  \param[in]  place          - \ru Локальная система координат, в плоскости XY которай расположен двумерный контур.
                               \en A local coordinate system the two-dimensional contour is located in XY plane of. \~
  \param[in]  contour        - \ru Двумерный контур выдавливания расположен в плоскости XY локальной системы координат.
                               \en The two-dimensional contour of extrusion is located in XY plane of the local coordinate system. \~
  \param[in]  der            - \ru Направление выдавливания контура.
                               \en Extrusion direction of the contour. \~
  \param[in]  part           - \ru Сохраняемая часть исходной оболочки (+1, -1).
                               \en A part of the source shell to be kept (+1, -1). \~
  \param[in]  closed         - \ru Построить замкнутую оболочку.
                               \en Create a closed shell. \~
  \param[in]  flags          - \ru Флаги слияния элементов оболочки.
                               \en Control flags of shell items merging. \~
  \param[in]  operNames      - \ru Именователь операции.
                               \en An object defining a name of the operation. \~
  \param[out] res            - \ru Код результата операции.
                               \en Operation result code. \~
  \param[out] shell          - \ru Построенный набор граней.
                               \en Constructed set of faces. \~  
  \result \ru Возвращает строитель, если операция была выполнена успешно.
          \en Returns the constructor if the operation has been successfully performed. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreatePart(       MbFaceShell *    solid, 
                                          MbeCopyMode      sameShell, 
                                    const MbPlacement3D &  place, 
                                    const MbContour &      contour, 
                                    const MbVector3D &     der, 
                                          int              part, 
                                          bool             closed, 
                                    const MbMergingFlags & flags,
                                    const MbSNameMaker &   operNames, 
                                          MbResultType &   res, 
                                          MbFaceShell *&   shell );


#endif // __CR_CUTTING_SOLID_H
