#include "stdafx.h"
#include "openglwindow.h"

#include <vsn_window.h>
#include <vsn_openglcontext.h>
#include <vsn_processevent.h>
#include <vsn_vision.h>
#include <last.h>

VSN_BEGIN_NAMESPACE

/* OpenGLWindowPrivate */
class OpenGLWindowPrivate
{
public:
  VSN_DECLARE_PUBLIC(OpenGLWindow)
public:
  explicit OpenGLWindowPrivate();
  virtual ~OpenGLWindowPrivate();

public:
  void Initialize();
  void InvokeUserPaint();
  void Render();
  void Reset();

public:
  Window* m_pWindow;
  OpenGLContext* m_pContext;
  BOOL m_bInitialized;
  BOOL m_bInPaintOpenGL;
  GLSurfaceFormat m_requestedFormat;
private:
  VSN_DECLARE_PRIVATE(OpenGLWindowPrivate)
};

OpenGLWindowPrivate::OpenGLWindowPrivate()
  : m_pWindow(V_NULL)
  , m_pContext(V_NULL)
  , m_bInitialized(FALSE)
  , m_bInPaintOpenGL(FALSE)
{
  m_requestedFormat = GLSurfaceFormat::GetDefaultFormat();
}

OpenGLWindowPrivate::~OpenGLWindowPrivate()
{
}

void OpenGLWindowPrivate::Initialize()
{
  VSN_P(OpenGLWindow)
  if (m_bInitialized)
    return;

  m_requestedFormat.SetSamples(4);
  m_requestedFormat.SetDepthBufferSize(24);
  m_requestedFormat.SetStencilBufferSize(8);

  m_pWindow = new Window();
  m_pContext = new OpenGLContext();
  m_pContext->SetFormat(m_requestedFormat);
  m_pWindow->Create(p.m_hWnd, m_pContext);

  m_bInitialized = TRUE;
  p.InitializeOpenGL();
}

void OpenGLWindowPrivate::InvokeUserPaint()
{
  VSN_P(OpenGLWindow)
  OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
  _ASSERT(pContext != V_NULL);

  m_bInPaintOpenGL = TRUE;

  p.PaintOpenGL();
  pContext->SwapBuffers(m_pWindow);
  p.ValidateRect(V_NULL);

  m_bInPaintOpenGL = FALSE;
}

void OpenGLWindowPrivate::Render()
{
  VSN_P(OpenGLWindow)
  if (!m_bInitialized)
    return;
  p.MakeCurrent();

  InvokeUserPaint();
}

void OpenGLWindowPrivate::Reset()
{
  VSN_P(OpenGLWindow)

  if (m_bInitialized)
    p.MakeCurrent();

  if (m_bInitialized)
    p.DoneCurrent();

  delete m_pContext;
  m_pContext = V_NULL;

  delete m_pWindow;
  m_pWindow = 0;

  m_bInitialized = FALSE;
}

BEGIN_MESSAGE_MAP(OpenGLWindow, CWnd)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_PAINT()
  ON_WM_SIZE()
END_MESSAGE_MAP()


OpenGLWindow::OpenGLWindow()
{
  VSN_INIT_PRIVATE(OpenGLWindow);
}

OpenGLWindow::~OpenGLWindow()
{
  VSN_FINI_PRIVATE();
}

void OpenGLWindow::MakeCurrent()
{
  VSN_D(OpenGLWindow)
  if (!d.m_bInitialized)
    return;
  _ASSERT(d.m_pContext != V_NULL);
  d.m_pContext->MakeCurrent(d.m_pWindow);
}

void OpenGLWindow::DoneCurrent()
{
  VSN_D(OpenGLWindow)
  if (!d.m_bInitialized)
    return;

  _ASSERT(d.m_pContext != V_NULL);
  d.m_pContext->DoneCurrent();
}

void OpenGLWindow::Update()
{
}

void OpenGLWindow::UpdateSelectionGL()
{
}

void OpenGLWindow::InitializeOpenGL()
{
}

void OpenGLWindow::PaintOpenGL()
{
}

void OpenGLWindow::ResizeOpenGL(int w, int h)
{
  w = w; h = h;
}

//-----------------------------------------------------------------------------
int OpenGLWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  VSN_D(OpenGLWindow)
  d.Initialize();
  return 0;
}

//-----------------------------------------------------------------------------
void OpenGLWindow::OnDestroy()
{
  VSN_D(OpenGLWindow)
  d.Reset();
  CWnd::OnDestroy();
}

void OpenGLWindow::OnPaint()
{
  VSN_D(OpenGLWindow)
  if (!d.m_bInitialized)
    return;
  d.Render();
}


//-----------------------------------------------------------------------------
//
// ---
void OpenGLWindow::OnSize(UINT nType, int cx, int cy)
{
  VSN_D(OpenGLWindow)
  CWnd::OnSize(nType, cx, cy);

  if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED)
    return;

  d.Initialize();

  if (!d.m_bInitialized)
    return;

  ResizeOpenGL(cx, cy);
}

VSN_END_NAMESPACE



