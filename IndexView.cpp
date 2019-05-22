// IndexView.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"

#include "PiggViewerDoc.h"
#include "IndexView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexView

IMPLEMENT_DYNCREATE(CIndexView, CTreeView)

BEGIN_MESSAGE_MAP(CIndexView, CTreeView)
	//{{AFX_MSG_MAP(CIndexView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexView construction/destruction

CIndexView::CIndexView()
{
    	// TODO: add construction code here

}

CIndexView::~CIndexView()
{
}

BOOL CIndexView::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIndexView drawing

void CIndexView::OnDraw(CDC* pDC)
{
	CPiggViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CIndexView diagnostics

#ifdef _DEBUG
void CIndexView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CIndexView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CPiggViewerDoc* CIndexView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPiggViewerDoc)));
	return (CPiggViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIndexView message handlers

void CIndexView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CPiggViewerDoc* pDoc = GetDocument();
	CTreeCtrl& refCtrl = GetTreeCtrl();
    HTREEITEM hItemSelected = refCtrl.GetSelectedItem();
    pDoc->SetSelectionOffset(hItemSelected ? refCtrl.GetItemData(hItemSelected) : 0);	
	*pResult = 0;
}

void CIndexView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CIndexView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPiggViewerDoc* pDoc = GetDocument();
	CTreeCtrl& refCtrl = GetTreeCtrl();
    if (lHint != -1)
    {
        pDoc->UpdateIndexList(refCtrl);
    }
}
