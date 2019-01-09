////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс ProcessRep для представления процесса.
         \en ProcessRep class for process representation. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PROCESSREP_H
#define __VSN_PROCESSREP_H

#include <mb_vector3d.h>
#include <mb_matrix3d.h>

#include "vsn_color.h"
#include "vsn_renderstate.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;

//------------------------------------------------------------------------------
/** \brief \ru Класс ProcessRep для представления процесса.
           \en ProcessRep class for process representation. \~
  \ingroup NO GROUP
*/
// ---
class SceneProcessRep
{
public:

  /** \brief \ru Вспомогательная структура данных для управления представлением.
             \en Auxiliary data structure for representation control. \~
  */
  struct RepProcessInfo
  {
    std::vector<MbMatrix3D> m_matrix; ///< \ru Матрица преобразования. \en Transformation matrix.
    std::vector<MbVector3D> m_vector; ///< \ru Направление взгляда. \en View direction.
    std::vector<double> m_dValue; ///< \ru Список параметров. \en Parameter list.
    std::vector<int> m_iValue; ///< \ru Список параметров. \en Parameter list.
    std::vector<std::string> m_strValue; ///< \ru Список строковых параметров. \en List of string parameters.
  };

public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~ 
    \param[in] pViewport - \ru Порт представления.
                           \en Representation port. \~
  */
  SceneProcessRep( Viewport* pViewport );

  /** \brief \ru Конструктор копирования.
             \en Copy constructor. \~ 
    \param[in] other - \ru Другое представление.
                       \en Another representation. \~
  */
  SceneProcessRep( const SceneProcessRep& other );

  /// \ru Деструктор. \en Destructor. 
  virtual ~SceneProcessRep();

public:
  /** \brief \ru Вернуть цвет представления.
             \en Returns representation color. \~ 
    \return \ru Цвет.
            \en Color. \~
  */
  const Color& GetColor() const;

  /** \brief \ru Установить цвет представления.
             \en Sets representation color. \~ 
    \param[in] color - \ru Цвет.
                       \en Color. \~
  */
  virtual void SetColor( const Color& color );

  /** \brief \ru Вернуть указатель на копию представления.
             \en Returns pointer to representation copy. \~ 
    \return \ru Указатель на копию представления.
            \en Pointer to representation copy. \~
  */
  virtual SceneProcessRep* Duplicate() const= 0;

  /** \brief \ru Установить толщину каркаса представления.
             \en Sets thickness of representation wireframe. \~ 
    \param[in] thickness - \ru Толщина.
                           \en Thickness. \~
  */
  virtual void SetThickness( double thickness );

  /** \brief \ru Инициализировать представление.
             \en Initializes representation. \~ 
  */
  virtual void Init();

  /** \brief \ru Обновить представление.
             \en Updates representation. \~ 
  */
  virtual void Update();

  /** \brief \ru Установить данные представления.
             \en Sets representation data. \~ 
    \param[in] pRepDatas - \ru Указатель на данные представления.
                           \en Pointer to representation data. \~
  */
  void SetRepData( RepProcessInfo* pRepData );

public:
  /** \brief \ru Отрисовать представление.
             \en Renders representation. \~ 
  */
  void OGLDrawEx();

protected:
  /** \brief \ru Отрисовать представление.
             \en Renders representation. \~ 
  */
  virtual void OGLDraw() = 0;

private:
  Viewport* m_pViewport;         ///< \ru Порт отрисовки представления. \en Representation rendering port.
  Color m_color;                 ///< \ru Цвет представления. \en Representation color.
  double m_thickness;            ///< \ru Толщина каркаса представления. \en Representation wireframe thickness.
  RenderState m_renderState;     ///< \ru Состояние отрисовки. \en Rendering state.
  RepProcessInfo* m_pRepData;    ///< \ru Данные представления. \en Representation data.
};
VSN_END_NAMESPACE

#endif /* __VSN_PROCESSREP_H */
