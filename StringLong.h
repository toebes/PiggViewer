// StringLong.h: interface for the CStringLong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGLONG_H__F0ABE6D5_EA2F_497D_A80C_B76CC333A22E__INCLUDED_)
#define AFX_STRINGLONG_H__F0ABE6D5_EA2F_497D_A80C_B76CC333A22E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHUNK_SIZE (32768)

class CStringLong  
{
public:
	CStringLong();
	virtual ~CStringLong();
    void operator +=(CString strAppend);
    void operator =(CString strSource);
    CString &operator =(CStringLong &strlSource);
    CString GetString();

protected:
	CString m_strStorage;
	int m_nAllocatedLength;
	int m_nCurrentLength;
};

#endif // !defined(AFX_STRINGLONG_H__F0ABE6D5_EA2F_497D_A80C_B76CC333A22E__INCLUDED_)
