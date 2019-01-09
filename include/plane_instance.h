////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Вставка двумерного объекта.
         \en Instance of a two-dimensional object. \~ 

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PLANE_INSTANCE_H
#define __PLANE_INSTANCE_H


#include <model_item.h>
#include <mb_placement3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Вставка двумерных объектов эскиза.
           \en Instance of a two-dimensional sketch objects. \~
  \details \ru Вставка позволяет работать с двумерными геометрическими объектами как с объектом геометрической модели.
    Двумерные геометрические объекты MbPlaneItem располагаются в плоскости XOY локальной системы координат MbPlacement3D.\n
           \en The instance allows to deal with two-dimensional geometric objects as with object of geometric model.
    Two-dimensional MbPlaneItem geometric objects located in XOY-plane of MbPlacement3D local coordinate system.\n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbPlaneInstance : public MbItem {
protected :
  MbPlacement3D              place;      ///< \ru Локальная система координат объекта. \en Local coordinate system of the object. 
  std::vector<MbPlaneItem *> planeItems; ///< \ru Множество двумерных объектов эскиза. \en A set of two-dimensional sketch objects. 

protected :
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPlaneInstance( const MbPlaneInstance &, MbRegDuplicate * );
public :
  /// \ru Конструктор по двумерному объекту и локальной системе координат. \en Constructor by two-dimensional object and a local coordinate system. 
  MbPlaneInstance( MbPlaneItem &, const MbPlacement3D & );
  /// \ru Деструктор \en Destructor 
  virtual ~MbPlaneInstance();

public :
  VISITING_CLASS( MbPlaneInstance ); 

  // \ru Общие функции геометрического объекта. \en Common functions of a geometric object.  

  virtual MbeSpaceType  IsA()  const; // \ru Тип объекта. \en A type of an object. 
  virtual MbSpaceItem & Duplicate( MbRegDuplicate * iReg = NULL ) const; // \ru Создать копию. \en Create a copy. 
  virtual void    Transform( const MbMatrix3D &, MbRegTransform * iReg = NULL ); // \ru Преобразовать согласно матрице. \en Transform according to the matrix.  
  virtual void    Move     ( const MbVector3D &, MbRegTransform * iReg = NULL ); // \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  virtual void    Rotate   ( const MbAxis3D &, double angle, MbRegTransform * iReg = NULL ); // \ru Повернуть вокруг оси. \en Rotate about an axis. 
  virtual	bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const; // \ru Являются ли объекты равными? \en Are the objects equal? 
  virtual bool    IsSimilar( const MbSpaceItem & init ) const; // \ru Являются ли объекты подобными? \en Are the objects similar? 
  virtual bool    SetEqual ( const MbSpaceItem & init ); // \ru Сделать объекты равными. \en Make the objects equal. 
  virtual double  DistanceToPoint ( const MbCartPoint3D & ) const; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  virtual void    AddYourGabaritTo( MbCube & r ) const; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  virtual void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  virtual void    CalculateWire( const MbStepData & stepData, MbMesh & ) const; // \ru Создать полигональную копию. \en A polygonal copy. 

  virtual MbProperty & CreateProperty( MbePrompt n ) const;     // \ru Создать собственное свойство. \en Create a custom property. 
  virtual void    GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties & properties ); // \ru Установить свойства объекта. \en Set properties of the object. 
  virtual void    GetBasisPoints( MbControlData & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.
  // \ru Получить локальную систему координат объекта. \en Get the local coordinate system of an object. 
  virtual bool    GetPlacement( MbPlacement3D & ) const; 
  // \ru Установить локальную систему координат объекта. \en Set the local coordinate system of an object. 
  virtual bool    SetPlacement( const MbPlacement3D & p );

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  virtual MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const; 
  // \ru Найти объект по геометрическому объекту (MbPlaneItem). \en Find the object by a geometric object (MbSpaceItem). 
  virtual const MbItem * FindItem( const MbPlaneItem * s, MbPath & path, MbMatrix3D & from ) const;

  /** \ru \name Общие функции вставки двумерного объекта.
      \en \name Common functions of instance of two-dimensional object.
      \{ */
  /// \ru Выдать число двумерных геометрических объектов. \en Get number of two-dimensional geometric objects. 
          size_t  PlaneItemsCount() const;
          /// \ru Выдать двумерный геометрический объект. \en Get two-dimensional geometric object. 
  const   MbPlaneItem * GetPlaneItem( size_t ind = 0 ) const; 
          /// \ru Выдать двумерный геометрический объект для возможного редактирования. \en Get two-dimensional object for possible editing. 
          MbPlaneItem * SetPlaneItem( size_t ind = 0 );
          /// \ru Заменить двумерный геометрический объект. \en Replace two-dimensional geometric object. 
          void    SetPlaneItem( MbPlaneItem * init, size_t ind = 0 ); 
          /// \ru Заменить двумерный геометрический объект. \en Replace two-dimensional geometric object. 
          void    AddPlaneItem( MbPlaneItem * init ); 
          /// \ru Выдать локальную систему координат объекта. \en Get the local coordinate system of an object. 
  const   MbPlacement3D & GetPlacement() const { return place; }
          /// \ru Выдать локальную систему координат объекта для редактирования. \en Get the local coordinate system of an object for editing. 
          MbPlacement3D & SetPlacement()       { return place; } 
          
          /// \ru Преобразовать двумерный объект согласно матрице. \en Transform two-dimensional object according to the matrix. 
          void    Transform( const MbMatrix &, MbRegTransform * iReg = NULL ); 
          /// \ru Сдвинуть двумерный объект вдоль вектора. \en Translate two-dimensional object along a vector. 
          void    Move     ( const MbVector &, MbRegTransform * iReg = NULL ); 
          /// \ru Повернуть двумерный объект вокруг точки на заданный угол. \en Rotate two-dimensional object at a given angle around an axis. 
          void    Rotate   ( const MbCartPoint & pnt, const MbDirection & angle, MbRegTransform * iReg = NULL );
          /// \ru Повернуть двумерный объект вокруг точки на заданный угол. \en Rotate two-dimensional object at a given angle around an axis. 
          void    Rotate   ( const MbCartPoint & pnt, double angle, MbRegTransform * iReg = NULL ); 

          /// \ru Удалить все объекты эскиза. \en Delete all the sketch items. 
          void    DeleteItems(); 
          /// \ru Удалить объект эскиза. \en Delete the sketch item by index. 
          void    DeleteItem( size_t ind ); 
          /// \ru Удалить объект эскиза. \en Delete the sketch item by index. 
          MbPlaneItem * DetachItem( size_t ind ); 

          /** \brief \ru Заменить объект.
                     \en Replace an item. \~
            \details \ru Заменить объект новым.
                     \en Replace an item by a new one. \~
            \param[in] item - \ru Заменяемый объект.
                              \en An item to be replaced. \~
            \param[in] newItem - \ru Новый объект.
                                 \en A new item. \~
            \return \ru Возвращает true, если замена была выполнена.
                    \en Returns true if the replacement has been performed. \~
          */
          bool    ReplaceItem( MbPlaneItem & item, MbPlaneItem & newItem ); 

          /// \ru Выдать все объекты. \en Get all the items.  
          void    GetItems( RPArray<MbPlaneItem> & items ); 

  /** \} */

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPlaneInstance ) 
OBVIOUS_PRIVATE_COPY( MbPlaneInstance )
};


#endif // __PLANE_INSTANCE_H
