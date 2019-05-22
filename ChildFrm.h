// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__CA848DBB_E612_42D6_9DC7_D31EBC2CF6D4__INCLUDED_)
#define AFX_CHILDFRM_H__CA848DBB_E612_42D6_9DC7_D31EBC2CF6D4__INCLUDED_

#include "ParsePigg.h"	// Added by ClassView
#include "PiggViewerDoc.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPiggViewerView;

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPiggViewerView* GetRightPane();
// Generated message map functions
protected:
	CPiggViewerDoc* m_pDocument;
	CPiggViewerDoc* GetDocument();
	CFileViewType *m_pFileViewType;
	BOOL SwitchToView(CRuntimeClass* pNewViewClass);
	DWORD m_dwViewMode;
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnViewText();
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewDetails();
	afx_msg void OnViewHex();
	afx_msg void OnViewItem();
    afx_msg LRESULT OnUserCheckView(WPARAM, LPARAM);
	//}}AFX_MSG
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__CA848DBB_E612_42D6_9DC7_D31EBC2CF6D4__INCLUDED_)
