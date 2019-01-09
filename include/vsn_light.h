///////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Источник света.
         \en Light source. \~
*/
///////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_LIGHT_H
#define __VSN_LIGHT_H

#include <unordered_map>
#include <set>

#include <mb_vector3d.h>
#include <mb_cart_point3d.h>

#include "vsn_openglcontext.h"
#include "vsn_color.h"
#include "vsn_object.h"
#include "vsn_namespace.h"

#include "vsn_global.h"


VSN_BEGIN_NAMESPACE

class LightPrivate;
//----------------------------------------------------------------------------
//
/** \brief \ru Источник света.
           \en Light source. \~
  \details \ru Light - точечный источник света. Источник света имеет свою 3D позицию, а также свой сложный цвет. \n
           \en Light is a point light source. The light source has its 3D position, and its complex color. \n \~
  \ingroup Vision_Viewport
*/
// ---
class VSN_CLASS Light : public Object  // Источник света переводится как LightSource. Может имеет смысл так обозвать и класс.
{
public:
  /// \ru Конструктор по умолчанию. По умолчанию цвет окружения черный, диффузный цвет белый и цвет зеркальности белый. \en Default constructor. Default color of ambience is black, diffuse color is white and specular color is white.
  Light(const OpenGLContext* pContext = V_NULL, const Color& color = Color(255, 255, 255));
  /// \ru Конструктор с выбором типа освещения. По умолчанию цвет окружения черный, диффузный цвет белый и цвет зеркальности белый. \en Constructor with light type selection. Default color of ambience is black, diffuse color is white and specular color is white.
  Light(C3DVision::LightTypes lightType, const OpenGLContext* pContext = V_NULL, const Color& color = Color(255, 255, 255));
  /// \ru Конструктор копирования. \en Copy-constructor.
  Light( const Light& other );
  /// \ru Деструктор. \en Destructor.
  virtual ~Light();
  
public:
  /// \ru Включен ли источник света. \en If light source is enabled.
  bool IsLightEnabled() const;
  /// \ru Включить или выключить источник света. \en Turns the light on/off. 
  void SetLightEnabled( bool bEnable );
  /// \ru Вернуть позицию источника света. \en Returns the position of the light source.
  MbCartPoint3D GetPosition() const;  
  /// \ru Установить позицию источника света. \en Sets the position of the light source.
  void SetPosition( const MbCartPoint3D& pos );
  /// \ru Установить позицию источника света. \en Sets the position of the light source.
  void SetPosition( GLfloat x, GLfloat y, GLfloat z );

  /// \ru Вернуть цвет фонового излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0)). \en Returns the ambient color of the light. Default value is [0.0, 0.0, 0.0, 1.0].
  Color GetAmbientColor() const;
  /// \ru Установить цвет фонового излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0)). \en Sets the ambient color of the light. Default value is [0.0, 0.0, 0.0, 1.0].
  void SetAmbientColor( const Color& color );
  
  /// \ru Вернуть цвет рассеянного излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0) или (1.0, 1.0, 1.0, 1.0)). \en Returns the diffuse color of the light. Default value is [0.0, 0.0, 0.0, 1.0] or [1.0, 1.0, 1.0, 1.0].
  Color GetDiffuseColor() const;
  /// \ru Установить цвет рассеянного излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0) или (1.0, 1.0, 1.0, 1.0)). \en Sets the diffuse color of the light. Default value is [0.0, 0.0, 0.0, 1.0] or [1.0, 1.0, 1.0, 1.0].
  void SetDiffuseColor( const Color& color );

  /// \ru Вернуть цвет зеркального излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0) или (1.0, 1.0, 1.0, 1.0)). \en Returns the specular color of the light. Default value is [0.0, 0.0, 0.0, 1.0] or [1.0, 1.0, 1.0, 1.0].
  Color GetSpecularColor() const;
  /// \ru Установить цвет зеркального излучения источника света (значение по умолчанию (0.0, 0.0, 0.0, 1.0) или (1.0, 1.0, 1.0, 1.0)). \en Sets the specular color of the light. Default value is [0.0, 0.0, 0.0, 1.0] or [1.0, 1.0, 1.0, 1.0].
  void SetSpecularColor( const Color& color );
  
