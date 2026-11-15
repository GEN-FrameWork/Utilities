/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GeoIP.cpp
*
* @class      XTRACEMONITOR_GEOIP
* @brief      Background public-IP geolocation + static map via GEN scrapers (in-memory cache)
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

#include "XTraceMonitor_GeoIP.h"

#include "XFactory.h"
#include "XThread.h"
#include "XThreadCollected.h"
#include "XDir.h"
#include "XPath.h"
#include "XPathsManager.h"
#include "DIOIP.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GEOIP_ENTRY::XTRACEMONITOR_GEOIP_ENTRY()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GEOIP_ENTRY::XTRACEMONITOR_GEOIP_ENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GEOIP_ENTRY::~XTRACEMONITOR_GEOIP_ENTRY()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GEOIP_ENTRY::~XTRACEMONITOR_GEOIP_ENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GEOIP_ENTRY::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GEOIP_ENTRY::Clean()
{
  publicIP     = 0;
  state        = XTRACEMONITOR_GEOIP_STATE_PENDING;
  country.Empty();
  state_name.Empty();
  city.Empty();
  isp.Empty();
  organization.Empty();
  latitude     = 0.0f;
  longitude    = 0.0f;
  map_state    = XTRACEMONITOR_GEOMAP_STATE_NONE;
  map_path.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GEOIP::XTRACEMONITOR_GEOIP()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GEOIP::XTRACEMONITOR_GEOIP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GEOIP::~XTRACEMONITOR_GEOIP()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GEOIP::~XTRACEMONITOR_GEOIP()
{
  End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds this module to the application instance.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::Ini()
* @brief      Starts the worker thread that drains geo + map queues.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::Ini()
{
#ifndef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE
  return false;
#else
  if(!XTRACEMONITOR_CFG::GetInstance(false).GetGeolocalizationPublicIP()) return false;
  if(thread) return true;

  mutex = GEN_XFACTORY.Create_Mutex();
  if(!mutex) return false;

  thread = CREATEXTHREAD(XTHREADGROUPID_APPOWNER, NL("XTRACEMONITOR_GEOIP"), ThreadFunction, this);
  if(!thread)
    {
      GEN_XFACTORY.Delete_Mutex(mutex);
      mutex = NULL;
      return false;
    }

  thread->SetPriority(XTHREADPRIORITY_LOW);
  return thread->Ini();
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::End()
* @brief      Stops the worker thread and clears the in-memory cache.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::End()
{
  if(thread)
    {
      thread->End();
      DELETEXTHREAD(XTHREADGROUPID_APPOWNER, thread);
      thread = NULL;
    }

  if(mutex)
    {
      GEN_XFACTORY.Delete_Mutex(mutex);
      mutex = NULL;
    }

  queue.DeleteAll();
  map_queue.DeleteAll();
  entries.DeleteContents();
  entries.DeleteAll();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::Request(XDWORD publicIP)
* @brief      Queues a public IP for geolocation (and later map). No-op if already tracked.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::Request(XDWORD publicIP)
{
#ifndef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE
  return false;
#else
  if(!XTRACEMONITOR_CFG::GetInstance(false).GetGeolocalizationPublicIP()) return false;
  if(!publicIP) return false;
  if(!mutex) return false;

  mutex->Lock();

  XTRACEMONITOR_GEOIP_ENTRY* existing = FindEntry(publicIP);
  if(existing)
    {
      mutex->UnLock();
      return true;
    }

  XTRACEMONITOR_GEOIP_ENTRY* entry = GEN_NEW XTRACEMONITOR_GEOIP_ENTRY();
  if(!entry)
    {
      mutex->UnLock();
      return false;
    }

  entry->publicIP  = publicIP;
  entry->state     = XTRACEMONITOR_GEOIP_STATE_PENDING;
  entry->map_state = XTRACEMONITOR_GEOMAP_STATE_NONE;
  entries.Add(entry);
  queue.Add(publicIP);

  mutex->UnLock();
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::Forget(XDWORD publicIP)
* @brief      Drops geo/map cache and pending work for one public IP (tree node removed).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::Forget(XDWORD publicIP)
{
  if(!publicIP || !mutex) return false;

  mutex->Lock();

  for(XDWORD c = queue.GetSize(); c > 0; c--)
    {
      if(queue.Get(c - 1) == publicIP) queue.DeleteIndex(c - 1);
    }

  for(XDWORD c = map_queue.GetSize(); c > 0; c--)
    {
      if(map_queue.Get(c - 1) == publicIP) map_queue.DeleteIndex(c - 1);
    }

  XTRACEMONITOR_GEOIP_ENTRY* entry = FindEntry(publicIP);
  if(entry)
    {
      entries.Delete(entry);
      delete entry;
    }

  mutex->UnLock();

  if(owner) owner->gui_tree.HideGeoTooltip();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::ClearCache()
* @brief      Clears all geo/map cache entries and pending queues (tree IP wipe).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::ClearCache()
{
  if(mutex)
    {
      mutex->Lock();
      queue.DeleteAll();
      map_queue.DeleteAll();
      entries.DeleteContents();
      entries.DeleteAll();
      mutex->UnLock();
    }
   else
    {
      queue.DeleteAll();
      map_queue.DeleteAll();
      entries.DeleteContents();
      entries.DeleteAll();
    }

  if(owner) owner->gui_tree.HideGeoTooltip();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::TryGet(XDWORD publicIP, XTRACEMONITOR_GEOIP_ENTRY& out)
* @brief      Copies a cache entry (any state) for the given public IP.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::TryGet(XDWORD publicIP, XTRACEMONITOR_GEOIP_ENTRY& out)
{
  if(!publicIP || !mutex) return false;

  mutex->Lock();
  XTRACEMONITOR_GEOIP_ENTRY* entry = FindEntry(publicIP);
  if(!entry)
    {
      mutex->UnLock();
      return false;
    }

  out.publicIP     = entry->publicIP;
  out.state        = entry->state;
  out.country      = entry->country;
  out.state_name   = entry->state_name;
  out.city         = entry->city;
  out.isp          = entry->isp;
  out.organization = entry->organization;
  out.latitude     = entry->latitude;
  out.longitude    = entry->longitude;
  out.map_state    = entry->map_state;
  out.map_path     = entry->map_path;
  mutex->UnLock();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::FormatTooltipText(XDWORD publicIP, XSTRING& out)
* @brief      Builds tooltip body: optional map IMAGE + address + operator.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::FormatTooltipText(XDWORD publicIP, XSTRING& out)
{
  out.Empty();

  XTRACEMONITOR_GEOIP_ENTRY entry;
  if(!TryGet(publicIP, entry))
    {
      out = NL("Geolocalizacion: pendiente…");
      return true;
    }

  if(entry.state == XTRACEMONITOR_GEOIP_STATE_PENDING)
    {
      out = NL("Geolocalizacion: consultando…");
      return true;
    }

  if(entry.state == XTRACEMONITOR_GEOIP_STATE_FAILED)
    {
      out = NL("Geolocalizacion: no disponible");
      return true;
    }

  XSTRING ipstr;
  ipstr.Format(NL("%d.%d.%d.%d"),
               (int)((publicIP >> 24) & 0xFF),
               (int)((publicIP >> 16) & 0xFF),
               (int)((publicIP >>  8) & 0xFF),
               (int)( publicIP        & 0xFF));

  XSTRING address;
  if(!entry.city.IsEmpty()) address = entry.city;
  if(!entry.state_name.IsEmpty())
    {
      if(!address.IsEmpty()) address.Add(NL(", "));
      address.Add(entry.state_name.Get());
    }
  if(!entry.country.IsEmpty())
    {
      if(!address.IsEmpty()) address.Add(NL(", "));
      address.Add(entry.country.Get());
    }
  if(address.IsEmpty()) address = NL("(sin ubicacion)");

  XSTRING oper;
  if(!entry.isp.IsEmpty())               oper = entry.isp;
   else if(!entry.organization.IsEmpty()) oper = entry.organization;
   else                                  oper = NL("(sin operador)");

  if((entry.map_state == XTRACEMONITOR_GEOMAP_STATE_READY) && (!entry.map_path.IsEmpty()))
    {
      // Prefer a GRAPHICS-relative resource name for ![IMAGE] (LoadFromFile resolves under graphics/).
      // Absolute paths also work after UI_ANIMATION absolute-path support.
      XSTRING image_ref = entry.map_path;
      XPATH   graphics;
      XPATH   mappath;

      mappath = entry.map_path.Get();

      if(GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, graphics))
        {
          graphics.Slash_Normalize(false);
          mappath.Slash_Normalize(false);
          graphics.Slash_Delete();

          if((mappath.GetSize() > graphics.GetSize()) && (mappath.Find(graphics.Get(), true) == 0))
            {
              image_ref = mappath.Get() + graphics.GetSize();
              while(image_ref.GetSize() && ((image_ref.Get()[0] == _C('/')) || (image_ref.Get()[0] == _C('\\'))))
                {
                  image_ref.DeleteCharacters(0, 1);
                }
            }
        }

      // Full-width map, one blank line, then public IP + location + operator.
      // After ![IMAGE] the TextBox caret is already on the first line under the map; the \n
      // advances one more line so the IP sits below a blank row (same 6 px padding as the tip).
      out.Format(NL("![IMAGE %s]\n%s\n%s\nOperador: %s"), image_ref.Get(), ipstr.Get(), address.Get(), oper.Get());
    }
   else if(entry.map_state == XTRACEMONITOR_GEOMAP_STATE_PENDING)
    {
      out.Format(NL("%s\n%s\nOperador: %s\nMapa: consultando…"), ipstr.Get(), address.Get(), oper.Get());
    }
   else
    {
      out.Format(NL("%s\n%s\nOperador: %s"), ipstr.Get(), address.Get(), oper.Get());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GEOIP::ThreadFunction(void* data)
* @brief      Worker loop: drain geo queue then map queue.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GEOIP::ThreadFunction(void* data)
{
  XTRACEMONITOR_GEOIP* self = (XTRACEMONITOR_GEOIP*)data;
  if(!self || !self->thread) return;

  while(self->thread->GetStatus() != XTHREADSTATUS_EXIT)
    {
      if(!self->PumpOne()) self->thread->Wait(80);
       else                self->thread->Wait(20);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::PumpOne()
* @brief      Processes one map job if any, else one geo job.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::PumpOne()
{
  if(PumpOneMap()) return true;
  return PumpOneGeo();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::PumpOneGeo()
* @brief      Resolves the next queued public IP (blocking scraper — worker thread only).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::PumpOneGeo()
{
#ifndef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE
  return false;
#else
  if(!mutex) return false;

  mutex->Lock();
  if(!queue.GetSize())
    {
      mutex->UnLock();
      return false;
    }

  XDWORD publicIP = queue.Get(0);
  queue.DeleteIndex(0);
  XTRACEMONITOR_GEOIP_ENTRY* entry = FindEntry(publicIP);
  mutex->UnLock();

  if(!entry) return true;

  DIOIP dioip;
  dioip.Set((XBYTE)((publicIP >> 24) & 0xFF),
            (XBYTE)((publicIP >> 16) & 0xFF),
            (XBYTE)((publicIP >>  8) & 0xFF),
            (XBYTE)( publicIP        & 0xFF));

  DIOGEOLOCATIONIP_RESULT geo;
  const bool ok = scraper.Get(dioip, geo, DIOSCRAPERWEBGEOLOCATIONIP_MAXTIMEOUT, NULL, true);

  bool enqueue_map = false;

  mutex->Lock();
  entry = FindEntry(publicIP);
  if(entry)
    {
      if(ok)
        {
          entry->state = XTRACEMONITOR_GEOIP_STATE_READY;
          if(geo.GetCountry())      entry->country      = geo.GetCountry();
          if(geo.GetState())        entry->state_name   = geo.GetState();
          if(geo.GetCity())         entry->city         = geo.GetCity();
          if(geo.GetISP())          entry->isp          = geo.GetISP();
          if(geo.GetOrganization()) entry->organization = geo.GetOrganization();
          entry->latitude  = geo.GetLatitude();
          entry->longitude = geo.GetLongitude();

          if((entry->latitude != 0.0f) || (entry->longitude != 0.0f))
            {
              entry->map_state = XTRACEMONITOR_GEOMAP_STATE_PENDING;
              enqueue_map = true;
            }
           else
            {
              entry->map_state = XTRACEMONITOR_GEOMAP_STATE_FAILED;
            }
        }
       else
        {
          entry->state     = XTRACEMONITOR_GEOIP_STATE_FAILED;
          entry->map_state = XTRACEMONITOR_GEOMAP_STATE_FAILED;
        }
    }
  mutex->UnLock();

  if(enqueue_map) EnqueueMap(publicIP);
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::PumpOneMap()
* @brief      Downloads the next queued static map PNG (worker thread only).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::PumpOneMap()
{
#ifndef DIO_SCRAPERWEB_STATICMAP_ACTIVE
  return false;
#else
  if(!mutex) return false;

  mutex->Lock();
  if(!map_queue.GetSize())
    {
      mutex->UnLock();
      return false;
    }

  XDWORD publicIP = map_queue.Get(0);
  map_queue.DeleteIndex(0);
  XTRACEMONITOR_GEOIP_ENTRY* entry = FindEntry(publicIP);

  float latitude  = 0.0f;
  float longitude = 0.0f;
  if(entry)
    {
      latitude  = entry->latitude;
      longitude = entry->longitude;
    }
  mutex->UnLock();

  if(!entry) return true;

  XSTRING outpath;
  if(!BuildMapOutPath(latitude, longitude, outpath))
    {
      mutex->Lock();
      entry = FindEntry(publicIP);
      if(entry) entry->map_state = XTRACEMONITOR_GEOMAP_STATE_FAILED;
      mutex->UnLock();
      return true;
    }

  XPATH xpath;
  xpath = outpath.Get();

  DIOSTATICMAP_RESULT mapresult;
  const bool ok = mapscraper.Get(latitude, longitude, xpath, mapresult,
                                 XTRACEMONITOR_GEOMAP_WIDTH, XTRACEMONITOR_GEOMAP_HEIGHT, XTRACEMONITOR_GEOMAP_ZOOM,
                                 NULL, DIOSCRAPERWEBSTATICMAP_MAXTIMEOUT, true);

  mutex->Lock();
  entry = FindEntry(publicIP);
  if(entry)
    {
      if(ok && mapresult.GetPath())
        {
          entry->map_state = XTRACEMONITOR_GEOMAP_STATE_READY;
          entry->map_path  = mapresult.GetPath();
        }
       else
        {
          entry->map_state = XTRACEMONITOR_GEOMAP_STATE_FAILED;
          entry->map_path.Empty();
        }
    }
  mutex->UnLock();
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::EnqueueMap(XDWORD publicIP)
* @brief      Queues a map download for an IP that already has coordinates.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::EnqueueMap(XDWORD publicIP)
{
#ifndef DIO_SCRAPERWEB_STATICMAP_ACTIVE
  (void)publicIP;
  return false;
#else
  if(!publicIP || !mutex) return false;

  mutex->Lock();
  for(XDWORD c = 0; c < map_queue.GetSize(); c++)
    {
      if(map_queue.Get(c) == publicIP)
        {
          mutex->UnLock();
          return true;
        }
    }
  map_queue.Add(publicIP);
  mutex->UnLock();
  return true;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GEOIP::BuildMapOutPath(float latitude, float longitude, XSTRING& outpath)
* @brief      Builds ROOT/cache/staticmaps/map_lat_lon_WxH_zZ.png and ensures the directory exists.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GEOIP::BuildMapOutPath(float latitude, float longitude, XSTRING& outpath)
{
  outpath.Empty();

  // Store under GRAPHICS/staticmaps so ![IMAGE] can also resolve as a relative UI resource.
  XPATH dir;
  if(!GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, dir)) return false;

  dir.Slash_Add();
  dir += NL("staticmaps");

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(xdir)
    {
      if(!xdir->Exist(dir.Get())) xdir->Make(dir.Get(), true);
      GEN_XFACTORY.Delete_Dir(xdir);
    }

  XSTRING name;
  name.Format(NL("map_%.4f_%.4f_%dx%d_z%d.png"),
              latitude, longitude,
              XTRACEMONITOR_GEOMAP_WIDTH, XTRACEMONITOR_GEOMAP_HEIGHT, XTRACEMONITOR_GEOMAP_ZOOM);

  dir.Slash_Add();
  dir += name.Get();
  outpath = dir.Get();
  return !outpath.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GEOIP_ENTRY* XTRACEMONITOR_GEOIP::FindEntry(XDWORD publicIP)
* @brief      Finds a cache entry by public IP (caller holds mutex).
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GEOIP_ENTRY* XTRACEMONITOR_GEOIP::FindEntry(XDWORD publicIP)
{
  for(XDWORD c = 0; c < entries.GetSize(); c++)
    {
      XTRACEMONITOR_GEOIP_ENTRY* entry = entries.Get(c);
      if(entry && (entry->publicIP == publicIP)) return entry;
    }
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GEOIP::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GEOIP::Clean()
{
  owner  = NULL;
  mutex  = NULL;
  thread = NULL;
}
