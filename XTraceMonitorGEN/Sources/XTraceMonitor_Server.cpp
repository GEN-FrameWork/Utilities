/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Server.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_Server.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"
#include "XTraceMonitor_TranslateID.h"

#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Manager.h"

#include "XTrace.h"
#include "XString.h"
#include "XTimer.h"

#ifdef DIO_ACTIVE
#include "DIOIP.h"
#endif

// Color index lives in the low nibble; XTRACE_PRINTCOLOR sets XTRACE_LEVEL_WITHCOLOR (0x80).
#define XTRACEMONITOR_SERVER_LEVEL_COLORINDEX(level)  ((XBYTE)((level) & 0x0F))

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_SERVER::XTRACEMONITOR_SERVER()
* @brief      Constructor of class
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_SERVER::XTRACEMONITOR_SERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_SERVER::~XTRACEMONITOR_SERVER()
* @brief      Destructor of class
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_SERVER::~XTRACEMONITOR_SERVER()
{
  End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::SetOwner(XTRACEMONITOR* owner)
* @brief      Sets the owning XTM instance (console / workingmode / status).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::Ini()
* @brief      Opens UDP (and UART when CFG is valid) via GEN XTRACESERVER.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::Ini()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  server.SetPriority(priority);
  // Accept WM_COPYDATA even before UDP/UART open; traces route to Local (no internet).
  XTRACESERVER::SetCopyDataReceiver(&server);
  return OpenFromCFG();
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::End()
* @brief      Closes UDP/UART listen paths.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::End()
{
  return CloseAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::RestartFromCFG()
* @brief      Re-opens listen paths from the current CFG (port / UART change after Accept).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::RestartFromCFG()
{
  CloseAll();
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  XTRACESERVER::SetCopyDataReceiver(&server);
#endif
  return OpenFromCFG();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::IsOpen()
* @brief      True when at least one GEN receive path is open.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::IsOpen()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  return server.IsOpen();
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_SERVER::SetPriority(XTHREADPRIORITY priority)
* @brief      Sets GEN server parse/stream thread priority.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_SERVER::SetPriority(XTHREADPRIORITY priority)
{
  if(priority == XTHREADPRIORITY_UNKNOWN) priority = XTHREADPRIORITY_NORMAL;
  this->priority = priority;
  this->highpriority = (priority >= XTHREADPRIORITY_HIGH);

#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  server.SetPriority(priority);
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_SERVER::SetHighPriority(bool enable)
* @brief      HIGH when enable, otherwise NORMAL (does not use REALTIME).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_SERVER::SetHighPriority(bool enable)
{
  highpriority = enable;
  SetPriority(enable ? XTHREADPRIORITY_HIGH : XTHREADPRIORITY_NORMAL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTHREADPRIORITY XTRACEMONITOR_SERVER::GetPriority()
* @brief      Current requested thread priority.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTHREADPRIORITY XTRACEMONITOR_SERVER::GetPriority()
{
  return priority;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::Pump()
* @brief      Pops queued GEN messages and feeds the console when working.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::Pump()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  if(!owner) return false;
  // UDP/UART open OR pending WM_COPYDATA queue (IPC does not require a stream).
  if(!server.IsOpen() && !server.GetCount()) return false;

  XDWORD fed = 0;

  for(XDWORD n = 0; n < XTRACEMONITOR_SERVER_MAX_PUMP_PER_FRAME; n++)
    {
      XTRACESERVER_MSG msg;
      if(!server.Pop(msg)) break;

      // Status / clear / clear-screen always applied; console text only while working.
      if(HandleSpecialCommands(msg))
        {
          fed++;
          continue;
        }

      if(!owner->workingmode) continue;

      if(!MessagePassesFilters(msg)) continue;

      if(FeedConsole(msg))
        {
          fedcount++;
          fed++;
        }
    }

  RefreshValorHighlightRedraw();

  return (fed > 0);
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_SERVER::GetDroppedCount()
* @brief      Messages dropped by the GEN bounded queue.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_SERVER::GetDroppedCount()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  return server.GetDroppedCount();
#else
  return 0;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_SERVER::GetQueuedCount()
* @brief      Messages waiting in the GEN queue.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_SERVER::GetQueuedCount()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  return server.GetCount();
#else
  return 0;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_SERVER::GetPushedCount()
* @brief      Total messages pushed into the GEN queue.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_SERVER::GetPushedCount()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  return server.GetPushedCount();
#else
  return 0;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_SERVER::GetFedCount()
* @brief      Total messages successfully painted to the console.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_SERVER::GetFedCount()
{
  return fedcount;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_SERVER::GetMaxMessages()
* @brief      GEN queue capacity.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_SERVER::GetMaxMessages()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  return server.GetMaxMessages();
#else
  return 0;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::OpenFromCFG()
* @brief      Opens UDP listen port and optional UART from XTRACEMONITOR_CFG.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::OpenFromCFG()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  bool ok = false;

  server.SetPriority(priority);

#if defined(DIO_STREAMUDP_ACTIVE)
  {
    const int port = cfg.ResolveUDPListenPort(cfg.GetUDPListenPort());
    if(server.Ini((XWORD)port))
      {
        listenport_open = (XWORD)port;
        ok = true;
        if(owner)
          {
            XSTRING port_s;
            port_s.Format(NL("%d"), (int)listenport_open);
            owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_TRACE_UDP_OPEN, port_s.Get());
          }
      }
     else if(owner)
      {
        XSTRING port_s;
        port_s.Format(NL("%d"), port);
        owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_TRACE_UDP_FAIL, port_s.Get());
      }
  }
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  {
    XSTRING* uart = cfg.GetUARTPort();
    if(uart && cfg.IsUARTPortValid(uart))
      {
        if(server.IniUART(uart))
          {
            uart_open = true;
            ok = true;
            if(owner) owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_TRACE_UART_OPEN);
          }
         else if(owner)
          {
            owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_TRACE_UART_FAIL);
          }
      }
  }
#endif

  return ok;
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::CloseAll()
* @brief      Ends the GEN server (UDP + UART).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::CloseAll()
{
#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))
  XTRACESERVER::SetCopyDataReceiver(NULL);
  server.End();
#endif
  listenport_open = 0;
  uart_open       = false;
  return true;
}


#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_SERVER::LevelMaskForTraceLevel(XBYTE level)
* @brief      Maps XTrace color index 0..5 (low nibble) to the CFG filter bitmask.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_SERVER::LevelMaskForTraceLevel(XBYTE level)
{
  // No WITHCOLOR → default/black ink (looks gray on dark theme); still gated by Black bit.
  if((level & XTRACE_LEVEL_WITHCOLOR) != XTRACE_LEVEL_WITHCOLOR)
    {
      return XTRACEMONITOR_CFG_FILTER_LEVEL_BLACK;
    }

  return XTRACEMONITOR_CFG_LevelMaskForColorIndex((int)XTRACEMONITOR_SERVER_LEVEL_COLORINDEX(level));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::MessagePassesFilters(XTRACESERVER_MSG& msg)
* @brief      Level bitmask + optional text substring filter from CFG [filters].
* @note       Uncolored lines count as Black so "no colors checked" hides the default gray-looking ink.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::MessagePassesFilters(XTRACESERVER_MSG& msg)
{
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  const int mask = LevelMaskForTraceLevel(msg.level);
  if((cfg.Filters_GetLevels() & mask) == 0) return false;

  if(cfg.Filters_IsTextActive())
    {
      XSTRING* filter = cfg.Filters_GetText();
      if(filter && !filter->IsEmpty())
        {
          if(msg.text.Find(filter->Get(), false) == XSTRING_NOTFOUND) return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::FormatConsoleLine(XTRACESERVER_MSG& msg, XSTRING& out)
* @brief      Builds the console line with optional date/time and IP prefixes from CFG [console].
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::FormatConsoleLine(XTRACESERVER_MSG& msg, XSTRING& out)
{
  out.Empty();

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  if(cfg.GetAddDateTimeConsole())
    {
      XSTRING dt;
      msg.time.GetDateTimeToString(XDATETIME_FORMAT_STANDARD | XDATETIME_FORMAT_TIMEWITHMILLISECONDS, dt);
      if(!dt.IsEmpty())
        {
          out.Add(dt);
          out.Add(NL(" "));
        }
    }

  if(cfg.GetAddIPConsole())
    {
      // Prefer public when it is a real remote address; otherwise local (DIOIP::IsLocal / empty).
      DIOIP showip;
      DIOIP publicip;

      if(msg.publicIP)
        {
          publicip.Set((XBYTE)((msg.publicIP >> 24) & 0xFF),
                       (XBYTE)((msg.publicIP >> 16) & 0xFF),
                       (XBYTE)((msg.publicIP >>  8) & 0xFF),
                       (XBYTE)( msg.publicIP        & 0xFF));
          if(!publicip.IsEmpty() && !publicip.IsLocal() && !publicip.IsAPIPA()) showip.Set(publicip.Get());
        }

      if(showip.IsEmpty() && msg.localIP)
        {
          showip.Set((XBYTE)((msg.localIP >> 24) & 0xFF),
                     (XBYTE)((msg.localIP >> 16) & 0xFF),
                     (XBYTE)((msg.localIP >>  8) & 0xFF),
                     (XBYTE)( msg.localIP        & 0xFF));
        }

      if(showip.IsEmpty() && !publicip.IsEmpty()) showip.Set(publicip.Get());

      // Fixed width like legacy XTM: always [XXX.XXX.XXX.XXX] (zero-padded octets via DIOIP::Get).
      XBYTE* a = showip.Get();
      XSTRING ip;
      ip.Format(NL("[%03d.%03d.%03d.%03d] "), (int)a[0], (int)a[1], (int)a[2], (int)a[3]);
      out.Add(ip);
    }

  out.Add(msg.text);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::FormatStatusValue(XTRACE_STATUS_MSG* status_msg, XSTRING& out)
* @brief      Formats the display value for the valor property list.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::FormatStatusValue(XTRACE_STATUS_MSG* status_msg, XSTRING& out)
{
  out.Empty();
  if(!status_msg) return false;

  switch(status_msg->GetType())
    {
      case XTRACE_TYPE_STATUS_MSG_BOOLEAN :
        out = status_msg->Value_GetBoolean() ? NL("true") : NL("false");
        break;

      case XTRACE_TYPE_STATUS_MSG_INTEGER :
        out.Format(NL("%d"), status_msg->Value_GetInteger());
        break;

      case XTRACE_TYPE_STATUS_MSG_STRING  :
        if(status_msg->Value_GetString()) out = status_msg->Value_GetString()->Get();
        break;

      case XTRACE_TYPE_STATUS_MSG_XDWORD  :
        out.Format(NL("%u"), status_msg->Value_GetDword());
        break;

      case XTRACE_TYPE_STATUS_MSG_FLOAT   :
        out.Format(NL("%f"), status_msg->Value_GetFloat());
        break;

      case XTRACE_TYPE_STATUS_MSG_RGBCOLOR:
        {
          XBYTE* c = status_msg->Value_GetColor();
          if(c) out.Format(NL("%02X,%02X,%02X"), (int)c[0], (int)c[1], (int)c[2]);
        }
        break;

      default:
        break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::ApplyStatusToValor(XTRACE_STATUS_MSG* status_msg)
* @brief      Adds or updates a Name/Value row in the valor property list (marks changed → red).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::ApplyStatusToValor(XTRACE_STATUS_MSG* status_msg)
{
  if(!owner || !owner->gui_varstatus.GetElement() || !status_msg) return false;
  if(!status_msg->GetName() || status_msg->GetName()->IsEmpty()) return false;

  XSTRING value;
  FormatStatusValue(status_msg, value);

  // SetValue finds by name: update existing or Add new; markchanged resets row XTIMER (red for 10s).
  if(!owner->gui_varstatus.GetElement()->SetValue(status_msg->GetName()->Get(), value.Get(), true)) return false;

  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::ClearStatusValor()
* @brief      Clears GEN status cache and the valor property list.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::ClearStatusValor()
{
  status_msgs.StatusMsg_DeleteAll();

  if(owner && owner->gui_varstatus.GetElement())
    {
      owner->gui_varstatus.GetElement()->Clear();
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::RefreshValorHighlightRedraw()
* @brief      Keeps valor dirty while any row is still inside the red highlight window.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::RefreshValorHighlightRedraw()
{
  if(!owner || !owner->gui_varstatus.GetElement()) return false;

  if(owner->gui_varstatus.GetElement()->HasChangedHighlight(UI_ELEMENT_PROPERTYLIST_CHANGED_HIGHLIGHT_SECONDS))
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::HandleSpecialCommands(XTRACESERVER_MSG& msg)
* @brief      Handles XTRACE control strings (clear screen / status / clear status).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::HandleSpecialCommands(XTRACESERVER_MSG& msg)
{
  if(!owner) return false;

  if(msg.text.Find(XTRACE_IDMSGCLEARSCREEN, false) != XSTRING_NOTFOUND)
    {
      owner->origins.ClearHistoryMatching(msg.publicIP, msg.localIP);
      return true;
    }

  if(msg.text.Find(XTRACE_IDMSGSTATUSCLEAR, false) != XSTRING_NOTFOUND)
    {
      owner->origins.ApplyStatusLine(&msg.text, msg.publicIP, msg.localIP);
      return true;
    }

  if(msg.text.Find(XTRACE_IDMSGSTATUS, false) != XSTRING_NOTFOUND)
    {
      owner->origins.ApplyStatusLine(&msg.text, msg.publicIP, msg.localIP);
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_SERVER::FeedConsole(XTRACESERVER_MSG& msg)
* @brief      Appends one filtered message to the graphical console.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_SERVER::FeedConsole(XTRACESERVER_MSG& msg)
{
  if(!owner || !owner->gui_console.GetElement()) return false;

  owner->origins.EnsureFromMessage(msg.publicIP, msg.localIP);

  XSTRING line;
  FormatConsoleLine(msg, line);

  // Packet level is flags|index (e.g. XTRACE_PRINTCOLOR(2) → 0x82). Color = low nibble when WITHCOLOR.
  XBYTE color = UI_ELEMENT_CONSOLE_COLOR_BLACK;
  if((msg.level & XTRACE_LEVEL_WITHCOLOR) == XTRACE_LEVEL_WITHCOLOR)
    {
      color = XTRACEMONITOR_SERVER_LEVEL_COLORINDEX(msg.level);
      if(color > UI_ELEMENT_CONSOLE_COLOR_GRAY) color = UI_ELEMENT_CONSOLE_COLOR_BLACK;
    }

  if(!owner->origins.PushLine(msg.publicIP, msg.localIP, color, line)) return false;

  // Session follow is applied at resolve, toggle and replay. AddLine already
  // honors the current autoscroll flag; resyncing here dirties the follow button
  // once per message (up to the pump cap per frame).
  return true;
}

#endif // DIO + UDP/UART


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_SERVER::Clean()
* @brief      Clean
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_SERVER::Clean()
{
  owner           = NULL;
  highpriority    = true;
  priority        = XTHREADPRIORITY_HIGH;
  listenport_open = 0;
  uart_open       = false;
  fedcount        = 0;
}
