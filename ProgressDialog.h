#if !defined(AFX_PROGRESSDIALOG_H__E5F1F172_A888_46CB_A2E1_9B2BA0CA3EA3__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__E5F1F172_A888_46CB_A2E1_9B2BA0CA3EA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

const int WU_PROGRESS_START      = WM_APP + 1;   // Parm LPARAM = Pointer to ProgressInfo structure
const int WU_PROGRESS_STOP       = WM_APP + 2;   // Stop the dialog, no parameters
const int WU_PROGRESS_FILANEME   = WM_APP + 3;   // Parm LPARAM = Pointer to ProgressInfo structure
const int WU_PROGRESS_STEP       = WM_APP + 4;   // Parm LPARAM = Pointer to ProgressInfo structure

typedef struct {
    HANDLE m_hEvent;
    HWND m_hwnd;
    DWORD dwLimit;
    DWORD dwCurrent;
    char szFilename[257];
    char szTitle[128];
} ProgressInfo;

class CProgressDialog : public CDialog
{
// Construction
public:
	CProgressDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_PROGRESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg void OnStart(WPARAM, LPARAM);
	afx_msg void OnStop(WPARAM, LPARAM);
	afx_msg void OnFilename(WPARAM, LPARAM);
	afx_msg void OnStep(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__E5F1F172_A888_46CB_A2E1_9B2BA0CA3EA3__INCLUDED_)
