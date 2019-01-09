////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс PrCameraZoom обеспечивает масштабирование сцены относительно ее центра.
         \en PrCameraZoom class provides scene zooming about its center. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PRCAMERAZOOM_H
#define __VSN_PRCAMERAZOOM_H

#include "vsn_process.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;

//------------------------------------------------------------------------------
/** \brief \ru Класс PrCameraZoom масштабирования сцены относительно ее центра.
           \en PrCameraZoom class provides scene zooming about its center. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS PrCameraZoom : public Process
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~ 
    \param[in] pViewport - \ru Порт представления.
                           \en Representation port. \~
  */
  PrCameraZoom( Viewport* pViewport );

  /** \brief \ru Конструктор копирования.
             \en Copy constructor. \~ 
    \param[in] other - \ru Процесс масштабирования.
                       \en Zoom process. \~
  */
  PrCameraZoom( const PrCameraZoom& other );

  /** \brief \ru Деструктор.
             \en Destructor. \~ 
  */
  /// \ru Деструктор. \en Destructor. 
  virtual ~PrCameraZoom();

public:
  /// \ru Вернуть указатель на копию процесса. \en Returns pointer to process copy.
  virtual Process* Duplicate() const;
public:
  /// \ru Активировать процесс. \en Activates process.
  virtual void Activate(ProcessEvent* event );
  /// \ru Обработка события перемещения. \en Processing movement event.
  virtual bool Motion(ProcessEvent* event );

  /** \brief \ru Установить максимальный коэффициент масштаба.
             \en Sets maximum zoom factor. \~ 
    \return \ru Возвращает коэффициент масштабирования.
            \en Returns zoom factor. \~
  */
  double GetMaxZoomFactor() const;

  /** \brief \ru Установить максимальный коэффициент масштаба.
             \en Sets maximum zoom factor. \~ 
    \param[in] factor - \ru Коэффициент масштабирования.
                        \en Zoom factor. \~
  */
  void SetMaxZoomFactor( double factor );

private:
  MbCartPoint m_pntOld;     ///< \ru Старое положение пользовательского ввода. \en Old user input position.
  MbCartPoint m_pntNew;     ///< \ru Новое положение пользовательского ввода. \en New user input position.
  MbCartPoint3D	m_ptDist, m_ptCamera; ///< \ru Положение камеры и цели. \en Camera and target position.
  double m_length;         ///< \ru Расстояние между камерой и целью. \en Distance between target and camera.  
  double m_maxZoomFactor;  ///< \ru Максимальный коэффициент масштаба. \en Maximum zoom factor. 
};
VSN_END_NAMESPACE

#endif /* __VSN_PRCAMERAZOOM_H */
