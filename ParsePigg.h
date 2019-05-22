// ParsePigg.h: interface for the CParsePigg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEPIGG_H__CEE26132_D88D_4A6A_8131_C747C54124EB__INCLUDED_)
#define AFX_PARSEPIGG_H__CEE26132_D88D_4A6A_8131_C747C54124EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "FileViewType.h"
class CPiggViewerDoc;

const USHORT UNKNOWN2_VALUE = 0x0002;
const USHORT UNKNOWN3_VALUE = 0x0002;
const USHORT UNKNOWN4_VALUE = 0x0010;
const USHORT UNKNOWN5_VALUE = 0x0030;
const DWORD  UNKNOWN6_VALUE = 0x00000000;
const DWORD  MARKER_PIGG_START      = 0x00000123;
const DWORD  MARKER_DIRECTORY_ENTRY = 0x00003456;
const DWORD  MARKER_STRING_TABLE    = 0x00006789;
const DWORD  MARKER_SLOT_TABLE      = 0x00009ABC;
typedef struct {
    DWORD  m_dwMarker;          // 0x00000123
    USHORT m_usUnknown2;        // 0x0002
    USHORT m_usUnknown3;        // 0x0002
    USHORT m_usUnknown4;        // 0x0010
    USHORT m_usUnknown5;        // 0x0030
    DWORD  m_dwDirEntCount;     // Number of Directory entries at the start of the file
} PiggHeader;

typedef struct {
    DWORD m_dwMarker;           // 0x00003456
    DWORD m_dwStringNum;        // Which string in the string table this is associated with
    DWORD m_dwFileSize;         // Uncompressed File Size
    DWORD m_dwDateStamp;        // Date stamp when this entry was created
    DWORD m_dwFileStart;        // Location in the file where this chunk starts
    DWORD m_dwUnknown6;         // Currently always zero
    DWORD m_dwIndex;            // Location of secondary slot this is associated with
    unsigned char m_ausMD5[16]; // MD5 Hash of the uncompressed data
    DWORD m_dwCompressedSize;   // Compressed length of the file
} PiggDirectoryEntry;

typedef struct {
    DWORD m_dwMarker;           // 0x00006789
    DWORD m_dwStringCount;      // Number of strings in the table
    DWORD m_dwLength;           // Number of bytes occupied by the string table
} PiggStringTable;

typedef struct {
    DWORD m_dwMarker;           // 0x00009ABC
    DWORD m_dwEntryCount;       // Number of entries in the table
    DWORD m_dwLength;           // Length of the table in bytes
} PiggSlotTable;

typedef struct {
    DWORD m_dwEntryLength;      // Length of following table entry
} PiggSlotLength;

typedef struct {
    DWORD m_dwLength;           // Length of the entry  0x00000070
    DWORD m_dwUnknown8;         // 0x00400080  Some sort of flags
    DWORD m_dwWidth;            // Width of the rendered bitmap
    DWORD m_dwHeight;           // Height of the rendered bitmap
    DWORD m_dwUnknown11;        // c2 00 01 00
    DWORD m_dwUnknown12;        // 00 00 00 00
    DWORD m_dwUnknown13;        // 00 00 00 00
    unsigned char m_ausExtension[4];    // .TX2
    unsigned char m_ausTitle[1];        // Text string (NULL Terminated)
} PiggSlotEntry;

typedef struct {
    DWORD m_dwLength;           // Length of the entry
    DWORD m_dwUncompressedSize; // Size of the uncompressed data
    unsigned char m_aucData[1]; // Data portion
} PiggCompressedSlotEntry;

typedef struct {
    DWORD m_dwLength;           // Length of the entry
    unsigned char m_aucData[1]; // Data portion
} PiggUnCompressedSlotEntry;

typedef struct {
    CString m_strName;
    CString m_strTitle;
    DWORD m_dwFileSize;         // Uncompressed File Size
    DWORD m_dwDateStamp;        // Date stamp when this entry was created
    DWORD m_dwFileStart;        // Location in the file where this chunk starts
    DWORD m_dwUnknown6;         // Currently always zero
    DWORD m_dwIndex;            // Location of secondary slot this is associated with
    unsigned char m_ausMD5[16]; // MD5 Hash of uncompressed data
    DWORD m_dwCompressedSize;   // Compressed length of the file
    DWORD m_dwSlotLength;       // Length of the entry  0x00000070
    DWORD m_dwUnknown8;         // 0x00400080  Some sort of flags
    DWORD m_dwWidth;            // Width of the rendered bitmap
    DWORD m_dwHeight;           // Height of the rendered bitmap
    DWORD m_dwUnknown11;        // c2 00 01 00
    DWORD m_dwUnknown12;        // 00 00 00 00
    DWORD m_dwUnknown13;        // 00 00 00 00
    unsigned char m_ausExtension[4];    // .TX2
    BOOL  m_bValid;             // Flag indicating that the data is valid
    CFileViewType *m_pFileViewType;  // Class for viewing the file contents
    DWORD m_dwSecondarySize;    // Size of secondary data
    const char *m_pszSecondaryData;   // Secondary Data
} ExternalInfo;
    
class CParsePigg  
{
public:
	CString GetDirectoryName(int nDirectoryEntry);
	DWORD GetDirectoryEntries(void)
    {
        return m_PiggHeader.m_dwDirEntCount;
    }
	void WriteOutput(LPCTSTR pstrFormat, ...);
	CParsePigg();
	virtual ~CParsePigg();
    BOOL ReadPigg(CArchive& ar);
    void DumpPigg(CString strOutfile);

    CString GetString(int nString);
    void GetExternalInfo(ExternalInfo &externalInfo, int nDirectoryEntry);

protected:
	BOOL ReadData(void *pvData, UINT nLength);
    PiggHeader m_PiggHeader;
	CFile *m_filePigg;
    CStdioFile m_fileOutput;
    PiggStringTable m_PiggStringTable;
    PiggSlotTable m_PiggSlotTable;
    CArray<PiggDirectoryEntry, PiggDirectoryEntry&>m_aDirectoryEntries;
	CMapStringToPtr m_mapExtensions;
    CFileViewType m_FileViewTypeDefault;
    CTypedPtrArray<CPtrArray, PiggSlotEntry *>m_aSlotEntries;
    CStringArray m_aStrings;
};

#endif // !defined(AFX_PARSEPIGG_H__CEE26132_D88D_4A6A_8131_C747C54124EB__INCLUDED_)
