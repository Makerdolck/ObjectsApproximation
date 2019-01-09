////////////////////////////////////////////////////////////////////////////////
/** 
  \file  
  \brief \ru Класс ProcessEvent является событием, а также базовым классом для всех классов событий.
         \en ProcessEvent class is the event and also base class for all event classes. \~

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __VSN_PROCESSEVENT_H
#define __VSN_PROCESSEVENT_H

#include <mb_cart_point.h>
#include <mb_cube.h>
#include <vsn_point.h>

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE


#define PR_CONTROL              1<<1   ///< \ru PR_CONTROL Любая клавиша Ctrl. \en PR_CONTROL Any Ctrl Key.
#define PR_SHIFT                1<<2   ///< \ru PR_SHIFT Любая клавиша Shift. \en PR_SHIFT Any Shift Key.
#define PR_ALT                  1<<3   ///< \ru PR_ALT Любая клавиша Alt. \en PR_ALT Any Alt Key.
#define PR_LBUTTON              1<<4   ///< \ru PR_LBUTTON Левая клавиша мыши. \en PR_LBUTTON Left Mouse Button.
#define PR_MBUTTON              1<<5   ///< \ru PR_MBUTTON Средняя клавиша мыши. \en PR_MBUTTON Middle Mouse Button.
#define PR_RBUTTON              1<<6   ///< \ru PR_RBUTTON Правая клавиша мыши. \en PR_RBUTTON Right Mouse Button.
#define PR_RIGHT_CONTROL        1<<7   ///< \ru PR_RIGHT_CONTROL Правая клавиша Ctrl. \en PR_RIGHT_CONTROL Right Ctrl Key.
#define PR_RIGHT_SHIFT          1<<8   ///< \ru PR_RIGHT_SHIFT Правая клавиша Shift. \en PR_RIGHT_SHIFT Right Shift Key.
#define PR_RIGHT_ALT            1<<9   ///< \ru PR_RIGHT_ALT Правая клавиша Alt. \en PR_RIGHT_ALT Right Alt Key.
#define PR_LEFT_CONTROL         1<<10  ///< \ru PR_LEFT_CONTROL Левая клавиша Ctrl. \en PR_LEFT_CONTROL Left Ctrl Key.
#define PR_LEFT_SHIFT           1<<11  ///< \ru PR_LEFT_SHIFT Левая клавиша Shift. \en PR_LEFT_SHIFT Left Shift Key.
#define PR_LEFT_ALT             1<<12  ///< \ru PR_LEFT_ALT Левая клавиша Alt. \en PR_LEFT_ALT Left Alt Key.
#define PR_LEFT_ARROW           1<<13  ///< \ru PR_LEFT_ARROW Клавиша стрелка влево. \en PR_LEFT_ARROW Left Arrow Key.
#define PR_UP_ARROW             1<<14  ///< \ru PR_UP_ARROW Клавиша стрелка вверх. \en PR_UP_ARROW Up Arrow Key.
#define PR_RIGHT_ARROW          1<<15  ///< \ru PR_RIGHT_ARROW Клавиша стрелка вправо. \en PR_RIGHT_ARROW Right Arrow Key.
#define PR_DOWN_ARROW           1<<16  ///< \ru PR_DOWN_ARROW Клавиша стрелка вниз. \en PR_DOWN_ARROW Down Arrow Key.
#define PR_JOYSTICK_BUTTON_1    1<<17  ///< \ru PR_JOYSTICK_BUTTON_1 Кнопка джойстика 1. \en PR_JOYSTICK_BUTTON_1 Joystick Button 1.
#define PR_JOYSTICK_BUTTON_2    1<<18  ///< \ru PR_JOYSTICK_BUTTON_2 Кнопка джойстика 2. \en PR_JOYSTICK_BUTTON_2 Joystick Button 2.
#define PR_JOYSTICK_BUTTON_3    1<<19  ///< \ru PR_JOYSTICK_BUTTON_3 Кнопка джойстика 3. \en PR_JOYSTICK_BUTTON_3 Joystick Button 3.
#define PR_JOYSTICK_BUTTON_4    1<<20  ///< \ru PR_JOYSTICK_BUTTON_4 Кнопка джойстика 4. \en PR_JOYSTICK_BUTTON_4 Joystick Button 4.
#define PR_JOYSTICK_BUTTON_5    1<<21  ///< \ru PR_JOYSTICK_BUTTON_5 Кнопка джойстика 5. \en PR_JOYSTICK_BUTTON_5 Joystick Button 5.
#define PR_JOYSTICK_BUTTON_6    1<<22  ///< \ru PR_JOYSTICK_BUTTON_6 Кнопка джойстика 6. \en PR_JOYSTICK_BUTTON_6 Joystick Button 6.
#define PR_JOYSTICK_BUTTON_7    1<<23  ///< \ru PR_JOYSTICK_BUTTON_7 Кнопка джойстика 7. \en PR_JOYSTICK_BUTTON_7 Joystick Button 7.
#define PR_JOYSTICK_BUTTON_8    1<<24  ///< \ru PR_JOYSTICK_BUTTON_8 Кнопка джойстика 8. \en PR_JOYSTICK_BUTTON_8 Joystick Button 8.


//------------------------------------------------------------------------------
/** \brief \ru Класс ProcessEvent является событием, а также базовым классом для всех классов событий. Объекты событий содержат параметры события.
           \en ProcessEvent class is the event and also base class for all event classes. Event objects contain event parameters. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS ProcessEvent
{
public:
  /// \ru Тип события, который определяет состояние мыши, клавиатуры и подобных устройств в реальном режиме времени. \en The event type defines state of mouse, keyboard and such devices in real time mode.
  enum EventType
  {
    None = 0,           ///< \ru Недействительное событие. \en Invalid event.

    LButtonDown,        ///< \ru Левая кнопка мыши нажата. \en The left mouse button is pressed.
    LButtonUp,          ///< \ru Левая кнопка мыши отпущена. \en The left mouse button is released.
    LButtonDblClk,      ///< \ru Левая кнопка мыши нажата дважды. \en The left mouse button is double-clicked.

    MButtonDown,        ///< \ru Средняя кнопка мыши нажата. \en The middle mouse button is pressed.
    MButtonUp,          ///< \ru Средняя кнопка мыши отпущена. \en The middle mouse button is released.
    MButtonDblClk,      ///< \ru Средняя кнопка мыши нажата дважды. \en The middle mouse button is double-clicked.

    RButtonDown,        ///< \ru Правая кнопка мыши нажата. \en The right mouse button is pressed.
    RButtonUp,          ///< \ru Правая кнопка мыши отпущена. \en The right mouse button is released.
    RButtonDblClk,      ///< \ru Правая кнопка мыши нажата дважды. \en The right mouse button is double-clicked.

    MouseMove,          ///< \ru Курсор мыши движется. \en The mouse cursor moves.

    MouseWheel,         ///< \ru Колесико мыши крутится. \en The mouse wheel moves.
    MouseWheelUp,       ///< \ru Колесико мыши прокручивается вверх. \en The mouse wheel is scrolled up.
    MouseWheelDown,     ///< \ru Колесико мыши прокручивается вниз. \en The mouse wheel is scrolled down.
	
    Timer,              ///< \ru Говорит само за себя. \en Self-explanatory.

    KeyDown,            ///< \ru Клавиша на клавиатуре нажата. \en A key on the keyboard is pressed.
    KeyUp,              ///< \ru Клавиша на клавиатуре отпущена. \en A key on the keyboard is released.

    Selection,          ///< \ru Событие общего выбора. \en General selection event.

    JoyStickTranslate,  ///< \ru Трехмерное перемещение вектора от 3D устройства. \en Vector movement from a 3D device.
    JoyStickRotate,     ///< \ru Набор значений поворота от 3D устройства. \en A rotation set values from a 3D device.
    JoyStickButton,     ///< \ru Кнопка нажатая от 3D устройства. \en A button pressed from a 3D device.

    Camera,             ///< \ru Изменение позиции камеры. \en Repositioning camera.

    User = 500,         ///< \ru Идентификатор начального события пользователя. \en First user event id.
    MaxUser = 65535     ///< \ru Идентификатор конечного события пользователя. \en Last user event id.
  };

public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  explicit ProcessEvent(EventType type);
  /// \ru Конструктор копирования. \en Copy constructor. 
  ProcessEvent(const ProcessEvent& other);
  /// \ru Деструктор. \en Destructor. 
  virtual ~ProcessEvent();
public:
  /// \ru Вернуть идентификатор типа событий. \en Returns event type identifier. 
  inline EventType GetType() const { return static_cast<EventType>(m_type); }
private:
  unsigned short m_type;
};

//------------------------------------------------------------------------------
/** \brief \ru Класс InputEvent является базовым классом для событий, который описывает вводимые пользователем данные.
           \en InputEvent class is the base class for events that describes user input. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS InputEvent : public ProcessEvent
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  InputEvent(EventType type);
  /// \ru Конструктор копирования. \en Copy constructor. 
  InputEvent(const InputEvent& other);
  /// \ru Деструктор. \en Destructor.
  ~InputEvent();
public:
  /// \ru True, если клавиша Ctrl сейчас нажата. \en True if Ctrl key is currently pressed.
	bool Control() const	    { return(m_flags & PR_CONTROL) != 0; }
  /// \ru True, если клавиша Shift сейчас нажата. \en True if Shift key is currently pressed.
	bool Shift() const	      { return(m_flags & PR_SHIFT) != 0; }
  /// \ru True, если клавиша Alt сейчас нажата. \en True if Alt key is currently pressed.
	bool Alt() const	        { return(m_flags & PR_ALT) != 0; }
  /// \ru True, если левая клавиша Alt нажата. \en True if the left Alt key is pressed.
	bool LeftAlt() const	    { return(m_flags & PR_LEFT_ALT) != 0; }
  /// \ru True, если левая клавиша Ctrl нажата. \en True if the left Ctrl key is pressed.
	bool LeftControl() const  { return(m_flags & PR_LEFT_CONTROL) != 0; }
  /// \ru True, если левая клавиша Shift нажата. \en True if the left Shift key is pressed.
	bool LeftShift() const	  { return(m_flags & PR_LEFT_SHIFT) != 0; }
  /// \ru True, если правая клавиша Alt нажата. \en True if the right Alt key is pressed.
	bool RightAlt() const	    { return(m_flags & PR_RIGHT_ALT) != 0; }
  /// \ru True, если правая клавиша Ctrl нажата. \en True if the right Ctrl key is pressed.
	bool RightControl() const { return(m_flags & PR_RIGHT_CONTROL) != 0; }
  /// \ru True, если правая клавиша Shift нажата. \en True if the right Shift key is pressed.
	bool RightShift() const   { return(m_flags & PR_RIGHT_SHIFT) != 0; }

  /// \ru Возвращает бит ProcessEvent флагов, таких как PR_LBUTTON, для специальных событий клавиш Shift, Ctrl и левая, средняя и правая кнопки мыши. \en Returns ProcessEvent bit flags like PR_LBUTTON for special events of Shift and Ctrl keys as well as left, middle and right mouse buttons.
	unsigned int GetFlags( void ) const { return m_flags; }

  /** \brief \ru Этот метод устанавливает побитовые флаги, которые показывают состояние кнопок мыши, клавиш Shift и Ctrl.
             \en This method sets bitwise flags that shows the state of mouse buttons, Shift and Ctrl keys. \~
      \param[in] flags - \ru Побитовые флаги для состояния мыши и клавиш. Например, если и клавиша Shift, и левая кнопка мыши нажаты, пропустить PR_SHIFT|PR_LBUTTON.   
                         \en Bitwise flags for mouse and key states. For example, if both Shift key and left mouse button are pressed, pass PR_SHIFT|PR_LBUTTON. \~
  */
	void SetFlags(int flags) { m_flags = flags; }

