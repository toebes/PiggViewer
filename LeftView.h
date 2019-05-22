// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__1B635602_8DB3_44D6_8C75_8B9D3BB44E15__INCLUDED_)
#define AFX_LEFTVIEW_H__1B635602_8DB3_44D6_8C75_8B9D3BB44E15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPiggViewerDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CPiggViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void ExtractTree(HTREEITEM hItemRoot, CString strFolderPath, BOOL bSaveBitmap);
	void ShowPopupMenu( CPoint& point );
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void DoExtract(BOOL bSaveBitmap);
    void DoExtractToDefault(BOOL bSaveBitmap);

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExtract();
	afx_msg void OnUpdateExtract(CCmdUI* pCmdUI);
	afx_msg void OnExtractToDefault();
	afx_msg void OnUpdateExtractToDefault(CCmdUI* pCmdUI);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    HTREEITEM m_pOldSel;

public:
    afx_msg void OnExtractBitmaps();
    afx_msg void OnUpdateExtractBitmaps(CCmdUI *pCmdUI);
    afx_msg void OnExtractBitmapsToDefault();
    afx_msg void OnUpdateExtractBitmapsToDefault(CCmdUI *pCmdUI);
    CString GenFileName(CString strFolderPath, CString strCurrent, CString strExtension);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CPiggViewerDoc* CLeftView::GetDocument()
   { return (CPiggViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__1B635602_8DB3_44D6_8C75_8B9D3BB44E15__INCLUDED_)
