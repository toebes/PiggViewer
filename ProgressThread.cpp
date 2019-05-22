// ProgressThread.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "ProgressThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressThread

IMPLEMENT_DYNCREATE(CProgressThread, CWinThread)

CProgressThread::CProgressThread()
{
    m_pProgressDialog = NULL;
}

CProgressThread::~CProgressThread()
{
}

BOOL CProgressThread::InitInstance()
{
	return TRUE;
}

int CProgressThread::ExitInstance()
{
    if (m_pProgressDialog != NULL)
    {
        m_pProgressDialog->DestroyWindow();
        m_pProgressDialog = NULL;
    }
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProgressThread, CWinThread)
	//{{AFX_MSG_MAP(CProgressThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
    ON_THREAD_MESSAGE(WU_PROGRESS_START,   OnStart)
	ON_THREAD_MESSAGE(WU_PROGRESS_STOP, OnStop)
    ON_THREAD_MESSAGE(WU_PROGRESS_FILANEME,   OnFilename)
	ON_THREAD_MESSAGE(WU_PROGRESS_STEP, OnStep)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressThread message handlers
void CProgressThread::OnStart(WPARAM, LPARAM lp)
{
    CWnd *pWndProgressControl;
    CWnd *pWndProgressText;
            
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    m_pProgressDialog = new CProgressDialog(CWnd::FromHandle(pProgressInfo->m_hwnd));
    m_pProgressDialog->Create(CProgressDialog::IDD, CWnd::FromHandle(pProgressInfo->m_hwnd));
    m_pProgressDialog->SetWindowText(pProgressInfo->szTitle);
    pWndProgressControl = m_pProgressDialog->GetDlgItem(IDC_PROGRESS);
    pWndProgressText = m_pProgressDialog->GetDlgItem(IDC_PROGRESS_TEXT);
    pWndProgressControl->ShowWindow(pProgressInfo->dwLimit != 0);
    pWndProgressText->ShowWindow(pProgressInfo->dwLimit == 0);
    m_pProgressDialog->ShowWindow(TRUE);
    
    //	ty_args* pargs = (ty_args*)lp;
    ::PulseEvent(pProgressInfo->m_hEvent);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressThread::OnStop(WPARAM, LPARAM)
{
    if (m_pProgressDialog != NULL)
    {
        m_pProgressDialog->DestroyWindow();
        m_pProgressDialog = NULL;
    }
	PostQuitMessage(123);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressThread::OnFilename(WPARAM, LPARAM lp)
{
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    ::PulseEvent(pProgressInfo->m_hEvent);
//	PostQuitMessage(123);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressThread::OnStep(WPARAM, LPARAM lp)
{
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    ::PulseEvent(pProgressInfo->m_hEvent);
//	PostQuitMessage(123);
}

#if 0
		return;

	// Prepare the user interface thread
	m_pargs = new ty_args;
	m_pargs->hWnd = GetSafeHwnd();
	m_pargs->inputVal = m_pargs->returnVal = 0;
	
	m_pUiThread  =  (CUiThread*)AfxBeginThread( RUNTIME_CLASS(CUiThread),
									THREAD_PRIORITY_NORMAL);

	m_pDgAbort = new CDgAbort;
	m_pDgAbort->Create(IDD_ABORT, this);
	m_pargs->inputVal = 1234567;

	//post a user defined message to the "lenghty op" user interface thread(having no user interface at all)
	m_pUiThread->PostThreadMessage(WU_UITHREAD_START,0,(LPARAM)m_pargs);	
#endif
