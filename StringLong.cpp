// StringLong.cpp: implementation of the CStringLong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PiggViewer.h"
#include "StringLong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringLong::CStringLong()
{
    m_nCurrentLength = 0;
    m_nAllocatedLength = 0;
    m_strStorage.Empty();
}

CStringLong::~CStringLong()
{

}

void CStringLong::operator +=(CString strAppend)
{
    int nNewLength = strAppend.GetLength() + m_nCurrentLength;
    LPCTSTR pstrData;
    if (nNewLength > m_nAllocatedLength)
    {
        m_strStorage.ReleaseBuffer(m_nCurrentLength);
        m_nAllocatedLength = ((nNewLength + CHUNK_SIZE - 1) / CHUNK_SIZE) * CHUNK_SIZE;
    }
    pstrData = m_strStorage.GetBuffer(m_nAllocatedLength);
    memcpy((char *)pstrData+m_nCurrentLength, (LPCTSTR)strAppend, strAppend.GetLength());
    m_nCurrentLength += strAppend.GetLength();
}

void CStringLong::operator =(CString strSource)
{
    m_nCurrentLength = 0;
    m_strStorage.Empty();
    m_strStorage = strSource;
}

CString CStringLong::GetString()
{
    m_strStorage.ReleaseBuffer(m_nCurrentLength);
    return m_strStorage;
}