#include "StdAfx.h"

#include "openglview.h"
#include <vsn_window.h>
#include <vsn_vision.h>

#include "resource.h"
#include "prmanager.h"


/* OpenGLViewPrivate */
class OpenGLViewPrivate
{
public:
  VSN_DECLARE_PUBLIC(OpenGLView)
public:
  explicit OpenGLViewPrivate();
  virtual ~OpenGLViewPrivate();
  void InitPrivate();
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

  C3DVision::Viewport* m_pViewPort;
  C3DVision::Light* m_pLight;
  C3DVision::SceneContent* m_pSceneContent;
  C3DVision::ProcessManager* m_pProcessManager;
  C3DVision::RenderMode m_renderMode; // render mode for this view
  ProcessManagerEx* m_pProcessManagerEx;
  BaseProcess* m_pCurrentProcess;
  ProcessDefault* m_pDefaultProcess;
  uint m_aciveInteractorStyle;

private:
  VSN_DECLARE_PRIVATE(OpenGLView)
};

OpenGLViewPrivate::OpenGLViewPrivate()
  : m_pWindow(V_NULL)
  , m_pContext(V_NULL)
  , m_bInitialized(FALSE)
  , m_bInPaintOpenGL(FALSE)
  , m_pViewPort( new Viewport )
  , m_pLight( new Light )
  , m_pSceneContent( new SceneContent )
  , m_pProcessManager( new ProcessManager )
  , m_renderMode( C3DVision::ShadedWithEdgesMode )
  , m_pProcessManagerEx( new ProcessManagerEx() )
  , m_pCurrentProcess( V_NULL )
  , m_pDefaultProcess( V_NULL )
  , m_aciveInteractorStyle( ProcessManager::PR_NoActive )
{
  m_requestedFormat = GLSurfaceFormat::GetDefaultFormat();
  m_pViewPort->GetCamera()->SetDefaultUpVector( C3DVision::Y_AXIS );
  m_pViewPort->GetCamera()->SetViewOrientation( C3DVision::IsoXYZ );

  m_pLight->SetType(C3DVision::DirectionLight);
  m_pLight->SetPosition(10.0, 20.0, 100.0);

  m_pSceneContent->SetViewport(m_pViewPort);

  m_pProcessManager->AddProcess( new PrCameraPan   (m_pViewPort), ProcessManager::PR_Panoramic );
  m_pProcessManager->AddProcess( new PrCameraZoom  (m_pViewPort), ProcessManager::PR_Zoom );
  m_pProcessManager->AddProcess( new PrCameraRotate(m_pViewPort), ProcessManager::PR_Rotate);
}

OpenGLViewPrivate::~OpenGLViewPrivate()
{
  delete m_pViewPort;
  delete m_pLight;
  delete m_pSceneContent;
  delete m_pProcessManager;

  m_pProcessManagerEx->RemoveProcess( m_pDefaultProcess );
  delete m_pProcessManagerEx;
  delete m_pDefaultProcess;
}

void OpenGLViewPrivate::InitPrivate()
{
  VSN_P(OpenGLView)
  m_pDefaultProcess = new ProcessDefault(&p);
  m_pProcessManagerEx->AppendProcess( m_pDefaultProcess );
}

