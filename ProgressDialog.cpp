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
// CProgressDialog dialog


CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
    ON_THREAD_MESSAGE(WU_PROGRESS_START,   OnStart)
	ON_THREAD_MESSAGE(WU_PROGRESS_STOP, OnStop)
    ON_THREAD_MESSAGE(WU_PROGRESS_FILANEME,   OnFilename)
	ON_THREAD_MESSAGE(WU_PROGRESS_STEP, OnStep)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog message handlers
void CProgressDialog::OnStart(WPARAM, LPARAM lp)
{
    CWnd *pWndProgressControl;
    CWnd *pWndProgressText;
    
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    SetWindowText(pProgressInfo->szTitle);
    pWndProgressControl = GetDlgItem(IDC_PROGRESS);
    pWndProgressText = GetDlgItem(IDC_PROGRESS_TEXT);
    pWndProgressControl->ShowWindow(pProgressInfo->dwLimit != 0);
    pWndProgressText->ShowWindow(pProgressInfo->dwLimit == 0);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressDialog::OnStop(WPARAM, LPARAM)
{
    EndDialog(IDOK);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressDialog::OnFilename(WPARAM, LPARAM lp)
{
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    SetDlgItemText(IDC_PROGRESS_TEXT, pProgressInfo->szFilename);
//	PostQuitMessage(123);
}
/////////////////////////////////////////////////////////////////////////////
void CProgressDialog::OnStep(WPARAM, LPARAM lp)
{
    ProgressInfo *pProgressInfo = (ProgressInfo *)lp;
    CProgressCtrl *pwndProgressControl = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS);
    if (pwndProgressControl != NULL)
    {
        pwndProgressControl->SetPos(pProgressInfo->dwCurrent);
    }
}