protected:
	unsigned int m_flags; ///< \ru Флаги показывают, какая из кнопок мыши нажата и нажаты ли клавиши Shift и Ctrl на клавиатуре. \en Flags indicate which of mouse buttons is pressed and if Shift and Ctrl keyboard keys are pressed.
};


//------------------------------------------------------------------------------
/** \brief \ru Класс MouseEvent содержит параметры, которые описывают события мыши.
           \en MouseEvent class contains parameters that describe a mouse event. \~
    \ingroup Vision_Process
*/
// ---
class VSN_CLASS MouseEvent : public InputEvent
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  MouseEvent(EventType type);
  /// \ru Конструктор копирования. \en Copy constructor. 
  MouseEvent(const MouseEvent& other);
  /// \ru Деструктор. \en Destructor.
  ~MouseEvent();
public:
  /// \ru True, если левая кнопка мыши нажата. \en True if the left mouse button is pressed.
  bool LButton() const	{ return(m_flags & PR_LBUTTON) != 0; }
  /// \ru True, если средняя кнопка мыши нажата. \en True if the middle mouse button is pressed.
  bool MButton() const	{ return(m_flags & PR_MBUTTON) != 0; }
  /// \ru True, если правая кнопка мыши нажата. \en True if the right mouse button is pressed.
  bool RButton() const	{ return(m_flags & PR_RBUTTON) != 0; }

