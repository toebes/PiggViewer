// FileViewType.cpp: implementation of the CFileViewType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PiggViewer.h"
#include "FileViewType.h"
#include "HexView.h"
#include "TextView.h"
#include "TextureView.h"
#include "StringLong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFileViewType::CFileViewType()
{

}

CFileViewType::~CFileViewType()
{

}

CRuntimeClass * CFileViewType::GetViewClass()
{
    return RUNTIME_CLASS(CHexView);
}

CString CFileViewType::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "Unknown File Type";
}

void CFileViewType::UpdateIndexList(CTreeCtrl &treeCtrl, const BYTE *pData, DWORD dwLength)
{
    treeCtrl.DeleteAllItems();
    HTREEITEM hItemNew = treeCtrl.InsertItem("Contents", TVI_ROOT, TVI_SORT);
    if (hItemNew != NULL)
    {
        treeCtrl.SetItemData(hItemNew, 0);
    }
}

//////////////////////////////////////////////////////////////////////
// CFileViewTypeBin
//////////////////////////////////////////////////////////////////////
typedef enum {
    SECTION_UNKNOWN,
    SECTION_DEF,
    SECTION_GROUP,
    SECTION_GROUP_GEOBIN,
    SECTION_GROUP_ATTRIB_NAMES,
    SECTION_GROUP_SEQUENCERS,
    SECTION_PROPERTY,
    SECTION_REPLACETEX,
    SECTION_BEACON,
    SECTION_SOUND,
    SECTION_AMBIENT,
    SECTION_TINTCOLOR,
    SECTION_FOG,
    SECTION_OMNI,
    SECTION_BADGE,
    SECTION_DAMAGE,
    SECTION_DEFENSE,
    SECTION_BOOST,
    SECTION_CLASS,
    SECTION_CAPE,
    SECTION_CAPE_CAPES,
    SECTION_CAPE_FXINFO,
    SECTION_BODYPART,
    SECTION_BEHAVIOR,
    SECTION_WINDINFO,
    SECTION_POWER,
    SECTION_POWER_ITEMS,
    SECTION_POWER_POWERS,
    SECTION_POWERCATEGORY,
    SECTION_POWERSET,
    SECTION_SYSTEM,
    SECTION_FXINFO,
    SECTION_EMOTE,
    SECTION_MOVE,
    SECTION_CYCLEMOVE,
    SECTION_TAILORCOSTSET,
    SECTION_TRICK,
    SECTION_STORE,
    SECTION_SELL,
    SECTION_BUY,
    SECTION_STANIM,
    SECTION_NPC,
    SECTION_TEXTURE,
    SECTION_COLOR,
    SECTION_COMMANDCATEGORY,
    SECTION_INPUT,
    SECTION_CATEGORY,
    SECTION_DEPARTMENT,
    SECTION_COSTUME,
    SECTION_COSTUME_VILLAINCOSTUME,
    SECTION_COSTUME_COSTUME,
    SECTION_ORIGIN,
    SECTION_REGION,
    SECTION_ITEM,
    SECTION_ITEM_ITEMS,
    SECTION_ITEM_HELP,
    SECTION_TYPE,
    SECTION_ANIM,
    SECTION_PLAYFX,
    SECTION_CONDITION,
    SECTION_EVENT,
    SECTION_MASK,
    SECTION_KEYPROFILE,
    SECTION_BONESET,
    SECTION_GEOSET,
    SECTION_ZONE,
    SECTION_TYPEDEF,
    SECTION_MENU,
    SECTION_INFO,
    SECTION_COMMAND,
    SECTION_KEYBIND,
    SECTION_COLINFO,
    SECTION_NEXTMOVE,
    SECTION_SEQUENCER,
    SECTION_LOD,
    SECTION_LOD1,
    SECTION_BODYPALETTE,
    SECTION_SKINPALETTE,
    SECTION_GEOM,
    SECTION_PART,
    SECTION_BONE,
    SECTION_HIGHERLEVEL,
    SECTION_LOWERLEVEL,
    SECTION_ATTRIBMAXTABLE,
    SECTION_STRENGTHMAXTABLE,
    SECTION_RESISTANCEMAXTABLE,
    SECTION_ATTRIBMAXMAXTABLE,
    SECTION_ATTRIBMIN,
    SECTION_STRENGTHMIN,
    SECTION_RESISTANCEMIN,
    SECTION_ATTRIBBASE,
    SECTION_MODTABLE,
    SECTION_TRIGGERBITS,
    SECTION_COSTUMEPART,
    SECTION_SPLAT,
    SECTION_REF,
	SECTION_MODE,
	SECTION_CARD,
	SECTION_CARD_CARDS,
	SECTION_CARD_DECKS,
	SECTION_DECK,
	SECTION_COMBATMODS,
	SECTION_DEVEMOTE,
	SECTION_MAPSITUATIONS,
	SECTION_SITUATION,
	SECTION_CHALLENGE,
	SECTION_STAGE,
    SECTION_FONT,
    SECTION_DROPSHADOW,
    SECTION_LAYER,
    SECTION_TEXWORD,
    SECTION_SIZE,
    SECTION_FILTER,
    SECTION_OFFSET,
    SECTION_SUBLAYER,
} eSectionIds;

