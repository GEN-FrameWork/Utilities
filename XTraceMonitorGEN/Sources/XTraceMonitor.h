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
#include "XTraceMonitor_Form_About.h"
#include "XTraceMonitor_Form_DynDNS.h"
#include "XTraceMonitor_Server.h"
#include "XTraceMonitor_Origins.h"
#include "XTraceMonitor_GUI_Tree.h"
#include "XTraceMonitor_GUI_Console.h"
#include "XTraceMonitor_GUI_VarStatus.h"
#include "XTraceMonitor_GeoIP.h"



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
  XTRACEMONITOR_BUTTON_A                        ,
  XTRACEMONITOR_BUTTON_C                        ,
  XTRACEMONITOR_BUTTON_CTRL_LEFT                ,
  XTRACEMONITOR_BUTTON_CTRL_RIGHT               ,
  XTRACEMONITOR_BUTTON_SHIFT_LEFT               ,
  XTRACEMONITOR_BUTTON_SHIFT_RIGHT              ,
  XTRACEMONITOR_BUTTON_MOUSE                    ,
  XTRACEMONITOR_BUTTON_TOUCHSCREEN              ,

  XTRACEMONITOR_BUTTON_MAX
};


#define APPLICATION_VERSION                         8
#define APPLICATION_SUBVERSION                      0
#define APPLICATION_SUBVERSIONERR                   0

#define APPLICATION_NAMEAPP                         NL("eXtended Trace Monitor")
#define APPLICATION_NAMEFILE                        NL("xtracemonitor")

#define APPLICATION_OWNER                           NL("EndoraSoft")

#define APPLICATION_YEAROFCREATION                  2026

#define XTRACEMONITOR_WINDOW_WIDTH                  1200   // minimum client width (also design size)
#define XTRACEMONITOR_WINDOW_HEIGHT                 699    // minimum client height (also design size)

// Flexible resize layout (only these grow/shrink; everything else stays fixed at design coords).
#define XTRACEMONITOR_LAYOUT_MARGIN                 6.0
#define XTRACEMONITOR_LAYOUT_GAP                    6.0
#define XTRACEMONITOR_LAYOUT_PANELS_X               222.0  // tree 6+210 + gap 6
#define XTRACEMONITOR_LAYOUT_CAPTION_H              40.0   // matches chrome app icon (40×40)
#define XTRACEMONITOR_LAYOUT_CHROME_GAP             0.0    // toolbar bubbles flush under caption
#define XTRACEMONITOR_LAYOUT_VALOR_TOP              134.0  // 8px under legend bottom (y=126)
#define XTRACEMONITOR_LAYOUT_VALOR_H                158.0  // header 26 + 6 rows×22 (exact 6 lines)
#define XTRACEMONITOR_LAYOUT_FOOT_H                 22.0
#define XTRACEMONITOR_LAYOUT_TREESTATS_H            22.0   // IP counters strip under the tree
#define XTRACEMONITOR_LAYOUT_OPSTATUS_TEXT_PAD_X    8.0
#define XTRACEMONITOR_LAYOUT_NETSTATUS_LED          12.0   // visible circle; must stay paintable (opaque fill)
#define XTRACEMONITOR_LAYOUT_NETSTATUS_LINE_H       16.0
#define XTRACEMONITOR_LAYOUT_NETSTATUS_LINE_GAP     2.0
#define XTRACEMONITOR_LAYOUT_NETSTATUS_LED_GAP      6.0    // LED → "Conexion a internet" 
#define XTRACEMONITOR_LAYOUT_NETSTATUS_TEXT_PAD     16.0   // measure slack inside the frame (long EN/DE/FR/CJK lines)
#define XTRACEMONITOR_LAYOUT_NETSTATUS_FRAME_PAD    6.0    // inner pad LED/text ↔ frame (same chrome as valor)
#define XTRACEMONITOR_LAYOUT_TOOLBAR_LBL_MIN_W      86.0   // XML default legend width
#define XTRACEMONITOR_LAYOUT_TOOLBAR_LBL_PAD        8.0    // text ↔ legend form edges
#define XTRACEMONITOR_NETSTATUS_POLL_MS             1000   // UI refresh from GEN-cached InternetStatus
#define XTRACEMONITOR_SAT_CHART_W                   220.0  // sparkline widget width; visible samples = plot px
#define XTRACEMONITOR_SAT_CHART_PAD                 4.0    // plot inner pad + text margin from widget TL
#define XTRACEMONITOR_SAT_CHART_TEXT_SIZE           11     // same as toolbar bubble legends
#define XTRACEMONITOR_SAT_CHART_BUBBLE_GAP          9.0    // edge gap between toolbar bubbles (xpos step 81 - 72)
#define XTRACEMONITOR_SAT_CHART_AFTER_BUBBLES_GAP   (XTRACEMONITOR_SAT_CHART_BUBBLE_GAP * 4.0) // 4× inter-bubble gap (was 2×; doubled again)
#define XTRACEMONITOR_SAT_CHART_MIN_W               120.0
// Live toolbar sparkline paints on GRP2DCANVAS (not UI statisticschart): SVG rebuild + sharp
// overlay dropped XTM from ~60 to ~45 fps when updated every second.

