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
#include "XPath.h"
#include "XScheduler.h"
#include "XVector.h"

#include "DIOStream.h"
#include "DIOURL.h"

#include "Script_XEvent.h"

#include "APPFlowConsole.h"

class XFILETXT;

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

#define APPLICATION_NAMEAPP                       _L("ActionScriptQA")
#define APPLICATION_NAMEFILE                      _L("actionscriptqa")

#define APPLICATION_OWNER                         _L("EndoraSoft")

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

enum ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE
{
  ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK = 0 ,
  ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT      ,
  ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT      ,
  ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY       ,
};


class ACTIONSCRIPTQA_SCRIPTRECORD_STEP
{
  public:
                                    ACTIONSCRIPTQA_SCRIPTRECORD_STEP  ();
                                   ~ACTIONSCRIPTQA_SCRIPTRECORD_STEP  ();

    ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE type;
    XSTRING                         bitmapname;
    XSTRING                         text;          // typed text or key literal (ENTER, TAB, ...)
    int                             layoutx;
    int                             layouty;
};


class ACTIONSCRIPTQA_SCRIPTRECORD_SESSION
{
  public:
                                    ACTIONSCRIPTQA_SCRIPTRECORD_SESSION ();
                                   ~ACTIONSCRIPTQA_SCRIPTRECORD_SESSION ();

    XDWORD                          id;
    XSTRING                         appname;
    XPATH                           apppath;
    XSTRING                         windowtitle;
    XSTRING                         appkey;        // EnsureApplication_<appkey>
};

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

    bool                            Test_GetBaseName            (XSTRING& scriptentry, XSTRING& basename);
    bool                            Test_GetRootPath            (XSTRING& basename, XPATH& testroot);
    bool                            Test_EnsureLayout           (XSTRING& basename);
    bool                            Test_BindGraphics           (XSTRING& basename);
    bool                            Test_RestoreDefaultGraphics ();
    bool                            Test_LoadAndRun             (XSTRING& scriptentry);

    bool                            ScriptRecord_IsActive       ();
    bool                            ScriptRecord_Toggle         ();
    bool                            ScriptRecord_ClearRecorded  ();
    bool                            ScriptRecord_Update         ();
    bool                            ScriptRecord_UpdateKeys     ();
    bool                            ScriptRecord_SelectApp      (int screenx, int screeny);
    bool                            ScriptRecord_CaptureClick   (int screenx, int screeny, ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype);
    bool                            ScriptRecord_FlushText      (bool writescript = true);
    bool                            ScriptRecord_AddKeyLiteral  (XCHAR* literal);
    bool                            ScriptRecord_WriteScript    ();
    bool                            ScriptRecord_WriteCommonHelpers(XFILETXT& file);
    bool                            ScriptRecord_WriteEnsureApplicationForApp(XFILETXT& file, XSTRING& appname, XPATH& apppath, XSTRING& windowtitle, XSTRING& appkey);
    bool                            ScriptRecord_WriteMain      (XFILETXT& file, ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* currentsession);
    bool                            ScriptRecord_ExistingHasHelpers();
    bool                            ScriptRecord_ExistingHasEnsureAppKey(XSTRING& appkey);
    bool                            ScriptRecord_ExistingHasReadyMap();
    bool                            ScriptRecord_ParseQuotedAssign(XSTRING* line, XCHAR* varname, XSTRING& outvalue);
    void                            ScriptRecord_MakeAppKey     (XSTRING& appname, XSTRING& appkey);
    bool                            ScriptRecord_IsBrowserApp   (XSTRING& appname, XPATH& apppath);
    void                            ScriptRecord_BrowserWindowTitle(XSTRING& appname, XPATH& apppath, XSTRING& windowtitle);
    void                            ScriptRecord_AddJSVarString (XFILETXT& file, XCHAR* varname, XSTRING& value);
    bool                            ScriptRecord_GetWindowTextSafe(void* hwnd, XSTRING& title, XDWORD timeoutms = 300);
    bool                            ScriptRecord_GetRecordedBaseName(XSTRING& basename);
    bool                            ScriptRecord_ResolveScriptPath(XPATH& xpath);
    bool                            ScriptRecord_PrepareRecordedLayout();
    bool                            ScriptRecord_Reset          ();
    bool                            ScriptRecord_PrepareExisting();
    void                            ScriptRecord_Print          (XCHAR* mask, ...);
    void                            ScriptRecord_PathForScript  (XPATH& path);
    void                            ScriptRecord_EscapeForJS    (XSTRING& source, XSTRING& target);

    void                            Clean                       ();

    XTIMER*                         xtimerupdateconsole;
    XTIMER*                         xtimerscriptrun;

    XMUTEX*                         xmutexshowallstatus;

    SCRIPT*                         script;
    bool                            scriptsautorundone;

    bool                            scriptrecord_active;
    bool                            scriptrecord_appselected;
    bool                            scriptrecord_mousewasdown;
    bool                            scriptrecord_rbuttonwasdown;
    bool                            scriptrecord_keywasdown[256];
    XSTRING                         scriptrecord_textpending;
    XSTRING                         scriptrecord_appname;
    XPATH                           scriptrecord_apppath;
    XSTRING                         scriptrecord_windowtitle;
    void*                           scriptrecord_windowhandle;
    XVECTOR<ACTIONSCRIPTQA_SCRIPTRECORD_STEP*> scriptrecord_steps;
    XVECTOR<XSTRING*>               scriptrecord_existinglines;
    XVECTOR<ACTIONSCRIPTQA_SCRIPTRECORD_SESSION*> scriptrecord_sessions;
    XVECTOR<XSTRING*>               scriptrecord_ensurekeyswritten;
    bool                            scriptrecord_readymapwritten;
    XDWORD                          scriptrecord_sessionindex;
    XDWORD                          scriptrecord_nextbitmapindex;
};

#pragma endregion


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
#pragma region FUNCTIONS_PROTOTYPES


#pragma endregion


#endif
