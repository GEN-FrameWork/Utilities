/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Origins.h
*
* @class      XTRACEMONITOR_ORIGINS
* @brief      Tree origins + single console history (BelongsTo + replay)
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
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XVector.h"
#include "XTrace.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XTRACEMONITOR_ORIGIN_KIND
{
  XTRACEMONITOR_ORIGIN_KIND_ALL = 0 ,
  XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET ,
  XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD ,
  XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT ,
  XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL ,
  XTRACEMONITOR_ORIGIN_KIND_UART ,
  XTRACEMONITOR_ORIGIN_KIND_CFG
};

#define XTRACEMONITOR_ORIGIN_UART_IP              0xFFFFFFFFu
// WM_COPYDATA local IP packed by XWINDOWSTRACE (1.1.1.1); maps to Local (no internet).
#define XTRACEMONITOR_ORIGIN_COPYDATA_LOCALIP     0x01010101u
#define XTRACEMONITOR_ORIGINS_HISTORY_DEFAULT_MAX 8000
#define XTRACEMONITOR_SAVED_DIR                   NL("saved")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct UI_ELEMENT_TREE_NODE;
class  XTRACEMONITOR;

class XTRACEMONITOR_ORIGIN
{
  public:
                                    XTRACEMONITOR_ORIGIN            ();
    virtual                        ~XTRACEMONITOR_ORIGIN            ();

    XTRACEMONITOR_ORIGIN_KIND       kind;
    XDWORD                          publicIP;
    XDWORD                          localIP;
    bool                            public_from_peer;
    XSTRING                         name;
    UI_ELEMENT_TREE_NODE*           node;
    XTRACE_STATUS_MSGS              status_msgs;

  private:

    void                            Clean                           ();
};


class XTRACEMONITOR_ORIGIN_LINE
{
  public:
                                    XTRACEMONITOR_ORIGIN_LINE       ();
    virtual                        ~XTRACEMONITOR_ORIGIN_LINE       ();

    XDWORD                          publicIP;
    XDWORD                          localIP;
    XBYTE                           color;
    XSTRING                         text;

  private:

    void                            Clean                           ();
};


class XTRACEMONITOR_ORIGINS
{
  public:
                                    XTRACEMONITOR_ORIGINS           ();
    virtual                        ~XTRACEMONITOR_ORIGINS           ();

    bool                            SetOwner                        (XTRACEMONITOR* owner);

    bool                            SeedTree                        ();
    bool                            UpdateUARTLabel                 ();
    bool                            RefreshFixedLabels              ();

    bool                            EnsureFromMessage               (XDWORD publicIP, XDWORD localIP);
    bool                            BelongsTo                       (XTRACEMONITOR_ORIGIN* origin, XDWORD publicIP, XDWORD localIP);

    XTRACEMONITOR_ORIGIN*           GetSelected                     ();
    XTRACEMONITOR_ORIGIN*           GetFromNode                     (UI_ELEMENT_TREE_NODE* node);
    bool                            SyncSelectionFromTree           ();

    bool                            PushLine                        (XDWORD publicIP, XDWORD localIP, XBYTE color, XSTRING& text);
    bool                            ReplaySelectedToConsole         ();
    bool                            ClearSelectedHistory            ();
    bool                            ClearHistoryMatching            (XDWORD publicIP, XDWORD localIP);
    bool                            ClearReceivedTraces             ();
    bool                            SaveSelectedConsole             (XSTRING* out_filename = NULL);

    bool                            ApplyStatusLine                 (XSTRING* line, XDWORD publicIP, XDWORD localIP);
    bool                            ClearSelectedStatus             ();
    bool                            RebuildSelectedValor            ();

    void                            SetHistoryMax                   (XDWORD maxlines);
    XDWORD                          GetHistoryMax                   ();
    bool                            ApplyHistoryMaxFromCFG          ();

    bool                            QueuePublicIPGeolocation        ();
    bool                            RefreshIPCounters               ();

  private:

    XTRACEMONITOR_ORIGIN*           AddOrigin                       (XTRACEMONITOR_ORIGIN_KIND kind, XDWORD publicIP, XDWORD localIP, XCHAR* label, XCHAR* icon, UI_ELEMENT_TREE_NODE* father, bool public_from_peer = false);
    XTRACEMONITOR_ORIGIN*           FindExact                       (XDWORD publicIP, XDWORD localIP);
    XTRACEMONITOR_ORIGIN*           FindCFGByPublicIP               (XDWORD publicIP);
    XTRACEMONITOR_ORIGIN*           ResolveOwnerOrigin              (XDWORD publicIP, XDWORD localIP);

    XDWORD                          CountLinesForOrigin             (XTRACEMONITOR_ORIGIN* origin);
    bool                            RecycleOriginIfNeeded           (XTRACEMONITOR_ORIGIN* origin);
    bool                            FormatIPLabel                   (XDWORD ip, bool starred, XSTRING& out);
    bool                            IconForIP                       (XDWORD ip, XCHAR*& icon);
    bool                            SanitizeFileToken               (XSTRING& token);
    bool                            ResolveCFGOriginName            (XDWORD publicIP, XSTRING& out);
    bool                            BuildSaveFileName               (XTRACEMONITOR_ORIGIN* origin, XSTRING& out);
    bool                            BuildRecycleStatusLabel         (XTRACEMONITOR_ORIGIN* origin, XSTRING& out);

    void                            Clean                           ();

    XTRACEMONITOR*                  owner;
    XVECTOR<XTRACEMONITOR_ORIGIN*>  origins;
    XVECTOR<XTRACEMONITOR_ORIGIN_LINE*> history;
    XDWORD                          history_max;

    XTRACEMONITOR_ORIGIN*           root;
    XTRACEMONITOR_ORIGIN*           local_no_internet;
    XTRACEMONITOR_ORIGIN*           uart;
    XTRACEMONITOR_ORIGIN*           selected;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