CFileViewTypeBin::CFileViewTypeBin()
{
    m_mapSectionNames["Def"]                = (void *)SECTION_DEF;
    m_mapSectionNames["Ref"]                = (void *)SECTION_REF;
    m_mapSectionNames["Group"]              = (void *)SECTION_GROUP;
    m_mapSectionNames["Property"]           = (void *)SECTION_PROPERTY;
    m_mapSectionNames["ReplaceTex"]         = (void *)SECTION_REPLACETEX;
    m_mapSectionNames["Beacon"]             = (void *)SECTION_BEACON;
    m_mapSectionNames["Sound"]              = (void *)SECTION_SOUND;
    m_mapSectionNames["Ambient"]            = (void *)SECTION_AMBIENT;
    m_mapSectionNames["TintColor"]          = (void *)SECTION_TINTCOLOR;
    m_mapSectionNames["Lod"]                = (void *)SECTION_LOD1;
    m_mapSectionNames["Fog"]                = (void *)SECTION_FOG;
    m_mapSectionNames["Omni"]               = (void *)SECTION_OMNI;
    m_mapSectionNames["Badge"]              = (void *)SECTION_BADGE;
    m_mapSectionNames["Damage"]             = (void *)SECTION_DAMAGE;
    m_mapSectionNames["Defense"]            = (void *)SECTION_DEFENSE;
    m_mapSectionNames["Boost"]              = (void *)SECTION_BOOST;
    m_mapSectionNames["Class"]              = (void *)SECTION_CLASS;
    m_mapSectionNames["Cape"]               = (void *)SECTION_CAPE;
    m_mapSectionNames["BodyPart"]           = (void *)SECTION_BODYPART;
    m_mapSectionNames["Behavior"]           = (void *)SECTION_BEHAVIOR;
    m_mapSectionNames["WindInfo"]           = (void *)SECTION_WINDINFO;
    m_mapSectionNames["Power"]              = (void *)SECTION_POWER;
    m_mapSectionNames["PowerSet"]           = (void *)SECTION_POWERSET;
    m_mapSectionNames["System"]             = (void *)SECTION_SYSTEM;
    m_mapSectionNames["FxInfo"]             = (void *)SECTION_FXINFO;
    m_mapSectionNames["Emote"]              = (void *)SECTION_EMOTE;
    m_mapSectionNames["Move"]               = (void *)SECTION_MOVE;
    m_mapSectionNames["CycleMove"]          = (void *)SECTION_CYCLEMOVE;
    m_mapSectionNames["Trick"]              = (void *)SECTION_TRICK;
    m_mapSectionNames["Store"]              = (void *)SECTION_STORE;
    m_mapSectionNames["Sell"]               = (void *)SECTION_SELL;
    m_mapSectionNames["Buy"]                = (void *)SECTION_BUY;
    m_mapSectionNames["StAnim"]             = (void *)SECTION_STANIM;
    m_mapSectionNames["NPC"]                = (void *)SECTION_NPC;
    m_mapSectionNames["Texture"]            = (void *)SECTION_TEXTURE;
    m_mapSectionNames["Color"]              = (void *)SECTION_COLOR;
    m_mapSectionNames["Input"]              = (void *)SECTION_INPUT;
    m_mapSectionNames["Category"]           = (void *)SECTION_CATEGORY;
    m_mapSectionNames["Department"]         = (void *)SECTION_DEPARTMENT;
    m_mapSectionNames["Costume"]            = (void *)SECTION_COSTUME;
    m_mapSectionNames["Origin"]             = (void *)SECTION_ORIGIN;
    m_mapSectionNames["Region"]             = (void *)SECTION_REGION;
    m_mapSectionNames["Item"]               = (void *)SECTION_ITEM;
    m_mapSectionNames["Type"]               = (void *)SECTION_TYPE;
    m_mapSectionNames["Anim"]               = (void *)SECTION_ANIM;
    m_mapSectionNames["PlayFx"]             = (void *)SECTION_PLAYFX;
    m_mapSectionNames["Condition"]          = (void *)SECTION_CONDITION;
    m_mapSectionNames["Event"]              = (void *)SECTION_EVENT;
    m_mapSectionNames["Mask"]               = (void *)SECTION_MASK;
    m_mapSectionNames["KeyProfile"]         = (void *)SECTION_KEYPROFILE;
    m_mapSectionNames["BoneSet"]            = (void *)SECTION_BONESET;
    m_mapSectionNames["GeoSet"]             = (void *)SECTION_GEOSET;
    m_mapSectionNames["Zone"]               = (void *)SECTION_ZONE;
    m_mapSectionNames["TypeDef"]            = (void *)SECTION_TYPEDEF;
    m_mapSectionNames["Menu"]               = (void *)SECTION_MENU;
    m_mapSectionNames["Info"]               = (void *)SECTION_INFO;
    m_mapSectionNames["Command"]            = (void *)SECTION_COMMAND;
    m_mapSectionNames["KeyBind"]            = (void *)SECTION_KEYBIND;
    m_mapSectionNames["ColInfo"]            = (void *)SECTION_COLINFO;
    m_mapSectionNames["NextMove"]           = (void *)SECTION_NEXTMOVE;
    m_mapSectionNames["Sequencer"]          = (void *)SECTION_SEQUENCER;
    m_mapSectionNames["LOD"]                = (void *)SECTION_LOD;
    m_mapSectionNames["BodyPalette"]        = (void *)SECTION_BODYPALETTE;
    m_mapSectionNames["SkinPalette"]        = (void *)SECTION_SKINPALETTE;
    m_mapSectionNames["Geom"]               = (void *)SECTION_GEOM;
    m_mapSectionNames["Part"]               = (void *)SECTION_PART;
    m_mapSectionNames["Bone"]               = (void *)SECTION_BONE;
    m_mapSectionNames["HigherLevel"]        = (void *)SECTION_HIGHERLEVEL;
    m_mapSectionNames["LowerLevel"]         = (void *)SECTION_LOWERLEVEL;
    m_mapSectionNames["AttribMin"]          = (void *)SECTION_ATTRIBMIN;
    m_mapSectionNames["StrengthMin"]        = (void *)SECTION_STRENGTHMIN;
    m_mapSectionNames["AttribBase"]         = (void *)SECTION_ATTRIBBASE;
    m_mapSectionNames["ModTable"]           = (void *)SECTION_MODTABLE;
    m_mapSectionNames["PowerCategory"]      = (void *)SECTION_POWERCATEGORY;
    m_mapSectionNames["TailorCostSet"]      = (void *)SECTION_TAILORCOSTSET;
    m_mapSectionNames["CommandCategory"]    = (void *)SECTION_COMMANDCATEGORY;
    m_mapSectionNames["AttribMaxTable"]     = (void *)SECTION_ATTRIBMAXTABLE;
    m_mapSectionNames["StrengthMaxTable"]   = (void *)SECTION_STRENGTHMAXTABLE;
    m_mapSectionNames["ResistanceMaxTable"] = (void *)SECTION_RESISTANCEMAXTABLE;
    m_mapSectionNames["AttribMaxMaxTable"]  = (void *)SECTION_ATTRIBMAXMAXTABLE;
    m_mapSectionNames["ResistanceMin"]      = (void *)SECTION_RESISTANCEMIN;
    m_mapSectionNames["TriggerBits"]        = (void *)SECTION_TRIGGERBITS;
    m_mapSectionNames["CostumePart"]        = (void *)SECTION_COSTUMEPART;
    m_mapSectionNames["Splat"]              = (void *)SECTION_SPLAT;
	m_mapSectionNames["Mode"]               = (void *)SECTION_MODE;
	m_mapSectionNames["Card"]               = (void *)SECTION_CARD;
	m_mapSectionNames["Deck"]               = (void *)SECTION_DECK;
	m_mapSectionNames["CombatMods"]         = (void *)SECTION_COMBATMODS;
	m_mapSectionNames["DevEmote"]           = (void *)SECTION_DEVEMOTE;
	m_mapSectionNames["Situation"]          = (void *)SECTION_SITUATION;
	m_mapSectionNames["MapSituations"]      = (void *)SECTION_MAPSITUATIONS;
	m_mapSectionNames["Challenge"]          = (void *)SECTION_CHALLENGE;
	m_mapSectionNames["Stage"]              = (void *)SECTION_STAGE;
	m_mapSectionNames["Font"]               = (void *)SECTION_FONT;
	m_mapSectionNames["DropShadow"]         = (void *)SECTION_DROPSHADOW;
	m_mapSectionNames["Layer"]              = (void *)SECTION_LAYER;
	m_mapSectionNames["TexWord"]            = (void *)SECTION_TEXWORD;
	m_mapSectionNames["Size"]               = (void *)SECTION_SIZE;
	m_mapSectionNames["Filter"]             = (void *)SECTION_FILTER;
	m_mapSectionNames["Offset"]             = (void *)SECTION_OFFSET;
	m_mapSectionNames["SubLayer"]           = (void *)SECTION_SUBLAYER;
}

