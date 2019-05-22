#if !defined(AFX_INDEXVIEW_H__97D7FEF1_DAE5_40E4_A4DE_C50CF00CAB66__INCLUDED_)
#define AFX_INDEXVIEW_H__97D7FEF1_DAE5_40E4_A4DE_C50CF00CAB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexView view

class CPiggViewerDoc;

class CIndexView : public CTreeView
{
protected:
	CIndexView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIndexView)

// Attributes
public:
	CPiggViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIndexView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndexView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CPiggViewerDoc* CIndexView::GetDocument()
   { return (CPiggViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXVIEW_H__97D7FEF1_DAE5_40E4_A4DE_C50CF00CAB66__INCLUDED_)
