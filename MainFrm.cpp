// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "ProgressThread.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pProgressCtrl = NULL;
    m_pProgressThread = NULL;
}

CMainFrame::~CMainFrame()
{
    EndProgress();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
#if 0
    if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
#endif
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) /*||
		!m_wndReBar.AddBar(&m_wndDlgBar)*/)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::StartProgress(DWORD dwMax)
{
#if 0
    if (m_pProgressThread == NULL)
    {
        m_pProgressThread = AfxBeginThread(RUNTIME_CLASS(CProgressThread));
        if (m_pProgressThread != NULL)
        {
            m_ProgressInfo.dwLimit = dwMax;
            strncpy(m_ProgressInfo.szTitle, "Saving...", sizeof(m_ProgressInfo.szTitle));
            m_pProgressThread->PostThreadMessage(WU_PROGRESS_START, 0, (LPARAM)&m_ProgressInfo);
        }
    }
#endif
    if (m_pProgressCtrl == NULL)
    {
        RECT rc;
        m_pProgressCtrl = new CProgressCtrl();
        m_wndStatusBar.GetItemRect (0, &rc);
        VERIFY (m_pProgressCtrl->Create(WS_CHILD | WS_VISIBLE, rc,
                                               &m_wndStatusBar, 1));
        m_pProgressCtrl->SetRange32(0, dwMax);
    }
}

BOOL CMainFrame::StepProgress(DWORD dwCurrent)
{
    if (m_pProgressThread != NULL)
    {
        m_ProgressInfo.dwCurrent = dwCurrent;
        m_pProgressThread->PostThreadMessage(WU_PROGRESS_STEP, 0, (LPARAM)&m_ProgressInfo);
    }
    if (m_pProgressCtrl != NULL)
    {
        m_pProgressCtrl->SetPos(dwCurrent);
    }
    return FALSE;
}

void CMainFrame::EndProgress(void)
{
    if (m_pProgressThread != NULL)
    {
        m_pProgressThread->PostThreadMessage(WU_PROGRESS_STOP, 0, 0);
        m_pProgressThread = NULL;
    }
    if (m_pProgressCtrl != NULL)
    {
        m_pProgressCtrl->DestroyWindow();
        delete m_pProgressCtrl;
        m_pProgressCtrl = NULL;
    }
}
