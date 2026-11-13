/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_Filters.cpp
*
* @class      XTRACEMONITOR_FORM_FILTERS
* @brief      Filters modal: text filter + level bitmask; persists under CFG [filters]
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

#include "XTraceMonitor_Form_Filters.h"

#include "GRPScreen.h"

#include "UI_Element.h"
#include "UI_Element_Form.h"
#include "UI_Element_Option.h"
#include "UI_Element_Text.h"
#include "UI_Element_Animation.h"
#include "UI_Element_CheckBox.h"
#include "UI_Element_EditText.h"
#include "UI_Element_RadioGroup.h"
#include "UI_Property_Selectable.h"
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "UI_Manager.h"

#include "XFactory.h"
#include "XTimer.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_FILTERS::XTRACEMONITOR_FORM_FILTERS()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_FILTERS::XTRACEMONITOR_FORM_FILTERS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_FILTERS::~XTRACEMONITOR_FORM_FILTERS()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_FILTERS::~XTRACEMONITOR_FORM_FILTERS()
{
  if(close_timer)
    {
      GEN_XFACTORY.DeleteTimer(close_timer);
      close_timer = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds the form helper to the application instance (screen / OpStatus).
* @ingroup    UTILITIES
*
* @param[in]  owner : Application instance.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (this->owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_FORM_FILTERS::LevelMaskForIndex(int index)
* @brief      Maps UI level row 0..5 (Black,Blue,Green,Purple,Red,Gray) to CFG bitmask.
* @ingroup    UTILITIES
*
* @param[in]  index : Level row index (0..5).
*
* @return     int : Bit mask for that level; 0 if index is out of range.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_FORM_FILTERS::LevelMaskForIndex(int index)
{
  switch(index)
    {
      case 0: return XTRACEMONITOR_CFG_FILTER_LEVEL_BLACK;
      case 1: return XTRACEMONITOR_CFG_FILTER_LEVEL_BLUE;
      case 2: return XTRACEMONITOR_CFG_FILTER_LEVEL_GREEN;
      case 3: return XTRACEMONITOR_CFG_FILTER_LEVEL_PURPLE;
      case 4: return XTRACEMONITOR_CFG_FILTER_LEVEL_RED;
      case 5: return XTRACEMONITOR_CFG_FILTER_LEVEL_GRAY;
      default: return 0;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::LevelCheckGet(int index)
* @brief      Reads a level checkbox status.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::LevelCheckGet(int index)
{
  if((index < 0) || (index > 5) || !chk_level[index]) return false;
  return chk_level[index]->GetStatus();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_FILTERS::LevelCheckSet(int index, bool on)
* @brief      Sets a level checkbox status and marks it dirty.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_FILTERS::LevelCheckSet(int index, bool on)
{
  if((index < 0) || (index > 5) || !chk_level[index]) return;
  chk_level[index]->SetStatus(on);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_level[index]);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::Resolve()
* @brief      Resolves filters modal elements and loads committed filter draft from CFG.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::Resolve()
{
  filters_modal = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("filters_modal"), UI_ELEMENT_TYPE_FORM);
  if(!filters_modal) return false;

  filters_modal_title       = GEN_USERINTERFACE.Element_Get(NL("filters_modal_title")      , UI_ELEMENT_TYPE_TEXT);
  filters_modal_btn_accept  = GEN_USERINTERFACE.Element_Get(NL("filters_modal_btn_accept") , UI_ELEMENT_TYPE_MULTIOPTION);
  filters_modal_btn_cancel  = GEN_USERINTERFACE.Element_Get(NL("filters_modal_btn_cancel") , UI_ELEMENT_TYPE_MULTIOPTION);
  filters_modal_btn_default = GEN_USERINTERFACE.Element_Get(NL("filters_modal_btn_default"), UI_ELEMENT_TYPE_MULTIOPTION);

  text_filter_group     = (UI_ELEMENT_RADIOGROUP*)GEN_USERINTERFACE.Element_Get(NL("text_filter_group")  , UI_ELEMENT_TYPE_RADIOGROUP);
  chk_textfilter_active = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_textfilter_active"), UI_ELEMENT_TYPE_CHECKBOX);
  edit_textfilter       = (UI_ELEMENT_EDITTEXT*)GEN_USERINTERFACE.Element_Get(NL("edit_textfilter")      , UI_ELEMENT_TYPE_EDITTEXT);
  level_filter_group    = (UI_ELEMENT_RADIOGROUP*)GEN_USERINTERFACE.Element_Get(NL("level_filter_group") , UI_ELEMENT_TYPE_RADIOGROUP);

  chk_level[0] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_0"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_level[1] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_1"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_level[2] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_2"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_level[3] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_3"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_level[4] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_4"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_level[5] = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_level_5"), UI_ELEMENT_TYPE_CHECKBOX);

  if(!filters_modal_title || !filters_modal_btn_accept || !filters_modal_btn_cancel || !filters_modal_btn_default)
    {
      return false;
    }
  if(!text_filter_group || !chk_textfilter_active || !edit_textfilter || !level_filter_group)
    {
      return false;
    }
  for(int i=0; i<6; i++)
    {
      if(!chk_level[i]) return false;
    }

  // Secondary layout inherits the skin page fill; keep it invalid so Layout_PutBackground
  // never canvas-Clears through this layout (would erase the main form under the dialog).
  UI_LAYOUT* modal_layout = GEN_USERINTERFACE.Layouts_Get(NL("filters_modal"));
  if(modal_layout && modal_layout->GetBackground() && modal_layout->GetBackground()->GetColor())
    {
      modal_layout->GetBackground()->GetColor()->SetValid(false);
    }

  if(!LoadCommittedFromCFG()) return false;
  if(!SyncControlsFromDraft()) return false;

  filters_modal->SetVisible(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::ApplyGeometry()
* @brief      Keeps the filters modal at the same fixed design size as Config and centers it.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::ApplyGeometry()
{
  if(!owner || !filters_modal) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  if(!screen) return false;

  double screen_w = (double)screen->GetWidth();
  double screen_h = (double)screen->GetHeight();
  if((screen_w < 1.0) || (screen_h < 1.0)) return false;

  // Frame height first — Filters modal H is sized so title→frames and frames→buttons
  // match Config's TITLE_TO_CONTENT / CONTENT_TO_BTNS (Config is the master).
  const double pad            = XTRACEMONITOR_CONFIGMODAL_PAD;
  const double chk_h          = 28.0;
  const double group_pad      = 12.0; // same as radiogroup.radio-group CSS padding L/R
  const double group_title_h  = 8.0;
  const double frames_gap     = 24.0;
  const double level_col_gap  = 16.0;
  const double level_row_gap  = 8.0;
  const double level_rows_h   = (chk_h * 3.0) + (level_row_gap * 2.0);
  const double frame_h        = group_pad + group_title_h + level_rows_h + group_pad;

  const double modal_w = XTRACEMONITOR_CONFIGMODAL_W;
  const double modal_h = XTRACEMONITOR_CONFIGMODAL_TITLE_PAD
                       + XTRACEMONITOR_CONFIGMODAL_TITLE_H
                       + XTRACEMONITOR_CONFIGMODAL_TITLE_TO_CONTENT
                       + frame_h
                       + XTRACEMONITOR_CONFIGMODAL_CONTENT_TO_BTNS
                       + XTRACEMONITOR_CONFIGMODAL_BTN_H
                       + XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD;

  double modal_x = (screen_w - modal_w) * 0.5;
  double modal_y = (screen_h + modal_h) * 0.5;

  filters_modal->SetXPosition(modal_x);
  filters_modal->SetYPosition(modal_y);
  filters_modal->GetBoundaryLine()->x      = modal_x;
  filters_modal->GetBoundaryLine()->y      = modal_y;
  filters_modal->GetBoundaryLine()->width  = modal_w;
  filters_modal->GetBoundaryLine()->height = modal_h;
  filters_modal->SetIntrinsicWidth(modal_w);
  filters_modal->SetIntrinsicHeight(modal_h);

  UI_BOUNDARYLINE* vis = filters_modal->GetVisibleRect();
  if(vis)
    {
      vis->x      = modal_x;
      vis->y      = modal_y;
      vis->width  = modal_w;
      vis->height = modal_h;
    }

  const double btn_w = XTRACEMONITOR_CONFIGMODAL_BTN_W;
  const double btn_h = XTRACEMONITOR_CONFIGMODAL_BTN_H;
  const double gap   = XTRACEMONITOR_CONFIGMODAL_BTN_GAP;
  const double row_w = (btn_w * 3.0) + (gap * 2.0);
  const double btn_x = (modal_w - row_w) * 0.5;
  const double btn_local_y = XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD;

  auto place_child = [](UI_ELEMENT* el, double local_x, double local_y, double w, double h)
    {
      if(!el || !el->GetBoundaryLine()) return;
      el->GetBoundaryLine()->x      = local_x;
      el->GetBoundaryLine()->y      = local_y;
      el->GetBoundaryLine()->width  = w;
      el->GetBoundaryLine()->height = h;
      el->SetIntrinsicWidth(w);
      el->SetIntrinsicHeight(h);

      UI_ELEMENT* father = el->GetFather();
      double      abs_x  = local_x;
      double      abs_y  = local_y;
      if(father)
        {
          abs_x = father->GetXPosition() + local_x;
          abs_y = father->GetYPosition() - local_y;
        }
      el->SetXPosition(abs_x);
      el->SetYPosition(abs_y);
    };

  if(filters_modal_title)
    {
      const double title_pad = XTRACEMONITOR_CONFIGMODAL_TITLE_PAD;
      const double title_h   = XTRACEMONITOR_CONFIGMODAL_TITLE_H;
      const double title_local_y = modal_h - title_pad - title_h;
      place_child(filters_modal_title, title_pad, title_local_y, modal_w - (title_pad * 2.0), title_h);
    }

  place_child(filters_modal_btn_default, btn_x,                       btn_local_y, btn_w, btn_h);
  place_child(filters_modal_btn_accept , btn_x + btn_w + gap,         btn_local_y, btn_w, btn_h);
  place_child(filters_modal_btn_cancel , btn_x + (btn_w + gap) * 2.0, btn_local_y, btn_w, btn_h);

  // Side-by-side frames: top = title_bot - TITLE_TO_CONTENT, bottom leaves CONTENT_TO_BTNS
  // above the footer buttons (same gaps as Configuración).
  const double content_w     = modal_w - (pad * 2.0);
  const double level_inner_w = content_w * 0.55;
  const double text_w        = content_w - frames_gap - level_inner_w;
  const double level_w       = level_inner_w;
  const double text_chk_w    = text_w - (group_pad * 2.0);
  const double text_edit_w   = text_chk_w;
  const double level_col_w   = (level_w - (group_pad * 2.0) - level_col_gap) * 0.5;

  const double frame_y = XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD
                       + XTRACEMONITOR_CONFIGMODAL_BTN_H
                       + XTRACEMONITOR_CONFIGMODAL_CONTENT_TO_BTNS;
  const double text_x  = pad;
  const double level_x = text_x + text_w + frames_gap;

  place_child((UI_ELEMENT*)text_filter_group, text_x, frame_y, text_w, frame_h);

  const double text_field_x = text_x + group_pad;
  // Enable text filter shares Black's row; edit shares Blue's row, nudged 6px down.
  const double text_enable_y = frame_y + frame_h - group_pad - group_title_h - chk_h;
  const double text_blue_y   = text_enable_y - (chk_h + level_row_gap);
  const double text_edit_y   = text_blue_y - 8.0;
  place_child((UI_ELEMENT*)chk_textfilter_active, text_field_x, text_enable_y, text_chk_w, chk_h);
  place_child((UI_ELEMENT*)edit_textfilter, text_field_x, text_edit_y, text_edit_w, chk_h);

  place_child((UI_ELEMENT*)level_filter_group, level_x, frame_y, level_w, frame_h);

  const double col0_x = level_x + group_pad;
  const double col1_x = col0_x + level_col_w + level_col_gap;
  double       row_y  = frame_y + frame_h - group_pad - group_title_h - chk_h;

  place_child((UI_ELEMENT*)chk_level[0], col0_x, row_y, level_col_w, chk_h);
  place_child((UI_ELEMENT*)chk_level[3], col1_x, row_y, level_col_w, chk_h);
  row_y -= (chk_h + level_row_gap);
  place_child((UI_ELEMENT*)chk_level[1], col0_x, row_y, level_col_w, chk_h);
  place_child((UI_ELEMENT*)chk_level[4], col1_x, row_y, level_col_w, chk_h);
  row_y -= (chk_h + level_row_gap);
  place_child((UI_ELEMENT*)chk_level[2], col0_x, row_y, level_col_w, chk_h);
  place_child((UI_ELEMENT*)chk_level[5], col1_x, row_y, level_col_w, chk_h);

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(filters_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(filters_modal_btn_default) skin->CalculeBoundaryLine_AllElements(filters_modal_btn_default, true);
      if(filters_modal_btn_accept)  skin->CalculeBoundaryLine_AllElements(filters_modal_btn_accept , true);
      if(filters_modal_btn_cancel)  skin->CalculeBoundaryLine_AllElements(filters_modal_btn_cancel , true);
      if(text_filter_group)         skin->CalculeBoundaryLine_AllElements(text_filter_group, true);
      if(chk_textfilter_active)     skin->CalculeBoundaryLine_AllElements(chk_textfilter_active, true);
      if(edit_textfilter)           skin->CalculeBoundaryLine_AllElements(edit_textfilter, true);
      if(level_filter_group)        skin->CalculeBoundaryLine_AllElements(level_filter_group, true);
      for(int i=0; i<6; i++)
        {
          if(chk_level[i]) skin->CalculeBoundaryLine_AllElements(chk_level[i], true);
        }

      // Radiogroup shrink-wrap can undo authored size — re-pin frames + inner controls.
      place_child((UI_ELEMENT*)text_filter_group, text_x, frame_y, text_w, frame_h);
      place_child((UI_ELEMENT*)chk_textfilter_active, text_field_x, text_enable_y, text_chk_w, chk_h);
      place_child((UI_ELEMENT*)edit_textfilter, text_field_x, text_edit_y, text_edit_w, chk_h);

      place_child((UI_ELEMENT*)level_filter_group, level_x, frame_y, level_w, frame_h);
      row_y = frame_y + frame_h - group_pad - group_title_h - chk_h;
      place_child((UI_ELEMENT*)chk_level[0], col0_x, row_y, level_col_w, chk_h);
      place_child((UI_ELEMENT*)chk_level[3], col1_x, row_y, level_col_w, chk_h);
      row_y -= (chk_h + level_row_gap);
      place_child((UI_ELEMENT*)chk_level[1], col0_x, row_y, level_col_w, chk_h);
      place_child((UI_ELEMENT*)chk_level[4], col1_x, row_y, level_col_w, chk_h);
      row_y -= (chk_h + level_row_gap);
      place_child((UI_ELEMENT*)chk_level[2], col0_x, row_y, level_col_w, chk_h);
      place_child((UI_ELEMENT*)chk_level[5], col1_x, row_y, level_col_w, chk_h);
    }

  auto center_btn_captions = [](UI_ELEMENT* multi)
    {
      if(!multi || !multi->GetComposeElements()) return;

      const double btn_w      = XTRACEMONITOR_CONFIGMODAL_BTN_W;
      const double btn_h      = XTRACEMONITOR_CONFIGMODAL_BTN_H;
      const double icon_clear = XTRACEMONITOR_CONFIGMODAL_BTN_ICON_RIGHT + XTRACEMONITOR_CONFIGMODAL_BTN_ICON_GAP;
      const double pad_r      = XTRACEMONITOR_CONFIGMODAL_BTN_TEXT_PAD_R;

      for(XDWORD c=0; c<multi->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* face = multi->GetComposeElements()->Get(c);
          if(!face || (face->GetType() != UI_ELEMENT_TYPE_BUTTON && face->GetType() != UI_ELEMENT_TYPE_OPTION)) continue;

          UI_ELEMENT_OPTION* opt  = (UI_ELEMENT_OPTION*)face;
          UI_ELEMENT_TEXT*   text = opt->Get_UIText();
          if(!text || !text->GetBoundaryLine()) continue;

          double tw = text->GetBoundaryLine()->width;
          double sf = (double)text->GetSizeFont();
          if(tw < 1.0) tw = 1.0;
          if(sf < 1.0) sf = text->GetBoundaryLine()->height;
          if(sf < 1.0) sf = 11.0;

          double local_x = (btn_w - tw) * 0.5;
          if(local_x < icon_clear) local_x = icon_clear;
          if((local_x + tw) > (btn_w - pad_r)) local_x = btn_w - pad_r - tw;
          if(local_x < icon_clear) local_x = icon_clear;

          const double icon_mid_from_top = 24.0;
          double       local_y           = btn_h - icon_mid_from_top - (sf * 0.5);
          if(local_y < 0.0) local_y = 0.0;

          text->SetXPosition(face->GetXPosition() + local_x);
          text->SetYPosition(face->GetYPosition() - local_y);
          text->GetBoundaryLine()->x      = local_x;
          text->GetBoundaryLine()->y      = local_y;
          text->GetBoundaryLine()->height = sf;
          text->SetMustReDraw(true);
        }
    };

  center_btn_captions(filters_modal_btn_default);
  center_btn_captions(filters_modal_btn_accept);
  center_btn_captions(filters_modal_btn_cancel);

  auto pack_check_row = [](UI_ELEMENT_CHECKBOX* chk)
    {
      if(!chk || !chk->GetBoundaryLine()) return;

      UI_ELEMENT_ANIMATION* anim = chk->Get_UIAnimation();
      UI_ELEMENT_TEXT*      text = chk->Get_UIText();
      if(!anim || !text || !anim->GetBoundaryLine() || !text->GetBoundaryLine()) return;

      const double glyph   = 16.0;
      const double gap_x   = 8.0;
      const double row_h   = chk->GetBoundaryLine()->height;
      double       sf      = (double)text->GetSizeFont();
      if(sf < 1.0) sf = 12.0;
      const double optical = sf * 0.22;

      double anim_y = (row_h - glyph) * 0.5;
      if(anim_y < 0.0) anim_y = 0.0;
      double text_y = anim_y + (glyph * 0.5) - optical;
      if(text_y < 0.0) text_y = 0.0;

      anim->GetBoundaryLine()->x      = 0.0;
      anim->GetBoundaryLine()->y      = anim_y;
      anim->GetBoundaryLine()->width  = glyph;
      anim->GetBoundaryLine()->height = glyph;

      text->GetBoundaryLine()->x      = glyph + gap_x;
      text->GetBoundaryLine()->y      = text_y;
      text->GetBoundaryLine()->width  = chk->GetBoundaryLine()->width - glyph - gap_x;
      if(text->GetBoundaryLine()->width < 1.0) text->GetBoundaryLine()->width = 1.0;
      text->GetBoundaryLine()->height = sf;

      const double fx = chk->GetXPosition();
      const double fy = chk->GetYPosition();
      anim->SetXPosition(fx);
      anim->SetYPosition(fy - anim_y);
      text->SetXPosition(fx + glyph + gap_x);
      text->SetYPosition(fy - text_y);
      anim->SetMustReDraw(true);
      text->SetMustReDraw(true);
    };

  pack_check_row(chk_textfilter_active);
  for(int i=0; i<6; i++) pack_check_row(chk_level[i]);

  filters_modal->SetMustReDraw(true);
  if(filters_modal_title)       filters_modal_title->SetMustReDraw(true);
  if(filters_modal_btn_accept)  filters_modal_btn_accept->SetMustReDraw(true);
  if(filters_modal_btn_cancel)  filters_modal_btn_cancel->SetMustReDraw(true);
  if(filters_modal_btn_default) filters_modal_btn_default->SetMustReDraw(true);
  if(text_filter_group)         text_filter_group->SetMustReDraw(true);
  if(chk_textfilter_active)     chk_textfilter_active->SetMustReDraw(true);
  if(edit_textfilter)           edit_textfilter->SetMustReDraw(true);
  if(level_filter_group)        level_filter_group->SetMustReDraw(true);
  for(int i=0; i<6; i++)
    {
      if(chk_level[i]) chk_level[i]->SetMustReDraw(true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::IsVisible()
* @brief      True when the filters modal form is currently shown.
* @ingroup    UTILITIES
*
* @return     bool : true if visible; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::IsVisible()
{
  return (filters_modal && filters_modal->IsVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::HoldPressVisual(UI_ELEMENT* btn)
* @brief      Forces the multioption Pressed face and holds it for PRESS_HOLD_MS.
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption (Accept / Cancel / Predeterminado).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::HoldPressVisual(UI_ELEMENT* btn)
{
  if(!btn) return false;

  UI_PROPERTY_SELECTABLE* sel = dynamic_cast<UI_PROPERTY_SELECTABLE*>(btn);
  if(sel)
    {
      sel->SetTimeSelected(XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS);
      btn->SetPreSelect(false);
      btn->SetSelected(true);
      if(sel->GetXTimerSelected()) sel->GetXTimerSelected()->Reset();
    }

  if(btn->GetComposeElements())
    {
      for(XDWORD c=0; c<btn->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* face = btn->GetComposeElements()->Get(c);
          if(!face) continue;
          face->SetMustReDraw(true);
          if(face->GetComposeElements())
            {
              for(XDWORD d=0; d<face->GetComposeElements()->GetSize(); d++)
                {
                  UI_ELEMENT* child = face->GetComposeElements()->Get(d);
                  if(child) child->SetMustReDraw(true);
                }
            }
        }
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(btn, true);
  if(filters_modal)
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(filters_modal, true);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::ArmCloseAfterPress(UI_ELEMENT* btn)
* @brief      Shows Pressed for PRESS_HOLD_MS, then closes the modal on Tick().
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption that was clicked.
*
* @return     bool : true if armed (or already armed); otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::ArmCloseAfterPress(UI_ELEMENT* btn)
{
  if(close_armed) return true;

  HoldPressVisual(btn);

  if(!close_timer)
    {
      close_timer = GEN_XFACTORY.CreateTimer();
      if(!close_timer) return Show(false);
    }

  close_timer->Reset();
  close_armed = true;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::Tick()
* @brief      Completes a deferred Accept/Cancel close after the press hold.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::Tick()
{
  if(!close_armed || !close_timer) return true;

  if(close_timer->GetMeasureMilliSeconds() >= (XQWORD)XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS)
    {
      close_armed = false;
      return Show(false);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::Show(bool show)
* @brief      Shows or hides the filters modal as a GEN modal layer.
* @ingroup    UTILITIES
*
* @param[in]  show : true to open; false to close.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::Show(bool show)
{
  if(!owner || !filters_modal) return false;

  close_armed = false;

  if(show)
    {
      if(owner->form_cfg.IsVisible())
        {
          owner->form_cfg.Show(false);
        }

      if(!ApplyGeometry()) return false;

      if(!LoadCommittedFromCFG()) return false;
      if(!SyncControlsFromDraft()) return false;

      GEN_USERINTERFACE.SetPreselectElement();
      filters_modal->SetVisible(true);
      GEN_USERINTERFACE.Element_SetModal(filters_modal);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      owner->OpStatus_Log(NL("Filters modal open."));
    }
   else
    {
      GEN_USERINTERFACE.Element_SetModal(NULL);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      filters_modal->SetVisible(false);

      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      owner->InternetStatus_Update(true);

      owner->OpStatus_Log(NL("Filters modal closed."));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::HandleElementSelected(UI_ELEMENT* element)
* @brief      Handles filters-modal UI selections. Returns true when the event was consumed.
* @ingroup    UTILITIES
*
* @param[in]  element : Selected element.
*
* @return     bool : true if consumed by this form; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::HandleElementSelected(UI_ELEMENT* element)
{
  if(!element) return false;

  XSTRING* name = element->GetName();
  if(!name) return false;

  if(close_armed)
    {
      if(!name->Compare(NL("filters_modal_btn_accept"), true) ||
         !name->Compare(NL("filters_modal_btn_cancel"), true) ||
         !name->Compare(NL("filters_modal_btn_default"), true))
        {
          return true;
        }
    }

  if(!name->Compare(NL("filter-btn"), true) ||
     !name->Compare(NL("filter-active-btn"), true))
    {
      return Show(true);
    }

  if(chk_textfilter_active && (element == (UI_ELEMENT*)chk_textfilter_active))
    {
      textfilter_active_draft = chk_textfilter_active->GetStatus();
      ApplyTextFilterEditEnabled();
      return true;
    }

  if(edit_textfilter && (element == (UI_ELEMENT*)edit_textfilter))
    {
      if(!textfilter_active_draft) return true;
      if(edit_textfilter->GetText()) textfilter_draft = (*edit_textfilter->GetText());
      return true;
    }

  for(int i=0; i<6; i++)
    {
      if(chk_level[i] && (element == (UI_ELEMENT*)chk_level[i]))
        {
          const int mask = LevelMaskForIndex(i);
          if(chk_level[i]->GetStatus()) levelfilter_draft |=  mask;
                                   else levelfilter_draft &= ~mask;
          return true;
        }
    }

  if(!name->Compare(NL("filters_modal_btn_accept"), true))
    {
      return OnAccept();
    }

  if(!name->Compare(NL("filters_modal_btn_cancel"), true))
    {
      return OnCancel();
    }

  if(!name->Compare(NL("filters_modal_btn_default"), true))
    {
      return OnDefault();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::LoadCommittedFromCFG()
* @brief      Loads committed filter options from CFG [filters] into draft.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::LoadCommittedFromCFG()
{
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  textfilter_active_committed = cfg.Filters_IsTextActive();
  textfilter_active_draft     = textfilter_active_committed;

  if(cfg.Filters_GetText()) textfilter_committed = (*cfg.Filters_GetText());
                       else textfilter_committed.Empty();
  textfilter_draft = textfilter_committed;

  levelfilter_committed = cfg.Filters_GetLevels();
  levelfilter_draft     = levelfilter_committed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::SyncControlsFromDraft()
* @brief      Pushes draft filter state into the modal widgets.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::SyncControlsFromDraft()
{
  if(!chk_textfilter_active || !edit_textfilter) return false;

  chk_textfilter_active->SetStatus(textfilter_active_draft);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_textfilter_active);

  if(edit_textfilter->GetText())
    {
      edit_textfilter->GetText()->Set(textfilter_draft.Get());
      edit_textfilter->Cursor_SetPosition(edit_textfilter->GetText()->GetSize());
    }
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_textfilter);

  for(int i=0; i<6; i++)
    {
      const int mask = LevelMaskForIndex(i);
      LevelCheckSet(i, (levelfilter_draft & mask) != 0);
    }

  return ApplyTextFilterEditEnabled();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::ReadDraftFromControls()
* @brief      Pulls draft filter state from the modal widgets.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::ReadDraftFromControls()
{
  if(chk_textfilter_active) textfilter_active_draft = chk_textfilter_active->GetStatus();

  if(edit_textfilter && edit_textfilter->GetText())
    {
      textfilter_draft = (*edit_textfilter->GetText());
    }

  int levels = 0;
  for(int i=0; i<6; i++)
    {
      if(LevelCheckGet(i)) levels |= LevelMaskForIndex(i);
    }
  levelfilter_draft = levels;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::ApplyDefaultsToDraft()
* @brief      Resets draft to factory [filters] defaults without saving.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::ApplyDefaultsToDraft()
{
  textfilter_active_draft = XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE_DEFAULT;
  textfilter_draft.Empty();
  levelfilter_draft = XTRACEMONITOR_CFG_FILTERS_LEVELFILTER_DEFAULT;
  return SyncControlsFromDraft();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::ApplyTextFilterEditEnabled()
* @brief      Enables/disables the text-filter edit from the Enable text filter draft flag.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::ApplyTextFilterEditEnabled()
{
  if(!edit_textfilter) return false;

  const bool on = textfilter_active_draft;
  edit_textfilter->SetActive(on);

  // Mute field chrome when off (SetActive alone does not change paint colors).
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const bool light = (cfg.ResolveColorTheme(cfg.GetColorTheme()) == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);

  if(edit_textfilter->GetBackgroundColor())
    {
      UI_COLOR* bg  = edit_textfilter->GetBackgroundColor();
      UI_COLOR* ink = edit_textfilter->GetColor();
      if(on)
        {
          if(light)
            {
              bg->SetRed(210); bg->SetGreen(214); bg->SetBlue(220); bg->SetAlpha(255);
              if(ink) { ink->SetRed(40); ink->SetGreen(44); ink->SetBlue(52); ink->SetAlpha(255); }
            }
           else
            {
              bg->SetRed(64); bg->SetGreen(72); bg->SetBlue(84); bg->SetAlpha(255);
              if(ink) { ink->SetRed(200); ink->SetGreen(208); ink->SetBlue(220); ink->SetAlpha(255); }
            }
        }
       else
        {
          if(light)
            {
              bg->SetRed(186); bg->SetGreen(190); bg->SetBlue(198); bg->SetAlpha(140);
              if(ink) { ink->SetRed(155); ink->SetGreen(160); ink->SetBlue(170); ink->SetAlpha(120); }
            }
           else
            {
              bg->SetRed(36); bg->SetGreen(40); bg->SetBlue(48); bg->SetAlpha(140);
              if(ink) { ink->SetRed(90); ink->SetGreen(98); ink->SetBlue(110); ink->SetAlpha(110); }
            }
        }
      edit_textfilter->SetBackgroundColorSet(true);
      if(ink) edit_textfilter->SetColorSet(true);
      edit_textfilter->SnapshotStyleVisual();
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(edit_textfilter);
  if(GEN_USERINTERFACE.Element_IsModalActive()) GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::OnAccept()
* @brief      Commits draft filter options to CFG [filters], saves the INI file, closes the modal.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::OnAccept()
{
  if(!owner) return false;

  ReadDraftFromControls();

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  cfg.Filters_SetIsTextActive(textfilter_active_draft);
  cfg.Filters_SetText(&textfilter_draft);
  cfg.Filters_SetLevels(levelfilter_draft);

  if(!cfg.Save())
    {
      owner->OpStatus_Log(NL("Filters: Aceptar - error al guardar CFG [filters]."));
      return false;
    }

  textfilter_active_committed = textfilter_active_draft;
  textfilter_committed        = textfilter_draft;
  levelfilter_committed       = levelfilter_draft;

  owner->Toolbar_ApplyFilterBtnVisibility();

  owner->OpStatus_Log(NL("Filters: Aceptar - CFG [filters] guardado."));
  return ArmCloseAfterPress(filters_modal_btn_accept);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::OnCancel()
* @brief      Discards draft filter options and closes the modal.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::OnCancel()
{
  if(!owner) return false;

  LoadCommittedFromCFG();
  SyncControlsFromDraft();

  owner->OpStatus_Log(NL("Filters: Cancelar."));
  return ArmCloseAfterPress(filters_modal_btn_cancel);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::OnDefault()
* @brief      Sets draft filter options to factory defaults without saving.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::OnDefault()
{
  if(!owner) return false;

  ApplyDefaultsToDraft();
  if(filters_modal) GEN_USERINTERFACE.ModalLayer_Invalidate();

  owner->OpStatus_Log(NL("Filters: Predeterminado."));
  return HoldPressVisual(filters_modal_btn_default);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_FILTERS::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_FILTERS::Clean()
{
  owner                     = NULL;
  filters_modal             = NULL;
  filters_modal_title       = NULL;
  filters_modal_btn_accept  = NULL;
  filters_modal_btn_cancel  = NULL;
  filters_modal_btn_default = NULL;
  text_filter_group         = NULL;
  chk_textfilter_active     = NULL;
  edit_textfilter           = NULL;
  level_filter_group        = NULL;
  for(int i=0; i<6; i++) chk_level[i] = NULL;

  textfilter_active_committed = false;
  textfilter_active_draft     = false;
  textfilter_committed.Empty();
  textfilter_draft.Empty();
  levelfilter_committed = XTRACEMONITOR_CFG_FILTERS_LEVELFILTER_DEFAULT;
  levelfilter_draft     = XTRACEMONITOR_CFG_FILTERS_LEVELFILTER_DEFAULT;

  close_timer = NULL;
  close_armed = false;
}
