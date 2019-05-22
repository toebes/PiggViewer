// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "PiggViewer.h"

#include <direct.h>
#include "PiggViewerDoc.h"
#include "LeftView.h"
#include "OptionsDialog.h"
#include ".\leftview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EXTRACT, OnExtract)
	ON_UPDATE_COMMAND_UI(ID_EXTRACT, OnUpdateExtract)
	ON_COMMAND(ID_EXTRACT_TO_DEFAULT, OnExtractToDefault)
	ON_UPDATE_COMMAND_UI(ID_EXTRACT_TO_DEFAULT, OnUpdateExtractToDefault)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
    ON_COMMAND(ID_EXTRACT_BITMAPS, OnExtractBitmaps)
    ON_UPDATE_COMMAND_UI(ID_EXTRACT_BITMAPS, OnUpdateExtractBitmaps)
    ON_COMMAND(ID_EXTRACT_BITMAPS_TO_DEFAULT, OnExtractBitmapsToDefault)
    ON_UPDATE_COMMAND_UI(ID_EXTRACT_BITMAPS_TO_DEFAULT, OnUpdateExtractBitmapsToDefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CPiggViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	CTreeCtrl& refCtrl = GetTreeCtrl();
	CPiggViewerDoc* pDoc = GetDocument();
    pDoc->UpdateTreeList(refCtrl);
	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CPiggViewerDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPiggViewerDoc)));
	return (CPiggViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CPiggViewerDoc* pDoc = GetDocument();
	CTreeCtrl& refCtrl = GetTreeCtrl();
    HTREEITEM hItemSelected = refCtrl.GetSelectedItem();
    pDoc->SetSelection(hItemSelected ? refCtrl.GetItemData(hItemSelected) : -1);	
	*pResult = 0;
}

void CLeftView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
       UINT uFlags;
        CTreeCtrl&      treeCtrl = GetTreeCtrl();
        CPoint ptTree = point;
        treeCtrl.ScreenToClient(&ptTree);
        HTREEITEM htItem = treeCtrl.HitTest(ptTree, &uFlags);

        if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) {
                ShowPopupMenu( point );
                treeCtrl.SetItemState(htItem, 0, TVIS_DROPHILITED);
        }
        else
                CTreeView::OnContextMenu(pWnd, point);

        if (m_pOldSel != NULL) {
                treeCtrl.Select(m_pOldSel, TVGN_DROPHILITE);
                m_pOldSel = NULL;
        }
}

void CLeftView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    UINT uFlags;
    HTREEITEM htItem = GetTreeCtrl().HitTest(point, &uFlags);
    if ((htItem != NULL) && (uFlags & TVHT_ONITEM))
    {
        GetTreeCtrl().Select(htItem, TVGN_DROPHILITE);
    }
    CTreeView::OnLButtonDown(nFlags, point);
}

void CLeftView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    UINT uFlags;
    HTREEITEM htItem = GetTreeCtrl().HitTest(point, &uFlags);
    if ((htItem != NULL) && (uFlags & TVHT_ONITEM))
    {
        m_pOldSel = GetTreeCtrl().GetSelectedItem();
        GetTreeCtrl().Select(htItem, TVGN_DROPHILITE);
    }
}

void CLeftView::ShowPopupMenu(CPoint &point)
{
    if (point.x == -1 && point.y == -1)
    {
        //keystroke invocation
        CRect rect;
        GetClientRect(rect);
        ClientToScreen(rect);
        
        point = rect.TopLeft();
        point.Offset(5, 5);
    }
    
    CMenu menu;
    VERIFY(menu.LoadMenu(IDR_POPUP_LEFT_MENU));
    
    CMenu* pPopup = menu.GetSubMenu(0);
    ASSERT(pPopup != NULL);
    CWnd* pWndPopupOwner = this;
    
    while (pWndPopupOwner->GetStyle() & WS_CHILD)
        pWndPopupOwner = pWndPopupOwner->GetParent();
    
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
        pWndPopupOwner);

}


CString CLeftView::GenFileName(CString strFolderPath, CString strCurrent, CString strExtension)
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char path_buffer[_MAX_PATH];
    _makepath( path_buffer, NULL, strFolderPath, strCurrent, NULL );
    if (!strExtension.IsEmpty())
    {
        _splitpath( path_buffer, drive, dir, fname, ext );
        _makepath( path_buffer, drive, dir, fname, strExtension);
    }
    return path_buffer;
}