CFileViewTypeBin::~CFileViewTypeBin()
{

}

CRuntimeClass * CFileViewTypeBin::GetViewClass()
{
    return RUNTIME_CLASS(CTextView);
}

CString DumpHex(const BYTE *pData, DWORD dwLength, CString strIndent)
{
    CStringLong strFinal;
    CString strLine;
    CString strAscii;

    if (dwLength > 0xffff) { dwLength = 0xffff; }

    for(DWORD i = 0; i < dwLength; i++)
    {
        CString strTemp;
        if ((i & 15) == 0)
        {
            if (!strLine.IsEmpty())
            {
                strFinal += strLine;
                strFinal += "  |";
                strFinal += strAscii;
                strFinal += "|\r\n" + strIndent;
                strLine.Empty();
                strAscii.Empty();
            }
            //
            // Put the current address on the line
            //
            strTemp.Format("%08lx: ", i);
            strFinal += strIndent + strTemp;
        }
        strTemp.Format("%02x ", *pData);
        strLine += strTemp;
        if (isprint(*pData))
        {
            strAscii += (char)*pData;
        }
        else
        {
            strAscii += '.';
        }
        pData++;
    }
    //
    // Output any trailing line
    //
    if (!strLine.IsEmpty())
    {
        CString strPad(' ', (16*3)-strLine.GetLength());
        strFinal += strLine;
        strFinal += strPad;
        strFinal += "  |";
        strAscii += "                ";
        strFinal += strAscii.Left(16);
        strFinal += "|";
    }
//    strFinal += "\r\n" + strIndent;
    return strFinal.GetString();
}
CString DumpString(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    if ((nPos+1) > dwLength)
    {
        strFinal.Format("***Invalid: nPos=%d dwLength=%d\r\n", nPos, dwLength);
    }
    else
    {
        short sLength = *(short *)(pData + nPos);
        nPos += sizeof(short);
        if ((sLength + nPos) > dwLength)
        {
            strFinal.Format("****Invalid: String %d length extends beyond end %d\r\n", sLength, nPos);
            nPos = dwLength;
        }
        else
        {
            strFinal.Format("%d: '%.*s'\r\n", sLength, sLength, pData+nPos);
            nPos += sLength;
        }
    }
    return strFinal;
}
CString DumpRoundUp(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    int nSkip = (4 - (nPos & 3)) & 3;
    if (nSkip)
    {
        if ((nPos + nSkip) > dwLength)
        {
            strFinal.Format("***Invalid: Too many bytes (%d) to skip beyond end %d\r\n", nPos, dwLength);
        }
        else
        {
            BOOL bDisplay = FALSE;
            strFinal.Format("Skip %d: ", nSkip);
            for(int i = 0; i < nSkip; i++)
            {
                CString strTemp;
                if (pData[nPos+i])
                {
                    bDisplay = TRUE;
                }
                strTemp.Format("%02x ", pData[nPos+i]);
                strFinal += strTemp;
            }
            if (bDisplay)
            {
                strFinal += "\r\n";
            }
            else
            {
                strFinal.Empty();
            }
            nPos += nSkip;
        }
    }
    return strFinal;
}


CString DumpLong(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    if ((nPos + sizeof(long)) > dwLength)
    {
        strFinal.Format("***Invalid: Not enough bytes from %d for a long in %d\r\n", nPos, dwLength);
    }
    else
    {
        strFinal.Format("0x%08lx\r\n", *(long *)(pData+nPos));
        nPos += sizeof(long);
    }
    return strFinal;
}

CString GetString(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    if ((nPos+1) > dwLength)
    {
        strFinal.Format("***Invalid: nPos=%d dwLength=%d\r\n", nPos, dwLength);
    }
    else
    {
        short sLength = *(short *)(pData + nPos);
        nPos += sizeof(short);
        if ((sLength + nPos) > dwLength)
        {
            strFinal.Format("****Invalid: String %d length extends beyond end %d\r\n", sLength, nPos);
            nPos = dwLength;
        }
        else
        {
            strFinal.Format("%.*s", sLength, pData+nPos);
            nPos += sLength;
            strFinal += DumpRoundUp(pData, nPos, dwLength);
        }
    }
    return strFinal;
}

CString GetLong(const BYTE *pData, DWORD &nPos, DWORD dwLength, DWORD *outVal)
{
    DWORD dwTemp;
    if (outVal == NULL)
    {
        outVal = &dwTemp;
    }
    CString strFinal;
    if ((nPos + sizeof(long)) > dwLength)
    {
        strFinal.Format("***Invalid: Not enough bytes from %d for a long in %d\r\n", nPos, dwLength);
    }
    else
    {
        *outVal = *(long *)(pData+nPos);
        strFinal.Format("0x%08lx", *outVal);
        nPos += sizeof(long);
    }
    return strFinal;
}

CString GetStringList(const BYTE *pData, DWORD &nPos, DWORD dwLength, CString strTitle, CString strEOL)
{
    DWORD dwStringCount;
    CString strFinal;
    DWORD nEntry;
    
    GetLong(pData, nPos, dwLength, &dwStringCount);
    strFinal.Format(" %d ", dwStringCount);
    strFinal += strTitle + strEOL;
    if (dwStringCount > 25)
    {
        strFinal += "******ERROR";
        dwStringCount = 1;
    }
    for (nEntry = 0; nEntry < dwStringCount; nEntry++)
    {
        strFinal += GetString(pData, nPos, dwLength) + strEOL;
    }
    return strFinal;
}

CString GetFloat(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    if ((nPos + sizeof(long)) > dwLength)
    {
        strFinal.Format("***Invalid: Not enough bytes from %d for a FLOAT in %d\r\n", nPos, dwLength);
    }
    else
    {
        float fTemp = *(float *)(pData+nPos);
        strFinal.Format("%.2f", fTemp);
        nPos += sizeof(long);
    }
    return strFinal;
}

