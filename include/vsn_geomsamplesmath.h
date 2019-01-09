////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Обертка тела.
         \en Solid body wrapper. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GEOMSAMPLESMATH_H
#define __VSN_GEOMSAMPLESMATH_H

#include "vsn_geommeshobject.h"
#include "vsn_global.h"

class MbMesh;
class MbAttributeContainer;

VSN_BEGIN_NAMESPACE

class GeometryRep3D;
//------------------------------------------------------------------------------
/** \brief \ru Обертка тела.
           \en Solid body wrapper. \~
  \details \ru Класс SolidObject представляет тело для отрисовки. \n.
           \en SolidObject class represents body for rendering. \n \~
  \ingroup NO GROUP
*/
// ---
class VSN_CLASS SolidObject : public GeomMeshObject
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  SolidObject( MbMesh * pMeshItem );
  /// \ru Конструктор копирования. \en Copy constructor.
  SolidObject( const SolidObject & other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~SolidObject();

public:
  /// \ru Cоздать представление тела. \en Creates body representation. 
  static GeometryRep3D * CreateGeometryRep3D( MbMesh * pMeshItem );  

public:
  /// \ru Обновить полигональную модель объекта. \en Updates object mesh. 
  void UpdateMesh( MbMesh* pMeshItem );

public:
  /// \ru Получить копию геометрического объекта. \en Duplicates geometry object. 
  virtual GeomObject * Duplicate() const;
  /// \ru Получить габаритный куб объекта. \en Returns object bounding box. 
  virtual const MbCube & GetBoundingBox();

private:
  /// \ru Отрисовка тела. \en Renders solid body. 
  virtual void OGLDraw( const RenderState & );

private:
  /// \ru Построить все тело со всеми полигонами для отрисовки. \en Builds the whole body with all polygons for rendering. 
  void BuildBodySolid();
  /// \ru Построить полигонное представление для отрисовки. \en Builds mesh representation of solid body for rendering. 
  void BuildMeshSolid();
  /// \ru Построить каркасное представление для отрисовки. \en Builds wireframe representation of solid body for rendering. 
  void BuildWireframeSolid();
  /// \ru Построить вершинное представление для выбора. \en Builds vertex representation of solid body for selection. 
  void BuildSelectionVertexSolid();
  /// \ru Построить каркасное представление для выбора. \en Builds wireframe representation of solid body for selection. 
  void BuildSelectionWireframeSolid();
  /// \ru Cоздать материал по атрибутам. \en Creates material by attributes. 
  Material * CreateMaterial( const MbAttributeContainer &, bool bFace/*, COLORREF desColor*/ );

private:
  /// \ru Указатель на математическое представление (указатель может быть и V_NULL). \en Pointer to math representation. Pointer can be V_NULL.  
  MbMesh * m_pMeshItem;
};

VSN_END_NAMESPACE

#endif /* __VSN_GEOMSAMPLESMATH_H */
