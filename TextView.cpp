// TextView.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "TextView.h"
#include "PiggViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView()
{
    m_pEditBkBrush = new CBrush(GetSysColor(COLOR_WINDOW));
}

CTextView::~CTextView()
{
}


BEGIN_MESSAGE_MAP(CTextView, CEditView)
	//{{AFX_MSG_MAP(CTextView)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView drawing

void CTextView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers

void CTextView::OnInitialUpdate() 
{
    CPiggViewerDoc* pDoc = (CPiggViewerDoc *)GetDocument();
	CEditView::OnInitialUpdate();
//    OnUpdate(NULL, 0, NULL);
}

void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    CPiggViewerDoc* pDoc = (CPiggViewerDoc *)GetDocument();
	//
    // Go through and populate it with the hex data
    //
    CEdit& theEdit = GetEditCtrl();
    theEdit.SetRedraw(FALSE);

    BOOL bIsModified = pDoc->IsModified();
    theEdit.SetReadOnly();
    theEdit.SetTabStops(16);
    theEdit.SetLimitText(4111222333);

    // adjust the left margin without changing the right margin
    DWORD dwMargins = theEdit.GetMargins();
    theEdit.SetMargins(20, HIWORD(dwMargins));

    theEdit.SetSel(0, 99999999, TRUE);
    theEdit.ReplaceSel((LPCTSTR)pDoc->GetSelectionText());
    theEdit.SetSel(0,0,FALSE);
    pDoc->SetModifiedFlag(bIsModified);
    theEdit.SetRedraw(TRUE);
}

HBRUSH CTextView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
    pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
    pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
    return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
}

void CTextView::OnDestroy() 
{
	CEditView::OnDestroy();
    delete m_pEditBkBrush;	
}

