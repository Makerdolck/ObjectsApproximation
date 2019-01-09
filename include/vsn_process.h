////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс Process является базой для классов, которые управляют входными данными пользователя, в том числе манипуляцией со сценой.
         \en Process class is a base for classes that handle user input or manipulate with the scene. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PROCESS_H
#define __VSN_PROCESS_H

#include "vsn_processrep.h"
#include "vsn_processmanager.h"

#include "vsn_global.h"


VSN_BEGIN_NAMESPACE
class Viewport;

//------------------------------------------------------------------------------
/** \brief \ru Класс Process служит в качестве базового класса для классов, который управляет входными данными пользователя, в том числе манипуляцией со сценой.
           \en Process class serves as a base for classes and handles user input or manipulates with the scene. \~
  \details \ru Process предоставляет поддержку для преобразования входных данных пользователя в логику взаимодействия. Объект пользовательского процесса 
               реализует виртуальные функции, которые предоставляются для управления процессом такими устройствами, как мышь и клавиатура и т. д. В качестве примера, 
               если бы приложение нуждалось во вращении камеры по орбите с помощью мышки, производный процесс рализовал бы методы OnLButtonDown, OnLButtonDownAndMove, 
               и OnLButtonUp, чтобы предоставить логику вращению камеры по орбите. \n 
               Модуль Vision/GUI предоставляет поддержку для преобразования специальных UI событий в соответствующий метод текущего process, связанного с видом.
               Аналогично, приложение могло бы вручную преобразовать UI событие в совпадающий метод в текущем процессе без использования Vision/GUI toolkit.    
           \en Process provides support for converting user input to interaction logic. A custom process object implements
               the virtual functions that are provided in order to handle process with such devices as mouse and keyboard, etc. For example,
               if the application needed to orbit the camera with the mouse, the derived process would implement the OnLButtonDown, OnLButtonDownAndMove, 
               and OnLButtonUp methods to provide logic for orbiting the camera. \n
               The Vision/GUI module provides support for converting special UI events to the corresponding method of the current 
               process associated with the view. Alternately, the application could manually convert a UI event to the 
               matching method in the current process without relying on the Vision/GUI toolkit.

  \ingroup Vision_Process
*/
// ---
class VSN_CLASS Process
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~ 
    \param[in] pViewport - \ru Порт представления.
                           \en Representation port. \~
  */
  Process( Viewport* pViewport );

  /** \brief \ru Конструктор копирования.
             \en Copy constructor. \~ 
    \param[in] other - \ru Другой процесс для копирования.
                       \en Other copying process. \~
  */
  Process( const Process& other );

  /// \ru Деструктор. \en Destructor. 
  virtual ~Process();

public:
  /** \brief \ru Вернуть указатель на копию процесса.
             \en Returns pointer to process copy. \~ 
    \return \ru Указатель на копию процесса.
            \en Pointer to process copy. \~
  */
  virtual Process* Duplicate() const = 0;

public:
  /** \brief \ru Активировать процесс.
             \en Activates process. \~ 
    \param[in] event - \ru Параметр события активации.
                       \en Activation event parameter. \~
  */
  virtual void Activate(ProcessEvent* event) = 0;

  /** \brief \ru Обработка события перемещения мыши.
             \en Processing mouse move event. \~ 
    \param[in] event - \ru Параметр события перемещения.
                       \en Move event parameter. \~
    \return \ru Возвращает состояние операции.
            \en Returns operation state. \~
  */
  virtual bool Motion(ProcessEvent* event) = 0;

  /** \brief \ru Деактивировать процесс.
             \en Deactivates process. \~ 
  */
  virtual void Deactivate();

  /** \brief \ru Установить новое представление процесса, если оно имеется.
             \en Sets new process representation, if available. \~ 
    \param[in] reps - \ru Список представлений.
                      \en Representation list. \~
  */
  void SetRepresentations( const std::list<SceneProcessRep*>& reps );

  /** \brief \ru Инициализация представления процесса, если оно имеется.
             \en Initializes process representation, if available. \~ 
  */
  void InitRepresentation();

  /** \brief \ru Обновить представление процесса, если оно имеется.
             \en Updates process representation, if available. \~ 
  */
  void UpdateRepresentation();

public:
  /** \brief \ru Отрисовка представления процесса, если оно имеется.
             \en Rendering process representation, if available. \~ 
  */
  void DrawRepresentation();

private:
  /** \brief \ru Удалить представления из списка представлений.
             \en Removes representation from representation list. \~ 
  */
  void DeleteRepresentations();

protected:
  std::list<SceneProcessRep*> m_representations; ///< \ru Массив представлений процесса. \en Process representation array.
  MbVector3D m_oldVector; ///< \ru Предыдущее состояние перемещения мыши.  \en Previous state of mouse move.
  Viewport* m_pViewport; ///< \ru Порт отображения модели.  \en Model view port.
  SceneProcessRep::RepProcessInfo m_processInfo; ///< \ru Информация о представлении процесса.  \en Process representation info.
};
VSN_END_NAMESPACE

#endif /* __VSN_PROCESS_H */
