/*****************************************************************************/
/* afxres_mini.h                          Copyright (c) Ladislav Zezula 2025 */
/*---------------------------------------------------------------------------*/
/* This header supplies the most common symbols from afxres..h,              */
/* which allows our VS2022 projects to be build without MFC installed        */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 07.06.04  1.00  Lad  The first version of TToolTip.h                      */
/*****************************************************************************/

#ifndef __AFXRES_MINI_H__
#define __AFXRES_MINI_H__
#ifndef __AFXRES_H__            // Back off if afxres.h has already been included

#ifndef _INC_WINDOWS
#include "winres.h"             // Definitions such as LANG_NEUTRAL
#endif

//-----------------------------------------------------------------------------
// File commands

#ifndef ID_FILE_NEW
#define ID_FILE_NEW                     0xE100
#endif

#ifndef ID_FILE_OPEN
#define ID_FILE_OPEN                    0xE101
#endif

#ifndef ID_FILE_CLOSE
#define ID_FILE_CLOSE                   0xE102
#endif

#ifndef ID_FILE_SAVE
#define ID_FILE_SAVE                    0xE103
#endif

#ifndef ID_FILE_SAVE_AS
#define ID_FILE_SAVE_AS                 0xE104
#endif

#ifndef ID_FILE_PRINT
#define ID_FILE_PRINT                   0xE107
#endif

//-----------------------------------------------------------------------------
// View commands (same number used as IDW used for control bar)
// E810 -> E81F must be kept in order for RANGE macros

#ifndef ID_VIEW_TOOLBAR
#define ID_VIEW_TOOLBAR                 0xE800
#endif

#ifndef ID_VIEW_STATUS_BAR
#define ID_VIEW_STATUS_BAR              0xE801
#endif

#ifndef ID_VIEW_REBAR
#define ID_VIEW_REBAR                   0xE804
#endif

#ifndef ID_VIEW_AUTOARRANGE
#define ID_VIEW_AUTOARRANGE             0xE805
#endif

#ifndef ID_VIEW_SMALLICON
#define ID_VIEW_SMALLICON               0xE810
#endif

#ifndef ID_VIEW_LARGEICON
#define ID_VIEW_LARGEICON               0xE811
#endif

#ifndef ID_VIEW_LIST
#define ID_VIEW_LIST                    0xE812
#endif

#ifndef ID_VIEW_DETAILS
#define ID_VIEW_DETAILS                 0xE813
#endif

#ifndef ID_VIEW_LINEUP
#define ID_VIEW_LINEUP                  0xE814
#endif

#ifndef ID_VIEW_BYNAME
#define ID_VIEW_BYNAME                  0xE815
#endif

//-----------------------------------------------------------------------------
// Edit commands

#ifndef ID_EDIT_CLEAR
#define ID_EDIT_CLEAR                   0xE120
#endif

#ifndef ID_EDIT_CLEAR_ALL
#define ID_EDIT_CLEAR_ALL               0xE121
#endif

#ifndef ID_EDIT_COPY
#define ID_EDIT_COPY                    0xE122
#endif

#ifndef ID_EDIT_CUT
#define ID_EDIT_CUT                     0xE123
#endif

#ifndef ID_EDIT_FIND
#define ID_EDIT_FIND                    0xE124
#endif

#ifndef ID_EDIT_PASTE
#define ID_EDIT_PASTE                   0xE125
#endif

#ifndef ID_EDIT_REPEAT
#define ID_EDIT_REPEAT                  0xE128
#endif

#ifndef ID_EDIT_REPLACE
#define ID_EDIT_REPLACE                 0xE129
#endif

#ifndef ID_EDIT_SELECT_ALL
#define ID_EDIT_SELECT_ALL              0xE12A
#endif

#ifndef ID_EDIT_UNDO
#define ID_EDIT_UNDO                    0xE12B
#endif

#ifndef ID_EDIT_REDO
#define ID_EDIT_REDO                    0xE12C
#endif

//-----------------------------------------------------------------------------
// Window commands

#ifndef ID_WINDOW_NEW
#define ID_WINDOW_NEW                   0xE130
#endif

#ifndef ID_WINDOW_ARRANGE
#define ID_WINDOW_ARRANGE               0xE131
#endif

#ifndef ID_WINDOW_CASCADE
#define ID_WINDOW_CASCADE               0xE132
#endif

#ifndef ID_WINDOW_TILE_HORZ
#define ID_WINDOW_TILE_HORZ             0xE133
#endif

#ifndef ID_WINDOW_TILE_VERT
#define ID_WINDOW_TILE_VERT             0xE134
#endif

#ifndef ID_WINDOW_SPLIT
#define ID_WINDOW_SPLIT                 0xE135
#endif

//-----------------------------------------------------------------------------
// Application commands

#ifndef ID_APP_ABOUT
#define ID_APP_ABOUT                    0xE140
#endif

#ifndef ID_APP_EXIT
#define ID_APP_EXIT                     0xE141
#endif

//-----------------------------------------------------------------------------
// Property Sheet control id's (determined with Spy++)

#ifndef ID_APPLY_NOW
#define ID_APPLY_NOW                    0x3021
#endif

#ifndef ID_WIZBACK
#define ID_WIZBACK                      0x3023
#endif

#ifndef ID_WIZNEXT
#define ID_WIZNEXT                      0x3024
#endif

#ifndef ID_WIZFINISH
#define ID_WIZFINISH                    0x3025
#endif

#ifndef AFX_IDC_TAB_CONTROL
#define AFX_IDC_TAB_CONTROL             0x3020
#endif

#endif  // __AFXRES_H__
#endif  // __AFXRES_MINI_H__
