// PiggViewerView.h : interface of the CPiggViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIGGVIEWERVIEW_H__8CC55768_8A15_4582_9806_2CC12AB28AF1__INCLUDED_)
#define AFX_PIGGVIEWERVIEW_H__8CC55768_8A15_4582_9806_2CC12AB28AF1__INCLUDED_

#include "ParsePigg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPiggViewerView : public CListView
{
protected: // create from serialization only
	CPiggViewerView();
	DECLARE_DYNCREATE(CPiggViewerView)

// Attributes
public:
	CPiggViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPiggViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // called first time after construct
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	ExternalInfo m_externalInfo;
	virtual ~CPiggViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPiggViewerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PiggViewerView.cpp
inline CPiggViewerDoc* CPiggViewerView::GetDocument()
   { return (CPiggViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIGGVIEWERVIEW_H__8CC55768_8A15_4582_9806_2CC12AB28AF1__INCLUDED_)
