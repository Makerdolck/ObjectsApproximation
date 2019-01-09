////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Класс ViewObject3D содержит GeometryRep3D + bounding box.
         \en ViewObject3D class contains GeometryRep3D + bounding box. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_VIEWOBJECT3D_H
#define __VSN_VIEWOBJECT3D_H

#include <set>

#include <mb_cube.h>
#include <mb_matrix3d.h>

#include "vsn_object.h"
#include "vsn_renderstate.h"
#include "vsn_namespace.h"
#include "vsn_globalflags.h"
#include "vsn_geometryrep3d.h"

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE
class Viewport;
class ViewObject3DPrivate;


//------------------------------------------------------------------------------
/** \brief \ru Класс ViewObject3D содержит GeometryRep3D + bounding box.
           \en ViewObject3D class contains GeometryRep3D + bounding box. \~
  \details \ru Класс ViewObject3D содержит GeometryRep3D + bounding box. \n
           \en ViewObject3D class contains GeometryRep3D + bounding box. \n \~
  \ingroup NO GROUP
*/
// ---
class VSN_CLASS ViewObject3D : public Object
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  ViewObject3D();
  /// \ru Специальный конструктор с инициализацией геометрии. \en Special constructor with geometry initialization.
  ViewObject3D( GeomObject * );
  /// \ru Специальный конструктор с инициализацией геометрии и уникальным идентификатором. \en Special constructor with geometry initialization and the unique identifier.
  ViewObject3D( GeomObject *, uint id );
  /// \ru Специальный конструктор с инициализацией геометрического представления. \en Special constructor with initialization of geometry representation.
  ViewObject3D( const GeometryRep3D & );
  /// \ru Специальный конструктор с инициализацией геометрического представления и идентификатором. \en Special constructor with initialization of geometry representation and the unique identifier.
  ViewObject3D( const GeometryRep3D &, uint id );
  /// \ru Конструктор копирования. \en Copy constructor.
  ViewObject3D( const ViewObject3D & );
  /// \ru Деструктор. \en Destructor. 
  virtual ~ViewObject3D();

public:
  /// \ru Прозрачен ли весь объект. \en Checks if the whole object is transparent.
  bool IsTransparent() const;
  /// \ru Есть ли хотя бы одна прозрачная часть объекта. \en Checks if transparent parts of object exist (at least one part).
  bool IsExistTransparentMaterials() const;

  /// \ru Является ли объект пустым. \en Checks if object is empty.
  bool IsEmpty() const;
  /// \ru Выбран ли объект. \en Checks if object is selected.
  bool IsSelected() const;

  /// \ru Получить количество частей геометрии объекта. \en Returns count of object geometry parts.
  size_t GetGeometryCount() const;
  /// \ru Вернуть геометрическую часть по индексу. \en Returns geometry part by index.
  GeomObject * GetGeometryByIndex( size_t index ) const;

  /// \ru Вернуть пользовательский идентификатор и тип примитива по внутреннему идентификатору. \en Returns the user identifier and primitive type by internal identifier.
  C3DVision::IdType GetUserIdPrimitiveById( int indexBody, uint id ) const;

  /// \ru Получить геометрическое представление объекта. \en Returns geometry representation of the object.
  const GeometryRep3D & GetRepresentation() const;

  /// \ru Получить копию габаритного куба объекта. \en Returns copy of object bounding box.
  MbCube GetBoundingBox();

  /// \ru Является ли габаритный куб действительным. \en Checks if bounding box is valid.
  bool IsBoundingBoxValid() const;

  /// \ru Вернуть абсолютную матрицу трансформации. \en Returns absolute transformation matrix.
  const MbMatrix3D & GetMatrix() const;

  /// \ru Дублировать объект полностью. \en Fully duplicates object.
  ViewObject3D FullDuplicate() const;
  /// \ru Дублировать объект. \en Duplicates object.
  ViewObject3D Duplicate();

  /** \brief \ru Вернуть режим отрисовки объекта.
             \en Returns object rendering mode. \~    
    \details \ru Вернуть режим отрисовки объекта. Режим может быть: GL_POINT, GL_LINE или GL_FILL. \n
             \en Returns object rendering mode. It can be GL_POINT, GL_LINE or GL_FILL. \n \~
  */
  GLenum GetPolygonMode() const;

  /** \brief \ru Установить режим отображения полигонов в выбранном стиле.
             \en Sets polygon mapping mode in selected style. \~    
    \details \ru Установить режим отображения полигонов в выбранном стиле. 
                 Режим сетки может быть: GL_FRONT_AND_BACK, GL_FRONT или GL_BACK.
                 Стиль может быть: GL_POINT, GL_LINE или GL_FILL. \n
             \en Sets polygon mapping mode in selected style.
                 Mesh mode can be GL_FRONT_AND_BACK, GL_FRONT or GL_BACK. 
                 The style can be GL_POINT, GL_LINE or GL_FILL. \n \~
  */
  void SetPolygonMode( GLenum face, GLenum mode );

  /// \ru Вернуть указатель на состояние отрисовки. \en Returns pointer to rendering state.
  RenderState * GetRenderState();
  /// \ru Установить состояние отрисовки для этого объекта. \en Sets rendering state for this object.
  void SetStateRender( const RenderState & );

  /// \ru Является ли объект видимым. \en Checks if object is visible.
  bool IsVisible() const;
  /// \ru Установить видимость объекту. \en Sets visibility for object.
  void SetVisible( bool bVisible );

  /// \ru Вернуть состояние видимости объекта. \en Returns object visibility state.
  C3DVision::VisibleState GetVisibleState() const;
  /// \ru Установить состояние видимости объекта. \en Sets object visibility state.
  bool SetStateVisible( C3DVision::VisibleState state );

  /// \ru Является ли видимым объект, полученный по индексу. \en Checks if object received by index is visible.
  bool IsGeomVisibleByIndex(size_t index) const;
  /// \ru Установить видимость геометрии внутри этого объекта по индексу. \en Sets geometry visibility inside the object by index.
  void SetVisibleGeomByIndex(size_t index, bool bVisible);

  /// \ru Получить количество граней. \en Returns face count.
  size_t GetFaceCount() const;
  /// \ru Получить количество вершин. \en Returns vertex count.
  size_t GetVertexCount() const;

  /// \ru Вернуть количество материалов. \en Returns material count.
  size_t GetMaterialCount() const;
  /// \ru Вернуть массив материалов. \en Returns material array.
  std::set<Material *> GetMaterials() const;

  /// \ru Вернуть значение LOD по умолчанию. \en Returns the default value of LOD.
  int GetDefaultLevelOfDetail() const;
  /// \ru Установить значение LOD по умолчанию этому объекту. \en Sets the default value of LOD for this object.
  void SetDefaultLevelOfDetail( int value );

  /// \ru Вернуть представление объекта. \en Returns object representation.
  GeometryRep3D GetRepObject() const;
  /// \ru Вернуть количество тел в объекте. \en Returns body count in the object.
  size_t GetBodyCount() const;

  /// \ru Вернуть глобальное значение уровня детализации. \en Returns the global value of LOD.
  static int GetGlobalDefaultLevelOfDetail();
  /// \ru Установить глобальное значение уровня детализации. \en Sets the global value of LOD.
  static void SetGlobalDefaultLevelOfDetail( int value );

