#if !defined(AFX_PROGRESSTHREAD_H__A35C217C_628B_412D_A168_AA521013ACD6__INCLUDED_)
#define AFX_PROGRESSTHREAD_H__A35C217C_628B_412D_A168_AA521013ACD6__INCLUDED_

#include "ProgressDialog.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressThread.h : header file
//

#if 0
const int WU_PROGRESS_START      = WM_APP + 1;   // Parm LPARAM = Pointer to ProgressInfo structure
const int WU_PROGRESS_STOP       = WM_APP + 2;   // Stop the dialog, no parameters
const int WU_PROGRESS_FILANEME   = WM_APP + 3;   // Parm LPARAM = Pointer to ProgressInfo structure
const int WU_PROGRESS_STEP       = WM_APP + 4;   // Parm LPARAM = Pointer to ProgressInfo structure

typedef struct {
    DWORD dwLimit;
    DWORD dwCurrent;
    char szFilename[257];
    char szTitle[128];
} ProgressInfo;

#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressThread thread

class CProgressThread : public CWinThread
{
	DECLARE_DYNCREATE(CProgressThread)
protected:
	CProgressThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CProgressDialog *m_pProgressDialog;
	virtual ~CProgressThread();

	// Generated message map functions
	//{{AFX_MSG(CProgressThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnStart(WPARAM, LPARAM);
	afx_msg void OnStop(WPARAM, LPARAM);
	afx_msg void OnFilename(WPARAM, LPARAM);
	afx_msg void OnStep(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSTHREAD_H__A35C217C_628B_412D_A168_AA521013ACD6__INCLUDED_)
