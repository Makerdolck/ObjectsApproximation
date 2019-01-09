#ifndef __PROCESSMANAGER_H
#define __PROCESSMANAGER_H

#include <vector>

#include "openglview.h"
#include "vsn_global.h"


/*! \enum HEventType 
	Defines mouse state, keyboard and timer event types
*/
enum PrEventType
{
	PR_NoEvent,        // No event ooccurred.

	PR_LButtonDown,    // The left mouse button was pressed.
	PR_LButtonUp,      // The left mouse button is up.
	PR_LButtonDblClk,  // The left mouse button was double-clicked.

	PR_MButtonDown,    // The middle mouse button was pressed.
	PR_MButtonUp,      // The middle mouse button is up.
	PR_MButtonDblClk,  // The middle mouse button was double-clicked.

	PR_RButtonDown,    // The right mouse button was pressed.
	PR_RButtonUp,      // The right mouse button is up.
	PR_RButtonDblClk,  // The right mouse button was double-clicked.

	PR_MouseMove,      // The mouse cursor has moved.

	PR_MouseWheel,     // The mouse wheel has moved.
	PR_MouseWheelUp,   // The mouse wheel was scrolled up.
	PR_MouseWheelDown, // The mouse wheel was scrolled down.
	
	PR_Timer,          // self-explanatory

	PR_KeyDown,        // A key on the keyboard was pressed.
	PR_KeyUp,          // A key on the keyboard was released.

	// Higher level
	PR_Selection,       //!< A general selection event.
};


/* PR_CONTROL Any Control Key */
#define PR_CONTROL 1<<1
/* PR_SHIFT Any Shift Key */
#define PR_SHIFT 1<<2
/* PR_ALT Any Alt Key */
#define PR_ALT 1<<3
/* PR_LBUTTON Left Mouse Button */
#define PR_LBUTTON 1<<4
/* PR_MBUTTON Middle Mouse Button */
#define PR_MBUTTON  1<<5
/* PR_RBUTTON Right Mouse Button */
#define PR_RBUTTON 1<<6
/* PR_RIGHT_CONTROL Right Control Button */
#define PR_RIGHT_CONTROL 1<<7
/* PR_RIGHT_SHIFT Right Shift Button */
#define PR_RIGHT_SHIFT 1<<8
/* PR_RIGHT_ALT Right Alt Button */
#define PR_RIGHT_ALT 1<<9
/* PR_LEFT_CONTROL Left Control Button */
#define PR_LEFT_CONTROL 1<<10
/* PR_LEFT_SHIFT Left Shift Button */
#define PR_LEFT_SHIFT 1<<11
/* PR_LEFT_ALT Left Alt Button */
#define PR_LEFT_ALT 1<<12
/* PR_LEFT_ARROW Left Arrow Button */
#define PR_LEFT_ARROW     1<<13
/* PR_UP_ARROW Up Arrow Button */
#define PR_UP_ARROW       1<<14
/* PR_RIGHT_ARROW Right Arrow Button */
#define PR_RIGHT_ARROW    1<<15
/* PR_DOWN_ARROW Down Arrow Button */
#define PR_DOWN_ARROW     1<<16
/* PR_JOYSTICK_BUTTON_1 Joystick Button 1 */
#define PR_JOYSTICK_BUTTON_1 1<<17
/* PR_JOYSTICK_BUTTON_2 Joystick Button 2 */
#define PR_JOYSTICK_BUTTON_2 1<<18
/* PR_JOYSTICK_BUTTON_3 Joystick Button 3 */
#define PR_JOYSTICK_BUTTON_3 1<<19
/* PR_JOYSTICK_BUTTON_4 Joystick Button 4 */
#define PR_JOYSTICK_BUTTON_4 1<<20
/* PR_JOYSTICK_BUTTON_5 Joystick Button 5 */
#define PR_JOYSTICK_BUTTON_5 1<<21
/* PR_JOYSTICK_BUTTON_6 Joystick Button 6 */
#define PR_JOYSTICK_BUTTON_6 1<<22
/* PR_JOYSTICK_BUTTON_7 Joystick Button 7 */
#define PR_JOYSTICK_BUTTON_7 1<<23
/* PR_JOYSTICK_BUTTON_8 Joystick Button 8 */
#define PR_JOYSTICK_BUTTON_8 1<<24


//----------------------------------------------------------------------------
//
/** \brief \ru НЕТ ПЕРЕВОДА.
           \en The ProcessEventEx class stores and manages event information. \~
  \details \ru НЕТ ПЕРЕВОДА. \n
           \en NO TRANSLATION. \n \~
*/
// ---
class ProcessEventEx 
{
public:
  // конструктор по умолчанию
	ProcessEventEx ( OpenGLView* pView ); 
public:
  // EventInfo Initialization
  void SetType( PrEventType type ) { m_eventType = type; }	/* This method sets the event type on the ProcessEventEx object. */
  /*! \return The eventy type associated to this HEventInfo. */
  PrEventType GetType() const { return m_eventType; }


