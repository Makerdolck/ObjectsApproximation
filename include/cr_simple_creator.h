////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Построение оболочки тела без истории.
         \en Constructor of solid shell without history. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_SIMPLE_CREATOR_H
#define __CR_SIMPLE_CREATOR_H


#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки тела без истории.
           \en Constructor of a solid shell without history. \~
  \details \ru Строитель оболочки тела без истории построения. \n
           \en Constructor of a solid shell without history tree. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbSimpleCreator : public MbCreator {
public :
  MbFaceShell * outer;     ///< \ru Набор граней без истории. \en Face set without history. 
  OperationType operation; ///< \ru Тип булевой операции. \en Boolean operation type. 
  double        buildSag;  ///< \ru Шаг построения булевой операции. \en Sag of Boolean operation construction. 

public :
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по оболочке с возможностью использования ее оригинала или копии.
             \en Constructor by a shell with possibilyty of using the original or a copy. \~
    \param[in] shell - \ru Оболочка.
                       \en A shell. \~
    \param[in] n     - \ru Именователь операции.
                       \en An object defining a name of the operation. \~
    \param[in] same  - \ru Признак использования оригинала оболочки.
                       \en Flag of using the original shell. \~
  */
  MbSimpleCreator( MbFaceShell & shell, const MbSNameMaker & n, bool same );

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по оболочке и типу операции.
             \en Constructor by a shell and a type of operation. \~
    \param[in] shell - \ru Оболочка.
                       \en A shell. \~
    \param[in] n     - \ru Именователь операции.
                       \en An object defining a name of the operation. \~
    \param[in] op    - \ru Тип булевой операции.
                       \en A Boolean operation type. \~
  */
  MbSimpleCreator( MbFaceShell & shell, const MbSNameMaker & n, OperationType op );

private :
  MbSimpleCreator( const MbSimpleCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbSimpleCreator( const MbSimpleCreator & ); 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbSimpleCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  virtual MbeCreatorType IsA() const; // \ru Тип элемента \en Type of element 
  virtual MbCreator & Duplicate( MbRegDuplicate * = NULL ) const ; // \ru Сделать копию \en Create a copy 
  virtual void        Transform( const MbMatrix3D &, MbRegTransform * = NULL );             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void        Move     ( const MbVector3D &, MbRegTransform * = NULL );             // \ru Сдвиг \en Translation 
  virtual void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate about an axis 

  virtual MbePrompt   GetPropertyName(); // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  virtual void        GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void        SetProperties( const MbProperties & properties ); // \ru Записать свойства объекта \en Set properties of the object 
  virtual void        GetBasisItems  ( RPArray<MbSpaceItem> & ); // \ru Дать базовые объекты \en Get the basis objects 
  virtual void        GetBasisPoints( MbControlData3D & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void        SetBasisPoints( const MbControlData3D & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  virtual bool        IsSame( const MbCreator &, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal?
  virtual bool        IsSimilar ( const MbCreator & ) const; // \ru Являются ли объекты подобными \en Whether the objects are similar 
  virtual bool        SetEqual( const MbCreator & );       // \ru Сделать равным \en Make equal 

  virtual MbFaceShell * GetShell() const; // \ru Дать оболочку \en Get a shell 
  virtual void        SetShell( MbFaceShell *s );  // \ru Заменить оболочку \en Replace a shell 
  virtual OperationType GetOperationType() { return operation; } // \ru Дать оболочку \en Get a shell 
  virtual void        SetOperationType( OperationType t ) { operation=t; } // \ru Заменить оболочку \en Replace a shell 

  virtual bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = NULL ); // \ru Построение \en Construction 
  /** \} */

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void      operator = ( const MbSimpleCreator & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSimpleCreator )
}; // MbSimpleCreator


//------------------------------------------------------------------------------
/** \brief \ru Строитель вывернутого "наизнанку" тела.
           \en Constructor of a reversed solid. \~
  \details \ru Строитель вывернутого "наизнанку" тела. \n
           \en Constructor of a reversed solid. \n \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbReverseCreator : public MbCreator {

public :
  /// \ru Конструктор. \en Constructor. \~
  MbReverseCreator( const MbSNameMaker & n );

private :
  MbReverseCreator( const MbReverseCreator &, MbRegDuplicate * iReg ); // \ru Конструктор копирования с регистратором \en Copy-constructor with the registrator 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbReverseCreator( const MbReverseCreator & ); 
public :
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbReverseCreator();

  /** \ru \name Общие функции строителя оболочки.
      \en \name Common functions of the shell creator.
      \{ */
  virtual MbeCreatorType IsA() const; // \ru Тип элемента \en Type of element 
  virtual MbCreator & Duplicate( MbRegDuplicate * = NULL ) const ; // \ru Сделать копию \en Create a copy 
  virtual void        Transform( const MbMatrix3D &, MbRegTransform * = NULL );             // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void        Move     ( const MbVector3D &, MbRegTransform * = NULL );             // \ru Сдвиг \en Translation 
  virtual void        Rotate   ( const MbAxis3D &, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate about an axis 

  virtual MbePrompt   GetPropertyName(); // \ru Выдать заголовок свойства объекта \en Get a name of object property 
  virtual void        GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void        SetProperties( const MbProperties & properties ); // \ru Записать свойства объекта \en Set properties of the object 

  virtual bool        IsSame( const MbCreator & other, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  virtual bool        SetEqual( const MbCreator & );       // \ru Сделать равным \en Make equal 
  virtual bool        IsSimilar ( const MbCreator & ) const; // \ru Являются ли объекты подобными \en Whether the objects are similar 

  virtual bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> * items = NULL ); // \ru Построение \en Construction 
  /** \} */

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void        operator = ( const MbReverseCreator & );

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbReverseCreator )
}; // MbReverseCreator


#endif // __CR_SIMPLE_CREATOR_H
