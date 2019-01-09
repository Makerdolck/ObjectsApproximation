////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Абстрактный базовый класс для отрисовочной геометрии.
         \en Abstract base class for rendering geometry. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GEOMOBJECT_H
#define __VSN_GEOMOBJECT_H

#include <set>
#include <list>
#include <unordered_map>

#include "vsn_object.h"
#include "vsn_color.h"
#include "vsn_namespace.h"

#include "vsn_global.h"

class MbCube;
class MbMatrix3D;
class MbCartPoint3D;

VSN_BEGIN_NAMESPACE

class Material;
class RenderState;
class GeomObjectPrivate;
//------------------------------------------------------------------------------
/** \brief  \ru Абстрактный базовый класс для отрисовочной геометрии.
            \en Abstract base class for rendering geometry. \~
  \details  \ru Абстрактный базовый класс для отрисовочной геометрии:
                - содержит основной атрибут, который является таблицой материалов.
                - содержит ряд виртуальных функций, которые позволяют отрисовать геометрию.
                - содержит механизм отрисовки детализации заданного уровня. \n.
            \en Abstract base class for rendering geometry:
                - contains the main attribute that is material table.
                - contains virtual function row to provide rendering geometry.
                - contains rendering mechanism of set level of detail (LOD). \n \~
    \ingroup Vision_Geometry
*/
// ---
class VSN_CLASS GeomObject : public Object
{
public:
  /// \ru Конструктор. \en Constructor.
  GeomObject( const std::wstring& name, bool type );
  /// \ru Конструктор копирования. \en Copy constructor.
  GeomObject( const GeomObject& other );
  /// \ru Деструктор. \en Destructor.
  virtual ~GeomObject();

public:
  /// \ru Действительна ли геометрия. \en Checks if geometry is valid.
  bool IsValid() const;

  /// \ru Задан ли материал. \en Checks if material is set.
  bool HasMaterial() const;

  /// \ru Получить первый материал из таблицы. \en Returns the first material from the table.
  Material* GetHeadMaterial() const;

  /// \ru Получить число материалов из таблицы. \en Returns material count from the table.
  size_t GetMaterialCount() const;

  /// \ru Получить указатель, соответствующий идентификатору. \en Returns the pointer corresponding to the identifier.
  Material* GetMaterialById( uint id ) const;

  /// \ru Получить таблицу материалов. \en Returns material table.
  std::set<Material*> GetMaterialSet() const;

  /// \ru Получить список идентификаторов материалов. \en Returns material identifier list.
  std::list<uint> GetMaterialIds() const;

  /// \ru Получить данные примитива, соответствующего идентификатору. \en Returns data of the primitive corresponding to the identifier.
  IdType GetUserIdPrimitiveById( uint id ) const;

  /// \ru Существует ли в таблице материал с заданным идентификатором. \en Checks if the material with set identifier exists in the table.
  bool HasMaterialById( uint id ) const;

  /// \ru Получить габаритный куб. \en Returns bounding box.
  virtual const MbCube& GetBoundingBox() = 0;

  /// \ru Получить габаритный куб каркаса. \en Returns wireframe bounding box.
  const MbCube& GetWireBoundingBox();

  /// \ru Является ли габаритный куб допустимым. \en Checks if bounding box is valid.
  bool IsBoundingBoxValid() const;

  /// \ru Клонировать всю геометрию. \en Duplicates all geometry.
  virtual GeomObject* Duplicate() const = 0;

  /// \ru Прозрачна ли геометрия. \en Checks if geometry is transparent.
  bool IsTransparent() const;

  /// \ru Имеется ли прозрачный материал в таблице. \en Checks if transparent material exists in the table.
  bool IsExistTransparentMaterials() const;

  /// \ru Задан ли цвет каждой вершины. \en Checks if color of each vertex is set.
  bool IsColorEachVertex() const;

  /// \ru Установить флаг назначения цвета для каждой вершины. \en Sets flag of setting color for each vertex.
  void SetColorEachVertex( bool bColorEachVertex );

  /// \ru Является ли геометрия каркасной. \en Checks if geometry is wireframe.
  bool IsWireframe() const;

  /// \ru Получить число граней. \en Returns face count.
  virtual size_t GetFaceCount( size_t indexLod = 0 ) const;

  /// \ru Получить число вершин. \en Returns vertex count.
  virtual size_t GetVertexCount() const;

  /// \ru Получить толщину линии. \en Returns line width.
  double GetLineWidth() const;

  /// \ru Задать толщину линии. \en Sets line width.
  void SetLineWidth( double lineWidth );

