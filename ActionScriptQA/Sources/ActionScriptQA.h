/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA.h
* 
* @class      ACTIONSCRIPTQA
* @brief      Locomotive Operations for Kinetic Inspections
* @ingroup    
* 
* @author     Abraham J. Velez 
* @date       24/07/2023 7:37:36
* 
* @copyright  CAF Signalling  All rights reserved.
* 
* --------------------------------------------------------------------------------------------------------------------*/

#ifndef _ACTIONSCRIPTQA_H_
#define _ACTIONSCRIPTQA_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
#pragma region INCLUDES

#include "XDateTime.h"
#include "XFSMachine.h"
#include "XString.h"
#include "XScheduler.h"

#include "DIOStream.h"
#include "DIOURL.h"

#include "Script_XEvent.h"

#include "APPFlowConsole.h"

#pragma endregion


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/
#pragma region DEFINES_ENUMS

enum ACTIONSCRIPTQAXFSMEVENTS
{
  ACTIONSCRIPTQA_XFSMEVENT_NONE                = 0 ,
  ACTIONSCRIPTQA_XFSMEVENT_INI                     ,
  ACTIONSCRIPTQA_XFSMEVENT_UPDATE                  ,
  ACTIONSCRIPTQA_XFSMEVENT_END                     ,

  ACTIONSCRIPTQA_LASTEVENT
};


enum ACTIONSCRIPTQAXFSMSTATES
{
  ACTIONSCRIPTQA_XFSMSTATE_NONE                = 0 ,
  ACTIONSCRIPTQA_XFSMSTATE_INI                     ,
  ACTIONSCRIPTQA_XFSMSTATE_UPDATE                  ,
  ACTIONSCRIPTQA_XFSMSTATE_END                     ,

  ACTIONSCRIPTQA_LASTSTATE
};


enum ACTIONSCRIPTQATASKID
{
  ACTIONSCRIPTQATASKID_UNKNOWN                 = 0 ,
  ACTIONSCRIPTQATASKID_CHECKMEMORYSTATUS           ,
};


#define APPLICATION_VERSION                       0
#define APPLICATION_SUBVERSION                    5
#define APPLICATION_SUBVERSIONERR                 0

#define APPLICATION_NAMEAPP                       __L("ActionScriptQA")
#define APPLICATION_NAMEFILE                      __L("actionscriptqa")

#define APPLICATION_OWNER                         __L("EndoraSoft")

#define APPLICATION_YEAROFCREATION                2023

#pragma endregion


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/
#pragma region CLASS

class XTIME;
class XTIMER;
class XRAND;
class XTHREAD;
class XDIR;
class XSCHEDULER;
class XSCHEDULER_XEVENT;
class DIOCHECKTCPIPCONNECTIONS;
class DIOCHECKINTERNETCONNECTION;
class DIOSCRAPERWEBPUBLICIP;
class DIOSCRAPERWEBGEOLOCATIONIP;
class DIOSCRAPERWEBUSERAGENTID;
class GRPBITMAPSECUENCE;
class GRPXEVENT;
class ACTIONSCRIPTQA_CFG;
class SCRIPT;

class ACTIONSCRIPTQA : public APPFLOWCONSOLE, public XFSMACHINE
{
  public:
                                    ACTIONSCRIPTQA                        ();
    virtual                        ~ACTIONSCRIPTQA                        ();

    bool                            InitFSMachine               ();

    bool                            AppProc_Ini                 ();
    bool                            AppProc_FirstUpdate         ();
    bool                            AppProc_Update              ();
    bool                            AppProc_End                 ();

    bool                            KeyValidSecuences           (int key);    

    bool                            ExecScripts                 ();
    
    bool                            Show_AppStatus              ();
    bool                            Show_ActionScriptQAStatus             ();
    bool                            Show_AllStatus              ();

  private:

    static void                     AdjustLibraries             (SCRIPT* script);

    void                            HandleEvent_Script          (SCRIPT_XEVENT* event);
    void                            HandleEvent                 (XEVENT* xevent);  

    void                            Clean                       ();

    XTIMER*                         xtimerupdateconsole;
    XTIMER*                         xtimerscriptrun;

    XMUTEX*                         xmutexshowallstatus;

    SCRIPT*                         script;
};

#pragma endregion


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
#pragma region FUNCTIONS_PROTOTYPES


#pragma endregion


#endif

