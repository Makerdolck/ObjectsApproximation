////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс PrCameraPan обеспечивает панорамирование сцены.
         \en PrCameraPan class provides scene panning. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PRCAMERAPAN_H
#define __VSN_PRCAMERAPAN_H

#include "vsn_process.h"
#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;
//------------------------------------------------------------------------------
/** \brief \ru Класс PrCameraPan обеспечивает панорамирование сцены.
           \en PrCameraPan class provides scene panning. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS PrCameraPan : public Process
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~ 
    \param[in] pViewport - \ru Порт представления.
                           \en Representation port. \~
  */
  PrCameraPan( Viewport* pViewport );

  /** \brief \ru Конструктор копирования.
             \en Copy constructor. \~ 
    \param[in] other - \ru Источник копирования.
                       \en Copying source. \~
  */
  PrCameraPan( const PrCameraPan& other );

  /// \ru Деструктор. \en Destructor. 
  virtual ~PrCameraPan();

public:
  /// \ru Вернуть указатель на копию процесса панорамирования. \en Returns pointer to panning process copy.
  virtual Process* Duplicate() const;
public:
  /// \ru Активировать процесс панорамирования. \en Activates panning process.
  virtual void Activate(ProcessEvent* event);
  /// \ru Обработка события панорамирования. \en Processing panning event.
  virtual bool Motion(ProcessEvent* event);
};

VSN_END_NAMESPACE

#endif /* __VSN_PRCAMERAPAN_H */