void CLeftView::ExtractTree(HTREEITEM hItemRoot, CString strFolderPath, BOOL bSaveBitmap)
{
    CTreeCtrl& refCtrl = GetTreeCtrl();
    CString strCurrent;
    CString strOutpath;
    strCurrent = refCtrl.GetItemText(hItemRoot);
    strOutpath = GenFileName(strFolderPath, strCurrent, "");

    if (refCtrl.ItemHasChildren(hItemRoot))
    {
        _mkdir(strOutpath);
        HTREEITEM hItem = refCtrl.GetChildItem(hItemRoot);
        
        while (hItem != NULL)
        {
            ExtractTree(hItem, strOutpath, bSaveBitmap);
            hItem = refCtrl.GetNextSiblingItem(hItem);
        }
    }
    else
    {
      	CPiggViewerDoc* pDoc = GetDocument();

        ExternalInfo ExternalInfo;
        CByteArray abDataBlock;
        if (pDoc->GetItemData(refCtrl.GetItemData(hItemRoot), ExternalInfo, abDataBlock))
        {
            if (bSaveBitmap)
            {
                strOutpath = GenFileName(strFolderPath, strCurrent, "bmp");
            }
            CFile FileOut(strOutpath, CFile::modeCreate |CFile::modeWrite|CFile::typeBinary );
            if (bSaveBitmap)
            {
                ID3DXBuffer *imagedata;
				imagedata = pDoc->TXTtoDIB(&abDataBlock);
                FileOut.Write(imagedata->GetBufferPointer(), imagedata->GetBufferSize());
				imagedata->Release();
            }
            else
            {
                FileOut.Write(abDataBlock.GetData(), ExternalInfo.m_dwFileSize);
            }

			if (NULL != ExternalInfo.m_pszSecondaryData)
			{
				char external_buffer[_MAX_PATH];
				int extensionIndex = strCurrent.Find('.');
				strCurrent = strCurrent.Left(extensionIndex) + CString("-ext") + strCurrent.Right(strCurrent.GetLength() - extensionIndex);
				_makepath(external_buffer, NULL, strFolderPath, strCurrent, NULL);
				CFile ExtFileOut(external_buffer, CFile::modeCreate |CFile::modeWrite|CFile::typeBinary );
				ExtFileOut.Write(ExternalInfo.m_pszSecondaryData, ExternalInfo.m_dwSecondarySize);
			}
        }
    }
}

void CLeftView::DoExtract(BOOL bSaveBitmap)
{
   	CTreeCtrl& refCtrl = GetTreeCtrl();
    CPiggViewerApp *pApp = (CPiggViewerApp *)AfxGetApp();

    HTREEITEM hItemSelected = refCtrl.GetSelectedItem();
    CString strFolderPath = pApp->GetLastDirectory();;

    if (GetFolder(&strFolderPath, "Select directory to extract to.", this->m_hWnd, NULL, strFolderPath))
    {
        if (!strFolderPath.IsEmpty())
        {
            pApp->SetLastDirectory(strFolderPath);
        }
    }
    ExtractTree(hItemSelected, strFolderPath, bSaveBitmap);
}

void CLeftView::OnExtractBitmaps()  { DoExtract(TRUE);  }
void CLeftView::OnExtract()         { DoExtract(FALSE); }

void CLeftView::OnUpdateExtract(CCmdUI* pCmdUI)  { }
void CLeftView::OnUpdateExtractBitmaps(CCmdUI *pCmdUI) { }

void CLeftView::DoExtractToDefault(BOOL bSaveBitmap)
{
	CTreeCtrl& refCtrl = GetTreeCtrl();
    CPiggViewerApp *pApp = (CPiggViewerApp *)AfxGetApp();

    HTREEITEM hItemSelected = refCtrl.GetSelectedItem();
    CString strFolderPath = pApp->GetDefaultDirectory();

    ExtractTree(hItemSelected, strFolderPath, bSaveBitmap);
}

void CLeftView::OnExtractToDefault()        { DoExtractToDefault(FALSE); }
void CLeftView::OnExtractBitmapsToDefault() { DoExtractToDefault(TRUE);  }

void CLeftView::OnUpdateExtractToDefault(CCmdUI* pCmdUI) 
{
    CPiggViewerApp *pApp = (CPiggViewerApp *)AfxGetApp();
    pCmdUI->Enable(!pApp->GetDefaultDirectory().IsEmpty());
}

void CLeftView::OnUpdateExtractBitmapsToDefault(CCmdUI *pCmdUI)
{
    CPiggViewerApp *pApp = (CPiggViewerApp *)AfxGetApp();
    pCmdUI->Enable(!pApp->GetDefaultDirectory().IsEmpty());
}

void CLeftView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
    TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
    if ( pTVKeyDown->wVKey == VK_APPS )
    {
        HTREEITEM hItem = GetTreeCtrl().GetSelectedItem( );
        CRect rect;
        GetTreeCtrl().GetItemRect( hItem, &rect, TRUE);
        ClientToScreen( rect );
        OnContextMenu( this, CPoint( rect.right, rect.top ) );
    }
    
    *pResult = 0;
}
