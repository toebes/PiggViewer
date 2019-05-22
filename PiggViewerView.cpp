// PiggViewerView.cpp : implementation of the CPiggViewerView class
//

#include "stdafx.h"
#include "PiggViewer.h"

#include "PiggViewerDoc.h"
#include "PiggViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView

IMPLEMENT_DYNCREATE(CPiggViewerView, CListView)

BEGIN_MESSAGE_MAP(CPiggViewerView, CListView)
	//{{AFX_MSG_MAP(CPiggViewerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView construction/destruction

CPiggViewerView::CPiggViewerView()
{
	// TODO: add construction code here

}

CPiggViewerView::~CPiggViewerView()
{
}

BOOL CPiggViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView drawing

void CPiggViewerView::OnDraw(CDC* pDC)
{
	CPiggViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CPiggViewerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
    CListCtrl& refCtrl = GetListCtrl();
    refCtrl.DeleteColumn(1);
    refCtrl.DeleteColumn(0);
    refCtrl.InsertColumn(0, "Item", LVCFMT_LEFT, 100);
    refCtrl.InsertColumn(1, "Value", LVCFMT_LEFT, 400);
    OnUpdate(NULL, 0, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView printing

BOOL CPiggViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPiggViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPiggViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView diagnostics

#ifdef _DEBUG
void CPiggViewerView::AssertValid() const
{
	CListView::AssertValid();
}

void CPiggViewerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPiggViewerDoc* CPiggViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPiggViewerDoc)));
	return (CPiggViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerView message handlers
void CPiggViewerView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CPiggViewerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPiggViewerDoc* pDoc = GetDocument();
    ExternalInfo *pExternalInfo = &pDoc->m_ExternalInfo;
    CListCtrl& refCtrl = GetListCtrl();
    int nItem;

    CString strFmt;
    CTime timeEntry((time_t)pExternalInfo->m_dwDateStamp);

    refCtrl.DeleteAllItems();

    nItem = refCtrl.InsertItem( 999, "Name");
    if (nItem != -1)
    {
        refCtrl.SetItemText(nItem, 1, pExternalInfo->m_strName); 
    }

    nItem = refCtrl.InsertItem( 999, "Title");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, pExternalInfo->m_strTitle); 
    }

    strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwFileSize, pExternalInfo->m_dwFileSize);
    nItem = refCtrl.InsertItem(999, "Uncompressed Size");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, strFmt); 
    }

    strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwCompressedSize, pExternalInfo->m_dwCompressedSize);
    nItem = refCtrl.InsertItem(999, "Compressed Size");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, strFmt); 
    }

    strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwFileStart, pExternalInfo->m_dwFileStart);
    nItem = refCtrl.InsertItem(999, "File Start");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, strFmt); 
    }

    nItem = refCtrl.InsertItem(999, "DateStamp");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, timeEntry.Format("%d-%b-%Y %I:%M%p")); 
    }

    strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwUnknown6, pExternalInfo->m_dwUnknown6);
    nItem = refCtrl.InsertItem(999, "Unknown6");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, strFmt); 
    }

    strFmt.Format("%s %02lx %02lx %02lx %02lx  %02lx %02lx %02lx %02lx  %02lx %02lx %02lx %02lx  %02lx %02lx %02lx %02lx",
        (pExternalInfo->m_bValid ? "Valid" : "INVALID"),
        pExternalInfo->m_ausMD5[0], pExternalInfo->m_ausMD5[1], pExternalInfo->m_ausMD5[2], pExternalInfo->m_ausMD5[3],
        pExternalInfo->m_ausMD5[4], pExternalInfo->m_ausMD5[5], pExternalInfo->m_ausMD5[6], pExternalInfo->m_ausMD5[7],
        pExternalInfo->m_ausMD5[8], pExternalInfo->m_ausMD5[9], pExternalInfo->m_ausMD5[10], pExternalInfo->m_ausMD5[11],
        pExternalInfo->m_ausMD5[12], pExternalInfo->m_ausMD5[13], pExternalInfo->m_ausMD5[14], pExternalInfo->m_ausMD5[15]);
    nItem = refCtrl.InsertItem(999, "MD5");
    if (nItem != -1)
    { 
        refCtrl.SetItemText(nItem, 1, strFmt); 
    }
    if (pExternalInfo->m_dwSecondarySize != 0)
    {
        strFmt.Format("%d", pExternalInfo->m_dwSecondarySize);
        nItem = refCtrl.InsertItem(999, "Secondary Size");
        if (nItem != -1)
        {
            refCtrl.SetItemText(nItem, 1, strFmt);
        }

        strFmt.Format("%08lx", pExternalInfo->m_pszSecondaryData);
        nItem = refCtrl.InsertItem(999, "Secondary Data");
        if (nItem != -1)
        {
            refCtrl.SetItemText(nItem, 1, strFmt);
        }
    }
    if (pExternalInfo->m_dwIndex != -1)
    {
        strFmt.Format("%d", pExternalInfo->m_dwIndex);
        nItem = refCtrl.InsertItem(999, "Index");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwWidth, pExternalInfo->m_dwWidth);
        nItem = refCtrl.InsertItem(999, "Width");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwHeight, pExternalInfo->m_dwHeight);
        nItem = refCtrl.InsertItem(999, "Height");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%c%c%c%c", pExternalInfo->m_ausExtension[0], pExternalInfo->m_ausExtension[1], pExternalInfo->m_ausExtension[2], pExternalInfo->m_ausExtension[3]);
        nItem = refCtrl.InsertItem(999, "Extension");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwUnknown8, pExternalInfo->m_dwUnknown8);
        nItem = refCtrl.InsertItem(999, "Unknown8");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwUnknown11, pExternalInfo->m_dwUnknown11);
        nItem = refCtrl.InsertItem(999, "Unknown11");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwUnknown12, pExternalInfo->m_dwUnknown12);
        nItem = refCtrl.InsertItem(999, "Unknown12");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
        
        strFmt.Format("%d (0x%08lx)", pExternalInfo->m_dwUnknown13, pExternalInfo->m_dwUnknown13);
        nItem = refCtrl.InsertItem(999, "Unknown13");
        if (nItem != -1)
        { 
            refCtrl.SetItemText(nItem, 1, strFmt); 
        }
    }
}
