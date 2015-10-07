/*****************************************************************************/
/* Utils.h                                Copyright (c) Ladislav Zezula 2004 */
/*---------------------------------------------------------------------------*/
/* Main header file for the Utils library. Include in your project to have   */
/* access to all utility functions                                           */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 29.05.04  1.00  Lad  The first version of Utils.h                         */
/*****************************************************************************/

#ifndef __UTILS_H__
#define __UTILS_H__

// In MSVC 8.0, there are some functions declared as deprecated.
#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>

//-----------------------------------------------------------------------------
// Use the appropriate library
//
// The library type is encoded in the library name as the following UtilsXYZ.lib:
// 
//  X - D for Debug version, R for Release version
//  Y - A for ANSI version, U for Unicode version
//  Z - S for static-linked CRT library, D for multithreaded DLL CRT library
//

#if defined(_MSC_VER) && !defined(__UTILS_SELF__)

  #ifdef _DEBUG                                 // DEBUG VERSIONS
    #ifndef _UNICODE                            
      #ifdef _DLL
        #pragma comment(lib, "UtilsDAD.lib")    // Debug Ansi CRT-DLL version
      #else        
        #pragma comment(lib, "UtilsDAS.lib")    // Debug Ansi CRT-LIB version
      #endif
    #else
      #ifdef _DLL
        #pragma comment(lib, "UtilsDUD.lib")    // Debug Unicode CRT-DLL version
      #else        
        #pragma comment(lib, "UtilsDUS.lib")    // Debug Unicode CRT-LIB version
      #endif
    #endif
  #else                                         // RELEASE VERSIONS
    #ifndef _UNICODE
      #ifdef _DLL
        #pragma comment(lib, "UtilsRAD.lib")    // Release Ansi CRT-DLL version
      #else        
        #pragma comment(lib, "UtilsRAS.lib")    // Release Ansi CRT-LIB version
      #endif
    #else
      #ifdef _DLL
        #pragma comment(lib, "UtilsRUD.lib")    // Release Unicode CRT-DLL version
      #else                                                                         
        #pragma comment(lib, "UtilsRUS.lib")    // Release Unicode CRT-LIB version
      #endif
    #endif
  #endif

#endif

//-----------------------------------------------------------------------------
// Manifest
//
// Set the linker and manifest options to the following:
// - Linker\Manifest File\Generate manifest:                   Yes
// - Linker\Manifest File\Additional Manifest Dependencies:    <empty>
// - Linker\Manifest File\Allow Isolation:                     Yes
// - Manifest Tool\Input and Output\Additional manifest files: <empty>
// - Manifest Tool\Input and Output\Input Resource Manifests:  <empty>
// - Manifest Tool\Input and Output\Embed Manifest:            Yes
//

#ifndef _NO_UTILS_MANIFEST
#if _MSC_VER >= 1400
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif  // _NO_UTILS_MANIFEST

//-----------------------------------------------------------------------------
// Strings

#define IDS_ERROR                  1            // "Error"
#define IDS_CONFIRM                2            // "Confirmation"
#define IDS_QUESTION               3            // "Question"
#define IDS_INFO                   4            // "Information"    
#define IDS_WARNING                5            // "Warning"
#define IDS_REQUEST                6            // "Request"
#define IDS_RETRYCANCEL            7            // Dummy string for "Retry/Cancel" MessageBox
#define IDS_BTN_OK                 8            // "&Ok"
#define IDS_BTN_CANCEL             9            // "&Cancel"
#define IDS_BTN_ABORT             10            // "&Abort"
#define IDS_BTN_RETRY             11            // "&Retry"
#define IDS_BTN_IGNORE            12            // "&Ignore"
#define IDS_BTN_YES               13            // "&Yes"
#define IDS_BTN_NO                14            // "&No"
#define IDS_BTN_CLOSE             15            // "&Close"
#define IDS_BTN_HELP              16            // "&Help"
#define IDS_BTN_TRYAGAIN          17            // "&Try Again"
#define IDS_BTN_CONTINUE          18            // "Continue"
#define IDS_BTN_TIMEOUT           19            // "Timeout"
#define IDS_BTN_YESALL            20            // "Yes All"
#define IDS_BTN_NOALL             21            // "No All"

//-----------------------------------------------------------------------------
// Special buttons for "Yes/Yes All/No/Cancel" message boxes

#define IDYESTOALL       (IDCONTINUE+2)
#define IDNOTOALL        (IDCONTINUE+3)