void OpenGLViewPrivate::Initialize()
{
  VSN_P(OpenGLView)
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

void OpenGLViewPrivate::InvokeUserPaint()
{
  VSN_P(OpenGLView)
  OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
  _ASSERT(pContext != V_NULL);
  CRect rc;
  p.GetClientRect(&rc);
  m_pViewPort->SetSizeViewport(rc.Width(), rc.Height());

  m_bInPaintOpenGL = TRUE;

  p.PaintOpenGL();
//  bool isSelectionMode = GlobalFlags::IsSelectionMode();
  if (!GlobalFlags::IsSelectionMode())
  {
    pContext->SwapBuffers(m_pWindow);
    p.ValidateRect(V_NULL);
  }

  m_bInPaintOpenGL = FALSE;
}

void OpenGLViewPrivate::Render()
{
  VSN_P(OpenGLView)
  if (!m_bInitialized)
    return;
  p.MakeCurrent();

  InvokeUserPaint();
}

void OpenGLViewPrivate::Reset()
{
  VSN_P(OpenGLView)

  // Destroy the OpenGL resources first. These need the context to be current.
  if (m_bInitialized)
    p.MakeCurrent();

  if (m_bInitialized)
    p.DoneCurrent();

  // Delete the context first, then the surface. Slots connected to
  // the context's aboutToBeDestroyed() may still call makeCurrent()
  // to perform some cleanup.
  delete m_pContext;
  m_pContext = V_NULL;

  delete m_pWindow;
  m_pWindow = 0;

  m_bInitialized = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLView

IMPLEMENT_DYNCREATE(OpenGLView, CView)
BEGIN_MESSAGE_MAP(OpenGLView, CView)
  //{{AFX_MSG_MAP(OpenGLView)
  ON_WM_PAINT()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_DESTROY()
  ON_WM_SETCURSOR()
  ON_WM_SIZE()
  ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
  ON_COMMAND(ID_GOURAUD_SHADED, OnGouraudShaded)
  ON_COMMAND(ID_WIREFRAME, OnWireframe)
  ON_COMMAND(ID_HIDDENLINE, OnHiddenline)
  ON_COMMAND(ID_ZOOM, OnZoom)
  ON_COMMAND(ID_ZOOM_TO_EXTENTS, OnZoomToExtents)
  ON_COMMAND(ID_ZOOM_TO_WINDOW, OnZoomToWindow)
  ON_COMMAND(ID_ORBIT, OnOrbit)
  ON_UPDATE_COMMAND_UI(ID_ORBIT, OnUpdateOrbit)
  ON_COMMAND(ID_PAN, OnPan)
  ON_UPDATE_COMMAND_UI(ID_PAN, OnUpdatePan)
  ON_COMMAND(ID_WINDOW_SELECT, OnWindowSelect)
  ON_UPDATE_COMMAND_UI(ID_WINDOW_SELECT, OnUpdateWindowSelect)
  ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateZoom)
  ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_WINDOW, OnUpdateZoomToWindow)
  //}}AFX_MSG_MAP
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//
// ---
OpenGLView::OpenGLView()
{
  VSN_INIT_PRIVATE(OpenGLView);
  VSN_D(OpenGLView)
  d.InitPrivate();
}

//-----------------------------------------------------------------------------
//
// ---
OpenGLView::~OpenGLView()
{
  MakeCurrent();
  VSN_FINI_PRIVATE();
}

unsigned long OpenGLView::MapFlags( unsigned long state )
{
  unsigned long flags = 0;

  /*map the mfc events state to Vision*/
  if(state & MK_LBUTTON) flags|= PR_LBUTTON;
  if(state & MK_RBUTTON) flags|= PR_RBUTTON;
  if(state & MK_MBUTTON) flags|= PR_MBUTTON;
  if(state & MK_SHIFT) flags  |= PR_SHIFT;
  if(state & MK_CONTROL) flags|= PR_CONTROL;

  return flags;
}

C3DVision::Viewport* OpenGLView::GetViewport() const 
{ 
  VSN_D(const OpenGLView)
  return d.m_pViewPort; 
}

C3DVision::SceneContent* OpenGLView::GetSceneContent() const 
{ 
  VSN_D(const OpenGLView)
  return d.m_pSceneContent; 
}

//-----------------------------------------------------------------------------
// Returns the current process pointer
// ---
BaseProcess* OpenGLView::GetCurrentProcess() const
{
  VSN_D(const OpenGLView)
  return d.m_pCurrentProcess; 
}

void OpenGLView::MakeCurrent()
{
  VSN_D(OpenGLView)
  if (!d.m_bInitialized)
    return;
  _ASSERT(d.m_pContext != V_NULL);
  d.m_pContext->MakeCurrent(d.m_pWindow);
}

void OpenGLView::DoneCurrent()
{
  VSN_D(OpenGLView)
  if (!d.m_bInitialized)
    return;

  _ASSERT(d.m_pContext != V_NULL);
  d.m_pContext->DoneCurrent();
}

