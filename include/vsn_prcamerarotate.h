////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Обработчик поворота камеры.
         \en NO TRANSLATION. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PRCAMERAROTATE_H
#define __VSN_PRCAMERAROTATE_H

#include "vsn_process.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;

//------------------------------------------------------------------------------
/** \brief \ru Обработчик поворота камеры.
           \en NO TRANSLATION. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS PrCameraRotate : public Process
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en NO TRANSLATION. \~ 
    \param[in] pViewport - \ru Порт представления.
                           \en NO TRANSLATION. \~
  */
  PrCameraRotate( Viewport* pViewport );

  /** \brief \ru Конструктор копирования.
             \en NO TRANSLATION. \~ 
    \param[in] other - \ru Обработчик поворота стола.
                       \en NO TRANSLATION. \~
  */
  PrCameraRotate( const PrCameraRotate& );

  /// \ru Деструктор. \en Destructor. 
  virtual ~PrCameraRotate();

public:
  /// \ru Вернуть указатель на копию обработчика действия. \en NO TRANSLATION.
  virtual Process* Duplicate() const;
public:
  /// \ru Активировать обработчик действия. \en NO TRANSLATION.
  virtual void Activate(ProcessEvent* event);
  /// \ru Обработка события перемещения. \en NO TRANSLATION.
  virtual bool Motion(ProcessEvent* event);

private:
  double m_sign;                  ///< \ru Знак угла поворота. \en NO TRANSLATION.
  MbCartPoint3D m_rotationCenter; ///< \ru Центр поворота. \en NO TRANSLATION.
};
VSN_END_NAMESPACE

#endif /* __VSN_PRCAMERAROTATE_H */
