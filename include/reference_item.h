//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Счетчик ссылок (владельцев объекта).
         \en Reference counter (of an object owners). \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REFERENCE_ITEM_H
#define __REFERENCE_ITEM_H


#include <math_define.h>
#include <system_types.h>
#include <templ_rp_array.h>
#include <templ_s_array.h>
#include <templ_sptr.h>
#include <system_cpp_standard.h>
#include <system_dependency.h>
#include <vector>


//----------------------------------------------------------------------------------------
/** \brief \ru Типы объекта со счетчиком ссылок.
           \en Types of object with reference counter. \~
  \details \ru Тип несёт информацию об объекте-наследнике. \n
           \en Type has an information of inheritor object. \n \~
  \ingroup Geometric_Items
*/
// ---
enum MbeRefType
{
  rt_RefItem = 0,  ///< \ru Некоторый объект. \en Some object. 
  rt_PlaneItem,    ///< \ru Двумерный геометрически объект. \en Two-dimensional geometric object. 
  rt_SpaceItem,    ///< \ru Трехмерный геометрический объект. \en Three-dimensional geometric object. 
  rt_Item,         ///< \ru Объект геометрической модели. \en A model geometric object. 
  rt_TopItem,      ///< \ru Топологический объект. \en A topological object. 
  rt_Creator,      ///< \ru Строитель объекта. \en Object constructor 
  rt_Attribute,    ///< \ru Аттрибут объекта. \en Attribute of an object.  
  rt_Model,        ///< \ru Геометрическая модель. \en Geometric model. 
  // \ru В конец можно добавлять новые нужные \en It is possible to add new necessary ones to the end 
};


//----------------------------------------------------------------------------------------
/** \brief \ru Объект с подсчетом ссылок.
           \en Reference-counted object.  \~
  \details \ru Объект, считающий количество своих владельцев. \n
      Используется в качестве одного из родительских классов геометрических объектов. \n
      Если наследник данного класса захватывается другим объектом или алгоритмом,то другой 
  объект или алгоритм должен увеличить счетчик ссылок на единицу методом AddRef().
  При отказе от использования наследника данного класса другим объектом (например, при деструктурировании) 
  или алгоритмом другой объект или алгоритм должны уменьшить счетчик ссылок на единицу 
  методом Release(). Такое правило позволяет использовать одного и того же наследника
  данного класса несколькими другим объектами или алгоритмами одновременно и гарантирует, 
  что объект будет удалён, когда он станет никому не нужен.\n
              
           \en Object counting number of its owners. \n
      Is used as one of parent classes of geometric objects. \n
      If inheritor of current class is captured by other object or algorithm, then the other 
  object or algorithm has to increase reference counter by one by AddRef() method.
  At refusal of use of the successor of this class by other object (for example at destruction) 
  or by algorithm, the other object or algorithm has to decrease reference counter 
  by one by Release() method. Such rule allows to use the same inheritor of current 
  class simultaneously by several other objects or algorithms and guarantees that the 
  object will be removed when it becomes unnecessary.\n \~

  \note \ru Рекомендуется применение автоматических указателей типа SPtr к экземплярам 
    данного класса. Это упростит работу с кодом, где нужно позаботится об автоматической 
    сборке мусора. 
        \en It is recommended to use smart pointers of type SPtr to instances of 
  this class. This will simplify the work with the code where you want to take care of 
  the automatic garbage collection. \~
  \sa #SPtr
  \ingroup Geometric_Items
*/
// ---
class MATH_CLASS MbRefItem {
private:
  mutable use_count_type useCount; ///< \ru Счетчик ссылок на объект, изменяемый владельцами объекта. \en A counter of references to an object modifiable by owners of object.  
public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbRefItem();

protected:  
  virtual ~MbRefItem();

public:
  /** \ru \name Функции регистрации ссылок на геометрический объект владельцами объекта.
      \en \name Functions for registration of references to geometric object by owners of object.
      \{ */
  /// \ru Выдать количество ссылок (выдать количество владельцев объекта). \en Get count of references (get count of owners of an object). 
  refcount_t GetUseCount() const;
  /// \ru Увеличить количество ссылок на единицу. \en Increase count of references by one. 
  refcount_t AddRef() const;
  /// \ru Уменьшить количество ссылок на единицу. \en Decrease count of references by one. 
  refcount_t DecRef() const;
  /// \ru Уменьшить количество ссылок на единицу и, если количество ссылок стало равным нулю, удалить себя. \en Decrease count of references by one and if count of references became zero, then remove itself. 
  refcount_t Release() const; 
  /** \} */
public:
  /// \ru Регистрационный тип (для копирования, дублирования). \en Registration type (for copying, duplication). 
  virtual MbeRefType RefType() const; 

OBVIOUS_PRIVATE_COPY( MbRefItem )
};


