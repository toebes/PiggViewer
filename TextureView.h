#if !defined(AFX_TEXTUREVIEW_H__86605C44_BDB9_4EC1_8D8E_D0DBC82443CE__INCLUDED_)
#define AFX_TEXTUREVIEW_H__86605C44_BDB9_4EC1_8D8E_D0DBC82443CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureView.h : header file
//
#include <d3dx9.h>

/////////////////////////////////////////////////////////////////////////////
// CTextureView view

class CTextureView : public CScrollView
{
protected:
	CTextureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextureView)

// Attributes
public:

// Operations
public:
//	CDDrawSystem m_DDrawSystem;
	BOOL m_bDDrawActive;
	HBITMAP m_hBitmap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTextureView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTUREVIEW_H__86605C44_BDB9_4EC1_8D8E_D0DBC82443CE__INCLUDED_)
