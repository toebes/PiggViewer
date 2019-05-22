// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "PiggViewer.h"

#include "ChildFrm.h"
#include "LeftView.h"
#include "PiggViewerView.h"
#include "PiggViewerDoc.h"
#include "TextView.h"
#include "HexView.h"
#include "TextureView.h"
#include "IndexView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_COMMAND(ID_VIEW_TEXT, OnViewText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEXT, OnUpdateViewStyles)
	ON_COMMAND(ID_VIEW_DETAILS, OnViewDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, OnUpdateViewStyles)
	ON_COMMAND(ID_VIEW_HEX, OnViewHex)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HEX, OnUpdateViewStyles)
	ON_COMMAND(ID_VIEW_ITEM, OnViewItem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITEM, OnUpdateViewStyles)
    ON_MESSAGE( WM_USER_CHECK_VIEW, OnUserCheckView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_dwViewMode = ID_VIEW_ITEM;
    m_pFileViewType = NULL;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
    m_pDocument = DYNAMIC_DOWNCAST(CPiggViewerDoc, pContext->m_pCurrentDoc);

	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 3))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(250, 100), pContext) ||
        !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CIndexView), CSize(200, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CPiggViewerView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
CPiggViewerView* CChildFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CPiggViewerView* pView = DYNAMIC_DOWNCAST(CPiggViewerView, pWnd);
	return pView;
}

void CChildFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
    if (pWnd == NULL)
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable();  // For now, but we need to actually enable only the ones which make sense
        BOOL bChecked = (m_dwViewMode == pCmdUI->m_nID);
        
        pCmdUI->SetRadio(bChecked ? 1 : 0);
    }
}


void CChildFrame::OnViewText() 
{
    if (m_dwViewMode != ID_VIEW_TEXT)
    {
        //
        // We need to replace the view
        //
        SwitchToView(RUNTIME_CLASS(CTextView));
        m_dwViewMode = ID_VIEW_TEXT;
    }
}

void CChildFrame::OnViewDetails() 
{
    if (m_dwViewMode != ID_VIEW_DETAILS)
    {
        //
        // We need to replace the view
        //
    	SwitchToView(RUNTIME_CLASS(CPiggViewerView));
        m_dwViewMode = ID_VIEW_DETAILS;
    }	
}

void CChildFrame::OnViewHex() 
{
    if (m_dwViewMode != ID_VIEW_HEX)
    {
        //
        // We need to replace the view
        //
    	SwitchToView(RUNTIME_CLASS(CHexView));
        m_dwViewMode = ID_VIEW_HEX;
    }	
}

void CChildFrame::OnViewItem() 
{
    if ((m_dwViewMode != ID_VIEW_ITEM) || (m_pDocument->m_ExternalInfo.m_pFileViewType != m_pFileViewType))
    {
        //
        // We need to replace the view
        //
        m_pFileViewType = m_pDocument->m_ExternalInfo.m_pFileViewType;;

        SwitchToView(m_pFileViewType->GetViewClass());
        m_dwViewMode = ID_VIEW_ITEM;
    }	
}


BOOL CChildFrame::SwitchToView(CRuntimeClass *pNewViewClass)
{
    CWnd* pWnd = m_wndSplitter.GetPane(0, 2);
    CView* pView = DYNAMIC_DOWNCAST(CView, pWnd);
    CPiggViewerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    return pDoc->SwitchToView(pNewViewClass, &m_wndSplitter, pView);
    
}

CPiggViewerDoc* CChildFrame::GetDocument()
{
    return m_pDocument;
}



LRESULT CChildFrame::OnUserCheckView(WPARAM, LPARAM)
{
    if (m_dwViewMode == ID_VIEW_ITEM)
    {
        OnViewItem();
    }
    return 0;
}