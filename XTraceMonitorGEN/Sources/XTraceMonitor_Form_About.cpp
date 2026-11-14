/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_About.cpp
*
* @class      XTRACEMONITOR_FORM_ABOUT
* @brief      About modal: program info; Accept closes. Opened from the GEN Framework brand icon.
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

#include "XTraceMonitor_Form_About.h"

#include "GRPScreen.h"

#include "UI_Element.h"
#include "UI_Element_Form.h"
#include "UI_Element_Option.h"
#include "UI_Element_Text.h"
#include "UI_Element_Image.h"
#include "UI_Element_Animation.h"
#include "UI_Property_Selectable.h"
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "UI_Manager.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XDateTime.h"
#include "XProcessManager.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_ABOUT::XTRACEMONITOR_FORM_ABOUT()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_ABOUT::XTRACEMONITOR_FORM_ABOUT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_ABOUT::~XTRACEMONITOR_FORM_ABOUT()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_ABOUT::~XTRACEMONITOR_FORM_ABOUT()
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
* @fn         bool XTRACEMONITOR_FORM_ABOUT::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds the form helper to the application instance.
* @ingroup    UTILITIES
*
* @param[in]  owner : Application instance.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (this->owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::Resolve()
* @brief      Resolves about modal elements and seeds static content.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::Resolve()
{
  about_modal = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("about_modal"), UI_ELEMENT_TYPE_FORM);
  if(!about_modal) return false;

  about_modal_title      = GEN_USERINTERFACE.Element_Get(NL("about_modal_title")     , UI_ELEMENT_TYPE_TEXT);
  about_modal_btn_accept = GEN_USERINTERFACE.Element_Get(NL("about_modal_btn_accept"), UI_ELEMENT_TYPE_MULTIOPTION);
  about_brand            = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("about_brand")     , UI_ELEMENT_TYPE_IMAGE);
  about_gen_dark         = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("about_gen_dark")  , UI_ELEMENT_TYPE_IMAGE);
  about_gen_light        = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("about_gen_light") , UI_ELEMENT_TYPE_IMAGE);
  brand_toolbar          = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("brand_toolbar")   , UI_ELEMENT_TYPE_IMAGE);
  about_app_name         = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("about_app_name")     , UI_ELEMENT_TYPE_TEXT);
  about_version          = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("about_version")      , UI_ELEMENT_TYPE_TEXT);
  about_gen_tagline      = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("about_gen_tagline")  , UI_ELEMENT_TYPE_TEXT);
  about_gen_url          = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("about_gen_url")      , UI_ELEMENT_TYPE_TEXT);
  about_copyright        = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("about_copyright")    , UI_ELEMENT_TYPE_TEXT);

  if(!about_modal_title || !about_modal_btn_accept || !about_brand ||
     !about_gen_dark || !about_gen_light || !brand_toolbar ||
     !about_app_name || !about_version || !about_gen_tagline || !about_gen_url || !about_copyright)
    {
      return false;
    }

  // Secondary layout inherits the skin page fill; keep it invalid so Layout_PutBackground
  // never canvas-Clears through this layout (would erase the main form under the dialog).
  UI_LAYOUT* modal_layout = GEN_USERINTERFACE.Layouts_Get(NL("about_modal"));
  if(modal_layout && modal_layout->GetBackground() && modal_layout->GetBackground()->GetColor())
    {
      modal_layout->GetBackground()->GetColor()->SetValid(false);
    }

  about_brand->SetAlpha((XBYTE)XTRACEMONITOR_ABOUTMODAL_ICON_ALPHA);
  about_gen_dark->SetAlpha((XBYTE)XTRACEMONITOR_ABOUTMODAL_GEN_ALPHA);
  about_gen_light->SetAlpha((XBYTE)XTRACEMONITOR_ABOUTMODAL_GEN_ALPHA);

  if(!RefreshContent()) return false;

  // Default dark until ApplyColorTheme / Show seeds the committed theme.
  ApplyTheme(false);

  about_modal->SetVisible(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::RefreshContent()
* @brief      Fills About text lines from APPLICATION_* macros.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::RefreshContent()
{
  if(about_app_name && about_app_name->GetText())
    {
      about_app_name->GetText()->Set(APPLICATION_NAMEAPP);
      about_app_name->SetMustReDraw(true);
    }

  if(about_version && about_version->GetText())
    {
      XSTRING ver;
      ver.Empty();
      ver.AddFormat(NL("%d.%d.%d"), APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
      about_version->GetText()->Set(ver.Get());

      // Same light blue as the GEN Framework mark on the main toolbar (not theme label_ink).
      if(about_version->GetColor())
        {
          about_version->GetColor()->SetRed  ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_R);
          about_version->GetColor()->SetGreen((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_G);
          about_version->GetColor()->SetBlue ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_B);
          about_version->GetColor()->SetAlpha(255);
          about_version->GetColor()->SetValid(true);
          about_version->SetColorSet(true);
        }
      about_version->SetMustReDraw(true);
    }

  if(about_gen_url && about_gen_url->GetText())
    {
      about_gen_url->GetText()->Set(XTRACEMONITOR_GENFRAMEWORK_URL_TEXT);
      if(about_gen_url->GetColor())
        {
          about_gen_url->GetColor()->SetRed  ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_R);
          about_gen_url->GetColor()->SetGreen((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_G);
          about_gen_url->GetColor()->SetBlue ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_B);
          about_gen_url->GetColor()->SetAlpha(255);
          about_gen_url->GetColor()->SetValid(true);
          about_gen_url->SetColorSet(true);
        }
      about_gen_url->SetMustReDraw(true);
    }

  if(about_copyright && about_copyright->GetText())
    {
      // APPLICATION_YEAROFCREATION = first year. Same as runtime year → one year; else start-current.
      const int year_start = APPLICATION_YEAROFCREATION;
      int       year_now   = year_start;
      XDATETIME* now = GEN_XFACTORY.CreateDateTime();
      if(now)
        {
          now->Read(true);
          year_now = now->GetYear();
          GEN_XFACTORY.DeleteDateTime(now);
        }
      if(year_now < year_start) year_now = year_start;

      XSTRING copy;
      copy.Empty();
      if(year_now == year_start)
            copy.AddFormat(NL("Copyright (c) %d EndoraSoft S.L."), year_start);
       else copy.AddFormat(NL("Copyright (c) %d-%d EndoraSoft S.L."), year_start, year_now);
      about_copyright->GetText()->Set(copy.Get());
      about_copyright->SetMustReDraw(true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::ApplyGeometry()
* @brief      Centers the About modal and lays out title, brand, texts and Accept.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::ApplyGeometry()
{
  if(!owner || !about_modal) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  if(!screen) return false;

  double screen_w = (double)screen->GetWidth();
  double screen_h = (double)screen->GetHeight();
  if((screen_w < 1.0) || (screen_h < 1.0)) return false;

  const double modal_w = XTRACEMONITOR_ABOUTMODAL_W;
  const double modal_h = XTRACEMONITOR_ABOUTMODAL_H;

  double modal_x = (screen_w - modal_w) * 0.5;
  double modal_y = (screen_h + modal_h) * 0.5;

  about_modal->SetXPosition(modal_x);
  about_modal->SetYPosition(modal_y);
  about_modal->GetBoundaryLine()->x      = modal_x;
  about_modal->GetBoundaryLine()->y      = modal_y;
  about_modal->GetBoundaryLine()->width  = modal_w;
  about_modal->GetBoundaryLine()->height = modal_h;
  about_modal->SetIntrinsicWidth(modal_w);
  about_modal->SetIntrinsicHeight(modal_h);

  UI_BOUNDARYLINE* vis = about_modal->GetVisibleRect();
  if(vis)
    {
      vis->x      = modal_x;
      vis->y      = modal_y;
      vis->width  = modal_w;
      vis->height = modal_h;
    }

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

  const double pad       = XTRACEMONITOR_CONFIGMODAL_PAD;
  const double title_pad = XTRACEMONITOR_CONFIGMODAL_TITLE_PAD;
  const double title_h   = XTRACEMONITOR_CONFIGMODAL_TITLE_H;
  const double btn_w     = XTRACEMONITOR_CONFIGMODAL_BTN_W;
  const double btn_h     = XTRACEMONITOR_CONFIGMODAL_BTN_H;
  const double btn_y     = XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD;
  const double brand_w   = XTRACEMONITOR_ABOUTMODAL_ICON_W;
  const double brand_h   = XTRACEMONITOR_ABOUTMODAL_ICON_H;
  const double gen_w     = XTRACEMONITOR_ABOUTMODAL_GEN_W;
  const double gen_h     = XTRACEMONITOR_ABOUTMODAL_GEN_H;
  const double line_h    = 22.0;
  const double line_gap  = 6.0;

  if(about_modal_title)
    {
      const double title_local_y = modal_h - title_pad - title_h;
      place_child(about_modal_title, title_pad, title_local_y, modal_w - (title_pad * 2.0), title_h);
    }

  place_child(about_modal_btn_accept, (modal_w - btn_w) * 0.5, btn_y, btn_w, btn_h);

  // Title → app icon → name / version → tagline → GEN → URL → copyright → Accept.
  const double title_to_brand = 20.0;
  const double brand_to_name  = 14.0;
  const double ver_to_tag     = 26.0; // tagline raised 4px vs version
  const double tag_to_gen     = 12.0; // keep GEN/URL/copyright where they were
  const double gen_to_url     = 6.0;
  const double url_to_copy    = 14.0;
  const double brand_y        = modal_h - title_pad - title_h - title_to_brand - brand_h;
  const double name_y         = brand_y - brand_to_name - line_h;
  const double ver_y          = name_y - line_gap - 20.0;
  const double tag_y          = ver_y - ver_to_tag - 20.0;
  const double gen_y          = tag_y - tag_to_gen - gen_h;
  const double url_y          = gen_y - gen_to_url - 18.0;
  const double copy_y         = url_y - url_to_copy - 20.0;
  const double text_w         = modal_w - (pad * 2.0);

  place_child((UI_ELEMENT*)about_brand, (modal_w - brand_w) * 0.5, brand_y, brand_w, brand_h);
  place_child((UI_ELEMENT*)about_app_name    , pad, name_y, text_w, line_h);
  place_child((UI_ELEMENT*)about_version     , pad, ver_y , text_w, 20.0);
  place_child((UI_ELEMENT*)about_gen_tagline , pad, tag_y , text_w, 20.0);
  place_child((UI_ELEMENT*)about_gen_dark , (modal_w - gen_w) * 0.5, gen_y, gen_w, gen_h);
  place_child((UI_ELEMENT*)about_gen_light, (modal_w - gen_w) * 0.5, gen_y, gen_w, gen_h);
  // URL centered under the GEN logo (full modal width + text-align center).
  place_child((UI_ELEMENT*)about_gen_url   , pad, url_y , text_w, 18.0);
  place_child((UI_ELEMENT*)about_copyright , pad, copy_y, text_w, 20.0);

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(about_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(about_modal_btn_accept) skin->CalculeBoundaryLine_AllElements(about_modal_btn_accept, true);
      if(about_brand)            skin->CalculeBoundaryLine_AllElements(about_brand, true);
      if(about_gen_dark)         skin->CalculeBoundaryLine_AllElements(about_gen_dark, true);
      if(about_gen_light)        skin->CalculeBoundaryLine_AllElements(about_gen_light, true);
      if(about_app_name)         skin->CalculeBoundaryLine_AllElements(about_app_name, true);
      if(about_version)          skin->CalculeBoundaryLine_AllElements(about_version, true);
      if(about_gen_tagline)      skin->CalculeBoundaryLine_AllElements(about_gen_tagline, true);
      if(about_gen_url)          skin->CalculeBoundaryLine_AllElements(about_gen_url, true);
      if(about_copyright)        skin->CalculeBoundaryLine_AllElements(about_copyright, true);
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

          text->GetBoundaryLine()->x = local_x;
          text->GetBoundaryLine()->y = local_y;
          text->SetXPosition(face->GetXPosition() + local_x);
          text->SetYPosition(face->GetYPosition() - local_y);
          text->SetMustReDraw(true);
        }
    };

  center_btn_captions(about_modal_btn_accept);

  about_modal->SetMustReDraw(true);
  if(about_modal_title)      about_modal_title->SetMustReDraw(true);
  if(about_modal_btn_accept) about_modal_btn_accept->SetMustReDraw(true);
  if(about_brand)            about_brand->SetMustReDraw(true);
  if(about_gen_dark)         about_gen_dark->SetMustReDraw(true);
  if(about_gen_light)        about_gen_light->SetMustReDraw(true);
  if(about_app_name)         about_app_name->SetMustReDraw(true);
  if(about_version)          about_version->SetMustReDraw(true);
  if(about_gen_tagline)      about_gen_tagline->SetMustReDraw(true);
  if(about_gen_url)          about_gen_url->SetMustReDraw(true);
  if(about_copyright)        about_copyright->SetMustReDraw(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::ApplyTheme(bool light)
* @brief      Shows the black GEN mark on Light theme and the white mark on Dark theme.
* @ingroup    UTILITIES
*
* @param[in]  light : true = Light theme (black GEN logo); false = Dark (white GEN logo).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::ApplyTheme(bool light)
{
  if(!about_gen_dark || !about_gen_light) return false;

  // Only one twin visible — same shared-slot idea as Stop/Run (no stacked soft-PNG cost).
  about_gen_dark->SetVisible(!light);
  about_gen_light->SetVisible(light);
  about_gen_dark->SetMustReDraw(true);
  about_gen_light->SetMustReDraw(true);

  // Version stays GEN toolbar blue through Dark/Light (not remapped with label_ink).
  if(about_version && about_version->GetColor())
    {
      about_version->GetColor()->SetRed  ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_R);
      about_version->GetColor()->SetGreen((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_G);
      about_version->GetColor()->SetBlue ((XBYTE)XTRACEMONITOR_ABOUTMODAL_VERSION_B);
      about_version->GetColor()->SetAlpha(255);
      about_version->GetColor()->SetValid(true);
      about_version->SetColorSet(true);
      about_version->SetMustReDraw(true);
    }

  if(IsVisible())
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(about_gen_dark, true);
      GEN_USERINTERFACE.Elements_SetToRedraw(about_gen_light, true);
      if(about_version) GEN_USERINTERFACE.Elements_SetToRedraw(about_version, true);
      if(about_modal) GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::IsVisible()
* @brief      True when the about modal form is currently shown.
* @ingroup    UTILITIES
*
* @return     bool : true if visible; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::IsVisible()
{
  return (about_modal && about_modal->IsVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::HoldPressVisual(UI_ELEMENT* btn)
* @brief      Forces the multioption Pressed face and holds it for PRESS_HOLD_MS.
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption (Accept).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::HoldPressVisual(UI_ELEMENT* btn)
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
  if(about_modal)
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(about_modal, true);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::ArmCloseAfterPress(UI_ELEMENT* btn)
* @brief      Shows Pressed for PRESS_HOLD_MS, then closes the modal on Tick().
* @ingroup    UTILITIES
*
* @param[in]  btn : Modal multioption that was clicked.
*
* @return     bool : true if armed (or already armed); otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::ArmCloseAfterPress(UI_ELEMENT* btn)
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
* @fn         bool XTRACEMONITOR_FORM_ABOUT::Tick()
* @brief      Completes a deferred Accept close after the press hold.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::Tick()
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
* @fn         bool XTRACEMONITOR_FORM_ABOUT::Show(bool show)
* @brief      Shows or hides the about modal as a GEN modal layer.
* @ingroup    UTILITIES
*
* @param[in]  show : true to open; false to close.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::Show(bool show)
{
  if(!owner || !about_modal) return false;

  close_armed = false;

  if(show)
    {
      if(owner->form_cfg.IsVisible())     owner->form_cfg.Show(false);
      if(owner->form_filters.IsVisible()) owner->form_filters.Show(false);

      {
        XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
        const bool light = (cfg.ResolveColorTheme(cfg.GetColorTheme()) == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);
        ApplyTheme(light);
      }

      if(!RefreshContent()) return false;
      if(!ApplyGeometry()) return false;

      GEN_USERINTERFACE.SetPreselectElement();
      about_modal->SetVisible(true);
      GEN_USERINTERFACE.Element_SetModal(about_modal);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      owner->OpStatus_Log(NL("About modal open."));
    }
   else
    {
      GEN_USERINTERFACE.Element_SetModal(NULL);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      about_modal->SetVisible(false);

      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      owner->InternetStatus_Update(true);

      owner->OpStatus_Log(NL("About modal closed."));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::HandleElementSelected(UI_ELEMENT* element)
* @brief      Handles about-modal UI selections. Returns true when the event was consumed.
* @ingroup    UTILITIES
*
* @param[in]  element : Selected element.
*
* @return     bool : true if consumed by this form; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::HandleElementSelected(UI_ELEMENT* element)
{
  if(!element) return false;

  XSTRING* name = element->GetName();
  if(!name) return false;

  if(close_armed)
    {
      if(!name->Compare(NL("about_modal_btn_accept"), true)) return true;
    }

  if(!name->Compare(NL("about_modal_btn_accept"), true))
    {
      return OnAccept();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::BrandToolbar_HitTest(int screen_x, int screen_y)
* @brief      True when the pointer is over the GEN Framework toolbar logo (opens About on click).
* @ingroup    UTILITIES
*
* @param[in]  screen_x : Cursor X in screen / canvas coords.
* @param[in]  screen_y : Cursor Y in screen / canvas coords (top-down, same as OpStatus_HitTest).
*
* @return     bool : true if the point hits the brand image; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::BrandToolbar_HitTest(int screen_x, int screen_y)
{
  if(!brand_toolbar || !brand_toolbar->IsVisible() || !brand_toolbar->GetBoundaryLine()) return false;
  if(GEN_USERINTERFACE.Element_IsModalActive()) return false;

  double x = brand_toolbar->GetXPosition();
  double y = brand_toolbar->GetYPosition();
  double w = brand_toolbar->GetBoundaryLine()->width;
  double h = brand_toolbar->GetBoundaryLine()->height;
  double top = y - h;

  if((double)screen_x < x || (double)screen_x >= (x + w)) return false;
  if((double)screen_y < top || (double)screen_y >= y) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::GenUrl_HitTest(int screen_x, int screen_y)
* @brief      True when the pointer is over the GEN Framework URL text link in About.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::GenUrl_HitTest(int screen_x, int screen_y)
{
  if(!IsVisible() || !about_gen_url || !about_gen_url->IsVisible() || !about_gen_url->GetBoundaryLine()) return false;

  double x = about_gen_url->GetXPosition();
  double y = about_gen_url->GetYPosition();
  double w = about_gen_url->GetBoundaryLine()->width;
  double h = about_gen_url->GetBoundaryLine()->height;
  double top = y - h;

  if((double)screen_x < x || (double)screen_x >= (x + w)) return false;
  if((double)screen_y < top || (double)screen_y >= y) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::OpenGenFrameworkPage()
* @brief      Opens the GEN Framework site via GEN_XPROCESSMANAGER.OpenURL (singleton).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::OpenGenFrameworkPage()
{
  #ifdef XPROCESSMANAGER_ACTIVE
  if(!XPROCESSMANAGER::GetIsInstanced())
    {
      if(owner) owner->OpStatus_Log(NL("About: XPROCESSMANAGER not instanced."));
      return false;
    }

  if(!GEN_XPROCESSMANAGER.OpenURL(XTRACEMONITOR_GENFRAMEWORK_URL_OPEN))
    {
      if(owner) owner->OpStatus_Log(NL("About: failed to open GEN Framework URL."));
      return false;
    }

  if(owner) owner->OpStatus_Log(NL("About: opened GEN Framework page."));
  return true;
  #else
  if(owner) owner->OpStatus_Log(NL("About: XPROCESSMANAGER not active."));
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_ABOUT::OnAccept()
* @brief      Closes the About modal after the Accept press hold.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_ABOUT::OnAccept()
{
  if(!owner) return false;

  owner->OpStatus_Log(NL("About: Aceptar."));
  return ArmCloseAfterPress(about_modal_btn_accept);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_ABOUT::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_ABOUT::Clean()
{
  owner                  = NULL;
  about_modal            = NULL;
  about_modal_title      = NULL;
  about_modal_btn_accept = NULL;
  about_brand            = NULL;
  about_gen_dark         = NULL;
  about_gen_light        = NULL;
  brand_toolbar          = NULL;
  about_app_name         = NULL;
  about_version          = NULL;
  about_gen_tagline      = NULL;
  about_gen_url          = NULL;
  about_copyright        = NULL;
  close_timer            = NULL;
  close_armed            = false;
}
