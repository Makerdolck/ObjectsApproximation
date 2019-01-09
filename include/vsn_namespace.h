////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru
         \en
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_NAMESPACE_H
#define __VSN_NAMESPACE_H

#include <mb_cart_point3d.h>
#include <mb_vector3d.h>
#include <system_types.h>

#include "vsn_opengl.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE

// буфер смещения используется для вершинного буфера
#define BUFFER_OFFSET(i) ((char*)V_NULL + (i))

enum ClippingPlaneId
{
  LeftPlane   = 0, // левая плоскость отсечения
  RightPlane  = 1, // правая плоскость отсечения
  TopPlane    = 2, // верхняя плоскость отсечения
  BottomPlane = 3, // нижняя плоскость отсечения
  NearPlane   = 4, // ближайшая плоскость отсечения
  FarPlane    = 5  // дальняя плоскость отсечения
};

enum FrustumClassification
{
  InFrustum = 0,
  IntersectFrustum = 1,
  OutFrustum  = 2
};

// тип освещения
enum LightTypes
{
  PointLight      = 0, /// точечные источники света испускают свет одинаково во всех направлениях, поэтому требуют только позицию без направления.
  DirectionLight  = 1, /// направленный свет моделирует параллельные лучи света от удаленного источника, следовательно, имеет направление, но не позицию.
  SpotLight       = 2  /// прожектор имитирует конус света от источника, что требует определения положения и направления. 
};

// стандартные типы ориентаций отображения
enum Orientation
{
  Front,    // Спереди  - Фронтальная плоскость
  Rear,     // Сзади
  Up,       // Сверху   - Горизонтальная плоскость
  Down,     // Снизу
  Left,     // Слева    - Профильная плоскость
  Right,    // Справа
  IsoXYZ,   // Изометрия XYZ
};
  
// состояние визуализации геометрии
enum RenderingState
{
  NormalRenderState = 0,
  SpecialMaterialState,
  SpecialTransparencyState,
  SpecialMaterialTransparencyState,
  SpecialMaterialPrimitiveState,
  PrimitiveSelectedState,
  BodySelectionState,
  PrimitiveSelectionState
};

// флаги отображения геометрии
enum RenderMode
{
  ShadedMode,
  ShadedWithEdgesMode,
  WireframeMode,
  HiddenLinesRemovedMode,
  HiddenLinesVisibleMode,
  TransparentMode,
  OutlineSilhouetteMode
};

// применяемый тип Vertex Buffer Object
enum VertexBufferObjectType
{
  buf_Vertex = 10,
  buf_Color,
  buf_Index,
  buf_Normal,
  buf_Texel,
};

enum TypeTriangulation
{
  Triangles,
  TrianglesStrip,
  TrianglesFan,
};

// тип представления
enum TypeRep
{
  Geometry_VBO = 1
};

// состояние видимости 
enum VisibleState
{
  Absolute_Visible = 300,  /// абсолютно видимый
  Relative_Visible = 301,  /// относительно видимый
  Absolute_Hide    = 302,  /// абсолютно невидимый
  Absolute_Visible_Pixel_Culling = 304, /// абсолютно видимый
  Absolute_Hide_Pixel_Culling = 308, /// абсолютно невидимый
};

// настройки осей по умолчанию
// вектор X
const MbVector3D X_AXIS( 1.0, 0.0, 0.0 );
// вектор Y
const MbVector3D Y_AXIS( 0.0, 1.0, 0.0 );
// вектор Z
const MbVector3D Z_AXIS( 0.0, 0.0, 1.0 );

// структура, описывающая тип объекта и его уникальный идентификатор
struct IdType
{
  enum TypePrim { type_none, type_vertex, type_edge, type_face };
  IdType() { m_id = 0; m_type = type_none; }
  IdType( uint32 id, TypePrim type ) { m_id = id; m_type = type; }
  IdType& operator = ( const IdType& other )
  {
    m_id = other.m_id;
    m_type = other.m_type;
    return (*this);
  }

  uint32 m_id;
  TypePrim m_type;
};


VSN_END_NAMESPACE


#endif /* __VSN_NAMESPACE_H */