#define LISTVIEW_LAST_ITEM   0x7FFFFFFF         // The highest item ID

//-----------------------------------------------------------------------------
// Structure sizes for older Windows

// Windows 2000 or newer doesn't accept sizeof(MENUITEMINFO) greater than MENUITEMINFO_V4_SIZE
#define MENUITEMINFO_V4_SIZE  FIELD_OFFSET(MENUITEMINFO, hbmpItem)

//-----------------------------------------------------------------------------
// Macros

//
// Use with #pragma TODO("This needs to be fixed")
//

#ifndef TODO
#define chSTR2(x) #x
#define chSTR(x)  chSTR2(x)
#define TODO(todo)  message(__FILE__ "(" chSTR(__LINE__) ") : TODO: " #todo)
#endif // TODO

//
// Macro for getting max. number of characters in a buffer
// Removed old "_tsize" macro for being confusing
//

#ifndef _maxchars
#define _maxchars(buff)  ((sizeof(buff) / sizeof(buff[0])) - 1)
#define _maxchars_remaining(buff, ptr)  (_maxchars(buff) - (ptr - buff) - 1)
#endif  // _maxchars

//
// Macro for setting pointer window data, to compensate the improper declaration
// of SetWindowLongPtr on 32-bit targets in the SDK
// SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)ptr) should no longer give any warnings on /W4
//

#if !defined(WIN64) && !defined(_WIN64)
#undef SetWindowLongPtrA
#undef SetWindowLongPtrW
#define SetWindowLongPtrA(hwnd, nIndex, value)  SetWindowLongA(hwnd, nIndex, (LONG)(LONG_PTR)value);
#define SetWindowLongPtrW(hwnd, nIndex, value)  SetWindowLongW(hwnd, nIndex, (LONG)(LONG_PTR)value);
#endif

//
// Macro for self-crash dump creation
//

#define CRASHDUMP_TRY        __try
#define CRASHDUMP_EXCEPT     __except(WriteDumpFile(NULL, GetExceptionInformation()))

//
// Macro for aligning value to nearest-upper size of 'a'
//

#ifndef ALIGN_TO_SIZE
#define ALIGN_TO_SIZE(x, a)   (((x) + (a)-1) & ~((a)-1))
#endif

//
// Macros for handling LIST_ENTRY-based lists
//

#if !defined(_WDMDDK_) && !defined(_LIST_ENTRY_MACROS_DEFINED_)
#define _LIST_ENTRY_MACROS_DEFINED_

BOOLEAN
FORCEINLINE
IsListEmpty(
    IN const LIST_ENTRY * ListHead
    )
{
    return (BOOLEAN)(ListHead->Flink == ListHead);
}

FORCEINLINE
VOID
InitializeListHead(
    IN PLIST_ENTRY ListHead
    )
{
    ListHead->Flink = ListHead->Blink = ListHead;
}

FORCEINLINE
VOID
InsertHeadList(
    IN PLIST_ENTRY ListHead,
    IN PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Flink;

    Flink = ListHead->Flink;
    Entry->Flink = Flink;
    Entry->Blink = ListHead;
    Flink->Blink = Entry;
    ListHead->Flink = Entry;
}

FORCEINLINE
VOID
InsertTailList(
    IN PLIST_ENTRY ListHead,
    IN PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Blink;

    Blink = ListHead->Blink;
    Entry->Flink = ListHead;
    Entry->Blink = Blink;
    Blink->Flink = Entry;
    ListHead->Blink = Entry;
}

FORCEINLINE
BOOLEAN
RemoveEntryList(
    IN PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Blink;
    PLIST_ENTRY Flink;

    Flink = Entry->Flink;
    Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;
    return (BOOLEAN)(Flink == Blink);
}
#endif  // #if !defined(_WDMDDK_) && !defined(_LIST_ENTRY_MACROS_DEFINED_)

FORCEINLINE bool IsDotDirectoryName(LPCTSTR szFileName)
{
    if(szFileName[0] == _T('.'))
    {
        if(szFileName[1] == _T('.'))
            return (szFileName[2] == 0);
        return (szFileName[1] == 0);
    }
    return false;
}

//-----------------------------------------------------------------------------
// Structures

struct TListViewColumns
{
    UINT nIDTitle;                      // The title of the list view (string resource ID)
    int  nWidth;                        // The width of the list view. -1 means that the item's
                                        // width is up to the listview width 
};

