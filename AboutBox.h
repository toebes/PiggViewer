#if !defined(AFX_ABOUTBOX_H__8C08DC00_CA58_11D3_AAB7_000039399717__INCLUDED_)
#define AFX_ABOUTBOX_H__8C08DC00_CA58_11D3_AAB7_000039399717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog

class CAboutBox : public CDialog
{
// Construction
public:
	CAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutBox)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutBox)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTBOX_H__8C08DC00_CA58_11D3_AAB7_000039399717__INCLUDED_)
