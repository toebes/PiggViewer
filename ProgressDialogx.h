#if !defined(AFX_PROGRESSDIALOG_H__99872B24_E23F_4085_BE48_D30B33987CAA__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__99872B24_E23F_4085_BE48_D30B33987CAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CProgressDialog thread

class CProgressDialog : public CWinThread
{
	DECLARE_DYNCREATE(CProgressDialog)
protected:
	CProgressDialog();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProgressDialog();

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnStart(WPARAM, LPARAM);
	afx_msg void OnTerminate(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__99872B24_E23F_4085_BE48_D30B33987CAA__INCLUDED_)
