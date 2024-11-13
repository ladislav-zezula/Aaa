/*****************************************************************************/
/* TStringConvert.h                       Copyright (c) Ladislav Zezula 2024 */
/*---------------------------------------------------------------------------*/
/* Easy-to-use string conversions Wide <-> Ansi and Wide <-> UTF8            */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 04.10.24  1.00  Lad  Created                                              */
/*****************************************************************************/

//-----------------------------------------------------------------------------
// Class template for conversions UNICODE <--> UTF-8

template <typename SRCCHAR, typename TRGCHAR, UINT CodePage>
class TConvertString
{
    public:

    TConvertString()
    {
        m_szStr = NULL;
        m_nLen = 0;
    }

    TConvertString(const SRCCHAR * szSrcStr, const SRCCHAR * szSrcEnd = NULL)
    {
        // Initially, set to NULL string
        m_szStr = NULL;
        m_nLen = 0;

        // Set the string
        SetString(szSrcStr, szSrcEnd);
    }

    ~TConvertString()
    {
        if(m_szStr != NULL && m_szStr != m_StaticBuffer)
            delete [] m_szStr;
        m_szStr = NULL;
        m_nLen = 0;
    }

    TRGCHAR * SetString(const SRCCHAR * szSrcStr, const SRCCHAR * szSrcEnd = NULL)
    {
        if(szSrcStr != NULL)
        {
            // Set to the pointer to the internal buffer
            m_szStr = m_StaticBuffer;
            m_StaticBuffer[0] = 0;

            // Retrieve the length of the UTF-8 string
            if((m_nLen = ConvertString(NULL, 0, szSrcStr, szSrcEnd)) != 0)
            {
                // Allocate buffer if too long
                if((m_nLen + 1) > _countof(m_StaticBuffer))
                {
                    if((m_szStr = new TRGCHAR[m_nLen + 1]) == NULL)
                    {
                        assert(false);
                        return NULL;
                    }
                }

                // Convert the string
                ConvertString(m_szStr, m_nLen + 1, szSrcStr, szSrcEnd);
            }
        }
        return m_szStr;
    }

    size_t ConvertString(LPSTR szTrgStr, size_t ccTrgStr, LPCWSTR szSrcStr, LPCWSTR szSrcEnd)
    {
        size_t ccSrcStr = (szSrcEnd > szSrcStr) ? (szSrcEnd - szSrcStr) : INVALID_SIZE_T;
        size_t nResult;

        // Perform the conversion
        nResult = WideCharToMultiByte(CodePage, 0, szSrcStr, (int)(ccSrcStr), szTrgStr, (int)(ccTrgStr), NULL, NULL);

        // Terminate with zero, if needed
        if(szSrcEnd && szSrcEnd[0] && szTrgStr)
            szTrgStr[nResult] = 0;
        return nResult;
    }

    size_t ConvertString(LPWSTR szTrgStr, size_t ccTrgStr, LPCSTR szSrcStr, LPCSTR szSrcEnd)
    {
        // Make sure that we know the length of the target
        size_t ccSrcStr = (szSrcEnd > szSrcStr) ? (szSrcEnd - szSrcStr) : INVALID_SIZE_T;

        int nResult = MultiByteToWideChar(CodePage, 0, szSrcStr, (int)ccSrcStr, szTrgStr, (int)(ccTrgStr));

        if(szTrgStr != NULL)
            szTrgStr[nResult] = 0;
        return nResult;
    }

    TRGCHAR * Buffer()
    {
        return m_szStr;
    }

    size_t Length()
    {
        return m_nLen;
    }

    operator const TRGCHAR *()
    {
        return m_szStr;
    }

    protected:

    bool ConversionFailed(int nResult)
    {
        return (nResult == 0) && (GetLastError() == ERROR_NO_UNICODE_TRANSLATION);
    }

    TRGCHAR * m_szStr;
    size_t m_nLen;
    TRGCHAR m_StaticBuffer[0x80];
};

//-----------------------------------------------------------------------------
// Classes for conversions Ansi <-> UTF16 and UTF8 <-> UTF16

typedef TConvertString<char, WCHAR, CP_ACP>  TAnsiToWide;
typedef TConvertString<char, WCHAR, CP_UTF8> TUTF8ToWide;
typedef TConvertString<WCHAR, char, CP_ACP>  TWideToAnsi;
typedef TConvertString<WCHAR, char, CP_UTF8> TWideToUTF8;
