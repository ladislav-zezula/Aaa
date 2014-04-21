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

struct TAnchor
{
    TAnchor * pNext;                    // Pointer to the next anchor (or NULL if none)
    HWND   hWnd;                        // Handle to the child (anchored) window

    int    nLeftRel;                    // Here are the anchor points. The nXXXRel means relative position
    int    nLeftSpace;                  // of the anchor point (in percent of the parent client area)
    int    nTopRel;                     // Example :   0 is left-most or top-most point in the parent's client area
    int    nTopSpace;                   //            50 is is center of the parent's client area
    int    nRightRel;                   //           100 is right-most or bottom-most point of the parent's client area
    int    nRightSpace;                 // akNotAnchored means that this side of the control is not anchored
    int    nBottomRel;
    int    nBottomSpace;
    int    nThemePartId;
    BOOL   bIsGroupBox;
};

class TAnchors
{
  public:
    TAnchors();                         // In the new version is a new "hParent" parameter 
    ~TAnchors();

    // Adding anchors
    BOOL AddAnchor(HWND hWnd, DWORD dwAnchors);
    BOOL AddAnchor(HWND hDlg, UINT nIDCtrl, DWORD dwAnchors);

    BOOL AddAnchorEx(HWND hWnd, int nLeftRel, int nTopRel, int nRightRel, int nBottomRel);
    BOOL AddAnchorEx(HWND hDlg, UINT nIDCtrl, int nLeftRel, int nTopRel, int nRightRel, int nBottomRel);

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

    void InsertAnchor(TAnchor * pNewAnchor);
    void GetParentWindowRect(HWND hParent, RECT & rect);
    void SaveParentWindowInfo(HWND hParent);
    void SaveChildWindowInfo(HWND hWnd, TAnchor * pAnchor);

    // Calculates new window position and size
    BOOL GetNewWindowRect(HWND hParent, TAnchor * pAnchor, RECT & rect);
    void AddWsClipChildren(HWND hParent);

    // Double buffering repaint logic
    void MoveWindowOrg(HDC hDC, int dx, int dy);
//  int  FindThemePartId(LPCTSTR szPartsName);
//  void PaintThemedFrame(TAnchor * pAnchor, HDC hDC, LPRECT pEdgeRect);
    BOOL PaintChildrenTo(HWND hWnd, HDC hDC);
    BOOL PaintWindow(HWND hWnd);

    // xxxWindowPos helpers
    void DisableGroupBoxRedrawing();
    void RedrawGroupBoxes();

    static BOOL WindowIsGroupBox(HWND hWnd);

    TAnchor * m_pFirstAnchor;
    TAnchor * m_pLastAnchor;
    RECT      m_rectParent;             // Parent rectangle (initialized in SetParentRectManual)
    HWND      m_hWndParent;             // Parent window of the anchored controls
    BOOL      m_bHasGroupBox;           // TRUE if one of the windows is a group box
    BOOL      m_bThemeActive;           // TRUE if the XP+ theme is active
    int       m_nAnchors;               // Number of anchors
    int       m_nMinSizeX;
    int       m_nMinSizeY;
    int       m_nFirstThemeParts;       
};

#endif
