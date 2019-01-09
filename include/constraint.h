//////////////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Геометрическое ограничение.
         \en Geometric constraint. \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONSTRAINT_H
#define __CONSTRAINT_H


#include <assembly.h>
#include <vector>


struct           CNodeIterator;


//----------------------------------------------------------------------------------------
/// \ru Ссылка на объект модели. \en Reference to the model object (item). \~
//---
class MtItemReference 
{
private:
  uint32             hash; // Hash code of the path from the root to the item.
  SPtr<const MbItem> item; // The referenced sub-item contained in the sub-tree.
  const MbAssembly * root; // Root of the model sub-tree.

public:
  MtItemReference();
  MtItemReference( const MbItem * );
  MtItemReference( const MtItemReference & );
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
  MtItemReference & operator = ( const MtItemReference & );

public:
  const MbItem *  Item() const { return item; }
  /** \brief \ru Получить непосредственный объект сборки, содержащий ссылочный объект.
             \en Get immediate object of the assembly containing the reference object.
      \param trans - \ru Матрица ссылочного объекта в системе координат непосредственного объекта сборки.
                   - \en Matrix from the reference object to the sub-item of the assembly. \~
  */
  const MbItem *  SubItemOf( const MbAssembly *, MbMatrix3D & trans ) const;
  /// \ru Оператор равенства объектов. \en Objects equality operator. \~
  inline bool     operator == ( const MtItemReference & r ) const {
                    if ( (item == r.item) && (hash == r.hash) )
                      return true;
                    if ( (hash == r.hash) && (root == r.root) ) 
                      return true;
                    return false;
                  }

public: // Serializing into a file format
  KNOWN_OBJECTS_RW_REF_OPERATORS( MtItemReference );
}; // MtItemReference 


//----------------------------------------------------------------------------------------
/** \brief \ru Аргумент геометрического ограничения.
           \en Argument of geometric constraint. \~
  \details \ru Этот класс представляет аргумент ограничений, назначаемых между
               объектами геометрической модели сборки.
           \en This class represents an argument of constraints within geometric
               model which provided assembly unit.\~
  \ingroup Model_Items
*/
//---
class MATH_CLASS MtGeomArgument
{
  SimpleName            propName; ///< \ru Имя связываемого объекта. \en Name of a connected object.
  SPtr<const MbRefItem> propItem; ///< \ru Указатель на объект, непосредственно выбранный для связи. \en The pointer to geom entity which constraints are connected to.        
  MtItemReference       hostRef;  ///< \ru Объект сборки или подсборки, содержащий объект связи с ограничением. \en Assembly object that hosts geom entity connected to constraint.

public:
  MtGeomArgument( const MbRefItem * p, const MbItem * h );
  MtGeomArgument( const MtGeomArgument & );
  MtGeomArgument() : propItem(NULL), hostRef(), propName( UNDEFINED_SNAME ) {}

public:
  /// \ru Объект геометрической модели, владеющий аргументом. \en Geometry model object which is a host of an argument.
  const MbItem *  HostItem() const { return hostRef.Item(); }
  const MbItem *  SubItemOf( const MbAssembly * model, MbMatrix3D & t ) const { return hostRef.SubItemOf( model,t); }
  /// \ru Аргумент ограничения, заданный в ЛСК хозяина. \en Geometric constraint argument given in the host's LCS.
  const MbRefItem * PropItem() const { return propItem; }
  /// \ru Выдать значение геометрии аргумента, заданное в ЛСК хозяина. \en Get geometric value of argument given in the host LCS.
  MtGeomVariant   PropGeom() const;
  /// \ru Выдать хэш-имя объекта. \en Get a hash name of the object.
  SimpleName      PropName() const;
  /// \ru Оператор равенства объектов. \en Objects equality operator. \~
  inline bool     operator == ( const MtGeomArgument & r ) const {
                    if ( propItem != r.propItem )
                      return false;
                    if ( propName != r.propName )
                      return false;
                    return hostRef == r.hostRef;
                  }

public: // Serializing into a file format
  KNOWN_OBJECTS_RW_REF_OPERATORS( MtGeomArgument );

public:
  MtGeomArgument & operator = ( const MtGeomArgument & arg )
  { 
    propItem = arg.propItem;
    hostRef  = arg.hostRef;
    return *this; 
  }
}; // MtGeomArgument


