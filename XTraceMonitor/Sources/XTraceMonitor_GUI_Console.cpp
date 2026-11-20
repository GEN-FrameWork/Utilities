/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_Console.cpp
*
* @class      XTRACEMONITOR_GUI_CONSOLE
* @brief      Trace console panel (resolve, seed, layout)
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

#include "XTraceMonitor_GUI_Console.h"

#include "GRPScreen.h"
#include "GRP2DRebuildAreas.h"

#include "UI_BoundaryLine.h"
#include "UI_Element.h"
#include "UI_Element_Animation.h"
#include "UI_Element_Console.h"
#include "UI_Element_Option.h"
#include "UI_Layout.h"
#include "UI_Manager.h"
#include "UI_Property_Scrolleable.h"
#include "UI_Skin.h"
#include "UI_SkinCanvas.h"

#include "INPManager.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_CONSOLE::XTRACEMONITOR_GUI_CONSOLE()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_CONSOLE::XTRACEMONITOR_GUI_CONSOLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_CONSOLE::~XTRACEMONITOR_GUI_CONSOLE()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_CONSOLE::~XTRACEMONITOR_GUI_CONSOLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds this module to the application instance.
* @ingroup    UTILITIES
*
* @param[in]  owner : Application owner.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_CONSOLE* XTRACEMONITOR_GUI_CONSOLE::GetElement()
* @brief      Returns the resolved console UI element.
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_CONSOLE* : Console element, or NULL if unresolved.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_CONSOLE* XTRACEMONITOR_GUI_CONSOLE::GetElement()
{
  return consoleelement;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT* XTRACEMONITOR_GUI_CONSOLE::GetTrackButton()
* @brief      Returns the session follow button, or NULL if unresolved.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT* XTRACEMONITOR_GUI_CONSOLE::GetTrackButton()
{
  return track_btn;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::Resolve()
* @brief      Resolves the layout console element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::Resolve()
{
  consoleelement = (UI_ELEMENT_CONSOLE*)GEN_USERINTERFACE.Element_Get(NL("log_console"), UI_ELEMENT_TYPE_CONSOLE);
  if(!consoleelement) return false;

  // New element: the previous box cache must not skip the first pack of this button.
  layout_geom_valid = false;

  // Multioption root: faces keep bg alpha 0 so Draw_Option skips the white erase plate under the PNG bubble.
  track_btn = GEN_USERINTERFACE.Element_Get(NL("console_track_btn"), UI_ELEMENT_TYPE_MULTIOPTION);

  // Seed session follow from CFG (button is session-only after that).
  SyncTextTrackingFromCFG();
  // Layout maxlines is only a fallback. The live cap is Max Console Count.
  ApplyMaxLinesFromCFG();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SeedStartupLines()
* @brief      Clears the graphical console at startup (live lines arrive from the server pump).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SeedStartupLines()
{
  if(!consoleelement) return false;

  consoleelement->Clear();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::Detach()
* @brief      Clears the resolved UI pointer (UI tear-down).
* @ingroup    UTILITIES
*
* @return     bool : true always.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::Detach()
{
  consoleelement     = NULL;
  track_btn          = NULL;
  selection_dragging = false;
  selection_anchor   = 0;
  layout_geom_valid  = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::ApplyLayoutGeometry(double panel_w, double console_h, double console_bot)
* @brief      Applies flexible-resize geometry to the console panel.
*             Same box as the last call is a no-op (idle must not dirty the follow button).
* @ingroup    UTILITIES
*
* @param[in]  panel_w     : Console width.
* @param[in]  console_h   : Console height.
* @param[in]  console_bot : Console bottom Y (EdgeBottom).
*
* @return     bool : true if the console was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::ApplyLayoutGeometry(double panel_w, double console_h, double console_bot)
{
  if(!consoleelement || !consoleelement->GetBoundaryLine()) return false;

  // UpdateInput calls this every frame via Layout_ApplyConsoleAboveOpStatus. PackTrackButton
  // and SyncTrackButton set MustReDraw on the follow button even when the slot did not move.
  if(layout_geom_valid &&
     (layout_geom_w == panel_w) &&
     (layout_geom_h == console_h) &&
     (layout_geom_bot == console_bot))
    {
      return true;
    }

  consoleelement->GetBoundaryLine()->width  = panel_w;
  consoleelement->GetBoundaryLine()->height = console_h;
  consoleelement->GetBoundaryLine()->y      = console_bot;
  consoleelement->SetIntrinsicWidth(panel_w);
  consoleelement->SetIntrinsicHeight(console_h);
  consoleelement->SetYPosition(console_bot);

  // Overlay follow button at the console top-right (session toggle; not CFG).
  // Keep clear of the overlay scrollbar gutter so Option/Anim rebuilds cannot bake the thumb.
  if(track_btn && track_btn->GetBoundaryLine())
    {
      const double btn_sz = 40.0;
      const double pad    = 2.0;
      const double gutter = (double)UI_PROPERTY_SCROLLEABLE_BARWIDTH + 4.0;
      double       left   = consoleelement->GetXPosition();
      double       top    = UI_BOUNDARYLINE_EdgeTop(console_bot, console_h);
      double       btn_x  = left + panel_w - pad - gutter - btn_sz;
      double       btn_y  = top + pad + btn_sz; // EdgeBottom

      track_btn->GetBoundaryLine()->x      = btn_x;
      track_btn->GetBoundaryLine()->y      = btn_y;
      track_btn->GetBoundaryLine()->width  = btn_sz;
      track_btn->GetBoundaryLine()->height = btn_sz;
      track_btn->SetIntrinsicWidth(btn_sz);
      track_btn->SetIntrinsicHeight(btn_sz);
      track_btn->SetXPosition(btn_x);
      track_btn->SetYPosition(btn_y);
      track_btn->SetVisible(true);
      track_btn->SetZLevel(consoleelement->GetZLevel() + 1);

      UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(track_btn);
      UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
      if(skin) skin->CalculeBoundaryLine_AllElements(track_btn, true);

      PackTrackButton();
      SyncTrackButton();
      GEN_USERINTERFACE.Element_PutToLastPositionLayout(track_btn);
    }

  layout_geom_valid = true;
  layout_geom_w     = panel_w;
  layout_geom_h     = console_h;
  layout_geom_bot   = console_bot;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SetToRedraw()
* @brief      Marks the console element dirty for the next paint.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SetToRedraw()
{
  if(!consoleelement) return false;
  GEN_USERINTERFACE.Elements_SetToRedraw(consoleelement);
  // Draw_Console fills its rect; keep the overlay in the same dirty pass, after the console.
  RefreshTrackOverlay();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::RefreshTrackOverlay()
* @brief      Re-arms the session follow button above the console fill for the next paint.
* @ingroup    UTILITIES
*
* @return     bool : true if the button exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::RefreshTrackOverlay()
{
  if(!track_btn || !track_btn->GetBoundaryLine()) return false;

  const double x = track_btn->GetXPosition();
  const double y = track_btn->GetYPosition();
  const double w = track_btn->GetBoundaryLine()->width;
  const double h = track_btn->GetBoundaryLine()->height;

  // Live traces dirty the console every frame. The slot does not move, and
  // Draw_MultiOption already drops Option/Anim plates when this button sits on
  // the console. Packing every face and walking the caches again is the repair
  // that showed up in the frame. Re-dirtying the button is enough: the console
  // fill wipes the glyph, and the button is already after the console in the list.
  if(overlay_repair_valid &&
     (overlay_repair_x == x) && (overlay_repair_y == y) &&
     (overlay_repair_w == w) && (overlay_repair_h == h))
    {
      track_btn->SetMustReDraw(true);
      return true;
    }

  // Console fill + DrawScrollBars already own the pixels under this overlay. Drop any cached
  // Option/Anim/rebuild plates WITHOUT restoring — a PutBitmapNoAlpha of a prior thumb capture
  // is what produced the double scrollbar fragments.
  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(track_btn);
  UI_SKINCANVAS* skin = (layout && layout->GetSkin() &&
                         (layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS))
                        ? (UI_SKINCANVAS*)layout->GetSkin() : NULL;
  if(skin)
    {
      GRP2DREBUILDAREA* area = skin->GetRebuildAreaByElement(track_btn);
      if(area)
        {
          XVECTOR<GRP2DREBUILDAREA*>* areas = skin->GetRebuildAreas();
          if(areas) areas->Delete(area);
          GEN_DELETE area;
        }

      if(track_btn->GetComposeElements())
        {
          for(XDWORD c=0; c<track_btn->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* face = track_btn->GetComposeElements()->Get(c);
              if(!face) continue;
              skin->DiscardOptionBackdrop(face);
              if(!face->GetComposeElements()) continue;
              for(XDWORD d=0; d<face->GetComposeElements()->GetSize(); d++)
                {
                  UI_ELEMENT* kid = face->GetComposeElements()->Get(d);
                  if(!kid) continue;
                  if(kid->GetType() == UI_ELEMENT_TYPE_ANIMATION) skin->DiscardAnimBackdrop(kid);
                  if(kid->GetType() == UI_ELEMENT_TYPE_TEXT)      skin->DiscardTextBackdrop(kid);
                }
            }
        }
    }

  PackTrackButton();
  GEN_USERINTERFACE.Element_PutToLastPositionLayout(track_btn);
  GEN_USERINTERFACE.Elements_SetToRedraw(track_btn, true);

  overlay_repair_valid = true;
  overlay_repair_x     = track_btn->GetXPosition();
  overlay_repair_y     = track_btn->GetYPosition();
  overlay_repair_w     = track_btn->GetBoundaryLine()->width;
  overlay_repair_h     = track_btn->GetBoundaryLine()->height;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::ApplyMaxLinesFromCFG()
* @brief      Sets the graphical console cap to Max Console Count.
*             Follow on and follow off share that value. The widget default (8000 off)
*             must not stay in front of a higher configured count.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::ApplyMaxLinesFromCFG()
{
  if(!consoleelement) return false;

  int configured = XTRACEMONITOR_CFG::GetInstance(false).GetMaxLinesConsole();
  if(configured < 1) configured = 1;

  XDWORD cap = (XDWORD)configured;
  consoleelement->SetMaxLines(cap);
  consoleelement->SetMaxLinesOff(cap);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::ApplyTextTracking()
* @brief      Applies the session follow flag onto console autoscroll (does not touch CFG).
*             No-op when autoscroll and the follow glyph already match the session flag.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::ApplyTextTracking()
{
  if(!consoleelement) return false;

  const int frame = tracking_session ? 1 : 0;
  bool glyphs_match = true;

  if(track_btn && track_btn->GetComposeElements())
    {
      for(XDWORD c=0; c<track_btn->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* face = track_btn->GetComposeElements()->Get(c);
          if(!face) continue;
          if((face->GetType() != UI_ELEMENT_TYPE_BUTTON) && (face->GetType() != UI_ELEMENT_TYPE_OPTION)) continue;

          UI_ELEMENT_OPTION*    opt  = (UI_ELEMENT_OPTION*)face;
          UI_ELEMENT_ANIMATION* anim = opt->Get_UIAnimation();
          if(!anim || (anim->IndexImage_Get() != frame))
            {
              glyphs_match = false;
              break;
            }
        }
    }

  // Replay and resolve call this again. Skip when the console and the glyph
  // already show this session flag, so the follow button is not marked dirty.
  if((consoleelement->GetAutoscroll() == tracking_session) && glyphs_match) return true;

  // OFF: keep displacement (user/manual scroll). ON: pin to live tail each paint.
  consoleelement->SetAutoscroll(tracking_session);
  SyncTrackButton();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SyncTextTrackingFromCFG()
* @brief      Reseeds the session follow flag from CFG texttracking, then applies it.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SyncTextTrackingFromCFG()
{
  tracking_session = XTRACEMONITOR_CFG::GetInstance(false).GetTextTracking();
  return ApplyTextTracking();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::HitTest(int screen_x, int screen_y)
* @brief      Returns whether a screen point hits a console text line.
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Screen X coordinate.
* @param[in]  screen_y : Screen Y coordinate.
*
* @return     bool : true if a line was hit; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::HitTest(int screen_x, int screen_y)
{
  if(!consoleelement) return false;
  if(HitTestTrackButton(screen_x, screen_y)) return false;

  XDWORD lineindex = 0;
  return consoleelement->HitTestLine(screen_x, screen_y, lineindex);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::HitTestTrackButton(int screen_x, int screen_y)
* @brief      Returns whether a screen point hits the session follow button.
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Screen X coordinate.
* @param[in]  screen_y : Screen Y coordinate.
*
* @return     bool : true if the button was hit; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::HitTestTrackButton(int screen_x, int screen_y)
{
  if(!track_btn || !track_btn->IsVisible() || !track_btn->GetBoundaryLine()) return false;

  double x   = track_btn->GetXPosition();
  double y   = track_btn->GetYPosition();
  double w   = track_btn->GetBoundaryLine()->width;
  double h   = track_btn->GetBoundaryLine()->height;
  double top = UI_BOUNDARYLINE_EdgeTop(y, h);

  if((double)screen_x < x || (double)screen_x >= (x + w)) return false;
  if((double)screen_y < top || (double)screen_y >= y) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::PointerOverTrackButton()
* @brief      True when the live pointer is inside the follow button.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     bool : true if the pointer hits the button.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::PointerOverTrackButton()
{
  if(!owner) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  if(!screen) return false;

  INPDEVICE* mouse = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(!mouse) mouse = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_TOUCHSCREEN);
  if(!mouse) return false;

  INPCURSOR* pointer = mouse->GetCursor(0);
  if(!pointer) return false;

  const int x = (int)pointer->GetX();
  const int y = (int)screen->GetHeight() - (int)pointer->GetY();
  return HitTestTrackButton(x, y);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_CONSOLE::ClearTrackButtonHover()
* @brief      Drops preselect on the follow button and every face.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_CONSOLE::ClearTrackButtonHover()
{
  if(!track_btn) return;

  track_btn->SetPreSelect(false);

  // The multioption is selectable. Once it is selected, a later hover falls through to the
  // faces. Leaving the button does not visit those faces, so a face can stay preselected and
  // the next click anywhere in the window selects it again.
  XVECTOR<UI_ELEMENT*> pending;
  pending.Add(track_btn);
  for(XDWORD i=0; i<pending.GetSize(); i++)
    {
      UI_ELEMENT* element = pending.Get(i);
      if(!element || !element->GetComposeElements()) continue;

      for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* child = element->GetComposeElements()->Get(c);
          if(!child) continue;
          child->SetPreSelect(false);
          pending.Add(child);
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_CONSOLE::ArmTrackButtonSelection(int screen_x, int screen_y)
* @brief      Keeps a follow-button click, and clears a stale face hover for every other click.
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Screen X of the click.
* @param[in]  screen_y : Screen Y of the click.
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_CONSOLE::ArmTrackButtonSelection(int screen_x, int screen_y)
{
  if(HitTestTrackButton(screen_x, screen_y)) return;
  ClearTrackButtonHover();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::HandleElementSelected(UI_ELEMENT* element)
* @brief      Toggles session follow when the click is on the follow button (no CFG write).
* @ingroup    UTILITIES
*
* @param[in]  element : Selected UI element.
*
* @return     bool : true if the element was handled; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::HandleElementSelected(UI_ELEMENT* element)
{
  if(!element || !element->GetName() || !track_btn) return false;
  if(element->GetName()->Compare(NL("console_track_btn"), true)) return false;

  // A face can still be the selected element after the pointer has left the button.
  // Only a click that lands on the button changes the session flag.
  if(!PointerOverTrackButton()) return false;

  tracking_session = !tracking_session;
  ApplyTextTracking();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::HandleSelection(int screen_x, int screen_y, bool extend, bool dragging)
* @brief      Updates console line selection from a click or drag (range of lines).
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Screen X coordinate.
* @param[in]  screen_y : Screen Y coordinate.
* @param[in]  extend : True to extend from the current anchor (Shift or active drag).
* @param[in]  dragging : True while the mouse is held and dragging.
*
* @return     bool : true if the console consumed the gesture; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::HandleSelection(int screen_x, int screen_y, bool extend, bool dragging)
{
  if(!consoleelement) return false;
  if(HitTestTrackButton(screen_x, screen_y)) return false;

  XDWORD lineindex = 0;
  if(!consoleelement->HitTestLine(screen_x, screen_y, lineindex)) return false;

  if(extend || dragging)
    {
      if(!consoleelement->Selection_Has() && !dragging) selection_anchor = lineindex;
      if(dragging && !selection_dragging) selection_anchor = lineindex;
      consoleelement->Selection_Set(selection_anchor, lineindex);
    }
   else
    {
      selection_anchor = lineindex;
      consoleelement->Selection_Set(lineindex, lineindex);
    }

  selection_dragging = dragging;
  GEN_USERINTERFACE.Elements_SetToRedraw(consoleelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_CONSOLE::EndSelectionDrag()
* @brief      Clears the in-progress console selection drag state.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_CONSOLE::EndSelectionDrag()
{
  selection_dragging = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::CopySelection()
* @brief      Copies the selected console lines (or the whole buffer if none) to the clipboard.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::CopySelection()
{
  if(!consoleelement) return false;
  return consoleelement->Selection_CopyToClipboard();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SelectAll()
* @brief      Selects every line in the console buffer.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SelectAll()
{
  if(!consoleelement) return false;

  consoleelement->Selection_SelectAll();
  GEN_USERINTERFACE.Elements_SetToRedraw(consoleelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::PackTrackButton()
* @brief      Pins every multioption face glyph into the overlay slot.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     bool : true if the button was packed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::PackTrackButton()
{
  if(!track_btn || !track_btn->GetBoundaryLine() || !track_btn->GetComposeElements()) return false;

  const double glyph = track_btn->GetBoundaryLine()->width;
  const double fx    = track_btn->GetXPosition();
  const double fy    = track_btn->GetYPosition();
  if(glyph < 1.0) return false;

  for(XDWORD c=0; c<track_btn->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* face = track_btn->GetComposeElements()->Get(c);
      if(!face || !face->GetBoundaryLine()) continue;
      if((face->GetType() != UI_ELEMENT_TYPE_BUTTON) && (face->GetType() != UI_ELEMENT_TYPE_OPTION)) continue;

      face->GetBoundaryLine()->x      = 0.0;
      face->GetBoundaryLine()->y      = 0.0;
      face->GetBoundaryLine()->width  = glyph;
      face->GetBoundaryLine()->height = glyph;
      face->SetXPosition(fx);
      face->SetYPosition(fy);

      UI_ELEMENT_OPTION*    opt  = (UI_ELEMENT_OPTION*)face;
      UI_ELEMENT_ANIMATION* anim = opt->Get_UIAnimation();
      if(!anim || !anim->GetBoundaryLine()) continue;

      anim->GetBoundaryLine()->x      = 0.0;
      anim->GetBoundaryLine()->y      = 0.0;
      anim->GetBoundaryLine()->width  = glyph;
      anim->GetBoundaryLine()->height = glyph;
      anim->SetXPosition(fx);
      anim->SetYPosition(fy);

      if(anim->GetComposeElements())
        {
          for(XDWORD i=0; i<anim->GetComposeElements()->GetSize(); i++)
            {
              UI_ELEMENT* img = anim->GetComposeElements()->Get(i);
              if(!img || !img->GetBoundaryLine()) continue;
              img->GetBoundaryLine()->x      = 0.0;
              img->GetBoundaryLine()->y      = 0.0;
              img->GetBoundaryLine()->width  = glyph;
              img->GetBoundaryLine()->height = glyph;
              img->SetXPosition(fx);
              img->SetYPosition(fy);
              img->SetMustReDraw(true);
            }
        }

      anim->SetMustReDraw(true);
      face->SetMustReDraw(true);
    }

  track_btn->SetMustReDraw(true);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SyncTrackButton()
* @brief      Mirrors the session follow flag onto every face glyph (0=off, 1=on).
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     bool : true if the button was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SyncTrackButton()
{
  if(!track_btn || !track_btn->GetComposeElements()) return false;

  const XDWORD frame = tracking_session ? 1 : 0;

  for(XDWORD c=0; c<track_btn->GetComposeElements()->GetSize(); c++)
    {
      UI_ELEMENT* face = track_btn->GetComposeElements()->Get(c);
      if(!face) continue;
      if((face->GetType() != UI_ELEMENT_TYPE_BUTTON) && (face->GetType() != UI_ELEMENT_TYPE_OPTION)) continue;

      UI_ELEMENT_OPTION*    opt  = (UI_ELEMENT_OPTION*)face;
      UI_ELEMENT_ANIMATION* anim = opt->Get_UIAnimation();
      if(anim) anim->IndexImage_Set(frame);
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(track_btn, true);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_CONSOLE::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_CONSOLE::Clean()
{
  owner              = NULL;
  consoleelement     = NULL;
  track_btn          = NULL;
  tracking_session   = false;
  selection_dragging = false;
  selection_anchor   = 0;
  layout_geom_valid    = false;
  layout_geom_w        = 0.0;
  layout_geom_h        = 0.0;
  layout_geom_bot      = 0.0;
  overlay_repair_valid = false;
  overlay_repair_x     = 0.0;
  overlay_repair_y     = 0.0;
  overlay_repair_w     = 0.0;
  overlay_repair_h     = 0.0;
}