CString DumpLongTable(const BYTE *pData, DWORD &nPos, DWORD dwLength, CString strIndent)
{
    DWORD dwValues;
    CStringLong strFinal;
    CString strTemp;
    GetLong(pData, nPos, dwLength, &dwValues);

    strTemp.Format("<%d:", dwValues);
    strFinal += strTemp;
    if (dwValues > 250)
    {
        strTemp.Format("*****Error (%08lx)", dwValues);
        strFinal += strTemp;
        dwValues = 1;
    }
    for(DWORD nEntry = 0; nEntry < dwValues; nEntry++)
    {
        if (nEntry)
        {
            strFinal += ", ";
        }
        strFinal += GetLong(pData, nPos, dwLength, NULL);
    }
    strFinal += ">\r\n" + strIndent;
    return strFinal.GetString();
}


CString DumpFloatTable(const BYTE *pData, DWORD &nPos, DWORD dwLength, CString strIndent)
{
    DWORD dwValues;
    CStringLong strFinal;
    CString strTemp;

    GetLong(pData, nPos, dwLength, &dwValues);
    strTemp.Format("%d Values\r\n", dwValues);
    strFinal += strTemp + strIndent;
    if (dwValues > 250)
    {
        strFinal += "******ERROR";
        dwValues = 1;
    }
    for(DWORD i = 0; i < dwValues; i++)
    {
        FLOAT fTest;
        GetLong(pData, nPos, dwLength, (DWORD *)&fTest);
        strTemp.Format("%3d: %.2f\r\n", i, fTest);
        strFinal +=  strTemp + strIndent;
    }
    return strFinal.GetString();
}

