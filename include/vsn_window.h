////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс Window представляет собой реализацию окна для отрисовки OpenGl'ем.
         \en Class Window is a implementation of the window rendering using OpenGL. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_WINDOW_H
#define __VSN_WINDOW_H

#include "vsn_glsurface.h"
#include "vsn_glsurfaceformat.h"

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

class OpenGLContext;
class WindowPrivate;

//------------------------------------------------------------------------------
/** \brief \ru Класс Window представляет собой реализацию окна для отрисовки OpenGl'ем.
           \en Class Window is a implementation of the window rendering using OpenGL. \~
    \ingroup Vision_OpenGL
*/
// ---
class VSN_CLASS Window : public GLSurface
{
public:
  explicit Window();
  virtual ~Window();

public:
  // созать окно
  void Create(void* hWindow, OpenGLContext* pContext);
  // вернуть хандл окна
  void* GetHWND() const;

  // вернуть формат окна
  GLSurfaceFormat GetFormat() const;
  // установить формат отрисовочному окну
  void SetFormat(const GLSurfaceFormat& format);
  void SetUpdateSelectGLCallBack(CallBackFunc callBack);

public:
  virtual GLPlatformSurface* GetSurfaceHandle() const;
  virtual GLSurfaceType GetSurfaceType() const;
  virtual Size GetSize() const;
  virtual void Update();
  virtual void UpdateSelectionGL();

private:
  OBVIOUS_PRIVATE_COPY(Window)
  VSN_DECLARE_EX_PRIVATE(Window)
};

VSN_END_NAMESPACE

#endif // __VSN_WINDOW_H
