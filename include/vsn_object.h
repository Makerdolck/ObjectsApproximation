////////////////////////////////////////////////////////////////////////////////
/**
  \file 
  \brief \ru Класс Object является базовым классом для всех объектов сцены.
         \en Object class is the base class for all scene objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_OBJECT_H
#define __VSN_OBJECT_H

#include <string>
#include <vsn_hiddenmutex.h>

#include <system_types.h>

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

class ObjectPrivate;
//------------------------------------------------------------------------------
/** \brief \ru Класс Object является базовым классом для всех объектов сцены.
           \en Object class is the base class for all scene objects. \~
    \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS Object
{
public:
  /**
    \brief \ru Конструктор. 
           \en Constructor.
    \detaild \ru Идентификатор генерируется автоматически. 
             \en Identifier is generated automatically.
  */
  Object( const std::wstring& name = std::wstring() );
  /**
    \brief \ru Конструктор. 
           \en Constructor.
    \detaild \ru Идентификатор задаётся пользователем. 
             \en Identifier is assigned by user.
  */
  Object( uint id, const std::wstring& name = std::wstring() );
  /// \ru Конструктор копирования. \en Copy constructor.
  Object( const Object& sourceObject );
  /// \ru Деструктор. \en Destructor.
  virtual ~Object();

public:
  /// \ru Получить идентификатор. \en Returns identifier.
  uint GetId() const;
  /// \ru Задать идентификатор. \en Sets identifier.
  void SetId( uint id );
  /// \ru Получить имя. \en Returns name.
  const std::wstring& GetName() const;
  /// \ru Задать имя. \en Sets name.
  void SetName( const std::wstring& name );

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  Object& operator = ( const Object& );

protected:
  VSN_DECLARE_PRIVATE(Object)

protected:
  uint m_id;           ///< \ru Идентификатор. \en Identifier.
  std::wstring m_name; ///< \ru Имя объекта. \en Object name.
  HiddenMutex m_mutex;  ///< \ru Мьютекс объекта. \en Object mutex.
};
VSN_END_NAMESPACE

#endif // __VSN_OBJECT_H
