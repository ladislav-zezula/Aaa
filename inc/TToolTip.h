/*****************************************************************************/
/* TToolTip.h                             Copyright (c) Ladislav Zezula 2004 */
/*---------------------------------------------------------------------------*/
/* Header file which interfaces the class implementing a tooltip             */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 07.06.04  1.00  Lad  The first version of TToolTip.h                      */
/*****************************************************************************/

#ifndef __TTOOLTIP_H__
#define __TTOOLTIP_H__

class TToolTip
{
    public:

    // Constructor uses the handle to the window which will show the tooltips,
    // e.g. dialog box window. For a dialog, a good place to construct the
    // TToolTip object is the OnInitDialog method.
    TToolTip(HWND hParent);
    ~TToolTip();

    // Adding tooltips for child windows and/or rectangles
    LRESULT AddTipForChild(HWND hWnd, UINT_PTR nIDString);
    LRESULT AddTipForChild(HWND hDlg, UINT nIDCtrl, UINT nIDString);
    LRESULT AddTipForRect(RECT & rect, UINT_PTR nIDString);

    // Configuring tooltip
//  void RemoveAll();


    protected:

    HWND m_hParent;
    HWND m_hToolTip;
    UINT m_nIdCounter;
};

#endif // __TTOOLTIP_H__
