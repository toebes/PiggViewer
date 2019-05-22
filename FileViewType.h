// FileViewType.h: interface for the CFileViewType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEVIEWTYPE_H__AD40A959_F0BE_4261_A691_1D98DD810A14__INCLUDED_)
#define AFX_FILEVIEWTYPE_H__AD40A959_F0BE_4261_A691_1D98DD810A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual void UpdateIndexList(CTreeCtrl &treeCtrl, const BYTE *pData, DWORD dwLength);
	virtual CRuntimeClass * GetViewClass();
	CFileViewType();
	virtual ~CFileViewType();

};

class CFileViewTypeBin : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual void UpdateIndexList(CTreeCtrl &treeCtrl, const BYTE *pData, DWORD dwLength);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeBin();
	virtual ~CFileViewTypeBin();

protected:
    CString DumpSection(const BYTE *pData, DWORD &nPos, DWORD dwLength, CString strIndent, BOOL bContiguous);
    void IndexSection(CTreeCtrl &treeCtrl, HTREEITEM hParent, const BYTE *pData, DWORD &nPos, DWORD dwLength);
    CMapStringToPtr m_mapSectionNames;
    CPiggViewerApp *m_pApp;
    DWORD m_dwIncrement;
    DWORD m_dwNextIncrement;
};

class CFileViewTypeTTF : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeTTF();
	virtual ~CFileViewTypeTTF();

};

class CFileViewTypeGeo : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeGeo();
	virtual ~CFileViewTypeGeo();

};

class CFileViewTypeAnim : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeAnim();
	virtual ~CFileViewTypeAnim();

};

class CFileViewTypeTxt : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeTxt();
	virtual ~CFileViewTypeTxt();

};

class CFileViewTypeOgg : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeOgg();
	virtual ~CFileViewTypeOgg();

};

class CFileViewTypeTexture : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeTexture();
	virtual ~CFileViewTypeTexture();

};

class CFileViewTypeCode : public CFileViewType  
{
public:
	virtual CString ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName);
	virtual CRuntimeClass * GetViewClass();
	CFileViewTypeCode();
	virtual ~CFileViewTypeCode();

};


#endif // !defined(AFX_FILEVIEWTYPE_H__AD40A959_F0BE_4261_A691_1D98DD810A14__INCLUDED_)
