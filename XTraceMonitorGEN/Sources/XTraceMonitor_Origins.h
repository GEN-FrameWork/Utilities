/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Origins.h
*
* @brief      Tree origins + single console history (BelongsTo + replay)
* @ingroup    UTILITIES
*
* @copyright  EndoraSoft. All rights reserved.
*
*---------------------------------------------------------------------------------------------------------------------*/

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
#define XTRACEMONITOR_ORIGINS_HISTORY_DEFAULT_MAX 8000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct UI_ELEMENT_TREE_NODE;
class  UI_ELEMENT_TREE;
class  UI_ELEMENT_CONSOLE;
class  UI_ELEMENT_PROPERTYLIST;
class  XTRACEMONITOR;

class XTRACEMONITOR_ORIGIN
{
  public:
                                    XTRACEMONITOR_ORIGIN            ();
    virtual                        ~XTRACEMONITOR_ORIGIN            ();

    XTRACEMONITOR_ORIGIN_KIND       kind;
    XDWORD                          publicIP;
    XDWORD                          localIP;
    bool                            public_from_peer;               // legacy IP* label
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

    bool                            ApplyStatusLine                 (XSTRING* line, XDWORD publicIP, XDWORD localIP);
    bool                            ClearSelectedStatus             ();
    bool                            RebuildSelectedValor            ();

    void                            SetHistoryMax                   (XDWORD maxlines);

    bool                            RefreshIPCounters               ();
    bool                            QueuePublicIPGeolocation        ();

  private:

    XTRACEMONITOR_ORIGIN*           AddOrigin                       (XTRACEMONITOR_ORIGIN_KIND kind, XDWORD publicIP, XDWORD localIP, XCHAR* label, XCHAR* icon, UI_ELEMENT_TREE_NODE* father, bool public_from_peer = false);
    XTRACEMONITOR_ORIGIN*           FindExact                       (XDWORD publicIP, XDWORD localIP);
    XTRACEMONITOR_ORIGIN*           FindCFGByPublicIP               (XDWORD publicIP);

    bool                            FormatIPLabel                   (XDWORD ip, bool starred, XSTRING& out);
    bool                            IconForIP                       (XDWORD ip, XCHAR*& icon);
    bool                            TrimHistory                     ();

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
