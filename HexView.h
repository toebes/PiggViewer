#if !defined(AFX_HEXVIEW_H__38B86956_A991_4A6B_8D1A_2EFD7C9BCE7A__INCLUDED_)
#define AFX_HEXVIEW_H__38B86956_A991_4A6B_8D1A_2EFD7C9BCE7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexView.h : header file
//

#include "PiggViewerDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHexView view

class CHexView : public CScrollView
{
protected:
	CHexView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHexView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexView)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSize m_sizeCharacter;
	CPiggViewerDoc* GetDocument();
	virtual ~CHexView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHexView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PiggViewerView.cpp
inline CPiggViewerDoc* CHexView::GetDocument()
   { return (CPiggViewerDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXVIEW_H__38B86956_A991_4A6B_8D1A_2EFD7C9BCE7A__INCLUDED_)