CString CFileViewTypeBin::DumpSection(const BYTE *pData, DWORD &nPos, DWORD dwLength, CString strIndent, BOOL bContiguous)
{
    static const char *aszBadgeTypes[] = {"NULL", "Exploration", "History", "Accomplishment", "Achievement", "Accolade" };
    CStringLong strFinal;
    CString strEOL1 = "\r\n" + strIndent + "   ";
    CString strEOL2 = "\r\n" + strIndent + "    ";
    while(nPos < dwLength)
    {
        DWORD nStartPos;
        DWORD nPosSave;
        DWORD dwTotalLength;
        DWORD dwInnerLength;
        DWORD dwRemainder;
        DWORD dwTemp1;
        DWORD dwTemp2;
        CString strTotal;
        CString strInner;
        CString strDef;
        eSectionIds eThisSection;
        DWORD nEntry;
        int nSet = 0;
        CString strTemp;
        
        CString strValue3;
        
        CString strType = GetString(pData, nPos, dwLength);
        strTotal = GetLong(pData, nPos, dwLength, &dwTotalLength);
        strInner = GetLong(pData, nPos, dwLength, &dwInnerLength);

        if (((nPos + dwTotalLength) > (dwLength+8)) ||
            ((nPos + dwInnerLength) > (dwLength+8)) ||
            (dwInnerLength > dwTotalLength))
        {
            //
            // We are out of sync
            //
            strTemp.Format("***Out of Sync: nPos=%08lx dwTotalLength = %08lx dwInnerLength = %08lx dwLength=%08lx\r\n", nPos, dwTotalLength, dwInnerLength, dwLength);
            strFinal += strTemp;
            break;
        }
        nStartPos = nPos;
        strFinal += strIndent + strType + ":" + strEOL1;
        if (!m_mapSectionNames.Lookup(strType, (void *&)eThisSection))
        {
            eThisSection = SECTION_UNKNOWN;
        }        
        
        switch(eThisSection)
        {
        case SECTION_LOD:
        case SECTION_REF:
        case SECTION_ITEM:
        case SECTION_BODYPALETTE:           // costume.bin
        case SECTION_SKINPALETTE:           // costume.bin
        case SECTION_CAPE:
        case SECTION_COSTUME:
        case SECTION_POWER:
        case SECTION_CARD:              // NONE
            if (dwInnerLength & 3)
            {
                dwInnerLength = dwTotalLength - sizeof(long);
            }
            // These actually have no content
            break;

        case SECTION_SEQUENCER:             // sequencers.bin
        case SECTION_COLINFO:               // clothColInfo.bin
        case SECTION_NEXTMOVE:              // sequencers.bin
        case SECTION_GROUP_SEQUENCERS:      // sequencers.bin
        case SECTION_BEHAVIOR:              // behaviors.bin
        case SECTION_COMMANDCATEGORY:       // command.bin
        case SECTION_INPUT:                 // fxinfo.bin
        case SECTION_CATEGORY:              // help.bin
        case SECTION_DEPARTMENT:            // depts.bin
        case SECTION_STORE:                 // stores.bin
        case SECTION_CYCLEMOVE:             // sequencers.bin
        case SECTION_COSTUME_COSTUME:       // costume.bin
        case SECTION_TEXWORD:               // texWords.bin
            strFinal += GetString(pData, nPos, dwLength);
            break;

        case SECTION_ITEM_HELP:             // help.bin
        case SECTION_REGION:                // costume.bin
        case SECTION_MASK:
        case SECTION_KEYPROFILE:            // kb.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;

        case SECTION_TRICK:                 // tricks.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";  // Always zero
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetFloat(pData, nPos, dwLength);
            break;

        case SECTION_SITUATION:             // situations.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetStringList(pData, nPos, dwLength, "Action", strEOL1);
            strFinal += GetString(pData, nPos, dwLength);
            break;

        case SECTION_CHALLENGE:             // situations.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_STAGE:                 // situations.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetStringList(pData, nPos, dwLength, "Action", strEOL1);
            strFinal += GetStringList(pData, nPos, dwLength, "Extra", strEOL1);
            break;

        case SECTION_POWERCATEGORY:         // powers.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
		case SECTION_MAPSITUATIONS:         // mapsituations.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;

        case SECTION_OFFSET:                // texWords.bin
        case SECTION_DROPSHADOW:            // texWords.bin
        case SECTION_SIZE:                  // texWords.bin
        case SECTION_COMBATMODS:            // combat_mods.bin, combat_mods_villain.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_FONT:                  // texWords.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            break;

        case SECTION_SUBLAYER:              // texWords.bin
        case SECTION_LAYER:                 // texWords.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetFloat(pData, nPos, dwLength);
            break;

        case SECTION_FILTER:                // texWords.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

#if 0
        case SECTION_SUBLAYER:              // texWords.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            break;
#endif

        case SECTION_CARD_DECKS:            // decks.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

		case SECTION_COMMAND:               // command.bin
        case SECTION_KEYBIND:               // kb.bin
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetString(pData, nPos, dwLength);
            break;

		case SECTION_MODE:                  // attrib_names
        case SECTION_DAMAGE:                // attrib_names
        case SECTION_BOOST:                 // attrib_names
        case SECTION_GROUP_ATTRIB_NAMES:    // attrib_names
		case SECTION_DECK:                  // decks.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;

        case SECTION_DEFENSE:
        case SECTION_TYPEDEF:               // sequencers.bin
        case SECTION_NPC:                   // VillainCostume.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;
            
        case SECTION_DEF:                   // geobin/object_library/*
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;
            
        case SECTION_CARD_CARDS:         // cards.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetStringList(pData, nPos, dwLength, "Type", strEOL1);
            strFinal += GetStringList(pData, nPos, dwLength, "UseTest", strEOL1);
            strFinal += GetStringList(pData, nPos, dwLength, "Execution", strEOL1);
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_SYSTEM:            // particles.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            nPosSave = nPos;
            strFinal += "S00: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "S01: " + DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
//            strFinal += "S02: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "S02: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ") + strEOL1;
            strFinal += "S03: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + strEOL1;
//            strFinal += "S03: " + DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            strFinal += "S04: " + DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            strFinal += "S05: " + GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += "S06: " + DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            strFinal += "S07: " + GetFloat(pData, nPos, dwLength) + strEOL1;
//            strFinal += "S07: " + DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            nPos = nPosSave;
            break;

        case SECTION_EVENT:                 // fxinfo.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            break;
            
        case SECTION_POWERSET:          // powers.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            break;
                
        case SECTION_CLASS:             // classes.bin, villain_classes.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetStringList(pData, nPos, dwLength, "Origin", strEOL1);
            strFinal += GetStringList(pData, nPos, dwLength, "Type", strEOL1);
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            break;
            
        case SECTION_COSTUME_VILLAINCOSTUME:    // VillainCostume.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_COSTUMEPART:               // VillainCostume.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_PLAYFX:                // sequencers.bin
        case SECTION_ANIM:                  // sequencers.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_GROUP_GEOBIN:          // geobin/object_library/*
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
#if 0
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
#else
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength);
#endif
            break;

        case SECTION_SPLAT:
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength);
            break;

        case SECTION_WINDINFO:              // clothWindInfo.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;

            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_PROPERTY:              // geobin/object_library/*
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_REPLACETEX:
        case SECTION_BEACON:
            strValue3 = GetLong(pData, nPos, dwLength, NULL);
            strDef = GetString(pData, nPos, dwLength);
            strFinal += "    " + strDef + " = " + strValue3;
            break;
            
        case SECTION_TYPE:                  // sequencers.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            for(nEntry = 0; nEntry < 7; nEntry++)
            {
                strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            }
            break;
            
        case SECTION_SOUND:
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            for(nEntry = 0; nEntry < 3; nEntry++)
            {
                strFinal += GetFloat(pData, nPos, dwLength) + " ";
            }
            break;

        case SECTION_CONDITION:             // fxinfo.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            for(nEntry = 0; nEntry < 4; nEntry++)
            {
                strFinal += GetFloat(pData, nPos, dwLength) + " ";
            }
            break;

        case SECTION_FXINFO:            // fxinfo.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " "; // Always zero
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";      // Always 1.0
            strFinal += GetLong(pData, nPos, dwLength, NULL);       // Always zero
            break;

        case SECTION_AMBIENT:
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_TINTCOLOR:
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_OMNI:
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_ITEM_ITEMS:            // items.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            break;

        case SECTION_LOD1:                  // clothColInfo.bin
        case SECTION_FOG:
            for(nEntry = 0; nEntry < 5; nEntry++)
            {
                strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            }
            break;
            
            
        case SECTION_BADGE:                 // Badges.bin
            strFinal += "Name: " + GetString(pData, nPos, dwLength) + strEOL1;
            GetLong(pData, nPos, dwLength, &dwTemp1);
            GetLong(pData, nPos, dwLength, &dwTemp2);
            strTemp.Format("Order=%d Type=%s ", dwTemp1, (dwTemp2 <= 5) ? aszBadgeTypes[dwTemp2] : "Unknown");
            strFinal += strTemp;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "Id: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "Image: " + GetString(pData, nPos, dwLength) + strEOL1;
            for(nEntry = 0; nEntry < 4; nEntry++)
            {
                strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            }
            break;
            
        case SECTION_CAPE_CAPES:            // capes.bin
            strFinal += "C00: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C01: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C02: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C03: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C04: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C05: " + DumpHex(pData+nPos, 16, strIndent + "   ") + strEOL1;
            nPos += 16;
            strFinal += "C06: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C07: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C08: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C09: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C10: ";
            for(nEntry = 0; nEntry < 5; nEntry++)
            {
                strFinal += GetFloat(pData, nPos, dwLength) + " ";
            }
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "C11: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "C12: " + GetString(pData, nPos, dwLength) + strEOL1;
            break;
            
        case SECTION_BODYPART:              // BodyParts.bin
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + ",";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + ",";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_POWER_ITEMS:           // items.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_POWER_POWERS:
            strFinal += "P01: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P02: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P03: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P04: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P05: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P06: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P07: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P08: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P09: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P10: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P11: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P12: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P13: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P14: " + GetStringList(pData, nPos, dwLength, "Code", strEOL1);
            strFinal += "P15: " + GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += "P16: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;

            strFinal += "P17: " + GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + strEOL1;

            strFinal += "P18: " + GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += "P19: " + GetFloat(pData, nPos, dwLength) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;

            strFinal += "P20: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P21: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P22: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P23: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P24: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P25: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P26: " + GetStringList(pData, nPos, dwLength, "Strings", strEOL1);
            strFinal += "P27: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal +=           GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P28: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P29: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P30: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P31: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P32: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P33: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P34: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P35: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P36: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P37: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P38: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P39: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P40: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P41: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += "P42: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += "P43: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P44: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P45: " + GetLong(pData, nPos, dwLength, NULL)  + strEOL1;
            strFinal += "P46: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P47: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P48: " + GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += "P49: " + DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            strFinal += "P50: " + GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += "P51: " + GetLong(pData, nPos, dwLength, NULL) + " ";
            break;
            
		case SECTION_DEVEMOTE:              // emotes.bin
            strFinal += GetString(pData, nPos, dwLength) + " ";
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ") + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_EMOTE:                 // emotes.bin
            strFinal += GetString(pData, nPos, dwLength) + " ";
            strFinal += DumpLongTable(pData, nPos, dwLength, strIndent + "   ");
            break;
            
        case SECTION_SELL:                  // stores.bin
        case SECTION_BUY:                   // stores.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " = ";
            strFinal += GetFloat(pData, nPos, dwLength);
            break;

        case SECTION_MOVE:                  // sequencers.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetString(pData, nPos, dwLength) + strEOL2;
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetStringList(pData, nPos, dwLength, "Actions", strEOL2);
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_TAILORCOSTSET:         // tailorcost.bin
            while(nPos < (nStartPos + dwInnerLength))
            {
                strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            }
            break;

        case SECTION_STANIM:                // tricks.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;

        case SECTION_GROUP:
            break;

        case SECTION_TEXTURE:
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_COLOR:                 // costume.bin, supergroupColors.bin
            for(nEntry = 0; nEntry < 3; nEntry++)
            {
                DWORD color;
                CString strColor;
                GetLong(pData, nPos, dwLength, &color);
                strColor.Format("COLOR(%3d,%3d,%3d) ", (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff);
                strFinal += strColor;
            }
            break;
            
        case SECTION_ORIGIN:                // costume.bin
            strFinal += GetString(pData, nPos, dwLength);
            //
            // There are some files where origin has only one string and others where it is much longer
            //
            if (nPos < (nStartPos + dwInnerLength)) // origins.bin, villain_origin.bin
            {
                strFinal += strEOL1;
                strFinal += GetString(pData, nPos, dwLength) + strEOL1;
                strFinal += GetString(pData, nPos, dwLength) + strEOL1;
                strFinal += GetString(pData, nPos, dwLength) + strEOL1;
                strFinal += GetString(pData, nPos, dwLength);
            }
            break;
            
        case SECTION_BONESET:               // costume.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + " = ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_GEOSET:                // costume.bin
            DWORD dwStringCount;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            for(nEntry = 0; nEntry < 5; nEntry++)
            {
                strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            }
            strFinal += strEOL1;
            
            GetLong(pData, nPos, dwLength, &dwStringCount);
            for (nEntry = 0; nEntry < dwStringCount; nEntry++)
            {
                strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            }
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_ZONE:                  // map.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL);
            break;
            
        case SECTION_MENU:                  // MenuDefs.bin
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;
            
        case SECTION_INFO:                  // costume.bin, supergroupEmblems.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + " ";
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) ;
            break;
            
        case SECTION_GEOM:              // fxinfo.bin
        case SECTION_PART:              // fxinfo.bin
        case SECTION_TRIGGERBITS:
        case SECTION_CAPE_FXINFO:
            for(nEntry = 0; nEntry < dwInnerLength; nEntry++)
            {
                strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            }
            //
            // Because the inner length is wrong, we need to fix it
            //
            dwInnerLength = nPos - nStartPos;
            break;
            
        case SECTION_BONE:              // clothColInfo.bin
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetLong(pData, nPos, dwLength, NULL) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + " ";
            strFinal += GetFloat(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength) + strEOL1;
            strFinal += GetString(pData, nPos, dwLength);
            break;
            
        case SECTION_HIGHERLEVEL:           // combat_mods.bin, combat_mods_villain.bin
        case SECTION_LOWERLEVEL:            // combat_mods.bin, combat_mods_villain.bin
        case SECTION_ATTRIBMAXTABLE:        // classes.bin, villain_classes.bin
        case SECTION_ATTRIBMAXMAXTABLE:     // classes.bin, villain_classes.bin
        case SECTION_STRENGTHMAXTABLE:      // classes.bin, villain_classes.bin
        case SECTION_RESISTANCEMAXTABLE:    // classes.bin, villain_classes.bin
            while(nPos < (nStartPos + dwInnerLength))
            {
                CString strTemp;
                strTemp.Format("---Set %d--- ", nSet++);
                strFinal += strEOL1 + strTemp;
                strFinal += DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            }
            break;
            
        case SECTION_ATTRIBMIN:             // classes.bin, villain_classes.bin
        case SECTION_ATTRIBBASE:            // classes.bin, villain_classes.bin
        case SECTION_STRENGTHMIN:           // classes.bin, villain_classes.bin
        case SECTION_RESISTANCEMIN:         // classes.bin, villain_classes.bin
            nEntry = 0;
            while(nPos < (nStartPos + dwInnerLength))
            {
                CString strTemp;
                strTemp.Format("%3d: ", nEntry++);
                strFinal += strTemp + GetFloat(pData, nPos, dwLength) + strEOL1;
            }
            break;
            
        case SECTION_MODTABLE:              // classes.bin, villain_classes.bin
            strFinal += GetString(pData, nPos, dwLength);
            strFinal += DumpFloatTable(pData, nPos, nStartPos + dwInnerLength, strIndent + "   ");
            break;
            
        default:
#ifdef _DEBUG
            AfxMessageBox(strType, MB_OK);
#endif
            strFinal += "Unknown Type:" + strType;
            break;
        }

        if (nPos < (nStartPos + dwInnerLength))
        {
            strFinal += DumpHex(pData+nPos, max(0, (nStartPos + dwInnerLength) - nPos), strIndent + "   ");
            nPos = nStartPos + dwInnerLength;
            strFinal += "\r\n";
        }

        //
        // Make sure we used up the inner amount
        //
        else if (nPos > (nStartPos + dwInnerLength))
        {
            CString strDebug;
            strDebug.Format("Expected to be at %08lx for %08lx+%08lx but was at %08lx (diff=%08lx)", nStartPos + dwInnerLength, nStartPos, dwInnerLength, nPos, (nStartPos+dwInnerLength)-nPos);
            strFinal += strDebug + "\r\n";
            strDebug = DumpHex(pData+nStartPos, dwInnerLength, strIndent + "   ");
            strFinal += strDebug; 
#ifdef _DEBUG
            nPos = 0x7fffffff;
            AfxMessageBox(strDebug, MB_OK);
#endif
            break;
        }
        else
        {
            strFinal += "\r\n";
        }
        //
        // Now see if we have any children to handle
        //
        dwRemainder = dwTotalLength - dwInnerLength - sizeof(long);
        if (nPos >= m_dwNextIncrement)
        {
            m_pApp->StepProgress(nPos);
            m_dwNextIncrement = nPos + m_dwIncrement;
        }
        if (dwRemainder)
        {
            strFinal += DumpSection(pData, nPos, nPos + dwRemainder, "|   " + strIndent, TRUE);
        }
        if (!bContiguous)
        {
            break;
        }
    }
    return strFinal.GetString();
}

