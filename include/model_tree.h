//////////////////////////////////////////////////////////////////////////////////////////
/** \file 
    \brief  \ru Реализация дерева модели 
            \en Implementation of Model Tree classes \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODEL_TREE_H
#define __MODEL_TREE_H


#include <model_item.h>
#include <mb_cube.h>
#include <templ_s_array.h>
#include <tool_cstring.h>
#include <io_tree.h>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <instance.h>
#include <templ_multimap.h>

//----------------------------------------------------------------------------------------
// \ru Реализация интерфейсов дерева модели. \en Implementation of Model Tree interfaces.
//----------------------------------------------------------------------------------------

namespace c3d // namespace C3D
{
//----------------------------------------------------------------------------------------
/** \brief   \ru Тип пользовательских данных узла дерева модели.
             \en A type of user data of the model tree node. \~
    \details \ru \ru Тип пользовательских данных узла дерева модели.
             \en A type of user data of the model tree node. \~
    \ingroup Base_Tools_IO
*/
// ---
enum MbeItemDataType
{
  idtBool,            // bool
  idtInteger,         // int
  idtDouble,          // double
  idtString,          // c3d::string_t

  // \ru Данные атрибутов, для которых хранится тип и значение атрибута.
  // \en Attributes data which keeps attribute type and value.
  idtAttrBool,        // MbBoolAttribute   (bool)
  idtAttrInt,         // MbIntAttribute    (int)
  idtAttrDouble,      // MbDoubleAttribute (double)
  idtAttrString,      // MbStringAttribute (c3d::string_t)
  idtAttrInt64,       // MbInt64Attribute  (int64)
  idtAttrIdentifier,  // MbIdentifier      (int32)
  idtAttrColor,       // MbColor           (uint32)
  idtAttrWidth,       // MbWidth           (int)
  idtAttrStyle,       // MbStyle           (int)
  idtAttrSelected,    // MbSelected        (bool)
  idtAttrVisible,     // MbVisible         (bool)
  idtAttrChanged,     // MbChanged         (bool)
  idtAttrDencity,     // MbDencity         (double)
  idtAttrUpdateStamp, // MbUpdateStamp     (uint32)
  idtAttrAnchor,      // MbAnchorAttribute (uint8)

  // \ru Данные сложных трибутов, для которых хранится только тип.
  // \en Complex attributes data which keeps attribute type only.
  idtAttrVisual,                 // MbVisual
  idtAttrWireCount,              // MbWireCount
  idtAttrName,                   // MbNameAttribute
  idtAttrGeom,                   // MbGeomAttribute
  idtAttrStampRib,               // MbStampRibAttribute
  idtAttrModelInfo,              // MbModelInfo
  idtAttrPersonOrganizationInfo, // MbPersonOrganizationInfo
  idtAttrProductInfo,            // MbProductInfo
  idtAttrSTEPTextDescription,    // MbSTEPTextDescription
  idtAttrSTEPReferenceHolder,    // MbSTEPReferenceHolder
  idtAttrBinary,                 // MbBinaryAttribute

