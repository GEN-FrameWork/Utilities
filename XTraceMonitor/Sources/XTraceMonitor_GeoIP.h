/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GeoIP.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XVector.h"

#ifdef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE
#include "DIOScraperWebGeolocationIP.h"
#endif

#ifdef DIO_SCRAPERWEB_STATICMAP_ACTIVE
#include "DIOScraperWebStaticMap.h"
#endif

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XTRACEMONITOR_GEOIP_STATE
{
  XTRACEMONITOR_GEOIP_STATE_PENDING = 0 ,
  XTRACEMONITOR_GEOIP_STATE_READY       ,
  XTRACEMONITOR_GEOIP_STATE_FAILED
};

enum XTRACEMONITOR_GEOMAP_STATE
{
  XTRACEMONITOR_GEOMAP_STATE_NONE    = 0 ,
  XTRACEMONITOR_GEOMAP_STATE_PENDING     ,
  XTRACEMONITOR_GEOMAP_STATE_READY       ,
  XTRACEMONITOR_GEOMAP_STATE_FAILED
};

#ifndef XTRACEMONITOR_GEOMAP_WIDTH
  #define XTRACEMONITOR_GEOMAP_WIDTH    240
#endif
#ifndef XTRACEMONITOR_GEOMAP_HEIGHT
  #define XTRACEMONITOR_GEOMAP_HEIGHT   140
#endif
#ifndef XTRACEMONITOR_GEOMAP_ZOOM
  #define XTRACEMONITOR_GEOMAP_ZOOM     11
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;
class XTHREADCOLLECTED;
class XTRACEMONITOR;

class XTRACEMONITOR_GEOIP_ENTRY
{
  public:
                                    XTRACEMONITOR_GEOIP_ENTRY               ();
    virtual                        ~XTRACEMONITOR_GEOIP_ENTRY               ();

    XDWORD                          publicIP;
    XTRACEMONITOR_GEOIP_STATE       state;
    XSTRING                         country;
    XSTRING                         state_name;
    XSTRING                         city;
    XSTRING                         isp;
    XSTRING                         organization;
    float                           latitude;
    float                           longitude;

    XTRACEMONITOR_GEOMAP_STATE      map_state;
    XSTRING                         map_path;

  private:

    void                            Clean                                   ();
};


class XTRACEMONITOR_GEOIP
{
  public:
                                    XTRACEMONITOR_GEOIP                     ();
    virtual                        ~XTRACEMONITOR_GEOIP                     ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);
    bool                            Ini                                     ();
    bool                            End                                     ();

    bool                            Request                                 (XDWORD publicIP);
    bool                            Forget                                  (XDWORD publicIP);
    bool                            ClearCache                              ();
    bool                            TryGet                                  (XDWORD publicIP, XTRACEMONITOR_GEOIP_ENTRY& out);
    bool                            FormatTooltipText                       (XDWORD publicIP, XSTRING& out);

  private:

    static void                     ThreadFunction                          (void* data);

    bool                            PumpOne                                 ();
    bool                            PumpOneGeo                              ();
    bool                            PumpOneMap                              ();
    bool                            EnqueueMap                              (XDWORD publicIP);
    bool                            BuildMapOutPath                         (float latitude, float longitude, XSTRING& outpath);
    XTRACEMONITOR_GEOIP_ENTRY*      FindEntry                               (XDWORD publicIP);

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;
    XMUTEX*                         mutex;
    XTHREADCOLLECTED*               thread;
    XVECTOR<XDWORD>                 queue;
    XVECTOR<XDWORD>                 map_queue;
    XVECTOR<XTRACEMONITOR_GEOIP_ENTRY*> entries;

  #ifdef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE
    DIOSCRAPERWEBGEOLOCATIONIP      scraper;
  #endif

  #ifdef DIO_SCRAPERWEB_STATICMAP_ACTIVE
    DIOSCRAPERWEBSTATICMAP          mapscraper;
  #endif
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