CString CFileViewTypeBin::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    CStringLong strFinal;
    CString strWork;
    DWORD dwMax;
    m_pApp = (CPiggViewerApp *)AfxGetApp();
    m_dwNextIncrement = m_dwIncrement = dwLength/50;
    if (strFileName == "bin/attrib_names.bin")
    {
        m_mapSectionNames["Group"]              = (void *)SECTION_GROUP_ATTRIB_NAMES;
    }
    else if (strFileName == "bin/sequencers.bin")
    {
        m_mapSectionNames["Group"]              = (void *)SECTION_GROUP_SEQUENCERS;
    }
    else if (strFileName == "bin/help.bin")
    {
        m_mapSectionNames["Item"]              = (void *)SECTION_ITEM_HELP;
    }
    else if (strFileName == "bin/items.bin")
    {
        m_mapSectionNames["Item"]              = (void *)SECTION_ITEM_ITEMS;
        m_mapSectionNames["Power"]              = (void *)SECTION_POWER_ITEMS;
    }
    else if (strFileName == "bin/fxinfo.bin")
    {
        m_mapSectionNames["Cape"]              = (void *)SECTION_CAPE_FXINFO;
    }
    else if (strFileName == "bin/capes.bin")
    {
        m_mapSectionNames["Cape"]              = (void *)SECTION_CAPE_CAPES;
    }
    else if (strFileName == "bin/costume.bin")
    {
        m_mapSectionNames["Costume"]            = (void *)SECTION_COSTUME_COSTUME;
    }
    else if (strFileName == "bin/VillainCostume.bin")
    {
        m_mapSectionNames["Costume"]            = (void *)SECTION_COSTUME_VILLAINCOSTUME;
    }
    else if (strFileName == "bin/powers.bin")
    {
        m_mapSectionNames["Power"]              = (void *)SECTION_POWER_POWERS;
    }
    else if (strFileName == "bin/cards.bin")
    {
    	m_mapSectionNames["Card"]               = (void *)SECTION_CARD_CARDS;
    }
    else if (strFileName == "bin/decks.bin")
    {
    	m_mapSectionNames["Card"]               = (void *)SECTION_CARD_DECKS;
    }
    else if (strFileName.Left(3) == "geo")
    {
        m_mapSectionNames["Group"]              = (void *)SECTION_GROUP_GEOBIN;
    }
    else
    {
    	m_mapSectionNames["Card"]               = (void *)SECTION_CARD;
        m_mapSectionNames["Power"]              = (void *)SECTION_POWER;
        m_mapSectionNames["Costume"]            = (void *)SECTION_COSTUME;
        m_mapSectionNames["Cape"]               = (void *)SECTION_CAPE;
        m_mapSectionNames["Item"]               = (void *)SECTION_ITEM;
        m_mapSectionNames["Group"]              = (void *)SECTION_GROUP;
    }


    //
    // It should start out as 'CrypticS'
    //                        07 20 43 d3
    //                        0006  'Parse4'
    //                        0006  'Files1'
    //                        xxxxxxxx  4 Byte length
    DWORD nPos;
    DWORD nBytes;
    DWORD nEntries;
    DWORD nExtra;
    if (memcmp(pData, "CrypticS", 8))
    {
        if (strFileName == "bin/defnames.bin")
        {
            nPos = 0;
            m_pApp->StartProgress(dwLength);
            while(nPos < dwLength)
            {
                const BYTE *pEnd = (const BYTE *)memchr(pData+nPos, 0, dwLength-nPos);
                int nLen;
                if (pEnd == NULL)
                {
                    nLen = dwLength-nPos;
                }
                else
                {
                    nLen = pEnd - (pData + nPos);
                }
                CString strAdd((LPCTSTR)pData+nPos, nLen);
                strFinal += strAdd + "\r\n";
                nPos += nLen + 1;
                if (nPos >= m_dwNextIncrement)
                {
                    m_pApp->StepProgress(nPos);
                    m_dwNextIncrement = nPos + m_dwIncrement;
                }
            }
            m_pApp->EndProgress();
            return strFinal.GetString();
        }
        else
        {
            return DumpHex(pData, dwLength, "");
        }
    }
    if (dwOffset)
    {
        nPos = dwOffset;
        return DumpSection(pData, nPos, dwLength, "", FALSE);
    }
    m_pApp->StartProgress(dwLength);
    nPos = 8;  // skip the initial stuff

    strFinal += "CrypticS Checksum=" + GetLong(pData, nPos, dwLength, NULL) + " ";
    strFinal += GetString(pData, nPos, dwLength) + " ";
    strFinal += GetString(pData, nPos, dwLength) + "\r\n";
    strFinal += GetLong(pData, nPos, dwLength, &nBytes) + " Bytes ";
    strFinal += GetLong(pData, nPos, dwLength, &nEntries) + " Entries\r\n";
    //
    // Now output the bytes of the section
    //
    dwMax = nPos + nBytes;
    for(DWORD nEntry = 0; nEntry < nEntries; nEntry++)
    {
        CString strEntry = GetString(pData, nPos, dwMax);
        CString strLong = GetLong(pData, nPos, dwMax, NULL);
        strFinal += strLong + " -- " + strEntry + "\r\n";
    }
    strWork = GetLong(pData, nPos, dwMax, &nExtra);
    nExtra &= 0xffff;
    if (nExtra)
    {
        DWORD dwEnd;
        DWORD dwTest;
        dwEnd = nPos + nExtra;
        
        dwTest = ((long *)(pData+nPos))[0];
        if ((dwTest < 300) && (strFileName.Left(12) != "geobin/maps/"))
        {
            while(nPos < dwEnd)
            {
                if ((strFileName == "bin/experience.bin") ||
                    (strFileName == "bin/schedules.bin"))
                {
                    strFinal += DumpLongTable(pData, nPos, dwEnd, "");
                }
                else
                {
                    strFinal += DumpFloatTable(pData, nPos, dwEnd, "");
                }
            }
        }
        else
        {
            while(nPos < dwEnd)
            {
                strFinal += GetString(pData, nPos, dwEnd)+"\r\n";
            }
        }

    }
    //
    // Dump out strings to the end
    //