//-----------------------------------------------------------------------------
// Global variables
                                                         
// Must be defined by the application. It is the instance of the
// module where to load resources from
extern HINSTANCE g_hInst;

//-----------------------------------------------------------------------------
// Utility function prototypes

// Adds/removes backslash to/from end of path name
LPTSTR AddBackslash(LPTSTR szPathName);
LPTSTR RemoveBackslash(LPTSTR szPathName);

// String allocations
LPTSTR NewStr(LPCSTR szString, size_t nCharsToReserve = 0);
LPTSTR NewStr(LPCWSTR szString, size_t nCharsToReserve = 0);
LPTSTR NewStr(LPCSTR szStringBegin, LPCSTR szStringEnd);
LPTSTR NewStr(LPCWSTR szStringBegin, LPCWSTR szStringEnd);
LPSTR  NewStrAnsi(LPCSTR szString, size_t nCharsToReserve = 0);
LPSTR  NewStrAnsi(LPCWSTR szString, size_t nCharsToReserve = 0);

// Creates the full path from a directory and file name
// Handles directory names with or without ending backslashes
// Returns the pointer to plain file name
// The caller needs to free the path using delete []
LPTSTR CreateFullPath(LPCTSTR szDirectory, LPCTSTR szSubDir, LPCTSTR szPlainName);

// Adds a new string to the existing one. The existing must have been
// allocated by new, the result must be freed using "FreeAppendedString", when no longer needed.
// The "szString" may be NULL.
LPTSTR AppendString(LPTSTR szString, LPCTSTR szAdd, LPCTSTR szSeparator = _T("\r\n"));
void FreeAppendedString(LPTSTR szString);

// Browses for a file. The initial dir and/or the file may be
// entered by a string or by a control ID.
//BOOL BrowseForFile(HWND hParent, LPTSTR szDir, LPTSTR szFile, UINT nIDTitle, UINT nIDFilters);
void InitOpenFileName(LPOPENFILENAME pOFN);
BOOL GetOpenFileNameRc(HWND hWndParent, LPOPENFILENAME pOFN);
BOOL GetSaveFileNameRc(HWND hWndParent, LPOPENFILENAME pOFN);

// Browses for a folder. The folder is stored into the "nIDEdit"
// control
BOOL BrowseForDirectory(HWND hDlg, LPTSTR szDir, UINT nIDTitle);

// Centers a window by its parent or screen
void CenterWindow(HWND hWnd);
void CenterWindowToParent(HWND hWnd);

// This function compares a string with a wildcard search string.
// returns TRUE, when the string matches with the wildcard.
BOOL CompareStringWildCard(LPCTSTR szString, LPCTSTR szWildCard);

BOOL CompareWindowsTexts(HWND hDlg, UINT nID1, UINT nID2, UINT nIDMsg);

// Creates a HDROP structure for the file.
// The caller must free it using GlobalFree.
HDROP CreateDropForFile(LPCTSTR szFileName);
HDROP CreateDropForDirectory(LPCTSTR szDirName, PLARGE_INTEGER pFileSize);

// Multistring support
LPTSTR AddStringToMultiString(LPTSTR szMultiString, LPCTSTR szString);
size_t GetMultiStringLength(LPCTSTR szMultiString);
DWORD GetMultiStringCount(LPCTSTR szMultiString);
void FreeMultiString(LPTSTR szMultiString);

// In debug version, shows a formatted text on debug output
#if defined(_DEBUG) || defined(DBG)
int DebugPrint(LPCTSTR szFmt, ...);
#else
__inline int DebugPrint(LPCTSTR, ...) { return 0; }
#endif

// Shows the message box dialog with "Don't display again" text
INT_PTR DontDisplayAgainDialog(HWND hParent, UINT nIDTemplate, int * piLastAnswer);

// Converts a GUID to registry string format (i.e. {XXXXXXXX-XXXX-...})
int GuidToString(GUID * pGuid, LPTSTR szBuffer);

// Enables/disables a group of dialog items by their ID.
// The ID list must end with 0.
int ShowDlgItems(HWND hDlg, int nCmdShow, ...);
int EnableDlgItems(HWND hDlg, BOOL bEnable, ...);

// Enables a privilege to the current process
int EnablePrivilege(LPCTSTR szPrivilegeName);

// Gets the rectangle of a dialog's template
BOOL GetDialogRect(HWND hParent, UINT nIDDlgTemplate, RECT & rect);

