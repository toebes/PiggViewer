// VersionInfo.h: interface for the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSIONINFO_H__340C6763_C8AA_11D3_B3E9_00608CCF3DA8__INCLUDED_)
#define AFX_VERSIONINFO_H__340C6763_C8AA_11D3_B3E9_00608CCF3DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVersionInfo  
{
public:
	void UpdateDlgVersionStrings(CWnd *pDialog, int nIDCFirst, int nIDCLast = -1);
	CVersionInfo(LPCTSTR lpModuleName = NULL);
	virtual ~CVersionInfo();
    CString GetVersionString(LPCTSTR lpszStringFileInfo);
    // FUNCTION
    //    GetComments()
    // PURPOSE
    //    Specifies additional information that should be displayed for diagnostic purposes. 
    CString GetComments() { return GetVersionString("Comments"); }
    // FUNCTION
    //    GetCompanyName()
    // PURPOSE
    //    Returns the company that produced the file
    //    for example, "Cisco Systems" or "Standard Microsystems Corporation, Inc."
    CString GetCompanyName() { return GetVersionString("CompanyName"); }
    // FUNCTION
    //    GetFileDescription()
    // PURPOSE
    //    Returns a file description to be presented to users.
    //    This string may be displayed in a list box when the user is choosing files to install
    //    for example, "Keyboard Driver for AT-Style Keyboards".
    //    This string is required. 
    CString GetFileDescription() { return GetVersionString("FileDescription"); }
    // FUNCTION
    //    GetFileVersion()
    // PURPOSE
    //    Returns the version number of the file
    //    for example, "3.10" or "5.00.RC2".
    //    This string is required. 
    CString GetFileVersion() { return GetVersionString("FileVersion"); }
    // FUNCTION
    //    GetInternalName()
    // PURPOSE
    //    Returns the internal name of the file, if one exists
    //    for example, a module name if the file is a dynamic-link library.
    //    If the file has no internal name, this string should be the original
    //    filename, without extension.
    //    This string is required. 
    CString GetInternalName() { return GetVersionString("InternalName"); }
    // FUNCTION
    //    GetLegalCopyright()
    // PURPOSE
    //    Returns all copyright notices that apply to the file.
    //    This should include the full text of all notices, legal symbols,
    //    copyright dates, and so on
    //    for example, "Copyright© Cisco Systems 1990 1998".
    //    This string is optional. 
    CString GetLegalCopyright() { return GetVersionString("LegalCopyright"); }
    // FUNCTION
    //    GetLegalTrademarks()
    // PURPOSE
    //    Returns all trademarks and registered trademarks that apply to the file.
    //    This should include the full text of all notices, legal symbols, trademark numbers, and so on.
    //    This string is optional. 
    CString GetLegalTrademarks() { return GetVersionString("LegalTrademarks"); }
    // FUNCTION
    //    GetOriginalFilename()
    // PURPOSE
    //    Returns the original name of the file, not including a path.
    //    This information enables an application to determine whether
    //    a file has been renamed by a user. The format of the name
    //    depends on the file system for which the file was created.
    //    This string is required. 
    CString GetOriginalFilename() { return GetVersionString("OriginalFilename"); }
    // FUNCTION
    //    GetPrivateBuild()
    // PURPOSE
    //    Returns information about a private version of the file
    //    for example, "Built by TESTER1 on \TESTBED".
    //    This string should be present only if VS_FF_PRIVATEBUILD is specified in the fileflags parameter of the root block. 
    CString GetPrivateBuild() { return GetVersionString("PrivateBuild"); }
    // FUNCTION
    //    GetProductName()
    // PURPOSE
    //    Returns the name of the product with which the file is distributed.
    //    This string is required. 
    CString GetProductName() { return GetVersionString("ProductName"); }
    // FUNCTION
    //    GetProductVersion()
    // PURPOSE
    //    Returns the version of the product with which the file is distributed
    //    for example, "3.10" or "5.00.RC2". This string is required.
    CString GetProductVersion() { return GetVersionString("ProductVersion"); }
    // FUNCTION
    //    GetSpecialBuild()
    // PURPOSE
    //    Returns how this version of the file differs from the standard version
    //    for example, "Private build for TESTER1 solving mouse problems on M250 and M250E computers".
    //    This string should be present only if VS_FF_SPECIALBUILD
    //    is specified in the fileflags parameter of the root block. 
    CString GetSpecialBuild() { return GetVersionString("SpecialBuild"); }
protected:
    DWORD m_dwVerInfoSize;
    char *m_lpstrVffInfo;
    CString m_szFileName;
    CString m_strPrefix;
};

#endif // !defined(AFX_VERSIONINFO_H__340C6763_C8AA_11D3_B3E9_00608CCF3DA8__INCLUDED_)
