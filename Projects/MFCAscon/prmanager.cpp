#include "StdAfx.h"
#include "prmanager.h"
#include <vsn_viewport.h>
#include <vsn_glplatformsurface.h>

#include <last.h>

VSN_USE_NAMESPACE


//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ---
ProcessEventEx::ProcessEventEx( OpenGLView* pView )
  : m_pView( pView )
  , m_eventType( PR_NoEvent ) 
  , m_flags( 0 )
{
}

void ProcessEventEx::SetPoint( PrEventType type, int x, int y, unsigned int flags ) 
{
  SetType(type);
  m_flags = flags;

  m_pntPixel.x = static_cast<double>(x); 
  m_pntPixel.y = static_cast<double>(y); 
  m_pntPixel.z = 0.0;

  Viewport* pViewport = m_pView->GetViewport();
  MbCartPoint3D wp;
  pViewport->ConvertScreenPointToParallelScreenPlane( MbCartPoint(static_cast<double>(x), static_cast<double>(y)), MbCartPoint3D(0,0,0), wp );

  m_pntWorld.x = wp.x;
  m_pntWorld.y = wp.y;
  m_pntWorld.z = wp.z;
}


//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ---
ProcessManagerEx::ProcessManagerEx()
{
}

//-----------------------------------------------------------------------------
// деструктор
// ---
ProcessManagerEx::~ProcessManagerEx()
{
}

void ProcessManagerEx::AppendProcess( BaseProcess* pProcess )
{
  m_stackProcess.push_back( pProcess );
}

//-----------------------------------------------------------------------------
//
// ---
void ProcessManagerEx::RemoveProcess( BaseProcess* pProcess )
{
  for (std::vector<BaseProcess*>::iterator it = m_stackProcess.begin(); it != m_stackProcess.end(); ++it)
  {
    if (pProcess == *it)
    {
      m_stackProcess.erase(it);
      break;
    }
  }
}

//-----------------------------------------------------------------------------
//
// ---
void ProcessManagerEx::OnDraw( C3DVision::RenderMode renderingMode )
{
  for (std::vector<BaseProcess*>::iterator it = m_stackProcess.begin(); it != m_stackProcess.end(); ++it)
    (*it)->OnDraw( renderingMode );
}

//-----------------------------------------------------------------------------
//
// ---
void ProcessManagerEx::OnMouseMove( ProcessEventEx& event )
{
  if (m_stackProcess.empty())
    return;
  if (BaseProcess* pBaseProcess = m_stackProcess.back())
    pBaseProcess->OnMouseMove( event );
}

//-----------------------------------------------------------------------------
//
// ---
void ProcessManagerEx::OnLButtonDown( ProcessEventEx& event )
{
  if (m_stackProcess.empty())
    return;

  if (BaseProcess* pBaseProcess = m_stackProcess.back())
    pBaseProcess->OnLButtonDown( event );
}

//-----------------------------------------------------------------------------
//
// ---
void ProcessManagerEx::OnLButtonUp( ProcessEventEx& event )
{
  if (m_stackProcess.empty())
    return;

  if (BaseProcess* pBaseProcess = m_stackProcess.back())
    pBaseProcess->OnLButtonUp( event );
}



//-----------------------------------------------------------------------------
// конструктор по умолчанию
// ---
BaseProcess::BaseProcess( OpenGLView* pView )
  : m_pView( pView )
  , m_pSceneContent( new SceneContent )
{
  m_pSceneContent->SetViewport( m_pView->GetViewport() );
}

//-----------------------------------------------------------------------------
// деструктор
// ---
BaseProcess::~BaseProcess()
{
  delete m_pSceneContent;
}

//-----------------------------------------------------------------------------
//
// ---
MbCube BaseProcess::GetTotalBoundingBox() const
{
  return m_pSceneContent->GetBoundingBox();
}

//-----------------------------------------------------------------------------
//
// ---
void BaseProcess::OnDraw( C3DVision::RenderMode /*renderingMode*/ )
{
  if ( !m_pSceneContent->IsEmpty() )
  {
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    m_pSceneContent->OGLDraw(0, C3DVision::ShadedWithEdgesMode);
    m_pSceneContent->OGLDraw(0, C3DVision::TransparentMode);

    // Restore 3DState
    glPopAttrib();
    glDisable(GL_BLEND);
  }
}

//-----------------------------------------------------------------------------
//
// ---
int BaseProcess::OnMouseMove( ProcessEventEx& event )
{
  return -1;
}

//-----------------------------------------------------------------------------
//
// ---
int BaseProcess::OnLButtonDown( ProcessEventEx& event )
{
  return -1;
}

