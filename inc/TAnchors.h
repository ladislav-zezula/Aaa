/*****************************************************************************/
/* TAnchors.h                                 Copyright Ladislav Zezula 2001 */
/*---------------------------------------------------------------------------*/
/* A class for easy object anchoring. Those who compare the Delphi program-  */
/* ming with Visual C++, will certainly miss the "Anchors" property in visual*/
/* design. This class supports something like that, although it's not so     */
/* comfort like the visual design in Delphi/C++ builder                      */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -----------------------------------------------------*/
/* 19.12.01  1.00  Lad  Created                                              */
/* 31.08.04  2.00  Lad  Added support for proportional resizing              */
/*****************************************************************************/

#ifndef __TANCHORS_H__
#define __TANCHORS_H__

//-----------------------------------------------------------------------------
// Constants

// Anchor types
#define akTop           0x00000001          // Bind the window's upper edge to the parent's top border
#define akTopCenter     0x00000002          // Bind the window's upper edge to the parent's center
#define akLeft          0x00000010          // Bind the window's left edge to the parent's left border
#define akLeftCenter    0x00000020          // Bind the window's left edge to the parent's center
#define akRight         0x00000100          // Bind the window's right edge to the parent's right border
#define akRightCenter   0x00000200          // Bind the window's right edge to the parent's center
#define akBottom        0x00001000          // Bind the window's bottom edge to the parent's bottom border
#define akBottomCenter  0x00002000          // Bind the window's bottom edge to the parent's center

#define akNotAnchored (int)0x80000000       // This edge is not anchored

#define akLeftTop     (akLeft  | akTop)
#define akRightBottom (akRight | akBottom)
#define akAll         (akLeft  | akTop | akRight | akBottom)

typedef BOOL (*CALCCHILDPOS)(HWND hWndChild, PVOID pvUserParam, const RECT & NewClientRect, WINDOWPOS & NewChildPos);

typedef enum _WNDCLS
{
    WndNotSpecified,
    WndDialogBox,                           // The control is a dialog box
    WndTabControl,                          // The control is a tab control
    WndComboDropDown,                       // The control is a combo box with editable window (CBS_DROPDOWN)
} WNDCLS, *PWNDCLS;

struct TAnchorPT
{
    TAnchorPT(int nRelPt, int nDelta);      // Bind the window to a specified point in the parent's client area
    TAnchorPT();

    // Recalculates the relative point to absolute point
    int AbsPoint(const SIZE & sizeParent, DWORD dwEdge);

    bool IsAnchored()
    {
        return (RelPt != akNotAnchored);
    }

    int RelPt;                              // The anchor is related to the percent size of the parent's CX/CY
                                            // Example :   0 is left-most or top-most point in the parent's client area
                                            //            50 is is center of the parent's client CX/CY
                                            //           100 is right-most or bottom-most point of the parent's client area
                                            // The value of RelPt = akNotAnchored means that this edge is not anchored

    int Delta;                              // Fixed distance from the relative point. This only changes when the DPI changes
                                            // The client position of the window is at RelativePoint + Delta
};

class TAnchor
{
    public:

    TAnchor(HWND hWndChild);
    ~TAnchor();

    // Initializing the anchor either from anchors or from custom callback
    void Initialize(HWND hWndParent, int aptLeft, int aptTop, int aptRight, int aptBottom);
    void Initialize(HWND hWndParent, CALCCHILDPOS PfnCalcChildPos, PVOID pvUserParam);

    // Recalculate for a new DPI
    void NewDpi(const RECT & rectParent);

    protected:

    // Calculates the control's rectangle, relative to parent's client area
    void UpdateControlRect(HWND hWndParent, LPRECT pRectParent, LPRECT pRectControl);

    public:

    LIST_ENTRY Entry;                   // Links to other anchors

#ifdef _DEBUG
    TCHAR szClassName[80];              // Class name of the control
    TCHAR szWindowText[80];             // Text of the control (possibly truncated
#endif

    HWND hWnd;                          // Handle to the child (anchored) window
    RECT rect;                          // Rectangle of the control (relative to parent's client area)

    CALCCHILDPOS PfnCalcChildPos;       // Custom callback for calculating the child window position
    LPARAM SaveSelection;               // Used for dropdown combo boxes during resize
    PVOID pvUserParam;                  // User parameter for PfnCalcChildPos

    public:

    TAnchorPT Top;                      // Anchor of the top edge of the window
    TAnchorPT Left;                     // Anchor of the left edge of the window
    TAnchorPT Right;                    // Anchor of the right edge of the window
    TAnchorPT Bottom;                   // Anchor of the bottom edge of the window
    WNDCLS WndType;                     // Special window type
};

class TAnchors
{
    public:

    TAnchors(HWND hWndParent);
    ~TAnchors();

    // Adding anchors
    TAnchor * AddAnchor(HWND hWndChild, DWORD dwAnchors);
    TAnchor * AddAnchor(HWND hDlg, UINT nIDCtrl, DWORD dwAnchors);

    TAnchor * AddAnchor(HWND hWndChild, CALCCHILDPOS PfnGetNewChildRect, PVOID pvUserParam);
    TAnchor * AddAnchor(HWND hDlg, UINT nIDCtrl, CALCCHILDPOS PfnGetNewChildRect, PVOID pvUserParam);

    TAnchor * AddAnchor(HWND hWndChild, int aptLeft, int aptTop, int aptRight, int aptBottom);
    TAnchor * AddAnchor(HWND hDlg, UINT nIDCtrl, int aptLeft, int aptTop, int aptRight, int aptBottom);

    // Removes the anchor from the list. Note that this does NOT destroy the window.
    bool RemoveAnchor(TAnchor * pAnchor);

    // Returns the parent window
    HWND GetParentWindow()
    {
        return m_hWndParent;
    }

    // Call this method in the handler of WM_SIZE (or OnSize in MFC)
    BOOL OnSize();
    BOOL OnGetMinMaxInfo(LPARAM lParam);
    BOOL OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:

    TAnchor * CreateNewAnchor(HWND hWndChild);
    void InsertAnchor(TAnchor * pAnchor);
    SIZE & SavePreviousSize();

    // Calculates new window position and size
    BOOL GetNewChildRect(TAnchor * pAnchor, const RECT & NewClientRect, WINDOWPOS & NewChildPos);
    void ComboBox_SaveSelection(TAnchor * pAnchor);
    bool IsDialogAfterDpiChange(bool bUpdateDPI = false);
    void RescaleForDpi();

    LIST_ENTRY m_AnchorLinks;
    SIZE m_sizeLast;                    // Previously saved window rectangle of the dialog
    SIZE m_sizeMin;                     // Minimal size of the dialog
    HWND m_hWndParent;                  // Parent window of the anchored controls
    UINT m_bThemeActive:1;              // TRUE if the XP+ theme is active
    UINT m_bIsDialog:1;                 // TRUE if the marend window is a dialog box
    UINT m_nAnchors;                    // Number of anchors
    UINT m_nDPI;                        // Previously saved DPI
};

#endif
