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
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "UI_Manager.h"

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

  if(!config_modal_title || !config_modal_btn_accept || !config_modal_btn_cancel || !config_modal_btn_default)
    {
      return false;
    }

  if(!color_theme_group || !theme_light || !theme_dark)
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

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(config_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(config_modal_btn_default) skin->CalculeBoundaryLine_AllElements(config_modal_btn_default, true);
      if(config_modal_btn_accept)  skin->CalculeBoundaryLine_AllElements(config_modal_btn_accept , true);
      if(config_modal_btn_cancel)  skin->CalculeBoundaryLine_AllElements(config_modal_btn_cancel , true);
      if(color_theme_group)        skin->CalculeBoundaryLine_AllElements(color_theme_group, true);
    }

  auto center_btn_captions = [](UI_ELEMENT* multi)
    {
      if(!multi || !multi->GetComposeElements()) return;

      const double btn_w      = XTRACEMONITOR_CONFIGMODAL_BTN_W;
      const double btn_h      = XTRACEMONITOR_CONFIGMODAL_BTN_H;
      const double icon_clear = XTRACEMONITOR_CONFIGMODAL_BTN_ICON_RIGHT + XTRACEMONITOR_CONFIGMODAL_BTN_ICON_GAP;
      const double pad_r      = XTRACEMONITOR_CONFIGMODAL_BTN_TEXT_PAD_R;
      const double optical_y  = 2.0;

      for(XDWORD c=0; c<multi->GetComposeElements()->GetSize(); c++)
        {
          UI_ELEMENT* face = multi->GetComposeElements()->Get(c);
          if(!face || (face->GetType() != UI_ELEMENT_TYPE_BUTTON && face->GetType() != UI_ELEMENT_TYPE_OPTION)) continue;

          UI_ELEMENT_OPTION* opt  = (UI_ELEMENT_OPTION*)face;
          UI_ELEMENT_TEXT*   text = opt->Get_UIText();
          if(!text || !text->GetBoundaryLine()) continue;

          double tw = text->GetBoundaryLine()->width;
          double th = text->GetBoundaryLine()->height;
          if(tw < 1.0) tw = 1.0;
          if(th < 1.0) th = 1.0;

          double local_x = (btn_w - tw) * 0.5;
          if(local_x < icon_clear) local_x = icon_clear;
          if((local_x + tw) > (btn_w - pad_r)) local_x = btn_w - pad_r - tw;
          if(local_x < icon_clear) local_x = icon_clear;

          double local_y = (btn_h - th) * 0.5 + optical_y;

          text->SetXPosition(face->GetXPosition() + local_x);
          text->SetYPosition(face->GetYPosition() - local_y);
          text->GetBoundaryLine()->x = local_x;
          text->GetBoundaryLine()->y = local_y;
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

  if(show)
    {
      if(!ApplyGeometry()) return false;

      // Draft starts from the last committed CFG value (radios only — no runtime theme apply yet).
      colortheme_draft = colortheme_committed;
      if(!SyncRadiosFromDraft()) return false;

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
* @fn         bool XTRACEMONITOR_FORM_CFG::OnAccept()
* @brief      Commits draft to CFG, saves the INI file, closes the modal.
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

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  cfg.SetColorTheme(colortheme_draft);

  if(!cfg.Save())
    {
      owner->OpStatus_Log(NL("Config: Aceptar - error al guardar CFG."));
      return false;
    }

  colortheme_committed = colortheme_draft;

  if(!owner->ApplyColorTheme(colortheme_committed))
    {
      owner->OpStatus_Log(NL("Config: Aceptar - CFG guardado; error al aplicar tema."));
      return Show(false);
    }

  owner->OpStatus_Log(NL("Config: Aceptar - CFG guardado y tema aplicado."));
  return Show(false);
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

  colortheme_draft = colortheme_committed;
  SyncRadiosFromDraft();

  owner->OpStatus_Log(NL("Config: Cancelar."));
  return Show(false);
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

  // Factory default for colortheme only (does not rewrite the whole CFG file).
  colortheme_draft = XTRACEMONITOR_CFG_COLORTHEME_DEFAULT;
  SyncRadiosFromDraft();

  owner->OpStatus_Log(NL("Config: Predeterminado."));
  return true;
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
  color_theme_group         = NULL;
  theme_light               = NULL;
  theme_dark                = NULL;
  colortheme_committed      = XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK;
  colortheme_draft          = XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK;
}
