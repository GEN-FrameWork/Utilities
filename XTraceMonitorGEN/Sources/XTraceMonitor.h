/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor.h
*
* @class      XTRACEMONITOR
* @brief      GEN graphical eXtended Trace Monitor
* @ingroup    UTILITIES
*
* @copyright  EndoraSoft. All rights reserved.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDateTime.h"
#include "XFSMachine.h"
#include "XString.h"

#include "APPFlowGraphics.h"

#include "XTraceMonitor_Form_CFG.h"
#include "XTraceMonitor_Form_Filters.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XTRACEMONITORXFSMEVENTS
{
  XTRACEMONITOR_XFSMEVENT_NONE              = 0 ,
  XTRACEMONITOR_XFSMEVENT_INI                   ,
  XTRACEMONITOR_XFSMEVENT_END                   ,

  XTRACEMONITOR_LASTEVENT
};


enum XTRACEMONITORXFSMSTATES
{
  XTRACEMONITOR_XFSMSTATE_NONE              = 0 ,
  XTRACEMONITOR_XFSMSTATE_INI                   ,
  XTRACEMONITOR_XFSMSTATE_END                   ,

  XTRACEMONITOR_LASTSTATE
};


enum XTRACEMONITOR_BUTTONS
{
  XTRACEMONITOR_BUTTON_ESC                  = 0 ,
  XTRACEMONITOR_BUTTON_MOUSE                    ,
  XTRACEMONITOR_BUTTON_TOUCHSCREEN              ,

  XTRACEMONITOR_BUTTON_MAX
};


#define APPLICATION_VERSION                       0
#define APPLICATION_SUBVERSION                    1
#define APPLICATION_SUBVERSIONERR                 0

#define APPLICATION_NAMEAPP                       NL("XTraceMonitor")
#define APPLICATION_NAMEFILE                      NL("xtracemonitor")

#define APPLICATION_OWNER                         NL("EndoraSoft")

#define APPLICATION_YEAROFCREATION                2026

#define XTRACEMONITOR_WINDOW_WIDTH                1200   // minimum client width (also design size)
#define XTRACEMONITOR_WINDOW_HEIGHT               699    // minimum client height (also design size)

// Flexible resize layout (only these grow/shrink; everything else stays fixed at design coords).
#define XTRACEMONITOR_LAYOUT_MARGIN               6.0
#define XTRACEMONITOR_LAYOUT_GAP                  6.0
#define XTRACEMONITOR_LAYOUT_PANELS_X             222.0  // tree 6+210 + gap 6
#define XTRACEMONITOR_LAYOUT_CAPTION_H            46.0
#define XTRACEMONITOR_LAYOUT_CHROME_GAP           2.0    // air under custom caption before content
#define XTRACEMONITOR_LAYOUT_VALOR_TOP            148.0  // clears legends (y=134) + compact legend wipe
#define XTRACEMONITOR_LAYOUT_VALOR_H              158.0  // header 26 + 6 rows×22 (exact 6 lines)
#define XTRACEMONITOR_LAYOUT_FOOT_H               22.0
#define XTRACEMONITOR_LAYOUT_BRAND_W              82.0
#define XTRACEMONITOR_LAYOUT_BRAND_H              82.0
#define XTRACEMONITOR_LAYOUT_BRAND_GAP_VALOR      3.0    // brand bottom above valor top
#define XTRACEMONITOR_LAYOUT_OPSTATUS_TEXT_PAD_X  8.0

#define XTRACEMONITOR_OPSTATUS_MAXLINES           20
#define XTRACEMONITOR_OPSTATUS_LINEHEIGHT         18.0f
#define XTRACEMONITOR_OPSTATUS_PAD                4.0f
#define XTRACEMONITOR_OPSTATUS_DBLCLICK_MS        700

// Config modal: fixed size = 75% of the app minimum client (1200×699), always; only recentered.
#define XTRACEMONITOR_CONFIGMODAL_W               900.0  // 1200 * 0.75
#define XTRACEMONITOR_CONFIGMODAL_H               524.0  // 699  * 0.75 (rounded)
#define XTRACEMONITOR_CONFIGMODAL_BTN_W           160.0  // 120×40 + 1/3
#define XTRACEMONITOR_CONFIGMODAL_BTN_H           53.0
#define XTRACEMONITOR_CONFIGMODAL_BTN_GAP         16.0
#define XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD  16.0  // gap from modal bottom to button bottom
// PNG icon ends ~x=40; caption is centered in the remaining slot with a gap after the icon.
#define XTRACEMONITOR_CONFIGMODAL_BTN_ICON_RIGHT  40.0
#define XTRACEMONITOR_CONFIGMODAL_BTN_ICON_GAP     8.0
#define XTRACEMONITOR_CONFIGMODAL_BTN_TEXT_PAD_R  16.0
#define XTRACEMONITOR_CONFIGMODAL_PAD             24.0
#define XTRACEMONITOR_CONFIGMODAL_TITLE_PAD       16.0
#define XTRACEMONITOR_CONFIGMODAL_TITLE_H         22.0



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class INPBUTTON;
class INPCURSOR;
class UI_XEVENT;
class UI_ELEMENT;
class UI_ELEMENT_TEXT;
class UI_ELEMENT_TEXTBOX;
class UI_ELEMENT_CONSOLE;
class UI_ELEMENT_PROPERTYLIST;
class UI_ELEMENT_TREE;
class UI_ELEMENT_IMAGE;
class GRPXEVENT;
class XTRACEMONITOR_CFG;
class UI_STYLE;
class UI_STYLESHEET;

