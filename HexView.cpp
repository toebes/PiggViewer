// HexView.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "PiggViewerDoc.h"
#include "HexView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexView

IMPLEMENT_DYNCREATE(CHexView, CScrollView)

CHexView::CHexView()
{
}

CHexView::~CHexView()
{
}


BEGIN_MESSAGE_MAP(CHexView, CScrollView)
	//{{AFX_MSG_MAP(CHexView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexView drawing

void CHexView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    OnUpdate(NULL, 0, NULL);
}

void CHexView::OnDraw(CDC* pDC)
{
	CPiggViewerDoc* pDoc = GetDocument();
    CFont font;
    LOGFONT lf;
    CPoint ptUL;
    CRect rectClient;
    int nLines;
    int nFirstLine;
    int nPos;
    
    GetClientRect(&rectClient);

    memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
    lf.lfHeight = 14;                      // request a 12-pixel-height font
    lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
    VERIFY(font.CreateFontIndirect(&lf));  // create the font

    CFont* def_font = pDC->SelectObject(&font);

    //
    // Ok now we need to draw the hex lines.  Let us just make sure we get it right first
    //
    ptUL = GetScrollPosition();
    //
    // That tells us what line to work on
    //
    nFirstLine = ptUL.y / m_sizeCharacter.cy;
    nLines = (rectClient.Height() / m_sizeCharacter.cy)+2;
    nPos = nFirstLine*16;

    for(int i = 0; i < nLines; i++)
    {
        char acDisplay[18];
        CString strAscii;
        int y = (nFirstLine+i) * m_sizeCharacter.cy;
        CString strOutput;
        pDC->FillSolidRect(0, y, rectClient.Width(), m_sizeCharacter.cy, GetSysColor(COLOR_WINDOW)); 
        if (nPos < pDoc->m_abDataBlock.GetSize())
        {
            memset(acDisplay, 0, sizeof(acDisplay));
            acDisplay[0] = '|';
            acDisplay[17] = '|';
            
            strOutput.Format("%08lx:", nPos);
            
            //
            // Now go through and format each of the 16 bytes
            //
            for(int b = 0; b < 16; b++)
            {
                if (nPos < pDoc->m_abDataBlock.GetSize())
                {
                    CString strHex;
                    int c = pDoc->m_abDataBlock[nPos];
                    if (isprint(c))
                    {
                        acDisplay[b+1] = c;
                    }
                    else
                    {
                        acDisplay[b+1] = '.';
                    }
                    strHex.Format("%02x ", c & 0xff);
                    strOutput += strHex;
                    nPos++;
                }
            }
            pDC->TextOut(0, y, strOutput);
            pDC->TextOut(m_sizeCharacter.cx * 60, y, acDisplay, 18);
        }
    }
    pDC->SelectObject(def_font);
}

/////////////////////////////////////////////////////////////////////////////
// CHexView diagnostics

#ifdef _DEBUG
void CHexView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHexView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPiggViewerDoc* CHexView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPiggViewerDoc)));
	return (CPiggViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHexView message handlers
BOOL CHexView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
      SCROLLINFO info;
      info.cbSize = sizeof(SCROLLINFO);
      info.fMask = SIF_TRACKPOS;

      if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_HORZ, &info);
         nPos = info.nTrackPos;
      }

      if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
      {
         GetScrollInfo(SB_VERT, &info);
         nPos = info.nTrackPos;
      }

      return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

BOOL CHexView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return FALSE;
}

void CHexView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   	CPiggViewerDoc* pDoc = GetDocument();
    ExternalInfo *pExternalInfo = &pDoc->m_ExternalInfo;
    CClientDC dc(this); 
    CFont font;
    CSize sizeTotal;
    LOGFONT lf;

    memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
    lf.lfHeight = 14;                      // request a 12-pixel-height font
    lf.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
    VERIFY(font.CreateFontIndirect(&lf));  // create the font

    m_sizeCharacter = dc.GetTextExtent("X");
    CFont* def_font = dc.SelectObject(&font);
    // Get the metrics of a character


    // TODO: calculate the total size of this view
	sizeTotal.cx = 80 * m_sizeCharacter.cx;
    sizeTotal.cy = ((pExternalInfo->m_dwFileSize+15)/16) * m_sizeCharacter.cy;
	SetScrollSizes(MM_TEXT, sizeTotal);
    Invalidate(FALSE);
}
