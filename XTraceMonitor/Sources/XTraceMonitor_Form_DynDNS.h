/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_DynDNS.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifndef XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS
#define XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS           300
#endif

#define XTRACEMONITOR_DYNDNSMODAL_W                     720.0
#define XTRACEMONITOR_DYNDNSMODAL_H                     540.0
#define XTRACEMONITOR_DYNDNS_UPDATE_TIMEOUT_SEC            12
#define XTRACEMONITOR_DYNDNS_HOST_GAP_MS                400

enum XTRACEMONITOR_DYNDNS_JOBSTATE
{
  XTRACEMONITOR_DYNDNS_JOBSTATE_IDLE = 0 ,
  XTRACEMONITOR_DYNDNS_JOBSTATE_RUNNING  ,
  XTRACEMONITOR_DYNDNS_JOBSTATE_DONE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT;
class UI_ELEMENT_FORM;
class UI_ELEMENT_CONSOLE;
class XTIMER;
class XMUTEX;
class XTHREADCOLLECTED;

class XTRACEMONITOR_DYNDNS_LOGLINE
{
  public:

                                    XTRACEMONITOR_DYNDNS_LOGLINE            ();
                                   ~XTRACEMONITOR_DYNDNS_LOGLINE            ();

    XSTRING                         text;
    XBYTE                           color;
};


class XTRACEMONITOR_FORM_DYNDNS
{
  public:
                                    XTRACEMONITOR_FORM_DYNDNS               ();
    virtual                        ~XTRACEMONITOR_FORM_DYNDNS               ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            ApplyGeometry                           ();
    bool                            Show                                    (bool show);
    bool                            IsVisible                               ();
    bool                            Tick                                    ();

    bool                            HandleElementSelected                   (UI_ELEMENT* element);

  private:

    bool                            StartJob                                ();
    bool                            StopJob                                 (bool wait_thread);
    bool                            PumpLogLines                            ();
    bool                            ApplyButtonStates                       ();
    bool                            ConsolePrint                            (XBYTE color, XCHAR* text);
    bool                            QueueLog                                (XBYTE color, XCHAR* text);
    bool                            QueueLogf                               (XBYTE color, XCHAR* mask, ...);
    bool                            OnAccept                                ();
    bool                            OnCancel                                ();
    bool                            HoldPressVisual                         (UI_ELEMENT* btn);
    bool                            ArmCloseAfterPress                      (UI_ELEMENT* btn);

    static void                     ThreadFunction                          (void* data);
    bool                            RunJob                                  ();

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;

    UI_ELEMENT_FORM*                dyndns_modal;
    UI_ELEMENT*                     dyndns_modal_title;
    UI_ELEMENT*                     dyndns_modal_btn_accept;
    UI_ELEMENT*                     dyndns_modal_btn_cancel;
    UI_ELEMENT_CONSOLE*             dyndns_console;

    XTIMER*                         close_timer;
    bool                            close_armed;

    XMUTEX*                         mutex;
    XTHREADCOLLECTED*               thread;
    XVECTOR<XTRACEMONITOR_DYNDNS_LOGLINE*> log_queue;
    XVECTOR<XSTRING*>               host_queue;

    XSTRING                         job_login;
    XSTRING                         job_password;
    volatile bool                   cancel_requested;
    volatile bool                   job_finished;
    XTRACEMONITOR_DYNDNS_JOBSTATE   job_state;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