//    if (dwLength > 2000000) { dwLength = 2000000; }
    strFinal += DumpSection(pData, nPos, dwLength, "", TRUE);
    
    m_pApp->EndProgress();
    return strFinal.GetString();
}

void SkipString(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    CString strFinal;
    if ((nPos+1) <= dwLength)
    {
        short sLength = *(short *)(pData + nPos);
        nPos += sizeof(short);
        nPos += sLength;
        DumpRoundUp(pData, nPos, dwLength);
    }
}

DWORD SkipLong(const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    DWORD dwTemp = 0;
    CString strFinal;
    if ((nPos + sizeof(long)) <= dwLength)
    {
        dwTemp = *(long *)(pData+nPos);
        nPos += sizeof(long);
    }
    return dwTemp;
}

void CFileViewTypeBin::IndexSection(CTreeCtrl &treeCtrl, HTREEITEM hParent, const BYTE *pData, DWORD &nPos, DWORD dwLength)
{
    while(nPos < dwLength)
    {
        DWORD dwBasePos;
        DWORD nStartPos;
        DWORD dwTotalLength;
        DWORD dwInnerLength;
        DWORD dwRemainder;
        CString strTotal;
        CString strInner;
        CString strDef;
        
        dwBasePos = nPos;
        CString strType = GetString(pData, nPos, dwLength);
        dwTotalLength = SkipLong(pData, nPos, dwLength);
        dwInnerLength = SkipLong(pData, nPos, dwLength);
        nStartPos = nPos;
        HTREEITEM hItemNew = treeCtrl.InsertItem(strType, hParent, TVI_LAST);
        if (hItemNew != NULL)
        {
            treeCtrl.SetItemData(hItemNew, dwBasePos);
        }
 
        if ((strType == "Geom") ||
            (strType == "Part"))
        {
            for(DWORD i = 0; i < dwInnerLength; i++)
            {
                SkipString(pData, nPos, dwLength);
            }
            //
            // Because the inner length is wrong, we need to fix it
            //
            dwInnerLength = nPos - nStartPos;
        }

        nPos = nStartPos + dwInnerLength;
        dwRemainder = dwTotalLength - dwInnerLength - sizeof(long);

        if (nPos >= m_dwNextIncrement)
        {
            m_pApp->StepProgress(nPos);
            m_dwNextIncrement = nPos + m_dwIncrement;
        }

        //
        // Now see if we have any children to handle
        //
        if (dwRemainder)
        {
            IndexSection(treeCtrl, hItemNew, pData, nPos, nPos + dwRemainder);
        }
    }
}


