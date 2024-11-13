/*****************************************************************************/
/* TShellCtxMenu.h                        Copyright (c) Ladislav Zezula 2004 */
/*---------------------------------------------------------------------------*/
/* Description:                                                              */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 01.07.04  1.00  Lad  The first version of TShellCtxMenu.h                 */
/*****************************************************************************/

#ifndef __TSHELLCTXMENU_H__
#define __TSHELLCTXMENU_H__

#include <shlobj.h>

class TShellContextMenu
{
public:

    TShellContextMenu();
    virtual ~TShellContextMenu();

    int AddFile(LPCTSTR szFileName);
    int RunMenu(HWND hWndParent, POINT pt);

protected:

    static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    int GetPIDLCount(LPCITEMIDLIST pidl);
    UINT GetPIDLSize(LPCITEMIDLIST pidl);
    void FreePIDLList(LPITEMIDLIST * ppItemIDList, DWORD nItems);
    LPBYTE GetPIDLPos(LPCITEMIDLIST pidl, int nPos);
    LPITEMIDLIST CopyPIDL(LPCITEMIDLIST pidl, INT_PTR cb = -1);

    DWORD GetIContextMenu(HWND hWndParent, IContextMenu ** pContextMenu, DWORD * pdwVersion);
    HRESULT SHBindToParentEx(LPCITEMIDLIST pidl, REFIID riid, VOID ** ppv, LPCITEMIDLIST * ppidlLast);

    LPITEMIDLIST * m_ppItemIDList;      // Item ID list
    IShellFolder * m_pShellFolder;      // IShellFolder relative to the parent
    OLECHAR     ** m_ppFileNames;
    DWORD          m_dwFiles;
    HANDLE         m_hHeap;
};


#endif // __TSHELLCTXMENU_H__
