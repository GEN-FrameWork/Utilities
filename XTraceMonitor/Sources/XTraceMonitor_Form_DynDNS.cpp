/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_DynDNS.cpp
*
* @class      XTRACEMONITOR_FORM_DYNDNS
* @brief      DynDNS modal: updates all [target] UDP hosts to the machine public IP; progress in a mini console.
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_Form_DynDNS.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"
#include "XTraceMonitor_Form_CFG.h"
#include "XTraceMonitor_TranslateID.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XTranslation.h"
#include "XSleep.h"

#include "GRPScreen.h"

#include "UI_Manager.h"
#include "UI_Layout.h"
#include "UI_Skin.h"
#include "UI_Background.h"
#include "UI_Color.h"
#include "UI_BoundaryLine.h"
#include "UI_Element.h"
#include "UI_Element_Form.h"
#include "UI_Element_Console.h"
#include "UI_Element_Option.h"
#include "UI_Element_Text.h"
#include "UI_Property_Scrolleable.h"

#ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
#include "APPFlowExtended.h"
#include "APPFlowExtended_InternetStatus.h"
#include "APPFlowInternetServices.h"
#endif

#ifdef DIO_DYNDNS_ACTIVE
#include "DIODynDNS.h"
#include "DIOURL.h"
#include "DIOIP.h"
#endif

