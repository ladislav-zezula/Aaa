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
#define akLeft          0x00000001      // The child's left side is anchored to parent's left side
#define akLeftCenter    0x00000002      // The child's left side is anchored to parent's center
#define akTop           0x00000004      // The child's top side is anchored to parent's top side
#define akTopCenter     0x00000008      // The child's top side is anchored to parent's center
#define akRight         0x00000010      // The child's right side is anchored to parent's right side
#define akRightCenter   0x00000020      // The child's right side is anchored to parent's center
#define akBottom        0x00000040      // The child's bottom side is anchored to parent's bottom side
#define akBottomCenter  0x00000080      // The child's bottom side is anchored to parent's center

#define akNotAnchored (int)0x80000000   // A side is not anchored (use with AddAnchorEx)

#define akLeftTop     (akLeft  | akTop)
#define akRightBottom (akRight | akBottom)
#define akAll         (akLeft  | akTop | akRight | akBottom)

typedef BOOL (*CALCCHILDPOS)(HWND hWndChild, PVOID pvUserParam, const RECT & NewClientRect, struct _WNDPOS & NewChildPos);

typedef struct _WNDPOS
{
    int x;
    int y;
    int cx;
    int cy;
} WNDPOS, *PWNDPOS;

typedef enum _WNDTYP
{
    WndNone,
    WndGroupBox,                        // The window is a group box
    WndComboDropDown,                   // The window is a combo box with editable window (CBS_DROPDOWN)
} WNDTYP, *PWNDTYP;

struct TAnchor
{
    LIST_ENTRY Entry;                   // Links to other anchors
    HWND   hWnd;                        // Handle to the child (anchored) window

    CALCCHILDPOS PfnCalcChildPos;       // Custom callback for calculating the child window position
    LPARAM SaveSelection;               // Used for dropdown combo boxes during resize
    PVOID  pvUserParam;
    WNDTYP WndType;                     // Special window type
    int    nLeftRel;                    // Here are the anchor points. The nXXXRel means relative position
    int    nLeftSpace;                  // of the anchor point (in percent of the parent client area)
    int    nTopRel;                     // Example :   0 is left-most or top-most point in the parent's client area
    int    nTopSpace;                   //            50 is is center of the parent's client area
    int    nRightRel;                   //           100 is right-most or bottom-most point of the parent's client area
    int    nRightSpace;                 // akNotAnchored means that this side of the control is not anchored
    int    nBottomRel;
    int    nBottomSpace;
    int    nThemePartId;
};

class TAnchors
{
  public:
    TAnchors();                         // In the new version is a new "hParent" parameter 
    ~TAnchors();

    // Adding anchors
    TAnchor * AddAnchor(HWND hWndChild, DWORD dwAnchors);
    TAnchor * AddAnchor(HWND hDlg, UINT nIDCtrl, DWORD dwAnchors);

    TAnchor * AddAnchor(HWND hWndChild, CALCCHILDPOS PfnGetNewChildRect, PVOID pvUserParam);
    TAnchor * AddAnchor(HWND hDlg, UINT nIDCtrl, CALCCHILDPOS PfnGetNewChildRect, PVOID pvUserParam);

    TAnchor * AddAnchorEx(HWND hWndChild, int nLeftRel, int nTopRel, int nRightRel, int nBottomRel);
    TAnchor * AddAnchorEx(HWND hDlg, UINT nIDCtrl, int nLeftRel, int nTopRel, int nRightRel, int nBottomRel);

    // Removes the anchor from the list. Note that this does NOT destroy the window.
    bool RemoveAnchor(TAnchor * pAnchor);

    // Sets the size of the parent window manually
    void SetParentRectManual(RECT & rectParent);

    // Call this method in the handler of WM_SIZE (or OnSize in MFC)
    BOOL OnSize();
    
    // Call this method in the handler of WM_GETMINMAXINFO (or OnGetMinMaxInfo in MFC)
    // This methos will set minimum dialog size to the designed size
    BOOL OnGetMinMaxInfo(LPARAM lParam);

    // Returns the parent dialog of the dialog controls
    HWND GetParentWindow()  { return m_hWndParent; }

    protected:

    TAnchor * CreateNewAnchor(HWND hWndChild, RECT & rectParent, RECT & rectChild);
    TAnchor * InsertAnchor(TAnchor * pAnchor);
    void GetParentWindowRect(HWND hParent, RECT & rect);
    void SaveParentWindowInfo(HWND hWndParent);
    void SaveChildWindowInfo(HWND hWndChild, TAnchor * pAnchor);
    WNDTYP GetChildWindowType(HWND hWndChild);

    // Calculates new window position and size
    BOOL GetNewChildRect(TAnchor * pAnchor, const RECT & NewClientRect, WNDPOS & NewChildPos);
    void UpdateWsClipChildren(HWND hWndParent);
    void SaveComboBoxSelection(TAnchor * pAnchor, DWORD & RefComboCount);

    // Double buffering repaint logic
    void MoveWindowOrg(HDC hDC, int dx, int dy);
//  int  FindThemePartId(LPCTSTR szPartsName);
//  void PaintThemedFrame(TAnchor * pAnchor, HDC hDC, LPRECT pEdgeRect);
    BOOL PaintChildrenTo(HWND hWnd, HDC hDC);
    BOOL PaintWindow(HWND hWnd);

    LIST_ENTRY m_AnchorLinks;
    RECT      m_rectParent;             // Parent rectangle (initialized in SetParentRectManual)
    HWND      m_hWndParent;             // Parent window of the anchored controls
    UINT      m_bHasGroupBox:1;         // TRUE if one of the windows is a group box
    UINT      m_bThemeActive:1;         // TRUE if the XP+ theme is active
    int       m_nAnchors;               // Number of anchors
    int       m_nMinSizeX;
    int       m_nMinSizeY;
    int       m_nFirstThemeParts;       
};

#endif
