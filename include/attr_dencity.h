////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Атрибуты. Плотность.
         \en Attributes. Density. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_DENCITY_H
#define __ATTR_DENCITY_H


#include <attr_elementary_attribut.h>


//------------------------------------------------------------------------------
/** \brief \ru Плотность.
           \en Density. \~
  \details \ru Плотность. \n
           \en Density. \n \~
  \ingroup Model_Attributes
*/
// ---
class MATH_CLASS MbDencity : public MbElementaryAttribute {
protected :
  double dencity; ///< \ru Плотность. \en Density. 

protected :
  /// \ru Конструктор. \en Constructor. 
  MbDencity( const MbDencity & init ); 
public :
  /// \ru Конструктор. \en Constructor. 
  MbDencity( double init );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbDencity();

  // \ru Общие функции объекта \en Common functions of object. 

  virtual MbeAttributeType AttributeType() const; // \ru Дать подтип атрибута. \en Get subtype of an attribute. 
  virtual MbAttribute & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Сделать копию элемента. \en Create a copy of the element. 
  virtual bool    IsSame( const MbAttribute &, double accuracy ) const; // \ru Определить, являются ли объекты равными. \en Determine whether objects are equal. 
  virtual bool    Init( const MbAttribute & ); // \ru Инициализировать данные по присланным. \en Initialize data. 

          /// \ru Установить плотность. \en Set a density. 
          void    Init( double init ) { dencity = init; }
          /// \ru Дать плотность. \en Get a density. 
          double  Dencity() const  { return dencity; }

  virtual void    GetProperties( MbProperties & ); // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual size_t  SetProperties( const MbProperties & ); // \ru Установить свойства объекта. \en Set properties of object. 
  virtual MbePrompt GetPropertyName(); // \ru Выдать заголовок свойства объекта. \en Get a name of object property. 

private:
          void        operator = ( const MbDencity & ); // \ru Не реализовано \en Not implemented 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDencity ) 

}; // MbDencity


#endif // __ATTR_DENCITY_H