#define XTRACEMONITOR_OPSTATUS_MAXLINES             20
#define XTRACEMONITOR_OPSTATUS_MAXARGS              2
#define XTRACEMONITOR_OPSTATUS_LINEHEIGHT           18.0f
#define XTRACEMONITOR_OPSTATUS_PAD                  4.0f
#define XTRACEMONITOR_OPSTATUS_DBLCLICK_MS          700


/**-------------------------------------------------------------------------------------------------------------------
*
* @struct     XTRACEMONITOR_OPSTATUS_ENTRY
* @brief      Status foot line: translate ID + optional Format args (rebuilt on language change).
*
*---------------------------------------------------------------------------------------------------------------------*/
struct XTRACEMONITOR_SAT_SAMPLE
{
  float                         in_rate;     // msg/s pushed into GEN queue
  float                         fed_rate;    // msg/s painted to console
  float                         drop_rate;   // msg/s dropped by bounded queue
  float                         queue_pct;   // 0..1 occupancy
};


struct XTRACEMONITOR_OPSTATUS_ENTRY
{
  XDWORD  id;
  XDWORD  nargs;
  XSTRING arg[XTRACEMONITOR_OPSTATUS_MAXARGS];

  XTRACEMONITOR_OPSTATUS_ENTRY()
  {
    id    = 0;
    nargs = 0;
  }
};