class XTRACEMONITOR : public APPFLOWGRAPHICS, public XFSMACHINE
{
  public:
                                    XTRACEMONITOR                           ();
    virtual                        ~XTRACEMONITOR                           ();

    bool                            InitFSMachine                           ();

    bool                            AppProc_Ini                             ();
    bool                            AppProc_FirstUpdate                     ();
    bool                            AppProc_Update                          ();
    bool                            AppProc_End                             ();

  private:

    friend class                    XTRACEMONITOR_FORM_CFG;
    friend class                    XTRACEMONITOR_FORM_FILTERS;

    bool                            UpdateInput                             ();

    bool                            Ini_Graphics                            (GRPSCREEN* screen);
    bool                            Ini_Canvas                              (GRPSCREEN* screen);
    bool                            Ini_UserInterface                       (bool on);

    bool                            DrawFrame                               ();

    bool                            Layout_ApplyFlexibleResize              (bool compose = true);

    bool                            Console_Resolve                         ();
    bool                            Console_SeedStartupLines                ();

    bool                            Valor_Resolve                           ();
    bool                            Valor_Seed                              ();
    bool                            Valor_UpdateSplitter                    ();

    bool                            Tree_Resolve                            ();
    bool                            Tree_Seed                               ();
    bool                            Tree_UpdateInput                        ();

    bool                            BrandMonitor_Resolve                    ();
    bool                            BrandMonitor_RepaintAfterChrome         ();

    bool                            OpStatus_Resolve                        ();
    bool                            OpStatus_Log                            (XCHAR* message);
    bool                            OpStatus_Refresh                        ();
    bool                            OpStatus_DisableScroll                  ();
    bool                            OpStatus_ApplyGeometry                  (XDWORD visible_lines);
    bool                            OpStatus_SetExpanded                    (bool expanded);
    bool                            OpStatus_ToggleExpanded                 ();
    bool                            OpStatus_UpdateInput                    ();
    bool                            OpStatus_OnClick                        ();
    bool                            OpStatus_HitTest                        (int screen_x, int screen_y);

    bool                            Toolbar_SetWorkingMode                  (bool on);
    bool                            Toolbar_ApplyRunStopVisibility          ();
    bool                            Toolbar_SlotSetVisible                  (UI_ELEMENT* element, bool visible);

    bool                            ApplySavedScreenGeometry                ();
    bool                            PersistScreenGeometryToCFG              ();
    void                            PersistScreenGeometryIfExiting          ();
    bool                            ApplyColorTheme                         (int colortheme);
    bool                            ApplyColorTheme_Element                 (UI_ELEMENT* element, bool light);
    bool                            ApplyColorTheme_PatchStyleBag          (UI_STYLE& style, bool light);
    bool                            ApplyColorTheme_PatchStyleSheet         (UI_STYLESHEET* sheet, bool light);
    bool                            ApplyColorTheme_RefreshChrome           (bool light);

    bool                            UserInterface_ElementSelected           (UI_ELEMENT* element);
    bool                            UserInterface_ChangeLiteralText         (UI_ELEMENT_TEXT* element_text, XSTRING* maskvalue, XSTRING* maskresult);
    bool                            UserInterface_CFGChromes                (GRPSCREEN* screen);

    void                            HandleEvent_UserInterface               (UI_XEVENT* event);
    void                            HandleEvent_Graphics                    (GRPXEVENT* event);
    void                            HandleEvent                             (XEVENT* xevent);

    void                            Clean                                   ();

    XTIMER*                         xtimer;

    INPBUTTON*                      button[XTRACEMONITOR_BUTTON_MAX];
    INPCURSOR*                      cursor;

    UI_ELEMENT_CONSOLE*             consoleelement;
    UI_ELEMENT_PROPERTYLIST*        valorelement;
    bool                            valor_prevpressed;

    UI_ELEMENT_TREE*                treeelement;
    bool                            tree_prevpressed;

    UI_ELEMENT_FORM*                opstatus_form;
    UI_ELEMENT_TEXTBOX*             opstatus_text;
    XSTRING                         opstatus_msgs[XTRACEMONITOR_OPSTATUS_MAXLINES];
    XDWORD                          opstatus_count;
    double                          opstatus_foot_y;
    double                          opstatus_last_baseline;
    double                          opstatus_line_h;
    bool                            opstatus_expanded;
    bool                            opstatus_prevpressed;
    bool                            opstatus_click_armed;
    XTIMER*                         opstatus_click_timer;

    UI_ELEMENT_IMAGE*               brand_monitor;

    XTRACEMONITOR_FORM_CFG          form_cfg;
    XTRACEMONITOR_FORM_FILTERS      form_filters;

    int                             layout_applied_w;
    int                             layout_applied_h;
    int                             resize_recover_frames; // full wipe+redraw after maximize/extend

    bool                            workingmode;   // true = working (Stop visible); false = paused (Run visible)
    bool                            chrome_theme_dirty; // RefreshChrome pending until chrome layout exists
    bool                            exitapp;
    bool                            screen_geometry_persisted; // one-shot save before mainscreen is destroyed
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
