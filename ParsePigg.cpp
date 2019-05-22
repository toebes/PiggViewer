// ParsePigg.cpp: implementation of the CParsePigg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PiggViewer.h"
#include "ParsePigg.h"
#include "PiggViewerDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParsePigg::CParsePigg()
{
    m_mapExtensions[".bin"] = new CFileViewTypeBin();
    m_mapExtensions[".ttf"] = new CFileViewTypeTTF();
    m_mapExtensions[".TTF"] = new CFileViewTypeTTF();
    m_mapExtensions[".ttc"] = new CFileViewTypeTTF();
    m_mapExtensions[".geo"] = new CFileViewTypeGeo();
    m_mapExtensions[".anim"] = new CFileViewTypeAnim();
    m_mapExtensions[".txt"] = new CFileViewTypeTxt();
    m_mapExtensions[".ogg"] = new CFileViewTypeOgg();
    m_mapExtensions[".texture"] = new CFileViewTypeTexture();
    m_mapExtensions[".tga"] = new CFileViewTypeTexture();
    m_mapExtensions[".rcp"] = new CFileViewTypeCode();
    m_mapExtensions[".vp"] = new CFileViewTypeCode();
    m_mapExtensions[".fp"] = new CFileViewTypeCode();
    m_mapExtensions[".ms"] = new CFileViewTypeCode();
    m_mapExtensions[".tec"] = new CFileViewTypeCode();
    m_mapExtensions[".mnu"] = new CFileViewTypeCode();
    m_mapExtensions[".def"] = new CFileViewTypeCode();
    m_mapExtensions[".types"] = new CFileViewTypeCode();
    m_mapExtensions[".old"] = new CFileViewTypeCode();
    m_mapExtensions[".ch"] = new CFileViewTypeCode();
    m_mapExtensions[".ko"] = new CFileViewTypeCode();
}