void CFileViewTypeBin::UpdateIndexList(CTreeCtrl &treeCtrl, const BYTE *pData, DWORD dwLength)
{
    treeCtrl.SetRedraw(FALSE);
    treeCtrl.SelectItem(NULL);
    treeCtrl.DeleteAllItems();
    m_pApp = (CPiggViewerApp *)AfxGetApp();
    m_dwNextIncrement = m_dwIncrement = dwLength/50;

    HTREEITEM hItemNew = treeCtrl.InsertItem("Contents", TVI_ROOT, TVI_LAST);
    if (hItemNew != NULL)
    {
        treeCtrl.SetItemData(hItemNew, 0);
    }
    //
    // Now go through and add all the sub items
    //
    //
    // It should start out as 'CrypticS'
    //                        07 20 43 d3
    //                        0006  'Parse4'
    //                        0006  'Files1'
    //                        xxxxxxxx  4 Byte length
    DWORD nPos;
    int nBytes;
    int nEntries;
    DWORD nExtra;
    DWORD dwMax;
    if (memcmp(pData, "CrypticS", 8))
    {
        treeCtrl.SetRedraw(TRUE);
        return;
    }
    m_pApp->StartProgress(dwLength);
    nPos = 8+4; // skip the initial stuff
    SkipString(pData, nPos, dwLength);
    SkipString(pData, nPos, dwLength);
    nBytes = SkipLong(pData, nPos, dwLength);
    nEntries = SkipLong(pData, nPos, dwLength);
    //
    // Now output the bytes of the section
    //
    dwMax = nPos + nBytes;
    for(int nEntry = 0; nEntry < nEntries; nEntry++)
    {
        SkipString(pData, nPos, dwMax);
        SkipLong(pData, nPos, dwMax);
    }
    nExtra = SkipLong(pData, nPos, dwMax);
    nPos += nExtra;
    //
    // Skip the initial float tables
    //
    IndexSection(treeCtrl, TVI_ROOT, pData, nPos, dwLength);
    m_pApp->EndProgress();
    treeCtrl.SetRedraw(TRUE);
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeTTF
//////////////////////////////////////////////////////////////////////

CFileViewTypeTTF::CFileViewTypeTTF()
{

}

CFileViewTypeTTF::~CFileViewTypeTTF()
{

}

CRuntimeClass * CFileViewTypeTTF::GetViewClass()
{
    return RUNTIME_CLASS(CHexView);
}

CString CFileViewTypeTTF::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "TTF File Type";
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeGeo
//////////////////////////////////////////////////////////////////////

CFileViewTypeGeo::CFileViewTypeGeo()
{

}

CFileViewTypeGeo::~CFileViewTypeGeo()
{

}

CRuntimeClass * CFileViewTypeGeo::GetViewClass()
{
    return RUNTIME_CLASS(CHexView);
}

CString CFileViewTypeGeo::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "Geo File Type";
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeAnim
//////////////////////////////////////////////////////////////////////

CFileViewTypeAnim::CFileViewTypeAnim()
{

}

CFileViewTypeAnim::~CFileViewTypeAnim()
{

}

CRuntimeClass * CFileViewTypeAnim::GetViewClass()
{
    return RUNTIME_CLASS(CHexView);
}

CString CFileViewTypeAnim::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "Animary File Type";
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeTxt
//////////////////////////////////////////////////////////////////////

CFileViewTypeTxt::CFileViewTypeTxt()
{

}

CFileViewTypeTxt::~CFileViewTypeTxt()
{

}

CRuntimeClass * CFileViewTypeTxt::GetViewClass()
{
    return RUNTIME_CLASS(CTextView);
}

CString CFileViewTypeTxt::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return CString((LPCTSTR)pData, dwLength);
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeOgg
//////////////////////////////////////////////////////////////////////

CFileViewTypeOgg::CFileViewTypeOgg()
{

}

CFileViewTypeOgg::~CFileViewTypeOgg()
{

}

CRuntimeClass * CFileViewTypeOgg::GetViewClass()
{
    return RUNTIME_CLASS(CHexView);
}

CString CFileViewTypeOgg::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "Oggary File Type";
}
//////////////////////////////////////////////////////////////////////
// CFileViewTypeTexture
//////////////////////////////////////////////////////////////////////

CFileViewTypeTexture::CFileViewTypeTexture()
{

}

CFileViewTypeTexture::~CFileViewTypeTexture()
{

}

CRuntimeClass * CFileViewTypeTexture::GetViewClass()
{
    return RUNTIME_CLASS(CTextureView);
}

CString CFileViewTypeTexture::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return "Texture File Type";
}

//////////////////////////////////////////////////////////////////////
// CFileViewTypeCode
//////////////////////////////////////////////////////////////////////

CFileViewTypeCode::CFileViewTypeCode()
{

}

CFileViewTypeCode::~CFileViewTypeCode()
{

}

CRuntimeClass * CFileViewTypeCode::GetViewClass()
{
    return RUNTIME_CLASS(CTextView);
}

CString CFileViewTypeCode::ConvertFileContents(const BYTE *pData, DWORD dwLength, DWORD dwOffset, CString strFileName)
{
    return CString((LPCTSTR)pData, dwLength);
}
