////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс PrCameraOrbit обеспечивает интерактивные действия пользователя, а именно управление камерой, ее вращением по орбите.
         \en PrCameraOrbit class provides interactive user actions, i.e. camera control and its orbiting. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PRCAMERAORBIT_H
#define __VSN_PRCAMERAORBIT_H

#include "vsn_process.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;

//------------------------------------------------------------------------------
/** \brief \ru Класс PrCameraOrbit обеспечивает вращение по орбите вокруг цели.
           \en PrCameraOrbit class provides camera orbiting around the target. \~
    \ingroup Vision_Process
*/
// ---
class VSN_CLASS PrCameraOrbit : public Process
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~
  \param[in] pViewport - \ru Порт представления.
                         \en Representation port. \~
  */
  PrCameraOrbit(Viewport* pViewport);

  /** \brief \ru Конструктор копирования.
             \en Copy constructor. \~
  \param[in] other - \ru Источник процесса для копирования.
                     \en Copying process source. \~
  */
  PrCameraOrbit(const PrCameraOrbit& other);

  /** \brief \ru Деструктор.
             \en Destructor. \~
  */
  /// \ru Деструктор. \en Destructor. 
  virtual ~PrCameraOrbit();

public:
  /// \ru Вернуть указатель на копию данного процесса. \en Returns pointer to process copy.
  virtual Process* Duplicate() const;

public:
  /// \ru Активировать процесс. \en Activates process.
  virtual void Activate(ProcessEvent* event);
  /// \ru Обработка события перемещения мыши. \en Processing mouse move event.
  virtual bool Motion(ProcessEvent* event);

  /// \ru Выдать угловую позицию камеры. \en Returns an angular camera position.
  void GetAngles( double& alpha, double& beta, double& gamma );
  /// \ru Cбросить все углы в ноль. \en Resets all angles to zero.
  void ResetAngles();

protected:
  MbCartPoint3D m_pntOld;
  MbCartPoint3D m_pntNew;
  MbCartPoint3D	m_pntRealOld;	

private:
  double m_alpha;
  double m_beta;
  double m_gamma;
};

VSN_END_NAMESPACE

#endif /* __VSN_PRCAMERAORBIT_H */