public:
  /// \ru Возвращает позицию мыши в координатах окна. \en Returns the mouse position in window coordinates.
  const Point& GetScreenPos() const { return m_pntScreen; }
  /** \brief \ru Этот метод получает и хранит информацию о событиях мыши в координатах окна.
             \en This method receives and stores mouse event information in window coordinates. \~
      \param[in] point - \ru Точка в координатах окна.
                         \en Point in window coordinates. \~
  */
  void SetScreenPos(Point& point) { m_pntScreen = point; }
protected:
  Point m_pntScreen;
};


//------------------------------------------------------------------------------
/** \brief \ru Класс WheelEvent содержит параметры, которые описывают события колесика мыши.
           \en WheelEvent class contains parameters that describes a scroll wheel event. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS WheelEvent : public InputEvent
{
public:
  /// \ru Конструктор по умолчанию. \en Default constructor.
  WheelEvent();
  /// \ru Конструктор копирования. \en Copy constructor. 
  WheelEvent(const WheelEvent& other);
  /// \ru Деструктор. \en Destructor.
  ~WheelEvent();
public:
  /// \ru Возвращает измененное значение позиции колесика мыши. \en Returns the mouse wheel position changed value.
	int GetWheelDelta() const { return m_wheelDelta; }
  /** \brief \ru Этот метод устанавливает измененное значение позиции колесика мыши.
             \en This method sets the mouse wheel position changed value. \~
      \param[in] wheelDelta - \ru Новое измененное значение колесика мыши.
                              \en New mouse wheel changed value. \~
  */
	void SetWheelDelta(int wheelDelta);

