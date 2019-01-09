
#ifndef __OPENGLWINDOW_H
#define __OPENGLWINDOW_H

#include <afxwin.h>
#include <vsn_global.h>


VSN_BEGIN_NAMESPACE

class OpenGLWindow : public CWnd
{
public:
  OpenGLWindow();
  virtual ~OpenGLWindow();

protected:
  void Init();

public:
  void MakeCurrent();
  void DoneCurrent();

  // If the GLContext is a widget this function requests a redraw and generates an updateEvent().
  virtual void Update();
  virtual void UpdateSelectionGL();

protected:
  virtual void InitializeOpenGL();
  virtual void PaintOpenGL();
  virtual void ResizeOpenGL(int w, int h);

protected:
  //{{AFX_MSG(MFCContext
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
  DECLARE_MESSAGE_MAP()
private:
  VSN_DECLARE_PRIVATE(OpenGLWindow)
};

VSN_END_NAMESPACE

#endif /* __OPENGLWINDOW_H */