void OpenGLView::InitializeOpenGL()
{
  VSN_D(OpenGLView)
  d.m_pViewPort->InitOpenGl();
  MakeCurrent();
  GlobalFlags::Initialize();
  glEnable(GL_NORMALIZE);

  d.m_pViewPort->SetBackgroundColour(Color(255, 255, 255));

  MbCube& boundingBox = d.m_pSceneContent->GetBoundingBox();
  if (!boundingBox.IsEmpty())
    d.m_pViewPort->ZoomToFit(boundingBox);
}

void OpenGLView::PaintOpenGL()
{
  VSN_D(OpenGLView)

  OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
  _ASSERT(pContext != V_NULL);

  if (pContext->IsValid())
  {
    bool isSelectionMode = GlobalFlags::IsSelectionMode();

    d.m_pViewPort->SetDistanceClippingPlane( GetTotalBoundingBox() );
    d.m_pSceneContent->GetContainer()->UpdateObjectVisibleState();
//    OpenGLFunctions* func = OpenGLContext::GetCurrentContext()->functions();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // загрузить ед. матрицу
    pContext->oglLoadIdentity();

    if ( !isSelectionMode )
      d.m_pLight->oglPerform();

    d.m_pViewPort->SetClipPlane(true);

    if (!d.m_pSceneContent->IsEmpty())
    {
      d.m_pSceneContent->OGLDraw(0, d.m_renderMode);
      if ( !isSelectionMode )
        d.m_pSceneContent->OGLDraw(0, C3DVision::TransparentMode);
    }

    d.m_pViewPort->SetClipPlane(false);
    d.m_pProcessManagerEx->OnDraw( d.m_renderMode );
  }
}

void OpenGLView::ResizeOpenGL(int w, int h)
{
  VSN_D(OpenGLView)
//  MakeCurrent();
//  d.m_pViewPort->SetSizeViewport(w, h);
//  UpdateWindow();
}

//-----------------------------------------------------------------------------
//
// ---
MbCube OpenGLView::GetTotalBoundingBox() const
{
  VSN_D(const OpenGLView)
  MbCube boundingBox = d.m_pSceneContent->GetBoundingBox();
  if (d.m_pCurrentProcess)
    boundingBox |= d.m_pCurrentProcess->GetTotalBoundingBox();

  return boundingBox;
}

//-----------------------------------------------------------------------------
//
// ---
void OpenGLView::StartProcess( BaseProcess* pProcess )
{
  VSN_D(OpenGLView)
  if (pProcess == V_NULL)
    return;

  OnWindowSelect();
  d.m_pCurrentProcess = pProcess;
  d.m_pProcessManagerEx->AppendProcess(d.m_pCurrentProcess);
}