protected:
	int  m_wheelDelta;
};


//------------------------------------------------------------------------------
/** \brief \ru Событие изменения положения камеры.
           \en Camera position change event. \~
  \ingroup Vision_Process
*/
// ---
class VSN_CLASS CameraEvent : public ProcessEvent
{
public:
  /** \brief \ru Конструктор по умолчанию.
             \en Default constructor. \~ 
    \param[in] x - \ru Значение вдоль оси Ox.
                   \en Value along X-axis. \~
    \param[in] y - \ru Значение вдоль оси Oy.
                   \en Value along Y-axis. \~
  */
  CameraEvent(double x = 0.0, double y = 0.0);

  /// \ru Деструктор. \en Destructor. 
  virtual ~CameraEvent();

public:
  /** \brief \ru Вернуть пользовательскую позицию.
             \en Returns user position. \~ 
    \return \ru Точка.
            \en Point. \~
  */
  const MbCartPoint& GetPosition() const;

  /** \brief \ru Установить позицию.
             \en Sets position. \~ 
    \param[in] point - \ru Точка.
                       \en Point. \~
  */
  void SetPosition( const MbCartPoint& point );

  /** \brief \ru Установить позицию.
             \en Sets position. \~ 
    \param[in] x - \ru Значение вдоль оси Ox.
                   \en Value along X-axis. \~
    \param[in] y - \ru Значение вдоль оси Oy.
                   \en Value along Y-axis. \~
  */
  void SetPosition( double x, double y );