  // \ru Новый тип должен добавляться непосредственно перед idtCount (после всех определенных ранее типов).
  // \en New type should be added just before idtCount (after all types defined before).
  idtCount // \ru Число поддерживаемых типов данных. \en Number of supported data types
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Базовый класс для пользовательских данных узла дерева.
             \en A base class for user data of a tree node. \~
    \details \ru Базовый класс для пользовательских данных узла дерева.
             \en A base class for user data of a tree node. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataBase
{
// \ru Приведение объекта 'item' к типу данных 'Type' (тип проверен заранее).
// \en Cast object 'item' to the data type 'Type' (type already verified).
#define CAST(Type, item) dynamic_cast<Type*>(const_cast<ItemDataBase*>(item))

protected:
  bool m_filterByType; // \ru Фильтр только по типу (значение данных игнорируется). \en Filter by type only (ignore data value).

public:

  ItemDataBase() : m_filterByType(false) {}
  virtual ~ItemDataBase() {}

  // \ru Тип данных. \en The data type.
  virtual MbeItemDataType Type() const = 0;

  // \ru Размер записи данных в поток. \en The data size in the stream.
  virtual size_t Size( writer& ) const = 0;

  // \ru Создать данные заданного типа. \en Create data of the given type.
  static ItemDataBase* Create( MbeItemDataType type );

  // \ru Создать копию данных. \en Create data copy.
  static ItemDataBase* Create( ItemDataBase* item );

  // \ru Прочитать данные. \en Read data.
  reader& operator << ( reader& in );

  // \ru Записать данные. \en Write data.
  writer& operator >> ( writer& out ) const;

  // \ru Сравнить данные. \en Compare data.
  bool operator == ( ItemDataBase* item2 ) const;

  /// \ru Сравнить данные. \en Compare data.
  bool operator < ( ItemDataBase* item2 ) const;

  // \ru Выдать/установить флаг сравнения только по типу (значение игнорируется).
  // \en Get/set flag for comparing by type only (value ignored).
  bool IgnoreValue() const { return m_filterByType;  }
  void SetIgnoreValue( bool ignore ) { m_filterByType = ignore; }
};

//----------------------------------------------------------------------------------------
// \ru Функции чтения и записи для пользовательских данных атрибута.
// \en Functions of reading and writing for attribute user data.
//---
#define MTREE_PERSISTENT_DATA_OBJ(Class)                      \
  public:                                                            \
  static void Read(reader& in, Class* item) { in >> item->m_value; } \
  static void Write(writer& out, const Class* item) { out << item->m_value; }

//----------------------------------------------------------------------------------------
// \ru Определение типа для пользовательских данных атрибута.
// \en Type definition for attribute user data.
//---
#define MTREE_DEFINE_DATA_TYPE(type)  \
  public:                                   \
  virtual MbeItemDataType Type() const { return type; }

//----------------------------------------------------------------------------------------
/// \ru Определение размера записи пользовательских данных атрибута в поток, как sizeof.
/// \en Definition of data size of attribute user data in the stream as sizeof.
//---
#define MTREE_DEFINE_DATA_SIZE_STD(data) \
  public:                              \
  virtual size_t Size(writer&) const { return sizeof(data); }

//----------------------------------------------------------------------------------------
// \ru Макрос для объявления класса без данных (dataless) для атрибута.
// \en Macro for defining attribute dataless class.
//---
#define MTREE_ATTR_DATALESS_CLASS(Class,ClassType)            \
class Class : public ItemDataBase {                           \
public:                                                       \
  virtual size_t Size( writer& ) const { return 0; }          \
  virtual MbeItemDataType Type() const { return ClassType; }  \
  static void Read( reader&, Class* ) {}                      \
  static void Write( writer&, const Class* ) {}               \
};

//----------------------------------------------------------------------------------------
// \ru Макрос для обработки switch по типу с помощью действия 'ACTION'.
// \en Macro for processing switch by type using 'ACTION' action.
//---
#define MTREE_SWITCH_BY_DATA_TYPE(ACTION)                       \
    switch ( type ) {                                     \
    case idtBool:            ACTION(ItemDataBool);        \
    case idtInteger:         ACTION(ItemDataInteger);     \
    case idtDouble:          ACTION(ItemDataDouble);      \
    case idtString:          ACTION(ItemDataString);      \
    case idtAttrBool:        ACTION(ItemAttrBool);        \
    case idtAttrInt:         ACTION(ItemAttrInteger);     \
    case idtAttrDouble:      ACTION(ItemAttrDouble);      \
    case idtAttrString:      ACTION(ItemAttrString);      \
    case idtAttrInt64:       ACTION(ItemAttrInt64);       \
    case idtAttrIdentifier:  ACTION(ItemAttrIdentifier);  \
    case idtAttrColor:       ACTION(ItemAttrColor);       \
    case idtAttrWidth:       ACTION(ItemAttrWidth);       \
    case idtAttrStyle:       ACTION(ItemAttrStyle);       \
    case idtAttrSelected:    ACTION(ItemAttrSelected);    \
    case idtAttrVisible:     ACTION(ItemAttrVisible);     \
    case idtAttrChanged:     ACTION(ItemAttrChanged);     \
    case idtAttrDencity:     ACTION(ItemAttrDencity);     \
    case idtAttrUpdateStamp: ACTION(ItemAttrUpdateStamp); \
    case idtAttrAnchor:      ACTION(ItemAttrAnchor);      \
  }

//----------------------------------------------------------------------------------------
// \ru Макрос для обработки switch по типу с помощью действия 'ACTION' (для классов без данных).
// \en Macro for processing switch by type, using 'ACTION' action (for dataless classes).
//---
#define MTREE_SWITCH_BY_DATA_TYPE2(ACTION)                                      \
    switch ( type ) {                                                           \
    case idtAttrVisual:                 ACTION(ItemAttrVisual);                 \
    case idtAttrWireCount:              ACTION(ItemAttrWireCount);              \
    case idtAttrName:                   ACTION(ItemAttrName);                   \
    case idtAttrGeom:                   ACTION(ItemAttrGeom);                   \
    case idtAttrStampRib:               ACTION(ItemAttrStampRib);               \
    case idtAttrModelInfo:              ACTION(ItemAttrModelInfo);              \
    case idtAttrPersonOrganizationInfo: ACTION(ItemAttrPersonOrganizationInfo); \
    case idtAttrProductInfo:            ACTION(ItemAttrProductInfo);            \
    case idtAttrSTEPTextDescription:    ACTION(ItemAttrSTEPTextDescription);    \
    case idtAttrSTEPReferenceHolder:    ACTION(ItemAttrSTEPReferenceHolder);    \
    case idtAttrBinary:                 ACTION(ItemAttrBinary);                 \
    }

//----------------------------------------------------------------------------------------
/// \ru Создать объект пользовательских данных для атрибута. Возвращает NULL, если данный атрибут не поддерживается деревом модели.
/// \en Create user data object for the attribute. Return NULL if this attribute is not supported in the model tree.
//---
MATH_FUNC(ItemDataBase*) CreateAttributeData( MbAttribute* attr );

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа bool.
             \en Bool property. \~
    \details \ru Свойство типа bool.
             \en Bool property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataBool : public ItemDataBase
{
public:
  bool m_value;

  ItemDataBool() : m_value( false ) {}
  ItemDataBool( bool value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtBool)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataBool)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа integer.
             \en Integer property. \~
    \details \ru Свойство типа integer.
             \en Integer property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataInteger : public ItemDataBase
{
public:
  int m_value;

  ItemDataInteger() : m_value( 0 ) {}
  ItemDataInteger( int value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(int32)
  MTREE_DEFINE_DATA_TYPE(idtInteger)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataInteger)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа double.
             \en Double property. \~
    \details \ru Свойство типа double.
             \en Double property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataDouble : public ItemDataBase
{
public:
  double m_value;

  ItemDataDouble() : m_value( 0 ) {}
  ItemDataDouble( double value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtDouble)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataDouble)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Свойство типа string.
             \en String property. \~
    \details \ru Свойство типа string.
             \en String property. \~
    \ingroup Base_Tools_IO
*/
// ---
class ItemDataString : public ItemDataBase
{
public:
  c3d::string_t m_value;

  ItemDataString() {}
  ItemDataString( c3d::string_t value ) : m_value( value ) {}

  /// \ru Размер записи данных в поток. \en The data size in the stream.
  virtual size_t Size( writer& out ) const { return out.__lenWchar( m_value.c_str() ); }

  MTREE_DEFINE_DATA_TYPE(idtString)
  MTREE_PERSISTENT_DATA_OBJ(ItemDataString)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута bool.
             \en Data of Bool attribute. \~
    \details \ru Данные атрибута bool.
             \en Data of Bool attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrBool : public ItemDataBool
{
public:
  ItemAttrBool() : ItemDataBool() {}
  ItemAttrBool( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrBool)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrBool)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута integer.
             \en Data of Integer attribute. \~
    \details \ru Данные атрибута integer.
             \en Data of Integer attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrInteger : public ItemDataInteger
{
public:
  ItemAttrInteger() : ItemDataInteger() {}
  ItemAttrInteger( int value ) : ItemDataInteger( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrInt)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrInteger)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута double.
             \en Data of Double attribute. \~
    \details \ru Данные атрибута double.
             \en Data of Double attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrDouble : public ItemDataDouble
{
public:
  ItemAttrDouble() : ItemDataDouble() {}
  ItemAttrDouble( double value ) : ItemDataDouble( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrDouble)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrDouble)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута string.
             \en Data of String attribute. \~
    \details \ru Данные атрибута string.
             \en Data of String attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrString : public ItemDataString
{
public:
  ItemAttrString() : ItemDataString() {}
  ItemAttrString( c3d::string_t value ) : ItemDataString( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrString)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrString)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута int64.
             \en Data of int64 attribute. \~
    \details \ru Данные атрибута int64.
             \en Data of int64 attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrInt64 : public ItemDataBase
{
public:
  int64 m_value;

  ItemAttrInt64() : m_value( 0 ) {}
  ItemAttrInt64( int64 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrInt64)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrInt64)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Идентификатор.
             \en Data of Identifier attribute. \~
    \details \ru Данные атрибута Идентификатор.
             \en Data of Identifier attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrIdentifier : public ItemDataBase
{
public:
  int32 m_value;

  ItemAttrIdentifier() : m_value( 0 ) {}
  ItemAttrIdentifier( int32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrIdentifier)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrIdentifier)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Цвет.
             \en Data of Color attribute. \~
    \details \ru Данные атрибута Цвет.
             \en Data of Color attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrColor : public ItemDataBase
{
public:
  uint32 m_value;

  ItemAttrColor() : m_value( 0 ) {}
  ItemAttrColor( uint32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrColor)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrColor)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Толщина.
             \en Data of Width attribute. \~
    \details \ru Данные атрибута Толщина.
             \en Data of Width attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrWidth : public ItemDataInteger
{
public:
  ItemAttrWidth() : ItemDataInteger() {}
  ItemAttrWidth( int value ) : ItemDataInteger( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrWidth)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrWidth)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Стиль.
             \en Data of Style attribute. \~
    \details \ru Данные атрибута Стиль.
             \en Data of Style attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrStyle : public ItemDataInteger
{
public:
  ItemAttrStyle() : ItemDataInteger() {}
  ItemAttrStyle( int value ) : ItemDataInteger( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrStyle)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrStyle)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Селектированность.
             \en Data of Selection attribute. \~
    \details \ru Данные атрибута Селектированность.
             \en Data of Selection attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrSelected : public ItemDataBool
{
public:
  ItemAttrSelected() : ItemDataBool() {}
  ItemAttrSelected( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrSelected)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrSelected)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Видимость.
             \en Data of Visibility attribute. \~
    \details \ru Данные атрибута Видимость.
             \en Data of Visibility attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrVisible : public ItemDataBool
{
public:
  ItemAttrVisible() : ItemDataBool() {}
  ItemAttrVisible( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrVisible)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrVisible)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Изменённость.
             \en Data of Modification attribute. \~
    \details \ru Данные атрибута Изменённость.
             \en Data of Modification attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrChanged : public ItemDataBool
{
public:
  ItemAttrChanged() : ItemDataBool() {}
  ItemAttrChanged( bool value ) : ItemDataBool( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrChanged)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrChanged)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Плотность.
             \en Data of Dencity attribute. \~
    \details \ru Данные атрибута Плотность.
             \en Data of Dencity attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrDencity : public ItemDataDouble
{
public:
  ItemAttrDencity() : ItemDataDouble( 0 ) {}
  ItemAttrDencity( double value ) : ItemDataDouble( value ) {}

  MTREE_DEFINE_DATA_TYPE(idtAttrDencity)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrDencity)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Метка времени обновления.
             \en Data of Update timestamp attribute. \~
    \details \ru Данные атрибута Метка времени обновления.
             \en Data of Update timestamp attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrUpdateStamp : public ItemDataBase
{
public:
  uint32 m_value;

  ItemAttrUpdateStamp() : m_value( 0 ) {}
  ItemAttrUpdateStamp( uint32 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrUpdateStamp)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrUpdateStamp)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные атрибута Якорь.
             \en Data of Anchor attribute. \~
    \details \ru Данные атрибута Якорь.
             \en Data of Anchor attribute. \~
\ingroup Base_Tools_IO
*/
// ---
class ItemAttrAnchor : public ItemDataBase
{
public:
  uint8 m_value;

  ItemAttrAnchor() : m_value( 0 ) {}
  ItemAttrAnchor( uint8 value ) : m_value( value ) {}

  MTREE_DEFINE_DATA_SIZE_STD(m_value)
  MTREE_DEFINE_DATA_TYPE(idtAttrAnchor)
  MTREE_PERSISTENT_DATA_OBJ(ItemAttrAnchor)
};

//----------------------------------------------------------------------------------------
/// \ru Объявление классов без данных для атрибутов.
/// \en Definition of attribute dataless classes.
//---
MTREE_ATTR_DATALESS_CLASS(ItemAttrVisual, idtAttrVisual);
MTREE_ATTR_DATALESS_CLASS(ItemAttrWireCount, idtAttrWireCount);
MTREE_ATTR_DATALESS_CLASS(ItemAttrName, idtAttrName);
MTREE_ATTR_DATALESS_CLASS(ItemAttrGeom, idtAttrGeom);
MTREE_ATTR_DATALESS_CLASS(ItemAttrStampRib, idtAttrStampRib);
MTREE_ATTR_DATALESS_CLASS(ItemAttrModelInfo, idtAttrModelInfo);
MTREE_ATTR_DATALESS_CLASS(ItemAttrPersonOrganizationInfo, idtAttrPersonOrganizationInfo);
MTREE_ATTR_DATALESS_CLASS(ItemAttrProductInfo, idtAttrProductInfo);
MTREE_ATTR_DATALESS_CLASS(ItemAttrSTEPTextDescription, idtAttrSTEPTextDescription);
MTREE_ATTR_DATALESS_CLASS(ItemAttrSTEPReferenceHolder, idtAttrSTEPReferenceHolder);
MTREE_ATTR_DATALESS_CLASS(ItemAttrBinary, idtAttrBinary);

//----------------------------------------------------------------------------------------
/** \brief   \ru Контейнер для пользовательских данных узла дерева.
             \en A container for user data of a tree node. \~
    \details \ru Контейнер для пользовательских данных узла дерева (владеет данными).
             \en A container for user data of a tree node (owns the data). \~
    \ingroup Base_Tools_IO
*/
// ---
class UserDataMap : public MultiMap<MbeItemDataType,ItemDataBase*>
{
public:
  UserDataMap() {}
  UserDataMap( const UserDataMap& other )
  {
    Iterator curIter = other.First();
    while ( !curIter.Empty() ) {
      ItemDataBase* item = curIter.Value();
      if ( item != NULL ) {
        ItemDataBase* item2 = ItemDataBase::Create( item );
        Associate( item->Type(), item2 );
      }
      curIter++;
    }
  }

  ~UserDataMap() // \ru Владеет данными. \en Owns the data.
  {
    Iterator curIter = First();
    while ( !curIter.Empty() ) {
      ItemDataBase* item = curIter.Value();
      if ( item != NULL ) {
        delete item;
      }
      item = NULL;
      curIter++;
    }
    Flush();
  }

  /// \ru Оператор ==. \en Operator ==.
  bool operator == ( const UserDataMap& other ) const
  {
    if ( other.Count() != Count() )
      return false;
    Iterator curIter = First();
    while( !curIter.Empty() ) {
      ItemDataBase& data = *(curIter.Value());
      std::pair<Iterator, Iterator> range = other.EqualRange( curIter.Key() );
      Iterator otherIter = range.first;
      while ( otherIter != range.second ) {
        if ( data == otherIter.Value() )
          break;
        otherIter++;
      }
      if ( otherIter == range.second )
        return false;
      curIter++;
    }
    return true; 
  }

  /// \ru Оператор <. \en Operator <.
  bool operator < ( const UserDataMap& other ) const
  {
    if ( other.Count() != Count() )
      return other.Count() < Count();
    Iterator curIter = First();
    Iterator otherIter = other.First();
    while ( !curIter.Empty() ) {
      if ( curIter.Key() != otherIter.Key() )
        return otherIter.Key() < curIter.Key();
      ItemDataBase& data = *(curIter.Value());
      std::pair<Iterator, Iterator> range = other.EqualRange( curIter.Key() );
      Iterator iter = range.first;
      while ( iter != range.second ) {
        if ( data == iter.Value() )
          break;
        iter++;
      }
      if ( iter == range.second )
        return *(otherIter.Value()) < curIter.Value();
      curIter++;
      otherIter++;
    }
    return false; 
  }

  /// \ru Оператор ==. \en Operator ==.
  UserDataMap& operator = ( const UserDataMap& other )
  {
    Flush();
    Iterator curIter = other.First();
    while ( !curIter.Empty() ) {
      ItemDataBase* item = curIter.Value();
      if ( item != NULL ) {
        ItemDataBase* item2 = ItemDataBase::Create( item );
        Associate( item->Type(), item2 );
      }
      curIter++;
    }
    return *this;
  }
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Данные узла дерева.
             \en Tree node data. \~
    \details \ru Данные узла дерева. \n
             \en Tree node data. \n \~
    \ingroup Base_Tools_IO
*/
// ---
struct MbItemData
{
  // \ru Признак наличия локальной системы координат.
  // \en Token of local coordinate system presence.
  enum PlacementPresenceToken
  {
    ppt_No  = 0x01,
    ppt_Yes = 0x02
  };

  // \ru Уникальный ID узла в дереве модели. \en Unique id in the model tree.
  // \ru Не учитывается при сравнении. \en Not considered during comparison.
  size_t id;
  // \ru Тип объекта. \en Object type.
  // \ru type==st_Undefined означает, что фильтр по типу не определен.
  // \en type==st_Undefined means that filter is undefined.
  MbeSpaceType type;
  // \ru иИмя объекта. \en Object name.
  // \ru name==SYS_MAX_UINT32 означает, что фильтр по имени не определен.
  // \en name==SYS_MAX_UINT32 means that filter is undefined.
  SimpleName   name;
  // \ru gabarit.IsEmpty()==true означает, что фильтр по габариту не определен.
  // \en gabarit.IsEmpty()==true means that filter is undefined.
  MbCube       gabarit;
  // \ru Позиция записи/чтения узла. \en Position for the node writing/reading. 
  // \ru position.IsValid()==false означает, что это поле не учитывается при сравнении.
  // \en position.IsValid()==false means that this field is not considered during comparison.
  ClusterReference position;
  // \ru Локальная система координат объекта. \en Local coordinate system for the object.
  // \ru Не учитывается при сравнении. \en Not considered during comparison.
  MbPlacement3D    placement;

  // \ru Кроме обязательных данных узла, описанных выше, можно задать пользовательские данные,
  //     которые содержатся в контейнере 'properties'.
  // \en In addition to the mandatory node data, described above, it is possible to define user data,
  //     which is kept in the 'properties' container.
  // \ru Контейнер для пользовательских данных узла.
  // \en Container for user data of a node.
  UserDataMap      properties;


  // \ru Конструкторы. \en Constructors.
  MbItemData() : id(SYS_MAX_T), type(st_Undefined), name(SYS_MAX_UINT32) {}
  MbItemData ( MbeSpaceType t, SimpleName n, const MbCube& c, ClusterReference& pos ) : id(SYS_MAX_T), type(t), name(n), gabarit(c), position(pos) {}
  MbItemData ( MbeSpaceType t, SimpleName n, const MbCube& c, ClusterReference& pos, const UserDataMap& prop) : 
    id(SYS_MAX_T), type(t), name(n), gabarit(c), position(pos), properties(prop) {}
  MbItemData ( const MbItemData& data ) : id(data.id), type(data.type), 
    name(data.name), gabarit(data.gabarit), position(data.position), 
    placement(data.placement), properties(data.properties) {}

  // \ru Признак пустых (неинициализированных) данных. \en Indicator of empty (uninitialized) data.
  bool IsEmpty() const { return type == st_Undefined && name == SYS_MAX_UINT32 && gabarit.IsEmpty() && !position.IsValid() && properties.Count() == 0; }

  // \ru Идентичность полей-фильтров (id не важен).
  // \en Equality of filtering fields (id is irrelevant).
  bool operator == ( const MbItemData& rt ) const {
    return (type == rt.type || type == st_Undefined || rt.type == st_Undefined) && 
           (name == rt.name || name == SYS_MAX_UINT32 || rt.name == SYS_MAX_UINT32) && 
           ((gabarit.Contains(rt.gabarit.pmin) && gabarit.Contains(rt.gabarit.pmax)) ||
            gabarit.IsEmpty() || rt.gabarit.IsEmpty()) &&
           (position == rt.position || !position.IsValid() || !rt.position.IsValid()) &&
           (properties == rt.properties);
  }

  // \ru Сравнение для полей-фильтров (id не важен).
  // \en Comparison of filtering fields (id is irrelevant).
  bool operator < ( const MbItemData& rt ) const {
    // \ru Тип не важен или типы равны. \en Type is not important or tapes are equal.
    if ( rt.type == st_Undefined || type == st_Undefined || type == rt.type ) {
      // \ru Имя не важно или имена равны. \en Name is not important or names are equal.
      if ( rt.name == SYS_MAX_UINT32 || name == SYS_MAX_UINT32 || name == rt.name ) {
        bool gabEmpty = gabarit.IsEmpty();
        bool rGabEmpty = rt.gabarit.IsEmpty();

        // \ru Если хотя бы один из габаритов пустой. \en If at least one of gabarites is empty.
        if ( gabEmpty ) {
          if ( !rGabEmpty )
            return true;  // \ru Пустой габарит меньше. \en Empty gabarit is less.
          return false;   // \ru Пустые габариты равны. \en Empty gabarites are equal.
        }
        else if ( rGabEmpty )
          return false;   // \ru Пустой габарит меньше. \en Empty gabarit is less.

        // \ru Оба габарита непустые. \en Both gabarites are non empty.
        if ( gabarit == rt.gabarit ) {
          // \ru Сравниваем позиции. \en Compare positions.
          if ( position == rt.position ) {
            // \ru Сравниваем пользовательские данные. \en Compare user data.
            return properties < rt.properties;
          }
          else
            return position < rt.position;
        }
        else
          return CompareGabarits ( gabarit, rt.gabarit );
      }
      else
        return name < rt.name;
    }
    else
      return type < rt.type;
  }

  // \ru Специальное сравнение габаритов для сортировки объектов.
  // \en Special comparison of gabarites for object sorting.
  static bool CompareGabarits( const MbCube& a, const MbCube& b )
  {
    double eps = Math::region;
    if ( ::fabs(a.pmin.x - b.pmin.x) <= eps ) {
      if ( ::fabs(a.pmin.y - b.pmin.y) <= eps ) {
        if ( ::fabs(a.pmin.z - b.pmin.z) <= eps ) {
          if ( ::fabs(a.pmax.x - b.pmax.x) <= eps ) {
            if ( ::fabs(a.pmax.y - b.pmax.y) <= eps )
              return a.pmax.z < b.pmax.z;
            else
              return a.pmax.y < b.pmax.y;
          }
          else
            return a.pmax.x < b.pmax.x;
        }
        else
          return a.pmin.z < b.pmin.z;
      }
      else
        return a.pmin.y < b.pmin.y;
    }
    else
      return a.pmin.x < b.pmin.x;
  }

  writer& operator >> ( writer & out );
  reader& operator << ( reader & out );
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Узел дерева.
             \en Tree node. \~
    \details \ru Узел дерева (может иметь несколько потомков). \n
             \en Tree node (can have several children). \n \~
    \ingroup Base_Tools_IO
*/
// ---
class MbTreeNode : public IModelTreeNode
{
  // \ru Временное хранилище для индексов потомков узла (используется при чтении узла).
  // \en Temporary storage for indices of child nodes (used while reading the node).
  std::vector<size_t> m_childrenIndices;
  // \ru Данные узла \en The node data.
  MbItemData m_data;
  // \ru Флаг, указывающий, открыт ли узел при проходе вглубь по дереву.
  // \en Flag which indicates whether the node is entered during traversing into depth over the tree.
  mutable bool m_open;
  // \ru Флаг, указывающий, читать ли только часть узла.
  // \en Flag which indicates whether to read only a part of the node.
  mutable bool m_partial;
public:
  MbTreeNode() : m_open(false), m_partial(false) {}
  MbTreeNode ( const MbItemData& data ) : m_data(data), m_open(false), m_partial(false) {}
  MbTreeNode ( const MbTreeNode& node ) : m_data(node.m_data), m_childrenIndices(node.m_childrenIndices), 
    m_open(node.m_open), m_partial(node.m_partial) { GetChildren() = node.GetChildren(); GetParents() = node.GetParents(); }

  ///---------
  /// \ru Методы IModelTreeNode. \en IModelTreeNode methods. 

  /// \ru Доступ к данным узла. \en Access to the node data. 
  virtual MbItemData& GetData() { return m_data; }
  virtual const MbItemData& GetData() const { return m_data; }

  /// \ru Доступ к позиции чтения/записм узла. \en Access to the node read/write position. 
  virtual ClusterReference& GetPosition() { return m_data.position; }
  virtual const ClusterReference& GetPosition() const { return m_data.position; }

  /// \ru Узнать, читать ли только часть узла.
  /// \en Check whether to read the node partially.
  virtual bool PartialRead() const { return m_partial; };

  /// \ru Установить признак частичного или полного чтения узла.
  /// \en Set indication of full or partial node reading.
  virtual void SetPartialRead ( bool partial ) const { m_partial = partial; };

  /// \ru Записать узел. \en Write the node. 
  virtual writer &   operator >> ( writer & );

  /// \ru Прочитать узел. \en Read the node. 
  virtual reader &   operator << ( reader & );

  /// \ru Доступ ко все потомкам узла. \en Access to the all descendants of the node. 
  void GetAllDescendants ( std::set<IModelTreeNode*>& nodes ) const;

  /// \ru Создать узел с данными текущего узла и добавить его в дерево.
  ///     Рекурсивно скопировать в дерево всех предков текущего узла с сохранением иерархии.
  /// \en Create a node with data from the current node. 
  ///     Copy recursively all parents of the node to the tree preserving the hierarchy.
  MbTreeNode* CopyToTree ( c3d::IModelTree* tree, bool partial );

  /// \ru Доступ к флагу, который указывет, открыт ли узел при проходе вглубь по дереву
  ///     (false означает, что узел и его потомки уже пройдены или еще не обнаружены).
  /// \en Access to the flag which indicates whether the node is entered during traversing into depth over the tree
  ///     (false - means that the node and its children are already leaved or are not met yet).
  bool IsOpen() const { return m_open; }
  void SetOpen ( bool open ) { m_open = open; }
  void SetOpen ( bool open ) const { m_open = open; }

  friend class MbModelTree;

};


//----------------------------------------------------------------------------------------
/// \ru Предварительное объявление callback функции.
/// \en Forward declaration of a callback function.
//---
static bool CALL_DECLARATION FilterNodesCallback ( std::vector<IModelTree::NodeBranch>& result, const std::vector<MbItemData>& filters, IModelTree* tree );

/// \ru Callback функция определяет, нужно ли добавлять узел для объекта в дерево модели.
/// В случае, если надо добавить, заполняются данные узла.
///     \param mem -  объект геометрической модели.
///     \param data - данные узла (заполняются, если узел должен быть добавлен).
///     \return true, если узел для объекта должен быть добавлен в дерево модели, и false в противном случае.
/// \en The callback function determines whether to add a node for the given object to the model tree.
/// If the node should be added, its data is filled. 
///     \param mem -  the model object.
///     \param data - the node data (should be filled, if the node should be added to the tree).
///     \return true, if a node for the object should be added to the model tree, and false otherwise.
//---
static bool NodeToAddCallback( const TapeBase* mem, MbItemData& data )
{
  // \ru Если это MbItem, добавляем в дерево. \en If this is MbItem, put it to tree.
  const MbItem* item = dynamic_cast<const MbItem*>( mem );
  if ( item && item->Family() == st_Item ) {
    data.type = item->Type();
    data.name = item->GetItemName();
    data.gabarit.SetEmpty();
    item->AddYourGabaritTo ( data.gabarit );
    if ( item->IsA() == st_Instance )
      static_cast<const MbInstance*>(item)->GetPlacement( data.placement );
    // Fill attributes data
    const MbAttributeContainer*attributes = dynamic_cast<const MbAttributeContainer*>(item);
    if ( attributes != NULL ) {
      std::vector<MbAttribute *> attrs;
      attributes->GetAttributes( attrs,  at_Undefined, at_Undefined );
      for ( size_t i = 0; i < attrs.size(); ++i ) {
        ItemDataBase* attrData = CreateAttributeData( attrs[i] );
        if ( attrData != NULL )
          data.properties.Associate( attrData->Type(), attrData );
      }
    }
    return true;
  }
  return false;
}


//----------------------------------------------------------------------------------------
/** \brief   \ru Дерево геометрической модели.
             \en Tree of geometric model. \~
    \details \ru Дерево геометрической модели. (может иметь несколько корней).
             \en Tree of geometric model (can have several roots). \n \~
    \ingroup Base_Tools_IO
*/
// ---
class MbModelTree : public IModelTree
{
private:
  // \ru Временное хранилище для индексов корней дерева (используется при чтении дерева).
  // \en Temporary storage for indices of the tree roots (used while reading the tree).
  std::vector<size_t> m_rootsIndices;
  // \ru Все узлы дерева, упорядоченные по данным.
  // \en All nodes of the tree, ordered by data.
  std::map<MbItemData, MbTreeNode> m_filteredNodes;
  // \ru Стек узлов, открытых при чтении/записи дерева.
  // \en Stack of nodes opened during reading/writing the tree.
  std::stack<MbTreeNode*> m_nestedNodesStack;
  // \ru Все узлы дерева, упорядоченные по ID. 
  // \en All nodes of the tree, ordered by ID.
  std::map<size_t, MbTreeNode*> m_indexToNode; // \ru Вспомогательный массив. \en Auxiliary map.
  VERSION                       m_currentVersion;

public:

  // \ru Конструктор. \en Constructor.
  // \ru По умолчанию для выбора объектов для дерева модели используется функция NodeToAddCallback
  //     и для фильтрации узлов дерева - функция FilterNodesCallback.
  // \en By default, the function NodeToAddCallback is used to select objects for the model tree
  //     and the function FilterNodesCallback is used for the tree nodes filtering.
  MbModelTree() : m_currentVersion( GetCurrentExtendedFormatVersion() ) {
    IModelTree::SetNodeToAddFunction( NodeToAddCallback );
    IModelTree::SetFilterFunction( FilterNodesCallback );
  }

  ///---------
  /// \ru Методы IModelTreeNode. \en IModelTreeNode methods. 

  /// \ru Создать узел по данными и добавить в дерево. \en Create a node by data and add to the tree. 
  virtual void AddNode ( const TapeBase* mem, const ClusterReference& ref );

  /// \ru Закрыть узел (удалить узел из стека, так что родительский узел станет текущим).
  /// \en Close the node (remove it from the stack so that its parent becomes the current node). 
  virtual void CloseNode ( const TapeBase* mem );

  /// \ru Построить дерево из узлов, выбранных по заданным фильтрам.
  ///     Вызывающая функция несет ответственность за уничтожение дерева.
  /// \en Build a tree with nodes, selected by given filters.
  ///     Caller is responsible for destroying the tree.
  virtual IModelTree* GetFilteredTree ( const std::vector<MbItemData>& filters );

  /// \ru Построить дерево по заданным узлам. Вызывающая функция несет ответственность за уничтожение дерева.
  /// \en Build a tree for given nodes. Caller is responsible for destroying the tree.
  virtual IModelTree* GetFilteredTree ( std::vector<IModelTreeNode*>& nodes );

  /// \ru Версия дерева. \en Tree version.
  virtual VERSION GetVersion() { return m_currentVersion; }
  virtual void SetVersion( VERSION version ) { m_currentVersion = version; }

  /// \ru Записать дерево. \en Write the tree. 
  virtual writer &   operator >> ( writer & );

  /// \ru Прочитать дерево. \en Read the tree. 
  virtual reader &   operator << ( reader & );

  ///---------

  /// \ru Добавить узел с данными из указанного узла, если узел с такими данными не существует.
  ///     \param node  - узел с данными.
  ///     \param added - заполняется, если ненулевой (true - узел добавлен, false - узел уже существует).
  ///     \return      - возвращает указатель на узел дерева.
  /// \en Add a node with the data from the given node if a node with such data does not exist.
  ///     \param node  - a node with data.
  ///     \param added - filled if non-null (true - if a node added, false - a node already exists).
  ///     \return      - a pointer to the tree node.
  MbTreeNode* AddNode ( const MbTreeNode& node, bool* added = NULL );

  /// \ru Доступ к узлам дерева, упорядоченным по данным.
  /// \en Access to nodes of the tree, ordered by data.
  std::map<MbItemData, MbTreeNode>& GetFilteredNodes() { return m_filteredNodes; }
  const std::map<MbItemData, MbTreeNode>& GetFilteredNodes() const { return m_filteredNodes; }

protected:
  /// \ru Добавить ветвь в дерево:
  ///     \param branch - листовой узел с ветвью дерева, ведущей к нему, начиная с корневого узла дерева;
  ///     \param partial - определяет тип чтения листового узла (частичное или полное).
  /// \en Add a branch to the tree:
  ///     \param branch - a leaf node with the tree branch, leading to it;
  ///     \param partial - defines partial or full read of the leaf node.
  void AddBranch( const NodeBranch& branch, bool partial );

  /// \ru Получить уникальные узлы для данного набора узлов. Проходятся все заданные узлы и исключаются те,
  ///     которые являются потомками заданных узлов (и будут прочитаны, как их часть).
  ///     Таким образом, результат будет содержать узлы поддеревьев, содержащих все заданные узлы.
  /// \en Get unique nodes for given set of nodes. Walk through the given nodes and exclude nodes,
  ///     which are children of other given nodes (and will be read as a part of them).
  ///     Thus, the result set of nodes will represent the roots of subtrees containing all given nodes.
  std::vector<IModelTreeNode*> GetUniqueNodes ( std::vector<IModelTreeNode*>& nodes );

  /// \ru Построить дерево по индексам (используется при чтении дерева). 
  /// \en Build the tree using indices (used during reading the tree). 
  void BuildTree();

  /// \ru Заполнить массив корней дерева. 
  /// \en Fill the tree roots. 
  void FillRoots();

private:
  OBVIOUS_PRIVATE_COPY(MbModelTree)
};

//----------------------------------------------------------------------------------------
/// \ru Создать данные заданного типа. \en Create data of the given type.
//---
inline ItemDataBase* ItemDataBase::Create( MbeItemDataType type ) {
#define RETURN_NEW_OBJ(Class) \
  return new Class();

  MTREE_SWITCH_BY_DATA_TYPE(RETURN_NEW_OBJ);
  MTREE_SWITCH_BY_DATA_TYPE2(RETURN_NEW_OBJ);
  return NULL;
#undef RETURN_NEW_OBJ
}

//----------------------------------------------------------------------------------------
/// \ru Создать копию данных. \en Create data copy.
//---
inline ItemDataBase* ItemDataBase::Create(ItemDataBase* item) {
#define RETURN_NEW_OBJ_COPY(Class)                              \
{                                                               \
  ItemDataBase* obj = new Class( CAST(Class, item)->m_value );  \
  obj->SetIgnoreValue( item->IgnoreValue() );                   \
  return obj;                                                   \
}
#define RETURN_NEW_OBJ_COPY2(Class)           \
{                                             \
  ItemDataBase* obj = new Class();            \
  obj->SetIgnoreValue( item->IgnoreValue() ); \
  return obj;                                 \
}

  MbeItemDataType type = item->Type();
  MTREE_SWITCH_BY_DATA_TYPE(RETURN_NEW_OBJ_COPY);
  MTREE_SWITCH_BY_DATA_TYPE2(RETURN_NEW_OBJ_COPY2);
  return NULL;
#undef RETURN_NEW_OBJ_COPY
#undef RETURN_NEW_OBJ_COPY2
}

//----------------------------------------------------------------------------------------
/// \ru Прочитать данные. \en Read data.
//---
inline reader& ItemDataBase::operator << ( reader& in )
{
#define READ_OBJ(Class)                 \
  Class::Read( in, CAST(Class, this) ); \
  break;

  MbeItemDataType type = Type();
  MTREE_SWITCH_BY_DATA_TYPE(READ_OBJ);
  MTREE_SWITCH_BY_DATA_TYPE2(READ_OBJ);
  return in;
#undef READ_OBJ
}

//----------------------------------------------------------------------------------------
/// \ru Записать данные. \en Write data.
//---
inline writer& ItemDataBase::operator >> ( writer& out ) const 
{
#define WRITE_OBJ(Class)                  \
  Class::Write( out, CAST(Class, this) ); \
  break;

  MbeItemDataType type = Type();
  MTREE_SWITCH_BY_DATA_TYPE(WRITE_OBJ);
  MTREE_SWITCH_BY_DATA_TYPE2(WRITE_OBJ);
  return out;
#undef WRITE_OBJ
}

//----------------------------------------------------------------------------------------
/// \ru Сравнить данные. \en Compare data.
//---
inline bool ItemDataBase::operator == ( ItemDataBase* item2 ) const 
{
#define EQUAL_OBJ(Class) \
  return CAST(Class,this)->m_value == CAST(Class,item2)->m_value;

  if ( item2 == NULL )
    return false;
  if ( Type() != item2->Type() )
    return false;
  if ( IgnoreValue() || item2->IgnoreValue() )
    return true;
  MbeItemDataType type = Type();
  MTREE_SWITCH_BY_DATA_TYPE(EQUAL_OBJ);
  return true;
#undef EQUAL_OBJ
}

//----------------------------------------------------------------------------------------
/// \ru Сравнить данные. \en Compare data.
//---
inline bool ItemDataBase::operator < ( ItemDataBase* item2 ) const
{
#define COMPARE_OBJ(Class) \
  return CAST(Class,this)->m_value < CAST(Class,item2)->m_value;

  if ( item2 == NULL )
    return false;
  if ( Type() != item2->Type() )
    return Type() < item2->Type();
  if (m_filterByType != item2->m_filterByType )
    return m_filterByType < item2->m_filterByType;
  MbeItemDataType type = Type();
  MTREE_SWITCH_BY_DATA_TYPE(COMPARE_OBJ);
  return false;
#undef COMPARE_OBJ
}

//----------------------------------------------------------------------------------------
/// \ru Чтение UserDataMap. \en UserDataMap reading.
// ---
inline reader& operator >> ( reader & in, UserDataMap& itemmap )
{
  size_t typeCount = ::ReadCOUNT( in ); // \ru Количество типов данных в наборе. \en A number of types in the map.

  if ( in.good() ) {
    PRECONDITION( typeCount < idtCount );
    if ( typeCount > 0 && typeCount < idtCount ) {
      for ( size_t i = 0; i < typeCount; i++ ) {
        int t;
        in >> t;
        MbeItemDataType type = (MbeItemDataType)t; // \ru Тип данных. \en A data type.
        size_t typeSize = ::ReadCOUNT( in );
        //PRECONDITION( type < idtCount );
        if ( type < idtCount ) {
          ItemDataBase* item = ItemDataBase::Create( type );
          *item << in;
          itemmap.Associate( type, item );
        }
        else { // skip unknown data
          char* buff = new char[typeSize];
          in.readBytes( buff, typeSize );
          delete[] buff;
        }

        if ( !in.good() ) {
          in.setState( io::fail );
          break;
        }
      }
    }
  }
  return in;
}

//----------------------------------------------------------------------------------------
/// \ru Запись UserDataMap. \en UserDataMap writing.
// ---
inline writer& operator << ( writer & out, const UserDataMap& itemmap )
{
  size_t mapCount = itemmap.Count();
  ::WriteCOUNT( out, mapCount ); // \ru Количество типов данных в наборе. \en A number of types in the map.

  if ( out.good() && mapCount ) {
    UserDataMap::Iterator curIter = itemmap.First();
    while ( !curIter.Empty() ) {
      MbeItemDataType type = curIter.Key();
      ItemDataBase* item = curIter.Value();
      PRECONDITION( type < idtCount && item != NULL );
      if ( type < idtCount && item != NULL ) {
        out << (int)type; // \ru Тип данных. \en A data type.
        size_t dataSize = item->Size( out );
        ::WriteCOUNT( out, dataSize );// \ru Размер данных. \en Data size.
        *item >> out;
      }
      if ( !out.good() ) {
        out.setState( io::fail );
        break;
      }
      curIter++;
    }
  }
  return out;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение MbItemData. \en MbItemData reading.
// ---
inline reader& MbItemData::operator << ( reader & in )
{
  uint32 t = 0;
  id = ::ReadCOUNT( in, true );
  in >> t;
  type = MbeSpaceType(t);
  in >> name;
  in >> gabarit;
  position.clusterIndex = ::ReadCOUNT( in, true );
  in >> position.offset;
  int placementMark =  in.readByte();
  PRECONDITION ( placementMark == MbItemData::ppt_Yes || placementMark == MbItemData::ppt_No );
  if ( placementMark == MbItemData::ppt_Yes ) // read placement
    in >> placement;
  if ( in.MathVersion() >= MATH_18_START_VERSION ) {
    // read user data
    in >> properties;
  }
  return in;
}

//----------------------------------------------------------------------------------------
/// \ru Запись MbItemData. \en MbItemData writing.
// ---
inline writer& MbItemData::operator >> ( writer & out )
{
  ::WriteCOUNT( out, id );
  out << (uint32)type;
  out << name;
  out << gabarit;
  ::WriteCOUNT( out, position.clusterIndex );
  out << position.offset;
  if ( !placement.IsSingle() ) {
    out.writeByte ( MbItemData::ppt_Yes);
    out << placement;
  }
  else {
    out.writeByte ( MbItemData::ppt_No);
  }
  if ( out.MathVersion() >= MATH_18_START_VERSION ) {
    // write user data
    out << properties;
  }
  return out;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение узла дерева. \en Tree node reading.
// ---
inline reader& MbTreeNode::operator << ( reader & in )
{
  m_data << in;

  size_t size = ::ReadCOUNT( in, true );
  m_childrenIndices.resize( size );
  for ( size_t i = 0; i < size; ++i ) {
    m_childrenIndices[i] = ::ReadCOUNT( in, true );
  }
  return in;
}

//----------------------------------------------------------------------------------------
/// \ru Запись узла дерева. \en Tree node writing.
// ---
inline writer&  MbTreeNode::operator >> ( writer & out )
{
  m_data >> out;

  const std::set<IModelTreeNode*>& children = GetChildren();
  ::WriteCOUNT(out, children.size());
  for ( std::set<IModelTreeNode*>::const_iterator itr = children.begin(); itr != children.end(); ++itr ) {
    ::WriteCOUNT( out, (*itr)->GetData().id );
  }
  return out;
}

//----------------------------------------------------------------------------------------
/// \ru Чтение дерева. \en Tree reading.
// ---
inline reader& MbModelTree::operator << ( reader & in )
{
  if ( m_currentVersion > GetCurrentExtendedFormatVersion() )
    return in; // \ru Не читаем следующие версии. \en Can't read next versions.

  size_t size = ::ReadCOUNT(in, true);
  // read all nodes
  for ( size_t i = 0; i < size; ++i ) {
    MbTreeNode node;
    node << in;
    AddNode( node );
  }
  // read roots
  size = ::ReadCOUNT( in, true );
  m_rootsIndices.resize(size);
  for ( size_t i = 0; i < size; ++i ) {
    m_rootsIndices[i] = ::ReadCOUNT( in, true );
  }
  BuildTree();
  return in;
}

//----------------------------------------------------------------------------------------
/// \ru Запись дерева. \en Tree writing.
// ---
inline writer& MbModelTree::operator >> ( writer & out )
{
  size_t size = m_filteredNodes.size();
  // write all nodes
  ::WriteCOUNT( out, size );
  for ( std::map<MbItemData, MbTreeNode>::const_iterator i = m_filteredNodes.begin();
    i != m_filteredNodes.end(); ++i ) {
    const_cast<MbTreeNode&>(i->second) >> out;
  }
  // write IDs of roots 
  size = GetRoots().size();
  ::WriteCOUNT(out, size);
  for ( std::vector<IModelTreeNode*>::const_iterator itr = GetRoots().begin();
    itr != GetRoots().end(); ++itr ) {
    ::WriteCOUNT( out, (*itr)->GetData().id );
  }
  return out;
}

//----------------------------------------------------------------------------------------
/// \ru Реализация операторов для записи дерева в xml формате.
/// \en Implementation of operators for outputing a tree to xml.
// ---

inline void PutTabs( c3d::t_ofstream& s, size_t l ) 
{ 
  for ( size_t i = 0; i < l; ++i )
    s << _T("  ");
};

inline c3d::string_t GetItemString( MbeSpaceType type )
{
  switch (type)
  {
  case st_AssistedItem:
    return _T("AssistedItem");
  case st_Collection:
    return _T("Collection");
  case st_PointFrame:
    return _T( "PointFrame" );
  case st_WireFrame:
    return _T("WireFrame");
  case st_Solid:
    return _T("Solid");
  case st_Instance:
    return _T("Instance");
  case st_Assembly:
    return _T("Assembly");
  case st_Mesh:
    return _T("Mesh");
  case st_SpaceInstance:
    return _T("SpaceInstance");
  case st_PlaneInstance:
    return _T("PlaneInstance");
  default:
    return _T("UnknownItem");
  }
};

//----------------------------------------------------------------------------------------
/// \ru Запись узла дерева в xml формате. \en Tree node writing to xml.
// ---
inline c3d::t_ofstream& CALL_DECLARATION operator << ( c3d::t_ofstream& file, const IModelTreeNode& node )
{
  static size_t level = 0;

  MbItemData& data = const_cast<IModelTreeNode &>(node).GetData();
  size_t id = data.id;
  c3d::string_t name = GetItemString( data.type );

  ++level;
  PutTabs(file, level);
  file << "<" << name.c_str() << _T(" id='") << id << _T("' name='") << data.name <<
    _T("' gabarit=' min=(") << data.gabarit.pmin.x << _T("; ") << data.gabarit.pmin.y << _T("; ") << data.gabarit.pmin.z <<
    _T(") max=(") << data.gabarit.pmax.x << _T("; ") << data.gabarit.pmax.y << _T("; ") << data.gabarit.pmax.z << _T(")'");

  const MbTreeNode& mbnode = dynamic_cast<const MbTreeNode&>(node);
  const std::set<IModelTreeNode*>& children = node.GetChildren();
  if ( children.size() == 0 || mbnode.IsOpen() ) { // \ru Предотвращаем рекурсию. \en Avoiding recursion.
    file << _T("/>\n");
    if ( mbnode.IsOpen() && children.size() )
      file << _T("  <!-- ") << _T("Node already open. Skip children") << _T("-->\n");
  }
  else {
    mbnode.SetOpen( true );
    file << _T(">\n");
    for ( std::set<IModelTreeNode*>::const_iterator itr = children.begin(); itr != children.end(); ++itr ) {
      file << *( *itr );
    }
    mbnode.SetOpen( false );
    PutTabs( file, level );
    file << "</" << name.c_str() << _T(">\n");
  }
  level--;
  return file;
}

//----------------------------------------------------------------------------------------
/// \ru Запись узла дерева в xml формате. \en Tree node writing to xml.
// ---
inline c3d::t_ofstream& CALL_DECLARATION operator << ( c3d::t_ofstream& file, IModelTreeNode& node )
{
  return operator << (file, (const IModelTreeNode&)node);
}

//----------------------------------------------------------------------------------------
/// \ru Запись дерева в xml формате. \en Tree writing to xml.
// ---
inline c3d::t_ofstream& CALL_DECLARATION operator << ( c3d::t_ofstream& file, const IModelTree& tree )
{
  file << _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

  file << _T("<tree>\n");
  for (std::vector<IModelTreeNode*>::const_iterator i = tree.GetRoots().begin(); i != tree.GetRoots().end(); ++i) {
    file << *(*i);
  }
  file << _T("</tree>\n");
  return file;
}

//----------------------------------------------------------------------------------------
/// \ru Запись дерева в xml формате. \en Tree writing to xml.
// ---
inline c3d::t_ofstream& CALL_DECLARATION operator << ( c3d::t_ofstream& file, IModelTree& tree )
{
  return operator << (file, (const IModelTree&)tree);
}


//----------------------------------------------------------------------------------------
/// \ru Доступ ко все потомкам узла. \en Access to the all descendants of the node. 
//---
inline void MbTreeNode::GetAllDescendants( std::set<IModelTreeNode*>& nodes ) const
{
  for ( std::set<IModelTreeNode*>::const_iterator itr = m_children.begin(); itr != m_children.end(); ++itr ) {
    m_open = true;
    if ( *itr != this && !static_cast<MbTreeNode*>(*itr)->m_open ) {
      nodes.insert ( *itr );
      std::set<IModelTreeNode*> children;
      static_cast<MbTreeNode*>(*itr)->GetAllDescendants ( children );
      nodes.insert ( children.begin(), children.end() );
    }
    m_open = false;
  }
}

//----------------------------------------------------------------------------------------
/// \ru Добавить ветвь в дерево. \en Add a branch to the tree.
//---
inline void MbModelTree::AddBranch ( const NodeBranch& branch, bool partial )
{
  c3d::IModelTreeNode* node = branch.first;
  const std::vector<c3d::IModelTreeNode*>& parents = branch.second;
  if ( parents.size() == 0 ) {
    static_cast<MbTreeNode*>(node)->CopyToTree ( this, partial );
    return;
  }

  MbItemData data(node->GetData());
  bool added = false;
  MbTreeNode* newNode = AddNode( MbTreeNode(data), &added );
  newNode->SetPartialRead ( added ? partial : partial | newNode->PartialRead() );
  for ( std::vector<IModelTreeNode*>::const_reverse_iterator itr = parents.rbegin(); itr != parents.rend(); ++itr ) {
    MbItemData newData((*itr)->GetData());
    MbTreeNode* newParent = AddNode( MbTreeNode(newData), &added );
    newParent->AddChild ( newNode );
    newParent->SetPartialRead ( true );
    newNode = newParent;
  }
}

//----------------------------------------------------------------------------------------
/// \ru Создать узел с данными текущего узла и добавить его в дерево.
///     Рекурсивно скопировать в дерево всех предков текущего узла с сохранением иерархии.
//  \en Create a node with data from the current node.
///     Copy recursively all parents of the node to the tree preserving the hierarchy.
//---
inline MbTreeNode* MbTreeNode::CopyToTree ( c3d::IModelTree* tree, bool partial )
{
  MbItemData data(m_data);
  bool added = false;
  MbTreeNode* newNode = static_cast<MbModelTree*>(tree)->AddNode( MbTreeNode(data), &added );
  newNode->SetPartialRead ( added ? partial : partial && newNode->PartialRead() );
  for ( std::set<IModelTreeNode*>::const_iterator itr = m_parents.begin(); itr != m_parents.end(); ++itr ) {
    if ( (*itr) && *itr != this ) {
      MbTreeNode* newParent = static_cast<MbTreeNode*>(*itr)->CopyToTree ( tree, true );
      newParent->AddChild ( newNode );
    }
  }
  return newNode;
}

//----------------------------------------------------------------------------------------
/// \ru Закрыть узел (удалить узел из стека, так что родительский узел станет текущим).
/// \en Close the node (remove it from the stack so that its parent becomes the current node). 
//---
inline void MbModelTree::CloseNode( const TapeBase* mem )
{
  // if this is MbItem
  const MbItem* item = dynamic_cast<const MbItem*>(mem);
  if ( item && item->Family() == st_Item ) {
    if ( m_nestedNodesStack.size() ) {
      if ( m_nestedNodesStack.top() )
        m_nestedNodesStack.top()->SetOpen ( false );
      m_nestedNodesStack.pop();
    }
  }
}

//----------------------------------------------------------------------------------------
/// \ru Добавить узел. \en Add a node. 
//---
inline MbTreeNode* MbModelTree::AddNode( const MbTreeNode& node, bool* added )
{
  std::pair<std::map<MbItemData, MbTreeNode>::iterator, bool> ret = 
    GetFilteredNodes().insert( std::pair<MbItemData, MbTreeNode>(node.GetData(), node) );
  if ( ret.second ) {
    MbTreeNode& n = ret.first->second;
    if ( n.GetData().id == SYS_MAX_T )
      n.GetData().id = GetFilteredNodes().size() - 1;
    m_indexToNode[n.GetData().id] = &n;
  }
  if ( added )
    *added = ret.second;
  return &ret.first->second;
}

//----------------------------------------------------------------------------------------
/// \ru Создать узел по данными и добавить в дерево. \en Create a node by data and add to the tree. 
//---
inline void MbModelTree::AddNode( const TapeBase* mem, const ClusterReference& ref )
{
  MbItemData data;
  if ( m_nodeToAddFunc( mem, data ) ) {
    // \ru Добавляем узел в дерево. \en Put a node to the tree.
    data.position = ref;
    MbTreeNode* node = AddNode( MbTreeNode(data) );
    node->SetOpen( true );
    if ( m_nestedNodesStack.size() )
      m_nestedNodesStack.top()->AddChild ( node );
    else
      GetRoots().push_back ( node );
    m_nestedNodesStack.push ( node );
  }
}

//----------------------------------------------------------------------------------------
/// \ru Построить дерево по индексам (используется при чтении дерева). 
/// \en Build the tree using indices (used during reading the tree). 
//---
inline void MbModelTree::BuildTree()
{
  for ( std::map<MbItemData, MbTreeNode>::iterator i = GetFilteredNodes().begin();
    i != GetFilteredNodes().end(); ++i ) {
    MbTreeNode& node = i->second;
    for ( size_t k = 0; k < node.m_childrenIndices.size(); k++ ) {
      std::map<size_t, MbTreeNode*>::iterator itr = m_indexToNode.find ( node.m_childrenIndices[k] );
      if ( itr != m_indexToNode.end() ) {
        node.m_children.insert( itr->second );
        itr->second->AddParent ( (c3d::IModelTreeNode*)&node );
      }
    }
  }
  std::vector<c3d::IModelTreeNode*>& roots = GetRoots();
  roots.clear();
  for ( size_t k  = 0; k < m_rootsIndices.size(); k++ ) {
    std::map<size_t, MbTreeNode*>::iterator itr = m_indexToNode.find ( m_rootsIndices[k] );
    if ( itr != m_indexToNode.end() )
      roots.push_back( itr->second );
  }
}

//----------------------------------------------------------------------------------------
/// \ru Заполнить массив корней дерева. \en Fill the tree roots. 
//---
inline void MbModelTree::FillRoots()
{
  std::vector<c3d::IModelTreeNode*>& roots = GetRoots();
  roots.clear();
  for ( std::map<MbItemData, MbTreeNode>::iterator i = GetFilteredNodes().begin();
    i != GetFilteredNodes().end(); ++i ) {
    MbTreeNode& node = i->second;
    if ( node.GetParents().size() == 0 )
      roots.push_back ( &node );
  }
}

//----------------------------------------------------------------------------------------
/// \ru Предикат для сравнения узлов по имени и габариту. 
/// \en Predicate for comparison of nodes by name and gabarit.
//---
struct NodeComparer
{
  bool operator () ( IModelTreeNode* a, IModelTreeNode* b ) {
    return a->GetData().name == b->GetData().name ?
      MbItemData::CompareGabarits ( a->GetData().gabarit, b->GetData().gabarit ) :
      a->GetData().name < b->GetData().name; }
};

//----------------------------------------------------------------------------------------
/// \ru Унарный предикат для сравнения узлов по имени и габариту. 
/// \en Unary predicate for comparison of nodes by name and gabarit.
//---
struct NodeComparerUnary
{
  const MbItemData* _data;
  NodeComparerUnary ( const MbItemData& data ) : _data(&data) {}
  NodeComparerUnary ( const NodeComparerUnary& ncu ) : _data(ncu._data) {}
  bool operator() ( IModelTreeNode* a ) {
    return a->GetData().name == _data->name &&
      (_data->gabarit.IsEmpty() || _data->gabarit == a->GetData().gabarit ||
        MbItemData::CompareGabarits ( _data->gabarit, a->GetData().gabarit) );
  }
};

//----------------------------------------------------------------------------------------
/// \ru Предикат для сортировки ветвей по пользовательским данным листового узла. 
/// \en Predicate for sorting of branches by user data of leaf node.
//---
struct CompareBranchByUserData
{
  bool operator() ( const IModelTree::NodeBranch& a, const IModelTree::NodeBranch& b )
  {
    return a.first->GetData().properties < b.first->GetData().properties;
  }
};

//----------------------------------------------------------------------------------------
/// \ru Сравнение ветвей по пользовательским данным листового узла.
/// \en Comparison of branches by user data of leaf node.
//---
struct BranchByUserDataComparer
{
  UserDataMap _data;
  BranchByUserDataComparer ( const UserDataMap& data ) : _data(data) {}
  BranchByUserDataComparer ( const BranchByUserDataComparer& ncu ) : _data(ncu._data) {}

  /// \ru Проверить, содержит ли узел заданные пользовательские данные.
  /// \en Check, whether a node contains the specified user data.
  bool operator == ( const IModelTreeNode* a ) const {
    if (_data.Count() == 0)
      return true;
    const UserDataMap& other = a->GetData().properties;
    if ( other.Count() == 0 )
      return false;
    UserDataMap::Iterator curIter = _data.First();
    while ( !curIter.Empty() ) {
      std::pair<UserDataMap::Iterator, UserDataMap::Iterator> range = other.EqualRange( curIter.Key() );
      UserDataMap::Iterator iter = range.first;
      while ( iter != range.second ) {
        if ( *(curIter.Value()) == iter.Value() )
          break;
        iter++;
      }
      if ( iter == range.second )
        return false;
      curIter++;
    }
    return true; 
  }

  /// \ru Найти потомков узла с заданными пользовательскими данными.
  /// \en Get node children with the specified user data.
  static void GetChildren( std::vector<IModelTreeNode*>& result, IModelTreeNode* parent, BranchByUserDataComparer& comparer )
  {
    const std::set<IModelTreeNode*>& children = parent->GetChildren();
    for ( std::set<IModelTreeNode*>::const_iterator itr = children.begin(); itr != children.end(); itr++ ) {
      if ( comparer == *itr )
        result.push_back( *itr ); 
      else
        BranchByUserDataComparer::GetChildren( result, *itr, comparer );
    }
  }

};

//----------------------------------------------------------------------------------------
/// \ru Поиск узлов по габариту, начиная с листового узла. 
///     \param node -          текущий узел.
///     \param init -          начальный узел, с которого стартовал поиск.
///     \param filterGabarit - фильтрующий габарит.
///     \param branch -        набор узлов дерева от начального узла к текущему.
///     \param ready -         финальный набор ветвей дерева, входящих в указанный габарит.
/// \en Searching nodes by gabarit, starting from a leaf node.
///     \param node -          current node.
///     \param init -          initial node, the search started from.
///     \param filterGabarit - filtering gabarit.
///     \param branch -        a set of tree nodes from the initial node to the current one.
///     \param ready -         the final set of the tree branches, which are included to the gabarit.
//---
static void FindMatchGabaritsFromLeaf ( IModelTreeNode* node, IModelTreeNode* init, const MbCube& filterGabarit, std::vector<IModelTreeNode*>& branch,
  std::vector<MbModelTree::NodeBranch>& ready )
{
  if ( !node->GetParents().size() ) { // \ru Корневой узел. \en Root node.
    MbCube gab = init->GetData().gabarit;
    for ( size_t i = branch.size() - 1; i < branch.size(); --i ) {
      MbMatrix3D matrix = branch[i]->GetData().placement.GetMatrixFrom();
      if ( !matrix.IsSingle() )
        gab.Transform ( matrix );
    }
    if ( filterGabarit.Contains ( gab.pmin ) && filterGabarit.Contains ( gab.pmax ) )
      ready.push_back ( std::make_pair ( init,branch ) );
    return;
  }

  std::set<IModelTreeNode*>& parents = node->GetParents();
  for ( std::set<IModelTreeNode*>::iterator itr = parents.begin(); itr != parents.end(); ++itr) {
    branch.insert ( branch.begin(), *itr );
    FindMatchGabaritsFromLeaf ( *itr, init, filterGabarit, branch, ready );
    if ( branch.size() )
      branch.erase ( branch.begin() );
  }
  return;
}

//----------------------------------------------------------------------------------------
/// \ru Поиск узлов по габариту, начиная с корневого узла.
///     \param node -          текущий узел.
///     \param filterGabarit - фильтрующий габарит.
///     \param parents -       набор родительских узлов для текущего узла.
///     \param ready -         финальный набор ветвей дерева, входящих в указанный габарит.
/// \en Searching nodes by gabarit, starting from a root node.
///     \param node -          current node.
///     \param filterGabarit - filtering gabarit.
///     \param parents -       a set of parent nodes for the current node.
///     \param ready -         the final set of the tree branches, which are included to the gabarit.
//---
static void FindMatchGabaritFromRoot ( IModelTreeNode* node, const MbCube& filterGabarit, std::vector<IModelTreeNode*>& parents, 
  std::vector<MbModelTree::NodeBranch>& ready )
{
  MbCube gab = node->GetData().gabarit;
  for ( size_t i = parents.size() - 1; i < parents.size(); --i ) {
    MbMatrix3D matrix = parents[i]->GetData().placement.GetMatrixFrom();
    if ( !matrix.IsSingle() )
      gab.Transform ( matrix );
  }
  if ( filterGabarit.Contains ( gab.pmin ) && filterGabarit.Contains ( gab.pmax ) ) {
    ready.push_back( std::make_pair(node, parents) );
    return;
  }

  parents.push_back ( node );

  const std::set<IModelTreeNode*>& children = node->GetChildren();
  for ( std::set<IModelTreeNode*>::const_iterator itr = children.begin(); itr != children.end(); ++itr) {
    FindMatchGabaritFromRoot ( *itr, filterGabarit, parents, ready );
  }
  if ( parents.size() )
    parents.resize ( parents.size() - 1 );
}

//----------------------------------------------------------------------------------------
/// \ru Функция выбирает узлы из дерева модели, используя указанные фильтры.
///     \param tree -    дерево модели для фильтрации.
///     \param filters - фильтры.
///     \param result -  выбранные ветви дерева модели.
/// \en Function selects nodes from the model tree using the defined filters.
///     \param tree  -   model tree for filtering.
///     \param filters - filters.
///     \param result -  selected tree branches.
//---
static bool FilterNodesCallback ( std::vector<IModelTree::NodeBranch>& result, const std::vector<MbItemData>& filters, IModelTree* tree )
{
  if ( filters.size() == 0 )
    return false;

  MbModelTree* modelTree = dynamic_cast<MbModelTree*>(tree);
  if ( !modelTree )
    return false;

  std::vector<IModelTreeNode*> nodes;
  std::map<MbItemData, MbTreeNode>& filteredNodes = modelTree->GetFilteredNodes();
  for ( std::map<MbItemData, MbTreeNode>::iterator m = filteredNodes.begin(); 
    m != filteredNodes.end(); ++m )
    nodes.push_back ( &m->second );

  for ( std::vector<MbItemData>::const_iterator fitr = filters.begin(); fitr != filters.end(); ++fitr ) {
    std::vector<IModelTree::NodeBranch> resultLoc;
    const MbItemData& data = *fitr;
    if ( data.IsEmpty() )
      continue;
    if ( data.type != st_Undefined ) {
      std::map<MbItemData, MbTreeNode>::iterator begin = filteredNodes.lower_bound ( data );
      for ( std::map<MbItemData, MbTreeNode>::iterator itr = begin; itr != filteredNodes.end(); ++itr ) {
        if ( data.type == itr->first.type && (data.name == SYS_MAX_UINT32 || data.name == itr->first.name ) ) {
          // \ru Фильтрация по габариту. \en Filtering by gabarit.
          std::vector<c3d::IModelTreeNode*> parents;
          if ( data.gabarit.IsEmpty() ||  itr->first.gabarit.IsEmpty() ) 
            resultLoc.push_back ( std::make_pair ( &itr->second, parents ) );
          else 
            FindMatchGabaritsFromLeaf ( &itr->second, &itr->second, data.gabarit, parents, resultLoc );
        }
      }
    }
    else {
      // \ru Тип не определен: нужно пройти по всем узлам. \en Undefined type: should go through all nodes.
      if ( data.name != SYS_MAX_UINT32 ) {
        NodeComparer comparer;
        std::sort( nodes.begin(), nodes.end(), comparer );
        NodeComparerUnary comparer2( data );
        std::vector<IModelTreeNode*>::iterator begin = std::find_if( nodes.begin(), nodes.end(), comparer2 );
        for ( std::vector<IModelTreeNode*>::iterator itr = begin; itr != nodes.end(); ++itr ) {
          if ( data.name == (*itr)->GetData().name ) {
            // \ru Фильтрация по габариту. \en Filtering by gabarit.
            std::vector<c3d::IModelTreeNode*> parents;
            if ( data.gabarit.IsEmpty() ||  (*itr)->GetData().gabarit.IsEmpty() )
              resultLoc.push_back ( std::make_pair ( *itr, parents ) );
            else  
              FindMatchGabaritsFromLeaf ( *itr, *itr, data.gabarit, parents, resultLoc );
          }
        }
      }
      else {
        const std::vector<IModelTreeNode*>& roots = modelTree->GetRoots();
        for ( std::vector<IModelTreeNode*>::const_iterator itr = roots.begin(); itr != roots.end(); ++itr ) {
          std::vector<IModelTreeNode*> parents;
          if ( data.gabarit.IsEmpty() || (*itr)->GetData().gabarit.IsEmpty() )
            resultLoc.push_back ( std::make_pair ( *itr, parents ) );
          else
            FindMatchGabaritFromRoot ( *itr, data.gabarit, parents, resultLoc );
        }
      }
    }
    // \ru Выбираем по пользовательским данным. \en Choose by user data.
    if ( data.properties.Count() > 0 && resultLoc.size() > 0 ) {
      // \ru Выбираем из найденных узлов. \en Choose from found nodes.
      std::vector<IModelTree::NodeBranch> result2;
      CompareBranchByUserData comparer;
      std::sort( resultLoc.begin(), resultLoc.end(), comparer );
      BranchByUserDataComparer comparer2( data.properties );
      std::vector<IModelTree::NodeBranch>::iterator begin = resultLoc.begin();
      for ( std::vector<IModelTree::NodeBranch>::iterator itr = begin; itr != resultLoc.end(); ++itr ) {
        if ( comparer2 == itr->first ) {
          result2.push_back ( *itr );
        }
        else {
          // go through children
          std::vector<IModelTreeNode*> children;
          std::vector<IModelTreeNode*> parents;
          BranchByUserDataComparer::GetChildren( children, itr->first, comparer2 );
          std::vector<IModelTreeNode*>::iterator ii = children.begin();
          for ( ; ii != children.end(); ii++ )
            result2.push_back ( std::make_pair ( *ii, parents ) );
        }
      }
      resultLoc.swap( result2 );
    } // data.properties.Count() > 0
    if ( resultLoc.size() > 0 )
      result.insert( result.end(), resultLoc.begin(), resultLoc.end() );
  }
  return true;
}

//----------------------------------------------------------------------------------------
/// \ru Получить значимые узлы поддерева с указанным корнем
///     (исключить узлы, которые добавлены для восстановления иерархии дерева).
///     Возвращает количество значимых узлов.
/// \en Get substantive nodes in a subtree with the given root
///     (exclude nodes that were added to reconstruct the tree hierarchy).
///     Return a number of substantive nodes.
//---
static size_t GetSubstantiveNodes ( c3d::IModelTreeNode* node, std::set<c3d::IModelTreeNode*>& mnodes )
{
  size_t num = 0;
  if ( !node->PartialRead() ) {
    std::pair<std::set<c3d::IModelTreeNode*>::iterator,bool> res = mnodes.insert(node);
    if ( res.second )
      ++num;
  }
  const std::set<c3d::IModelTreeNode*>& children = node->GetChildren();
  std::set<c3d::IModelTreeNode*>::const_iterator itr = children.begin();
  while ( itr != children.end() ) {
    num += GetSubstantiveNodes ( *itr, mnodes );
    ++itr;
  }
  return num;
}

//----------------------------------------------------------------------------------------
/// \ru Построить дерево из узлов, выбранных по фильтрам. Вызывающая функция несет ответственность за уничтожение дерева.
//  \en Build a tree with nodes, selected by filters. Caller is responsible for destroying the tree.
//---
inline IModelTree* MbModelTree::GetFilteredTree ( const std::vector<MbItemData>& filters )
{
  if ( filters.size() == 0 )
    return this;

  std::vector<NodeBranch> result;
  if ( m_filterFunc( result, filters, this ) )
  {
    std::auto_ptr<IModelTree> tempTree(IModelTree::CreateModelTree());
    for ( std::vector<NodeBranch>::iterator itr = result.begin(); itr != result.end(); ++itr ) {
      static_cast<MbModelTree*>(tempTree.get())->AddBranch ( NodeBranch(itr->first, itr->second), false );
    }
    static_cast<MbModelTree*>(tempTree.get())->FillRoots();
  
    // \ru Удаление дубликатов. \en Removing duplicates.
    std::set<c3d::IModelTreeNode*> uniqueNodes;
    std::vector<c3d::IModelTreeNode*> roots = tempTree->GetRoots();
    for ( std::vector<c3d::IModelTreeNode*>::iterator itr = roots.begin(); itr != roots.end(); ++itr ) {
      GetSubstantiveNodes ( *itr, uniqueNodes );
    }

    std::vector<IModelTreeNode*> nodes;
    for ( std::set<c3d::IModelTreeNode*>::iterator itr = uniqueNodes.begin(); itr != uniqueNodes.end(); ++itr ) 
      nodes.push_back(*itr);

    return GetFilteredTree ( nodes );
  }
  return this;
}

//----------------------------------------------------------------------------------------
/// \ru Получить уникальные узлы для данного набора узлов. Проходятся все заданные узлы и исключаются те,
///     которые являются потомками заданных узлов (и будут прочитаны, как их часть).
///     Таким образом, результат будет содержать узлы поддеревьев, содержащих все заданные узлы.
/// \en Get unique nodes for given set of nodes. Walk through the given nodes and exclude nodes,
///     which are children of other given nodes (and will be read as a part of them).
///     Thus, the result set of nodes will represent the roots of subtrees containing all given nodes.
//---
inline std::vector<IModelTreeNode*> MbModelTree::GetUniqueNodes ( std::vector<IModelTreeNode*>& nodes )
{
  std::vector<IModelTreeNode*> res(nodes);
  std::vector<IModelTreeNode*> prepared;
  std::vector<IModelTreeNode*> ready;

  std::vector<IModelTreeNode*>::iterator itr = res.begin();
  while ( itr != res.end() ) {
    IModelTreeNode* node = *itr;
    res.erase ( itr );
    if ( node != NULL ) {
      std::set<IModelTreeNode*> children;
      static_cast<MbTreeNode*>(node)->GetAllDescendants ( children );
      if ( children.size() ) {
        itr = res.begin();
        while ( itr != res.end() ) {
          if ( *itr ) {
            std::set<IModelTreeNode*>::iterator ch = children.find ( *itr );
            if ( ch != children.end() )
              *itr = NULL;
          }
          ++itr;
        }
        std::vector<IModelTreeNode*>::iterator itr2 = prepared.begin();
        while ( itr2 != prepared.end() ) {
          if ( *itr2 ) {
            std::set<IModelTreeNode*>::iterator ch = children.find ( *itr2 );
            if ( ch != children.end() )
              *itr2 = NULL;
          }
          ++itr2;
        }
      }
      prepared.push_back ( node );
    }
    itr = res.begin();
  }

  itr = prepared.begin();
  while ( itr != prepared.end() ) {
    if ( *itr )
      ready.push_back ( *itr );
    ++itr;
  }

  return ready;
}

//----------------------------------------------------------------------------------------
/// \ru Построить дерево по заданным узлам. Вызывающая функция несет ответственность за уничтожение дерева.
/// \en Build a tree for given nodes. Caller is responsible for destroying the tree.
//---
inline IModelTree* MbModelTree::GetFilteredTree ( std::vector<IModelTreeNode*>& nodes )
{
  if ( nodes.size() == 0 )
    return NULL;

  std::vector<IModelTreeNode*> result = GetUniqueNodes ( nodes );

  // \ru Для выбранного узла, проходим по родителям узлам вплоть до корня и строим поддерево с этим узлом.
  // \en For each selected node, go through parents up to the root and build a subtree with the node.
  IModelTree* tree = IModelTree::CreateModelTree();
  std::vector<IModelTreeNode*>::iterator itr = result.begin();
  while ( itr != result.end() ) {
    if ( *itr )
      static_cast<MbTreeNode*>(*itr)->CopyToTree(tree, false);
    ++itr;
  }
  static_cast<MbModelTree*>(tree)->FillRoots();
  return tree;
}

} //namespace c3d

#endif // __MODEL_TREE_H
