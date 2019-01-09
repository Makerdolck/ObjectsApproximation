////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс GLPlatformSurface обеспечивает абстракцию для поверхности.
         \en GLPlatformSurface class provides abstraction for surface. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GLPLATFORMSURFACE_H
#define __VSN_GLPLATFORMSURFACE_H

#include "vsn_glsurface.h"
#include "vsn_glsurfaceformat.h"
#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

class GLPlatformSurfacePrivate;
//------------------------------------------------------------------------------
/** \brief \ru Класс GLPlatformSurface обеспечивает абстракцию для поверхности.
           \en GLPlatformSurface class provides abstraction for surface. \~
    \ingroup Vision_OpenGL
*/
// ---
class VSN_CLASS GLPlatformSurface
{
protected:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit GLPlatformSurface(GLSurface* pSurface);
public:
  /// \ru Деструктор. \en Destructor.
  virtual ~GLPlatformSurface();
public:
  virtual GLSurfaceFormat GetFormat() const = 0;
public:
  /// \ru Вернуть указатель на поверхность отрисовки. \en Returns pointer to rendering surface.
  GLSurface* GetSurface() const;
private:
  VSN_DECLARE_PRIVATE(GLPlatformSurface)
};

VSN_END_NAMESPACE

#endif /* __VSN_GLPLATFORMSURFACE_H */
