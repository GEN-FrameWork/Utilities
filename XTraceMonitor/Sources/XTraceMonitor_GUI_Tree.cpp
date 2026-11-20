/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_Tree.cpp
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_GUI_Tree.h"

#include "GRPBitmap.h"
#include "GRPScreen.h"
#include "GRP2DCanvas.h"

#include "UI_BoundaryLine.h"
#include "UI_Element_Form.h"
#include "UI_Element_Image.h"
#include "UI_Element_Text.h"
#include "UI_Element_Tree.h"
#include "UI_Manager.h"

#include "UI_Element_Console.h"
#include "UI_Element_Tooltip.h"
#include "UI_SkinCanvas.h"

#include "XFactory.h"
#include "XTimer.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"
#include "XTraceMonitor_GeoIP.h"
#include "XTraceMonitor_Origins.h"
#include "XTraceMonitor_TranslateID.h"

#include "XTranslation.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_TREE::XTRACEMONITOR_GUI_TREE()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_TREE::XTRACEMONITOR_GUI_TREE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_TREE::~XTRACEMONITOR_GUI_TREE()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_TREE::~XTRACEMONITOR_GUI_TREE()
{
  if(geotooltip_hover_timer)
    {
      GEN_XFACTORY.DeleteTimer(geotooltip_hover_timer);
      geotooltip_hover_timer = NULL;
    }
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds this module to the application instance.
* @ingroup    UTILITIES
*
* @param[in]  owner : Application owner.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_TREE* XTRACEMONITOR_GUI_TREE::GetElement()
* @brief      Returns the resolved tree UI element.
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_TREE* : Tree element, or NULL if unresolved.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TREE* XTRACEMONITOR_GUI_TREE::GetElement()
{
  return treeelement;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_TREE_NODE* XTRACEMONITOR_GUI_TREE::GetUARTNode()
* @brief      Returns the UART origin tree node (if seeded).
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_TREE_NODE* : UART node, or NULL.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_TREE_NODE* XTRACEMONITOR_GUI_TREE::GetUARTNode()
{
  return tree_uart_node;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_TREE::SetUARTNode(UI_ELEMENT_TREE_NODE* node)
* @brief      Stores the UART origin tree node pointer.
* @ingroup    UTILITIES
*
* @param[in]  node : UART node, or NULL to clear.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_TREE::SetUARTNode(UI_ELEMENT_TREE_NODE* node)
{
  tree_uart_node = node;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_FORM* XTRACEMONITOR_GUI_TREE::GetStatsForm()
* @brief      Returns the IP-counter strip form under the tree.
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_FORM* : Stats form, or NULL if unresolved.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_FORM* XTRACEMONITOR_GUI_TREE::GetStatsForm()
{
  return treestats_form;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Resolve()
* @brief      Resolves the layout "tree" element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Resolve()
{
  treeelement = (UI_ELEMENT_TREE*)GEN_USERINTERFACE.Element_Get(NL("tree"), UI_ELEMENT_TYPE_TREE);
  if(!treeelement) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Stats_Resolve()
* @brief      Resolves the IP-counter strip under the tree (same chrome as opstatus).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Stats_Resolve()
{
  if(!owner) return false;

  treestats_form = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("treestats"), UI_ELEMENT_TYPE_FORM);
  if(!treestats_form) return false;

  treestats_pub_icon = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("treestats_pub_icon"), UI_ELEMENT_TYPE_IMAGE);
  treestats_pub_txt  = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("treestats_pub_txt"), UI_ELEMENT_TYPE_TEXT);
  treestats_loc_icon = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("treestats_loc_icon"), UI_ELEMENT_TYPE_IMAGE);
  treestats_loc_txt  = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("treestats_loc_txt"), UI_ELEMENT_TYPE_TEXT);

  if(!treestats_pub_icon || !treestats_pub_txt || !treestats_loc_icon || !treestats_loc_txt) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  double foot_y = screen ? ((double)screen->GetHeight() - XTRACEMONITOR_LAYOUT_MARGIN) : treestats_form->GetYPosition();
  Stats_ApplyGeometry(foot_y, XTRACEMONITOR_LAYOUT_MARGIN,
                      XTRACEMONITOR_LAYOUT_PANELS_X - XTRACEMONITOR_LAYOUT_MARGIN - XTRACEMONITOR_LAYOUT_GAP);

  owner->origins.RefreshIPCounters();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Seed()
* @brief      Seeds the tree with All, Local (no internet), CFG [origins], and one UART node.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Seed()
{
  if(!owner) return false;

  owner->origins.SetOwner(owner);
  owner->origins.ApplyHistoryMaxFromCFG();

  return owner->origins.SeedTree();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Detach()
* @brief      Clears resolved UI pointers (UI tear-down).
* @ingroup    UTILITIES
*
* @return     bool : true always.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Detach()
{
  HideGeoTooltip();
  treeelement        = NULL;
  tree_uart_node     = NULL;
  treestats_form     = NULL;
  treestats_pub_icon = NULL;
  treestats_pub_txt  = NULL;
  treestats_loc_icon = NULL;
  treestats_loc_txt  = NULL;
  geotooltip         = NULL;
  geotooltip_node    = NULL;
  geotooltip_ip      = 0;
  geotooltip_hover_ip = 0;
  geotooltip_plate_valid    = false;
  geotooltip_redraw_latched = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Tooltip_Resolve()
* @brief      Resolves the geo tooltip element used over public IP tree rows.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Tooltip_Resolve()
{
  geotooltip = (UI_ELEMENT_TOOLTIP*)GEN_USERINTERFACE.Element_Get(NL("tree_geo_tip"), UI_ELEMENT_TYPE_TOOLTIP);
  if(!geotooltip) return false;

  // XTM geo tip look: pastel yellow plate, no frame, black ink (keep even after theme Accept).
  if(geotooltip->GetBackgroundColor())
    {
      geotooltip->GetBackgroundColor()->SetFromString(NL("255,249,196,255"));
      geotooltip->SetBackgroundColorSet(true);
    }
  // No frame ever: width 0 (skin Draw_Tooltip paints a fill-only plate; never stroke).
  geotooltip->SetBorderWidth(0.0);
  // Same 6 px inset for text and ![IMAGE] map on all four sides (no extra image-separation).
  geotooltip->SetPadding(UI_ELEMENT_TYPE_ALIGN_LEFT , 6.0);
  geotooltip->SetPadding(UI_ELEMENT_TYPE_ALIGN_RIGHT, 6.0);
  geotooltip->SetPadding(UI_ELEMENT_TYPE_ALIGN_UP   , 6.0);
  geotooltip->SetPadding(UI_ELEMENT_TYPE_ALIGN_DOWN , 6.0);
  geotooltip->SetImageSeparation(0);
  geotooltip->SetShowDelayMs(XTRACEMONITOR_GEO_TOOLTIP_SHOW_DELAY_MS);
  if(geotooltip->GetColor())
    {
      geotooltip->GetColor()->SetRed(0);
      geotooltip->GetColor()->SetGreen(0);
      geotooltip->GetColor()->SetBlue(0);
      geotooltip->GetColor()->SetAlpha(255);
      geotooltip->GetColor()->SetValid(true);
    }

  if(!geotooltip_hover_timer)
    {
      geotooltip_hover_timer = GEN_XFACTORY.CreateTimer();
      if(!geotooltip_hover_timer) return false;
    }

  geotooltip->Hide();
  ResetGeoTooltipHover();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::BuildUARTLabel(XSTRING& out)
* @brief      Builds the single UART tree label from the active uartport vs [target] type UART.
* @ingroup    UTILITIES
*
* @param[out] out : "UART" or "UART <TargetName>" when the serial line matches a Target.
*
* @return     bool : true always (out is always filled).
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::BuildUARTLabel(XSTRING& out)
{
  out = NL("UART");

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  XSTRING* uart = cfg.GetUARTPort();
  if(!uart || uart->IsEmpty()) return true;

  const int ntargets = cfg.GetTargetCount();
  for(int i=0; i<ntargets; i++)
    {
      XTRACEMONITOR_CFG_TARGET target;

      if(!cfg.GetTarget(i, target)) continue;
      if(target.type != XTRACEMONITOR_CFG_TARGETTYPE_UART) continue;
      if(target.name.IsEmpty() || target.uartconfig.IsEmpty()) continue;

      if(uart->Compare(target.uartconfig.Get()) != 0) continue;

      out.Format(NL("UART %s"), target.name.Get());
      return true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::UpdateUARTLabel()
* @brief      Refreshes the single UART tree node label after uartport / Target changes.
* @ingroup    UTILITIES
*
* @return     bool : true if the node was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::UpdateUARTLabel()
{
  if(!owner) return false;
  return owner->origins.UpdateUARTLabel();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::ClearReceivedTraces()
* @brief      Clears the console and removes IPs that were added only because a trace arrived.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::ClearReceivedTraces()
{
  if(!owner) return false;
  return owner->origins.ClearReceivedTraces();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::HandleClick(int screen_x, int screen_y)
* @brief      Handles a mouse click on the tree (select / expand) without generic UI selection.
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Screen X of the click.
* @param[in]  screen_y : Screen Y of the click (top-down).
*
* @return     bool : true if the click was consumed by the tree; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::HandleClick(int screen_x, int screen_y)
{
  if(!owner || !treeelement) return false;
  if(GEN_USERINTERFACE.Element_ModalBlocksInteraction(treeelement)) return false;

  double local_x = 0.0f;
  double local_y = 0.0f;
  if(!treeelement->ScreenToLocal(screen_x, screen_y, local_x, local_y)) return false;

  double box_w = treeelement->GetBoundaryLine()->width;
  double box_h = treeelement->GetBoundaryLine()->height;
  if((local_x < 0.0f) || (local_x >= box_w) || (local_y < 0.0f) || (local_y >= box_h)) return false;

  // Clicking a public IP dismisses its geo tooltip (selection still proceeds).
  UI_ELEMENT_TREE_NODE* hit = treeelement->HitTestNode(local_x, local_y);
  XDWORD hit_ip = 0;
  if(hit && PublicIPFromNode(hit, hit_ip) && (hit_ip == geotooltip_ip))
    {
      HideGeoTooltip();
    }

  treeelement->ClickAt(local_x, local_y);
  owner->origins.SyncSelectionFromTree();
  GEN_USERINTERFACE.Elements_SetToRedraw(treeelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::PublicIPFromNode(UI_ELEMENT_TREE_NODE* node, XDWORD& publicIP)
* @brief      Returns the public IP for CFG / PUBLIC_PARENT tree origins.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::PublicIPFromNode(UI_ELEMENT_TREE_NODE* node, XDWORD& publicIP)
{
  publicIP = 0;
  if(!owner || !node) return false;

  XTRACEMONITOR_ORIGIN* origin = owner->origins.GetFromNode(node);
  if(!origin) return false;
  if((origin->kind != XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT) &&
     (origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG)) return false;
  if(!origin->publicIP) return false;

  publicIP = origin->publicIP;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::CursorOverTooltip(int screen_x, int screen_y)
* @brief      True if the cursor is inside the visible geo tooltip AABB.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::CursorOverTooltip(int screen_x, int screen_y)
{
  if(!geotooltip || !geotooltip->IsVisible() || !geotooltip->GetBoundaryLine()) return false;

  double left   = geotooltip->GetXPosition();
  double bottom = geotooltip->GetYPosition();
  double w      = geotooltip->GetBoundaryLine()->width;
  double h      = geotooltip->GetBoundaryLine()->height;
  double top    = bottom - h;

  return ((screen_x >= left) && (screen_x <= (left + w)) &&
          (screen_y >= top)  && (screen_y <= bottom));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::ResetGeoTooltipHover()
* @brief      Clears the hover-dwell timer state for the geo tip (does not hide an open tip).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::ResetGeoTooltipHover()
{
  geotooltip_hover_ip = 0;
  if(geotooltip_hover_timer) geotooltip_hover_timer->Reset();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::HideGeoTooltip()
* @brief      Hides the geo tooltip and clears hover tracking.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::HideGeoTooltip()
{
  geotooltip_node = NULL;
  geotooltip_ip   = 0;
  ResetGeoTooltipHover();
  if(geotooltip && geotooltip->IsVisible())
    {
      // Always wipe, even if a redraw was latched. The latched snapshot is the
      // console as it was when the plate was already on screen, so restoring it
      // paints the plate back. A fast jump into the console hits this path.
      if(geotooltip_plate_valid) WipeGeoTooltipPlate();
      geotooltip->Hide();
      if(geotooltip_clear_valid) geotooltip_clear_frames = 5;
    }
  geotooltip_plate_valid      = false;
  geotooltip_redraw_latched   = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipKey(XDWORD publicIP, int& state, int& map_state, XSTRING& map_path)
* @brief      Reads the geo/map fields that change the tooltip body. Missing entry is state -1.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipKey(XDWORD publicIP, int& state, int& map_state, XSTRING& map_path)
{
  state     = -1;
  map_state = 0;
  map_path.Empty();
  if(!owner) return false;

  XTRACEMONITOR_GEOIP_ENTRY geo;
  if(!owner->geoip.TryGet(publicIP, geo)) return true;

  state     = (int)geo.state;
  map_state = (int)geo.map_state;
  map_path  = geo.map_path;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipKeyCurrent(XDWORD publicIP)
* @brief      True when the visible plate already shows this IP, geo/map state, path and language.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipKeyCurrent(XDWORD publicIP)
{
  if(!geotooltip_plate_valid || (geotooltip_cache_ip != publicIP)) return false;

  int state = -1;
  int map_state = 0;
  XSTRING map_path;
  if(!GeoTooltipKey(publicIP, state, map_state, map_path)) return false;
  if(state != geotooltip_cache_state) return false;
  if(map_state != geotooltip_cache_map_state) return false;
  if(map_path.Compare(geotooltip_cache_map_path)) return false;
  if(GEN_XTRANSLATION.GetActual() != geotooltip_cache_lang) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipAnchorCurrent(int screen_x, int screen_y)
* @brief      True when the cursor and the screen size still match the last placement.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipAnchorCurrent(int screen_x, int screen_y)
{
  if(!geotooltip_plate_valid) return false;
  if((screen_x != geotooltip_cache_x) || (screen_y != geotooltip_cache_y)) return false;

  int sw = 0;
  int sh = 0;
  if(owner && owner->GetMainScreen())
    {
      sw = (int)owner->GetMainScreen()->GetWidth();
      sh = (int)owner->GetMainScreen()->GetHeight();
    }
  return (sw == geotooltip_cache_screen_w) && (sh == geotooltip_cache_screen_h);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipHits(UI_ELEMENT* element)
* @brief      True when the element box meets the current tooltip plate (small pad).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipHits(UI_ELEMENT* element)
{
  if(!element || !element->IsVisible() || !element->GetBoundaryLine()) return false;
  if(!geotooltip || !geotooltip->GetBoundaryLine()) return false;

  const double pad  = 4.0;
  const double ax   = geotooltip->GetXPosition() - pad;
  const double ab   = geotooltip->GetYPosition() + pad;
  const double aw   = geotooltip->GetBoundaryLine()->width  + (pad * 2.0);
  const double ah   = geotooltip->GetBoundaryLine()->height + (pad * 2.0);
  const double atop = ab - ah;

  const double bx   = element->GetXPosition();
  const double bb   = element->GetYPosition();
  const double bw   = element->GetBoundaryLine()->width;
  const double bh   = element->GetBoundaryLine()->height;
  const double btop = bb - bh;

  if(((ax + aw) <= bx) || ((bx + bw) <= ax) || (ab <= btop) || (bb <= atop)) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipElementMeets(UI_ELEMENT* element, double left, double top, double right, double bottom)
* @brief      True when the element box meets an explicit screen rect.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipElementMeets(UI_ELEMENT* element, double left, double top, double right, double bottom)
{
  if(!element || !element->IsVisible() || !element->GetBoundaryLine()) return false;

  const double bx   = element->GetXPosition();
  const double bb   = element->GetYPosition();
  const double bw   = element->GetBoundaryLine()->width;
  const double bh   = element->GetBoundaryLine()->height;
  const double btop = bb - bh;

  if((right <= bx) || ((bx + bw) <= left) || (bottom <= btop) || (bb <= top)) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipDirtyCovered(double left, double top, double right, double bottom)
* @brief      Marks every widget under a wiped plate so it repaints over leftover ink.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipDirtyCovered(double left, double top, double right, double bottom)
{
  auto dirty = [&](UI_ELEMENT* el)
    {
      if(el && (el != (UI_ELEMENT*)geotooltip) && GeoTooltipElementMeets(el, left, top, right, bottom))
        GEN_USERINTERFACE.Elements_SetToRedraw(el, true);
    };
  auto dirty_named = [&](XCHAR* name, UI_ELEMENT_TYPE type)
    {
      dirty(GEN_USERINTERFACE.Element_Get(name, type));
    };

  dirty(treeelement);
  dirty_named(NL("log_console")    , UI_ELEMENT_TYPE_CONSOLE);
  dirty_named(NL("valor")          , UI_ELEMENT_TYPE_PROPERTYLIST);
  dirty_named(NL("opstatus")       , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("stop-btn")       , UI_ELEMENT_TYPE_MULTIOPTION);
  dirty_named(NL("run-btn")        , UI_ELEMENT_TYPE_MULTIOPTION);
  dirty_named(NL("clear-btn")      , UI_ELEMENT_TYPE_MULTIOPTION);
  dirty_named(NL("delstatus-btn")  , UI_ELEMENT_TYPE_MULTIOPTION);
  dirty_named(NL("filter-btn")     , UI_ELEMENT_TYPE_MULTIOPTION);
  dirty_named(NL("brand_toolbar")  , UI_ELEMENT_TYPE_IMAGE);
  dirty_named(NL("brand-ver-lbl")  , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("runstop-lbl")    , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("clear-lbl")      , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("delstatus-lbl")  , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("filter-lbl")     , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("netstatus")      , UI_ELEMENT_TYPE_FORM);
  dirty_named(NL("netstatus_state"), UI_ELEMENT_TYPE_TEXT);
  dirty_named(NL("netstatus_ip")   , UI_ELEMENT_TYPE_TEXT);
  dirty_named(NL("netstatus_udp")  , UI_ELEMENT_TYPE_TEXT);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipFlushClear()
* @brief      For a few frames after hide, force the widgets under the old plate to redraw.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipFlushClear()
{
  if(geotooltip_clear_frames <= 0 || !geotooltip_clear_valid) return true;

  // While a new plate is up, do not redraw the console over it. The frames are
  // only there to cover a hide whose snapshot still held the old ink.
  if(!geotooltip || !geotooltip->IsVisible())
    {
      GeoTooltipDirtyCovered(geotooltip_clear_l, geotooltip_clear_t, geotooltip_clear_r, geotooltip_clear_b);
    }

  geotooltip_clear_frames--;
  if(geotooltip_clear_frames <= 0) geotooltip_clear_valid = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipNeedsRepair()
* @brief      True when chrome, the tree, or a repair window will paint over the stationary plate.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipNeedsRepair()
{
  if(!owner || !geotooltip || !geotooltip->IsVisible()) return false;

  if(owner->chrome_theme_dirty) return true;
  if(owner->resize_recover_frames > 0) return true;
  if(owner->netstatus_repair_frames > 0) return true;
  if(owner->netstatus_ghost_frames > 0) return true;
  if(owner->brand_repair_frames > 0) return true;
  if(owner->sat_repair_frames > 0) return true;

  auto dirty_hit = [&](UI_ELEMENT* el) -> bool
    {
      return (el && el->MustReDraw() && GeoTooltipHits(el));
    };
  auto dirty_named = [&](XCHAR* name, UI_ELEMENT_TYPE type) -> bool
    {
      return dirty_hit(GEN_USERINTERFACE.Element_Get(name, type));
    };

  if(dirty_hit(treeelement)) return true;
  if(dirty_named(NL("log_console")    , UI_ELEMENT_TYPE_CONSOLE)) return true;
  if(dirty_named(NL("valor")          , UI_ELEMENT_TYPE_PROPERTYLIST)) return true;
  if(dirty_named(NL("opstatus")       , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("stop-btn")       , UI_ELEMENT_TYPE_MULTIOPTION)) return true;
  if(dirty_named(NL("run-btn")        , UI_ELEMENT_TYPE_MULTIOPTION)) return true;
  if(dirty_named(NL("clear-btn")      , UI_ELEMENT_TYPE_MULTIOPTION)) return true;
  if(dirty_named(NL("delstatus-btn")  , UI_ELEMENT_TYPE_MULTIOPTION)) return true;
  if(dirty_named(NL("filter-btn")     , UI_ELEMENT_TYPE_MULTIOPTION)) return true;
  if(dirty_named(NL("brand_toolbar")  , UI_ELEMENT_TYPE_IMAGE)) return true;
  if(dirty_named(NL("brand-ver-lbl")  , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("runstop-lbl")    , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("clear-lbl")      , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("delstatus-lbl")  , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("filter-lbl")     , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("netstatus")      , UI_ELEMENT_TYPE_FORM)) return true;
  if(dirty_named(NL("netstatus_state"), UI_ELEMENT_TYPE_TEXT)) return true;
  if(dirty_named(NL("netstatus_ip")   , UI_ELEMENT_TYPE_TEXT)) return true;
  if(dirty_named(NL("netstatus_udp")  , UI_ELEMENT_TYPE_TEXT)) return true;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::WipeGeoTooltipPlate()
* @brief      Erases the current plate and dirties whatever it covered.
* @note       Always, including when a redraw was latched. A fast jump onto the console
*             leaves the console snapshot holding the plate; deleting that snapshot and
*             repainting the console is what actually clears it.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::WipeGeoTooltipPlate()
{
  if(!geotooltip || !geotooltip_plate_valid || !geotooltip->GetBoundaryLine()) return false;

  const double extra = 16.0;
  const double ox    = geotooltip->GetXPosition();
  const double oy    = geotooltip->GetYPosition();
  const double ow    = geotooltip->GetBoundaryLine()->width;
  const double oh    = geotooltip->GetBoundaryLine()->height;

  geotooltip_clear_l     = ox - extra;
  geotooltip_clear_b     = oy + extra;
  geotooltip_clear_r     = ox + ow + extra;
  geotooltip_clear_t     = geotooltip_clear_b - (oh + (extra * 2.0));
  geotooltip_clear_valid = true;

  // EraseElementRebuildArea pads another 8 px and drops every rebuild snapshot
  // that crosses the plate (console included) instead of restoring it.
  geotooltip->SetXPosition(geotooltip_clear_l);
  geotooltip->SetYPosition(geotooltip_clear_b);
  geotooltip->GetBoundaryLine()->x      = geotooltip_clear_l;
  geotooltip->GetBoundaryLine()->y      = geotooltip_clear_b;
  geotooltip->GetBoundaryLine()->width  = ow + (extra * 2.0);
  geotooltip->GetBoundaryLine()->height = oh + (extra * 2.0);

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(geotooltip);
  if(layout && layout->GetSkin() && (layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS))
    {
      ((UI_SKINCANVAS*)layout->GetSkin())->EraseElementRebuildArea(geotooltip);
    }

  geotooltip->SetXPosition(ox);
  geotooltip->SetYPosition(oy);
  geotooltip->GetBoundaryLine()->x      = ox;
  geotooltip->GetBoundaryLine()->y      = oy;
  geotooltip->GetBoundaryLine()->width  = ow;
  geotooltip->GetBoundaryLine()->height = oh;

  // The layout background is a flat color, not a bitmap, so EraseElementRebuildArea
  // drops snapshots and then returns without painting. The gutter between the tree
  // and the console is nobody's widget: the yellow plate stays there. Draw_Console
  // then samples those pixels (2 px outside the frame) and strokes them back as the
  // panel seed. Fill the plate with the page color before that sample.
  if(layout && layout->GetSkin() && (layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS))
    {
      GRP2DCANVAS* canvas = ((UI_SKINCANVAS*)layout->GetSkin())->GetCanvas();
      if(canvas)
        {
          XBYTE pr = 24;
          XBYTE pg = 28;
          XBYTE pb = 36;
          UI_COLOR* page = (layout->GetBackground()) ? layout->GetBackground()->GetColor() : NULL;
          if(page && ((page->GetRed() + page->GetGreen() + page->GetBlue()) > 8))
            {
              pr = (XBYTE)page->GetRed();
              pg = (XBYTE)page->GetGreen();
              pb = (XBYTE)page->GetBlue();
            }

          GRP2DCOLOR_RGBA8 fill(pr, pg, pb, 255);
          GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);
          canvas->SetFillColor(&fill);
          canvas->SetLineColor(&noline);
          canvas->SetLineWidth(0.0f);
          canvas->Rectangle(geotooltip_clear_l, geotooltip_clear_b, geotooltip_clear_r, geotooltip_clear_t, true);
        }
    }

  GeoTooltipDirtyCovered(geotooltip_clear_l, geotooltip_clear_t, geotooltip_clear_r, geotooltip_clear_b);

  if(owner && owner->sat_geom_valid)
    {
      const bool miss = (geotooltip_clear_r < owner->sat_l) || (geotooltip_clear_l > owner->sat_r) ||
                        (geotooltip_clear_b < owner->sat_t) || (geotooltip_clear_t > owner->sat_b);
      if(!miss)
        {
          owner->sat_dirty = true;
          if(owner->sat_repair_frames < 3) owner->sat_repair_frames = 3;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::GeoTooltipRepairOrHold()
* @brief      Keeps the plate last. Marks it dirty only when something underneath is about to paint over it.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::GeoTooltipRepairOrHold()
{
  if(!geotooltip) return false;

  GEN_USERINTERFACE.Element_PutToLastPositionLayout(geotooltip);
  if(GeoTooltipNeedsRepair())
    {
      geotooltip->SetMustReDraw(true);
      GEN_USERINTERFACE.Elements_SetToRedraw(geotooltip);
      geotooltip_redraw_latched = true;
    }
   else
    {
      geotooltip_redraw_latched = false;
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::ShowGeoTooltipForIP(XDWORD publicIP, UI_ELEMENT_TREE_NODE* node, int screen_x, int screen_y)
* @brief      Fills and places the geo tip to the right of the cursor (box hangs above the public IP).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::ShowGeoTooltipForIP(XDWORD publicIP, UI_ELEMENT_TREE_NODE* node, int screen_x, int screen_y)
{
  if(!owner || !geotooltip || !treeelement || !node) return false;

  // Same IP, geo/map, language, cursor and screen: do not rebuild the body.
  // A stationary plate is not dirty, so RebuildAllAreas drops its under-snapshot.
  // The next move or hide wipes that rect itself (see WipeGeoTooltipPlate).
  const bool key_now = GeoTooltipKeyCurrent(publicIP);
  if(key_now && geotooltip->IsVisible() && GeoTooltipAnchorCurrent(screen_x, screen_y))
    {
      geotooltip_node = node;
      geotooltip_ip   = publicIP;
      return GeoTooltipRepairOrHold();
    }

  // Move/resize always erases the previous rect. Skipping this when a redraw
  // was latched left a trail of plates on the console under a fast cursor.
  if(geotooltip_plate_valid && geotooltip->IsVisible())
    WipeGeoTooltipPlate();

  // Re-assert borderless each show (theme / style reapply must not revive a stroke).
  geotooltip->SetBorderWidth(0.0);

  if(!key_now)
    {
      XSTRING body;
      owner->geoip.FormatTooltipText(publicIP, body);
      geotooltip->SetContent(body);

      int state = -1;
      int map_state = 0;
      XSTRING map_path;
      GeoTooltipKey(publicIP, state, map_state, map_path);
      geotooltip_cache_ip        = publicIP;
      geotooltip_cache_state     = state;
      geotooltip_cache_map_state = map_state;
      geotooltip_cache_map_path  = map_path;
      geotooltip_cache_lang      = GEN_XTRANSLATION.GetActual();
    }

  // Grow the tip when a map PNG is ready; otherwise keep the compact text plate.
  // Box = content + same 6 px padding used for text/map on every edge.
  XTRACEMONITOR_GEOIP_ENTRY geo;
  const bool have_map = owner->geoip.TryGet(publicIP, geo) &&
                        (geo.map_state == XTRACEMONITOR_GEOMAP_STATE_READY) &&
                        (!geo.map_path.IsEmpty());
  const double tip_pad = 6.0;
  if(geotooltip->GetBoundaryLine())
    {
      if(have_map)
        {
          // map + blank line + IP + address + operator (~17 px/line), framed by tip_pad.
          const double text_block = 17.0 + 51.0;
          geotooltip->GetBoundaryLine()->width  = (double)XTRACEMONITOR_GEOMAP_WIDTH  + tip_pad * 2.0;
          geotooltip->GetBoundaryLine()->height = tip_pad + (double)XTRACEMONITOR_GEOMAP_HEIGHT + text_block + tip_pad;
        }
       else
        {
          geotooltip->GetBoundaryLine()->width  = 280.0;
          geotooltip->GetBoundaryLine()->height = 72.0; // IP + address + operator
        }
    }

  double tip_w = geotooltip->GetBoundaryLine() ? geotooltip->GetBoundaryLine()->width  : 280.0;
  double tip_h = geotooltip->GetBoundaryLine() ? geotooltip->GetBoundaryLine()->height : 72.0;
  if(tip_w < 1.0) tip_w = 1.0;
  if(tip_h < 1.0) tip_h = 1.0;

  // Preferred: right of cursor, EdgeBottom on the pointer so the plate hangs above the IP.
  double tip_left   = (double)screen_x + 14.0;
  double tip_bottom = (double)screen_y;

  // Layout / screen bounds (GEN EdgeBottom Y: larger Y is lower on screen).
  double layout_w = tip_left + tip_w + 8.0;
  double layout_h = tip_bottom + 8.0;
  if(owner->GetMainScreen())
    {
      if(owner->GetMainScreen()->GetWidth()  > 32) layout_w = (double)owner->GetMainScreen()->GetWidth();
      if(owner->GetMainScreen()->GetHeight() > 32) layout_h = (double)owner->GetMainScreen()->GetHeight();
    }

  // Keep the tip clear of the main toolbar / labels (Parar, Limpiar, …) that sit above the tree.
  double chrome_bottom = 0.0;
  auto raise_chrome = [&](XCHAR* name, UI_ELEMENT_TYPE type)
    {
      UI_ELEMENT* el = GEN_USERINTERFACE.Element_Get(name, type);
      if(el && el->IsVisible() && el->GetBoundaryLine())
        {
          const double bot = el->GetBoundaryLine()->y;
          if(bot > chrome_bottom) chrome_bottom = bot;
        }
    };
  raise_chrome(NL("stop-btn")       , UI_ELEMENT_TYPE_MULTIOPTION);
  raise_chrome(NL("run-btn")        , UI_ELEMENT_TYPE_MULTIOPTION);
  raise_chrome(NL("clear-btn")      , UI_ELEMENT_TYPE_MULTIOPTION);
  raise_chrome(NL("delstatus-btn")  , UI_ELEMENT_TYPE_MULTIOPTION);
  raise_chrome(NL("filter-btn")     , UI_ELEMENT_TYPE_MULTIOPTION);
  raise_chrome(NL("brand_toolbar")  , UI_ELEMENT_TYPE_IMAGE);
  raise_chrome(NL("brand-ver-lbl")  , UI_ELEMENT_TYPE_FORM);
  raise_chrome(NL("runstop-lbl")    , UI_ELEMENT_TYPE_FORM);
  raise_chrome(NL("clear-lbl")      , UI_ELEMENT_TYPE_FORM);
  raise_chrome(NL("delstatus-lbl")  , UI_ELEMENT_TYPE_FORM);
  raise_chrome(NL("filter-lbl")     , UI_ELEMENT_TYPE_FORM);
  // Netstatus bubble texts also sit in the top chrome band.
  raise_chrome(NL("netstatus")      , UI_ELEMENT_TYPE_FORM);
  raise_chrome(NL("netstatus_state"), UI_ELEMENT_TYPE_TEXT);
  raise_chrome(NL("netstatus_ip")   , UI_ELEMENT_TYPE_TEXT);
  raise_chrome(NL("netstatus_udp")  , UI_ELEMENT_TYPE_TEXT);
  if(chrome_bottom < 1.0) chrome_bottom = 130.0;

  const double gap = 8.0;
  double tip_top = tip_bottom - tip_h;
  if(tip_top < chrome_bottom + gap)
    {
      // Shift the plate down so its top clears the toolbar / bubble chrome.
      tip_bottom = chrome_bottom + gap + tip_h;
      tip_top    = tip_bottom - tip_h;
    }
  if(tip_bottom > layout_h - gap)
    {
      tip_bottom = layout_h - gap;
      tip_top    = tip_bottom - tip_h;
      // Prefer clipping at the bottom over covering the toolbar again.
      if(tip_top < chrome_bottom + gap)
        {
          tip_top    = chrome_bottom + gap;
          tip_bottom = tip_top + tip_h;
        }
    }

  // Horizontal: stay on-screen; dodge the netstatus bubble when the tip would cover it.
  if(tip_left + tip_w > layout_w - gap) tip_left = layout_w - gap - tip_w;
  if(tip_left < gap) tip_left = gap;

  UI_ELEMENT* netstatus = GEN_USERINTERFACE.Element_Get(NL("netstatus"), UI_ELEMENT_TYPE_FORM);
  if(netstatus && netstatus->IsVisible() && netstatus->GetBoundaryLine())
    {
      const double nx = netstatus->GetBoundaryLine()->x;
      const double ny = netstatus->GetBoundaryLine()->y;
      const double nw = netstatus->GetBoundaryLine()->width;
      const double nh = netstatus->GetBoundaryLine()->height;
      const double ntop = ny - nh;

      const bool overlap_x = (tip_left < nx + nw) && (tip_left + tip_w > nx);
      const bool overlap_y = (tip_top  < ny)      && (tip_bottom > ntop);
      if(overlap_x && overlap_y)
        {
          // Prefer left of the cursor; else park just left of the bubble.
          double alt = (double)screen_x - tip_w - 14.0;
          if(alt < gap) alt = nx - tip_w - gap;
          if(alt < gap) alt = gap;
          tip_left = alt;
        }
    }

  geotooltip_node = node;
  geotooltip_ip   = publicIP;
  GEN_USERINTERFACE.Element_PutToLastPositionLayout(geotooltip);
  geotooltip->SetMustReDraw(true);
  GEN_USERINTERFACE.Elements_SetToRedraw(geotooltip);

  geotooltip_plate_valid    = true;
  geotooltip_redraw_latched = true;
  geotooltip_cache_x        = screen_x;
  geotooltip_cache_y        = screen_y;
  geotooltip_cache_screen_w = 0;
  geotooltip_cache_screen_h = 0;
  if(owner->GetMainScreen())
    {
      geotooltip_cache_screen_w = (int)owner->GetMainScreen()->GetWidth();
      geotooltip_cache_screen_h = (int)owner->GetMainScreen()->GetHeight();
    }
  return geotooltip->ShowAt(tip_left, tip_bottom);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::UpdateGeoTooltip(int screen_x, int screen_y, bool mouse_pressed_edge)
* @brief      Shows geo tooltip after a 1s dwell on a public IP (or keeps it while over the tip); hides on leave.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::UpdateGeoTooltip(int screen_x, int screen_y, bool mouse_pressed_edge)
{
  if(!owner || !treeelement || !geotooltip) return false;
  GeoTooltipFlushClear();
  if(!XTRACEMONITOR_CFG::GetInstance(false).GetGeolocalizationPublicIP())
    {
      return HideGeoTooltip();
    }
  if(GEN_USERINTERFACE.Element_IsModalActive())
    {
      return HideGeoTooltip();
    }

  if(mouse_pressed_edge)
    {
      // Click handling also dismisses; keep this as a safety net.
    }

  double local_x = 0.0f;
  double local_y = 0.0f;
  bool over_tree = treeelement->ScreenToLocal(screen_x, screen_y, local_x, local_y);
  UI_ELEMENT_TREE_NODE* hit = NULL;
  XDWORD hit_ip = 0;

  if(over_tree && treeelement->GetBoundaryLine())
    {
      double box_w = treeelement->GetBoundaryLine()->width;
      double box_h = treeelement->GetBoundaryLine()->height;
      if((local_x >= 0.0f) && (local_x < box_w) && (local_y >= 0.0f) && (local_y < box_h))
        {
          hit = treeelement->HitTestNode(local_x, local_y);
          if(hit && !PublicIPFromNode(hit, hit_ip)) hit = NULL;
        }
    }

  const bool over_tip = CursorOverTooltip(screen_x, screen_y);

  if(hit && hit_ip)
    {
      // Already visible for this IP: follow cursor and refresh body when geo completes.
      if(geotooltip->IsVisible() && (geotooltip_ip == hit_ip))
        {
          return ShowGeoTooltipForIP(hit_ip, hit, screen_x, screen_y);
        }

      // New hover target (or tip not yet shown): restart the dwell clock.
      if(geotooltip_hover_ip != hit_ip)
        {
          if(geotooltip->IsVisible()) HideGeoTooltip();
          geotooltip_hover_ip = hit_ip;
          if(geotooltip_hover_timer) geotooltip_hover_timer->Reset();
        }

      XDWORD delay_ms = geotooltip->GetShowDelayMs();
      if(delay_ms < 1) delay_ms = XTRACEMONITOR_GEO_TOOLTIP_SHOW_DELAY_MS;

      if(geotooltip_hover_timer &&
         (geotooltip_hover_timer->GetMeasureMilliSeconds() >= (XQWORD)delay_ms))
        {
          return ShowGeoTooltipForIP(hit_ip, hit, screen_x, screen_y);
        }

      return true;
    }

  if(over_tip && geotooltip_ip)
    {
      // Keep the anchor. Refresh the body when geo/map arrives; otherwise only
      // repaint if chrome or the tree is about to cover the plate.
      if(geotooltip_plate_valid && !GeoTooltipKeyCurrent(geotooltip_ip))
        {
          return ShowGeoTooltipForIP(geotooltip_ip, geotooltip_node, geotooltip_cache_x, geotooltip_cache_y);
        }
      return GeoTooltipRepairOrHold();
    }

  return HideGeoTooltip();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::ApplyLayoutGeometry(double margin, double tree_w, double tree_h, double tree_bot)
* @brief      Applies flexible-resize geometry to the tree panel.
* @ingroup    UTILITIES
*
* @param[in]  margin   : Left margin.
* @param[in]  tree_w   : Tree width.
* @param[in]  tree_h   : Tree height.
* @param[in]  tree_bot : Tree bottom Y (EdgeBottom).
*
* @return     bool : true if the tree was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::ApplyLayoutGeometry(double margin, double tree_w, double tree_h, double tree_bot)
{
  if(!treeelement || !treeelement->GetBoundaryLine()) return false;

  treeelement->GetBoundaryLine()->width  = tree_w;
  treeelement->GetBoundaryLine()->height = tree_h;
  treeelement->GetBoundaryLine()->x      = margin;
  treeelement->GetBoundaryLine()->y      = tree_bot;
  treeelement->SetIntrinsicWidth(tree_w);
  treeelement->SetIntrinsicHeight(tree_h);
  treeelement->SetXPosition(margin);
  treeelement->SetYPosition(tree_bot);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Stats_ApplyGeometry(double foot_y, double margin, double width)
* @brief      Places the IP-counter strip and keeps children glued (abs = father − local boundary).
* @ingroup    UTILITIES
*
* @param[in]  foot_y : Strip bottom Y (aligned with opstatus foot).
* @param[in]  margin : Left margin.
* @param[in]  width  : Strip width (matches tree width).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Stats_ApplyGeometry(double foot_y, double margin, double width)
{
  if(!treestats_form || !treestats_form->GetBoundaryLine()) return false;

  const double treestats_h = XTRACEMONITOR_LAYOUT_TREESTATS_H;
  if(width < 1.0) width = 1.0;

  treestats_form->GetBoundaryLine()->width  = width;
  treestats_form->GetBoundaryLine()->height = treestats_h;
  treestats_form->GetBoundaryLine()->x      = margin;
  treestats_form->GetBoundaryLine()->y      = foot_y;
  treestats_form->SetIntrinsicWidth(width);
  treestats_form->SetIntrinsicHeight(treestats_h);
  treestats_form->SetXPosition(margin);
  treestats_form->SetYPosition(foot_y);

  UI_BOUNDARYLINE* vis = treestats_form->GetVisibleRect();
  if(vis)
    {
      vis->x      = margin;
      vis->y      = foot_y;
      vis->width  = width;
      vis->height = treestats_h;
    }

  // Locals: x from form left; y = father_y − abs_y (child bottom / text baseline).
  // Draw_Image: bitmap hangs UP from y_position (must be scaled to icon_s, not 24px source).
  // Draw_Text (no plate): print_y = y_position = baseline; optical mid ≈ baseline + 0.22×sizefont
  // toward EdgeTop → local_y = form_h/2 − 0.22×sf centers the ink in the strip.
  const double icon_s  = 12.0;
  const double text_h  = 14.0;
  const double pad_x   = 4.0;   // left margin inside the frame
  const double gap_it  = 4.0;
  const double gap_grp = 8.0;
  const double sf      = 11.0;
  const double icon_y  = (treestats_h - icon_s) * 0.5;
  const double text_y  = (treestats_h * 0.5) - (sf * 0.22);

  // Scale authored 24×24 tree PNGs down once so Draw_Image respects the 12×12 slot.
  auto scale_icon = [&](UI_ELEMENT_IMAGE* img)
    {
      if(!img || !img->GetImage()) return;
      GRPBITMAP* bm = img->GetImage();
      XDWORD tw = (XDWORD)icon_s;
      if(tw < 1) tw = 1;
      if((bm->GetWidth() != tw) || (bm->GetHeight() != tw)) bm->Scale((int)tw, (int)tw);
    };
  scale_icon(treestats_pub_icon);
  scale_icon(treestats_loc_icon);

  // Fixed slots (no canvas SetSize — that mutated the shared vector-font config and
  // made the console look wrong on the next measure/draw path).
  double pub_tw = 92.0;
  double loc_tw = 80.0;

  double x = pad_x;
  auto place_child = [&](UI_ELEMENT* child, double local_x, double local_y, double cw, double ch)
    {
      if(!child || !child->GetBoundaryLine()) return;
      child->GetBoundaryLine()->x      = local_x;
      child->GetBoundaryLine()->y      = local_y;
      child->GetBoundaryLine()->width  = cw;
      child->GetBoundaryLine()->height = ch;
      child->SetIntrinsicWidth(cw);
      child->SetIntrinsicHeight(ch);
      child->SetXPosition(margin + local_x);
      child->SetYPosition(foot_y - local_y);
    };

  place_child(treestats_pub_icon, x, icon_y, icon_s, icon_s);
  x += icon_s + gap_it;
  place_child(treestats_pub_txt, x, text_y, pub_tw, text_h);
  x += pub_tw + gap_grp;
  place_child(treestats_loc_icon, x, icon_y, icon_s, icon_s);
  x += icon_s + gap_it;
  place_child(treestats_loc_txt, x, text_y, loc_tw, text_h);

  // Form paint clears the strip; children only redraw when dirty (empty-card defect).
  GEN_USERINTERFACE.Elements_SetToRedraw(treestats_form, true);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_TREE::Stats_Refresh(XDWORD public_count, XDWORD local_count)
* @brief      Updates labels to "Publicas (n)" / "Locales (n)" and recenters the strip.
* @ingroup    UTILITIES
*
* @param[in]  public_count : Number of public IP origins.
* @param[in]  local_count  : Number of local IP origins.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_TREE::Stats_Refresh(XDWORD public_count, XDWORD local_count)
{
  if(!treestats_pub_txt || !treestats_loc_txt) return false;

  XSTRING pub;
  XSTRING loc;
  pub.Format(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_STATS_PUBLIC), public_count);
  loc.Format(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_STATS_LOCAL), local_count);

  treestats_pub_txt->GetText()->Set(pub.Get());
  treestats_loc_txt->GetText()->Set(loc.Get());

  double foot_y = treestats_form ? treestats_form->GetYPosition() : 0.0;
  double margin = XTRACEMONITOR_LAYOUT_MARGIN;
  double width  = XTRACEMONITOR_LAYOUT_PANELS_X - XTRACEMONITOR_LAYOUT_MARGIN - XTRACEMONITOR_LAYOUT_GAP;
  if(treestats_form && treestats_form->GetBoundaryLine() && treestats_form->GetBoundaryLine()->width > 1.0)
    width = treestats_form->GetBoundaryLine()->width;
  if(foot_y > 0.0) Stats_ApplyGeometry(foot_y, margin, width);
   else            GEN_USERINTERFACE.Elements_SetToRedraw(treestats_form, true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_TREE::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_TREE::Clean()
{
  owner              = NULL;
  treeelement        = NULL;
  tree_uart_node     = NULL;
  treestats_form     = NULL;
  treestats_pub_icon = NULL;
  treestats_pub_txt  = NULL;
  treestats_loc_icon = NULL;
  treestats_loc_txt  = NULL;
  geotooltip         = NULL;
  geotooltip_node    = NULL;
  geotooltip_ip      = 0;
  geotooltip_hover_timer = NULL;
  geotooltip_hover_ip    = 0;
  geotooltip_plate_valid      = false;
  geotooltip_redraw_latched   = false;
  geotooltip_cache_ip         = 0;
  geotooltip_cache_state      = -1;
  geotooltip_cache_map_state  = 0;
  geotooltip_cache_lang       = 0;
  geotooltip_cache_x          = 0;
  geotooltip_cache_y          = 0;
  geotooltip_cache_screen_w   = 0;
  geotooltip_cache_screen_h   = 0;
  geotooltip_clear_valid      = false;
  geotooltip_clear_frames     = 0;
  geotooltip_clear_l          = 0.0;
  geotooltip_clear_t          = 0.0;
  geotooltip_clear_r          = 0.0;
  geotooltip_clear_b          = 0.0;
}
