#ifndef __SHADER_H
#define __SHADER_H

#ifdef __USE_QT__

#include <map>
#include <vector>
#include <unordered_map>

#include <QtOpenGL>

#include "vsn_global.h"

class QFile;

VSN_BEGIN_NAMESPACE
class ShaderPrivate;
///////////////////////////////////////////////////////////////////////////////
//
// класс Shader реализует приксельный шейдер и его программу, этот класс
// обеспечивает загрузку, а также компиляцию и выполнение подпрограммы вершин и самого шейдера 
//
///////////////////////////////////////////////////////////////////////////////
class VSN_CLASS Shader
{
public:
  // конструктор по умолчанию
  Shader();
  // конструктор с загрузкой вершин и фрагмента
  Shader( QFile& vertex, QFile& fragment );
  // конструктор копирования
  Shader(const Shader&);
  // деструктор
  ~Shader();

public:
  // вернуть идентификатор программы
  GLuint GetId() const;

  // вернуть true, если шейдер используется
  bool IsShaderUsed() const;

  // вернуть true, если шейдер можно удалить
  bool IsCanBeDeleted() const;

  // вернуть имя шейдера
  const std::wstring& GetName() const;

  // установить имя шейдера
  void SetName( const std::wstring& name );

  // вернуть указатель на QGLProgramShader для этого шейдера
  QGLShaderProgram* GetProgramShader();

  // вернуть идентификатор позиции
  int GetPositionAttrId() const;

  // вернуть идентификатор текстуры
  int GetTextureAttrId() const;

  // вернуть идентификатор цвета
  int GetColorAttrId() const;

  // вернуть идентификатор нормали
  int GetNormalAttrId() const;

public:
  // вернуть количество шейдеров
  static size_t GetShaderCount();

  // вернуть true, если идентификатор группы применяется для шейдеров
  static bool IsExistIdAsShader( uint shadingGroupId );

  // вернуть указатель на шейдер по идентификатору, если такого нет, то вернется V_NULL
  static Shader* GetShaderById(uint shadingGroupId);

  // вернуть True, если есть активный шейдер
  static bool IsExistActiveShader();

  // вернуть указатель на активный шейдер, если такого нет, то вернется V_NULL
  static Shader* GetActiveShader();

public:
  // установить из файла шейдеры вершин и фрагмента
  void SetupVertexFragmentShader( QFile& vertexFile, QFile& fragmentFile );

  // заменить этот шейдер копией другого шейдера
  void ReplaceShader( const Shader& other );

  // вернуть идентификатор матрицы вида модели
  int GetModelViewMatrixId() const;

  // вернуть идентификатор проекционной матрицы  вида модели
  int GetModelViewProjMatrixId() const;

  // вернуть идентификатор видовой обратной матрицы (местоположения)
  int GetModelViewInverseMatrixId() const;

  public:
  // вернуть идентификатор включения освещения
  int GetEnableLightingId() const;

  // вернуть идентификатор состояния освещенности
  int GetEnableLightingStateId() const;

  // вернуть положение источника света по его идентификатору
  int GetPositionLightingId( GLenum lightId ) const;

  // вернуть значение освешения общего фона по идентификатору
  int GetAmbientLightingId( GLenum lightId ) const;

  // вернуть значение диффузии по идентификатору
  int GetDiffuseLightingId( GLenum lightId ) const;

  // вернуть значение зеркальности  по идентификатору
  int GetSpecularLightingId( GLenum lightId ) const;

  // вернуть значение пятна освещения по идентификатору
  int GetSpotDirectionLightingId( GLenum lightId ) const;

  // вернуть значение ослабления освещения по идентификатору
  int GetAttebuationFactorsLightingId( GLenum lightId ) const;

  // вернуть значение одиночного освещения (пятно) по идентификатору
  int GetSpotExponentLightingId( GLenum lightId ) const;

  // вернуть значение среза пятна по идентификатору
  int GetSpotCutoffLightingId( GLenum lightId ) const;

  // вернуть значение ослабления освешения от дистанции по идентификатору
  int GetDistanceAttenuationLightingId( GLenum lightId ) const;

public:
  // активировать программу шейдера
  void ActivateProgram();

  // активировать программу шейдера по идентификатору
  static bool ActivateProgram(uint ShadingGroupId);

  // отключить программу шейдера
  static void DeactivateProgram();

  // создать и слинковать программу шейдера, если линковка с ошибкой, то сработает исключение
  void CreateLinkProgrammShader();

  // удалить шейдер
  void DeleteShader();

public:
  // присвоить данные другому шейдеру
  Shader& operator = ( const Shader& other );

private:
  VSN_DECLARE_PRIVATE( Shader )
};
VSN_END_NAMESPACE

#endif
#endif /* __SHADER_H */
