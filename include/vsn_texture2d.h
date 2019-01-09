#ifndef C3D_TEXTURE2D_H
#define C3D_TEXTURE2D_H

#ifdef __USE_QT__

#include <string>

#include <QFile>
#include <QImage>

//#include "vsn_glcontext.h"
#include "vsn_openglcontext.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Texture2DPrivate;
///////////////////////////////////////////////////////////////////////////////
//
// класс Texture2D представляет текстуру изображения
// текстура изображения определяют текстурной картой в 2D системе координат
// 
///////////////////////////////////////////////////////////////////////////////
class Texture2D
{
public:
  // конструктор по умолчанию
  Texture2D();
  // специальный конструктор с QFile
  Texture2D( const QFile& );
  // специальный конструктор с готовым изображением
  Texture2D( const QImage&, const std::wstring& fileName = std::wstring() );
  // конструктор копирования
  Texture2D( const Texture2D& other );
  // деструктор
  virtual ~Texture2D();

public:
  //вернуть указатель на контексты OpenGl этой текстуры
  OpenGLContext* GetContext() const;

  // вернуть имя файла текстуры
  const std::wstring& GetFileName() const;

  // вернуть идентификатор текстуры
  GLuint GetId() const;

  // вернуть true, если текстура загружена
  bool IsLoaded() const;

  // вернуть размер текстуры
  void GetSize( int& width, int& height ) const;

  // вернуть максимальный размер текстуры
  static void GetMaxTextureSize( int& width, int& height );
  // установить максимальные размеры текстуры
  static void SetMaxTextureSize( int width, int height );

  // вернуть true, если текстура использует альфа канал
  bool IsAlphaChannel() const;

  // вернуть изображения текстуры
  const QImage& GetImageTexture() const;

public:
  // загрузка текстуры
  void OGLLoadTexture( OpenGLContext* pContext = V_NULL );
  // привязать текстуру в 2D режиме
  void OGLBindTexture();

public:
  // оператор копирования
  Texture2D& operator = ( const Texture2D& other );
  // оператор тождества
  bool operator == ( const Texture2D& other ) const;

private:
  VSN_DECLARE_PRIVATE(Texture2D)
};
VSN_END_NAMESPACE

#endif

#endif /* C3D_TEXTURE2D_H */
