////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Точечный каркас.
         \en Point-frame. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __POINT_FRAME_H
#define __POINT_FRAME_H


#include <templ_s_array.h>
#include <templ_rp_array.h>
#include <mb_cart_point3d.h>
#include <space_item.h>
#include <topology.h>
#include <model_item.h>
#include <math_define.h>
#include <vector>


//------------------------------------------------------------------------------
/** \brief \ru Точечный каркас.
           \en Point-frame. \~
  \details \ru Точечный каркас состоит из множества декартовых точек, представленных в виде вершин MbVertex. \n
           \en The point frame consists of set of Cartesian points represented as MbVertex vertices. \n \~
  \ingroup Model_Items
*/
// ---
class MATH_CLASS MbPointFrame : public MbItem {
protected:
  std::vector<MbVertex *> vertices; ///< \ru Множество вершин каркаса. \en Set frame vertices. 

protected:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  explicit MbPointFrame( const MbPointFrame &, MbRegDuplicate * );
public:
  /// \ru Конструктор. \en Constructor. 
  MbPointFrame();
  /// \ru Конструктор по вершине и флагу использования этого объекта, а не его копии. \en Constructor by vertex and by flag of use of this object instead of its copy. 
  MbPointFrame( const MbVertex &, bool same );
  /// \ru Конструктор по точке. \en Constructor by point. 
  MbPointFrame( const MbCartPoint3D & );
  /// \ru Конструктор по координатам. \en Constructor by coordinates. 
  MbPointFrame( double x, double y, double z );
  /// \ru Конструктор по массиву вершин и флагу использования этих объектов, а не их копий. \en Constructor by array of vertices and by flag of use of these objects instead of their copies. 
  MbPointFrame( const RPArray<MbVertex> &, bool same );
  /// \ru Конструктор по массиву точек. \en Constructor by array of points. 
  template <class Points>
  MbPointFrame( const Points & pnts )
    : MbItem  ()
    , vertices()
  {
    for ( size_t k = 0, kcnt = pnts.size(); k < kcnt; k++ )
      AddVertex( pnts[k] );
  }

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbPointFrame();

public:
  VISITING_CLASS( MbPointFrame ); 

  // \ru Общие функции геометрического объекта \en Common functions of a geometric object  
  virtual MbeSpaceType  IsA()  const; // \ru Тип объекта. \en A type of an object. 
  virtual MbeSpaceType  Type() const; // \ru Групповой тип объекта. \en Group type of object. 
  virtual MbSpaceItem & Duplicate( MbRegDuplicate * = NULL ) const; // \ru Создать копию. \en Create a copy. 
  virtual void    Transform( const MbMatrix3D &, MbRegTransform * = NULL ); // \ru Преобразовать согласно матрице. \en Transform according to the matrix.  
  virtual void    Move     ( const MbVector3D &, MbRegTransform * = NULL ); // \ru Сдвинуть вдоль вектора. \en Translate along a vector. 
  virtual void    Rotate   ( const MbAxis3D   &, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси. \en Rotate about an axis. 
  virtual bool    IsSame   ( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const; // \ru Являются ли объекты равными? \en Are the objects equal? 
  virtual bool    IsSimilar( const MbSpaceItem & init ) const; // \ru Являются ли объекты подобными? \en Are the objects similar? 
  virtual bool    SetEqual ( const MbSpaceItem & init );   // \ru Сделать объекты равным. \en Make the objects equal. 
  virtual double  DistanceToPoint ( const MbCartPoint3D & ) const; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  virtual void    AddYourGabaritTo( MbCube & r ) const; // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  virtual void    CalculateLocalGabarit( const MbMatrix3D & into, MbCube & cube ) const; // \ru Рассчитать габарит в локальной системы координат. \en Calculate bounding box in the local coordinate system. 
  virtual void    CalculateWire( const MbStepData & stepData, MbMesh & mesh ) const; // \ru Создать полигональную копию. \en A polygonal copy. 

  virtual MbProperty & CreateProperty( MbePrompt n ) const; // \ru Создать собственное свойство. \en Create a custom property. 
  virtual void    GetProperties( MbProperties & ); // \ru Выдать свойства объекта. \en Get properties of the object. 
  virtual void    SetProperties( const MbProperties & ); // \ru Установить свойства объекта. \en Set properties of the object. 
  virtual void    GetBasisItems  ( RPArray<MbSpaceItem>   & ); // \ru Дать базовые объекты. \en Get the basis objects. 
  virtual void    GetBasisPoints( MbControlData3D & ) const; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  virtual void    SetBasisPoints( const MbControlData3D & ); // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  // \ru Перестроить объект по журналу построения. \en Reconstruct object according to the history tree. 
  virtual bool    RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd );

  // \ru Создать полигональный объект - упрощенную копию данного объекта. \en Create a polygonal object - a simplified copy of the given object. 
  virtual MbItem * CreateMesh( const MbStepData & stepData, const MbFormNote & note, MbRegDuplicate * iReg ) const; 

  /** \ru \name Общие функции каркаса.
      \en \name Common functions of a frame.
      \{ */

          /// \ru Выдать количество вершин. \en Get count of vertices. 
          size_t  GetVerticesCount() const { return vertices.size(); } 
          /// \ru Получить вершину по индексу. \en Get vertex by an index. 
    const MbVertex * GetVertex( size_t k ) const { return ((k < vertices.size()) ? vertices[k] : NULL ); }
          /// \ru Получить вершину по индексу для модификации. \en Get vertex by an index for modification. 
          MbVertex * GetVertex( size_t k )       { return ((k < vertices.size()) ? vertices[k] : NULL ); } 
          /// \ru Добавить вершину по точке. \en Add vertex by point. 
          void    AddVertex( const MbCartPoint3D & ); 
          /// \ru Добавить вершину или ее копию, что определяется флагом использования самого объекта. \en Add vertex or its copy (defined by flag of use of object). 
          void    AddVertex( const MbVertex &, bool same ); 
          /// \ru Вставить вершину по номеру и точке. \en Insert vertex by index and point. 
          void    InsertVertex( size_t k, const MbCartPoint3D & );
          /// \ru Вставить по номеру вершину или ее копию, что определяется флагом использования самого объекта. \en Insert vertex or its copy (defined by flag of use of object) by index. 
          void    InsertVertex( size_t k, const MbVertex &, bool same );
          /// \ru Удалить вершину с заданным номером. \en Delete vertex by given index. 
          bool    DeleteVertex( size_t k );
          /// \ru Удалить все вершины. \en Delete all vertices. 
          void    DeleteVertices(); 

          /// \ru Дать декартову точку начальной вершины. \en Get Cartesian point of the first vertex. 
          bool    GetCartPoint( MbCartPoint3D & ) const; 
          /// \ru Дать декартову точку вершины с заданным номером. \en Get Cartesian point of vertex by a given index. 
          bool    GetCartPoint( size_t k, MbCartPoint3D & ) const; 
          /// \ru Проверка на равенство с каркасом. \en Check for equality with frame. 
          bool    operator == ( const MbPointFrame & ) const;
          /// \ru Проверка на неравенство с каркасом. \en Check for inequality with frame. 
          bool    operator != ( const MbPointFrame & ) const;
  /** \} */
          /// \ru Установить заданный флаг измененности для всех вершин. \en Set flag of changes for all vertices. 
          void    SetOwnChangedThrough( MbeChangedType );

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbPointFrame )
OBVIOUS_PRIVATE_COPY( MbPointFrame )
};


#endif // __POINT_FRAME_H
