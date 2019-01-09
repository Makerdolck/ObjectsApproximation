////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс GLSurface представляет собой абстрактную поверхность для визуализации.
         \en GLSurface class represents abstract surface for visualization. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_GLSURFACE_H
#define __VSN_GLSURFACE_H

#include "vsn_glsurfaceformat.h"
#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

class Size;
class GLPlatformSurface;
class GLSurfacePrivate;
//------------------------------------------------------------------------------
/** \brief \ru Класс GLSurface представляет собой абстрактную поверхность для визуализации.
           \en GLSurface class represents abstract surface for visualization. \~
    \details \ru Класс GLSurface содержит информацию о размере поверхности, а также атрибуты поверхности, которые доступны через функцию GetFormat(). 
             \en GLSurface class contains information about surface size, and attributes that are available through GetFormat() function. \~ 
    \ingroup Vision_OpenGL
*/
// ---
class VSN_CLASS GLSurface
{
public:
  /** \brief \ru Перечисление классов GLSurface.
             \en Enumeration of GLSurface classes. \~
  */
  enum GLSurfaceClass
  {
    Window,    ///< \ru Поверхность является экземпляром окна. \en Surface is a window instance.
    Offscreen  ///< \ru Нет реализации. \en Not implemented.
  };

  /** \brief \ru Перечисление типов поверхностей.
             \en Enumeration of surface types. \~
  */
  enum GLSurfaceType
  {
    OpenGLSurface,   ///< \ru Поверхность может быть использована совместно с OpenGLContext. \en Surface can be used in common with OpenGLContext.
    RasterGLSurface  ///< \ru Нет реализации. \en Not implemented.
  };

protected:
  /// \ru Специальный конструктор по умолчанию для создания поверхности с заданным типом. \en The default special constructor for creating the surface with set type.
  explicit GLSurface(GLSurfacePrivate& d, GLSurfaceClass type);
public:
  /// \ru Деструктор. \en Destructor. 
  virtual ~GLSurface();
public:
  /// \ru Вернуть класс поверхности. \en Returns surface class.
  GLSurfaceClass GetSurfaceClass() const;
  /// \ru Вернуть формат поверхности. \en Returns surface format.
  virtual GLSurfaceFormat GetFormat() const = 0;
  /// \ru Вернуть указатель на созданную поверхность. \en Returns pointer to the created surface.
  virtual GLPlatformSurface* GetSurfaceHandle() const = 0;
  /// \ru Вернуть тип поверхности для отрисовки. \en Returns surface type for rendering.
  virtual GLSurfaceType GetSurfaceType() const = 0;
  /// \ru Вернуть true, если поверхность OpenGL совместима и может быть использована совместно с OpenGLContext, в противном случае вернет false. \en True if OpenGL surface is compatible and can be used in common with OpenGLContext.
  bool IsSupportsOpenGL() const;
  /// \ru Вернуть размер поверхности в пикселях. \en Returns surface size in pixels.
  virtual Size GetSize() const = 0;
  /// \ru Обновить изображение на этой поверхности. \en Updates the image on the surface.
  virtual void Update() = 0;
  /// \ru Обновить изображение на этой поверхности при выборе объекта. \en Updates the image on the surface when selecting object.
  virtual void UpdateSelectionGL() = 0;
protected:
  VSN_DECLARE_PRIVATE(GLSurface)
};

VSN_END_NAMESPACE


#endif /* __VSN_GLSURFACE_H */
