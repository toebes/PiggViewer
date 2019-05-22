// TextureView.cpp : implementation file
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "TextureView.h"
#include "PiggViewerDoc.h"
#include <d3d9.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MARGIN (10)
/////////////////////////////////////////////////////////////////////////////
// CTextureView

IMPLEMENT_DYNCREATE(CTextureView, CScrollView)

CTextureView::CTextureView()
{
	m_hBitmap = NULL;
}

CTextureView::~CTextureView()
{
    //
    // TODO: MARYELLEN
    // Free up any bitmap which you have allocated
    //
	if (m_hBitmap)
		DeleteObject(m_hBitmap);
}


BEGIN_MESSAGE_MAP(CTextureView, CScrollView)
	//{{AFX_MSG_MAP(CTextureView)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureView drawing

void CTextureView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    OnUpdate(NULL, 0, NULL);
}

void CTextureView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	if (m_hBitmap)
	{
        RECT rectClient;
        CRect rectBitmap;
		SIZE totalSize;
		HDC hMemDC;               // handle of the DC we will create 
		HBITMAP hOldBitmap;

        hMemDC = CreateCompatibleDC(pDC->m_hDC);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

        CPoint ptScroll = GetScrollPosition();
		GetClientRect( &rectClient );
		GetBitmapDimensionEx(m_hBitmap, &totalSize);

        rectBitmap = CRect(CPoint(MARGIN, MARGIN), totalSize);
        rectBitmap.InflateRect(2,2,2,2);

        pDC->FillSolidRect(0, 0, ptScroll.x+rectClient.right, rectBitmap.top, GetSysColor(COLOR_WINDOW));
        pDC->FillSolidRect(0, rectBitmap.top, rectBitmap.left, rectBitmap.bottom, GetSysColor(COLOR_WINDOW));
        pDC->FillSolidRect(rectBitmap.right, rectBitmap.top, ptScroll.x+rectClient.right-rectBitmap.right, rectBitmap.bottom, GetSysColor(COLOR_WINDOW));
        pDC->FillSolidRect(0, rectBitmap.bottom, ptScroll.x+rectClient.right, ptScroll.y+rectClient.bottom-rectBitmap.bottom, GetSysColor(COLOR_WINDOW));
        pDC->DrawEdge(rectBitmap, EDGE_BUMP, BF_RECT);
        
		pDC->BitBlt( 
			   MARGIN,
			   MARGIN, 
			   totalSize.cx, 
               totalSize.cy,
			   CDC::FromHandle(hMemDC),
			   0, 
			   0, 
			   SRCCOPY);

		SelectObject(hMemDC, hOldBitmap );
		DeleteObject(hMemDC); 

	}
}

/////////////////////////////////////////////////////////////////////////////
// CTextureView diagnostics

#ifdef _DEBUG
void CTextureView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTextureView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextureView message handlers

void CTextureView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    CPiggViewerDoc* pDoc = (CPiggViewerDoc *)GetDocument();
	SIZE            sizeTotal;
	CPalette		pal;
	CPalette*		pOldPal;
	CClientDC		dc(NULL);

	if (m_hBitmap)
		DeleteObject(m_hBitmap);
	m_hBitmap = NULL;

	ID3DXBuffer *imagedata = pDoc->TXTtoDIB( &pDoc->m_abDataBlock );
	if (imagedata)
	{
		PBITMAPFILEHEADER lpfi = (PBITMAPFILEHEADER)imagedata->GetBufferPointer();
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)(((BYTE *)lpfi) + sizeof(BITMAPFILEHEADER));
		RGBQUAD *lpClrs =  (RGBQUAD *)(((BYTE *)lpbi) + lpbi->biSize);

		sizeTotal.cx = lpbi->biWidth;
		sizeTotal.cy = lpbi->biHeight;

		int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 
						1 << lpbi->biBitCount;

		LPVOID lpDIBBits;
		if( lpbi->biBitCount > 8 )
			lpDIBBits = (LPVOID)((LPDWORD)(lpClrs + lpbi->biClrUsed) + 
				((lpbi->biCompression == BI_BITFIELDS) ? 3 : 0));
		else
			lpDIBBits = (LPVOID)(((BYTE *)lpClrs) + nColors);

		// Create and select a logical palette if needed
		if( nColors <= 256 && dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
	
			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed   = lpClrs[i].rgbRed;
				pLP->palPalEntry[i].peGreen = lpClrs[i].rgbGreen;
				pLP->palPalEntry[i].peBlue  = lpClrs[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
	
			pal.CreatePalette( pLP );
	
			delete[] pLP;
	
			// Select and realize the palette
			pOldPal = dc.SelectPalette( &pal, FALSE );
			dc.RealizePalette();
		}
	
		m_hBitmap = CreateDIBitmap(dc.GetSafeHdc(),		// handle to device context
			(LPBITMAPINFOHEADER)lpbi,	// pointer to bitmap info header 
			(LONG)CBM_INIT,			// initialization flag
			lpDIBBits,			// pointer to initialization data 
			(LPBITMAPINFO)lpbi,		// pointer to bitmap info
			DIB_RGB_COLORS );		// color-data usage 

		if (pal.GetSafeHandle())
			dc.SelectPalette(pOldPal,FALSE);
		
		imagedata->Release();

		if (m_hBitmap)
		{
			SIZE size;
			SetBitmapDimensionEx(m_hBitmap, sizeTotal.cx, sizeTotal.cy, &size);

			sizeTotal.cx += (MARGIN*2);  // Account for margins on each side
			sizeTotal.cy += (MARGIN*2);  // Account for margins on each side
			SetScrollSizes(MM_TEXT, sizeTotal);

			Invalidate ();	
			return;
		}
	}

	sizeTotal.cx = MARGIN;
	sizeTotal.cy = MARGIN;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

BOOL CTextureView::OnEraseBkgnd(CDC* pDC) 
{
    return FALSE;
}