//------------------------------------------------------------------------------
/// \ru Удалить объект без ссылок. \en Delete an object without references. 
//---
template <class Type>
inline void DeleteMatItem( Type *& item )
{
  if ( item != NULL ) {
    delete item;
    item = NULL;
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить объекты без ссылок. \en Delete objects without references. 
// ---
template <class Vector>
void DeleteMatItems( Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; k++ )
    ::DeleteMatItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type>
Type & DuplicateIfUsed( Type & item )
{
  Type * resItem = &item;
  if ( item.GetUseCount() > 0 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item.Duplicate() );

  return *resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type, class RegType>
Type & DuplicateIfUsed( Type & item, RegType * iReg )
{
  Type * resItem = &item;
  if ( item.GetUseCount() > 0 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item.Duplicate( iReg ) );

  return *resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type>
Type * DuplicateIfUsed( SPtr<Type> & item )
{
  Type * resItem = item.get();
  if ( item->GetUseCount() > 1 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item->Duplicate() );

  return resItem;
}


//------------------------------------------------------------------------------
/// \ru Сделать копию, если объект используется, иначе вернуть оригинал. \en Create a copy if object is used, otherwise return original. 
// ---
template <class Type, class RegType>
Type * DuplicateIfUsed( SPtr<Type> & item, RegType * iReg )
{
  Type * resItem = item.get();
  if ( item->GetUseCount() > 1 ) // \ru Если оригинал, то делаем копию. \en If there is original, then make a copy. 
    resItem = static_cast<Type *>( &item->Duplicate( iReg ) );

  return resItem;
}

//------------------------------------------------------------------------------
/// \ru Удалить объект, если он больше никому не нужен. \en Delete an object if it is unnecessary. 
// ---
template <class Type>
void DeleteItem( Type *& item )
{
  if ( item != NULL ) {
    if ( item->GetUseCount() < 1 )
      delete item;
    item = NULL;
  }
}

//------------------------------------------------------------------------------
/// \ru Освободить ссылку на объект. \en Release the reference to object. 
// ---
template <class Type>
void ReleaseItem( Type * &item )
{
  if ( item != NULL ) {
    item->Release();
    item = NULL;
  }
}

//------------------------------------------------------------------------------
/// \ru Захватить объект. \en Catch an object. 
// ---
template <class Type>
void AddRefItem( const Type * item )
{
  if ( item != NULL ) 
    item->AddRef();
}

//------------------------------------------------------------------------------
/// \ru Отпустить объект без удаления. \en Detach an object without removing. 
// ---
template <class Type>
void DecRefItem( const Type * item )
{
  if ( item != NULL ) 
    item->DecRef();
}


//------------------------------------------------------------------------------
/// \ru Захватить объекты. \en Catch objects. 
// ---
template <class Vector>
void AddRefItems( const Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; k++ ) {
    if ( items[k] != NULL )
      items[k]->AddRef();
  }
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты без удаления. \en Detach objects without removing. 
// ---
template <class Vector>
void DecRefItems( const Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; k++ ) {
    if ( items[k] != NULL )
      items[k]->DecRef();
  }
}


//------------------------------------------------------------------------------
/// \ru Удалить никому не нужные объекты. \en Remove unnecessary objects. 
// ---
template <class Vector>
void DeleteItems( Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; k++ )
    ::DeleteItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты с возможным удалением. \en Detach objects with possible removing. 
// ---
template <class Vector>
void ReleaseItems( Vector & items )
{
  for ( size_t k = 0, itemsCnt = items.size(); k < itemsCnt; k++ )
    ::ReleaseItem( items[k] );
  items.clear();
}


//------------------------------------------------------------------------------
/// \ru Удалить никому не нужные объекты. \en Remove unnecessary objects. 
// ---
template <class Vector, class coType>
void DeleteItems( Vector & items, SArray<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; k++ )
      ::DeleteItem( items[k] );
    items.clear();
    coItems.Flush();
  }
}


//------------------------------------------------------------------------------
/// \ru Отпустить объекты с возможным удалением. \en Detach objects with possible removing. 
// ---
template <class Vector, class coType>
void ReleaseItems( Vector & items, SArray<coType> & coItems )
{
  size_t itemsCnt = items.size();
  if ( itemsCnt > 0 ) {
    for ( size_t k = 0; k < itemsCnt; k++ )
      ::ReleaseItem( items[k] );
    items.clear();
    coItems.Flush();
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type>
void AddRefItems( const RPArray<Type> & srcItems, bool same, RPArray<Type> & dstItems )
{
  if ( &srcItems != &dstItems ) {
    dstItems.Reserve( srcItems.Count() );
    for ( size_t k = 0, cnt = srcItems.Count(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItem->AddRef();
        dstItems.Add( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type>
void AddRefItems( const RPArray<Type> & srcItems, bool same, std::vector< SPtr<Type> > & dstItems )
{
  if ( srcItems.size() > 0 ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        SPtr<Type> dstItem;
        dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type>
void AddRefItems( const std::vector<Type *> & srcItems, bool same, std::vector<Type *> & dstItems )
{
  if ( &srcItems != &dstItems ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate() );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type, class RegType >
void AddRefRegItems( const RPArray<Type> & srcItems, bool same, RPArray<Type> & dstItems, RegType * iReg )
{
  if ( &srcItems != &dstItems ) {
    dstItems.Reserve( srcItems.Count() );
    for ( size_t k = 0, cnt = srcItems.Count(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItem->AddRef();
        dstItems.Add( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type, class RegType >
void AddRefRegItems( const RPArray<Type> & srcItems, bool same, std::vector< SPtr<Type> > & dstItems, RegType * iReg )
{
  if ( srcItems.size() > 0 ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        SPtr<Type> dstItem;
        dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type, class RegType >
void AddRefRegItems( const std::vector<Type *> & srcItems, bool same, std::vector<Type *> & dstItems, RegType * iReg )
{
  if ( &srcItems != &dstItems ) {
    dstItems.reserve( dstItems.size() + srcItems.size() );
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      Type * srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
        dstItem->AddRef();
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Переложить элементы с захватом и возможным копированием. \en Put elements with capturing and with possible copying. 
// ---
template <class Type, class RegType >
void AddRefRegItems( const std::vector< SPtr<Type> > & srcItems, bool same, std::vector< SPtr<Type> > & dstItems, RegType * iReg )
{
  if ( &srcItems != &dstItems ) {
    for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
      const SPtr<Type> & srcItem = srcItems[k];
      if ( srcItem != NULL ) {
        SPtr<Type> dstItem( same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) ) );
        dstItems.push_back( dstItem );
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Item>
bool IsItemSame( const Item * item1, const Item * item2, double accuracy )
{
  if ( (item1 == NULL) && (item2 == NULL) )
    return true;
  else if ( (item1 != NULL) && (item2 != NULL) && item1->IsSame( *item2, accuracy ) )
    return true;
  return false;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreItemsSame( const Vector & items1, const Vector & items2, double accuracy )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; k++ ) {
      if ( (items1[k] == NULL) || (items2[k] == NULL) || !items1[k]->IsSame( *items2[k], accuracy ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreObjectsSame( const Vector & items1, const Vector & items2, double accuracy )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; k++ ) {
      if ( !items1[k].IsSame( items2[k], accuracy ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Являются ли объекты подобными. \en Determine whether the objects are similar. 
// ---
template <class Vector>
bool AreItemsSimilar( const Vector & items1, const Vector & items2 )
{
  bool areEqual = false;

  const size_t cnt = items1.size();
  if ( cnt == items2.size() ) {
    areEqual = true;
    for ( size_t k = 0; k < cnt; k++ ) {
      if ( (items1[k] == NULL) || (items2[k] == NULL) || !items1[k]->IsSimilar( *items2[k] ) ) {
        areEqual = false;
        break;
      }
    }
  }

  return areEqual;
}


//------------------------------------------------------------------------------
/// \ru Сделать равными. \en Make equal. 
// ---
template <class Vector>
bool SetItemsEqual( const Vector & srcItems, Vector & dstItems )
{
  bool setEqual = false; 

  size_t cnt = srcItems.size();
  if ( cnt == dstItems.size() ) {
    setEqual = ::AreItemsSimilar( srcItems, dstItems );

    if ( setEqual ) {
      for ( size_t k = 0; k < cnt; k++ ) {
        if ( srcItems[k] == NULL || dstItems[k] == NULL || !dstItems[k]->SetEqual( *srcItems[k] )  ) {
          setEqual = false;
          break;
        }
      }
    }
  }

  return setEqual;
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class Type, class RegType>
void DuplicateItems( const RPArray<Type> & srcItems, RegType * iReg, bool same, RPArray<Type> & dstItems )
{
  PRECONDITION( dstItems.size() < 1 );
  dstItems.Reserve( srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != NULL ) {
      Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class Type, class RegType>
void DuplicateItems( const RPArray<Type> & srcItems, RegType * iReg, bool same, std::vector< SPtr<Type> > & dstItems )
{
  PRECONDITION( dstItems.size() < 1 );
  dstItems.reserve( dstItems.size() + srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != NULL ) {
      SPtr<Type> dstItem;
      dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class Type, class RegType>
void DuplicateItems( const std::vector<Type *> & srcItems, RegType * iReg, bool same, std::vector<Type *> & dstItems )
{
  PRECONDITION( dstItems.size() < 1 );
  dstItems.reserve( dstItems.size() + srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
    Type * srcItem = srcItems[k];
    if ( srcItem != NULL ) {
      Type * dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Дублировать c регистратором (опционально переложить оригиналы). \en Duplicate with registrator (optionally put originals). 
// ---
template <class Type, class RegType>
void DuplicateItems( const std::vector< SPtr<Type> > & srcItems, RegType * iReg, bool same, std::vector< SPtr<Type> > & dstItems )
{
  PRECONDITION( dstItems.size() < 1 );
  dstItems.reserve( dstItems.size() + srcItems.size() );
  for ( size_t k = 0, cnt = srcItems.size(); k < cnt; k++ ) {
    const SPtr<Type> & srcItem = srcItems[k];
    if ( srcItem != NULL ) {
      SPtr<Type> dstItem;
      dstItem = same ? srcItem : static_cast<Type *>( &srcItem->Duplicate( iReg ) );
      dstItems.push_back( dstItem );
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
// ---
template <class Array, class Matrix, class RegType>
void TransformItems( Array & items, const Matrix & matr, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != NULL )
      items[k]->Transform( matr, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Преобразовать элементы согласно матрице. \en Transform elements according to the matrix. 
// ---
template <class Array, class Matrix>
void TransformObjects( Array & objects, const Matrix & matr )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Transform( matr );
}


//------------------------------------------------------------------------------
/// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
// ---
template <class Array, class Vector, class RegType>
void MoveItems( Array & items, const Vector & to, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != NULL )
      items[k]->Move( to, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Сдвинуть вдоль объекты вектора. \en Translate objects along a vector. 
// ---
template <class Array, class Vector>
void MoveObjects( Array & objects, const Vector & to )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Move( to );
}


//------------------------------------------------------------------------------
/// \ru Повернуть вокруг оси. \en Rotate about an axis. 
// ---
template <class Array, class Axis, class RegType>
void RotateItems( Array & items, const Axis & axis, double angle, RegType * iReg )
{
  for ( size_t k = 0, cnt = items.size(); k < cnt; ++k ) {
    if ( items[k] != NULL )
      items[k]->Rotate( axis, angle, iReg );
  }
}

//------------------------------------------------------------------------------
/// \ru Повернуть вокруг оси. \en Rotate about an axis. 
// ---
template <class Array, class Axis>
void RotateObjects( Array & objects, const Axis & axis, double angle )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k )
    objects[k].Rotate( axis, angle );
}


//------------------------------------------------------------------------------
/// \ru Запись объектов в поток. \en Write objects to the stream. 
// ---
template <class Vector, class Writer>
void WriteRefItems( const Vector & items, Writer & out )
{
  size_t k, cnt = items.size();

  for ( k = 0; k < cnt; k++ ) {
    if ( items[k] == NULL )
      cnt--;
  }

  WriteCOUNT( out, cnt );

  if ( out.good() ) {
    for ( k = 0; k < cnt; k++ ) {
      if ( items[k] != NULL ) {
        items[k]->PrepareWrite();
        out << items[k];
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Запись объекты в поток. \en Write objects to the stream. 
// ---
template <class Type, class Writer>
void WriteRefItems( const std::vector< SPtr<Type> > & items, Writer & out )
{
  size_t k, cnt = items.size();

  for ( k = 0; k < cnt; k++ ) {
    if ( items[k] == NULL )
      cnt--;
  }

  WriteCOUNT( out, cnt ); //OV_x64 out << cnt;

  if ( out.good() ) {
    for ( k = 0; k < cnt; k++ ) {
      if ( items[k] != NULL ) {
        items[k]->PrepareWrite();
        out << items[k].get();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, RPArray<Type> & items )
{  
  size_t cnt = ReadCOUNT( in ); //OV_x64 in >> cnt;

  if ( in.good() && cnt > 0 ) {
    items.reserve( items.size() + cnt );

    for ( size_t i = 0; i < cnt; i++ ) {
      Type * item = NULL;
      in >> item;
      if ( item != NULL ) {
        items.push_back( item );
        item->AddRef();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, std::vector<Type *> & items )
{  
  size_t cnt = ReadCOUNT( in ); //OV_x64 in >> cnt;

  if ( in.good() && cnt > 0 ) {
    for ( size_t i = 0; i < cnt; i++ ) {
      Type * item = NULL;
      in >> item;
      if ( item != NULL ) {
        items.push_back( item );
        item->AddRef();
      }
    }
  }
}


//------------------------------------------------------------------------------
/// \ru Чтение массива объектов из потока с захватом. \en Read an array of objects with capturing from the stream. 
// ---
template <class Type, class Reader>
void ReadRefItems( Reader & in, std::vector< SPtr<Type> > & items )
{  
  size_t cnt = ReadCOUNT( in ); //OV_x64 in >> cnt;

  if ( in.good() && cnt > 0 ) {
    for ( size_t i = 0; i < cnt; i++ ) {
      Type * item = NULL;
      in >> item;
      if ( item != NULL )
        items.push_back( SPtr<Type>(item) );
    }
  }
}


//------------------------------------------------------------------------------
/** \brief \ru Отцепить от массива и уменьшить счетчик ссылок.
           \en Detach from array and decrease reference counter. \~
  \details \ru Отцепить от массива и уменьшить счетчик ссылок без проверок.\n
           \en Detach from array and decrease reference counter without checks.\n \~  
  \param[in,out] items - \ru Множество элементов.
                         \en An array of elements. \~
  \param[in]     index - \ru Номер элемента. Не проверяется на корректность.
                         \en Index of element. Isn't checked for correctness. \~
*/
// --- 
template <class Vector>
void ReleaseAndDetachItem_( Vector & items, size_t index )
{
  items[index]->Release();
  items.erase( items.begin() + index );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~       
*/
// ---    
template <class Vector, class Type>
void AddRefAndAddItem_( Vector & items, Type * newItem )
{
  newItem->AddRef();
  items.push_back( newItem );     
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
  \param[in]     index   - \ru Номер элемента. Не проверяется на корректность.
                           \en Index of element. Isn't checked for correctness. \~
*/
// ---
template <class Vector, class Type>
void AddRefAndAddAtItem_( Vector & items, Type * newItem, size_t index )
{
  newItem->AddRef();
  items.insert( index, newItem );
}


//------------------------------------------------------------------------------
/** \brief \ru Отцепить от массива и уменьшить счетчик ссылок.
           \en Detach from array and decrease reference counter. \~
  \details \ru Отцепить от массива и уменьшить счетчик ссылок без проверок.\n
           \en Detach from array and decrease reference counter without checks.\n \~  
  \param[in,out] items - \ru Множество элементов.
                         \en An array of elements. \~
  \param[in]     index - \ru Номер элемента. Не проверяется на корректность.
                         \en Index of element. Isn't checked for correctness. \~
*/
// --- 
template <class Type>
void ReleaseAndDetachItem_( std::vector<SPtr<Type> > & items, size_t index )
{
  items[index].reset();
  items.erase( items.begin() + index );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
*/
// ---
template <class Type>
void AddRefAndAddItem_( std::vector<SPtr<Type> > & items, Type * newItem )
{
  items.push_back( SPtr<Type>(newItem) );
}


//------------------------------------------------------------------------------
/** \brief \ru Добавить объект и увеличить счетчик ссылок.
           \en Add an object and increase reference counter. \~
  \details \ru Добавить объект и увеличить счетчик ссылок без проверок.\n
           \en Add an object and increase reference counter without checks.\n \~    
  \param[in,out] items   - \ru Множество элементов.
                           \en An array of elements. \~
  \param[in]     newItem - \ru Новый элемент.
                           \en New element. \~
  \param[in]     index   - \ru Номер элемента. Не проверяется на корректность.
                           \en Index of element. Isn't checked for correctness. \~
*/
// ---
template <class Type>
void AddRefAndAddAtItem_( std::vector<SPtr<Type> > & items, Type * newItem, size_t index )
{
  items.insert( items.begin() + index, SPtr<Type>(newItem) );
}


//------------------------------------------------------------------------------
/// \ru Отцепить объект из владеющего указателя. \en Detach object from owning pointer. 
// ---
template <class Type>
Type * DetachItem( SPtr<Type> & itemOwner )
{
  Type * item = itemOwner.get();
  ::AddRefItem( item );
  itemOwner = NULL;
  ::DecRefItem( item );
  return item;
}


//------------------------------------------------------------------------------
/// \ru Заменить объект на копию. \en Replace object by copy. 
// ---
template <class Type>
void ReplaceByCopy( Type *& item )
{
  if ( item != NULL ) {
    Type * temp = (Type *)&item->Duplicate();
    ::DeleteItem( item );
    item = temp;
  }
}


//------------------------------------------------------------------------------
/// \ru Включить габариты объектов массива в общий габарит. \en Include bounding boxes of an array of objects in a common bounding box. 
//---
template<class Objects, class Gab>
void AddYourGabaritTo( Objects & objects, Gab & gab )
{
  for ( size_t k = 0, cnt = objects.size(); k < cnt; ++k ) {
    if ( objects[k] )
      objects[k]->AddYourGabaritTo( gab );
  }
}


#endif // __REFERENCE_ITEM_H
