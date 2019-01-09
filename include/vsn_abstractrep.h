////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Абстрактный класс представления.
         \en Abstract representation class. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_ABSTRACTREP_H
#define __VSN_ABSTRACTREP_H

#include <string>

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

class AbstractRepPrivate;

//------------------------------------------------------------------------------
/** \brief   \ru Абстрактный класс представления.
             \en Abstract representation class. \~
    \ingroup Vision_Geometry
*/
//---
class VSN_CLASS AbstractRep
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  AbstractRep();
  /// \ru Конструктор копирования. \en Copy constructor.
  AbstractRep( const AbstractRep& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~AbstractRep();

public:
  /// \ru Сделать копию представления. \en Makes representation copy.
  virtual AbstractRep* Duplicate() const = 0;

  /// \ru Создать представление с новыми геометрическими данными. \en Returns representation copy depth.
  virtual AbstractRep* CreateRepNewGeometryData() const = 0;

  /// \ru Является ли ссылка пустой. \en Checks if reference is empty.
  bool IsReferenceEmpty() const;

  /// \ru Вернуть тип представления. \en Returns representation type.
  virtual int GetRepType() const = 0;

  /// \ru Задать имя представления. \en Sets representation name.
  void SetNameRep( const std::wstring& name );

  /// \ru Вернуть имя представления. \en Returns representation name.
  const std::wstring& GetNameRep() const;

  /// \ru Является ли представление пустым. \en Checks if representation is empty.
  virtual bool IsEmpty() const = 0;

  /// \ru Является ли представление загруженным. \en Checks if representation is loaded.
  bool IsLoaded() const;

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  virtual AbstractRep &operator = (const AbstractRep&);
  /// \ru Являются ли представления одинаковыми. \en Checks if representations are equal.
  bool operator == (const AbstractRep& rep);

public:
  /// \ru Заменить представление. \en Replaces representation.
  virtual void ReplaceRep( AbstractRep* ) = 0;

protected:
  /// \ru Установить представление загруженным. \en Makes representation loaded.
  void SetLoaded( bool bLoaded );

private:
  VSN_DECLARE_PRIVATE(AbstractRep)
};

VSN_END_NAMESPACE

#endif /* __VSN_ABSTRACTREP_H */