public:
  /// \ru Добавить новую геометрию. \en Adds new geometry.
  bool AddGeometry( GeomObject * );
  /// \ru Удалить пустую геометрию. \en Deletes empty geometry.
  void DeleteEmptyGeometry();
  /// \ru Инвертировать нормали геометрии. \en Reverses geometry normals.
  void ReverseNormalsOfGeometry();

  /// \ru Преобразовать объект по параметрам. \en Transforms object by parameters.
  ViewObject3D & Translate( double px, double py, double pz );
  /// \ru Преобразовать объект по вектору. \en Transforms object by vector.
  ViewObject3D & Translate( const MbVector3D& );
  /// \ru Преобразовать объект по указанной матрице. \en Transforms object by specified matrix.
  ViewObject3D & MultMatrix( const MbMatrix3D& );
  /// \ru Установить абсолютную матрицу по матрице. \en Sets absolute matrix by specified matrix.
  ViewObject3D & SetMatrix( const MbMatrix3D& );

  /// \ru Сбросить абсолютную матрицу до единичной. \en Resets absolute matrix to identity matrix.
  ViewObject3D & ResetMatrixToIdentityMatrix();

  /// \ru Использовать полную матрицу с учетом абсолютной матрицы объекта. \en Uses full matrix with given absolute matrix of the object.
  void SetUseFullMatrix( bool bUse );
  /// \ru Установить полную матрицу с учетом абсолютной матрицы объекта. \en Sets full matrix with given absolute matrix of the object.
  void SetFullMatrix( const MbMatrix3D & mx );
  /// \ru Сбросить полную матрицу до единичной. \en Resets full matrix to identity matrix.
  void ResetFullMatrixToIdentityMatrix();

  /// \ru Установить признак выбранного объекта в геометрии. \en Sets flag of selected object in geometry.
  void SetSelect( bool primitive );
  /// \ru Снять выбор объекта в геометрии. \en Unselects object within geometry.
  void Unselect();

  /// \ru Установить идентификатор объекту. \en Sets identifier for object.
  void SetId( uint id );

  /// \ru Установить признак использования VBO. \en Sets flag of Vertex Buffer Object (VBO) usage.
  void SetUseVertexBufferObjects( bool usage );

public:
  /// \ru Отрисовать представления этого объекта в заданном режиме. \en Renders representation of the object in set mode.
  void OGLDrawGivenMode( C3DVision::RenderMode renderingMode = C3DVision::ShadedMode, bool useLod = false, Viewport* pView = V_NULL );
  /// \ru Отрисовка тела в режиме выбора. \en Renders body in selection mode.
  void OGLDrawBodySelectionMode();
  /// \ru Отрисовать примитивы объекта в режиме выбора по идентификатору тела и вернуть индекс тела. \en Renders object primitives in selection mode by body identifier and return body index.
  size_t OGLDrawPrimitiveSelectionMode( uint idBody );

  /// \ru Оператор копирования. \en Copy operator.
  ViewObject3D & operator = ( const ViewObject3D & );

private:
  friend class ViewContainer3DPrivate;
  VSN_DECLARE_PRIVATE(ViewObject3D)
};

VSN_END_NAMESPACE

#endif /* __VSN_VIEWOBJECT3D_H */
