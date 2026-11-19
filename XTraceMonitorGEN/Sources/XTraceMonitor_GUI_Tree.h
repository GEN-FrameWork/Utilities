/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_Tree.h
*
* @class      XTRACEMONITOR_GUI_TREE
* @brief      Tree panel + IP-counter strip (resolve, seed, click, UART label, layout)
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
*---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// Hover dwell on a public-IP tree row before showing the geo tip.
#define XTRACEMONITOR_GEO_TOOLTIP_SHOW_DELAY_MS         1000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class  XTRACEMONITOR;
class  UI_ELEMENT;
class  UI_ELEMENT_TREE;
struct UI_ELEMENT_TREE_NODE;
class  UI_ELEMENT_FORM;
class  UI_ELEMENT_IMAGE;
class  UI_ELEMENT_TEXT;
class  UI_ELEMENT_TOOLTIP;
class  XTIMER;

class XTRACEMONITOR_GUI_TREE
{
  public:
                                    XTRACEMONITOR_GUI_TREE                  ();
    virtual                        ~XTRACEMONITOR_GUI_TREE                  ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            Stats_Resolve                           ();
    bool                            Tooltip_Resolve                         ();
    bool                            Seed                                    ();
    bool                            Detach                                  ();

    bool                            BuildUARTLabel                          (XSTRING& out);
    bool                            UpdateUARTLabel                         ();
    bool                            ClearReceivedTraces                     ();
    bool                            HandleClick                             (int screen_x, int screen_y);
    bool                            UpdateGeoTooltip                        (int screen_x, int screen_y, bool mouse_pressed_edge);
    bool                            HideGeoTooltip                          ();

    bool                            ApplyLayoutGeometry                     (double margin, double tree_w, double tree_h, double tree_bot);
    bool                            Stats_ApplyGeometry                     (double foot_y, double margin, double width);
    bool                            Stats_Refresh                           (XDWORD public_count, XDWORD local_count);

    UI_ELEMENT_TREE*                GetElement                              ();
    UI_ELEMENT_TREE_NODE*           GetUARTNode                             ();
    void                            SetUARTNode                             (UI_ELEMENT_TREE_NODE* node);
    UI_ELEMENT_FORM*                GetStatsForm                            ();

  private:

    bool                            PublicIPFromNode                        (UI_ELEMENT_TREE_NODE* node, XDWORD& publicIP);
    bool                            ShowGeoTooltipForIP                     (XDWORD publicIP, UI_ELEMENT_TREE_NODE* node, int screen_x, int screen_y);
    bool                            GeoTooltipKey                           (XDWORD publicIP, int& state, int& map_state, XSTRING& map_path);
    bool                            GeoTooltipKeyCurrent                    (XDWORD publicIP);
    bool                            GeoTooltipAnchorCurrent                 (int screen_x, int screen_y);
    bool                            GeoTooltipHits                          (UI_ELEMENT* element);
    bool                            GeoTooltipElementMeets                  (UI_ELEMENT* element, double left, double top, double right, double bottom);
    bool                            GeoTooltipDirtyCovered                  (double left, double top, double right, double bottom);
    bool                            GeoTooltipFlushClear                    ();
    bool                            GeoTooltipNeedsRepair                   ();
    bool                            WipeGeoTooltipPlate                     ();
    bool                            GeoTooltipRepairOrHold                  ();
    bool                            CursorOverTooltip                       (int screen_x, int screen_y);
    bool                            ResetGeoTooltipHover                    ();

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;

    UI_ELEMENT_TREE*                treeelement;
    UI_ELEMENT_TREE_NODE*           tree_uart_node;

    UI_ELEMENT_FORM*                treestats_form;
    UI_ELEMENT_IMAGE*               treestats_pub_icon;
    UI_ELEMENT_TEXT*                treestats_pub_txt;
    UI_ELEMENT_IMAGE*               treestats_loc_icon;
    UI_ELEMENT_TEXT*                treestats_loc_txt;

    UI_ELEMENT_TOOLTIP*             geotooltip;
    UI_ELEMENT_TREE_NODE*           geotooltip_node;
    XDWORD                          geotooltip_ip;
    XTIMER*                         geotooltip_hover_timer;
    XDWORD                          geotooltip_hover_ip;

    // Last plate actually shown. RebuildAllAreas drops the under-tip snapshot when the
    // tip is not dirty, so a later move/hide must wipe that rect itself or the old
    // plate stays on screen. redraw_latched means the next frame still has that snapshot.
    bool                            geotooltip_plate_valid;
    bool                            geotooltip_redraw_latched;
    XDWORD                          geotooltip_cache_ip;
    int                             geotooltip_cache_state;
    int                             geotooltip_cache_map_state;
    XSTRING                         geotooltip_cache_map_path;
    XDWORD                          geotooltip_cache_lang;
    int                             geotooltip_cache_x;
    int                             geotooltip_cache_y;
    int                             geotooltip_cache_screen_w;
    int                             geotooltip_cache_screen_h;

    // Rect last wiped. For a few frames after hide, the console / valor / tree are
    // forced dirty so a rebuild snapshot that captured the plate cannot put it back.
    bool                            geotooltip_clear_valid;
    int                             geotooltip_clear_frames;
    double                          geotooltip_clear_l;
    double                          geotooltip_clear_t;
    double                          geotooltip_clear_r;
    double                          geotooltip_clear_b;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
