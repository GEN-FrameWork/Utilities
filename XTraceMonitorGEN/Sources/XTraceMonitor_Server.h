/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Server.h
*
* @class      XTRACEMONITOR_SERVER
* @brief      XTM wrapper around GEN XTRACESERVER (UDP/UART receive → console feed)
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
#include "XThread.h"

#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
#include "XTraceServer.h"
#include "XTrace.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// Cap lines drained into the UI console per Pump() to keep the frame budget.
// The count cap is the hard stop. The time budget stops a flood earlier so one
// frame cannot spend the whole refresh painting the burst. Leftover messages
// stay queued; the sparkline already shows that depth and GEN queue drops.
#define XTRACEMONITOR_SERVER_MAX_PUMP_PER_FRAME     128
#define XTRACEMONITOR_SERVER_PUMP_BUDGET_US         3000

// Saturation sparkline ring capacity (1 Hz). Visible samples = plot width in px
// (1 sample/px, right-aligned scroll); keep headroom above XTRACEMONITOR_SAT_CHART_W.
#define XTRACEMONITOR_SAT_HISTORY_MAX               1024

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class XTIMER;

class XTRACEMONITOR_SERVER
{
  public:
                                    XTRACEMONITOR_SERVER                    ();
    virtual                        ~XTRACEMONITOR_SERVER                    ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Ini                                     ();
    bool                            End                                     ();
    bool                            RestartFromCFG                          ();

    bool                            IsOpen                                  ();

    void                            SetPriority                             (XTHREADPRIORITY priority);
    void                            SetHighPriority                         (bool enable);
    XTHREADPRIORITY                 GetPriority                             ();

    // Drain GEN queue → console (respects workingmode + filters + console prefix options).
    bool                            Pump                                    ();

    XDWORD                          GetDroppedCount                         ();
    XDWORD                          GetQueuedCount                          ();
    XDWORD                          GetPushedCount                          ();
    XDWORD                          GetFedCount                             ();
    XDWORD                          GetMaxMessages                          ();

  private:

    bool                            OpenFromCFG                             ();
    bool                            CloseAll                                ();

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;
    bool                            highpriority;
    XTHREADPRIORITY                 priority;
    XWORD                           listenport_open;
    bool                            uart_open;

    XDWORD                          fedcount;
    XTIMER*                         pump_timer;

#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
    XTRACESERVER                    server;
    XTRACE_STATUS_MSGS              status_msgs;

    int                             LevelMaskForTraceLevel                  (XBYTE level);
    bool                            MessagePassesFilters                    (XTRACESERVER_MSG& msg);
    bool                            FormatConsoleLine                       (XTRACESERVER_MSG& msg, XSTRING& out);
    bool                            FormatStatusValue                       (XTRACE_STATUS_MSG* status_msg, XSTRING& out);
    bool                            ApplyStatusToValor                      (XTRACE_STATUS_MSG* status_msg);
    bool                            ClearStatusValor                        ();
    bool                            HandleSpecialCommands                   (XTRACESERVER_MSG& msg);
    bool                            FeedConsole                             (XTRACESERVER_MSG& msg);
    bool                            RefreshValorHighlightRedraw             ();
#endif
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