BOOL CParsePigg::ReadPigg(CArchive& ar)
{
    m_filePigg = ar.GetFile();
    CPiggViewerDoc *pDoc = (CPiggViewerDoc *)ar.m_pDocument;
    ULONGLONG dwSlotTableStart;
    if (!ReadData(&m_PiggHeader, sizeof(m_PiggHeader)))
    {
        return FALSE;
    }
    if ((m_PiggHeader.m_dwMarker   != MARKER_PIGG_START) ||
        (m_PiggHeader.m_usUnknown2 != UNKNOWN2_VALUE) ||
        (m_PiggHeader.m_usUnknown3 != UNKNOWN3_VALUE) ||
        (m_PiggHeader.m_usUnknown4 != UNKNOWN4_VALUE) ||
        (m_PiggHeader.m_usUnknown5 != UNKNOWN5_VALUE))
    {
        CString strOutput;
        strOutput.Format("Bad Header: U1=%08lx vs %08lx, U2=%04lx vs %04lx, U3=%04lx vs %04lx, U4=%04lx vs %04lx, U5=%04lx vs %04lx",
            m_PiggHeader.m_dwMarker, MARKER_PIGG_START,
            m_PiggHeader.m_usUnknown2, UNKNOWN2_VALUE,
            m_PiggHeader.m_usUnknown3, UNKNOWN3_VALUE,
            m_PiggHeader.m_usUnknown4, UNKNOWN4_VALUE,
            m_PiggHeader.m_usUnknown5, UNKNOWN5_VALUE);
        ::AfxMessageBox(strOutput, MB_OK);
        return FALSE;
    }
    //
    // Allocate space for all the directory entries
    //
    m_aDirectoryEntries.SetSize(m_PiggHeader.m_dwDirEntCount);
    // We got a header so now we need to read the data
    for(unsigned int nDirEnt = 0; nDirEnt < m_PiggHeader.m_dwDirEntCount; nDirEnt++)
    {
        PiggDirectoryEntry *pde = &m_aDirectoryEntries[nDirEnt];
        if (!ReadData(&m_aDirectoryEntries[nDirEnt], sizeof(PiggDirectoryEntry)))
        {
            return FALSE;
        }
    }
    //
    // Now we need to read in the string table
    //
    if (!ReadData(&m_PiggStringTable, sizeof(m_PiggStringTable)))
    {
        return FALSE;
    }

    if (m_PiggStringTable.m_dwMarker != MARKER_STRING_TABLE)
    {
        CString strOutput;
        strOutput.Format("Bad String Header: %08lx vs %08lx", m_PiggStringTable.m_dwMarker, MARKER_STRING_TABLE);
        ::AfxMessageBox(strOutput, MB_OK);
        return FALSE;
    }
    m_aStrings.SetSize(m_PiggStringTable.m_dwStringCount);
    //
    // Figure out where the SlotTable will start at
    //
    dwSlotTableStart = m_filePigg->GetPosition() + m_PiggStringTable.m_dwLength;
    //
    // Read in each of the strings
    //
    for(unsigned int nStringEnt = 0; nStringEnt < m_PiggStringTable.m_dwStringCount; nStringEnt++)
    {
        PiggSlotLength slotLength;
        LPTSTR lpstrString;
        if (!ReadData(&slotLength, sizeof(slotLength)))
        {
            return FALSE;
        }
        lpstrString = m_aStrings[nStringEnt].GetBuffer(slotLength.m_dwEntryLength);
        if (!ReadData(lpstrString, slotLength.m_dwEntryLength))
        {
            return FALSE;
        }
        m_aStrings[nStringEnt].ReleaseBuffer(-1);
    }
    //
    // Seek to the start of the SlotTable
    //
    m_filePigg->Seek(dwSlotTableStart, CFile::begin);
    //
    // Now we read in the information on the Slot Table
    //
    if (!ReadData(&m_PiggSlotTable, sizeof(m_PiggSlotTable)))
    {
        return FALSE;
    }
    if (m_PiggSlotTable.m_dwMarker != MARKER_SLOT_TABLE)
    {
        CString strOutput;
        strOutput.Format("Bad String Header: %08lx vs %08lx", m_PiggSlotTable.m_dwMarker, MARKER_SLOT_TABLE);
        ::AfxMessageBox(strOutput, MB_OK);
        return FALSE;
    }
    m_aSlotEntries.SetSize(m_PiggSlotTable.m_dwEntryCount);
    //
    // Now read in each entry from the table
    //
    for(unsigned int nSlotEnt = 0; nSlotEnt < m_PiggSlotTable.m_dwEntryCount; nSlotEnt++)
    {
        PiggSlotLength slotLength;
        if (!ReadData(&slotLength, sizeof(slotLength)))
        {
            return FALSE;
        }
        if (slotLength.m_dwEntryLength != 0x74)
        {
            //
            // We need to decompress the data.. but for now we just read the data in
            //
            BOOL rc;
            PiggCompressedSlotEntry *pCompressedSlotSource;
            PiggCompressedSlotEntry *pCompressedSlotDest;
            pCompressedSlotSource = (PiggCompressedSlotEntry *)new char[slotLength.m_dwEntryLength];
            if (!ReadData(pCompressedSlotSource, slotLength.m_dwEntryLength))
            {
                delete pCompressedSlotSource;
                return FALSE;
            }
            if (slotLength.m_dwEntryLength == pCompressedSlotSource->m_dwLength)
            {

  
                pCompressedSlotDest = (PiggCompressedSlotEntry *)new char[slotLength.m_dwEntryLength 
                                                                          + sizeof(PiggCompressedSlotEntry)
                                                                          - sizeof(PiggUnCompressedSlotEntry)];
                m_aSlotEntries[nSlotEnt] = (PiggSlotEntry *)pCompressedSlotDest;
                pCompressedSlotDest->m_dwLength = pCompressedSlotSource->m_dwLength;
                pCompressedSlotDest->m_dwUncompressedSize = pCompressedSlotSource->m_dwLength;
                memcpy(pCompressedSlotDest->m_aucData, &pCompressedSlotSource->m_dwUncompressedSize, slotLength.m_dwEntryLength-sizeof(PiggUnCompressedSlotEntry));
            }
            else
            {
                pCompressedSlotDest = (PiggCompressedSlotEntry *)new char[sizeof(PiggCompressedSlotEntry) + pCompressedSlotSource->m_dwUncompressedSize];
                m_aSlotEntries[nSlotEnt] = (PiggSlotEntry *)pCompressedSlotDest;
                pCompressedSlotDest->m_dwLength = pCompressedSlotSource->m_dwLength;
                pCompressedSlotDest->m_dwUncompressedSize = pCompressedSlotSource->m_dwUncompressedSize;
                rc = pDoc->UnCompress((BYTE *)pCompressedSlotDest->m_aucData, pCompressedSlotDest->m_dwUncompressedSize, pCompressedSlotSource->m_aucData, pCompressedSlotSource->m_dwLength);
            }
            delete pCompressedSlotSource;
        }
        else
        {
            PiggSlotEntry *piggSlotEntry = NULL;
            piggSlotEntry = (PiggSlotEntry *)new char[slotLength.m_dwEntryLength];
            m_aSlotEntries[nSlotEnt] = piggSlotEntry;
            if (!ReadData(piggSlotEntry, slotLength.m_dwEntryLength))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

CParsePigg::~CParsePigg()
{
    for(int i = 0; i < m_aSlotEntries.GetSize(); i++)
    {
        delete m_aSlotEntries[i];
    }
    //
    // We need to clean out all of the extensions
    //
    CMapStringToOb map;
    POSITION pos;
    CString key;
    CFileViewType* pFileViewType;
    for( pos = m_mapExtensions.GetStartPosition(); pos != NULL; )
    {
        m_mapExtensions.GetNextAssoc( pos, key, (void*&)pFileViewType );
        delete pFileViewType;
        m_mapExtensions.RemoveKey(key);
    }
}

//Using CString::FormatV(), you can write functions like the following:

void CParsePigg::WriteOutput(LPCTSTR pstrFormat, ...)
{
    CString strOutput;
   // format and write the data we were given
   va_list args;
   va_start(args, pstrFormat);
   strOutput.FormatV(pstrFormat, args);
   m_fileOutput.WriteString(strOutput);
}


void CParsePigg::DumpPigg(CString strOutfile)
{
    m_fileOutput.Open(strOutfile, CFile::modeCreate |CFile::modeWrite|CFile::typeText);
    CString strOutput;
    //
    // Dump out the header
    //
    WriteOutput("%d Directory entries\n", m_PiggHeader.m_dwDirEntCount);
    for(UINT nDirEntry = 0; nDirEntry < m_PiggHeader.m_dwDirEntCount; nDirEntry++)
    {
        CTime timeEntry((time_t)m_aDirectoryEntries[nDirEntry].m_dwDateStamp);


        WriteOutput("\n %3d: Flags=%08lx Date=%s String=%d Slot=%d Pos=%08lx Length=%08lx\n",
                           nDirEntry,
                           m_aDirectoryEntries[nDirEntry].m_dwFileSize,
                           timeEntry.Format("%d-%b-%Y %I:%M%p"),
                           m_aDirectoryEntries[nDirEntry].m_dwStringNum,
                           m_aDirectoryEntries[nDirEntry].m_dwIndex,
                           m_aDirectoryEntries[nDirEntry].m_dwFileStart,
                           m_aDirectoryEntries[nDirEntry].m_dwCompressedSize);
        WriteOutput("      Key= %02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x\n",
                           m_aDirectoryEntries[nDirEntry].m_ausMD5[ 0], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 1], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 2], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 3],
                           m_aDirectoryEntries[nDirEntry].m_ausMD5[ 4], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 5], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 6], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 7],
                           m_aDirectoryEntries[nDirEntry].m_ausMD5[ 8], m_aDirectoryEntries[nDirEntry].m_ausMD5[ 9], m_aDirectoryEntries[nDirEntry].m_ausMD5[10], m_aDirectoryEntries[nDirEntry].m_ausMD5[11],
                           m_aDirectoryEntries[nDirEntry].m_ausMD5[12], m_aDirectoryEntries[nDirEntry].m_ausMD5[13], m_aDirectoryEntries[nDirEntry].m_ausMD5[14], m_aDirectoryEntries[nDirEntry].m_ausMD5[15]);
        if (m_aDirectoryEntries[nDirEntry].m_dwStringNum < (DWORD)(m_aStrings.GetSize()))
        {
            WriteOutput("      Str:%s\n", m_aStrings[m_aDirectoryEntries[nDirEntry].m_dwStringNum]);
        }
        if (m_aDirectoryEntries[nDirEntry].m_dwIndex < (DWORD)(m_aSlotEntries.GetSize()))
        {
            PiggSlotEntry *piggSlotEntry = m_aSlotEntries[m_aDirectoryEntries[nDirEntry].m_dwIndex];
            WriteOutput("      Slot:%s\n", piggSlotEntry->m_ausTitle);
            WriteOutput("      Len=%d Ext=%c%c%c%c U8=%08lx U9=%08lx U10=%08lx U11=%08lx U12=%08lx U13=%08lx\n",
                               piggSlotEntry->m_dwLength,
                               (isprint(piggSlotEntry->m_ausExtension[0]) ? piggSlotEntry->m_ausExtension[0] : '.'),
                               (isprint(piggSlotEntry->m_ausExtension[1]) ? piggSlotEntry->m_ausExtension[1] : '.'),
                               (isprint(piggSlotEntry->m_ausExtension[2]) ? piggSlotEntry->m_ausExtension[2] : '.'),
                               (isprint(piggSlotEntry->m_ausExtension[3]) ? piggSlotEntry->m_ausExtension[3] : '.'),
                               piggSlotEntry->m_dwUnknown8,
                               piggSlotEntry->m_dwWidth,
                               piggSlotEntry->m_dwHeight,
                               piggSlotEntry->m_dwUnknown11,
                               piggSlotEntry->m_dwUnknown12,
                               piggSlotEntry->m_dwUnknown13);
        }
    }
    //
    // Now dump out all the sections
    //
    for(nDirEntry = 0; nDirEntry < m_PiggHeader.m_dwDirEntCount; nDirEntry++)
    {
        m_filePigg->Seek(m_aDirectoryEntries[nDirEntry].m_dwFileStart, CFile::begin);
        DWORD dwLength = m_aDirectoryEntries[nDirEntry].m_dwCompressedSize;
        if (dwLength == 0)
        {
            dwLength = m_aDirectoryEntries[nDirEntry].m_dwFileSize;
        }
        if (dwLength == 0)
        {
            continue;
        }
        CString strTitle;
        if (m_aDirectoryEntries[nDirEntry].m_dwStringNum < (DWORD)(m_aStrings.GetSize()))
        {
            strTitle = m_aStrings[m_aDirectoryEntries[nDirEntry].m_dwStringNum];
        }

        WriteOutput("\n====Section %d Start=%08lx Len=%08lx (%d) %s====\n",
                    nDirEntry,
                    m_aDirectoryEntries[nDirEntry].m_dwFileStart,
                    dwLength, dwLength, strTitle);
        for(UINT dwPos = 0; dwPos < dwLength; dwPos += 16)
        {
            int nRemain = dwLength-dwPos;
            int nSpot = dwPos % 16;
            char aucData[16];
            char aucAscii[16+(16/4)+1];
            char aucHex[(3*16)+(16/4)+1];

            if (nRemain > 16)
            {
                nRemain = 16;
            }
            if (!ReadData(aucData, nRemain))
            {
                return;
            }
            memset(aucHex,' ', sizeof(aucHex));
            aucHex[sizeof(aucHex)-1] = 0;
            memset(aucAscii,' ', sizeof(aucAscii));
            aucAscii[sizeof(aucAscii)-1] = 0;
            for(int nPos = 0; nPos < nRemain; nPos++)
            {
                int nDest = (nPos*3) + (nPos/4);
                int c = aucData[nPos];
                aucAscii[nPos + nPos/4] = isprint(c) ? c : '.';
                aucHex[nDest]   = "0123456789ABCDEF"[(c>>4)&0xf];
                aucHex[nDest+1] = "0123456789ABCDEF"[(c   )&0xf];
            }
            WriteOutput(" %08lx: %s  |%s|\n", dwPos, aucHex, aucAscii);
        }
    }
}

