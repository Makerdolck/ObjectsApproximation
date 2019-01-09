////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Менеджер интерактивного управления процессами.
         \en Interactive process control manager. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PROCESSMANAGER_H
#define __VSN_PROCESSMANAGER_H

#include "vsn_processevent.h"
#include "vsn_global.h"

VSN_BEGIN_NAMESPACE


class Process;
class ProcessManagerPrivate;

//------------------------------------------------------------------------------
/** \brief \ru Менеджер интерактивного управления процессами.
           \en Interactive process control manager. \~
  \ingroup NO GROUP
*/
// ---
class VSN_CLASS ProcessManager
{
public:
  /// \ru Идентификаторы стандартных процессов. \en Standard process identifiers.
  enum IdProcess
  {
    PR_NoActive  = 0,   ///< \ru Процесс не установлен. \en Unspecified process.
    PR_Panoramic = 1,   ///< \ru Процесс панорамирования. \en Panning process.
    PR_Zoom,            ///< \ru Процесс масштабирования. \en Zooming process.
    PR_Rotate,          ///< \ru Процесс вращения камеры. \en Camera rotation process.
    PR_Orbit,           ///< \ru Процесс вращения камеры по орбите. \en Camera orbiting process.
  };

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit ProcessManager();
  /// \ru Конструктор копирования. \en Copy constructor.
  ProcessManager( const ProcessManager& other );
  /// \ru Деструктор. \en Destructor. 
  virtual ~ProcessManager();

public:
  /** \brief \ru Является ли процесс активным.
             \en Check if process is active. \~ 
    \return \ru Вернуть true, если процесс активный.
            \en True if process is active. \~
  */
	bool IsExistActiveProcess() const;

  /** \brief \ru Вернуть идентификатор активного процесса.
             \en Returns active process identifier. \~ 
    \return \ru Идентификатор активного процесса.
            \en Active process identifier. \~
  */
	int GetActiveProcessId() const;

  /** \brief \ru Вернуть указатель на активный процесс.
             \en Returns pointer to active process. \~ 
    \return \ru Указатель на активный процесс.
            \en Pointer to active process. \~
  */
  Process* GetActiveProcess() const;

  /** \brief \ru Вернуть указатель на процесс.
             \en Returns pointer to process. \~ 
    \param[in] id - \ru Идентификатор интерактивного процесса.
                    \en Interactive process identifier. \~
    \return \ru Указатель на процесс.
            \en Pointer to process. \~
  */
	Process* GetProcess( int id ) const;

public:
  /** \brief \ru Добавить интерактивный процесс.
             \en Adds interactive process. \~ 
    \param[in] scene - \ru Идентификатор интерактивного процесса.
                       \en Interactive process identifier. \~
    \param[in] id - \ru Идентификатор интерактивного процесса.
                    \en Interactive process identifier. \~
  */
  void AddProcess( Process* scene, int id );

  /** \brief \ru Удалить интерактивный процесс.
             \en Removes interactive process. \~ 
    \param[in] id - \ru Идентификатор интерактивного процесса.
                    \en Interactive process identifier. \~
  */
  void RemoveProcess( int id );

  /** \brief \ru Удалить все процессы.
             \en Removes all processes. \~ 
  */
  void Clear();

  /** \brief \ru Активировать процесс и установить данные.
             \en Activates process and sets data. \~ 
    \param[in] id - \ru Идентификатор интерактивного процесса.
                    \en Interactive process identifier. \~
    \param[in] event - \ru Событие перемещения.
                       \en Movement event. \~
    
  */
  void ActivateProcess( int id, ProcessEvent* event );

  /** \brief \ru Сбросить активный процесс.
             \en Resets active process. \~ 
  */
  void DeactivateProcess();

  /** \brief \ru Принять событие от инициатора.
             \en Accepts event from initiator. \~ 
    \param[in] event - \ru Событие перемещения.
                       \en Movement event. \~
  */
  bool MoveActivateProcess( ProcessEvent* event );

public:
	/** \brief \ru Отрисовка текущего состояния.
                   \en Rendering current state. \~ 
  */
  void DrawActiveProcess();

  /** \brief \ru Оператор присваивания.
             \en Assignment operator. \~ 
    \param[in] other - \ru Источник для копирования.
                       \en Copying source. \~
  */
  ProcessManager& operator = ( const ProcessManager& other );

private:
  VSN_DECLARE_PRIVATE(ProcessManager)
};

VSN_END_NAMESPACE

#endif /* __VSN_PROCESSMANAGER_H */