  /// \ru Проверить, включен ли режим двустороннего освещения. \en Checks if two-sided lighting mode is enabled.
  bool IsTwoSided() const;
  /// \ru Установить режим двустороннего освещения. \en Sets the two-sided lighting mode.
  void SetTwoSided( bool bTwoSided );
  
  /// \ru Вернуть тип источника освещения. \en Returns the light type.
  C3DVision::LightTypes GetType() const;
  /// \ru Установить тип источника освещения. \en Sets the light type.
  void SetType( C3DVision::LightTypes type );

  /// \ru Вернуть идентификатор источника. \en Returns the identifier of the light source.
  GLenum GetGlLightID() const;

  
  /// \ru Вернуть постоянную ослабления источника освещения. \en Returns the constant attenuation of the light source.
  GLfloat GetConstantAttenuation() const;
  /// \ru Установить постоянную ослабления источника освещения. \en Sets the constant attenuation of the light source.
  void SetConstantAttenuation( GLfloat attenuationConstant );

  /// \ru Вернуть линейное ослабление источника освещения. \en Returns the linear attenuation of the light source.
  GLfloat GetLinearAttenuation() const;
  /// \ru Установить линейное ослабление источника освещения. \en Sets the constant attenuation of the light source.
  void SetLinearAttenuation( GLfloat linearAttenuation );

  /// \ru Вернуть квадратичное ослабление источника освещения. \en Returns the quadratic attenuation of the light source.
  GLfloat GetQuadraticAttenuation() const;
  /// \ru Установить квадратичное ослабление источника освещения. \en Sets the quadratic attenuation of the light source.
  void SetQuadraticAttenuation( GLfloat attenuationQuadric );
  
  /// \ru Вернуть направление освещения от точечного источника. (x, y, z) - направление прожектора. Может быть представлено в виде оси ограничевающего конуса. \en Returns the light direction from the point source. The spotlight direction is (x, y, z). It might be represented as an axis of bounding cone.
  MbVector3D GetSpotDirection() const;
  /// \ru Установить направление освещения от точечного источника. (x, y, z) - направление прожектора (ось ограничивающего конуса). \en Sets the light direction from the point source. The spotlight direction is (x, y, z). It might be represented as an axis of bounding cone.
  void SetSpotDirection( const MbVector3D& direction );

  /// \ru Вернуть угол между осью и стороной конуса (он же половина угла при вершине) точечного источника освещения. \en Returns angle between axis and cone side of the point light source(semivertex angle).
  GLfloat GetSpotCutoffAngle() const;
  /// \ru Установить угол между осью и стороной конуса (он же половина угла при вершине) точечного источника освещения. \en Sets angle between axis and cone side of the point light source(semivertex angle).
  void SetSpotCutoffAngle( GLfloat cutoffAngle );

  /// \ru Вернуть экспоненту убывания интенсивности точечного источника освещения. \en Returns the intensity decreasing exponent of the point light source.
  GLfloat GetSpotExponent() const;
  /// \ru Установить экспоненту убывания интенсивности точечного источника освещения. \en Sets the intensity decreasing exponent of the point light source.
  void SetSpotExponent( GLfloat exponent );

public:
  /// \ru Вернуть максимальное количество источников освещения. \en Returns maximum amount of light sources.
  static int GetLightCount();
  /// \ru Вернуть количество рабочих источников освещения. \en Returns amount of operating light sources.
  static size_t GetOperatingLightCount(OpenGLContext* pContext);
  /// \ru Настроить источник в OpenGL. \en Sets the light source in OpenGL.
  virtual void oglPerform();

private:
  VSN_DECLARE_PRIVATE(Light)
};

VSN_END_NAMESPACE


#endif /* __VSN_LIGHT_H */