//----------------------------------------------------------------------------------------
/** \brief \ru Геометрическое ограничение.
           \en Geometric constraint. \~
  \details \ru Этот класс представляет все виды ограничений, включая геометрические и 
               размерные отношения между объектами модели.
           \en This class represents all kinds of constraints of assembly, including 
               geometrical and dimensional relationships between the model objects. \~
  \ingroup Model_Items
*/
//---
class MATH_CLASS MtGeomConstraint
{
private:
  const ItConstraintItem * m_cItem; ///< Указатель на реализацию. \ru . \en Pointer to implementation.

public:
  MtGeomConstraint( const MtGeomConstraint & );
 ~MtGeomConstraint(); 

public:
  /// \ru Возвращает true, если ограничение не действительно. \en Return true iff the constraint is invalid.
  bool        IsNull() const { return m_cItem == NULL; }
  /// \ru Тип сопряжения (геометрического ограничения). \en Type of geometric constraint. 
  MtMateType  ConstraintType() const;
  /// \ru Текущее значение размера. \en Current value of the dimension. 
  double      DimValue() const;

private:
  // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию \en Declaration without implementation of the assignment operator to prevent an assignment by default 
  MtGeomConstraint & operator = (const MtGeomConstraint & );

protected:  
  friend class MbConstraintSystem;  
  friend class MtConstraintIter;  

  MtGeomConstraint( const ItConstraintItem * );  
  const ItConstraintItem * CItem() const { return m_cItem; }
}; // MtGeomConstraint


//----------------------------------------------------------------------------------------
/// \ru Итератор обходящий ограничения сборки. \en Iterator traversing assembly constraints.
//---
class MATH_CLASS MtConstraintIter
{
public: 
  MtConstraintIter();
  MtConstraintIter( const MtConstraintIter & );
  MtConstraintIter & operator = ( const MtConstraintIter & );
 ~MtConstraintIter();

public:
  MtGeomConstraint          Get() const;
  const MtConstraintIter &  Next();
  bool                      EqualTo( const MtConstraintIter & ) const;

public:
  operator CNodeIterator&() { return *impl; }
  MtGeomConstraint operator*() const { return Get(); }
  // prefix operator
  const MtConstraintIter & operator++() { return Next(); }
  bool operator ==( const MtConstraintIter & iter ) const { return  EqualTo( iter ); }
  bool operator !=( const MtConstraintIter & iter ) const { return !EqualTo( iter ); }

private:
  CNodeIterator * impl;
}; // MtConstraintIter


//----------------------------------------------------------------------------------------
// The user-defined class that handles events related to solving the assembly
//---
struct ItAssemblyReactor
{  
  // Capture the reactor instance by the assembly for further work.
  virtual  void Capture( const MbAssembly * ) = 0;
  // Release this instance by the assembly, stop working with it.
  virtual  void Release() = 0;
  // This called when geometric solver failed to try for constraint satisfaction.
  virtual void  EvaluationFailed( const MbAssembly * ) const {}
  // The geometric solver found a new position of a constrained sub-object belonging the assembly.
  virtual void  PositionChanged( const MbAssembly *, const MbItem * /*subItem*/ ) const {}

protected:
  ~ItAssemblyReactor() {}
}; // ItAssemblyReactor


//----------------------------------------------------------------------------------------
// The user defined interface for import constraint system of an assembly from CAD application
//---
struct ItAssemblyImportData
{
  // Import a constraint system of the assembly
  virtual bool      ImportCSystem( const MbAssembly &, GCM_system & ) const = 0;
  // Get a descriptor of assembly sub-item which used in the constraint system.
  virtual MtGeomId  GeomId( const MbAssembly &, const MbItem * ) const = 0;

protected:
  ~ItAssemblyImportData() {}
}; //ItAssemblyImportData


#endif // __CONSTRAINT_H