  /// \ru Получить цвет каркаса. \en Returns wireframe color.
  Color GetWireframeColor() const;

  /// \ru Задать цвет каркаса. \en Sets wireframe color.
  void SetWireframeColor( const Color& color );

  /// \ru Пуст ли каркас. \en Checks if wireframe is empty.
  bool IsWireframeEmpty() const;

  /// \ru Получить позиции вершин. \en Returns vertex positions.
  std::vector<GLfloat> GetWireFrameVertexPositions() const;

  /// \ru Получить число полилиний. \en Returns polyline count.
  int GetPolylineCount() const;

  /// \ru Получить смещение полилинии с заданным индексом в контейнере. \en Returns polyline offset with set index in the container.
  GLuint GetPolylineOffset( int index ) const;

  /// \ru Получить размер полилинии с данным индексом. \en Returns size of polyline with given index.
  GLsizei GetPolylineSize( int index ) const;

  /// \ru Получить объем геометрии. \en Returns geometry volume.
  virtual double GetVolume();

  /// \ru Используется ли вершинный буфер. \en Checks if vertex buffer is used.
  bool IsUsedVertexBuffer() const;

public:
  /// \ru Удалить всю геометрию. \en Deletes all geometry.
  virtual void Clear();

  /// \ru Заменить основной материал. \en Replaces the main material.
  virtual void ReplaceMainMaterial(Material*);

  /// \ru Добавить материал в таблицу. \en Adds material to the table.
  void AddMaterial( Material* pMaterial );

  /// \ru Обновить число прозрачных материалов после операций с таблицей. \en Updates transparent material count after table operations.
  void UpdateTransparentMaterialCount();

  /// \ru Инвертировать направление всех нормалей. \en Inverts direction of all vertex normals.
  virtual void InvertNormals();

  ///\ ru Задать уровень детализации в диапазоне от 0 до 100. \en Sets LOD in the range from 0 to 100.
  virtual void SetLevelDetail( int value );

  /// \ru Добавить группу вершин и вернуть её идентификатор. \en Adds vertex group and returns its identifier.
  uint AddVerticesGroup( const std::vector<GLfloat>& vector );

  /// \ru Добавить группу точек в геометрию и вернуть её идентификатор. \en Adds point group to geometry and returns its identifier.
  uint AddPeaksVerticesGroup( const std::vector<GLfloat>& vector );

  /// \ru Добавить группу рёбер в геометрию и вернуть её идентификатор. \en Adds edge group to geometry and returns its identifier.
  uint AddPeaksEdgesGroup( const std::vector<GLfloat>& vector );

  /// \ru Задать, будет ли использоваться глобальный идентификатор. \en Sets if the global identifier will be used.
  void SetUseGlobalIdentifier( int global );

  /// \ru Скопировать буфер вершин в память. \en Copies vertex buffer into memory.
  virtual void CopyVertexBufferToClientSide();

  /// \ru Создать буфер вершин. \en Creates vertex buffer.
  virtual void ReleaseVertexBufferClientSide( bool update = false );

  /// \ru Задать, будет ли использоваться буфер вершин. \en Sets if vertex buffer will be used.
  virtual void SetUseVertexBufferObjects(bool usage);

public:  
  /// \ru Загрузить текстуры. \en Loads textures.
  virtual void OGLLoadTexture();

  /// \ru Выполнить отрисовку. \en Performs rendering.
  void render( const RenderState& );

  /// \ru Оператор присваивания. \en Assignment operator.
  GeomObject& operator = (const GeomObject& sourceGeom);

protected:
  /// \ru Выполнить отрисовку. \en  Performs rendering.
  virtual void OGLDraw(const RenderState&) {}

  /// \ru Выполнить отрисовку. \en  Performs rendering.
  virtual void OGLInitGeom( const RenderState& );

  /// \ru Нарисовать каркас в одном из режимов: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP GL_LINES. \en Renders wireframe in one of the modes: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP GL_LINES.
  void OGLDrawWireframe( const RenderState&, GLenum mode );

protected:
  /// \ru Очистить всю каркасную геометрию. \en Clears all wireframe.
  void ClearWireframe();

protected:
  GeomObjectPrivate& GetGeomObjectPrivate();

protected:
  MbCube* m_pBoundingBox; /// \ru Габаритник. \en Bounding box.

private:
  VSN_DECLARE_PRIVATE( GeomObject )
};
VSN_END_NAMESPACE

#endif /* __VSN_GEOMOBJECT_H */
