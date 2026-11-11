/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_Filters.cpp
*
* @class      XTRACEMONITOR_FORM_FILTERS
* @brief      Filters modal: same chrome/geometry as Config; options TBD (CFG [filters])
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

  if(!filters_modal_title || !filters_modal_btn_accept || !filters_modal_btn_cancel || !filters_modal_btn_default)
    {
      return false;
    }

  // Secondary layout inherits the skin page fill; keep it invalid so Layout_PutBackground
  // never canvas-Clears through this layout (would erase the main form under the dialog).
  UI_LAYOUT* modal_layout = GEN_USERINTERFACE.Layouts_Get(NL("filters_modal"));
  if(modal_layout && modal_layout->GetBackground() && modal_layout->GetBackground()->GetColor())
    {
      modal_layout->GetBackground()->GetColor()->SetValid(false);
    }

  if(!LoadCommittedFromCFG()) return false;

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

  const double modal_w = XTRACEMONITOR_CONFIGMODAL_W;
  const double modal_h = XTRACEMONITOR_CONFIGMODAL_H;

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

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(filters_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(filters_modal_btn_default) skin->CalculeBoundaryLine_AllElements(filters_modal_btn_default, true);
      if(filters_modal_btn_accept)  skin->CalculeBoundaryLine_AllElements(filters_modal_btn_accept , true);
      if(filters_modal_btn_cancel)  skin->CalculeBoundaryLine_AllElements(filters_modal_btn_cancel , true);
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

  center_btn_captions(filters_modal_btn_default);
  center_btn_captions(filters_modal_btn_accept);
  center_btn_captions(filters_modal_btn_cancel);

  filters_modal->SetMustReDraw(true);
  if(filters_modal_title)       filters_modal_title->SetMustReDraw(true);
  if(filters_modal_btn_accept)  filters_modal_btn_accept->SetMustReDraw(true);
  if(filters_modal_btn_cancel)  filters_modal_btn_cancel->SetMustReDraw(true);
  if(filters_modal_btn_default) filters_modal_btn_default->SetMustReDraw(true);

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
      // One app modal at a time (same Element_SetModal slot as Config).
      if(owner->form_cfg.IsVisible())
        {
          owner->form_cfg.Show(false);
        }

      if(!ApplyGeometry()) return false;

      if(!LoadCommittedFromCFG()) return false;

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

      // Same as Config: page wipe under the modal must not leave a dead net-status cluster.
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

  if(!name->Compare(NL("filter-btn"), true))
    {
      return Show(true);
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
* @brief      Loads committed filter options from CFG (placeholder until options exist).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::LoadCommittedFromCFG()
{
  // Future filter options live under [filters] in xtracemonitor.ini via XTRACEMONITOR_CFG.
  (void)XTRACEMONITOR_CFG::GetInstance(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_FILTERS::OnAccept()
* @brief      Commits draft filter options to CFG, saves the INI file, closes the modal.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_FILTERS::OnAccept()
{
  if(!owner) return false;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  // Options TBD: assign draft → cfg setters here before Save().
  if(!cfg.Save())
    {
      owner->OpStatus_Log(NL("Filters: Aceptar - error al guardar CFG."));
      return false;
    }

  owner->OpStatus_Log(NL("Filters: Aceptar - CFG guardado."));
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

  // Options TBD: reset draft to XTRACEMONITOR_CFG filter defaults (DoDefault values).
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
  close_timer               = NULL;
  close_armed               = false;
}
