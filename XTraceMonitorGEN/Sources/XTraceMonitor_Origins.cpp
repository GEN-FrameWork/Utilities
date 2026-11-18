/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Origins.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_Origins.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"
#include "XTraceMonitor_TranslateID.h"

#include "XTranslation.h"

#include "UI_Element_Tree.h"
#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Manager.h"

#include "DIOIP.h"
#include "DIODNSResolver.h"
#include "XTimer.h"
#include "XFactory.h"
#include "XPath.h"
#include "XDir.h"
#include "XDateTime.h"
#include "XFileTXT.h"
#include "XPathsManager.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool XTraceMonitor_Origins_FormatStatusValue(XTRACE_STATUS_MSG* status_msg, XSTRING& out)
* @brief      Format the value of a status message as text according to its type.
* @ingroup    UTILITIES
*
* @param[in]  status_msg : status message to format.
* @param[out] out : string that receives the formatted value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN::XTRACEMONITOR_ORIGIN()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN::XTRACEMONITOR_ORIGIN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN::~XTRACEMONITOR_ORIGIN()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN::~XTRACEMONITOR_ORIGIN()
{
  status_msgs.StatusMsg_DeleteAll();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_ORIGIN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_ORIGIN::Clean()
{
  kind              = XTRACEMONITOR_ORIGIN_KIND_ALL;
  publicIP          = 0;
  localIP           = 0;
  public_from_peer  = false;
  name.Empty();
  node              = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN_LINE::XTRACEMONITOR_ORIGIN_LINE()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN_LINE::XTRACEMONITOR_ORIGIN_LINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN_LINE::~XTRACEMONITOR_ORIGIN_LINE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN_LINE::~XTRACEMONITOR_ORIGIN_LINE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_ORIGIN_LINE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_ORIGIN_LINE::Clean()
{
  publicIP = 0;
  localIP  = 0;
  color    = 0;
  text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINS::XTRACEMONITOR_ORIGINS()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGINS::XTRACEMONITOR_ORIGINS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINS::~XTRACEMONITOR_ORIGINS()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGINS::~XTRACEMONITOR_ORIGINS()
{
  history.DeleteContents();
  history.DeleteAll();
  origins.DeleteContents();
  origins.DeleteAll();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::SetOwner(XTRACEMONITOR* owner)
* @brief      Set the owner (application instance) of the origins.
* @ingroup    UTILITIES
*
* @param[in]  owner : trace monitor instance that owns this object.
*
* @return     bool : true if the owner is valid; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_ORIGINS::SetHistoryMax(XDWORD maxlines)
* @brief      Set the maximum number of lines kept in the console history.
* @ingroup    UTILITIES
*
* @param[in]  maxlines : maximum number of history lines (minimum 100).
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_ORIGINS::SetHistoryMax(XDWORD maxlines)
{
  if(maxlines < (XDWORD)XTRACEMONITOR_CFG_MAXLINESCONSOLE_MIN) maxlines = (XDWORD)XTRACEMONITOR_CFG_MAXLINESCONSOLE_MIN;
  history_max = maxlines;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_ORIGINS::GetHistoryMax()
* @brief      Current per-console history recycle threshold.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_ORIGINS::GetHistoryMax()
{
  return history_max;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ApplyHistoryMaxFromCFG()
* @brief      Reloads history_max from [console] maxlinesconsole.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::ApplyHistoryMaxFromCFG()
{
  SetHistoryMax((XDWORD)XTRACEMONITOR_CFG::GetInstance(false).GetMaxLinesConsole());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::FormatIPLabel(XDWORD ip, bool starred, XSTRING& out)
* @brief      Format a packed IPv4 address as a tree label.
* @ingroup    UTILITIES
*
* @param[in]  ip : packed IPv4 address (first octet in the most significant byte).
* @param[in]  starred : true to append a '*' to the label.
* @param[out] out : string that receives the label.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::IconForIP(XDWORD ip, XCHAR*& icon)
* @brief      Get the tree icon that corresponds to a packed IPv4 address.
* @ingroup    UTILITIES
*
* @param[in]  ip : packed IPv4 address.
* @param[out] icon : icon path (local and APIPA addresses use the packet icon).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::AddOrigin(XTRACEMONITOR_ORIGIN_KIND kind, XDWORD publicIP, XDWORD localIP, XCHAR* label, XCHAR* icon, UI_ELEMENT_TREE_NODE* father, bool public_from_peer)
* @brief      Add an origin and its node to the tree.
* @ingroup    UTILITIES
*
* @param[in]  kind : kind of origin.
* @param[in]  publicIP : packed public IPv4 address.
* @param[in]  localIP : packed local IPv4 address.
* @param[in]  label : text of the tree node.
* @param[in]  icon : icon of the tree node.
* @param[in]  father : parent tree node.
* @param[in]  public_from_peer : true if the public IP was obtained from the peer.
*
* @return     XTRACEMONITOR_ORIGIN* : created origin if successful; otherwise NULL.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::FindExact(XDWORD publicIP, XDWORD localIP)
* @brief      Find a dynamic origin that matches exactly the public and local IP.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address.
* @param[in]  localIP : packed local IPv4 address.
*
* @return     XTRACEMONITOR_ORIGIN* : origin found if successful; otherwise NULL.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::FindCFGByPublicIP(XDWORD publicIP)
* @brief      Find a configured (CFG) origin by its public IP.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address.
*
* @return     XTRACEMONITOR_ORIGIN* : origin found if successful; otherwise NULL.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::SeedTree()
* @brief      Clear and seed the tree with the fixed, configured and UART origins.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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

  root = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_ALL, 0, 0,
                   TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_TREE_ALL), NL("tree/all.png"), NULL);
  if(!root) return false;

  local_no_internet = AddOrigin(XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET, 0, 0,
                                TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_TREE_LOCAL_NO_INTERNET),
                                NL("tree/local.png"), root->node);
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::UpdateUARTLabel()
* @brief      Update the text of the UART origin node.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::RefreshFixedLabels()
* @brief      Refresh the translated labels of the fixed tree nodes.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::RefreshFixedLabels()
{
  bool changed = false;

  if(root && root->node)
    {
      XCHAR* label = TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_TREE_ALL);
      if(root->node->text.Compare(label))
        {
          root->node->text.Set(label);
          root->name.Set(label);
          changed = true;
        }
    }

  if(local_no_internet && local_no_internet->node)
    {
      XCHAR* label = TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_TREE_LOCAL_NO_INTERNET);
      if(local_no_internet->node->text.Compare(label))
        {
          local_no_internet->node->text.Set(label);
          local_no_internet->name.Set(label);
          changed = true;
        }
    }

  if(changed && owner && owner->gui_tree.GetElement())
    {
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_tree.GetElement());
    }

  RefreshIPCounters();
  UpdateUARTLabel();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::BelongsTo(XTRACEMONITOR_ORIGIN* origin, XDWORD publicIP, XDWORD localIP)
* @brief      Check if a message with the given IPs belongs to an origin.
* @ingroup    UTILITIES
*
* @param[in]  origin : origin to check.
* @param[in]  publicIP : packed public IPv4 address of the message.
* @param[in]  localIP : packed local IPv4 address of the message.
*
* @return     bool : true if the message belongs to the origin; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::EnsureFromMessage(XDWORD publicIP, XDWORD localIP)
* @brief      Ensure the tree has the origin nodes for the IPs of a message.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address of the message.
* @param[in]  localIP : packed local IPv4 address of the message.
*
* @return     bool : true if the origin exists or was created; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::EnsureFromMessage(XDWORD publicIP, XDWORD localIP)
{
  if(!owner || !owner->gui_tree.GetElement() || !root) return false;

  if((publicIP == XTRACEMONITOR_ORIGIN_UART_IP) && (localIP == XTRACEMONITOR_ORIGIN_UART_IP))
    {
      return (uart != NULL);
    }

  // Local-only under "Local (no internet)".
  if(!publicIP && localIP)
    {
      // WM_COPYDATA / XWINDOWSTRACE packs localIP 1.1.1.1 — attribute to the Local folder
      // itself (no extra "1.1.1.1" leaf); BelongsTo(LOCAL_NO_INTERNET) already matches.
      if(localIP == XTRACEMONITOR_ORIGIN_COPYDATA_LOCALIP)
        {
          return (local_no_internet != NULL);
        }

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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::GetSelected()
* @brief      Get the selected origin.
* @ingroup    UTILITIES
*
* @return     XTRACEMONITOR_ORIGIN* : selected origin, or NULL if none.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::GetSelected()
{
  return selected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::GetFromNode(UI_ELEMENT_TREE_NODE* node)
* @brief      Get the origin that owns a tree node.
* @ingroup    UTILITIES
*
* @param[in]  node : tree node to look up.
*
* @return     XTRACEMONITOR_ORIGIN* : origin found if successful; otherwise NULL.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::SyncSelectionFromTree()
* @brief      Synchronize the selected origin with the tree selection.
* @ingroup    UTILITIES
*
* @return     bool : true if the selection changed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::ResolveOwnerOrigin(XDWORD publicIP, XDWORD localIP)
* @brief      Most specific origin that owns a message (leaf console bucket for recycle).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGIN* XTRACEMONITOR_ORIGINS::ResolveOwnerOrigin(XDWORD publicIP, XDWORD localIP)
{
  if((publicIP == XTRACEMONITOR_ORIGIN_UART_IP) && (localIP == XTRACEMONITOR_ORIGIN_UART_IP))
    {
      return uart;
    }

  if(!publicIP && localIP)
    {
      if(localIP == XTRACEMONITOR_ORIGIN_COPYDATA_LOCALIP) return local_no_internet;
      XTRACEMONITOR_ORIGIN* child = FindExact(0, localIP);
      if(child) return child;
      return local_no_internet;
    }

  if(!publicIP) return NULL;

  if(localIP)
    {
      XTRACEMONITOR_ORIGIN* leaf = FindExact(publicIP, localIP);
      if(leaf) return leaf;
    }

  XTRACEMONITOR_ORIGIN* parent = FindCFGByPublicIP(publicIP);
  if(!parent) parent = FindExact(publicIP, 0);
  return parent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_ORIGINS::CountLinesForOrigin(XTRACEMONITOR_ORIGIN* origin)
* @brief      Count history lines that belong to an origin console.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_ORIGINS::CountLinesForOrigin(XTRACEMONITOR_ORIGIN* origin)
{
  if(!origin) return 0;

  XDWORD n = 0;
  for(XDWORD c = 0; c < history.GetSize(); c++)
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(c);
      if(!line) continue;
      if(BelongsTo(origin, line->publicIP, line->localIP)) n++;
    }

  return n;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::BuildRecycleStatusLabel(XTRACEMONITOR_ORIGIN* origin, XSTRING& out)
* @brief      Save-style console name for OpStatus: same tokens as save, spaces, normal date, no .txt.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::BuildRecycleStatusLabel(XTRACEMONITOR_ORIGIN* origin, XSTRING& out)
{
  out.Empty();
  if(!origin) return false;

  XSTRING public_part;
  XSTRING local_part;
  XSTRING cfg_name;

  switch(origin->kind)
    {
      case XTRACEMONITOR_ORIGIN_KIND_ALL :
        public_part = NL("All");
        break;

      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET :
      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD :
        public_part = NL("Local");
        break;

      case XTRACEMONITOR_ORIGIN_KIND_UART :
        public_part = NL("UART");
        break;

      default :
        if(origin->publicIP)
          {
            FormatIPLabel(origin->publicIP, false, public_part);
            ResolveCFGOriginName(origin->publicIP, cfg_name);
          }
         else
          {
            public_part = NL("Unknown");
          }
        break;
    }

  const bool have_local =
    origin->localIP &&
    (origin->localIP != XTRACEMONITOR_ORIGIN_UART_IP) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_UART) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_ALL) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT);

  if(have_local) FormatIPLabel(origin->localIP, false, local_part);

  XSTRING stamp;
  XDATETIME* now = GEN_XFACTORY.CreateDateTime();
  if(now)
    {
      now->Read();
      stamp.Format(NL("%02d/%02d/%04d %02d:%02d:%02d"),
                   now->GetDay(), now->GetMonth(), now->GetYear(),
                   now->GetHours(), now->GetMinutes(), now->GetSeconds());
      GEN_XFACTORY.DeleteDateTime(now);
    }
   else
    {
      stamp = NL("00/00/0000 00:00:00");
    }

  out = NL("Trace");
  if(!cfg_name.IsEmpty())
    {
      out += NL(" ");
      out += cfg_name.Get();
    }
  out += NL(" ");
  out += public_part.Get();
  if(!local_part.IsEmpty())
    {
      out += NL(" ");
      out += local_part.Get();
    }
  out += NL(" ");
  out += stamp.Get();

  return !out.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::RecycleOriginIfNeeded(XTRACEMONITOR_ORIGIN* origin)
* @brief      When an origin console exceeds history_max, drop the oldest 95% of its lines.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::RecycleOriginIfNeeded(XTRACEMONITOR_ORIGIN* origin)
{
  if(!origin || !history_max) return false;
  // All is only an aggregate view of the other consoles — never recycle it as a bucket.
  if(origin->kind == XTRACEMONITOR_ORIGIN_KIND_ALL) return false;

  const XDWORD count = CountLinesForOrigin(origin);
  if(count <= history_max) return false;

  XDWORD remove_n = (XDWORD)((count * 95) / 100);
  if(remove_n < 1) remove_n = 1;
  if(remove_n >= count) remove_n = count - 1;

  XDWORD removed = 0;
  for(XDWORD c = 0; (c < history.GetSize()) && (removed < remove_n); )
    {
      XTRACEMONITOR_ORIGIN_LINE* line = history.Get(c);
      if(!line)
        {
          c++;
          continue;
        }

      if(!BelongsTo(origin, line->publicIP, line->localIP))
        {
          c++;
          continue;
        }

      history.Delete(line);
      delete line;
      removed++;
      // Do not advance c: Delete shifts following entries down.
    }

  if(!removed) return false;

  // Selected console may still list surviving lines from this origin (All / parent / leaf).
  if(selected) ReplaySelectedToConsole();

  if(owner)
    {
      XSTRING label;
      if(BuildRecycleStatusLabel(origin, label))
        {
          owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_RECYCLE_CONSOLE, label.Get());
        }
       else
        {
          owner->OpStatus_Log(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_OPSTATUS_RECYCLE_CONSOLE, NL(""));
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::PushLine(XDWORD publicIP, XDWORD localIP, XBYTE color, XSTRING& text)
* @brief      Add a line to the history and print it if it belongs to the selected origin.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address of the line.
* @param[in]  localIP : packed local IPv4 address of the line.
* @param[in]  color : color index of the line.
* @param[in]  text : text of the line.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::PushLine(XDWORD publicIP, XDWORD localIP, XBYTE color, XSTRING& text)
{
  XTRACEMONITOR_ORIGIN_LINE* line = GEN_NEW XTRACEMONITOR_ORIGIN_LINE();
  if(!line) return false;

  line->publicIP = publicIP;
  line->localIP  = localIP;
  line->color    = color;
  line->text     = text;

  history.Add(line);

  // Per-console recycle for the owning leaf (and its parent folder). Never All: it is
  // only the aggregate of the other consoles.
  XTRACEMONITOR_ORIGIN* owner_origin = ResolveOwnerOrigin(publicIP, localIP);
  RecycleOriginIfNeeded(owner_origin);

  if(owner_origin)
    {
      if(owner_origin->kind == XTRACEMONITOR_ORIGIN_KIND_PUBLIC_LOCAL)
        {
          XTRACEMONITOR_ORIGIN* parent = FindCFGByPublicIP(owner_origin->publicIP);
          if(!parent) parent = FindExact(owner_origin->publicIP, 0);
          if(parent) RecycleOriginIfNeeded(parent);
        }
      else if(owner_origin->kind == XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD)
        {
          if(local_no_internet) RecycleOriginIfNeeded(local_no_internet);
        }
    }

  if(selected && BelongsTo(selected, publicIP, localIP) && owner && owner->gui_console.GetElement())
    {
      owner->gui_console.GetElement()->PrintColor(color, text.Get());
      GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
      return true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ReplaySelectedToConsole()
* @brief      Replay the history of the selected origin to the console, applying the filters.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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

  owner->gui_console.ApplyTextTracking();

  GEN_USERINTERFACE.Elements_SetToRedraw(owner->gui_console.GetElement());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ClearSelectedHistory()
* @brief      Clear the history lines of the selected origin and the console.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::SanitizeFileToken(XSTRING& token)
* @brief      Replace the characters not valid in a file name with underscores.
* @ingroup    UTILITIES
*
* @param[in,out]  token : text to sanitize.
*
* @return     bool : true if the resulting token is not empty; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::SanitizeFileToken(XSTRING& token)
{
  if(token.IsEmpty()) return false;

  for(XDWORD c = 0; c < token.GetSize(); c++)
    {
      XCHAR ch = token.Get()[c];
      if((ch == _C('\\')) || (ch == _C('/')) || (ch == _C(':')) || (ch == _C('*')) ||
         (ch == _C('?'))  || (ch == _C('"')) || (ch == _C('<')) || (ch == _C('>')) ||
         (ch == _C('|'))  || (ch == _C(' ')) || (ch == _C('(')) || (ch == _C(')')))
        {
          token.Get()[c] = _C('_');
        }
    }

  while(token.Find(NL("__"), true) != XSTRING_NOTFOUND)
    {
      token.DeleteCharacters(token.Find(NL("__"), true), 1);
    }

  return !token.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ResolveCFGOriginName(XDWORD publicIP, XSTRING& out)
* @brief      Resolve the configured origin name for a public IP.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address.
* @param[out] out : string that receives the origin name.
*
* @return     bool : true if a configured origin matches the IP; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::ResolveCFGOriginName(XDWORD publicIP, XSTRING& out)
{
  out.Empty();
  if(!publicIP) return false;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const int norigins = cfg.GetOriginCount();

  for(int i = 0; i < norigins; i++)
    {
      XTRACEMONITOR_CFG_ORIGIN cfgorigin;
      if(!cfg.GetOrigin(i, cfgorigin)) continue;
      if(cfgorigin.name.IsEmpty()) continue;

      XSTRING display_ip = cfgorigin.ip;
      if(display_ip.IsEmpty() && !cfgorigin.url.IsEmpty())
        {
          DIOIP resolved;
          if(GEN_DIODNSRESOLVER.ResolveURL(cfgorigin.url.Get(), resolved))
            {
              resolved.GetXString(display_ip);
            }
        }

      if(display_ip.IsEmpty()) continue;

      DIOIP dioip;
      if(!dioip.Set(display_ip)) continue;

      XBYTE* a = dioip.Get();
      XDWORD packed = ((XDWORD)a[0] << 24) | ((XDWORD)a[1] << 16) | ((XDWORD)a[2] << 8) | (XDWORD)a[3];
      if(packed != publicIP) continue;

      out = cfgorigin.name;
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::BuildSaveFileName(XTRACEMONITOR_ORIGIN* origin, XSTRING& out)
* @brief      Build the file name used to save the console of an origin.
* @ingroup    UTILITIES
*
* @param[in]  origin : origin to save.
* @param[out] out : string that receives the file name.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::BuildSaveFileName(XTRACEMONITOR_ORIGIN* origin, XSTRING& out)
{
  out.Empty();
  if(!origin) return false;

  XSTRING public_part;
  XSTRING local_part;
  XSTRING cfg_name;
  XSTRING stamp;

  switch(origin->kind)
    {
      case XTRACEMONITOR_ORIGIN_KIND_ALL :
        public_part = NL("All");
        break;

      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET :
      case XTRACEMONITOR_ORIGIN_KIND_LOCAL_CHILD :
        public_part = NL("Local");
        break;

      case XTRACEMONITOR_ORIGIN_KIND_UART :
        public_part = NL("UART");
        break;

      default :
        if(origin->publicIP)
          {
            FormatIPLabel(origin->publicIP, false, public_part);
            ResolveCFGOriginName(origin->publicIP, cfg_name);
          }
         else
          {
            public_part = NL("Unknown");
          }
        break;
    }

  const bool have_local =
    origin->localIP &&
    (origin->localIP != XTRACEMONITOR_ORIGIN_UART_IP) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_UART) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_ALL) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_LOCAL_NO_INTERNET) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_CFG) &&
    (origin->kind != XTRACEMONITOR_ORIGIN_KIND_PUBLIC_PARENT);

  if(have_local) FormatIPLabel(origin->localIP, false, local_part);

  XDATETIME* now = GEN_XFACTORY.CreateDateTime();
  if(now)
    {
      now->Read();
      stamp.Format(NL("%04d%02d%02d_%02d%02d%02d"),
                   now->GetYear(), now->GetMonth(), now->GetDay(),
                   now->GetHours(), now->GetMinutes(), now->GetSeconds());
      GEN_XFACTORY.DeleteDateTime(now);
    }
   else
    {
      stamp = NL("00000000_000000");
    }

  SanitizeFileToken(public_part);
  SanitizeFileToken(local_part);
  SanitizeFileToken(cfg_name);

  out = NL("Trace");
  if(!cfg_name.IsEmpty())
    {
      out += NL("_");
      out += cfg_name.Get();
    }
  out += NL("_");
  out += public_part.Get();
  if(!local_part.IsEmpty())
    {
      out += NL("_");
      out += local_part.Get();
    }
  out += NL("_");
  out += stamp.Get();
  out += NL(".txt");

  return !out.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::SaveSelectedConsole(XSTRING* out_filename)
* @brief      Save the status variables and console lines of the selected origin to a text file.
* @ingroup    UTILITIES
*
* @param[out] out_filename : string that receives the file name (can be NULL).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_ORIGINS::SaveSelectedConsole(XSTRING* out_filename)
{
  if(out_filename) out_filename->Empty();
  if(!selected || !owner || !owner->gui_console.GetElement()) return false;

  XSTRING filename;
  if(!BuildSaveFileName(selected, filename)) return false;

  XPATH dir;
  if(!GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, dir)) return false;

  dir.Slash_Add();
  dir += XTRACEMONITOR_SAVED_DIR;

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(!xdir->Exist(dir.Get()))
    {
      if(!xdir->Make(dir.Get(), true))
        {
          GEN_XFACTORY.Delete_Dir(xdir);
          return false;
        }
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  XPATH filepath = dir;
  filepath.Slash_Add();
  filepath += filename.Get();

  XFILETXT filetxt;
  if(!filetxt.Create(filepath, XFILETXTFORMATCHAR_UTF8, XFILETXTTYPELF_0D0A)) return false;

  // 1) Status variables first: pad names so every value starts on the same column.
  UI_ELEMENT_PROPERTYLIST* valor = owner->gui_varstatus.GetElement();
  const XDWORD nrows = valor ? valor->GetRowCount() : 0;
  XDWORD name_width = 0;

  if(nrows > 0)
    {
      for(XDWORD r = 0; r < nrows; r++)
        {
          UI_ELEMENT_PROPERTYLIST_ROW* row = valor->GetRow(r);
          if(!row) continue;
          if(row->name.GetSize() > name_width) name_width = row->name.GetSize();
        }

      for(XDWORD r = 0; r < nrows; r++)
        {
          UI_ELEMENT_PROPERTYLIST_ROW* row = valor->GetRow(r);
          if(!row) continue;

          XSTRING padded = row->name;
          while(padded.GetSize() < name_width) padded += NL(" ");

          XSTRING statusline;
          statusline.Format(NL("%s\t%s"), padded.Get(), row->value.Get());
          filetxt.AddLine(statusline);
        }

      // Two blank lines between status block and console lines.
      filetxt.AddLine(NL(""));
      filetxt.AddLine(NL(""));
    }

  // 2) Console lines: "[n] text..." (color index, space, remainder).
  UI_ELEMENT_CONSOLE* console = owner->gui_console.GetElement();
  const XDWORD nlines = console->GetLineCount();
  for(XDWORD c = 0; c < nlines; c++)
    {
      UI_ELEMENT_CONSOLE_LINE* line = console->GetLine(c);
      if(!line) continue;

      XSTRING outline;
      outline.Format(NL("[%u] %s"), (XDWORD)line->colorindex, line->text.Get());
      filetxt.AddLine(outline);
    }

  // XFILETXT::WriteAllFile refuses empty line buffers; keep a BOM-only file valid.
  if(filetxt.GetNLines() <= 0) filetxt.AddLine(NL(""));

  if(!filetxt.WriteAllFile())
    {
      filetxt.Close();
      return false;
    }

  filetxt.Close();

  if(out_filename) (*out_filename) = filename;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ClearReceivedTraces()
* @brief      Clear all received traces: history, console, status and trace-only tree nodes.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ClearHistoryMatching(XDWORD publicIP, XDWORD localIP)
* @brief      Clear the history lines that belong to the IP group of a message.
* @ingroup    UTILITIES
*
* @param[in]  publicIP : packed public IPv4 address of the message.
* @param[in]  localIP : packed local IPv4 address of the message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ApplyStatusLine(XSTRING* line, XDWORD publicIP, XDWORD localIP)
* @brief      Apply a status line to the matching origins and the status list.
* @ingroup    UTILITIES
*
* @param[in]  line : status line received.
* @param[in]  publicIP : packed public IPv4 address of the message.
* @param[in]  localIP : packed local IPv4 address of the message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::ClearSelectedStatus()
* @brief      Clear the status variables of the selected origin.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::RebuildSelectedValor()
* @brief      Rebuild the status list from the status variables of the selected origin.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::QueuePublicIPGeolocation()
* @brief      Queue the geolocation of all public IP origins.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_ORIGINS::RefreshIPCounters()
* @brief      Refresh the public and local IP counters of the tree.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
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


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_ORIGINS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_ORIGINS::Clean()
{
  owner             = NULL;
  history_max       = XTRACEMONITOR_CFG_MAXLINESCONSOLE_DEFAULT;
  root              = NULL;
  local_no_internet = NULL;
  uart              = NULL;
  selected          = NULL;
}
