/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Origins.cpp
*
* @brief      Tree origins + single console history (BelongsTo + replay)
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_Origins.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "UI_Element_Tree.h"
#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Manager.h"

#include "DIOIP.h"
#include "DIODNSResolver.h"
#include "XTimer.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


static bool XTraceMonitor_Origins_FormatStatusValue(XTRACE_STATUS_MSG* status_msg, XSTRING& out)
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


XTRACEMONITOR_ORIGIN::XTRACEMONITOR_ORIGIN()
{
  Clean();
}


XTRACEMONITOR_ORIGIN::~XTRACEMONITOR_ORIGIN()
{
  status_msgs.StatusMsg_DeleteAll();
  Clean();
}


void XTRACEMONITOR_ORIGIN::Clean()
{
  kind              = XTRACEMONITOR_ORIGIN_KIND_ALL;
  publicIP          = 0;
  localIP           = 0;
  public_from_peer  = false;
  name.Empty();
  node              = NULL;
}


XTRACEMONITOR_ORIGIN_LINE::XTRACEMONITOR_ORIGIN_LINE()
{
  Clean();
}


XTRACEMONITOR_ORIGIN_LINE::~XTRACEMONITOR_ORIGIN_LINE()
{
  Clean();
}


void XTRACEMONITOR_ORIGIN_LINE::Clean()
{
  publicIP = 0;
  localIP  = 0;
  color    = 0;
  text.Empty();
}


XTRACEMONITOR_ORIGINS::XTRACEMONITOR_ORIGINS()
{
  Clean();
}


XTRACEMONITOR_ORIGINS::~XTRACEMONITOR_ORIGINS()
{
  history.DeleteContents();
  history.DeleteAll();
  origins.DeleteContents();
  origins.DeleteAll();
  Clean();
}


bool XTRACEMONITOR_ORIGINS::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


void XTRACEMONITOR_ORIGINS::SetHistoryMax(XDWORD maxlines)
{
  if(maxlines < 100) maxlines = 100;
  history_max = maxlines;
  TrimHistory();
}


bool XTRACEMONITOR_ORIGINS::FormatIPLabel(XDWORD ip, bool starred, XSTRING& out)
{
  DIOIP dioip;
  dioip.Set((XBYTE)((ip >> 24) & 0xFF),
            (XBYTE)((ip >> 16) & 0xFF),
            (XBYTE)((ip >>  8) & 0xFF),
            (XBYTE)( ip        & 0xFF));

  XSTRING plain;
  dioip.GetXString(plain);

  if(starred) out.Format(NL("%s*"), plain.Get());
   else       out = plain;

  return true;
}


bool XTRACEMONITOR_ORIGINS::IconForIP(XDWORD ip, XCHAR*& icon)
{
  icon = NL("tree/ip.png");
  if(!ip) return true;

  DIOIP dioip;
  dioip.Set((XBYTE)((ip >> 24) & 0xFF),
            (XBYTE)((ip >> 16) & 0xFF),
            (XBYTE)((ip >>  8) & 0xFF),
            (XBYTE)( ip        & 0xFF));

  if(dioip.IsLocal() || dioip.IsAPIPA()) icon = NL("tree/packet.png");
  return true;
}


XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::AddOrigin(XTRACEMONITOR_ORIGIN_KIND kind, XDWORD publicIP, XDWORD localIP, XCHAR* label, XCHAR* icon, UI_ELEMENT_TREE_NODE* father, bool public_from_peer)
{
  if(!owner || !owner->gui_tree.GetElement() || !label) return NULL;

  UI_ELEMENT_TREE_NODE* node = owner->gui_tree.GetElement()->AddNode(father, label, icon);
  if(!node) return NULL;

  XTRACEMONITOR_ORIGIN* origin = GEN_NEW XTRACEMONITOR_ORIGIN();
  if(!origin) return NULL;

  origin->kind             = kind;
  origin->publicIP         = publicIP;
  origin->localIP          = localIP;
  origin->public_from_peer = public_from_peer;
  origin->name.Set(label);
  origin->node             = node;

  origins.Add(origin);
  return origin;
}


XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::FindExact(XDWORD publicIP, XDWORD localIP)
{
  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(!origin) continue;
      if(origin->kind == XTRACEMONITOR_ORIGIN_KIND_CFG) continue;
      if(origin->kind == XTRACEMONITOR_ORIGIN_KIND_ALL) continue;
      if(origin->kind == XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET) continue;

      if((origin->publicIP == publicIP) && (origin->localIP == localIP)) return origin;
    }

  return NULL;
}


XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::FindCFGByPublicIP(XDWORD publicIP)
{
  if(!publicIP) return NULL;

  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(!origin || origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG) continue;
      if(origin->publicIP == publicIP) return origin;
    }

  return NULL;
}


bool XTRACEMONITOR_ORIGINS::SeedTree()
{
  if(!owner || !owner->gui_tree.GetElement()) return false;

  history.DeleteContents();
  history.DeleteAll();
  origins.DeleteContents();
  origins.DeleteAll();

  // Tree wipe: drop geolocation + map cache so CFG re-seed re-fetches.
  owner->geoip.ClearCache();

  root = local_no_internet = uart = selected = NULL;
  owner->gui_tree.SetUARTNode(NULL);

  owner->gui_tree.GetElement()->Clear();

  root = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_ALL, 0, 0, NL("All"), NL("tree/all.png"), NULL);
  if(!root) return false;

  local_no_internet = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET, 0, 0,
                                NL("# Local (no internet)"), NL("tree/local.png"), root->node);
  if(!local_no_internet) return false;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const int norigins = cfg.GetOriginCount();

  for(int i = 0; i < norigins; i++)
    {
      XTRACEMONITOR_CFG_ORIGIN cfgorigin;
      if(!cfg.GetOrigin(i, cfgorigin)) continue;
      if(cfgorigin.name.IsEmpty()) continue;

      XSTRING display_ip = cfgorigin.ip;
      XDWORD  packed_ip  = 0;

      if(display_ip.IsEmpty() && !cfgorigin.url.IsEmpty())
        {
          DIOIP resolved;
          if(GEN_DIODNSRESOLVER.ResolveURL(cfgorigin.url.Get(), resolved))
            {
              resolved.GetXString(display_ip);
            }
        }

      if(!display_ip.IsEmpty())
        {
          DIOIP dioip;
          if(dioip.Set(display_ip))
            {
              XBYTE* a = dioip.Get();
              packed_ip = ((XDWORD)a[0] << 24) | ((XDWORD)a[1] << 16) | ((XDWORD)a[2] << 8) | (XDWORD)a[3];
            }
        }

      XSTRING label = cfgorigin.name;
      if(cfgorigin.showip && !display_ip.IsEmpty())
        {
          label.Format(NL("%s (%s)"), cfgorigin.name.Get(), display_ip.Get());
        }

      XCHAR* icon = NL("tree/ip.png");
      if(packed_ip) IconForIP(packed_ip, icon);

      AddOrigin(XTRACEMONITOR_ORIGIN_KIND_CFG, packed_ip, 0, label.Get(), icon, root->node);
      if(packed_ip) owner->geoip.Request(packed_ip);
    }

  XSTRING uart_label;
  // Label built by owner helper for Target name match.
  if(owner->gui_tree.BuildUARTLabel(uart_label))
    {
      uart = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_UART,
                       XTRACEMONITOR_ORIGIN_UART_IP, XTRACEMONITOR_ORIGIN_UART_IP,
                       uart_label.Get(), NL("tree/uart.png"), root->node);
      if(uart) owner->gui_tree.SetUARTNode(uart->node);
    }

  owner->gui_tree.GetElement()->Expand(root->node);
  owner->gui_tree.GetElement()->SetSelected(root->node);
  selected = root;

  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
  RefreshIPCounters();
  return true;
}


