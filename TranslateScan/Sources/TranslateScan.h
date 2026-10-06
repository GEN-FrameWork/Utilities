/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       TranslateScan.h
* 
* @class      TRANSLATESCAN
* @brief      Translate Scan class
* @ingroup    APPLICATION
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

#include "XDateTime.h"
#include "XFSMachine.h"
#include "XString.h"
#include "XScheduler.h"

#include "DIOStream.h"
#include "DIOURL.h"

#include "APPFlowConsole.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum TRANSLATESCANXFSMEVENTS
{
  TRANSLATESCAN_XFSMEVENT_NONE                        = 0 ,
  TRANSLATESCAN_XFSMEVENT_INI                             ,
  TRANSLATESCAN_XFSMEVENT_UPDATE                          ,
  TRANSLATESCAN_XFSMEVENT_END                             ,

  TRANSLATESCAN_LASTEVENT
};


enum TRANSLATESCANXFSMSTATES
{
  TRANSLATESCAN_XFSMSTATE_NONE                        = 0 ,
  TRANSLATESCAN_XFSMSTATE_INI                             ,
  TRANSLATESCAN_XFSMSTATE_UPDATE                          ,
  TRANSLATESCAN_XFSMSTATE_END                             ,

  TRANSLATESCAN_LASTSTATE
};


enum TRANSLATESCANTASKID
{
  TRANSLATESCANTASKID_UNKNOWN                         = 0 ,
  TRANSLATESCANTASKID_CHECKINTERTRANSLATESCANECTION       ,
  TRANSLATESCANTASKID_CHECKMEMORYSTATUS                   ,
};


#define APPLICATION_VERSION                       0
#define APPLICATION_SUBVERSION                    1
#define APPLICATION_SUBVERSIONERR                 0

#define APPLICATION_NAMEAPP                       _L("Translate Scan")
#define APPLICATION_NAMEFILE                      _L("translatescan")

#define APPLICATION_OWNER                         _L("EndoraSoft")

#define APPLICATION_YEAROFCREATION                2025




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIME;
class XTIMER;
class XRAND;
class XTHREAD;
class XDIR;
class XSCHEDULER;
class XSCHEDULER_XEVENT;
class DIOINTERNET;
class DIOCHECKTCPIPCONNECTIONS;
class DIOCHECKINTERTRANSLATESCANECTION;
class DIOSCRAPERWEBPUBLICIP;
class DIOSCRAPERWEBGEOLOCATIONIP;
class DIOSCRAPERWEBUSERAGENTID;
class DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT;
class APPFLOWINTERNETSERVICES;
class TRANSLATESCAN_MANAGER;


class TRANSLATESCAN : public APPFLOWCONSOLE, public XOBSERVER, public XFSMACHINE
{
  public:
                                                TRANSLATESCAN                       ();
    virtual                                    ~TRANSLATESCAN                       ();

    bool                                        InitFSMachine                       ();

    bool                                        AppProc_Ini                         ();
    bool                                        AppProc_FirstUpdate                 ();
    bool                                        AppProc_Update                      ();
    bool                                        AppProc_End                         ();

    bool                                        KeyValidSecuences                   (int key); 

    TRANSLATESCAN_MANAGER*                      GetManager                          ();

    bool                                        Show_AllStatus                      ();

  private:
    
    void                                        HandleEvent                         (XEVENT* xevent);

    void                                        Clean                               ();

    XTIMER*                                     xtimerupdateconsole;
    XMUTEX*                                     xmutexshowallstatus;   

    TRANSLATESCAN_MANAGER*                      manager;

    XPATH                                       operationdir;

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

extern TRANSLATESCAN* translatescan;