#include <stdarg.h>

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_DYNDNS_LOGLINE::XTRACEMONITOR_DYNDNS_LOGLINE()
* @brief      Constructor of class
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_DYNDNS_LOGLINE::XTRACEMONITOR_DYNDNS_LOGLINE()
{
  color = UI_ELEMENT_CONSOLE_COLOR_GRAY;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_DYNDNS_LOGLINE::~XTRACEMONITOR_DYNDNS_LOGLINE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_DYNDNS_LOGLINE::~XTRACEMONITOR_DYNDNS_LOGLINE()
{
  text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_DYNDNS::XTRACEMONITOR_FORM_DYNDNS()
* @brief      Constructor of class
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_DYNDNS::XTRACEMONITOR_FORM_DYNDNS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_FORM_DYNDNS::~XTRACEMONITOR_FORM_DYNDNS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_FORM_DYNDNS::~XTRACEMONITOR_FORM_DYNDNS()
{
  StopJob(true);

  if(close_timer)
    {
      GEN_XFACTORY.DeleteTimer(close_timer);
      close_timer = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::SetOwner(XTRACEMONITOR* owner)
* @brief      Sets the owning XTraceMonitor application.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::Resolve()
* @brief      Resolves DynDNS modal UI elements.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::Resolve()
{
  dyndns_modal = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("dyndns_modal"), UI_ELEMENT_TYPE_FORM);
  if(!dyndns_modal) return false;

  dyndns_modal_title      = GEN_USERINTERFACE.Element_Get(NL("dyndns_modal_title")     , UI_ELEMENT_TYPE_TEXT);
  dyndns_modal_btn_accept = GEN_USERINTERFACE.Element_Get(NL("dyndns_modal_btn_accept"), UI_ELEMENT_TYPE_MULTIOPTION);
  dyndns_modal_btn_cancel = GEN_USERINTERFACE.Element_Get(NL("dyndns_modal_btn_cancel"), UI_ELEMENT_TYPE_MULTIOPTION);
  dyndns_console          = (UI_ELEMENT_CONSOLE*)GEN_USERINTERFACE.Element_Get(NL("dyndns_console"), UI_ELEMENT_TYPE_CONSOLE);

  if(!dyndns_modal_title || !dyndns_modal_btn_accept || !dyndns_modal_btn_cancel || !dyndns_console) return false;

  UI_LAYOUT* layout = dyndns_modal->GetLayout();
  if(layout && layout->GetBackground() && layout->GetBackground()->GetColor())
    {
      layout->GetBackground()->GetColor()->SetValid(false);
    }

  if(!close_timer)
    {
      close_timer = GEN_XFACTORY.CreateTimer();
      if(!close_timer) return false;
    }

  dyndns_modal->SetVisible(false);
  job_state = XTRACEMONITOR_DYNDNS_JOBSTATE_IDLE;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::ApplyGeometry()
* @brief      Centers the DynDNS modal and places title, console and Accept/Cancel.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::ApplyGeometry()
{
  if(!owner || !dyndns_modal) return false;

  GRPSCREEN* screen = owner->GetMainScreen();
  if(!screen) return false;

  double screen_w = (double)screen->GetWidth();
  double screen_h = (double)screen->GetHeight();
  if((screen_w < 1.0) || (screen_h < 1.0)) return false;

  const double modal_w = XTRACEMONITOR_DYNDNSMODAL_W;
  const double modal_h = XTRACEMONITOR_DYNDNSMODAL_H;
  double modal_x = (screen_w - modal_w) * 0.5;
  double modal_y = (screen_h + modal_h) * 0.5;

  dyndns_modal->SetXPosition(modal_x);
  dyndns_modal->SetYPosition(modal_y);
  dyndns_modal->GetBoundaryLine()->x      = modal_x;
  dyndns_modal->GetBoundaryLine()->y      = modal_y;
  dyndns_modal->GetBoundaryLine()->width  = modal_w;
  dyndns_modal->GetBoundaryLine()->height = modal_h;
  dyndns_modal->SetIntrinsicWidth(modal_w);
  dyndns_modal->SetIntrinsicHeight(modal_h);

  UI_BOUNDARYLINE* vis = dyndns_modal->GetVisibleRect();
  if(vis)
    {
      vis->x = modal_x; vis->y = modal_y; vis->width = modal_w; vis->height = modal_h;
    }

  auto place_child = [](UI_ELEMENT* el, double local_x, double local_y, double w, double h)
    {
      if(!el || !el->GetBoundaryLine()) return;
      el->GetBoundaryLine()->x = local_x;
      el->GetBoundaryLine()->y = local_y;
      el->GetBoundaryLine()->width  = w;
      el->GetBoundaryLine()->height = h;
      el->SetIntrinsicWidth(w);
      el->SetIntrinsicHeight(h);

      UI_ELEMENT* father = el->GetFather();
      double abs_x = local_x;
      double abs_y = local_y;
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
  const double gap       = XTRACEMONITOR_CONFIGMODAL_BTN_GAP;
  const double btn_pad   = XTRACEMONITOR_CONFIGMODAL_BTN_BOTTOM_PAD;

  // Same title pad / TITLE_TO_CONTENT / CONTENT_TO_BTNS rhythm as Config / Filters / About.
  const double title_local_y   = modal_h - title_pad - title_h;
  const double console_top     = title_local_y - XTRACEMONITOR_CONFIGMODAL_TITLE_TO_CONTENT;
  const double console_local_y = btn_pad + btn_h + XTRACEMONITOR_CONFIGMODAL_CONTENT_TO_BTNS;
  double       console_h       = console_top - console_local_y;
  if(console_h < 1.0) console_h = 1.0;

  if(dyndns_modal_title)
    {
      place_child(dyndns_modal_title, title_pad, title_local_y, modal_w - (title_pad * 2.0), title_h);
    }

  if(dyndns_console)
    {
      place_child(dyndns_console, pad, console_local_y, modal_w - (pad * 2.0), console_h);
    }

  const double row_w  = (btn_w * 2.0) + gap;
  const double btn_x0 = (modal_w - row_w) * 0.5;
  place_child(dyndns_modal_btn_accept, btn_x0, btn_pad, btn_w, btn_h);
  place_child(dyndns_modal_btn_cancel, btn_x0 + btn_w + gap, btn_pad, btn_w, btn_h);

  UI_LAYOUT* layout = GEN_USERINTERFACE.Element_GetLayout(dyndns_modal);
  UI_SKIN*   skin   = layout ? layout->GetSkin() : NULL;
  if(skin)
    {
      if(dyndns_modal_btn_accept) skin->CalculeBoundaryLine_AllElements(dyndns_modal_btn_accept, true);
      if(dyndns_modal_btn_cancel) skin->CalculeBoundaryLine_AllElements(dyndns_modal_btn_cancel, true);
      if(dyndns_console)          skin->CalculeBoundaryLine_AllElements(dyndns_console, true);
      if(dyndns_modal_title)      skin->CalculeBoundaryLine_AllElements(dyndns_modal_title, true);

      // AllElements can undo authored sizes — re-pin.
      if(dyndns_modal_title)
        {
          place_child(dyndns_modal_title, title_pad, title_local_y, modal_w - (title_pad * 2.0), title_h);
        }
      if(dyndns_console)
        {
          place_child(dyndns_console, pad, console_local_y, modal_w - (pad * 2.0), console_h);
        }
      place_child(dyndns_modal_btn_accept, btn_x0, btn_pad, btn_w, btn_h);
      place_child(dyndns_modal_btn_cancel, btn_x0 + btn_w + gap, btn_pad, btn_w, btn_h);
    }

  auto center_btn_captions = [](UI_ELEMENT* multi)
    {
      if(!multi || !multi->GetComposeElements()) return;

      const double bw        = XTRACEMONITOR_CONFIGMODAL_BTN_W;
      const double bh        = XTRACEMONITOR_CONFIGMODAL_BTN_H;
      const double icon_clear = XTRACEMONITOR_CONFIGMODAL_BTN_ICON_RIGHT + XTRACEMONITOR_CONFIGMODAL_BTN_ICON_GAP;
      const double pad_r     = XTRACEMONITOR_CONFIGMODAL_BTN_TEXT_PAD_R;

      for(XDWORD c = 0; c < multi->GetComposeElements()->GetSize(); c++)
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

          double local_x = (bw - tw) * 0.5;
          if(local_x < icon_clear) local_x = icon_clear;
          if((local_x + tw) > (bw - pad_r)) local_x = bw - pad_r - tw;
          if(local_x < icon_clear) local_x = icon_clear;

          const double icon_mid_from_top = 24.0;
          double       local_y           = bh - icon_mid_from_top - (sf * 0.5);
          if(local_y < 0.0) local_y = 0.0;

          text->SetXPosition(face->GetXPosition() + local_x);
          text->SetYPosition(face->GetYPosition() - local_y);
          text->GetBoundaryLine()->x      = local_x;
          text->GetBoundaryLine()->y      = local_y;
          text->GetBoundaryLine()->height = sf;
          text->SetMustReDraw(true);
        }
    };

  center_btn_captions(dyndns_modal_btn_accept);
  center_btn_captions(dyndns_modal_btn_cancel);

  dyndns_modal->SetMustReDraw(true);
  if(dyndns_modal_title)      dyndns_modal_title->SetMustReDraw(true);
  if(dyndns_console)          dyndns_console->SetMustReDraw(true);
  if(dyndns_modal_btn_accept) dyndns_modal_btn_accept->SetMustReDraw(true);
  if(dyndns_modal_btn_cancel) dyndns_modal_btn_cancel->SetMustReDraw(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::IsVisible()
* @brief      Returns whether the DynDNS modal is visible.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::IsVisible()
{
  return (dyndns_modal && dyndns_modal->IsVisible());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::Show(bool show)
* @brief      Shows or hides the DynDNS modal; opening starts the update job.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::Show(bool show)
{
  if(!owner || !dyndns_modal) return false;

  close_armed = false;

  if(show)
    {
      if(owner->form_cfg.IsVisible())     owner->form_cfg.Show(false);
      if(owner->form_filters.IsVisible()) owner->form_filters.Show(false);
      if(owner->form_about.IsVisible())   owner->form_about.Show(false);

      if(!ApplyGeometry()) return false;

      if(dyndns_console)
        {
          dyndns_console->Clear();
          dyndns_console->SetAutoscroll(true);
        }

      GEN_USERINTERFACE.SetPreselectElement();
      dyndns_modal->SetVisible(true);
      GEN_USERINTERFACE.Element_SetModal(dyndns_modal);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      GEN_USERINTERFACE.Elements_SetToRedraw();

      if(!StartJob())
        {
          ApplyButtonStates();
          GEN_USERINTERFACE.ModalLayer_Invalidate();
        }

      owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_MODAL_OPEN);
    }
   else
    {
      StopJob(true);

      GEN_USERINTERFACE.Element_SetModal(NULL);
      GEN_USERINTERFACE.ModalLayer_Invalidate();
      dyndns_modal->SetVisible(false);

      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();
      owner->InternetStatus_Update(true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::Tick()
* @brief      Drains worker log lines, updates buttons, and completes deferred close.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::Tick()
{
  if(!IsVisible()) return true;

  PumpLogLines();

  if((job_state == XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING) && job_finished)
    {
      // Worker already Exit()'d; join/delete so it cannot fire again.
      if(thread)
        {
          thread->End();
          DELETEXTHREAD(XTHREADGROUPID_APPOWNER, thread);
          thread = NULL;
        }

      job_state = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      ApplyButtonStates();
      GEN_USERINTERFACE.ModalLayer_Invalidate();
    }

  if(close_armed && close_timer &&
     (close_timer->GetMeasureMilliSeconds() >= (XQWORD)XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS))
    {
      close_armed = false;
      return Show(false);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::HandleElementSelected(UI_ELEMENT* element)
* @brief      Handles DynDNS modal UI selections. Returns true when consumed.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::HandleElementSelected(UI_ELEMENT* element)
{
  if(!element) return false;

  XSTRING* name = element->GetName();
  if(!name) return false;

  if(close_armed)
    {
      if(!name->Compare(NL("dyndns_modal_btn_accept"), true) ||
         !name->Compare(NL("dyndns_modal_btn_cancel"), true))
        {
          return true;
        }
    }

  if(!name->Compare(NL("dyndns-btn"), true))
    {
      // Toolbar bubble must stay inert while DynDNS is off in CFG (face can still receive a
      // stale hit if a child button was not cascaded inactive).
      if(owner && !owner->form_cfg.GetDynDNSActiveCommitted()) return true;
      if(!XTRACEMONITOR_CFG::GetInstance(false).GetDynDNSActive()) return true;
      return Show(true);
    }

  if(!name->Compare(NL("dyndns_modal_btn_accept"), true))
    {
      if(job_state != XTRACEMONITOR_DYNDNS_JOBSTATE_DONE) return true;
      return OnAccept();
    }

  if(!name->Compare(NL("dyndns_modal_btn_cancel"), true))
    {
      if(job_state != XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING) return true;
      return OnCancel();
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::StartJob()
* @brief      Collects UDP target hosts and starts the DynDNS worker thread.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::StartJob()
{
  StopJob(true);

  job_state         = XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING;
  cancel_requested  = false;
  job_finished      = false;
  job_login.Empty();
  job_password.Empty();

  host_queue.DeleteContents();
  host_queue.DeleteAll();

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  // Prefer Form_CFG committed credentials (password already decrypted from INI).
  bool active = cfg.GetDynDNSActive();
  if(owner)
    {
      active = owner->form_cfg.GetDynDNSActiveCommitted();
      owner->form_cfg.GetDynDNSLoginCommitted(job_login);
      owner->form_cfg.GetDynDNSPasswordPlain(job_password);
    }

  if(!active)
    {
      ConsolePrint(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_INACTIVE));
      job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      job_finished = true;
      ApplyButtonStates();
      return false;
    }

  if(job_login.IsEmpty() && cfg.GetDynDNSLogin())
    {
      job_login = (*cfg.GetDynDNSLogin());
    }

  // INI stores AES+Base64 ciphertext only — never pass that to DIODYNDNS.
  if(job_password.IsEmpty() && cfg.GetDynDNSPassword() && !cfg.GetDynDNSPassword()->IsEmpty())
    {
      XSTRING cipher = (*cfg.GetDynDNSPassword());
      if(!XTRACEMONITOR_FORM_CFG::DecryptDynDNSPassword(cipher, job_password))
        {
          job_password.Empty();
        }
    }

  if(job_login.IsEmpty() || job_password.IsEmpty())
    {
      ConsolePrint(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_NO_CREDENTIALS));
      job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      job_finished = true;
      ApplyButtonStates();
      return false;
    }

  const int ntargets = cfg.GetTargetCount();
  for(int i = 0; i < ntargets; i++)
    {
      XTRACEMONITOR_CFG_TARGET target;
      if(!cfg.GetTarget(i, target)) continue;
      if(target.type != XTRACEMONITOR_CFG_TARGETTYPE_UDP) continue;
      if(target.host.IsEmpty()) continue;

      bool duplicate = false;
      for(XDWORD h = 0; h < host_queue.GetSize(); h++)
        {
          XSTRING* existing = host_queue.Get(h);
          if(existing && !existing->Compare(target.host.Get(), true))
            {
              duplicate = true;
              break;
            }
        }
      if(duplicate) continue;

      XSTRING* host = GEN_NEW XSTRING();
      if(!host) continue;
      (*host) = target.host;
      host_queue.Add(host);
    }

  if(!host_queue.GetSize())
    {
      ConsolePrint(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_NO_TARGETS));
      job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      job_finished = true;
      ApplyButtonStates();
      return false;
    }

  if(!mutex)
    {
      mutex = GEN_XFACTORY.Create_Mutex();
      if(!mutex)
        {
          job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
          job_finished = true;
          ApplyButtonStates();
          return false;
        }
    }

  thread = CREATEXTHREAD(XTHREADGROUPID_APPOWNER, NL("XTRACEMONITOR_DYNDNS"), ThreadFunction, this);
  if(!thread)
    {
      job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      job_finished = true;
      ApplyButtonStates();
      return false;
    }

  thread->SetPriority(XTHREADPRIORITY_NORMAL);
  if(!thread->Ini())
    {
      DELETEXTHREAD(XTHREADGROUPID_APPOWNER, thread);
      thread = NULL;
      job_state    = XTRACEMONITOR_DYNDNS_JOBSTATE_DONE;
      job_finished = true;
      ApplyButtonStates();
      return false;
    }

  ApplyButtonStates();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::StopJob(bool wait_thread)
* @brief      Requests cancel and optionally joins the worker thread.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::StopJob(bool wait_thread)
{
  cancel_requested = true;

  if(thread)
    {
      if(wait_thread) thread->End();
      DELETEXTHREAD(XTHREADGROUPID_APPOWNER, thread);
      thread = NULL;
    }

  if(mutex)
    {
      mutex->Lock();
      log_queue.DeleteContents();
      log_queue.DeleteAll();
      mutex->UnLock();

      GEN_XFACTORY.Delete_Mutex(mutex);
      mutex = NULL;
    }

  host_queue.DeleteContents();
  host_queue.DeleteAll();
  job_login.Empty();
  job_password.Empty();
  job_finished = true;
  if(job_state == XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING)
    {
      job_state = XTRACEMONITOR_DYNDNS_JOBSTATE_IDLE;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::PumpLogLines()
* @brief      Moves queued worker lines into the modal console (UI thread).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::PumpLogLines()
{
  if(!mutex || !dyndns_console) return false;

  XVECTOR<XTRACEMONITOR_DYNDNS_LOGLINE*> batch;

  mutex->Lock();
  for(XDWORD c = 0; c < log_queue.GetSize(); c++)
    {
      batch.Add(log_queue.Get(c));
    }
  log_queue.DeleteAll();
  mutex->UnLock();

  if(!batch.GetSize()) return true;

  // Modal mini-console always follows the last line (not user-configurable).
  dyndns_console->SetAutoscroll(true);

  for(XDWORD c = 0; c < batch.GetSize(); c++)
    {
      XTRACEMONITOR_DYNDNS_LOGLINE* line = batch.Get(c);
      if(!line) continue;
      dyndns_console->PrintColor(line->color, line->text.Get());
      GEN_DELETE line;
    }
  batch.DeleteAll();

  // Pin viewport to live tail so the summary line is never left below the clip.
  dyndns_console->Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 1.0);

  dyndns_console->SetMustReDraw(true);
  GEN_USERINTERFACE.Elements_SetToRedraw(dyndns_console);
  GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::ApplyButtonStates()
* @brief      Running → Cancel only; Done → Accept only.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::ApplyButtonStates()
{
  const bool running = (job_state == XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING);
  const bool done    = (job_state == XTRACEMONITOR_DYNDNS_JOBSTATE_DONE);

  if(dyndns_modal_btn_accept)
    {
      dyndns_modal_btn_accept->SetActive(done);
      dyndns_modal_btn_accept->SetMustReDraw(true);
      GEN_USERINTERFACE.Elements_SetToRedraw(dyndns_modal_btn_accept);
    }
  if(dyndns_modal_btn_cancel)
    {
      dyndns_modal_btn_cancel->SetActive(running);
      dyndns_modal_btn_cancel->SetMustReDraw(true);
      GEN_USERINTERFACE.Elements_SetToRedraw(dyndns_modal_btn_cancel);
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::ConsolePrint(XBYTE color, XCHAR* text)
* @brief      Prints immediately on the UI thread (before/without worker).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::ConsolePrint(XBYTE color, XCHAR* text)
{
  if(!dyndns_console || !text) return false;
  dyndns_console->SetAutoscroll(true);
  dyndns_console->PrintColor(color, text);
  dyndns_console->Scroll_SetFraction(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL, 1.0);
  dyndns_console->SetMustReDraw(true);
  GEN_USERINTERFACE.Elements_SetToRedraw(dyndns_console);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::QueueLog(XBYTE color, XCHAR* text)
* @brief      Queues a log line for the UI thread (worker-safe).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::QueueLog(XBYTE color, XCHAR* text)
{
  if(!text) return false;
  if(!mutex)
    {
      return ConsolePrint(color, text);
    }

  XTRACEMONITOR_DYNDNS_LOGLINE* line = GEN_NEW XTRACEMONITOR_DYNDNS_LOGLINE();
  if(!line) return false;
  line->color = color;
  line->text  = text;

  mutex->Lock();
  log_queue.Add(line);
  mutex->UnLock();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::QueueLogf(XBYTE color, XCHAR* mask, ...)
* @brief      Queues a formatted log line (worker-safe).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::QueueLogf(XBYTE color, XCHAR* mask, ...)
{
  if(!mask) return false;

  XSTRING line;
  va_list arg;
  va_start(arg, mask);
  line.FormatArg(mask, &arg);
  va_end(arg);
  return QueueLog(color, line.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::OnAccept()
* @brief      Closes the modal after the job finished.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::OnAccept()
{
  if(!owner) return false;
  owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_MODAL_CLOSE);
  return ArmCloseAfterPress(dyndns_modal_btn_accept);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::OnCancel()
* @brief      Requests cancel while the DynDNS job is running.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::OnCancel()
{
  if(job_state != XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING) return true;

  cancel_requested = true;
  HoldPressVisual(dyndns_modal_btn_cancel);
  if(dyndns_modal_btn_cancel) dyndns_modal_btn_cancel->SetActive(false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::HoldPressVisual(UI_ELEMENT* btn)
* @brief      Brief pressed visual without closing.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::HoldPressVisual(UI_ELEMENT* btn)
{
  if(!btn) return false;
  btn->SetMustReDraw(true);
  GEN_USERINTERFACE.Elements_SetToRedraw(btn);
  GEN_USERINTERFACE.ModalLayer_Invalidate();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::ArmCloseAfterPress(UI_ELEMENT* btn)
* @brief      Arms deferred close after Accept press-hold.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::ArmCloseAfterPress(UI_ELEMENT* btn)
{
  HoldPressVisual(btn);
  close_armed = true;
  if(close_timer) close_timer->Reset();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_DYNDNS::ThreadFunction(void* data)
* @brief      Worker entry: runs DynDNS updates for queued hosts.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_DYNDNS::ThreadFunction(void* data)
{
  XTRACEMONITOR_FORM_DYNDNS* self = (XTRACEMONITOR_FORM_DYNDNS*)data;
  if(!self) return;

  // GEN thread callbacks keep firing while status is RUN — one pass only, then Exit().
  if(self->job_finished) return;

  self->RunJob();
  self->job_finished = true;

  if(self->thread) self->thread->Exit();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_FORM_DYNDNS::RunJob()
* @brief      Resolves public IP and updates each queued DynDNS host.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_FORM_DYNDNS::RunJob()
{
#ifndef DIO_DYNDNS_ACTIVE
  QueueLog(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_API_DISABLED));
  return false;
#else

  bool online = false;
  XSTRING publicip_str;

#ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  APPFLOWEXTENDED_INTERNETSTATUS* inet = APPFLOW_EXTENDED.GetInternetStatus();
  APPFLOWINTERNETSERVICES*        svc  = inet ? inet->GetInternetServices() : NULL;
  if(svc)
    {
      online = svc->HaveInternetConnection();
      XSTRING* pip = svc->GetPublicIP();
      if(pip && !pip->IsEmpty()) publicip_str = (*pip);
    }
#endif

  if(!online)
    {
      QueueLog(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_NO_INTERNET));
      return false;
    }

  DIODYNDNS* dyndns = GEN_NEW DIODYNDNS();
  if(!dyndns)
    {
      QueueLog(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_API_DISABLED));
      return false;
    }

  dyndns->GetLogin()->Set(job_login.Get());
  dyndns->GetPassword()->Set(job_password.Get());

  DIOIP publicip;
  bool  have_ip = false;

  if(!publicip_str.IsEmpty())
    {
      have_ip = publicip.Set(publicip_str.Get());
    }

  if(!have_ip)
    {
      QueueLog(UI_ELEMENT_CONSOLE_COLOR_GRAY, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_RESOLVING_IP));
      have_ip = dyndns->GetPublicIP(publicip, XTRACEMONITOR_DYNDNS_UPDATE_TIMEOUT_SEC);
      if(have_ip) publicip.GetXString(publicip_str);
    }

  if(!have_ip || publicip_str.IsEmpty())
    {
      QueueLog(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_NO_PUBLIC_IP));
      GEN_DELETE dyndns;
      return false;
    }

  QueueLogf(UI_ELEMENT_CONSOLE_COLOR_BLUE, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_PUBLIC_IP), publicip_str.Get());
  QueueLogf(UI_ELEMENT_CONSOLE_COLOR_GRAY, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_HOSTS_COUNT), (int)host_queue.GetSize());

  // Single pass: one Update() per [target] UDP host, then the job ends.
  int ok_count  = 0;
  int err_count = 0;

  for(XDWORD c = 0; c < host_queue.GetSize(); c++)
    {
      if(cancel_requested) break;

      XSTRING* host = host_queue.Get(c);
      if(!host || host->IsEmpty()) continue;

      // Small gap between hosts: DynDNS / edge often stalls under a tight update burst.
      if(c > 0) GEN_XSLEEP.MilliSeconds(XTRACEMONITOR_DYNDNS_HOST_GAP_MS);

      QueueLogf(UI_ELEMENT_CONSOLE_COLOR_GRAY, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_UPDATING), host->Get());

      DIOURL url;
      url.Set(host->Get());

      DIODYNDNS_STATUSCODE status = dyndns->Update(url, publicip, XTRACEMONITOR_DYNDNS_UPDATE_TIMEOUT_SEC);

      if(status == DIODYNDNS_STATUSCODE_GOOD)
        {
          QueueLogf(UI_ELEMENT_CONSOLE_COLOR_GREEN, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_HOST_OK),
                    host->Get());
          ok_count++;
        }
       else if(status == DIODYNDNS_STATUSCODE_NOCHG)
        {
          QueueLogf(UI_ELEMENT_CONSOLE_COLOR_GREEN, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_HOST_OK_NOCHG),
                    host->Get());
          ok_count++;
        }
       else
        {
          XCHAR* code = NL("error");
          switch(status)
            {
              case DIODYNDNS_STATUSCODE_NOTCONNECT : code = NL("notconnect"); break;
              case DIODYNDNS_STATUSCODE_BADAUTH    : code = NL("badauth");    break;
              case DIODYNDNS_STATUSCODE_NOHOST     : code = NL("nohost");     break;
              case DIODYNDNS_STATUSCODE_NOTFQDN    : code = NL("notfqdn");    break;
              case DIODYNDNS_STATUSCODE_ABUSE      : code = NL("abuse");      break;
              case DIODYNDNS_STATUSCODE_YOURS      : code = NL("!yours");     break;
              case DIODYNDNS_STATUSCODE_DNSERR     : code = NL("dnserr");     break;
              case DIODYNDNS_STATUSCODE_911        : code = NL("911");        break;
              default                              : code = NL("unknown");    break;
            }
          QueueLogf(UI_ELEMENT_CONSOLE_COLOR_RED, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_HOST_ERROR),
                    host->Get(), code);
          err_count++;
        }
    }

  GEN_DELETE dyndns;

  if(cancel_requested)
    {
      QueueLog(UI_ELEMENT_CONSOLE_COLOR_PURPLE, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_CANCELLED));
    }
   else
    {
      QueueLogf(UI_ELEMENT_CONSOLE_COLOR_BLUE, TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_DYNDNS_SUMMARY),
                ok_count, err_count);
    }

  return (err_count == 0) && !cancel_requested;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_FORM_DYNDNS::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_FORM_DYNDNS::Clean()
{
  owner                   = NULL;
  dyndns_modal            = NULL;
  dyndns_modal_title      = NULL;
  dyndns_modal_btn_accept = NULL;
  dyndns_modal_btn_cancel = NULL;
  dyndns_console          = NULL;
  close_timer             = NULL;
  close_armed             = false;
  mutex                   = NULL;
  thread                  = NULL;
  cancel_requested        = false;
  job_finished            = false;
  job_state               = XTRACEMONITOR_DYNDNS_JOBSTATE_IDLE;
}