// Config modal: fixed size = 75% of the app minimum client (1200×699), always; only recentered.
#define XTRACEMONITOR_CONFIGMODAL_W               900.0  // 1200 * 0.75
#define XTRACEMONITOR_CONFIGMODAL_H               502.0  // trim from bottom (top/title fixed)
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
#define XTRACEMONITOR_CONFIGMODAL_TITLE_H         28.0
// Vertical content band shared by all modals (Config is the master):
//   CONTENT_TOP    = Color Theme EdgeTop  (theme_y 316 + theme_h 96)
//   CONTENT_BOTTOM = UDP/UART edit bottom (port_edit_y) — gap above button tops stays identical
#define XTRACEMONITOR_CONFIGMODAL_CONTENT_TOP     412.0
#define XTRACEMONITOR_CONFIGMODAL_CONTENT_BOTTOM  130.0
// Title↔content and content↔buttons gaps measured on Config (use for Filters modal height).
// title_bot = H - TITLE_PAD - TITLE_H = 458 → TITLE_TO_CONTENT = 458 - 412 = 46
// btn_top   = BTN_BOTTOM_PAD + BTN_H   = 69  → CONTENT_TO_BTNS  = 130 - 69  = 61
#define XTRACEMONITOR_CONFIGMODAL_TITLE_TO_CONTENT  (XTRACEMONITOR_CONFIGMODAL_H - XTRACEMONITOR_CONFIGMODAL_TITLE_PAD - XTRACEMONITOR_CONFIGMODAL_TITLE_H - XTRACEMONITOR_CONFIGMODAL_CONTENT_TOP)
#define XTRACEMONITOR_CONFIGMODAL_CONTENT_TO_BTNS   (XTRACEMONITOR_CONFIGMODAL_CONTENT_BOTTOM - (XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD + XTRACEMONITOR_CONFIGMODAL_BTN_H))



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
struct UI_ELEMENT_TREE_NODE;
class UI_ELEMENT_IMAGE;
class UI_ELEMENT_FORM;
class GRPXEVENT;
class XTRACEMONITOR_CFG;
class XTRACEMONITOR_SERVER;
class XTRACEMONITOR_ORIGINS;
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
    friend class                    XTRACEMONITOR_FORM_ABOUT;
    friend class                    XTRACEMONITOR_FORM_DYNDNS;
    friend class                    XTRACEMONITOR_SERVER;
    friend class                    XTRACEMONITOR_ORIGINS;
    friend class                    XTRACEMONITOR_GUI_TREE;
    friend class                    XTRACEMONITOR_GUI_CONSOLE;
    friend class                    XTRACEMONITOR_GUI_VARSTATUS;
    friend class                    XTRACEMONITOR_GEOIP;

    bool                            UpdateInput                             ();

    bool                            Ini_Graphics                            (GRPSCREEN* screen);
    bool                            Ini_Canvas                              (GRPSCREEN* screen);
    bool                            Ini_UserInterface                       (bool on);

    bool                            DrawFrame                               ();

    bool                            Layout_ApplyFlexibleResize              (bool compose = true);
    bool                            Layout_ApplyConsoleAboveOpStatus        (bool dirty = true);

    bool                            BrandMonitor_Resolve                    ();
    bool                            BrandMonitor_RepaintAfterChrome         ();

    bool                            InternetStatus_Resolve                  ();
    bool                            InternetStatus_DropStaleCaches          ();
    bool                            InternetStatus_ApplyGeometry            (double canvas_w);
    bool                            InternetStatus_Update                   (bool force = false);
    bool                            InternetStatus_WipeCluster              (double left, double top, double right, double bottom);
    bool                            InternetStatus_PaintCluster             ();
    bool                            InternetStatus_RepaintAfterChrome       ();

    bool                            SatChart_ApplyGeometry                  (double canvas_w);
    bool                            SatChart_Sample                         ();
    bool                            SatChart_Paint                          ();
    bool                            SatChart_Update                         (bool force = false);
    bool                            SatChart_RepaintAfterChrome             ();

    bool                            OpStatus_Resolve                        ();
    bool                            OpStatus_Log                            (XDWORD translate_id);
    bool                            OpStatus_Log                            (XDWORD translate_id, XCHAR* arg0);
    bool                            OpStatus_Log                            (XDWORD translate_id, XCHAR* arg0, XCHAR* arg1);
    bool                            OpStatus_FormatLine                     (const XTRACEMONITOR_OPSTATUS_ENTRY& entry, XSTRING& out);
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
    bool                            Toolbar_ApplyFilterBtnVisibility        ();
    bool                            Toolbar_SlotSetVisible                  (UI_ELEMENT* element, bool visible, XCHAR* legend_element_name = NULL);
    bool                            Toolbar_RelayoutLabels                  ();
    bool                            BrandToolbar_SetVersion                 ();
    bool                            UserInterface_AfterLanguageChange       ();
    static bool                     Filters_IsNonDefaultCommitted           ();

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
    bool                            Chrome_ApplyAndroidConstraints          ();

    void                            HandleEvent_UserInterface               (UI_XEVENT* event);
    void                            HandleEvent_Graphics                    (GRPXEVENT* event);
    void                            HandleEvent                             (XEVENT* xevent);

    void                            Clean                                   ();

    XTIMER*                         xtimer;

    INPBUTTON*                      button[XTRACEMONITOR_BUTTON_MAX];
    INPCURSOR*                      cursor;

    UI_ELEMENT_FORM*                opstatus_form;
    UI_ELEMENT_TEXTBOX*             opstatus_text;
    XTRACEMONITOR_OPSTATUS_ENTRY    opstatus_msgs[XTRACEMONITOR_OPSTATUS_MAXLINES];
    XDWORD                          opstatus_count;
    double                          opstatus_foot_y;
    double                          opstatus_last_baseline;
    double                          opstatus_line_h;
    bool                            opstatus_expanded;
    bool                            opstatus_prevpressed;
    bool                            opstatus_click_armed;
    XTIMER*                         opstatus_click_timer;

    UI_ELEMENT_IMAGE*               brand_monitor;
    // Short post-resize / theme / language / chrome window. Caption rebuild can crop the
    // logo top; idle DrawFrame must not wipe+blit the brand every frame.
    int                             brand_repair_frames;

    UI_ELEMENT_FORM*                netstatus_form;
    UI_ELEMENT_FORM*                netstatus_led;
    UI_ELEMENT_TEXT*                netstatus_state;
    UI_ELEMENT_TEXT*                netstatus_ip;
    UI_ELEMENT_TEXT*                netstatus_udp;
    XTIMER*                         netstatus_timer;
    bool                            netstatus_haveinternet;
    XSTRING                         netstatus_last_state;
    XSTRING                         netstatus_last_ip;
    XSTRING                         netstatus_last_udp;
    double                          netstatus_prev_l;
    double                          netstatus_prev_t;
    double                          netstatus_prev_r;
    double                          netstatus_prev_b;
    bool                            netstatus_prev_valid;
    // After ES→EN shrink, RebuildAllAreas can restore an old left-border column outside the new
    // AABB. Keep wiping this union for a few post-chrome frames until caches settle.
    double                          netstatus_ghost_l;
    double                          netstatus_ghost_t;
    double                          netstatus_ghost_r;
    double                          netstatus_ghost_b;
    int                             netstatus_ghost_frames;
    // Short post-resize / theme window for chrome caption rebuild vs netstatus AABB.
    // Must NOT run wipe+PaintCluster every idle DrawFrame (that alone dropped ~60→48 FPS).
    int                             netstatus_repair_frames;

    // Queue saturation sparkline (canvas): 1 Hz ring; paint shows last plot_w px (1 sample/px).
    XTRACEMONITOR_SAT_SAMPLE        sat_samples[XTRACEMONITOR_SAT_HISTORY_MAX];
    XDWORD                          sat_count;
    XDWORD                          sat_head;          // next write index in ring
    XDWORD                          sat_prev_pushed;
    XDWORD                          sat_prev_fed;
    XDWORD                          sat_prev_dropped;
    bool                            sat_have_prev;
    double                          sat_l;
    double                          sat_t;
    double                          sat_r;
    double                          sat_b;
    bool                            sat_geom_valid;
    bool                            sat_dirty;
    float                           sat_last_in;
    float                           sat_last_fed;
    float                           sat_last_drop;
    float                           sat_last_queue_pct;

    XTRACEMONITOR_FORM_CFG          form_cfg;
    XTRACEMONITOR_FORM_FILTERS      form_filters;
    XTRACEMONITOR_FORM_ABOUT        form_about;
    XTRACEMONITOR_FORM_DYNDNS       form_dyndns;
    XTRACEMONITOR_SERVER            traceserver;
    XTRACEMONITOR_ORIGINS           origins;
    XTRACEMONITOR_GUI_TREE          gui_tree;
    XTRACEMONITOR_GUI_CONSOLE       gui_console;
    XTRACEMONITOR_GUI_VARSTATUS     gui_varstatus;
    XTRACEMONITOR_GEOIP             geoip;

    int                             layout_applied_w;
    int                             layout_applied_h;
    int                             resize_recover_frames; // full wipe+redraw after maximize/extend

    bool                            workingmode;   // true = working (Stop visible); false = paused (Run visible)
    bool                            chrome_theme_dirty; // RefreshChrome pending until chrome layout exists
    bool                            exitapp;
    bool                            screen_geometry_persisted; // one-shot save before mainscreen is destroyed
    bool                            showaboutboot_pending;    // open About once after startup when CFG says so
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