// Get the title of the page from the dialog template
int GetDialogTitleFromTemplate(HINSTANCE hInst, LPCTSTR szDlgTemplate, LPTSTR szTitle);

// Retrieves the error text. The caller must free the text using
// delete [] szText;
LPTSTR GetErrorText(int nError);

// Fills the module version
int GetModuleVersion(LPCTSTR szModuleName, LARGE_INTEGER * pVersion);
int GetModuleVersion(HMODULE hModule, LARGE_INTEGER * pVersion);

// Returns a localized string for a few IDS_XXX strings.
LPCTSTR GetString(UINT_PTR nIDString);
bool SetLocalizedStrings(UINT nIDString, UINT nIDLocalizedString, ...);

// Returns the language of current Windows installation
// (Not the value set by the user in Regional Settings)
USHORT GetSystemLanguage();

// Fills the buffer by the current user's specific directory name
int GetShellFolderPath(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath);
   
// Gets the widths of the window borders (non-client area)
// and stores them to the "pRect" parameter
void GetWindowBorders(HWND hWnd, LPRECT pRect);

// Retrieves the Windows version (WinXP = 0x0501, Win7 = 0x0601)
DWORD GetWindowsVersion();

// Initializes the dialog controls, like combo boxes and list boxes.
int InitDialogControls(HWND hDlg, LPCTSTR lpszResourceName);

// Returns true if the user runs as administrator (or elevated in Vista)
BOOL IsAdministrator();

// REMOVED:
// This function decodes the WM_COMMAND message. If another message, this function
// returns FALSE. If it is a WM_COMMAND, it decodes the motification code, and control ID
// (Control ID is 0 for a menu and 1 for an accelerator).
//BOOL IsCommandMessage(UINT uMsg, WPARAM wParam, UINT & nNotify, UINT_PTR & nIDCtrl);
#define WMC_NOTIFY(wParam)  HIWORD(wParam)
#define WMC_CTRLID(wParam)  LOWORD(wParam)

// Checks of the directory is accessible at least for listin files
BOOL IsDirectoryAccessible(const LPTSTR szDirectory);

// Verifies the presence of SAG.sys in memory.
BOOL IsSagInMemoryAndEnabled();

// Shows a message box using resource strings
int MessageBoxRc(HWND hParent, UINT_PTR nIDCaption, UINT_PTR nIDText, ...);

// Shows a message box with appended error code text
// "Failed to open the file %s\nAccess denied"
int MessageBoxError(HWND hParent, UINT_PTR nIDText, int nError = ERROR_SUCCESS, ...);

// Shows a message box that also includes check box
int MessageBoxWithCheckBox(
    HWND hWndParent,
    LPCTSTR szText,
    LPCTSTR szTitle,
    LPCTSTR szCheckText,
    bool * pbCheckValue,
    UINT uType);

// Shows a message box that also includes check box.
// This one uses string resource IDs rather than texts.
int MessageBoxWithCheckBox(
    HWND hWndParent,
    UINT nIDText,
    UINT nIDTitle,
    UINT nIDCheckText,
    bool * pbCheckValue,
    UINT uType);

// Shows a question message box with "Yes - Yes All - No - Cancel" buttons
int MessageBoxYANC(
    HWND hWndParent,
    UINT_PTR nIDTitle,
    UINT_PTR nIDTextFmt,
    ...);

// Sets an icon to the dialog
void SetDialogIcon(HWND hDlg, UINT nIDIcon);

// Searches all available processes by given name.
// If the name is found, the function returns the process ID.
#define INVALID_PROCESS_ID ((DWORD)-1)
DWORD FindProcessByName(LPCTSTR szExeName);

// Ensures that the path exists.
int ForcePathExist(LPCTSTR szPathName, BOOL bIsDirectory = FALSE);

// Returns the domain name for currently logged on user
int GetDomainName(LPTSTR szText, LPDWORD pdwSize);

// Retrieves the pointer to plain name and extension
LPSTR GetPlainName(LPCSTR szFileName);
LPWSTR GetPlainName(LPCWSTR szFileName);
LPSTR GetFileExtension(LPCSTR szFileName);
LPWSTR GetFileExtension(LPCWSTR szFileName);

// RadioButton functions
DWORD GetRadioValue(HWND hDlg, UINT nIDFirst);
void  SetRadioValue(HWND hDlg, UINT nIDFirst, UINT nValue);