  const MbCartPoint3D& GetMousePixelPos() const { return m_pntPixel; }		/* \return The mouse position in pixel coordinates. */
  const MbCartPoint3D& GetMouseWorldPos() const { return m_pntWorld; }		/* \return The mouse position in world coordinates. */
	/*! 
		This method receives and stores mouse event information from the user interface and translates it for use 
		by Vision functions. 
		\param NewType The type of event.
		\param x The X coordinate of the mouse event, in window space.
		\param y The Y coordinate of the mouse event, in window space.
		\param NewFlags The bitwise MVP flags for mouse buttons, shift and control Key states.  
		For example, if both the shift key and left mouse button are depressed, pass PR_SHIFT|PR_LBUTTON.
	*/
	void SetPoint(PrEventType type, int x, int y, unsigned int flags); 

private:
  OpenGLView* m_pView;
  PrEventType m_eventType;   // Type of event
  unsigned int m_flags;      // Flags indicating which mouse buttons are pressed 
  MbCartPoint3D m_pntPixel;  // Mouse position in windows pixel space
  MbCartPoint3D m_pntWorld;  // Mouse position in Vision world space
};


class BaseProcess;
//----------------------------------------------------------------------------
//
/** \brief \ru НЕТ ПЕРЕВОДА.
           \en NO TRANSLATION. \~
  \details \ru НЕТ ПЕРЕВОДА. \n
           \en NO TRANSLATION. \n \~
*/
// ---
class ProcessManagerEx
{
public:
  // конструктор по умолчанию
	explicit ProcessManagerEx();
  // деструктор
	virtual ~ProcessManagerEx();

public:
  void AppendProcess( BaseProcess* pProcess );
  void RemoveProcess( BaseProcess* pProcess );

public:
  virtual void OnDraw( C3DVision::RenderMode renderingMode );
  virtual void OnMouseMove( ProcessEventEx& event );
  virtual void OnLButtonDown( ProcessEventEx& event );
  virtual void OnLButtonUp( ProcessEventEx& event );

protected:
  std::vector<BaseProcess*> m_stackProcess;
};


//----------------------------------------------------------------------------
//
/** \brief \ru НЕТ ПЕРЕВОДА.
           \en NO TRANSLATION. \~
  \details \ru НЕТ ПЕРЕВОДА. \n
           \en NO TRANSLATION. \n \~
*/
// ---
class BaseProcess
{
public:
  // конструктор по умолчанию
  explicit BaseProcess( OpenGLView* pView );
  // деструктор
  virtual ~BaseProcess();

public:
  OpenGLView* GetView() { return m_pView; }  //!< Returns a pointer to the view that this operator is associated with
  void SetView( OpenGLView* pView) { m_pView = pView; }  //!< Sets the view that this operator is associated with

  MbCube GetTotalBoundingBox() const;

protected:
  virtual void OnDraw( C3DVision::RenderMode renderingMode );
  virtual int OnMouseMove( ProcessEventEx& event );
  virtual int OnLButtonDown( ProcessEventEx& event );
  virtual int OnLButtonUp( ProcessEventEx& event );

protected:
  OpenGLView* m_pView;	//!< The Vision view corresponding to this operator
  SceneContent* m_pSceneContent;
private:
  friend class ProcessManagerEx;
};


//----------------------------------------------------------------------------
//
/** \brief \ru НЕТ ПЕРЕВОДА.
           \en NO TRANSLATION. \~
  \details \ru НЕТ ПЕРЕВОДА. \n
           \en NO TRANSLATION. \n \~
*/
// ---
class ProcessDefault : public BaseProcess
{
public:
  // конструктор по умолчанию
  explicit ProcessDefault( OpenGLView* pView );
  // деструктор
  virtual ~ProcessDefault();
private:
  bool HighlightSegment( uint segmentId );
  void UnhighlightSegment( uint segmentId );
  void DetectSegmentUnderCursor( int x, int y );

protected:
  virtual int OnMouseMove( ProcessEventEx& event );
  virtual int OnLButtonDown( ProcessEventEx& event );
  virtual int OnLButtonUp( ProcessEventEx& event );

protected:
  uint m_highlightSegmentId;
  uint m_currentSegmentId;

private:
  OBVIOUS_PRIVATE_COPY(ProcessDefault);
};


#endif /* __PROCESSMANAGER_H */