//-----------------------------------------------------------------------------
//
// ---
void OpenGLView::StopCurrentProcess()
{
  VSN_D(OpenGLView)
  if (d.m_pCurrentProcess == V_NULL)
    return;

  if (d.m_pCurrentProcess != d.m_pDefaultProcess)
  {
    d.m_pProcessManagerEx->RemoveProcess(d.m_pCurrentProcess);
    delete d.m_pCurrentProcess;
    d.m_pCurrentProcess = V_NULL;
    d.m_pCurrentProcess = d.m_pDefaultProcess;
  }
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLView message handlers

//-------------------------------------------------------------------------------
// resets the camera to view the world space extents of the model
// ---
void OpenGLView::OnZoomToExtents() 
{
  VSN_D(OpenGLView)
  if ( !GetTotalBoundingBox().IsEmpty() )
    d.m_pViewPort->ZoomToFit( GetTotalBoundingBox() );
  Invalidate();
}

//-------------------------------------------------------------------------------
// sets wireframe render mode
// ---
void OpenGLView::OnWireframe() 
{
  VSN_D(OpenGLView)
  d.m_renderMode = C3DVision::WireframeMode;
  Invalidate();
}

//-------------------------------------------------------------------------------
// sets gouraud-shaded render mode
// ---
void OpenGLView::OnGouraudShaded() 
{
  VSN_D(OpenGLView)
  d.m_renderMode = C3DVision::ShadedWithEdgesMode;
  Invalidate();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnHiddenline() 
{
  VSN_D(OpenGLView)
  d.m_renderMode = C3DVision::HiddenLinesVisibleMode;
  Invalidate();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnZoom() 
{
  VSN_D(OpenGLView)
  StopCurrentProcess();
  d.m_aciveInteractorStyle = ProcessManager::PR_Zoom;
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnUpdateZoom(CCmdUI* pCmdUI) 
{
  VSN_D(OpenGLView)
  pCmdUI->SetCheck(d.m_aciveInteractorStyle == ProcessManager::PR_Zoom);	
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnZoomToWindow() 
{
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnOrbit() 
{
  VSN_D(OpenGLView)
  StopCurrentProcess();
  d.m_aciveInteractorStyle = ProcessManager::PR_Rotate;
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnUpdateOrbit(CCmdUI* pCmdUI) 
{
  VSN_D(OpenGLView)
  pCmdUI->SetCheck(d.m_aciveInteractorStyle == ProcessManager::PR_Rotate);
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnPan() 
{
  VSN_D(OpenGLView)
  StopCurrentProcess();
  d.m_aciveInteractorStyle = ProcessManager::PR_Panoramic;
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnUpdatePan(CCmdUI* pCmdUI) 
{
  VSN_D(OpenGLView)
  pCmdUI->SetCheck(d.m_aciveInteractorStyle == ProcessManager::PR_Panoramic);
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnWindowSelect()
{
  VSN_D(OpenGLView)
  d.m_aciveInteractorStyle = ProcessManager::PR_NoActive;
  StopCurrentProcess();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnUpdateZoomToWindow(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(0);	
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnUpdateWindowSelect(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(0);
}

//-------------------------------------------------------------------------------
//
// ---
#ifdef _UNICODE
BOOL OpenGLView::Create(wchar_t const * lpszClassName, wchar_t const * lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
  dwStyle = (dwStyle | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED);
  return CWnd::Create((LPCTSTR)lpszClassName, (LPCTSTR)lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
BOOL OpenGLView::Create(unsigned short const * lpszClassName, unsigned short const * lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
  dwStyle = (dwStyle | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED);
  return CWnd::Create((LPCTSTR)lpszClassName, (LPCTSTR)lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
#else
BOOL OpenGLView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
  dwStyle = (dwStyle | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED);
  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
#endif

//-------------------------------------------------------------------------------
// this functions creates a new CView Object, and initializes its camera,
// render mode, query dialog, and operator
void OpenGLView::OnInitialUpdate() 
{
  VSN_D(OpenGLView)
	// we must call the base class initialization first to get the proper palette for this view
	// (either the same palette is shared among all views, or this view has a unique palette)
	CView::OnInitialUpdate();
  d.m_pCurrentProcess = d.m_pDefaultProcess;
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnDraw(CDC* pDC)
{
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrint(pDC, pInfo);
}

//-------------------------------------------------------------------------------
//
// ---
BOOL OpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.lpszClass = AfxRegisterWndClass(CS_OWNDC|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW);
  return CView::PreCreateWindow(cs);
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnPaint()
{
  VSN_D(OpenGLView)
  CView::OnPaint();
  if (!d.m_bInitialized)
    return;
  d.Render();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
  VSN_D(OpenGLView)
  CView::OnMouseMove(nFlags, point);
  if (d.m_pProcessManager->IsExistActiveProcess())
  {
    CameraEvent event(static_cast<double>(point.x), static_cast<double>(point.y));
    d.m_pProcessManager->MoveActivateProcess( &event );
    Invalidate();
  }
  else
  {
    ProcessEventEx ev( this );
    ev.SetPoint(PR_MouseMove, point.x, point.y, MapFlags(nFlags));
    d.m_pProcessManagerEx->OnMouseMove(ev);
  }
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
  VSN_D(OpenGLView)
  CView::OnLButtonDown(nFlags, point);
  if (d.m_aciveInteractorStyle != ProcessManager::PR_NoActive)
  {
    CameraEvent event(static_cast<double>(point.x), static_cast<double>(point.y));
    d.m_pProcessManager->ActivateProcess( d.m_aciveInteractorStyle, &event );
  }
  else
  {
    ProcessEventEx ev( this );
    ev.SetPoint(PR_LButtonDown, point.x, point.y, MapFlags(nFlags));
    d.m_pProcessManagerEx->OnLButtonDown(ev);
  }
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
  VSN_D(OpenGLView)
  CView::OnLButtonUp(nFlags, point);
  if (d.m_aciveInteractorStyle != ProcessManager::PR_NoActive)
  {
    d.m_pProcessManager->DeactivateProcess();
  }
  else
  {
    ProcessEventEx ev( this );
    ev.SetPoint(PR_LButtonUp, point.x, point.y, MapFlags(nFlags));
    d.m_pProcessManagerEx->OnLButtonUp(ev);
  }
}

//-------------------------------------------------------------------------------
//
// ---
BOOL OpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  VSN_D(OpenGLView)
  if (zDelta > 0)
  {
    double factor = 1.28;
    d.m_pViewPort->GetCamera()->SetZoom(factor);
  }
  else
  {
    double factor = 1.0 / 1.28;
    d.m_pViewPort->GetCamera()->SetZoom( factor );
  }
  Invalidate();
  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnDestroy()
{
  StopCurrentProcess();
//  DestroyContext();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnFileSaveAs() 
{
/*
	CString filter = _T("C3D Stream File (*.c3d)|*.hsf|C3D Metafile (*.c3d)|*.c3d||");

	CFileDialog my_dlg(FALSE, _T(".c3d"), V_NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN, 
						filter, V_NULL);

	CString pathname;
	int width = 8, height = 10;

	CSampleHDoc * pDoc = (CSampleHDoc *)GetDocument();

	if (my_dlg.DoModal())
	{
	}
*/
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
  if (OpenGLView* pDeactiveGlView = dynamic_cast<OpenGLView*>(pDeactiveView))
  {
    pDeactiveGlView->DoneCurrent();
    pDeactiveGlView->UpdateWindow();
  }

  if (OpenGLView* pActivateGlView = dynamic_cast<OpenGLView*>(pActivateView))
  {
    pActivateGlView->MakeCurrent();
    pActivateGlView->UpdateWindow();
  }

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//-----------------------------------------------------------------------------
//
// ---
BOOL OpenGLView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
  // let the processing continue - a child window may want to set one of its own
  return FALSE;
}

//-----------------------------------------------------------------------------
//
// ---
void OpenGLView::OnSize(UINT nType, int cx, int cy)
{
  VSN_D(OpenGLView)
  CView::OnSize(nType, cx, cy);

  if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED)
    return;

  d.Initialize();

  if (!d.m_bInitialized)
    return;

  ResizeOpenGL(cx, cy);
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLView diagnostics

#ifdef _DEBUG
//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::AssertValid() const
{
  CView::AssertValid();
}

//-------------------------------------------------------------------------------
//
// ---
void OpenGLView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG


void OpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	VSN_D(OpenGLView)

	CameraEvent event(static_cast<double>(point.x), static_cast<double>(point.y));
	d.m_pProcessManager->ActivateProcess(ProcessManager::PR_Panoramic, &event);
	CView::OnMButtonDown(nFlags, point);
}


void OpenGLView::OnMButtonUp(UINT nFlags, CPoint point)
{
	VSN_D(OpenGLView)

	CView::OnMButtonUp(nFlags, point);
	d.m_pProcessManager->DeactivateProcess();
}


void OpenGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	VSN_D(OpenGLView)

	CameraEvent event(static_cast<double>(point.x), static_cast<double>(point.y));
	d.m_pProcessManager->ActivateProcess(ProcessManager::PR_Rotate, &event);
	CView::OnRButtonDown(nFlags, point);
}


void OpenGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	VSN_D(OpenGLView)

	CView::OnRButtonUp(nFlags, point);
	d.m_pProcessManager->DeactivateProcess();
}
