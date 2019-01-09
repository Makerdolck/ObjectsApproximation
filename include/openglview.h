#ifndef __VSN_OPENGLVIEW_H
#define __VSN_OPENGLVIEW_H

#include <afxwin.h>
#include <vsn_light.h>
#include <vsn_viewport.h>
#include <vsn_scenecontent.h>
#include <vsn_processmanager.h>

class ProcessManagerEx;
class BaseProcess;
class ProcessDefault;

////////////////////////////////////////////////////////////////////////////////
// OpenGLView view

class OpenGLView : public CView
{
	DECLARE_DYNCREATE(OpenGLView)
protected:
	OpenGLView();  // protected constructor used by dynamic creation
private:
  /*! Maps the MFC button and Shift/Control state to abstracted Vision values */
  unsigned long MapFlags( unsigned long state ); 	

// Attributes
public:
  // Returns the Viewport pointer
  C3DVision::Viewport* GetViewport() const;
  // Returns the SceneContent pointer
  C3DVision::SceneContent* GetSceneContent() const;
  // Returns the current process pointer
  BaseProcess* GetCurrentProcess() const;

public:
  void MakeCurrent();
  void DoneCurrent();

protected:
  // initialization opengl
  virtual void InitializeOpenGL();
  virtual void PaintOpenGL();
  virtual void ResizeOpenGL(int w, int h);

// Operations
protected:
  MbCube GetTotalBoundingBox() const;
  void StartProcess( BaseProcess* pProcess );
  void StopCurrentProcess();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OpenGLView)
	public:
    //!< We overload the Create method to modify the CWnd window style; necessary for OpenGL support
#ifdef _UNICODE
    virtual BOOL Create(__wchar_t const * lpszClassName, __wchar_t const * lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = V_NULL);
    virtual BOOL Create(unsigned short const * lpszClassName, unsigned short const *  lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = V_NULL);
#else
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = V_NULL);
#endif

	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
  virtual void OnDraw(CDC* pDC);  //!< overridden but currently does nothing
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  //!< We overload the PreCreateWindow method to modify the CWnd window class; necessary for OpenGL support
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~OpenGLView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(OpenGLView)
  //  Overloaded CView method. Instructs Vision to traverse the scene-graph and update the display
  afx_msg void OnPaint();
	/*! 
		Dispatches the OnMouseMove event to all event receivers that are registered for the OnMouseMove event type.  
	*/
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnDestroy();
	afx_msg void OnFileSaveAs();
	afx_msg void OnGouraudShaded();
	afx_msg void OnWireframe();
	afx_msg void OnHiddenline();
	afx_msg void OnZoom();
	afx_msg void OnZoomToExtents();
	afx_msg void OnZoomToWindow();
	afx_msg void OnOrbit();
	afx_msg void OnUpdateOrbit(CCmdUI* pCmdUI);
	afx_msg void OnPan();
	afx_msg void OnUpdatePan(CCmdUI* pCmdUI);
	afx_msg void OnWindowSelect();
	afx_msg void OnUpdateWindowSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomToWindow(CCmdUI* pCmdUI);
	afx_msg void OnRunMyCode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

  afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  VSN_DECLARE_PRIVATE(OpenGLView)
};

#endif // __VSN_OPENGLVIEW_H
