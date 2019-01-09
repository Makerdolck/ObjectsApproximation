////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс OpenGLContext представляет свой контекст OpenGL, который воспроизводит отрисовку на специальном классе GLSurface.
         \en NO TRANSLATION. \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_OPENGLCONTEXT_H
#define __VSN_OPENGLCONTEXT_H

#include <utility>
#include <set>
#include <list>
#include <vector>
#include <unordered_set>
#include <mb_matrix3d.h>
#include "vsn_global.h"

#ifndef VSN_NO_OPENGL

#include "vsn_glsurfaceformat.h"
#include "vsn_opengl.h"


VSN_BEGIN_NAMESPACE

class OpenGLFunctionList;
class GLSurface;
class PlatformOpenGLContext;
class OpenGLContextList;
class OpenGLContextPrivate;

//------------------------------------------------------------------------------
/** \brief \ru Класс OpenGLContext представляет свой контекст OpenGL, который воспроизводит отрисовку на специальном классе GLSurface.
           \en  NO TRANSLATION. \~ 
    \details \ru .
             \en NO TRANSLATION. \n \~
    \ingroup Vision_OpenGL
*/
// ---
class VSN_CLASS OpenGLContext
{
public:
  /** \brief \ru Данное перечисление определяет тип базового реализацию OpenGL.
             \en NO TRANSLATION. \~
  */
  enum OpenGLLibraryType 
  {
    LibraryOGL,  ///< \ru OpenGL. \en OpenGL.
    LibraryGLES  ///< \ru OpenGL ES версии 2.0 или выше. \en OpenGL ES 2.0 or higher.
  };

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit OpenGLContext();
  /// \ru Деструктор. \en Destructor.
  virtual ~OpenGLContext();

public:
  void Clean();

  /// \ru Cоздать контекст OpenGL с текущей конфигурацией. \en NO TRANSLATION.
  bool Create(PlatformOpenGLContext* pPlatformOpenGLContext);

  /// \ru Возвращает формат контекста, если кортекст был создан с помощью функции Create(). \en NO TRANSLATION.
  GLSurfaceFormat GetFormat() const;
  /// \ru Установить формат контекста OpenGL совместимый с текущей версиеей. Эта функция работает после вызова функции Create(). \en NO TRANSLATION.
  void SetFormat(const GLSurfaceFormat& format);

  /// \ru Функция не реализована, не используете ее. \en NO TRANSLATION.
  OpenGLContext* GetShareContext() const;
  /// \ru Функция не реализована, не используете ее. \en NO TRANSLATION.
  void SetShareContext(OpenGLContext* pShareContext);

  /// \ru Функция не реализована, не используете ее. \en NO TRANSLATION.
  void* GetNativeHandle() const;
  /// \ru Функция не реализована, не используете ее. \en NO TRANSLATION.
  void SetNativeHandle(void* pHandle);

  /// \ru Возвращает указатель на экран для отрисовки OpenGL. \en NO TRANSLATION.
  GLSurface* GetSurface() const;

  /// \ru Разрешено ли освещение. \en NO TRANSLATION.
  bool IsAllowLighting() const;
  /// \ru Возвращает true, если этот контекст является действительным, т.е. был успешно создан. \en NO TRANSLATION.
  bool IsValid() const;
  /// \ru Возвращает объект фреймбуфера по умолчанию для текущей поверхности. \en NO TRANSLATION.
  GLuint GetDefaultFrameBufferObject() const;

  /// \ru Делает текущий контекст указанной поверхности. Возвращает true, если успешно.. \en NO TRANSLATION.
  bool MakeCurrent(GLSurface* pFrame);
  /// \ru Функция для удобства делает вызов makeCurrent с 0 поверхностью. \en NO TRANSLATION.
  void DoneCurrent();
  /// \ru Поменять задний и передний буферы указанной поверхности. \en NO TRANSLATION.
  void SwapBuffers(GLSurface* pFrame);

  /// \ru Это перегруженная функция. Возвращает указатель на указанную функцию. \en NO TRANSLATION.
  FunctionPtr GetProcAddress(const std::string& procName) const;
  /// \ru Получить экземпляр OpenGLFunctionList для этого контекста. \en NO TRANSLATION.
  OpenGLFunctionList* GetFunctionList() const;

  /// \ru Возвращает последний контекст, который был установлен при вызове MakeCurrent или 0, если контекст не установлен.  \en NO TRANSLATION.
  static OpenGLContext* GetCurrentContext();

  /// \ru Возвращает базовый контекст платформы. \en NO TRANSLATION.
  PlatformOpenGLContext* GetPlatformOpenGLContext() const;
  /// \ru Возвращает указатель на базовый контекст платформы. \en NO TRANSLATION.
  PlatformOpenGLContext* GetSharePlatformOpenGLContext() const;
  /// \ru Возвращает список расшаренных контекстов этого контекста. \en NO TRANSLATION.
  OpenGLContextList* GetShareContextList() const;

  /// \ru Возвращает true, если контекст является контекстом OpenGL ES. \en NO TRANSLATION.
  bool IsOpenGLES() const;
  /// \ru Возвращает true, если контекст работает совместно с контекстом QGLContext в противном случае false. \en NO TRANSLATION.
  bool IsSupportsQtContext() const;
  /// \ru Установить возможность работы с контекстом QGLContext. \en NO TRANSLATION.
  void SetQtContextSupport(bool support);

  MbMatrix3D GetModelViewMatrix() const;
  MbMatrix3D GetProjectionMatrix() const;

  virtual void oglMatrixMode( uint mode );
  virtual void oglLoadIdentity();
  virtual void oglPushMatrix();
  virtual void oglPopMatrix();
  virtual void oglLoadMatrix(const MbMatrix3D& matrix);
  virtual void oglMultMatrix( const MbMatrix3D& matrix );
  virtual void oglOrtho( double left, double right, double bottom, double top, double nearVal, double farVal );
  virtual void oglFrustum( double left, double right, double bottom, double top, double nearVal, double farVal );
  virtual void oglAllowLighting( bool bAllow );
  virtual void UpdateVariables();

private:
  friend class AbsGLFunctionListPrivate;
protected:
  VSN_DECLARE_PRIVATE(OpenGLContext)
};

VSN_END_NAMESPACE

#endif // VSN_NO_OPENGL

#endif /* __VSN_OPENGLCONTEXT_H */