  /** \brief \ru Вернуть нормализацию центра касания.
             \en Returns normalization of touch center. \~ 
    \return \ru Нормализация положения центра касаний.
            \en Normalization of touch center position. \~
  */
  const MbVector& GetNormalizeTouchCenter() const;

  /** \brief \ru Установить нормализацию центра касания.
             \en Sets normalization of touch center. \~ 
    \param[in] normalize - \ru Вектор.
                           \en Vector. \~
  */
  void SetNormalizeTouchCenterPosition( const MbVector& normalize );

  /** \brief \ru Вернуть вектор перемещения.
             \en Returns movement vector. \~ 
    \return \ru Вектор перемещения.
            \en Movement vector. \~
  */
  const MbVector& GetTranslation() const;

  /** \brief \ru Установить вектор перемещения.
             \en Sets movement vector. \~ 
    \param[in] translation - \ru Вектор перемещения.
                             \en Movement vector. \~
  */
  void SetTranslation( const MbVector& translation );

  /** \brief \ru Установить данные для трансформации.
             \en Sets transformation data. \~ 
    \param[in] translation - \ru Вектор перемещения.
                             \en Movement vector. \~
    \param[in] rotation - \ru Угол поворота.
                          \en Rotation angle. \~
    \param[in] zoomFactor - \ru Коэффициент масштабирования.
                            \en Zoom factor. \~
  */
  void SetTransformation( const MbVector& translation, double rotation = 0.0, double scaleFactor = 1.0 );

  /** \brief \ru Вернуть габаритный куб.
             \en Returns bounding box. \~ 
    \return \ru Габаритный куб.
            \en Bounding box. \~
  */
  const MbCube& GetBoundingBox() const;

  /** \brief \ru Установить габаритный куб.
             \en Sets bounding box. \~ 
    \param[in] boundingBox - \ru Габаритный куб.
                             \en Bounding box. \~
  */
  void SetBoundingBox( const MbCube& boundingBox );

  /** \brief \ru Вернуть угол поворота.
             \en Returns rotation angle. \~ 
    \return \ru Угол поворота.
            \en Rotation angle. \~
  */
  double GetRotationAngle() const;

  /** \brief \ru Установить угол поворота.
             \en Sets rotation angle. \~ 
    \param[in] rotation - \ru Угол поворота.
                          \en Rotation angle. \~
  */
  void SetRotation( double rotation );

  /** \brief \ru Вернуть коэффициент масштабирования.
             \en Returns zoom factor. \~ 
    \return \ru Коэффициент масштабирования.
            \en Zoom factor. \~
  */
  double GetZoomFactor() const;

  /** \brief \ru Установить коэффициент масштабирования.
             \en Sets zoom factor. \~ 
    \param[in] zoomFactor - \ru Коэффициент масштабирования.
                            \en Zoom factor. \~
  */
  void SetZoomFactor( double zoomFactor );

  /** \brief \ru Вернуть признак того, что преобразование было установлено.
             \en Returns flag that transformation is set. \~ 
    \return \ru True, если преобразование было установлено.
            \en True if transformation is set. \~
  */
  bool IsTransformation() const;

private:
  MbCartPoint m_position;     ///< \ru Положение пользовательского ввода. \en User input position.
  MbVector m_normalize;       ///< \ru Нормализация положения центра касаний. \en Normalization of touch center position.
  MbVector m_translation;     ///< \ru Данные трансформации. \en Transformation data.
  MbCube m_boundingBox;       ///< \ru Габаритный куб объекта. \en Object bounding box.
  double m_rotation;          ///< \ru Угол поворота. \en Rotation angle.
  double m_zoomFactor;       ///< \ru Коэффициент масштаба. \en Zoom factor.
  bool m_isTransformationIs;  ///< \ru Признак того, что преобразование было установлено. \en Flag that transformation is set.
};

VSN_END_NAMESPACE

#endif /* __VSN_PROCESSEVENT_H */
