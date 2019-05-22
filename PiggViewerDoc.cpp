// PiggViewerDoc.cpp : implementation of the CPiggViewerDoc class
//

#include "stdafx.h"
#include "PiggViewer.h"
#include "PiggViewerDoc.h"
#include "zlib.h"
#include "md5.h"
#include "FileViewType.h"
#include <d3d9.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerDoc

IMPLEMENT_DYNCREATE(CPiggViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CPiggViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CPiggViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPiggViewerDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CPiggViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPiggViewer to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {73063F77-0BE0-40CF-9D1E-D242183129E9}
static const IID IID_IPiggViewer =
{ 0x73063f77, 0xbe0, 0x40cf, { 0x9d, 0x1e, 0xd2, 0x42, 0x18, 0x31, 0x29, 0xe9 } };

BEGIN_INTERFACE_MAP(CPiggViewerDoc, CDocument)
	INTERFACE_PART(CPiggViewerDoc, IID_IPiggViewer, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerDoc construction/destruction

CPiggViewerDoc::CPiggViewerDoc()
{
	// TODO: add one-time construction code here
    m_parsePigg.GetExternalInfo(m_ExternalInfo, -1);
    m_dwSelectionOffset = 0;

	EnableAutomation();

	AfxOleLockApp();

}

CPiggViewerDoc::~CPiggViewerDoc()
{
	AfxOleUnlockApp();
}

BOOL CPiggViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPiggViewerDoc serialization

void CPiggViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
	    m_parsePigg.ReadPigg(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerDoc diagnostics

#ifdef _DEBUG
void CPiggViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPiggViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerDoc commands

void CPiggViewerDoc::UpdateTreeList(CTreeCtrl &treeCtrl)
{
    treeCtrl.DeleteAllItems();
    //
    // Now go through and add items
    //
    for(DWORD dwDirEnt = 0; dwDirEnt < m_parsePigg.GetDirectoryEntries(); dwDirEnt++)
    {
        CString strDirEnt = m_parsePigg.GetDirectoryName(dwDirEnt);
        AddToTree(treeCtrl, TVI_ROOT, strDirEnt, dwDirEnt);
    }
}

void CPiggViewerDoc::AddToTree(CTreeCtrl &refCtrl, HTREEITEM hParent, CString strDirEnt, int nItem)
{
    int nPos = strDirEnt.Find('/');
    if (nPos == -1)
    {
        HTREEITEM hItemNew = refCtrl.InsertItem(strDirEnt, hParent, TVI_SORT);
        if (hItemNew != NULL)
        {
            refCtrl.SetItemData(hItemNew, nItem);
        }
    }
    else
    {
        BOOL bInsert = TRUE;
        CString strPart = strDirEnt.Left(nPos);
        HTREEITEM hParentNew = NULL;
        HTREEITEM hChildItem = NULL;
        if (hParent == TVI_ROOT)
        {
            hChildItem = refCtrl.GetRootItem();
        }
        else
        {
            hChildItem = refCtrl.GetChildItem(hParent);
        }
        while (hChildItem != NULL)
        { 
            if (refCtrl.GetItemText(hChildItem) == strPart)
            {
                bInsert = FALSE;
                hParentNew = hChildItem;
                break;
            }
            hChildItem = refCtrl.GetNextItem(hChildItem, TVGN_NEXT);
        }
        if (bInsert)
        {
            hParentNew = refCtrl.InsertItem(strPart, hParent, TVI_SORT);
            if (hParentNew != NULL)
            {
                refCtrl.SetItemData(hParentNew, -1);
            }
        }
        AddToTree(refCtrl, hParentNew, strDirEnt.Mid(nPos+1), nItem);
    }
}

BOOL CPiggViewerDoc::GetDataBlock(CString strPathName, ExternalInfo &ExternalInfo, CByteArray &abDataBlock)
{
    //
    // First we need to open the file
    //
    CFileException fe;
    CFile* pFile = GetFile(strPathName,
        CFile::modeRead|CFile::shareDenyWrite, &fe);
    if (pFile != NULL)
    {
        BYTE *pDestData;
        //
        // Now we need to read in the data into the data block
        // For now we do it as uncompressed
        //
        pFile->Seek(ExternalInfo.m_dwFileStart, CFile::begin);
        if (ExternalInfo.m_dwCompressedSize == 0)
        {
            //
            // We just read the data right into the array
            //
            abDataBlock.SetSize(ExternalInfo.m_dwFileSize+3);
            
            pDestData = abDataBlock.GetData();
            pFile->Read(pDestData, ExternalInfo.m_dwFileSize);
            pDestData[ExternalInfo.m_dwFileSize] = 0;
        }
        else
        {
            //
            // We need to decompress the data.. but for now we just read the data in
            //
            CByteArray abSource;
            BYTE *pSourceData;
            DWORD dwDestLen;
            BOOL rc;
            
            abSource.SetSize(ExternalInfo.m_dwCompressedSize+3);
            abDataBlock.SetSize(ExternalInfo.m_dwFileSize+3);
            
            pSourceData = abSource.GetData();
            pDestData = abDataBlock.GetData();
            
            dwDestLen = ExternalInfo.m_dwFileSize;
            pFile->Read(pSourceData, ExternalInfo.m_dwCompressedSize);
            
            rc = UnCompress(pDestData, dwDestLen, pSourceData, ExternalInfo.m_dwCompressedSize);
            pDestData[dwDestLen] = 0;
        }
        //
        // Verify that the data is correct
        //
        {
            md5_state_t state;
            md5_byte_t digest[16];
            md5_init(&state);
            md5_append(&state, (const md5_byte_t *)pDestData, ExternalInfo.m_dwFileSize);
            md5_finish(&state, digest);
            if (!memcmp(digest, ExternalInfo.m_ausMD5, sizeof(digest)))
            {
                ExternalInfo.m_bValid = TRUE;
            }
        }
        pFile->Close();
        delete pFile;
        return TRUE;
    }
    return FALSE;
}

void CPiggViewerDoc::SetSelection(int nItem)
{
    m_ExternalInfo.m_bValid = FALSE;
    if ((nItem == -1) ||
        (((DWORD)nItem) < m_parsePigg.GetDirectoryEntries()))
    {
        m_nSelection = nItem;
        m_dwSelectionOffset = 0;
        m_abDataBlock.RemoveAll();
        CString strPathName = GetPathName();
        if (!strPathName.IsEmpty())
        {
            m_parsePigg.GetExternalInfo(m_ExternalInfo, nItem);
            GetDataBlock(strPathName, m_ExternalInfo, m_abDataBlock);
        }
        //
        // We need to notify the ChildFrm that the view may have to be reset
        //
    	POSITION pos = GetFirstViewPosition();
	    if (pos != NULL)
	    {
            //
            // Find the first view open (if any)
            //
		    CView* pView = GetNextView(pos);
		    ASSERT_VALID(pView);
            //
            // Get the parent which should be the SplitterWnd
            //
            CWnd *pParent = pView->GetParent();
            if (pParent != NULL)
            {
                //
                // Finally go to the CChildFrm which houses the SplitterWnd
                //
                pParent = pParent->GetParent();
                if (pParent != NULL)
                {
                    pParent->SendMessage(WM_USER_CHECK_VIEW, 0, 0);
                }
            }
        }
        UpdateAllViews(NULL, nItem);
    }
}

BOOL CPiggViewerDoc::SwitchToView(CRuntimeClass *pNewViewClass, CSplitterWnd *pSplitter, CView *pOldView)
{

	CRect viewrect;
	pOldView->GetWindowRect(&viewrect);

	// Create new view                      
	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = this;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = NULL;
    pSplitter->DeleteView(0,2);
	if (!pSplitter->CreateView(0, 2, pNewViewClass, viewrect.Size(), &context))
		return FALSE;

	// Set active 
	CView* pNewView = (CView *)pSplitter->GetPane(0, 2);
	pSplitter->GetParentFrame()->SetActiveView(pNewView);
   
	pSplitter->RecalcLayout();
    pNewView->OnInitialUpdate();
	pNewView->SendMessage(WM_PAINT);
	return TRUE;
}


BOOL CPiggViewerDoc::UnCompress(BYTE *pbDest, DWORD &dwDestLen, BYTE *pbSource, DWORD dwSourceLen)
{
	int err;
	z_stream d_stream; /* decompression stream */

	memset(&d_stream, 0, sizeof(d_stream));
	d_stream.next_in  = pbSource;
	d_stream.avail_in = dwSourceLen;
	d_stream.next_out = pbDest;
	d_stream.avail_out = dwDestLen;

	err = inflateInit2(&d_stream, MAX_WBITS);
	if (err != Z_OK)
    {
        return FALSE;
    }

	while (d_stream.total_in < dwSourceLen) {
		err = inflate(&d_stream, Z_SYNC_FLUSH);
		if (err == Z_STREAM_END) 
			break;
        if (err != Z_OK)
        {
            return FALSE;
        }
	}

	err = inflateEnd(&d_stream);
	return (err == Z_OK);
}

CString CPiggViewerDoc::GetSelectionText()
{
    return m_ExternalInfo.m_pFileViewType->ConvertFileContents(m_abDataBlock.GetData(), m_ExternalInfo.m_dwFileSize, m_dwSelectionOffset, m_ExternalInfo.m_strName);
}

void CPiggViewerDoc::UpdateIndexList(CTreeCtrl &treeCtrl)
{
    m_ExternalInfo.m_pFileViewType->UpdateIndexList(treeCtrl, m_abDataBlock.GetData(), m_ExternalInfo.m_dwFileSize);
}

void CPiggViewerDoc::SetSelectionOffset(DWORD dwOffset)
{
    if (dwOffset != m_dwSelectionOffset)
    {
        m_dwSelectionOffset = dwOffset;
        UpdateAllViews(NULL, -1);
    }
}


BOOL CPiggViewerDoc::GetItemData(DWORD dwItem, ExternalInfo &ExternalInfo, CByteArray &abDataBlock)
{
    ExternalInfo.m_bValid = FALSE;
    if ((dwItem >= 0) &&
        (dwItem < m_parsePigg.GetDirectoryEntries()))
    {
        abDataBlock.RemoveAll();
        if (dwItem >= 0)
        {
            CString strPathName = GetPathName();
            if (!strPathName.IsEmpty())
            {
                m_parsePigg.GetExternalInfo(ExternalInfo, dwItem);
                return GetDataBlock(strPathName, ExternalInfo, abDataBlock);
            }
        }
    }
    return FALSE;
}

ID3DXBuffer *CPiggViewerDoc::TXTtoDIB(CByteArray *data)
{
	CSize sizeTotal;

// TODO: MARYELLEN
// This is where you load the bitmap.  You can reference
//    pDoc->m_abDataBlock               - points to the actual data
//    pDoc->m_ExternalInfo.m_dwFileSize - Gives you the length of the data
// You want to set the size of the scroll region based on the width and height of the bitmap
// You can store the pointer to the allocated bitmap as a member variable on the view
// You should check to see if it has already been stored and free it up (suggest you call generic free routine which
//  you will have to also use for the destructor
//
	HRESULT hRet;
	BYTE *bytedata = data->GetData();
	int   bytesize = data->GetSize();
	
	long offset = *( (long *) (bytedata));
	bytedata += offset;
	bytesize -= offset;

    IDirect3DDevice9 *pDD = ((CPiggViewerApp *)AfxGetApp())->m_pDD;

	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 tempTexture;
	if (FAILED(hRet=D3DXCreateTextureFromFileInMemoryEx(pDD, 
		                         bytedata,
								 bytesize,
								 D3DX_DEFAULT,D3DXIFF_DDS,1,0,
								 D3DFMT_UNKNOWN,D3DPOOL_MANAGED,
								 D3DX_FILTER_NONE,D3DX_FILTER_NONE,
								 0,&info,NULL,&tempTexture))) {
		return NULL;
	}
	
	// get the texture format
	D3DSURFACE_DESC surfDesc;
	tempTexture->GetLevelDesc(0,&surfDesc);
	tempTexture->Release();


	// create a surface to hold the entire file
	LPDIRECT3DSURFACE9 destSurf;
	if (FAILED(pDD->CreateOffscreenPlainSurface(info.Width,info.Height,
						surfDesc.Format, D3DPOOL_SCRATCH, &destSurf, NULL))) {
		return NULL;
	}

	// load the image into the surface
	if (FAILED( D3DXLoadSurfaceFromFileInMemory(destSurf,NULL,NULL,
		bytedata,bytesize,
		NULL,D3DX_FILTER_NONE,0,NULL))) {
		return NULL;
	}

	//save its contents to a bitmap file.
	ID3DXBuffer *imagedata;
	if (FAILED(D3DXSaveSurfaceToFileInMemory(&imagedata,
                              D3DXIFF_DIB,
                              destSurf,
                              NULL,
                              NULL))) {
		return NULL;
	}
	
	destSurf->Release();

	return imagedata;
}


/////////////////////////////////////////////////////////////////////////////
