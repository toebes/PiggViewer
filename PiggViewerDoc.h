// PiggViewerDoc.h : interface of the CPiggViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIGGVIEWERDOC_H__8CFD0A8D_4B17_45F8_8508_4CF5CB9E50C6__INCLUDED_)
#define AFX_PIGGVIEWERDOC_H__8CFD0A8D_4B17_45F8_8508_4CF5CB9E50C6__INCLUDED_

#include "ParsePigg.h"	// Added by ClassView
#include "FileViewType.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_CHECK_VIEW (WM_USER+1)
class CPiggViewerDoc : public CDocument
{
protected: // create from serialization only
	CPiggViewerDoc();
	DECLARE_DYNCREATE(CPiggViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPiggViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetItemData(DWORD dwItem, ExternalInfo &ExternalInfo, CByteArray &abDataBlock);
	BOOL GetDataBlock(CString strPathName, ExternalInfo &ExternalInfo, CByteArray  &abDataBlock);
	void SetSelectionOffset(DWORD dwOffset);
	void UpdateIndexList(CTreeCtrl &treeCtrl);
	CFileViewType m_fileViewTypeDefault;
	CString GetSelectionText();
	BOOL UnCompress(BYTE *pbDest, DWORD &dwDestLen, BYTE *pbSource, DWORD dwSourceLen);
	BOOL SwitchToView(CRuntimeClass *pNewViewClass, CSplitterWnd *pSplitter, CView *pOldView);
	ExternalInfo m_ExternalInfo;
	CByteArray m_abDataBlock;
    int GetSelection(void) { return m_nSelection; }
    void SetSelection(int nItem);
	void UpdateTreeList(CTreeCtrl &treeCtrl);
	ID3DXBuffer *TXTtoDIB(CByteArray *data);
	virtual ~CPiggViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DWORD m_dwSelectionOffset;
	int m_nSelection;
    void AddToTree(CTreeCtrl &refCtrl, HTREEITEM hParent, CString strDirEnt, int nItem);
	CParsePigg m_parsePigg;
	//{{AFX_MSG(CPiggViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPiggViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIGGVIEWERDOC_H__8CFD0A8D_4B17_45F8_8508_4CF5CB9E50C6__INCLUDED_)
