/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_CFG.cpp
*
* @class      XTRACEMONITOR_FORM_CFG
* @brief      Configuracion modal: draft state, radios, Accept / Cancel / Predeterminado
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

#include "XTraceMonitor_Form_CFG.h"

#include "GRPScreen.h"

#include "UI_Element.h"
#include "UI_Element_Form.h"
#include "UI_Element_Option.h"
#include "UI_Element_Text.h"
#include "UI_Element_RadioGroup.h"
#include "UI_Element_RadioButton.h"
#include "UI_Element_CheckBox.h"
#include "UI_Element_EditText.h"
#include "UI_Element_Combo.h"
#include "UI_Element_MultiOption.h"
#include "UI_Element_Animation.h"
#include "UI_Property_Selectable.h"
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "XFactory.h"
#include "XTimer.h"
#include "UI_Manager.h"

#include "CipherAES.h"
#include "CipherKeySymmetrical.h"
#include "XBuffer.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_CFG::XTRACEMONITOR_FORM_CFG()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_CFG::XTRACEMONITOR_FORM_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_CFG::~XTRACEMONITOR_FORM_CFG()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_CFG::~XTRACEMONITOR_FORM_CFG()
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
* @fn         bool XTRACEMONITOR_FORM_CFG::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds the form helper to the application instance (screen / OpStatus).
* @ingroup    UTILITIES
*
* @param[in]  owner : Application instance.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (this->owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::Resolve()
* @brief      Resolves config modal elements and loads committed theme from CFG.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::Resolve()
{
  config_modal = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("config_modal"), UI_ELEMENT_TYPE_FORM);
  if(!config_modal) return false;

  config_modal_title       = GEN_USERINTERFACE.Element_Get(NL("config_modal_title")      , UI_ELEMENT_TYPE_TEXT);
  config_modal_btn_accept  = GEN_USERINTERFACE.Element_Get(NL("config_modal_btn_accept") , UI_ELEMENT_TYPE_MULTIOPTION);
  config_modal_btn_cancel  = GEN_USERINTERFACE.Element_Get(NL("config_modal_btn_cancel") , UI_ELEMENT_TYPE_MULTIOPTION);
  config_modal_btn_default = GEN_USERINTERFACE.Element_Get(NL("config_modal_btn_default"), UI_ELEMENT_TYPE_MULTIOPTION);
  color_theme_group        = (UI_ELEMENT_RADIOGROUP*)GEN_USERINTERFACE.Element_Get(NL("color_theme_group"), UI_ELEMENT_TYPE_RADIOGROUP);
  theme_light              = (UI_ELEMENT_RADIOBUTTON*)GEN_USERINTERFACE.Element_Get(NL("theme_light"), UI_ELEMENT_TYPE_RADIOBUTTON);
  theme_dark               = (UI_ELEMENT_RADIOBUTTON*)GEN_USERINTERFACE.Element_Get(NL("theme_dark") , UI_ELEMENT_TYPE_RADIOBUTTON);
  chk_savescreenposition   = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_savescreenposition"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_savescreensize       = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_savescreensize")    , UI_ELEMENT_TYPE_CHECKBOX);
  chk_adddatetimeconsole   = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_adddatetimeconsole"), UI_ELEMENT_TYPE_CHECKBOX);
  chk_addipconsole         = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_addipconsole")      , UI_ELEMENT_TYPE_CHECKBOX);
  chk_texttracking         = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_texttracking")      , UI_ELEMENT_TYPE_CHECKBOX);
  lbl_udplistenport        = GEN_USERINTERFACE.Element_Get(NL("lbl_udplistenport"), UI_ELEMENT_TYPE_TEXT);
  edit_udplistenport       = (UI_ELEMENT_COMBO*)GEN_USERINTERFACE.Element_Get(NL("edit_udplistenport"), UI_ELEMENT_TYPE_COMBO);
  lbl_uartport             = GEN_USERINTERFACE.Element_Get(NL("lbl_uartport"), UI_ELEMENT_TYPE_TEXT);
  edit_uartport            = (UI_ELEMENT_COMBO*)GEN_USERINTERFACE.Element_Get(NL("edit_uartport"), UI_ELEMENT_TYPE_COMBO);
  dyndns_group             = (UI_ELEMENT_RADIOGROUP*)GEN_USERINTERFACE.Element_Get(NL("dyndns_group"), UI_ELEMENT_TYPE_RADIOGROUP);
  chk_dyndns_active        = (UI_ELEMENT_CHECKBOX*)GEN_USERINTERFACE.Element_Get(NL("chk_dyndns_active"), UI_ELEMENT_TYPE_CHECKBOX);
  lbl_dyndns_login         = GEN_USERINTERFACE.Element_Get(NL("lbl_dyndns_login"), UI_ELEMENT_TYPE_TEXT);
  edit_dyndns_login        = (UI_ELEMENT_EDITTEXT*)GEN_USERINTERFACE.Element_Get(NL("edit_dyndns_login"), UI_ELEMENT_TYPE_EDITTEXT);
  lbl_dyndns_password      = GEN_USERINTERFACE.Element_Get(NL("lbl_dyndns_password"), UI_ELEMENT_TYPE_TEXT);
  edit_dyndns_password     = (UI_ELEMENT_EDITTEXT*)GEN_USERINTERFACE.Element_Get(NL("edit_dyndns_password"), UI_ELEMENT_TYPE_EDITTEXT);
  dyndns_btn               = (UI_ELEMENT_MULTIOPTION*)GEN_USERINTERFACE.Element_Get(NL("dyndns-btn"), UI_ELEMENT_TYPE_MULTIOPTION);

  if(!config_modal_title || !config_modal_btn_accept || !config_modal_btn_cancel || !config_modal_btn_default)
    {
      return false;
    }

  if(!color_theme_group || !theme_light || !theme_dark)
    {
      return false;
    }

  if(!chk_savescreenposition || !chk_savescreensize || !chk_adddatetimeconsole || !chk_addipconsole || !chk_texttracking)
    {
      return false;
    }

  if(!lbl_udplistenport || !edit_udplistenport)
    {
      return false;
    }

  if(!lbl_uartport || !edit_uartport)
    {
      return false;
    }

  if(!dyndns_group || !chk_dyndns_active || !lbl_dyndns_login || !edit_dyndns_login ||
     !lbl_dyndns_password || !edit_dyndns_password || !dyndns_btn)
    {
      return false;
    }

  // Secondary layout inherits the skin page fill; keep it invalid so Layout_PutBackground
  // never canvas-Clears through this layout (would erase the main form under the dialog).
  UI_LAYOUT* modal_layout = GEN_USERINTERFACE.Layouts_Get(NL("config_modal"));
  if(modal_layout && modal_layout->GetBackground() && modal_layout->GetBackground()->GetColor())
    {
      modal_layout->GetBackground()->GetColor()->SetValid(false);
    }

  if(!LoadCommittedFromCFG()) return false;
  if(!SyncRadiosFromDraft()) return false;
  if(!SyncChecksFromDraft()) return false;
  if(!SyncPortFromDraft()) return false;
  if(!SyncUARTFromDraft()) return false;
  if(!RebuildTargetCombos()) return false;
  if(!SyncDynDNSFromDraft()) return false;
  if(!ApplyDynDNSFieldsEnabled()) return false;
  if(!ApplyDynDNSToolbarButton(dyndns_active_committed)) return false;

  config_modal->SetVisible(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ApplyGeometry()
* @brief      Keeps the config modal at its fixed design size and centers it.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ApplyGeometry()
{
  if(!owner || !config_modal) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  if(!screen) return false;

  double screen_w = (double)screen->GetWidth();
  double screen_h = (double)screen->GetHeight();
  if((screen_w < 1.0) || (screen_h < 1.0)) return false;

  const double modal_w = XTRACEMONITOR_CONFIGMODAL_W;
  const double modal_h = XTRACEMONITOR_CONFIGMODAL_H;

  double modal_x = (screen_w - modal_w) * 0.5;
  double modal_y = (screen_h + modal_h) * 0.5;

  config_modal->SetXPosition(modal_x);
  config_modal->SetYPosition(modal_y);
  config_modal->GetBoundaryLine()->x      = modal_x;
  config_modal->GetBoundaryLine()->y      = modal_y;
  config_modal->GetBoundaryLine()->width  = modal_w;
  config_modal->GetBoundaryLine()->height = modal_h;
  config_modal->SetIntrinsicWidth(modal_w);
  config_modal->SetIntrinsicHeight(modal_h);

  UI_BOUNDARYLINE* vis = config_modal->GetVisibleRect();
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
  // Same bottom gap as filters_modal (BTN_BOTTOM_PAD); keep content locked to top.
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

  if(config_modal_title)
    {
      const double title_pad = XTRACEMONITOR_CONFIGMODAL_TITLE_PAD;
      const double title_h   = XTRACEMONITOR_CONFIGMODAL_TITLE_H;
      const double title_local_y = modal_h - title_pad - title_h;
      place_child(config_modal_title, title_pad, title_local_y, modal_w - (title_pad * 2.0), title_h);
    }

  place_child(config_modal_btn_default, btn_x,                       btn_local_y, btn_w, btn_h);
  place_child(config_modal_btn_accept , btn_x + btn_w + gap,         btn_local_y, btn_w, btn_h);
  place_child(config_modal_btn_cancel , btn_x + (btn_w + gap) * 2.0, btn_local_y, btn_w, btn_h);

  // Color Theme — left; DynDNS — right; same top edge + space-evenly gap rhythm.
  const double theme_x = XTRACEMONITOR_CONFIGMODAL_PAD;
  const double theme_y = 316.0; // bottom trim; top/title fixed
  const double theme_w = 180.0;
  const double theme_h = 96.0;
  const double theme_row_h = 24.0;                       // radiobutton row height (XML)
  const double theme_gap = (theme_h - (theme_row_h * 2.0)) / 3.0;  // space-evenly gap = 16
  place_child((UI_ELEMENT*)color_theme_group, theme_x, theme_y, theme_w, theme_h);

  const double chk_h        = 28.0;
  const double port_label_h = 18.0;
  const double port_edit_h  = 28.0;
  const double port_lbl_gap = 6.0;
  const double uart_edit_w  = 320.0;
  const double port_edit_w  = uart_edit_w; // UDP edit same width as Puerto COM / UART
  const double left_x       = XTRACEMONITOR_CONFIGMODAL_PAD;

  // DynDNS column (Login/Password left edge) — UART aligns here too.
  const double dyn_pad     = 16.0;
  const double dyn_edit_w  = uart_edit_w;
  const double dyn_w       = dyn_pad + dyn_edit_w + dyn_pad;
  const double dyn_x       = modal_w - XTRACEMONITOR_CONFIGMODAL_PAD - dyn_w;
  const double dyn_field_x = dyn_x + dyn_pad;

  // DynDNS — same top as Color Theme; peer gaps == Color Theme space-evenly gap.
  const double dyn_lbl_gap = port_lbl_gap;
  const double dyn_chk_h   = theme_row_h;
  const double dyn_block_h = port_label_h + dyn_lbl_gap + port_edit_h;
  const double dyn_gap     = theme_gap;
  const double dyn_content = dyn_chk_h + dyn_block_h + dyn_block_h;
  const double dyn_h       = dyn_content + (dyn_gap * 4.0); // 3 peers → 4 equal gaps
  const double theme_top   = theme_y + theme_h;
  const double dyn_y       = theme_top - dyn_h;           // TopY matches Color Theme

  place_child((UI_ELEMENT*)dyndns_group, dyn_x, dyn_y, dyn_w, dyn_h);

  auto place_dyn = [&](UI_ELEMENT* el, double from_top_pos, double h)
    {
      place_child(el, dyn_x + dyn_pad, dyn_y + (dyn_h - from_top_pos - h), dyn_edit_w, h);
    };

  const double dyn_fields_raise = 6.0; // Login/Password (+edits) nudged up toward Activar
  double from_top = dyn_gap;
  place_dyn((UI_ELEMENT*)chk_dyndns_active, from_top, dyn_chk_h);
  from_top += dyn_chk_h + dyn_gap;
  place_dyn(lbl_dyndns_login, from_top - dyn_fields_raise, port_label_h);
  place_dyn((UI_ELEMENT*)edit_dyndns_login, from_top - dyn_fields_raise + port_label_h + dyn_lbl_gap, port_edit_h);
  from_top += dyn_block_h + dyn_gap;
  place_dyn(lbl_dyndns_password, from_top - dyn_fields_raise, port_label_h);
  place_dyn((UI_ELEMENT*)edit_dyndns_password, from_top - dyn_fields_raise + port_label_h + dyn_lbl_gap, port_edit_h);

  // Guardar checks — center column between Color Theme and DynDNS, top-aligned with Color Theme.
  const double save_chk_w = 280.0;
  const double theme_right = theme_x + theme_w;
  const double save_slot   = dyn_x - theme_right;
  double       save_chk_x  = theme_right + (save_slot - save_chk_w) * 0.5 + 20.0;
  if(save_chk_x < theme_right + 8.0) save_chk_x = theme_right + 8.0;
  // Color Theme / DynDNS title sits on EdgeTop (baseline ≈ EdgeTop + 0.35*sf). Nudge Guardar
  // so the first caption reads level with that title notch (not the frame interior).
  const double theme_title_raise = 8.0;
  const double save_gap          = 8.0;
  double       save_y            = theme_top - chk_h + theme_title_raise;
  place_child((UI_ELEMENT*)chk_savescreenposition, save_chk_x, save_y, save_chk_w, chk_h);
  save_y -= (chk_h + save_gap);
  place_child((UI_ELEMENT*)chk_savescreensize, save_chk_x, save_y, save_chk_w, chk_h);

  // Añadir + seguimiento — left column under Color Theme (prior layout restored).
  const double add_chk_w  = 320.0;
  const double add_margin = 8.0 + 16.0 - 6.0; // raise block ~6px
  const double add_gap    = 8.0;
  double       add_y      = theme_y - add_margin - chk_h;
  place_child((UI_ELEMENT*)chk_adddatetimeconsole, left_x, add_y, add_chk_w, chk_h);
  add_y -= (chk_h + add_gap);
  place_child((UI_ELEMENT*)chk_addipconsole, left_x, add_y, add_chk_w, chk_h);
  add_y -= (chk_h + add_gap);
  place_child((UI_ELEMENT*)chk_texttracking, left_x, add_y, add_chk_w, chk_h);
  const double add_bottom = add_y;

  // Ports — under Añadir + DynDNS; UDP/UART share the same Y (vertical align).
  const double ports_gap    = 16.0;
  const double ports_ceil   = (add_bottom < dyn_y) ? add_bottom : dyn_y;
  const double port_label_y = ports_ceil - ports_gap - port_label_h;
  const double port_edit_y  = port_label_y - port_lbl_gap - port_edit_h;
  place_child(lbl_udplistenport,               left_x,      port_label_y, port_edit_w, port_label_h);
  place_child((UI_ELEMENT*)edit_udplistenport, left_x,      port_edit_y,  port_edit_w, port_edit_h);
  place_child(lbl_uartport,                    dyn_field_x, port_label_y, uart_edit_w, port_label_h);
  place_child((UI_ELEMENT*)edit_uartport,      dyn_field_x, port_edit_y,  uart_edit_w, port_edit_h);

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(config_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(config_modal_btn_default) skin->CalculeBoundaryLine_AllElements(config_modal_btn_default, true);
      if(config_modal_btn_accept)  skin->CalculeBoundaryLine_AllElements(config_modal_btn_accept , true);
      if(config_modal_btn_cancel)  skin->CalculeBoundaryLine_AllElements(config_modal_btn_cancel , true);
      if(color_theme_group)        skin->CalculeBoundaryLine_AllElements(color_theme_group, true);
      if(chk_savescreenposition)   skin->CalculeBoundaryLine_AllElements(chk_savescreenposition, true);
      if(chk_savescreensize)       skin->CalculeBoundaryLine_AllElements(chk_savescreensize, true);
      if(chk_adddatetimeconsole)   skin->CalculeBoundaryLine_AllElements(chk_adddatetimeconsole, true);
      if(chk_addipconsole)         skin->CalculeBoundaryLine_AllElements(chk_addipconsole, true);
      if(chk_texttracking)         skin->CalculeBoundaryLine_AllElements(chk_texttracking, true);
      if(lbl_udplistenport)        skin->CalculeBoundaryLine_AllElements(lbl_udplistenport, true);
      if(edit_udplistenport)       skin->CalculeBoundaryLine_AllElements(edit_udplistenport, true);
      if(lbl_uartport)             skin->CalculeBoundaryLine_AllElements(lbl_uartport, true);
      if(edit_uartport)            skin->CalculeBoundaryLine_AllElements(edit_uartport, true);
      if(dyndns_group)             skin->CalculeBoundaryLine_AllElements(dyndns_group, true);
      if(chk_dyndns_active)        skin->CalculeBoundaryLine_AllElements(chk_dyndns_active, true);
      if(lbl_dyndns_login)         skin->CalculeBoundaryLine_AllElements(lbl_dyndns_login, true);
      if(edit_dyndns_login)        skin->CalculeBoundaryLine_AllElements(edit_dyndns_login, true);
      if(lbl_dyndns_password)      skin->CalculeBoundaryLine_AllElements(lbl_dyndns_password, true);
      if(edit_dyndns_password)     skin->CalculeBoundaryLine_AllElements(edit_dyndns_password, true);
    }

  // AllElements recurses into anim/text and undoes the skin glyph pack — pin again here.
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

      if(anim->GetComposeElements())
        {
          for(XDWORD c=0; c<anim->GetComposeElements()->GetSize(); c++)
            {
              UI_ELEMENT* img = anim->GetComposeElements()->Get(c);
              if(!img || !img->GetBoundaryLine()) continue;
              img->GetBoundaryLine()->x      = 0.0;
              img->GetBoundaryLine()->y      = 0.0;
              img->GetBoundaryLine()->width  = glyph;
              img->GetBoundaryLine()->height = glyph;
              img->SetXPosition(anim->GetXPosition());
              img->SetYPosition(anim->GetYPosition());
              img->SetMustReDraw(true);
            }
        }

      anim->SetMustReDraw(true);
      text->SetMustReDraw(true);
      chk->SetMustReDraw(true);
    };

  pack_check_row(chk_savescreenposition);
  pack_check_row(chk_savescreensize);
  pack_check_row(chk_adddatetimeconsole);
  pack_check_row(chk_addipconsole);
  pack_check_row(chk_texttracking);

  // Re-pin frames + checks + ports after AllElements (radiogroup shrink-wrap must not undo size/top align).
  place_child((UI_ELEMENT*)color_theme_group, theme_x, theme_y, theme_w, theme_h);
  place_child((UI_ELEMENT*)dyndns_group, dyn_x, dyn_y, dyn_w, dyn_h);
  {
    double sy = theme_top - chk_h + theme_title_raise;
    place_child((UI_ELEMENT*)chk_savescreenposition, save_chk_x, sy, save_chk_w, chk_h);
    sy -= (chk_h + save_gap);
    place_child((UI_ELEMENT*)chk_savescreensize, save_chk_x, sy, save_chk_w, chk_h);

    double ay = theme_y - add_margin - chk_h;
    place_child((UI_ELEMENT*)chk_adddatetimeconsole, left_x, ay, add_chk_w, chk_h);
    ay -= (chk_h + add_gap);
    place_child((UI_ELEMENT*)chk_addipconsole, left_x, ay, add_chk_w, chk_h);
    ay -= (chk_h + add_gap);
    place_child((UI_ELEMENT*)chk_texttracking, left_x, ay, add_chk_w, chk_h);
  }
  // Re-pack after re-pin so anim/text hit-tests track the final checkbox Y.
  pack_check_row(chk_savescreenposition);
  pack_check_row(chk_savescreensize);
  pack_check_row(chk_adddatetimeconsole);
  pack_check_row(chk_addipconsole);
  pack_check_row(chk_texttracking);
  place_child(lbl_udplistenport,               left_x,      port_label_y, port_edit_w, port_label_h);
  place_child((UI_ELEMENT*)edit_udplistenport, left_x,      port_edit_y,  port_edit_w, port_edit_h);
  place_child(lbl_uartport,                    dyn_field_x, port_label_y, uart_edit_w, port_label_h);
  place_child((UI_ELEMENT*)edit_uartport,      dyn_field_x, port_edit_y,  uart_edit_w, port_edit_h);
  {
    double ft = dyn_gap;
    place_dyn((UI_ELEMENT*)chk_dyndns_active, ft, dyn_chk_h);
    ft += dyn_chk_h + dyn_gap;
    place_dyn(lbl_dyndns_login, ft - dyn_fields_raise, port_label_h);
    place_dyn((UI_ELEMENT*)edit_dyndns_login, ft - dyn_fields_raise + port_label_h + dyn_lbl_gap, port_edit_h);
    ft += dyn_block_h + dyn_gap;
    place_dyn(lbl_dyndns_password, ft - dyn_fields_raise, port_label_h);
    place_dyn((UI_ELEMENT*)edit_dyndns_password, ft - dyn_fields_raise + port_label_h + dyn_lbl_gap, port_edit_h);
  }
  pack_check_row(chk_dyndns_active);

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

          // VectorFont Y = baseline; ink fills the em box toward EdgeTop. Center the em box
          // on the circular icon mid in the 160×53 PNG (~y=24 from top) so mixed-case Latin
          // reads level with the glyph (0.22·sf was leaving captions optically high).
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

  center_btn_captions(config_modal_btn_default);
  center_btn_captions(config_modal_btn_accept);
  center_btn_captions(config_modal_btn_cancel);

  config_modal->SetMustReDraw(true);
  if(config_modal_title)       config_modal_title->SetMustReDraw(true);
  if(config_modal_btn_accept)  config_modal_btn_accept->SetMustReDraw(true);
  if(config_modal_btn_cancel)  config_modal_btn_cancel->SetMustReDraw(true);
  if(config_modal_btn_default) config_modal_btn_default->SetMustReDraw(true);
  if(color_theme_group)        color_theme_group->SetMustReDraw(true);
  if(chk_savescreenposition)   chk_savescreenposition->SetMustReDraw(true);
  if(chk_savescreensize)       chk_savescreensize->SetMustReDraw(true);
  if(chk_adddatetimeconsole)   chk_adddatetimeconsole->SetMustReDraw(true);
  if(chk_addipconsole)         chk_addipconsole->SetMustReDraw(true);
  if(chk_texttracking)         chk_texttracking->SetMustReDraw(true);
  if(lbl_udplistenport)        lbl_udplistenport->SetMustReDraw(true);
  if(edit_udplistenport)       edit_udplistenport->SetMustReDraw(true);
  if(lbl_uartport)             lbl_uartport->SetMustReDraw(true);
  if(edit_uartport)            edit_uartport->SetMustReDraw(true);
  if(dyndns_group)             dyndns_group->SetMustReDraw(true);
  if(chk_dyndns_active)        chk_dyndns_active->SetMustReDraw(true);
  if(lbl_dyndns_login)         lbl_dyndns_login->SetMustReDraw(true);
  if(edit_dyndns_login)        edit_dyndns_login->SetMustReDraw(true);
  if(lbl_dyndns_password)      lbl_dyndns_password->SetMustReDraw(true);
  if(edit_dyndns_password)     edit_dyndns_password->SetMustReDraw(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::IsVisible()
* @brief      True when the config modal form is currently shown.
* @ingroup    UTILITIES
*
* @return     bool : true if visible; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::IsVisible()
{
  return (config_modal && config_modal->IsVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::HoldPressVisual(UI_ELEMENT* btn)
* @brief      Forces the multioption Pressed face and holds it for PRESS_HOLD_MS.
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption (Accept / Cancel / Predeterminado).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::HoldPressVisual(UI_ELEMENT* btn)
{
  if(!btn) return false;

  // Draw_MultiOption picks the SELECTED face from the multioption itself (not the child button).
  UI_PROPERTY_SELECTABLE* sel = dynamic_cast<UI_PROPERTY_SELECTABLE*>(btn);
  if(sel)
    {
      sel->SetTimeSelected(XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS);
      btn->SetPreSelect(false);
      btn->SetSelected(true);
      if(sel->GetXTimerSelected()) sel->GetXTimerSelected()->Reset();
    }

  // Force every face (esp. selectablestatus=selected / Pressed PNG) dirty for the modal rebuild.
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
  if(config_modal)
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(config_modal, true);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ArmCloseAfterPress(UI_ELEMENT* btn)
* @brief      Shows Pressed for PRESS_HOLD_MS, then closes the modal on Tick().
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption that was clicked.
*
* @return     bool : true if armed (or already armed); otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ArmCloseAfterPress(UI_ELEMENT* btn)
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
* @fn         bool XTRACEMONITOR_FORM_CFG::Tick()
* @brief      Completes a deferred Accept/Cancel close after the press hold.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::Tick()
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
* @fn         bool XTRACEMONITOR_FORM_CFG::Show(bool show)
* @brief      Shows or hides the config modal as a GEN modal layer.
* @ingroup    UTILITIES
*
* @param[in]  show : true to open; false to close.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::Show(bool show)
{
  if(!owner || !config_modal) return false;

  close_armed = false;

  if(show)
    {
      // One app modal at a time (same Element_SetModal slot as Filters).
      if(owner->form_filters.IsVisible())
        {
          owner->form_filters.Show(false);
        }

      if(!ApplyGeometry()) return false;

      // Draft starts from the last committed CFG values (no runtime apply until Accept).
      colortheme_draft               = colortheme_committed;
      savescreenposition_draft       = savescreenposition_committed;
      savescreensize_draft           = savescreensize_committed;
      adddatetimeconsole_draft       = adddatetimeconsole_committed;
      addipconsole_draft             = addipconsole_committed;
      texttracking_draft             = texttracking_committed;
      savedscreenposition_draft      = savedscreenposition_committed;
      savedscreensize_draft          = savedscreensize_committed;
      udplistenport_draft            = udplistenport_committed;
      uartport_draft                 = uartport_committed;
      dyndns_active_draft            = dyndns_active_committed;
      dyndns_login_draft             = dyndns_login_committed;
      dyndns_password_draft          = dyndns_password_committed;
      if(!SyncRadiosFromDraft()) return false;
      if(!SyncChecksFromDraft()) return false;
      if(!SyncPortFromDraft()) return false;
      if(!SyncUARTFromDraft()) return false;
      if(!RebuildTargetCombos()) return false;
      if(!SyncDynDNSFromDraft()) return false;
      if(!ApplyDynDNSFieldsEnabled()) return false;

      GEN_USERINTERFACE.SetPreselectElement();
      config_modal->SetVisible(true);
      GEN_USERINTERFACE.Element_SetModal(config_modal);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      owner->OpStatus_Log(NL("Config modal open."));
    }
   else
    {
      GEN_USERINTERFACE.Element_SetModal(NULL);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      config_modal->SetVisible(false);

      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      // Page was wiped: rebuild net-status from committed CFG (UDP/target) + live internet values.
      owner->InternetStatus_Update(true);

      owner->OpStatus_Log(NL("Config modal closed."));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::HandleElementSelected(UI_ELEMENT* element)
* @brief      Handles config-modal UI selections. Returns true when the event was consumed.
* @ingroup    UTILITIES
*
* @param[in]  element : Selected element.
*
* @return     bool : true if consumed by this form; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::HandleElementSelected(UI_ELEMENT* element)
{
  if(!element) return false;

  // Ignore further modal-button clicks while the press-hold close is armed.
  if(close_armed)
    {
      XSTRING* n = element->GetName();
      if(n && (!n->Compare(NL("config_modal_btn_accept"), true) ||
               !n->Compare(NL("config_modal_btn_cancel"), true) ||
               !n->Compare(NL("config_modal_btn_default"), true)))
        {
          return true;
        }
    }

  // Color Theme draft only — theme is NOT applied until Accept (+ future Apply task).
  if(element->GetType() == UI_ELEMENT_TYPE_RADIOBUTTON)
    {
      if(theme_light && (element == (UI_ELEMENT*)theme_light))
        {
          return SetDraftColorTheme(XTRACEMONITOR_FORM_CFG_COLORTHEME_LIGHT);
        }
      if(theme_dark && (element == (UI_ELEMENT*)theme_dark))
        {
          return SetDraftColorTheme(XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK);
        }
    }

  // Manager already toggled checkbox status before this callback.
  if(element->GetType() == UI_ELEMENT_TYPE_CHECKBOX)
    {
      if(chk_savescreenposition && (element == (UI_ELEMENT*)chk_savescreenposition))
        {
          savescreenposition_draft = chk_savescreenposition->GetStatus();
          return true;
        }
      if(chk_savescreensize && (element == (UI_ELEMENT*)chk_savescreensize))
        {
          savescreensize_draft = chk_savescreensize->GetStatus();
          return true;
        }
      if(chk_adddatetimeconsole && (element == (UI_ELEMENT*)chk_adddatetimeconsole))
        {
          adddatetimeconsole_draft = chk_adddatetimeconsole->GetStatus();
          return true;
        }
      if(chk_addipconsole && (element == (UI_ELEMENT*)chk_addipconsole))
        {
          addipconsole_draft = chk_addipconsole->GetStatus();
          return true;
        }
      if(chk_texttracking && (element == (UI_ELEMENT*)chk_texttracking))
        {
          texttracking_draft = chk_texttracking->GetStatus();
          return true;
        }
      if(chk_dyndns_active && (element == (UI_ELEMENT*)chk_dyndns_active))
        {
          dyndns_active_draft = chk_dyndns_active->GetStatus();
          ApplyDynDNSFieldsEnabled();
          ApplyDynDNSToolbarButton(dyndns_active_draft);
          return true;
        }
    }

  XSTRING* name = element->GetName();
  if(!name) return false;

  if(!name->Compare(NL("config-btn"), true))
    {
      return Show(true);
    }

  if(!name->Compare(NL("config_modal_btn_accept"), true))
    {
      return OnAccept();
    }

  if(!name->Compare(NL("config_modal_btn_cancel"), true))
    {
      return OnCancel();
    }

  if(!name->Compare(NL("config_modal_btn_default"), true))
    {
      return OnDefault();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::HandleElementChangeContents(UI_ELEMENT* element)
* @brief      Keeps the UDP port draft in sync with edittext / spin changes.
* @ingroup    UTILITIES
*
* @param[in]  element : Element whose contents changed.
*
* @return     bool : true if consumed by this form; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::HandleElementChangeContents(UI_ELEMENT* element)
{
  if(!element) return false;

  if(edit_udplistenport && (element == (UI_ELEMENT*)edit_udplistenport))
    {
      return ReadDraftFromPort();
    }

  if(edit_uartport && (element == (UI_ELEMENT*)edit_uartport))
    {
      return ReadDraftFromUART();
    }

  if(edit_dyndns_login && (element == (UI_ELEMENT*)edit_dyndns_login))
    {
      return ReadDraftFromDynDNS();
    }

  if(edit_dyndns_password && (element == (UI_ELEMENT*)edit_dyndns_password))
    {
      return ReadDraftFromDynDNS();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_FORM_CFG::GetColorThemeCommitted()
* @brief      Last accepted / loaded color theme integer.
* @ingroup    UTILITIES
*
* @return     int : Color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_FORM_CFG::GetColorThemeCommitted()
{
  return colortheme_committed;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_FORM_CFG::GetColorThemeDraft()
* @brief      In-modal draft color theme integer.
* @ingroup    UTILITIES
*
* @return     int : Color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_FORM_CFG::GetColorThemeDraft()
{
  return colortheme_draft;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::LoadCommittedFromCFG()
* @brief      Reads colortheme from CFG and resolves INVALID to factory default.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::LoadCommittedFromCFG()
{
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  colortheme_committed = cfg.ResolveColorTheme(cfg.GetColorTheme());
  colortheme_draft     = colortheme_committed;

  savescreenposition_committed = cfg.GetSaveScreenPosition();
  savescreenposition_draft     = savescreenposition_committed;
  savescreensize_committed     = cfg.GetSaveScreenSize();
  savescreensize_draft         = savescreensize_committed;
  adddatetimeconsole_committed = cfg.GetAddDateTimeConsole();
  adddatetimeconsole_draft     = adddatetimeconsole_committed;
  addipconsole_committed       = cfg.GetAddIPConsole();
  addipconsole_draft           = addipconsole_committed;
  texttracking_committed       = cfg.GetTextTracking();
  texttracking_draft           = texttracking_committed;

  if(cfg.GetSavedScreenPosition()) savedscreenposition_committed = (*cfg.GetSavedScreenPosition());
                              else savedscreenposition_committed.Empty();
  savedscreenposition_draft = savedscreenposition_committed;

  if(cfg.GetSavedScreenSize()) savedscreensize_committed = (*cfg.GetSavedScreenSize());
                          else savedscreensize_committed.Empty();
  savedscreensize_draft = savedscreensize_committed;

  udplistenport_committed = cfg.ResolveUDPListenPort(cfg.GetUDPListenPort());
  udplistenport_draft     = udplistenport_committed;

  if(cfg.GetUARTPort()) uartport_committed = (*cfg.GetUARTPort());
                   else uartport_committed.Empty();
  if(uartport_committed.IsEmpty()) uartport_committed = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;
  uartport_draft = uartport_committed;
  uartport_valid = cfg.IsUARTPortValid(uartport_committed);

  dyndns_active_committed = cfg.GetDynDNSActive();
  dyndns_active_draft     = dyndns_active_committed;

  if(cfg.GetDynDNSLogin()) dyndns_login_committed = (*cfg.GetDynDNSLogin());
                      else dyndns_login_committed.Empty();
  dyndns_login_draft = dyndns_login_committed;

  // INI password is AES+Base64 when active; decrypt into plaintext draft/committed.
  dyndns_password_committed.Empty();
  if(cfg.GetDynDNSPassword() && !cfg.GetDynDNSPassword()->IsEmpty())
    {
      XSTRING cipher = (*cfg.GetDynDNSPassword());
      if(!DecryptDynDNSPassword(cipher, dyndns_password_committed))
        {
          dyndns_password_committed.Empty();
        }
    }
  dyndns_password_draft = dyndns_password_committed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SyncRadiosFromDraft()
* @brief      Selects the radiobutton that matches the draft color theme.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SyncRadiosFromDraft()
{
  if(!color_theme_group) return false;

  int theme = XTRACEMONITOR_CFG::GetInstance(false).ResolveColorTheme(colortheme_draft);
  colortheme_draft = theme;

  UI_ELEMENT_RADIOBUTTON* select = (theme == XTRACEMONITOR_FORM_CFG_COLORTHEME_LIGHT) ? theme_light : theme_dark;
  if(!select) return false;

  color_theme_group->SelectExclusive(select);
  GEN_USERINTERFACE.Elements_SetToRedraw(color_theme_group);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_FORM_CFG::ReadDraftFromRadios()
* @brief      Maps the selected Color Theme radio to a CFG integer.
* @ingroup    UTILITIES
*
* @return     int : Color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_FORM_CFG::ReadDraftFromRadios()
{
  if(color_theme_group)
    {
      UI_ELEMENT_RADIOBUTTON* selected = color_theme_group->GetSelected();
      if(selected && theme_light && (selected == theme_light)) return XTRACEMONITOR_FORM_CFG_COLORTHEME_LIGHT;
      if(selected && theme_dark  && (selected == theme_dark))  return XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK;
    }

  return XTRACEMONITOR_CFG::GetInstance(false).ResolveColorTheme(colortheme_draft);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SetDraftColorTheme(int colortheme)
* @brief      Updates the in-modal draft only (no CFG save, no runtime apply).
* @ingroup    UTILITIES
*
* @param[in]  colortheme : Draft color theme value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SetDraftColorTheme(int colortheme)
{
  colortheme_draft = XTRACEMONITOR_CFG::GetInstance(false).ResolveColorTheme(colortheme);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SyncChecksFromDraft()
* @brief      Applies draft window-save checkbox states to the UI.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SyncChecksFromDraft()
{
  if(!chk_savescreenposition || !chk_savescreensize || !chk_adddatetimeconsole || !chk_addipconsole || !chk_texttracking) return false;

  chk_savescreenposition->SetStatus(savescreenposition_draft);
  chk_savescreensize->SetStatus(savescreensize_draft);
  chk_adddatetimeconsole->SetStatus(adddatetimeconsole_draft);
  chk_addipconsole->SetStatus(addipconsole_draft);
  chk_texttracking->SetStatus(texttracking_draft);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_savescreenposition);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_savescreensize);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_adddatetimeconsole);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_addipconsole);
  GEN_USERINTERFACE.Elements_SetToRedraw(chk_texttracking);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ReadDraftFromChecks()
* @brief      Reads draft window-save flags from the checkbox widgets.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ReadDraftFromChecks()
{
  if(chk_savescreenposition) savescreenposition_draft = chk_savescreenposition->GetStatus();
  if(chk_savescreensize)     savescreensize_draft     = chk_savescreensize->GetStatus();
  if(chk_adddatetimeconsole) adddatetimeconsole_draft = chk_adddatetimeconsole->GetStatus();
  if(chk_addipconsole)       addipconsole_draft       = chk_addipconsole->GetStatus();
  if(chk_texttracking)       texttracking_draft       = chk_texttracking->GetStatus();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SyncPortFromDraft()
* @brief      Applies draft UDP listen port to the numeric edittext.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SyncPortFromDraft()
{
  if(!edit_udplistenport) return false;

  udplistenport_draft = XTRACEMONITOR_CFG::GetInstance(false).ResolveUDPListenPort(udplistenport_draft);
  edit_udplistenport->SetNumericValue(udplistenport_draft);
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_udplistenport);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SyncUARTFromDraft()
* @brief      Applies draft UART serial-line string to the edittext.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SyncUARTFromDraft()
{
  if(!edit_uartport || !edit_uartport->GetText()) return false;

  XTRACEMONITOR_CFG::GetInstance(false).ResolveUARTPort(uartport_draft);
  edit_uartport->GetText()->Set(uartport_draft.Get());
  edit_uartport->Cursor_SetPosition(edit_uartport->GetText()->GetSize());
  ApplyUARTFieldVisual();
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_uartport);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::RebuildTargetCombos()
* @brief      Fills UDP/UART combo menus from [target] (type1 / type2) with tabulated rows.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::RebuildTargetCombos()
{
  if(!edit_udplistenport || !edit_uartport) return false;

  edit_udplistenport->ClearItems();
  edit_uartport->ClearItems();

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const int n = cfg.GetTargetCount();

  // Columns are drawn at tab stops inside the edit-field width (see UI_ELEMENT_COMBO::RebuildMenu).
  for(int i=0; i<n; i++)
    {
      XTRACEMONITOR_CFG_TARGET target;
      if(!cfg.GetTarget(i, target)) continue;

      if(target.type == XTRACEMONITOR_CFG_TARGETTYPE_UDP)
        {
          XSTRING value;
          value.Format(NL("%d"), target.port);
          edit_udplistenport->AddItemColumns(value.Get(), target.name.Get(), target.host.Get(), value.Get());
        }
      else if(target.type == XTRACEMONITOR_CFG_TARGETTYPE_UART)
        {
          edit_uartport->AddItemColumns(target.uartconfig.Get(), target.name.Get(), target.uartconfig.Get(), NULL);
        }
    }

  // Menus are rebuilt lazily on OpenMenu; ensure they exist under the modal father.
  if(edit_udplistenport->GetLayout()) edit_udplistenport->EnsureMenu(edit_udplistenport->GetLayout());
  if(edit_uartport->GetLayout())      edit_uartport->EnsureMenu(edit_uartport->GetLayout());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ReadDraftFromPort()
* @brief      Reads and clamps the UDP listen port from the edittext.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ReadDraftFromPort()
{
  if(!edit_udplistenport) return false;

  // While hardware-editing, keep raw digits in the field (do not clamp-rewrite each keystroke).
  // Accept / focus-loss SanitizeTextToMode clamp the visible text.
  if(GEN_USERINTERFACE.EditText_GetFocus() == (UI_ELEMENT*)edit_udplistenport)
    {
      if(edit_udplistenport->GetText() && !edit_udplistenport->GetText()->IsEmpty())
        {
          udplistenport_draft = XTRACEMONITOR_CFG::GetInstance(false).ResolveUDPListenPort(edit_udplistenport->GetNumericValue());
        }
      return true;
    }

  udplistenport_draft = XTRACEMONITOR_CFG::GetInstance(false).ResolveUDPListenPort(edit_udplistenport->GetNumericValue());
  edit_udplistenport->SetNumericValue(udplistenport_draft);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ReadDraftFromUART()
* @brief      Reads the UART serial-line string from the edittext (validation later).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ReadDraftFromUART()
{
  if(!edit_uartport || !edit_uartport->GetText()) return false;

  uartport_draft = (*edit_uartport->GetText());

  // While editing: keep raw text; Draw_EditText paints the normal focus fill.
  if(GEN_USERINTERFACE.EditText_GetFocus() == (UI_ELEMENT*)edit_uartport)
    {
      return true;
    }

  // Focus lost: empty → default; otherwise keep text and mark valid/invalid (red idle).
  if(uartport_draft.IsEmpty())
    {
      XTRACEMONITOR_CFG::GetInstance(false).ResolveUARTPort(uartport_draft);
      edit_uartport->GetText()->Set(uartport_draft.Get());
    }

  ApplyUARTFieldVisual();
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_uartport);
  if(GEN_USERINTERFACE.Element_IsModalActive()) GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ApplyUARTFieldVisual()
* @brief      Idle fill: gray when UART string is valid, red when invalid (editing fill is drawn by skin).
* @ingroup    UTILITIES
*
* @return     bool : true if the visual was applied.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ApplyUARTFieldVisual()
{
  if(!edit_uartport || !edit_uartport->GetBackgroundColor()) return false;

  uartport_valid = XTRACEMONITOR_CFG::GetInstance(false).IsUARTPortValid(uartport_draft);

  // Idle follows committed --edit-idle-bg; error red is shared by Dark and Light.
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const bool light = (cfg.ResolveColorTheme(cfg.GetColorTheme()) == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);

  UI_COLOR* bg = edit_uartport->GetBackgroundColor();
  UI_COLOR* ink = edit_uartport->GetColor();
  if(uartport_valid)
    {
      if(light)
        {
          bg->SetRed(210);
          bg->SetGreen(214);
          bg->SetBlue(220);
          if(ink) { ink->SetRed(40); ink->SetGreen(44); ink->SetBlue(52); ink->SetAlpha(255); }
        }
       else
        {
          bg->SetRed(64);
          bg->SetGreen(72);
          bg->SetBlue(84);
          if(ink) { ink->SetRed(200); ink->SetGreen(208); ink->SetBlue(220); ink->SetAlpha(255); }
        }
      bg->SetAlpha(255);
    }
   else
    {
      // Same error red for both themes; white glyphs for contrast on red.
      bg->SetRed(160);
      bg->SetGreen(36);
      bg->SetBlue(36);
      bg->SetAlpha(255);
      if(ink) { ink->SetRed(255); ink->SetGreen(255); ink->SetBlue(255); ink->SetAlpha(255); }
    }

  edit_uartport->SetBackgroundColorSet(true);
  if(ink) edit_uartport->SetColorSet(true);
  // Snapshot so :hover/:preselect ReapplyStyleVisual restores error red + white ink (or idle), not load-time.
  edit_uartport->SnapshotStyleVisual();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::SyncDynDNSFromDraft()
* @brief      Pushes DynDNS draft into check + login/password edittexts.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::SyncDynDNSFromDraft()
{
  if(!chk_dyndns_active || !edit_dyndns_login || !edit_dyndns_password) return false;
  if(!edit_dyndns_login->GetText() || !edit_dyndns_password->GetText()) return false;

  chk_dyndns_active->SetStatus(dyndns_active_draft);
  edit_dyndns_login->GetText()->Set(dyndns_login_draft.Get());
  edit_dyndns_password->GetText()->Set(dyndns_password_draft.Get());
  edit_dyndns_login->Cursor_SetPosition(edit_dyndns_login->GetText()->GetSize());
  edit_dyndns_password->Cursor_SetPosition(edit_dyndns_password->GetText()->GetSize());

  GEN_USERINTERFACE.Elements_SetToRedraw(chk_dyndns_active);
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_dyndns_login);
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_dyndns_password);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ReadDraftFromDynDNS()
* @brief      Reads DynDNS login/password draft from the edittexts.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ReadDraftFromDynDNS()
{
  if(!edit_dyndns_login || !edit_dyndns_password) return false;
  if(!edit_dyndns_login->GetText() || !edit_dyndns_password->GetText()) return false;

  dyndns_login_draft    = (*edit_dyndns_login->GetText());
  dyndns_password_draft = (*edit_dyndns_password->GetText());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ApplyDynDNSFieldsEnabled()
* @brief      Enables/disables DynDNS login+password edits from the active draft flag.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ApplyDynDNSFieldsEnabled()
{
  if(!edit_dyndns_login || !edit_dyndns_password) return false;

  const bool on = dyndns_active_draft;
  edit_dyndns_login->SetActive(on);
  edit_dyndns_password->SetActive(on);
  if(lbl_dyndns_login)    lbl_dyndns_login->SetActive(on);
  if(lbl_dyndns_password) lbl_dyndns_password->SetActive(on);

  // Mute label + field chrome when off (SetActive alone does not change paint colors).
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const bool light = (cfg.ResolveColorTheme(cfg.GetColorTheme()) == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);

  auto paint_label = [&](UI_ELEMENT* lbl)
    {
      if(!lbl || !lbl->GetColor()) return;
      UI_COLOR* ink = lbl->GetColor();
      if(on)
        {
          if(light) { ink->SetRed(40);  ink->SetGreen(44);  ink->SetBlue(52);  ink->SetAlpha(255); }
               else { ink->SetRed(200); ink->SetGreen(208); ink->SetBlue(220); ink->SetAlpha(255); }
        }
       else
        {
          // Clearly deactivated vs active label_ink (CSS reapply is defeated by Snapshot).
          if(light) { ink->SetRed(155); ink->SetGreen(160); ink->SetBlue(170); ink->SetAlpha(120); }
               else { ink->SetRed(90);  ink->SetGreen(98);  ink->SetBlue(110); ink->SetAlpha(110); }
        }
      lbl->SetColorSet(true);
      lbl->SnapshotStyleVisual();
    };

  auto paint_edit = [&](UI_ELEMENT_EDITTEXT* edit)
    {
      if(!edit || !edit->GetBackgroundColor()) return;
      UI_COLOR* bg  = edit->GetBackgroundColor();
      UI_COLOR* ink = edit->GetColor();
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
      edit->SetBackgroundColorSet(true);
      if(ink) edit->SetColorSet(true);
      edit->SnapshotStyleVisual();
    };

  paint_label(lbl_dyndns_login);
  paint_label(lbl_dyndns_password);
  paint_edit(edit_dyndns_login);
  paint_edit(edit_dyndns_password);

  GEN_USERINTERFACE.Elements_SetToRedraw(edit_dyndns_login);
  GEN_USERINTERFACE.Elements_SetToRedraw(edit_dyndns_password);
  if(lbl_dyndns_login)    GEN_USERINTERFACE.Elements_SetToRedraw(lbl_dyndns_login);
  if(lbl_dyndns_password) GEN_USERINTERFACE.Elements_SetToRedraw(lbl_dyndns_password);
  if(GEN_USERINTERFACE.Element_IsModalActive()) GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::ApplyDynDNSToolbarButton(bool active)
* @brief      Enables/disables the main-screen DynDNS multioption button.
* @ingroup    UTILITIES
*
* @param[in]  active : Desired enabled state.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::ApplyDynDNSToolbarButton(bool active)
{
  if(!dyndns_btn) return false;

  dyndns_btn->SetActive(active);
  GEN_USERINTERFACE.Elements_SetToRedraw(dyndns_btn);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::EncryptDynDNSPassword(XSTRING& plain, XSTRING& out_b64)
* @brief      AES-CBC encrypt + Base64 using XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD.
* @ingroup    UTILITIES
*
* @param[in]  plain : Plaintext password.
* @param[out] out_b64 : Base64 ciphertext.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::EncryptDynDNSPassword(XSTRING& plain, XSTRING& out_b64)
{
  out_b64.Empty();
  if(plain.IsEmpty()) return true;

  XSTRING passkey;
  passkey = XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD;

  XBUFFER keybuf;
  passkey.ConvertToASCII(keybuf);
  while(keybuf.GetSize() < 16) keybuf.Add((XBYTE)0);
  keybuf.Resize(16);

  XBUFFER ivbuf;
  ivbuf.Add(keybuf);
  ivbuf.Resize(16);

  CIPHERAES            cipherAES;
  CIPHERKEYSYMMETRICAL key;
  key.Set(keybuf);
  cipherAES.SetKey(&key);
  cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);
  cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_SPACES);
  cipherAES.SetInitVector(ivbuf);

  XBUFFER charstr;
  plain.ConvertToASCII(charstr);
  if(!cipherAES.Cipher((XBYTE*)charstr.Get(), plain.GetSize())) return false;

  return cipherAES.GetResult()->ConvertToBase64(out_b64);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::DecryptDynDNSPassword(XSTRING& in_b64, XSTRING& out_plain)
* @brief      Base64 decode + AES-CBC decrypt using XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD.
* @ingroup    UTILITIES
*
* @param[in]  in_b64 : Base64 ciphertext.
* @param[out] out_plain : Plaintext password.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::DecryptDynDNSPassword(XSTRING& in_b64, XSTRING& out_plain)
{
  out_plain.Empty();
  if(in_b64.IsEmpty()) return true;

  XSTRING passkey;
  passkey = XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD;

  XBUFFER keybuf;
  passkey.ConvertToASCII(keybuf);
  while(keybuf.GetSize() < 16) keybuf.Add((XBYTE)0);
  keybuf.Resize(16);

  XBUFFER ivbuf;
  ivbuf.Add(keybuf);
  ivbuf.Resize(16);

  CIPHERAES            cipherAES;
  CIPHERKEYSYMMETRICAL key;
  key.Set(keybuf);
  cipherAES.SetKey(&key);
  cipherAES.SetChainingMode(CIPHERCHAININGMODE_CBC);
  cipherAES.SetPaddingType(XBUFFER_PADDINGTYPE_SPACES);
  cipherAES.SetInitVector(ivbuf);

  XBUFFER xbuffer;
  if(!xbuffer.ConvertFromBase64(in_b64)) return false;
  if(!cipherAES.Uncipher(xbuffer)) return false;

  for(XDWORD c=0; c<cipherAES.GetResult()->GetSize(); c++)
    {
      out_plain.Add((XCHAR)cipherAES.GetResult()->GetByte(c));
    }
  out_plain.DeleteCharacter(0x20, XSTRINGCONTEXT_TO_END);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::OnAccept()
* @brief      Commits draft to CFG, saves the INI (incl. live window rect if save flags on), closes.
* @note       Does not re-apply savedscreenposition/size to the live window — that was snapping
*             the app back to the rect loaded when the modal opened.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::OnAccept()
{
  if(!owner) return false;

  colortheme_draft = ReadDraftFromRadios();
  colortheme_draft = XTRACEMONITOR_CFG::GetInstance(false).ResolveColorTheme(colortheme_draft);
  ReadDraftFromChecks();
  ReadDraftFromPort();
  ReadDraftFromUART();
  ReadDraftFromDynDNS();
  if(chk_dyndns_active) dyndns_active_draft = chk_dyndns_active->GetStatus();

  // UART may be invalid: still leave the modal, but never persist a bad uartport.
  const bool uart_ok = XTRACEMONITOR_CFG::GetInstance(false).IsUARTPortValid(uartport_draft);
  uartport_valid = uart_ok;
  if(!uart_ok)
    {
      // Keep red idle while the modal is still up; discard draft back to last good commit.
      ApplyUARTFieldVisual();
      GEN_USERINTERFACE.Elements_SetToRedraw(edit_uartport);
      if(GEN_USERINTERFACE.Element_IsModalActive()) GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  cfg.SetColorTheme(colortheme_draft);
  cfg.SetSaveScreenPosition(savescreenposition_draft);
  cfg.SetSaveScreenSize(savescreensize_draft);
  // Do not write stale savedscreenposition/size drafts here — Accept must not snap the live
  // window back to the values loaded when the modal opened. Live geometry is captured below
  // via PersistScreenGeometryToCFG when the matching save checkboxes are on.
  cfg.SetUDPListenPort(udplistenport_draft);
  if(uart_ok) cfg.SetUARTPort(&uartport_draft);
  cfg.SetAddDateTimeConsole(adddatetimeconsole_draft);
  cfg.SetAddIPConsole(addipconsole_draft);
  cfg.SetTextTracking(texttracking_draft);

  cfg.SetDynDNSActive(dyndns_active_draft);
  if(dyndns_active_draft)
    {
      cfg.SetDynDNSLogin(&dyndns_login_draft);
      XSTRING cipher;
      if(!EncryptDynDNSPassword(dyndns_password_draft, cipher))
        {
          owner->OpStatus_Log(NL("Config: Aceptar - error al cifrar password DynDNS."));
          return false;
        }
      cfg.SetDynDNSPassword(&cipher);
    }
   else
    {
      // Inactive: persist empty login/password.
      cfg.SetDynDNSLogin(NL(""));
      cfg.SetDynDNSPassword(NL(""));
    }

  if(!cfg.Save())
    {
      owner->OpStatus_Log(NL("Config: Aceptar - error al guardar CFG."));
      return false;
    }

  // With save-position / save-size checked: refresh .ini from the live window so the next
  // start matches what the user sees now (and Accept never re-applies an old rect).
  owner->PersistScreenGeometryToCFG();
  if(cfg.GetSavedScreenPosition()) savedscreenposition_draft = (*cfg.GetSavedScreenPosition());
  if(cfg.GetSavedScreenSize())     savedscreensize_draft     = (*cfg.GetSavedScreenSize());

  colortheme_committed          = colortheme_draft;
  savescreenposition_committed  = savescreenposition_draft;
  savescreensize_committed      = savescreensize_draft;
  adddatetimeconsole_committed  = adddatetimeconsole_draft;
  addipconsole_committed        = addipconsole_draft;
  texttracking_committed        = texttracking_draft;
  savedscreenposition_committed = savedscreenposition_draft;
  savedscreensize_committed     = savedscreensize_draft;
  udplistenport_committed       = udplistenport_draft;
  dyndns_active_committed       = dyndns_active_draft;
  if(dyndns_active_committed)
    {
      dyndns_login_committed    = dyndns_login_draft;
      dyndns_password_committed = dyndns_password_draft;
    }
   else
    {
      dyndns_login_committed.Empty();
      dyndns_password_committed.Empty();
      dyndns_login_draft.Empty();
      dyndns_password_draft.Empty();
    }
  ApplyDynDNSToolbarButton(dyndns_active_committed);

  if(uart_ok)
    {
      uartport_committed = uartport_draft;
      owner->OpStatus_Log(NL("Config: Aceptar - CFG guardado y tema aplicado."));
    }
   else
    {
      uartport_draft = uartport_committed;
      owner->OpStatus_Log(NL("Config: Aceptar - CFG guardado; UART invalida (no se guarda)."));
    }

  if(!owner->ApplyColorTheme(colortheme_committed))
    {
      owner->OpStatus_Log(NL("Config: Aceptar - CFG guardado; error al aplicar tema."));
      return Show(false);
    }

  // Net-status refresh runs on Show(false) after the modal layer is gone (safe TextBackdrop path).
  return ArmCloseAfterPress(config_modal_btn_accept);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::OnCancel()
* @brief      Discards draft, restores radios to committed, closes the modal.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::OnCancel()
{
  if(!owner) return false;

  colortheme_draft               = colortheme_committed;
  savescreenposition_draft       = savescreenposition_committed;
  savescreensize_draft           = savescreensize_committed;
  adddatetimeconsole_draft       = adddatetimeconsole_committed;
  addipconsole_draft             = addipconsole_committed;
  texttracking_draft             = texttracking_committed;
  savedscreenposition_draft      = savedscreenposition_committed;
  savedscreensize_draft          = savedscreensize_committed;
  udplistenport_draft            = udplistenport_committed;
  uartport_draft                 = uartport_committed;
  dyndns_active_draft            = dyndns_active_committed;
  dyndns_login_draft             = dyndns_login_committed;
  dyndns_password_draft          = dyndns_password_committed;
  SyncRadiosFromDraft();
  SyncChecksFromDraft();
  SyncPortFromDraft();
  SyncUARTFromDraft();
  SyncDynDNSFromDraft();
  ApplyDynDNSFieldsEnabled();
  ApplyDynDNSToolbarButton(dyndns_active_committed);

  owner->OpStatus_Log(NL("Config: Cancelar."));
  return ArmCloseAfterPress(config_modal_btn_cancel);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_CFG::OnDefault()
* @brief      Sets draft to factory defaults (CFG DoDefault values) without saving.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_CFG::OnDefault()
{
  if(!owner) return false;

  // Factory defaults (does not rewrite the CFG file until Accept).
  colortheme_draft         = XTRACEMONITOR_CFG_COLORTHEME_DEFAULT;
  savescreenposition_draft = XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT;
  savescreensize_draft     = XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT;
  adddatetimeconsole_draft = XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT;
  addipconsole_draft       = XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT;
  texttracking_draft       = XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT;
  savedscreenposition_draft.Format(NL("%d, %d"), 0, 0);
  savedscreensize_draft.Format(NL("%d, %d"), XTRACEMONITOR_WINDOW_WIDTH, XTRACEMONITOR_WINDOW_HEIGHT);
  udplistenport_draft      = XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  uartport_draft           = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;
  dyndns_active_draft      = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login_draft.Empty();
  dyndns_password_draft.Empty();

  SyncRadiosFromDraft();
  SyncChecksFromDraft();
  SyncPortFromDraft();
  SyncUARTFromDraft();
  SyncDynDNSFromDraft();
  ApplyDynDNSFieldsEnabled();
  ApplyDynDNSToolbarButton(dyndns_active_draft);

  owner->OpStatus_Log(NL("Config: Predeterminado."));
  // Stay open: only hold the Pressed face so the click is visible.
  return HoldPressVisual(config_modal_btn_default);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_CFG::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_CFG::Clean()
{
  owner                     = NULL;
  config_modal              = NULL;
  config_modal_title        = NULL;
  config_modal_btn_accept   = NULL;
  config_modal_btn_cancel   = NULL;
  config_modal_btn_default  = NULL;
  close_timer               = NULL;
  close_armed               = false;
  color_theme_group         = NULL;
  theme_light               = NULL;
  theme_dark                = NULL;
  chk_savescreenposition    = NULL;
  chk_savescreensize        = NULL;
  chk_adddatetimeconsole    = NULL;
  chk_addipconsole          = NULL;
  chk_texttracking          = NULL;
  lbl_udplistenport         = NULL;
  edit_udplistenport        = NULL;
  lbl_uartport              = NULL;
  edit_uartport             = NULL;
  dyndns_group              = NULL;
  chk_dyndns_active         = NULL;
  lbl_dyndns_login          = NULL;
  edit_dyndns_login         = NULL;
  lbl_dyndns_password       = NULL;
  edit_dyndns_password      = NULL;
  dyndns_btn                = NULL;
  colortheme_committed      = XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK;
  colortheme_draft          = XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK;
  savescreenposition_committed = XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT;
  savescreenposition_draft     = XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT;
  savescreensize_committed     = XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT;
  savescreensize_draft         = XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT;
  adddatetimeconsole_committed = XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT;
  adddatetimeconsole_draft     = XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT;
  addipconsole_committed       = XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT;
  addipconsole_draft           = XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT;
  texttracking_committed       = XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT;
  texttracking_draft           = XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT;
  savedscreenposition_committed.Empty();
  savedscreenposition_draft.Empty();
  savedscreensize_committed.Empty();
  savedscreensize_draft.Empty();
  udplistenport_committed   = XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  udplistenport_draft       = XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  uartport_committed        = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;
  uartport_draft            = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;
  uartport_valid            = true;
  dyndns_active_committed   = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_active_draft       = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login_committed.Empty();
  dyndns_login_draft.Empty();
  dyndns_password_committed.Empty();
  dyndns_password_draft.Empty();
}
