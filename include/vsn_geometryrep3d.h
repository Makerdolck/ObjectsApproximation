////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru 3D представление геометрии.
         \en 3D geometry representation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GEOMETRYREP3D_H
#define __VSN_GEOMETRYREP3D_H

#include <set>

#include <mb_cube.h>
#include "vsn_geomobject.h"
#include "vsn_abstractrep.h"

#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Material;
class GeometryRep3DPrivate;


//------------------------------------------------------------------------------
/** \brief \ru 3D представление геометрических данных.
           \en 3D geometry data representation. \~
    \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeometryRep3D : public AbstractRep
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~    
    \details \ru Конструктор по умолчанию.
             \en Default constructor. \~
  */
  GeometryRep3D();
  /** \brief \ru Конструктор по геометрическим параметрам объекта.
             \en Constructor by geometry parameters of the object. \~    
    \details \ru Конструктор по геометрическим параметрам объекта.
             \en Constructor by geometry parameters of the object. \~
  */
  GeometryRep3D( GeomObject* );
  /// \ru Конструктор копирования. \en Copy constructor.
  GeometryRep3D( const GeometryRep3D& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~GeometryRep3D();

public:
  /// \ru Сделать копию представления геометрических данных. \en Duplicates geometry data representation.
  virtual AbstractRep* Duplicate() const;

  /// \ru Создать представление с новыми геометрическими данными. \en Creates representation with new geometry data.
  virtual AbstractRep* CreateRepNewGeometryData() const;

  /// \ru Вернуть тип представления геометрических данных. \en Returns representation type of geometry data.
  virtual int GetRepType() const;

  /// \ru Вернуть геометрические данные по индексу. \en Returns geometry data by index.
  GeomObject* GetGeomAt( size_t index ) const;

  /// \ru Вернуть список геометрических данных. \en Returns geometry data list.
  const std::list<GeomObject*>* GetGeometryList() const;

  /// \ru Вернуть количество тел. \en Returns body count.
  size_t GetBodyCount() const;

  /// \ru Вернуть true, если массив представлений пуст. \en True if representation array is empty.
  virtual bool IsEmpty() const;

  /// \ru Вернуть true, если ограничивающий габарит правильный. \en True if bounding box is valid.
  bool IsBoundingBoxValid() const;

  /// \ru Вернуть габарит представления. \en Returns bounding box of representation.
  MbCube GetBoundingBox() const;

  /// \ru Вернуть true, если представление содержит геометрические данные. \en True if representation contains geometry data.
  bool IsGeometryInRep( GeomObject* pGeom );

  /// \ru Вернуть количество граней. \en Returns face count.
  size_t GetFaceCount() const;

  /// \ru Вернуть количество вершин. \en Returns vertex count.
  size_t GetVertexCount() const;

  /// \ru Вернуть количество материалов. \en Returns material count.
  size_t GetMaterialCount() const;

  /// \ru Вернуть набор материалов. \en Returns set of materials.
  std::set<Material*> GetMaterials() const;

  /// \ru Вернуть объем представления. \en Returns representation volume.
  double GetVolume() const;

public: 
  /// \ru Добавить геометрические данные в представление. \en Adds geometry data to the representation.
  void AddGeometry( GeomObject* pGeom );

  /// \ru Удалить пустые геометрические данные и обновить материалы. \en Deletes empty geometry data and update materials.
  void DeleteEmptyGeometry();

  /// \ru Сменить направление нормалей геометрии на противоположное. \en Reverses direction of all geometry normals.
  void ReverseNormals();

  /// \ru Заменить представление. \en Replaces the representation.
  virtual void ReplaceRep( AbstractRep* );

  /// \ru Заменить указанный материал. \en Replaces the specified material.
  void ReplaceMaterialById( uint id , Material* pMaterial );

  /// \ru Объединить представления. \en Unites representations.
  void UniteRep( const GeometryRep3D* );

  /// \ru Переложить геометрические данные из заданного представления. \en Shifts geometry data from the set representation.
  void ShiftGeometryFromRep(GeometryRep3D* pSource);

  /// \ru Сделать копию буфера вершин (VBO) на сторону клиента. \en Duplicates Vertex Buffer Object (VBO) to the client side.
  void DuplicateVertexBufferToClientSide();

  /// \ru Сформировать буфер вершин (VBO). \en Creates Vertex Buffer Object (VBO).
  void CreateVertexBufferClientSide( bool update = false );

  /// \ru Преобразовать вершины сетки по данной матрице. \en Transforms mesh vertices by given matrix.
  void TransformGeometriesByMatrix( const MbMatrix3D& matrix );

  /// \ru Установить признак использования буфера вершин. \en Sets flag of Vertex Buffer Object (VBO) usage.
  void SetUseVertexBufferObjects( bool usage );

public:
  /// \ru Оператор присваивания. \en Assignment operator.
  virtual GeometryRep3D& operator = ( const GeometryRep3D& );

private:
  VSN_DECLARE_PRIVATE(GeometryRep3D)
};
VSN_END_NAMESPACE


#endif /* __VSN_GEOMETRYREP3D_H */
