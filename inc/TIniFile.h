/*****************************************************************************/
/* TIniFile.h                        Copyright (c) Ladislav Zezula 2007-2022 */
/*---------------------------------------------------------------------------*/
/* Description:                                                              */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 05.04.07  1.00  Lad  Created                                              */
/* 24.10.22  1.00  Lad  Added support for section-less config files (.vmx)   */
/*****************************************************************************/

#ifndef __TINIFILE_H__
#define __TINIFILE_H__

//-----------------------------------------------------------------------------
// Defines

#define TPI_NONE                0
#define TPI_BOOL                1
#define TPI_INT                 2
#define TPI_STRING              3
#define TPI_PASSWORD            4
#define TPI_BINARY              5
#define TPI_MULTISZ             6

#define INI_FLAG_QUOTED_VALUES  0x00000001  // Variable values are quoted (see a .VMX file)

//-----------------------------------------------------------------------------
// Structures and classes

enum TIniEncoding
{
    EncodingAnsi,
    EncodingUnicode,
    EncodingUnicodeBE,
    EncodingUTF8,
    EncodingMax
};

struct TIniVariable
{
    LIST_ENTRY Entry;                       // Link to other variables
    LPTSTR szVariableName;
    LPTSTR szValue;
    size_t nLength;                         // Length of the allocated buffer in szValue, in TCHARs
};

struct TIniSection
{
    LIST_ENTRY Entry;                       // Link to other sections
    LIST_ENTRY VarLinks;                    // Variables within this section
    LPTSTR szSectionName;
};

class TIniFile
{
    public:

    TIniFile(LPCTSTR szIniFileName = NULL);
    ~TIniFile();

    DWORD Load(LPBYTE pbData, size_t cbData, DWORD dwFlags = 0);
    DWORD Load(DWORD dwFlags = 0);
    DWORD Save();

    // The "GET" interface
    // Strings and multistrings are returned as buffer allocated by "new", the caller must free them
    int     GetIntValue    (LPCTSTR szSection, LPCTSTR szVarName, int nDefValue = 0) const;
    BOOL    GetBoolValue   (LPCTSTR szSection, LPCTSTR szVarName, BOOL bDefValue = FALSE) const;
    LPCTSTR GetStringValue (LPCTSTR szSection, LPCTSTR szVarName, LPCTSTR szDefValue = NULL) const;
    LPTSTR  GetMultiSZValue(LPCTSTR szSection, LPCTSTR szVarName, LPCTSTR szDefValue = NULL) const;

    // The "SET" interface
    // Strings and multistrings are returned as buffer allocated by "new", the caller must free them
    void SetBoolValue   (LPCTSTR szSection, LPCTSTR szVarName, BOOL bValue);
    void SetIntValue    (LPCTSTR szSection, LPCTSTR szVarName, int nValue);
    void SetStringValue (LPCTSTR szSection, LPCTSTR szVarName, LPCTSTR szValue);
    void SetMultiSZValue(LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue);

    // The "READ" interface
    void ReadBoolValue   (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue, LPCVOID pDefValue) const;
    void ReadIntValue    (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue, LPCVOID pDefValue) const;
    void ReadStringValue (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue, LPCVOID pDefValue, size_t nMaxSize) const;
    void ReadBinaryValue (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue, LPCVOID pDefValue, size_t nSize) const;

    // The "WRITE" interface
    void WriteBoolValue   (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue);
    void WriteIntValue    (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue);
    void WriteStringValue (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue);
    void WriteMultiSZValue(LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue);
    void WriteBinaryValue (LPCTSTR szSection, LPCTSTR szVarName, LPVOID pValue, size_t nSize);

    // Removing sections and variables
    bool RemoveVariable(LPCTSTR szSection, LPCTSTR szVarName);
    bool RemoveSection(LPCTSTR szSection);

    // Enumerating sections. NOT THREAD SAFE !!!
    TIniSection * FindSectionByName(LPCTSTR szSection) const;
    bool GetNextVariable(TIniSection * pSection, TIniVariable ** ppVar);
    bool GetNextSection(TIniSection ** ppSection);

    // Get the name of the INI file
    LPCTSTR GetFileName();

    protected:

    TIniSection * InsertNewSection(LPCTSTR szName);
    TIniVariable * FindVariableByName(TIniSection * pSection, LPCTSTR szVarName) const;
    TIniVariable * InsertNewVariable(TIniSection * pSection, LPCTSTR szName);
    void SetVariableValue(TIniVariable * pVar, LPCTSTR szValue);

    int ParseIniFile(LPTSTR szFile, LPTSTR szFileEnd, DWORD dwFlags);

    LPTSTR GetDefaultIniFileName(LPCTSTR szPlainName);
    LPTSTR FindEndOfWord(LPTSTR szFile, LPTSTR szFileEnd, TCHAR chExtraChar, LPTSTR * pszDataAfter);
    LPTSTR FindEndOfLine(LPTSTR szFile, LPTSTR szFileEnd, LPTSTR * pszDataAfter);
    LPTSTR TrimVariableValue(LPTSTR szString, DWORD dwFlags);
//  DWORD CalcNameHash(LPCTSTR szName);

    bool StringToBool(LPCTSTR szStr, PBOOL pbBool) const;
    bool StringToInt(LPCTSTR szStr, int * piInt) const;
    void SwapBuffer_UINT16(LPBYTE pbTrgBuffer, LPBYTE pbSrcBuffer, size_t cbBuffer);
    bool IsFullPathName(LPCTSTR szPath);
    LPTSTR NewName(LPCTSTR szString);

    LIST_ENTRY m_SectionLinks;
    LPTSTR m_szIniFileName;
    TIniEncoding m_Encoding;
};

#endif