//-----------------------------------------------------------------------------
//
// ---
int BaseProcess::OnLButtonUp( ProcessEventEx& event )
{
  return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
// ProcessDefault
//
///////////////////////////////////////////////////////////////////////////////
// конструктор по умолчанию
ProcessDefault::ProcessDefault( OpenGLView* pView )
  : BaseProcess(pView)
  , m_highlightSegmentId( -1 )
  , m_currentSegmentId( -1 )
{
}

//-----------------------------------------------------------------------------
// деструктор
// ----
ProcessDefault::~ProcessDefault()
{
}

//-----------------------------------------------------------------------------
//
// ----
bool ProcessDefault::HighlightSegment( uint segmentId )
{
  if ( segmentId == -1 )
    return false;

  SceneContent* pSceneContent = m_pView->GetSceneContent();
  ASSERT( pSceneContent != V_NULL );

	if ( pSceneContent->IsExistSegment(segmentId) )
	{
		//	----------------------------------------------------------------
		//ViewContainer3D* viewCont = pSceneContent->GetContainer();
		//bool flag = viewCont->IsSelected(segmentId);

		/*ViewContainer3D* pContainer3D = pSceneContent->GetContainer();

		std::list <ViewObject3D*> pObject3D_My = pContainer3D->GetObjects();
		bool flagObjectExist = false;
		for (ViewObject3D* obj : pObject3D_My)
		{
			if (obj->GetId() == segmentId)
			{
				flagObjectExist = true;
				break;
			}
		}

		if (flagObjectExist)*/
			
		if (pSceneContent->GetContainer()->IsExistViewObject3D(segmentId))

		//	----------------------------------------------------------------
		if ( ViewObject3D* pObject3D = pSceneContent->GetContainer()->GetObjectById(segmentId) )
		{
			pObject3D->GetRenderState()->SetSpecialMaterial(new Material(Color(255, 255, 0)));
			pObject3D->GetRenderState()->SetRenderingState( C3DVision::SpecialMaterialState );

			OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
			_ASSERT(pContext != V_NULL && pContext->IsValid());
			pContext->GetSurface()->Update();
			return true;
		}
	}
  return false;
}

//-----------------------------------------------------------------------------
//
// ----
void ProcessDefault::UnhighlightSegment( uint segmentId )
{
  if ( segmentId == -1 )
    return;

  SceneContent* pSceneContent = m_pView->GetSceneContent();
  ASSERT( pSceneContent != V_NULL );


	//	----------------------------------------------------------------
	if (pSceneContent->GetContainer()->IsExistViewObject3D(segmentId))

	//	----------------------------------------------------------------

  if ( ViewObject3D* pObject3D = pSceneContent->GetContainer()->GetObjectById(segmentId) )
    pObject3D->GetRenderState()->SetRenderingState( C3DVision::NormalRenderState );

  OpenGLContext* pContext = OpenGLContext::GetCurrentContext();
  _ASSERT(pContext != V_NULL && pContext->IsValid());
  pContext->GetSurface()->Update();
}

//-----------------------------------------------------------------------------
//
// ----
void ProcessDefault::DetectSegmentUnderCursor( int x, int y )
{
	try
	{
		Viewport* pViewport = m_pView->GetViewport();
		ASSERT(pViewport != V_NULL);
		SceneContent* pSceneContent = m_pView->GetSceneContent();
		ASSERT(pSceneContent != V_NULL);

		pSceneContent->GetContainer()->SetLevelOfDetailEnabled(true, pViewport);
		//  pViewport->glPerformCamera();

		//	----------------------------------------------------------------
		if(pViewport->Pick(x, y)!=0)
			m_currentSegmentId = pViewport->PickRender(x, y);
		//	----------------------------------------------------------------

		//m_currentSegmentId = pViewport->PickRender(x, y);
		pSceneContent->GetContainer()->SetLevelOfDetailEnabled(false, pViewport);
	}
	catch (const std::exception&)
	{
		//
	}
 
}

//-----------------------------------------------------------------------------
//
// ----
int ProcessDefault::OnMouseMove( ProcessEventEx& event )
{
  MbCartPoint3D pnt = event.GetMousePixelPos();
  DetectSegmentUnderCursor( static_cast<int>(pnt.x), static_cast<int>(pnt.y) );

  SceneContent* pSceneContent = m_pView->GetSceneContent();
  ASSERT( pSceneContent != V_NULL );
  if ( pSceneContent->IsExistSegment(m_currentSegmentId) )
  {
    if ( m_currentSegmentId != m_highlightSegmentId )
    {
      if (m_highlightSegmentId != -1)
      {
        UnhighlightSegment( m_highlightSegmentId );
        m_highlightSegmentId = -1;
      }

      if ( HighlightSegment(m_currentSegmentId) )
        m_highlightSegmentId = m_currentSegmentId;
    }
  }
  else if (m_highlightSegmentId != -1)
  {
    UnhighlightSegment( m_highlightSegmentId );
    m_highlightSegmentId = -1;
  }
  return -1;
}

//-----------------------------------------------------------------------------
//
// ----
int ProcessDefault::OnLButtonDown( ProcessEventEx& event )
{
  return -1;
}

//-----------------------------------------------------------------------------
//
// ----
int ProcessDefault::OnLButtonUp( ProcessEventEx& event )
{
  return -1;
}