bool XTRACEMONITOR_ORIGINS::UpdateUARTLabel()
{
  if(!owner || !uart || !uart->node) return false;

  XSTRING label;
  if(!owner->gui_tree.BuildUARTLabel(label)) return false;
  if(!uart->node->text.Compare(label.Get())) return true;

  uart->node->text.Set(label.Get());
  uart->name.Set(label.Get());
  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
  return true;
}


bool XTRACEMONITOR_ORIGINS::BelongsTo(XTRACEMONITOR_ORIGIN* origin, XDWORD publicIP, XDWORD localIP)
{
  if(!origin) return false;

  const bool is_uart = (publicIP == XTRACEMONITOR_ORIGIN_UART_IP) && (localIP == XTRACEMONITOR_ORIGIN_UART_IP);

  switch(origin->kind)
    {
      case XTRACEMONITOR_ORIGIN_KIND_ALL :
        return !is_uart;

      case XTRACEMONITOR_ORIGIN_KIND_UART :
        return is_uart;

      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET :
        return (!publicIP && localIP && !is_uart);

      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD :
        return (!publicIP && localIP && (localIP == origin->localIP));

      case XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT :
      case XTRACEMONITOR_ORIGIN_KIND_CFG :
        return (publicIP && (publicIP == origin->publicIP));

      case XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL :
        return (publicIP && localIP &&
                (publicIP == origin->publicIP) &&
                (localIP  == origin->localIP));

      default :
        return false;
    }
}


bool XTRACEMONITOR_ORIGINS::EnsureFromMessage(XDWORD publicIP, XDWORD localIP)
{
  if(!owner || !owner->gui_tree.GetElement() || !root) return false;

  if((publicIP == XTRACEMONITOR_ORIGIN_UART_IP) && (localIP == XTRACEMONITOR_ORIGIN_UART_IP))
    {
      return (uart != NULL);
    }

  // Local-only under "# Local (no internet)".
  if(!publicIP && localIP)
    {
      if(FindExact(0, localIP)) return true;
      if(!local_no_internet || !local_no_internet->node) return false;

      XSTRING label;
      FormatIPLabel(localIP, false, label);

      XCHAR* icon = NL("tree/packet.png");
      AddOrigin(XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD, 0, localIP, label.Get(), icon, local_no_internet->node);
      // Keep only All expanded by default; Local folder stays collapsed until the user opens it.
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
      RefreshIPCounters();
      return true;
    }

  if(!publicIP) return false;

  // Public parent: prefer CFG node with same IP; else create/find dynamic parent.
  XTRACEMONITOR_ORIGIN* parent = FindCFGByPublicIP(publicIP);
  if(!parent) parent = FindExact(publicIP, 0);

  bool added = false;

  if(!parent)
    {
      XSTRING label;
      FormatIPLabel(publicIP, false, label);

      XCHAR* icon = NL("tree/ip.png");
      IconForIP(publicIP, icon);

      parent = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT, publicIP, 0, label.Get(), icon, root->node);
      if(!parent) return false;
      added = true;
      owner->geoip.Request(publicIP);
      // Ensure All stays open so the new public leaf is visible; do not expand the public node itself.
      owner->gui_tree.GetElement()->Expand(root->node);
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
    }
   else if(parent && parent->publicIP)
    {
      // CFG / existing public parent: ensure geolocation is queued once.
      owner->geoip.Request(parent->publicIP);
    }

  if(localIP)
    {
      if(FindExact(publicIP, localIP))
        {
          if(added) RefreshIPCounters();
          return true;
        }

      XSTRING label;
      FormatIPLabel(localIP, false, label);

      XCHAR* icon = NL("tree/packet.png");
      AddOrigin(XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL, publicIP, localIP, label.Get(), icon, parent->node);
      added = true;
      // Public/local children stay collapsed until the user expands that IP group.
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
    }

  if(added) RefreshIPCounters();
  return true;
}


XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::GetSelected()
{
  return selected;
}


XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::GetFromNode(UI_ELEMENT_TREE_NODE* node)
{
  if(!node) return NULL;

  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(origin && origin->node == node) return origin;
    }

  return NULL;
}


bool XTRACEMONITOR_ORIGINS::SyncSelectionFromTree()
{
  if(!owner || !owner->gui_tree.GetElement()) return false;

  UI_ELEMENT_TREE_NODE* node = owner->gui_tree.GetElement()->GetSelected();
  XTRACEMONITOR_ORIGIN* next = GetFromNode(node);
  if(!next) next = root;

  if(next == selected) return false;

  selected = next;
  ReplaySelectedToConsole();
  RebuildSelectedValor();
  return true;
}


bool XTRACEMONITOR_ORIGINS::TrimHistory()
{
  while(history.GetSize() > history_max)
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(0);
      history.Delete(line);
      delete line;
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::PushLine(XDWORD publicIP, XDWORD localIP, XBYTE color, XSTRING& text)
{
  XTRACEMONITOR_ORIGIN_LINE* line = GEN_NEW XTRACEMONITOR_ORIGIN_LINE();
  if(!line) return false;

  line->publicIP = publicIP;
  line->localIP  = localIP;
  line->color    = color;
  line->text     = text;

  history.Add(line);
  TrimHistory();

  if(selected && BelongsTo(selected, publicIP, localIP) && owner && owner->gui_console.GetElement())
    {
      owner->gui_console.GetElement()->PrintColor(color, text.Get());
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
      return true;
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::ReplaySelectedToConsole()
{
  if(!owner || !owner->gui_console.GetElement() || !selected) return false;

  owner->gui_console.GetElement()->Clear();

  XTRACEMONITOR_CFG& cfg    = XTRACEMONITOR_CFG::GetInstance(false);
  const int          levels = cfg.Filters_GetLevels();
  XSTRING*           textfilter = NULL;
  if(cfg.Filters_IsTextActive()) textfilter = cfg.Filters_GetText();

  for(XDWORD c = 0; c < history.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(c);
      if(!line) continue;
      if(!BelongsTo(selected, line->publicIP, line->localIP)) continue;

      // Same level gate as intake (color index 0..5; uncolored was stored as Black).
      if((levels & XTRACEMONITOR_CFG_LevelMaskForColorIndex((int)line->color)) == 0) continue;

      if(textfilter && !textfilter->IsEmpty())
        {
          if(line->text.Find(textfilter->Get(), false) == XSTRING_NOTFOUND) continue;
        }

      owner->gui_console.GetElement()->PrintColor(line->color, line->text.Get());
    }

  if(cfg.GetTextTracking())
    {
      owner->gui_console.GetElement()->SetAutoscroll(true);
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
  return true;
}


bool XTRACEMONITOR_ORIGINS::ClearSelectedHistory()
{
  if(!selected) return false;

  for(XDWORD c = history.GetSize(); c > 0; c--)
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(c - 1);
      if(!line) continue;
      if(!BelongsTo(selected, line->publicIP, line->localIP)) continue;

      history.Delete(line);
      delete line;
    }

  if(owner && owner->gui_console.GetElement())
    {
      owner->gui_console.GetElement()->Clear();
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::ClearReceivedTraces()
{
  if(!owner || !owner->gui_tree.GetElement()) return false;

  // Full console session wipe — listen port / UART change starts a new receive context.
  history.DeleteContents();
  history.DeleteAll();

  if(owner->gui_console.GetElement())
    {
      owner->gui_console.GetElement()->Clear();
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
    }

  // Status/valor came from the previous listen session; drop it on every remaining origin.
  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(origin) origin->status_msgs.StatusMsg_DeleteAll();
    }

  if(owner->gui_varstatus.GetElement())
    {
      owner->gui_varstatus.GetElement()->Clear();
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
    }

  // Drop tree nodes added only from received traces. Keep All / Local box / CFG / UART.
  // PUBLIC_LOCAL children are destroyed with their PUBLIC_PARENT via RemoveNode.
  // Also drop geo/map cache for those public IPs (CFG IPs that remain keep / re-Request).
  for(XDWORD c = origins.GetSize(); c > 0; c--)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c - 1);
      if(!origin) continue;

      const bool is_trace_ip =
        (origin->kind == XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT) ||
        (origin->kind == XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL)  ||
        (origin->kind == XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD);

      if(!is_trace_ip) continue;

      if((origin->kind == XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT) && origin->publicIP)
        {
          owner->geoip.Forget(origin->publicIP);
        }

      if(origin->node &&
         ((origin->kind == XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT) ||
          (origin->kind == XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD)))
        {
          owner->gui_tree.GetElement()->RemoveNode(origin->node);
        }

      origin->node = NULL;
      origins.Delete(origin);
      delete origin;
    }

  // CFG public IPs that survived the wipe: keep requesting geo+map if missing.
  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(!origin || (origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG)) continue;
      if(origin->publicIP) owner->geoip.Request(origin->publicIP);
    }

  if(root && root->node)
    {
      owner->gui_tree.GetElement()->SetSelected(root->node);
      selected = root;
    }
   else
    {
      selected = NULL;
    }

  RefreshIPCounters();
  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
  return true;
}


bool XTRACEMONITOR_ORIGINS::ClearHistoryMatching(XDWORD publicIP, XDWORD localIP)
{
  // Clear lines for the IP group of this message (not unrelated publics).
  XTRACEMONITOR_ORIGIN scratch_uart;
  scratch_uart.kind     = XTRACEMONITOR_ORIGIN_KIND_UART;
  scratch_uart.publicIP = XTRACEMONITOR_ORIGIN_UART_IP;
  scratch_uart.localIP  = XTRACEMONITOR_ORIGIN_UART_IP;

  XTRACEMONITOR_ORIGIN scratch_pub;
  scratch_pub.kind     = XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT;
  scratch_pub.publicIP = publicIP;

  XTRACEMONITOR_ORIGIN scratch_leaf;
  scratch_leaf.kind     = XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL;
  scratch_leaf.publicIP = publicIP;
  scratch_leaf.localIP  = localIP;

  XTRACEMONITOR_ORIGIN scratch_locbox;
  scratch_locbox.kind = XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET;

  XTRACEMONITOR_ORIGIN scratch_locchild;
  scratch_locchild.kind    = XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD;
  scratch_locchild.localIP = localIP;

  for(XDWORD c = history.GetSize(); c > 0; c--)
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(c - 1);
      if(!line) continue;

      bool drop = false;

      if((publicIP == XTRACEMONITOR_ORIGIN_UART_IP) && (localIP == XTRACEMONITOR_ORIGIN_UART_IP))
        {
          drop = BelongsTo(&scratch_uart, line->publicIP, line->localIP);
        }
       else if(!publicIP && localIP)
        {
          drop = BelongsTo(&scratch_locbox, line->publicIP, line->localIP) ||
                 BelongsTo(&scratch_locchild, line->publicIP, line->localIP);
        }
       else if(publicIP)
        {
          drop = BelongsTo(&scratch_pub, line->publicIP, line->localIP);
          if(localIP) drop = drop || BelongsTo(&scratch_leaf, line->publicIP, line->localIP);
        }

      if(!drop) continue;

      history.Delete(line);
      delete line;
    }

  ReplaySelectedToConsole();
  return true;
}


