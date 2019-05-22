// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog

IMPLEMENT_DYNCREATE(CProgressDialog, CWinThread)

CProgressDialog::CProgressDialog()
{
}

CProgressDialog::~CProgressDialog()
{
}

BOOL CProgressDialog::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CProgressDialog::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProgressDialog, CWinThread)
	//{{AFX_MSG_MAP(CProgressDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WU_PROGRESS_START,   OnStart)
	ON_THREAD_MESSAGE(WU_PROGRESS_TERMINATED, OnTerminate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers
#if 0

/////////////////////////////////////////////////////////////////////////////
// CUiThread message handlers

//perform the lengthy operation
void CUiThread::OnStart(WPARAM, LPARAM lp)
{
	ty_args* pargs = (ty_args*)lp;
	int i = pargs->inputVal;
	int k = lengthyOperation (i);
	//signal the initiating window the termination of the lenop, together with some params
	//with a user defined message
	CWnd::FromHandle(pargs->hWnd)->PostMessage(WU_PROGRESS_TERMINATED, i, k);
}
/////////////////////////////////////////////////////////////////////////////
void CUiThread::OnTerminate(WPARAM, LPARAM)
{
	//terminate this ui thread with a return code indicating whether terminated normally,
	// or by user abort. The return code is evaluated by the mainframe.
	//you may check that exit code in DevStudio's Output window
	if (g_abort)
		PostQuitMessage(123);
	else
		PostQuitMessage(456);
}

#endif