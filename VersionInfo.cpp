// VersionInfo.cpp: implementation of the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VersionInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVersionInfo::CVersionInfo(LPCTSTR lpModuleName)
{
    char szModuleName[_MAX_PATH];
    DWORD   dwVerHnd=0;            // An 'ignored' parameter for GetFileVersionInfo, always '0'
    //
    // Initialize our defaults so that if anything fails during the initialization, 
    // all attempts to get version info strings will return nothing instead of
    // crashing
    //
    m_dwVerInfoSize = 0;
    m_lpstrVffInfo  = NULL;
    if (lpModuleName == NULL)
    {
        //
        // For convenience, if the pass null as the module name, it means that we want
        // to get the version information for the curent executable
        //
        if (!GetModuleFileName(NULL, szModuleName, sizeof(szModuleName)))
            return;
        lpModuleName = szModuleName;
    }
    //
    // Read in the version info structure from the module
    //
    m_dwVerInfoSize = GetFileVersionInfoSize((char *)lpModuleName, &dwVerHnd);
    m_lpstrVffInfo  = new char[m_dwVerInfoSize];
    if (!GetFileVersionInfo((char *)lpModuleName, dwVerHnd, m_dwVerInfoSize, m_lpstrVffInfo))
    {
        m_dwVerInfoSize = 0;
        delete m_lpstrVffInfo;
        return;
    }
    //
    // Figure out what the default language translation is
    //
    DWORD* lpBuffer;
    UINT  uiVerSize;
    m_strPrefix = "\\StringFileInfo\\040904E4\\";
    if (VerQueryValue( m_lpstrVffInfo, "\\VarFileInfo\\Translation", (void **)&lpBuffer, &uiVerSize))
    {
        if (uiVerSize)
            m_strPrefix.Format("\\StringFileInfo\\%04hX%04hX\\", LOWORD(*lpBuffer), HIWORD(*lpBuffer));
    }
}

CVersionInfo::~CVersionInfo()
{
    delete m_lpstrVffInfo;
}

CString CVersionInfo::GetVersionString(LPCTSTR lpszStringFileInfo)
{
    LPSTR   lpVersion;            // String pointer to 'version' text
    UINT    uVersionLen;
    //
    // Handle any situation where we couldn't get the version information originally
    //
    if (m_lpstrVffInfo == NULL)
        return ("");

    if (VerQueryValue(m_lpstrVffInfo, (char *)(LPCTSTR)(m_strPrefix + lpszStringFileInfo), (void**)&lpVersion,
                      &uVersionLen))
    {
        CString strResult(lpVersion, uVersionLen);
        return strResult;
    }
    return ("");
}

void CVersionInfo::UpdateDlgVersionStrings(CWnd *pDialog, int nIDCFirst, int nIDCLast)
{
    if (nIDCLast < nIDCFirst)
        nIDCLast = nIDCFirst;
    for(int nID = nIDCFirst; nID <= nIDCLast; nID++)
    {
        CString strText;
        if (pDialog->GetDlgItemText(nID, strText))
        {
            pDialog->SetDlgItemText(nID, GetVersionString(strText));
        }
    }
}