bool XTRACEMONITOR_ORIGINS::ApplyStatusLine(XSTRING* line, XDWORD publicIP, XDWORD localIP)
{
  if(!line) return false;

  // CLEAR_STATUS: wipe every origin cache + selected valor (legacy cleared per receiving origin; CLEAR is global).
  if(line->Find(XTRACE_IDMSGSTATUSCLEAR, false) != XSTRING_NOTFOUND)
    {
      for(XDWORD c = 0; c < origins.GetSize(); c++)
        {
          XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
          if(origin) origin->status_msgs.StatusMsg_DeleteAll();
        }

      if(owner && owner->gui_varstatus.GetElement())
        {
          owner->gui_varstatus.GetElement()->Clear();
          GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
        }

      return true;
    }

  EnsureFromMessage(publicIP, localIP);

  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(!origin) continue;
      if(!BelongsTo(origin, publicIP, localIP)) continue;

      XTRACE_STATUS_MSG* status_msg = origin->status_msgs.StatusMsg_Add(line);
      if(!status_msg) continue;

      if(status_msg->GetXTimerLastUpdate()) status_msg->GetXTimerLastUpdate()->Reset();

      if(origin == selected && owner && owner->gui_varstatus.GetElement() && status_msg->GetName())
        {
          XSTRING value;
          XTraceMonitor_Origins_FormatStatusValue(status_msg, value);
          owner->gui_varstatus.GetElement()->SetValue(status_msg->GetName()->Get(), value.Get(), true);
          GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
        }
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::ClearSelectedStatus()
{
  if(!selected) return false;

  selected->status_msgs.StatusMsg_DeleteAll();

  if(owner && owner->gui_varstatus.GetElement())
    {
      owner->gui_varstatus.GetElement()->Clear();
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::RebuildSelectedValor()
{
  if(!owner || !owner->gui_varstatus.GetElement() || !selected) return false;

  owner->gui_varstatus.GetElement()->Clear();

  XVECTOR<XTRACE_STATUS_MSG*>* all = selected->status_msgs.StatusMsg_GetAll();
  if(all)
    {
      for(XDWORD c = 0; c < all->GetSize(); c++)
        {
          XTRACE_STATUS_MSG* status_msg = all->Get(c);
          if(!status_msg || !status_msg->GetName()) continue;

          XSTRING value;
          XTraceMonitor_Origins_FormatStatusValue(status_msg, value);
          // Selection replay: no MarkChanged (would reset XTIMER); live updates mark via ApplyStatusLine.
          owner->gui_varstatus.GetElement()->SetValue(status_msg->GetName()->Get(), value.Get(), false);
        }
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_varstatus.GetElement());
  return true;
}


bool XTRACEMONITOR_ORIGINS::QueuePublicIPGeolocation()
{
  if(!owner) return false;

  for(XDWORD c = 0; c < origins.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(c);
      if(!origin || !origin->publicIP) continue;
      if((origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG) &&
         (origin->kind != XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT)) continue;

      owner->geoip.Request(origin->publicIP);
    }

  return true;
}


bool XTRACEMONITOR_ORIGINS::RefreshIPCounters()
{
  if(!owner) return false;

  XDWORD public_count = 0;
  XDWORD local_count  = 0;

  for(XDWORD i = 0; i < origins.GetSize(); i++)
    {
      XTRACEMONITOR_ORIGIN* origin = origins.Get(i);
      if(!origin) continue;

      switch(origin->kind)
        {
          case XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT :
          case XTRACEMONITOR_ORIGIN_KIND_CFG :
            public_count++;
            break;

          case XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD :
          case XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL :
            local_count++;
            break;

          default :
            break;
        }
    }

  return owner->gui_tree.Stats_Refresh(public_count, local_count);
}


void XTRACEMONITOR_ORIGINS::Clean()
{
  owner             = NULL;
  history_max       = XTRACEMONITOR_ORIGINS_HISTORY_DEFAULT_MAX;
  root              = NULL;
  local_no_internet = NULL;
  uart              = NULL;
  selected          = NULL;
}