// CheckBox functions
DWORD GetCheckBoxMask(HWND hDlg, UINT nIDFirst);
void  SetCheckBoxMask(HWND hDlg, UINT nIDFirst, DWORD dwMask, DWORD dwEnabled);

// Edit functions
BOOL SetEditCueBanner(HWND hEdit, LPCTSTR szText);
BOOL SetEditCueBanner(HWND hEdit, UINT nIDText);
BOOL SetEditCueBanner(HWND hDlg, UINT nIDEdit, LPCTSTR szText);
BOOL SetEditCueBanner(HWND hDlg, UINT nIDEdit, UINT nIDText);

// URL cursor functions
HCURSOR CreateUrlPointCursor();
HCURSOR CreateUrlWaitCursor();

// URL Buttons functions
BOOL ClickURLButton(HWND hButton);
BOOL DrawURLButton(HWND hDlg, LPDRAWITEMSTRUCT dis, BOOL bCentered = FALSE);
int  InitURLButton(HWND hDlg, UINT nIDCtrl, BOOL bBigFont);
int  InitURLButtons(HWND hDlg);
BOOL IsURLButton(HWND hWnd);

// Functions related to ListView columns and items
int    ListView_CreateColumns(HWND hList, TListViewColumns * pColumns);
void   ListView_ResizeColumns(HWND hList, TListViewColumns * pColumns, int nMinColumnWidth = 0);
int    InsertLVItem(HWND hList, int nIcon, LPCTSTR szText, LPARAM lParam);
int    ListView_SetSubItem(HWND hList, int nItem, int nSubItem, LPCTSTR szText);
LPARAM ListView_GetItemParam(HWND hList, int nItem);
BOOL   ListView_SetItemParam(HWND hList, int nItem, LPARAM lParam);

// TabControl support
int TabCtrl_Create(HWND hTabCtrl, PVOID pPropSheetHeader);
int TabCtrl_Resize(HWND hTabCtrl, int x, int y, int cx, int cy);
int TabCtrl_SelectPageByIndex(HWND hTabCtrl, UINT nPageIndex);
int TabCtrl_SelectPageByID(HWND hTabCtrl, LPCTSTR pszPageID);
HWND TabCtrl_GetSelectedPage(HWND hTabCtrl);
INT_PTR TabCtrl_HandleMessages(HWND hTabControl, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Reads line from text file. The file must be opened in text mode
// Returns -1 if end of file, otherwise number of characters read
int ReadLine(FILE * fp, LPTSTR szBuffer, int nMaxChars);

// Replaces the file name with the another one
int ReplaceFileName(LPTSTR szFullPath, LPCTSTR szPlainName);

// Replaces the file extension with another one.
int ReplaceFileExt(LPTSTR szFileName, LPTSTR szNewExt);

// Like sprintf, but the format string is taken from resources
int rsprintf(LPTSTR szBuffer, int nMaxChars, UINT nIDFormat, ...);

// Recalculates a screen window position (such as retrieved by GetWindowRect)
// to the client coordinates of the window "hWnd".
void ScreenRectToClientRect(HWND hWnd, LPRECT pRect);
void ClientRectToScreenRect(HWND hWnd, LPRECT pRect);

// Sets a bold font for a dialog control
int SetBoldFont(HWND hDlg, UINT nIDCtrl, int nPercentSize = 0);

// Sets a window text from resource
int SetWindowTextRc(HWND hWnd, UINT nIDText, ...);

// Shows a systray baloon or a timed messagebox
int ShowSystrayBaloon(HWND hDlg, UINT nIDIcon, UINT nIDTitle, UINT nIDText, ...);

// Verifies if the user's password is valid.
BOOL VerifyUserPassword(LPTSTR szUserName, LPTSTR szDomain, LPTSTR szPassword);

// Finds a path to WinDbg debugger
// Optionally sets the WINDBG_DIR environment variable
bool FindWindbgPath(LPTSTR szBuffer, size_t cchMaxChars, bool bSetEnvironmentVariable);

// Works with FS redirection
void DisableWoW64FsRedirection(PVOID * ppvOldValue);
void RevertWoW64FsRedirection(PVOID pvOldValue);
bool IsWow64Process();

// Writes the dump file after crash
LONG WriteDumpFile(HWND hWndParent, PEXCEPTION_POINTERS ExceptionPointers);

#endif // __UTILS_H__