BOOL CParsePigg::ReadData(void *pvData, UINT nLength)
{
    UINT nReadLength;
    nReadLength = m_filePigg->Read(pvData, nLength);
    if (nReadLength < nLength)
    {
        CString strOutput;
        strOutput.Format("Only able to read %d bytes from %d expected", nReadLength, nLength);
        ::AfxMessageBox(strOutput, MB_OK);
        return FALSE;
    }
    return TRUE;
}


CString CParsePigg::GetDirectoryName(int nDirectoryEntry)
{
    if ((nDirectoryEntry < 0) || (((DWORD)nDirectoryEntry) >= m_PiggHeader.m_dwDirEntCount))
    {
        return "";
    }
    return m_aStrings[m_aDirectoryEntries[nDirectoryEntry].m_dwStringNum];
}

void CParsePigg::GetExternalInfo(ExternalInfo &externalInfo, int nDirectoryEntry)
{
    int nExtPos;
    CString strExtension;
    externalInfo.m_strName = "";
    externalInfo.m_strTitle = "";
    externalInfo.m_dwFileSize = 0;
    externalInfo.m_dwDateStamp = 0;
    externalInfo.m_dwFileStart = 0;
    externalInfo.m_dwUnknown6 = 0;
    externalInfo.m_dwIndex = 0;
    memset(externalInfo.m_ausMD5, 0, sizeof(externalInfo.m_ausMD5));
    externalInfo.m_dwCompressedSize = 0;
    externalInfo.m_dwSlotLength = 0;
    externalInfo.m_dwUnknown8 = 0;
    externalInfo.m_dwWidth = 0;
    externalInfo.m_dwHeight = 0;
    externalInfo.m_dwUnknown11 = 0;
    externalInfo.m_dwUnknown12 = 0;
    externalInfo.m_dwUnknown13 = 0;
    externalInfo.m_dwSecondarySize = 0;
    externalInfo.m_pszSecondaryData = NULL;
    externalInfo.m_pFileViewType = &m_FileViewTypeDefault;

    memset(externalInfo.m_ausExtension, ' ', sizeof(externalInfo.m_ausExtension));

    if ((nDirectoryEntry < 0) || (((DWORD)nDirectoryEntry) >= m_PiggHeader.m_dwDirEntCount))
    {
        return;
    }
    PiggDirectoryEntry *pde = &m_aDirectoryEntries[nDirectoryEntry];

    externalInfo.m_dwCompressedSize = m_aDirectoryEntries[nDirectoryEntry].m_dwCompressedSize;
    externalInfo.m_strName = m_aStrings[m_aDirectoryEntries[nDirectoryEntry].m_dwStringNum];
    externalInfo.m_dwFileSize = m_aDirectoryEntries[nDirectoryEntry].m_dwFileSize;
    externalInfo.m_dwDateStamp = m_aDirectoryEntries[nDirectoryEntry].m_dwDateStamp;
    externalInfo.m_dwFileStart = m_aDirectoryEntries[nDirectoryEntry].m_dwFileStart;
    externalInfo.m_dwUnknown6 = m_aDirectoryEntries[nDirectoryEntry].m_dwUnknown6;
    externalInfo.m_dwIndex = m_aDirectoryEntries[nDirectoryEntry].m_dwIndex;
    memcpy(externalInfo.m_ausMD5, m_aDirectoryEntries[nDirectoryEntry].m_ausMD5, sizeof(externalInfo.m_ausMD5));
    if (m_aDirectoryEntries[nDirectoryEntry].m_dwIndex < (DWORD)(m_aSlotEntries.GetSize()))
    {
        PiggSlotEntry *piggSlotEntry = m_aSlotEntries[m_aDirectoryEntries[nDirectoryEntry].m_dwIndex];
        if (piggSlotEntry->m_dwLength == 0x70)
        {
            externalInfo.m_dwSlotLength = piggSlotEntry->m_dwLength;
            externalInfo.m_dwUnknown8 = piggSlotEntry->m_dwUnknown8;
            externalInfo.m_dwWidth = piggSlotEntry->m_dwWidth;
            externalInfo.m_dwHeight = piggSlotEntry->m_dwHeight;
            externalInfo.m_dwUnknown11 = piggSlotEntry->m_dwUnknown11;
            externalInfo.m_dwUnknown12 = piggSlotEntry->m_dwUnknown12;
            externalInfo.m_dwUnknown13 = piggSlotEntry->m_dwUnknown13;
            memcpy(externalInfo.m_ausExtension, piggSlotEntry->m_ausExtension, sizeof(externalInfo.m_ausExtension));
            externalInfo.m_strTitle = piggSlotEntry->m_ausTitle;
        }
        else
        {
            externalInfo.m_dwIndex = 0xffffffff;
            PiggCompressedSlotEntry *pCompressedSlotEntry = (PiggCompressedSlotEntry *)piggSlotEntry;
            externalInfo.m_dwSecondarySize = pCompressedSlotEntry->m_dwUncompressedSize;
            externalInfo.m_pszSecondaryData = (const char *)pCompressedSlotEntry->m_aucData;
        }
    }
    //
    // Determine the file type based on the extension
    //
    nExtPos = externalInfo.m_strName.ReverseFind('.');
    if (nExtPos == -1)
    {
        strExtension = externalInfo.m_strName;
    }
    else
    {
        strExtension = externalInfo.m_strName.Mid(nExtPos);
    }

    externalInfo.m_pFileViewType = (CFileViewType*)m_mapExtensions[strExtension];
    if (externalInfo.m_pFileViewType == NULL)
    {
        AfxMessageBox(strExtension, MB_OK);
        externalInfo.m_pFileViewType = &m_FileViewTypeDefault;
    }
}
