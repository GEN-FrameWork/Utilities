/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA.cpp
* 
* @class      ACTIONSCRIPTQA
* @brief      Locomotive Operations for Kinetic Inspections
* @ingroup    
* 
* @author     Abraham J. Velez 
* @date       24/07/2023 7:37:02
* 
* @copyright  CAF Signalling  All rights reserved.
* 
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ----------------------------------------------------------------------------------------*/
#pragma region PRECOMPILATION_INCLUDES

#include "GEN_Defines.h"

#pragma endregion


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
#pragma region INCLUDES

#include "ActionScriptQA.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "VersionFrameWork.h"

#include "XPath.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XFactory.h"
#include "XRand.h"
#include "XDir.h"
#include "XString.h"
#include "XSystem.h"
#include "XLog.h"
#include "XConsole.h"
#include "XFileTXT.h"
#include "XFileCSV.h"
#include "XFileXML.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XScheduler.h"
#include "XScheduler_XEvent.h"
#include "XConsole.h"
#include "XThread.h"
#include "XTrace.h"
#include "XObserver.h"
#include "XProcessManager.h"

#include "XSleep.h"

#include "HashMD5.h"

#include "DIOFactory.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"

#include "DIOWebClient_XEvent.h"
#include "DIOWebClient.h"

#include "DIOCheckTCPIPConnections.h"
#include "DIOCheckInternetConnection.h"

#include "DIOScraperWeb.h"
#include "DIOScraperWebPublicIP.h"
#include "DIOScraperWebGeolocationIP.h"
#include "DIOScraperWebUserAgentID.h"

#include "APPFlowBase.h"
#include "APPFlowExtended.h"
#include "APPFlowLog.h"



#include "Script_Language_G.h"
#include "Script_Language_Lua.h"
#include "Script_Language_Javascript.h"
#include "Script_Lib_Console.h"

#include "GRPFactory.h"
#include "GRPScreen.h"
#include "GRPBitmap.h"
#include "GRPBitmapFile.h"
#include "GRPRect.h"

#include "XProcessManager.h"
#include "XFileTXT.h"
#include "XPathsManager.h"

#include "ActionScriptQA_CFG.h"

#ifdef WINDOWS
#include <windows.h>

struct ACTIONSCRIPTQA_FINDWND
{
  XSTRING* appname;
  XSTRING* windowtitle;
  HWND     found;
};

static BOOL CALLBACK ActionScriptQA_EnumFindWindow(HWND enumhwnd, LPARAM lparam)
{
  ACTIONSCRIPTQA_FINDWND* find = (ACTIONSCRIPTQA_FINDWND*)lparam;
  if(!find || !IsWindowVisible(enumhwnd)) return TRUE;

  XCHAR titlebuf[1024];
  titlebuf[0] = 0;

  DWORD_PTR getresult = 0;
  if(!SendMessageTimeout(enumhwnd, WM_GETTEXT, (WPARAM)1024, (LPARAM)titlebuf, SMTO_ABORTIFHUNG | SMTO_BLOCK, 200, &getresult))
    {
      return TRUE;
    }
  titlebuf[1023] = 0;
  if(!titlebuf[0]) return TRUE;

  bool titlematch = find->windowtitle && !find->windowtitle->IsEmpty() &&
                    (find->windowtitle->Compare(titlebuf, true) == 0);

  DWORD pid = 0;
  GetWindowThreadProcessId(enumhwnd, &pid);
  bool namematch = false;
  if(pid && find->appname && !find->appname->IsEmpty())
    {
      HANDLE hproc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
      if(!hproc) hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
      if(hproc)
        {
          XCHAR pathbuf[MAX_PATH + 1];
          pathbuf[0] = 0;
          DWORD pathsize = MAX_PATH;
          if(QueryFullProcessImageName(hproc, 0, pathbuf, &pathsize) && pathbuf[0])
            {
              XPATH   fullpath = pathbuf;
              XSTRING name;
              fullpath.GetNamefileExt(name);
              if(!name.IsEmpty() && name.Compare((*find->appname), true) == 0) namematch = true;
            }
          CloseHandle(hproc);
        }
    }

  if(namematch || titlematch)
    {
      find->found = enumhwnd;
      return FALSE;
    }

  return TRUE;
}
#endif

#include "XMemory_Control.h"


#pragma endregion


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/
#pragma region GENERAL_VARIABLE

APPLICATIONCREATEINSTANCE(ACTIONSCRIPTQA, actionscriptqa)

#pragma endregion


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/
#pragma region CLASS_MEMBERS


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ACTIONSCRIPTQA::ACTIONSCRIPTQA()
* @brief      Constructor
* @ingroup    
* 
* @return     Does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA::ACTIONSCRIPTQA() : XFSMACHINE(0)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ACTIONSCRIPTQA::~ACTIONSCRIPTQA()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    
* 
* @return     Does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA::~ACTIONSCRIPTQA()
{
  ScriptRecord_Reset();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ACTIONSCRIPTQA_SCRIPTRECORD_STEP::ACTIONSCRIPTQA_SCRIPTRECORD_STEP()
* @brief      Constructor
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_SCRIPTRECORD_STEP::ACTIONSCRIPTQA_SCRIPTRECORD_STEP()
{
  type    = ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK;
  layoutx = 0;
  layouty = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ACTIONSCRIPTQA_SCRIPTRECORD_STEP::~ACTIONSCRIPTQA_SCRIPTRECORD_STEP()
* @brief      Destructor
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_SCRIPTRECORD_STEP::~ACTIONSCRIPTQA_SCRIPTRECORD_STEP()
{
  bitmapname.Empty();
  text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ACTIONSCRIPTQA_SCRIPTRECORD_SESSION::ACTIONSCRIPTQA_SCRIPTRECORD_SESSION()
* @brief      Constructor
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_SCRIPTRECORD_SESSION::ACTIONSCRIPTQA_SCRIPTRECORD_SESSION()
{
  id = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ACTIONSCRIPTQA_SCRIPTRECORD_SESSION::~ACTIONSCRIPTQA_SCRIPTRECORD_SESSION()
* @brief      Destructor
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_SCRIPTRECORD_SESSION::~ACTIONSCRIPTQA_SCRIPTRECORD_SESSION()
{
  appname.Empty();
  apppath.Empty();
  windowtitle.Empty();
  appkey.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::InitFSMachine()
* @brief      InitFSMachine
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::InitFSMachine()
{
  if(!AddState( ACTIONSCRIPTQA_XFSMSTATE_NONE            ,
                ACTIONSCRIPTQA_XFSMEVENT_INI             , ACTIONSCRIPTQA_XFSMSTATE_INI           ,
                ACTIONSCRIPTQA_XFSMEVENT_END             , ACTIONSCRIPTQA_XFSMSTATE_END           ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( ACTIONSCRIPTQA_XFSMSTATE_INI             ,
                ACTIONSCRIPTQA_XFSMEVENT_UPDATE          , ACTIONSCRIPTQA_XFSMSTATE_UPDATE        ,
                ACTIONSCRIPTQA_XFSMEVENT_END             , ACTIONSCRIPTQA_XFSMSTATE_END           ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if (!AddState(ACTIONSCRIPTQA_XFSMSTATE_UPDATE,
                ACTIONSCRIPTQA_XFSMEVENT_END             , ACTIONSCRIPTQA_XFSMSTATE_END           ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( ACTIONSCRIPTQA_XFSMSTATE_END             ,
                ACTIONSCRIPTQA_XFSMEVENT_NONE            , ACTIONSCRIPTQA_XFSMSTATE_NONE          ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::AppProc_Ini()
* @brief      AppProc_Ini
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::AppProc_Ini()
{
  XSTRING string;
  XSTRING stringresult;
  XPATH   xpathsection;
  XPATH   xpath;
  bool    status;

  //-------------------------------------------------------------------------------------------------

  GEN_SET_VERSION(APPLICATION_NAMEAPP, APPLICATION_NAMEFILE, APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR, APPLICATION_OWNER, APPLICATION_YEAROFCREATION)

  Application_GetName()->Set(APPLICATION_NAMEAPP);

  //--------------------------------------------------------------------------------------------------

  // ACTIVATEXTHREADGROUP(XTHREADGROUPID_SCHEDULER);
  // ACTIVATEXTHREADGROUP(XTHREADGROUPID_DIOSTREAM);

  //--------------------------------------------------------------------------------------------------

  XTRACE_SETAPPLICATIONNAME((*Application_GetName()));
  XTRACE_SETAPPLICATIONVERSION(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
  XTRACE_SETAPPLICATIONID(string);

  //--------------------------------------------------------------------------------------------------

  GEN_XPATHSMANAGER.AdjustRootPathDefault(APPFLOW_DEFAULT_DIRECTORY_ROOT);

  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS           ,  APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS            ,  APPFLOW_DEFAULT_DIRECTORY_SCRIPTS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_TESTS              ,  APPFLOW_DEFAULT_DIRECTORY_TESTS);

  GEN_XPATHSMANAGER.CreateAllPathSectionOnDisk();

  //--------------------------------------------------------------------------------------------------

  InitFSMachine();

  //--------------------------------------------------------------------------------------

  xmutexshowallstatus = GEN_XFACTORY.Create_Mutex();
  if(!xmutexshowallstatus) return false;

  //--------------------------------------------------------------------------------------

  APPFLOW_CFG_SETAUTOMATICTRACETARGETS

  
  //--------------------------------------------------------------------------------------------------

  /*
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathsection);
  xpath.Create(3 , xpathsection.Get(), MINIWEBSERVER_LNG_NAMEFILE, XTRANSLATION_NAMEFILEEXT);

  if(!GEN_XTRANSLATION.Ini(xpath))
    {
      return false;
    }
  */

  GEN_XTRANSLATION.SetActual(XLANGUAGE_ISO_639_3_CODE_SPA);

  //--------------------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPStart(&APPFLOW_CFG, this);

  //--------------------------------------------------------------------------------------------------

  SetEvent(ACTIONSCRIPTQA_XFSMEVENT_INI);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::AppProc_FirstUpdate()
* @brief      AppProc_FirstUpdate
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::AppProc_FirstUpdate()
{
  //--------------------------------------------------------------------------------------

  xtimerupdateconsole = GEN_XFACTORY.CreateTimer();
  if(!xtimerupdateconsole) return false;

  xtimerscriptrun = GEN_XFACTORY.CreateTimer();
  if(!xtimerscriptrun) return false;

  // Optional headless/CI autorun: ACTIONSCRIPTQA_AUTORUN=1 (or "si"/"yes"/"true")
  if(!scriptsautorundone)
    {
      char* envautorun = getenv("ACTIONSCRIPTQA_AUTORUN");
      if(envautorun)
        {
          XSTRING autorun;

          autorun.Set(envautorun);
          if((!autorun.Compare(__L("1"), true)) ||
             (!autorun.Compare(__L("si"), true)) ||
             (!autorun.Compare(__L("yes"), true)) ||
             (!autorun.Compare(__L("true"), true)))
            {
              scriptsautorundone = true;

              // Keep this console out of the way so InpSim mouse clicks hit the target UI.
              if(GetConsole())
                {
                  GetConsole()->Minimize();
                }

              GEN_XSLEEP.MilliSeconds(400);
              ExecScripts();
            }
        }
    }

  //--------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::AppProc_Update()
* @brief      AppProc_Update
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::AppProc_Update()
{
  if(GetEvent()==ACTIONSCRIPTQA_XFSMEVENT_NONE) // Not new event
    {
      switch(GetCurrentState())
        {
          case ACTIONSCRIPTQA_XFSMSTATE_NONE        : break;

          case ACTIONSCRIPTQA_XFSMSTATE_INI         : break;

          case ACTIONSCRIPTQA_XFSMSTATE_UPDATE      : if(GetExitType() == APPFLOWBASE_EXITTYPE_UNKNOWN)
                                                        {
                                                          if(ScriptRecord_IsActive())
                                                            {
                                                              ScriptRecord_Update();
                                                            }

                                                          if(xtimerupdateconsole)
                                                            {
                                                              if(!ScriptRecord_IsActive())
                                                                {
                                                                  if(xtimerupdateconsole->GetMeasureSeconds() >= 1)
                                                                    {
                                                                      Show_AllStatus();
                                                                      xtimerupdateconsole->Reset();
                                                                    }
                                                                }

                                                              if(console->KBHit())
                                                                {
                                                                  int key = console->GetChar();
                                                                  KeyValidSecuences(key);
                                                                }
                                                            }
                                                         }                            
                                                      break;

          case ACTIONSCRIPTQA_XFSMSTATE_END         : SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                                                      break;

        }
    }
   else //  New event
    {
      if(GetEvent()<ACTIONSCRIPTQA_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case ACTIONSCRIPTQA_XFSMSTATE_NONE    : break;

              case ACTIONSCRIPTQA_XFSMSTATE_INI     : SetEvent(ACTIONSCRIPTQA_XFSMEVENT_UPDATE);
                                                      break;

              case ACTIONSCRIPTQA_XFSMSTATE_UPDATE  : break;

              case ACTIONSCRIPTQA_XFSMSTATE_END     : break;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::AppProc_End()
* @brief      AppProc_End
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::AppProc_End()
{
  XSTRING string;
  XSTRING stringresult;

  //--------------------------------------------------------------------------------------

  SetEvent(ACTIONSCRIPTQA_XFSMEVENT_END);

  //--------------------------------------------------------------------------------------

  if(xmutexshowallstatus)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexshowallstatus);
      xmutexshowallstatus = NULL;
    }

  //--------------------------------------------------------------------------------------

  if(xtimerscriptrun)
    {
      GEN_XFACTORY.DeleteTimer(xtimerscriptrun);
      xtimerscriptrun = NULL;
    }

  if(xtimerupdateconsole)
    {
      GEN_XFACTORY.DeleteTimer(xtimerupdateconsole);
      xtimerupdateconsole = NULL;
    }


  //--------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPEnd();
  APPFLOW_EXTENDED.DelInstance();  
  APPFLOW_CFG.DelInstance();

  //--------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::KeyValidSecuences(int key)
* @brief      Processes valid key sequences.
* @ingroup    EXAMPLES
*
* @param[in]  key : Key code to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::KeyValidSecuences(int key)
{
  XCHAR character = (XCHAR)key;

  // Console extended keys (F1..): first byte 0x00 / 0xE0, second = scan code.
  if((key == 0x00) || (key == 0xE0))
    {
      if(console && console->KBHit())
        {
          int scancode = console->GetChar();
          if(scancode == 0x3B) // F1
            {
              ScriptRecord_Toggle();
              return true;
            }
          if(scancode == 0x3F) // F5
            {
              ScriptRecord_ClearRecorded();
              return true;
            }
        }
    }

  if((character<32) || (character>127)) character = __C('?');
  APPFLOW_LOG_ENTRY(XLOGLEVEL_WARNING, APPFLOW_CFG_LOG_SECTIONID_STATUSAPP, false, __L("Key pressed: 0x%02X [%c]"), key, character);

  if(console) console->Printf(__L("\r    \r"));

  switch(key)
    {
      case 0x20 : if(!ScriptRecord_IsActive()) ExecScripts();
                  break;                 

      case 0x1B : // ESC: leave record mode first, then exit app
                  if(ScriptRecord_IsActive())
                    {
                      ScriptRecord_Toggle();
                    }
                   else
                    {
                      SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                    }
                  break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::ExecScripts()
* @brief      ExecScripts
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ExecScripts()
{
  if(xtimerglobal)  
    {
      xtimerglobal->Reset();
    }

  XVECTOR<XSTRING*>* listscripts = APPFLOW_CFG.Scripts_GetAll();
  if(listscripts)
    {
      for(XDWORD c=0; c<listscripts->GetSize(); c++)
        {
          XSTRING* entry = listscripts->Get(c);
          if(!entry || entry->IsEmpty()) continue;
          Test_LoadAndRun(*entry);
        }
    }

  Test_RestoreDefaultGraphics();
                                                 
  Show_BlankLine();
                                                  
  if(xtimerglobal)
    { 
      XSTRING string;
                                                      
      xtimerglobal->GetMeasureString(string, true);
      console->Printf(__L("Total working time %s"), string.Get());                                                      
    }

  Show_BlankLine();
  Show_BlankLine();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_GetBaseName(XSTRING& scriptentry, XSTRING& basename)
* @brief      Basename of a CFG script entry (strip path and extension)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_GetBaseName(XSTRING& scriptentry, XSTRING& basename)
{
  basename.Empty();
  if(scriptentry.IsEmpty()) return false;

  XPATH entry = scriptentry.Get();
  entry.GetNamefile(basename);
  if(basename.IsEmpty())
    {
      basename = scriptentry.Get();
    }

  return !basename.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_GetRootPath(XSTRING& basename, XPATH& testroot)
* @brief      Absolute path assets/Tests/<basename>
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_GetRootPath(XSTRING& basename, XPATH& testroot)
{
  testroot.Empty();
  if(basename.IsEmpty()) return false;

  if(!GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_TESTS, testroot)) return false;
  testroot.Slash_Add();
  testroot += basename.Get();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_EnsureLayout(XSTRING& basename)
* @brief      Create Tests/<basename>/{graphics,evidences}
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_EnsureLayout(XSTRING& basename)
{
  XPATH testroot;
  if(!Test_GetRootPath(basename, testroot)) return false;

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(!xdir->Exist(testroot)) xdir->Make(testroot, true);

  XPATH graphics = testroot;
  graphics.Slash_Add();
  graphics += __L("graphics");
  if(!xdir->Exist(graphics)) xdir->Make(graphics, true);

  XPATH evidences = testroot;
  evidences.Slash_Add();
  evidences += __L("evidences");
  if(!xdir->Exist(evidences)) xdir->Make(evidences, true);

  GEN_XFACTORY.Delete_Dir(xdir);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_BindGraphics(XSTRING& basename)
* @brief      Point GRAPHICS section at Tests/<basename>/graphics
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_BindGraphics(XSTRING& basename)
{
  if(basename.IsEmpty()) return false;

  XSTRING section;
  section.Format(__L("%s/%s/graphics"), APPFLOW_DEFAULT_DIRECTORY_TESTS, basename.Get());
  return GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, section.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_RestoreDefaultGraphics()
* @brief      Restore GRAPHICS section to assets/graphics
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_RestoreDefaultGraphics()
{
  return GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::Test_LoadAndRun(XSTRING& scriptentry)
* @brief      Load and run one test from assets/Tests/<base>/<base>.js
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Test_LoadAndRun(XSTRING& scriptentry)
{
  XSTRING basename;
  if(!Test_GetBaseName(scriptentry, basename)) return false;
  if(!Test_EnsureLayout(basename)) return false;
  if(!Test_BindGraphics(basename)) return false;

  XPATH xpath;
  if(!Test_GetRootPath(basename, xpath)) return false;
  xpath.Slash_Add();

  // Keep original extension from CFG entry when present; default to .js
  XPATH entrypath = scriptentry.Get();
  XSTRING nameext;
  entrypath.GetNamefileExt(nameext);
  if(nameext.IsEmpty())
    {
      nameext = basename.Get();
      nameext += __L(".js");
    }
  xpath += nameext.Get();

  SCRIPT* script = SCRIPT::Create(nameext.Get());
  if(!script)
    {
      ScriptRecord_Print(__L("[Tests] Cannot create script engine for %s"), nameext.Get());
      Test_RestoreDefaultGraphics();
      return false;
    }

  AdjustLibraries(script);

  if(!script->Load(xpath))
    {
      ScriptRecord_Print(__L("[Tests] Cannot load %s"), xpath.Get());
      GEN_DELETE script;
      Test_RestoreDefaultGraphics();
      return false;
    }

  ScriptRecord_Print(__L("[Tests] Running %s"), xpath.Get());
  script->Run();
  GEN_DELETE script;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::Show_AppStatus()
* @brief      Show_AppStatus
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Show_AppStatus()
{
  XSTRING string;
  XSTRING string2;

  XDWORD  total;
  XDWORD  free;

  GEN_XSYSTEM.GetMemoryInfo(total,free);

  string  = __L("Memoria total");
  string2.Format(__L("%d Kb, libre %d Kb (el %d%%%%)"), total, free, GEN_XSYSTEM.GetFreeMemoryPercent());
  Show_Line(string, string2);

  XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
  if(datetime)
    {
      datetime->Read();

      string  = __L("Fecha ");
      datetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD | XDATETIME_FORMAT_TEXTMONTH | XDATETIME_FORMAT_ADDDAYOFWEEK, string2);
      Show_Line(string, string2);

      GEN_XFACTORY.DeleteDateTime(datetime);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::Show_ActionScriptQAStatus()
* @brief      Show_ActionScriptQAStatus
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Show_ActionScriptQAStatus()
{
  XSTRING string;
  XSTRING string2;

  string  = __L("Script Status");
  string2.Format(__L("%s"), script->GetNameScript()->Get());

  Show_Line(string, string2);


  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::Show_AllStatus()
* @brief      Show_AllStatus
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::Show_AllStatus()
{
  console->Clear();

  if(xmutexshowallstatus) xmutexshowallstatus->Lock();

  APPFLOW_EXTENDED.ShowAll();

  if(xmutexshowallstatus) xmutexshowallstatus->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ACTIONSCRIPTQA::AdjustLibraries(SCRIPT* script)
* @brief      AdjustLibraries
* @ingroup    
* 
* @param[in]  script : 
* 
* @return     void : does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::AdjustLibraries(SCRIPT* script)
{
  if(!script)
    {
      return;
    }

  #ifdef SCRIPT_LIB_CFG_ACTIVE
  
  SCRIPT_SET_LIB_CFG(script, APP_CFG);

  #endif

  #ifdef SCRIPT_LIB_CONSOLE_ACTIVE

  if(actionscriptqa && actionscriptqa->GetConsole())
    {
      SCRIPT_SET_LIB_CONSOLE(script, actionscriptqa->GetConsole());
    }

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ACTIONSCRIPTQA::HandleEvent_Script(SCRIPT_XEVENT* event)
* @brief      Handle Event for the observer manager of this class
* @note       INTERNAL
* @ingroup    
* 
* @param[in]  event : 
* 
* @return     void : does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::HandleEvent_Script(SCRIPT_XEVENT* event)
{
  switch(event->GetEventType())
    {
      case SCRIPT_XEVENT_TYPE_ERROR    : XTRACE_PRINTCOLOR(4,__L("Script ERROR [%d]: %s line %d -> \"%s\""), event->GetError(), event->GetErrorText()->Get(), event->GetNLine(), event->GetCurrentToken()->Get());
                                         break;

      case SCRIPT_XEVENT_TYPE_BREAK    : XTRACE_PRINTCOLOR(4,__L("Script BREAK: line %d -> \"%s\""), event->GetNLine(), event->GetCurrentToken()->Get());
                                         break;

    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ACTIONSCRIPTQA::HandleEvent(XEVENT* xevent)
* @brief      Handle Event for the observer manager of this class
* @note       INTERNAL
* @ingroup    
* 
* @param[in]  xevent : 
* 
* @return     void : does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
       case XEVENT_TYPE_SCRIPT     : { SCRIPT_XEVENT* event = (SCRIPT_XEVENT*)xevent;
                                       if(!event) return;

                                       HandleEvent_Script(event);
                                     }
                                     break; 
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ACTIONSCRIPTQA::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    
* 
* @return     void : does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::Clean()
{
  xtimerupdateconsole         = NULL;
  xtimerscriptrun             = NULL;

  xmutexshowallstatus         = NULL;

  script                      = NULL;
  scriptsautorundone          = false;

  scriptrecord_active           = false;
  scriptrecord_appselected      = false;
  scriptrecord_mousewasdown     = false;
  scriptrecord_rbuttonwasdown   = false;
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
  scriptrecord_readymapwritten  = false;
  scriptrecord_appname.Empty();
  scriptrecord_apppath.Empty();
  scriptrecord_windowtitle.Empty();
  scriptrecord_textpending.Empty();
  memset(scriptrecord_keywasdown, 0, sizeof(scriptrecord_keywasdown));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_IsActive()
* @brief      ScriptRecord_IsActive
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_IsActive()
{
  return scriptrecord_active;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_GetRecordedBaseName(XSTRING& basename)
* @brief      Basename for the recorder output (default Tests_Recorded)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_GetRecordedBaseName(XSTRING& basename)
{
  basename.Empty();

  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  if(!outname || outname->IsEmpty())
    {
      basename = __L("Tests_Recorded");
      return true;
    }

  return Test_GetBaseName(*outname, basename);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ResolveScriptPath(XPATH& xpath)
* @brief      Path of the recorded script: assets/Tests/<base>/<base>.js
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ResolveScriptPath(XPATH& xpath)
{
  XSTRING basename;
  if(!ScriptRecord_GetRecordedBaseName(basename)) return false;
  if(!Test_GetRootPath(basename, xpath)) return false;

  xpath.Slash_Add();

  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  if(outname && !outname->IsEmpty())
    {
      XPATH entry = outname->Get();
      XSTRING nameext;
      entry.GetNamefileExt(nameext);
      if(nameext.IsEmpty())
        {
          nameext = basename.Get();
          nameext += __L(".js");
        }
      xpath += nameext.Get();
    }
   else
    {
      xpath += __L("Tests_Recorded.js");
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_PrepareRecordedLayout()
* @brief      Ensure Tests_Recorded layout and bind its graphics folder
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_PrepareRecordedLayout()
{
  XSTRING basename;
  if(!ScriptRecord_GetRecordedBaseName(basename)) return false;
  if(!Test_EnsureLayout(basename)) return false;
  return Test_BindGraphics(basename);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_Print(XCHAR* mask, ...)
* @brief      ScriptRecord_Print
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_Print(XCHAR* mask, ...)
{
  if(!console || !mask) return;

  XSTRING  outstring;
  va_list  arg;

  va_start(arg, mask);
  outstring.FormatArg(mask, &arg);
  va_end(arg);

  outstring.Add(__L("\n"));
  // Use Print (not Printf): the message may contain '%' from window titles / URLs.
  console->Print(outstring.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_Reset()
* @brief      ScriptRecord_Reset
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_Reset()
{
  scriptrecord_steps.DeleteContents();
  scriptrecord_steps.DeleteAll();

  scriptrecord_existinglines.DeleteContents();
  scriptrecord_existinglines.DeleteAll();

  scriptrecord_sessions.DeleteContents();
  scriptrecord_sessions.DeleteAll();

  scriptrecord_ensurekeyswritten.DeleteContents();
  scriptrecord_ensurekeyswritten.DeleteAll();

  scriptrecord_appselected      = false;
  scriptrecord_mousewasdown     = false;
  scriptrecord_rbuttonwasdown   = false;
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
  scriptrecord_readymapwritten  = false;
  scriptrecord_appname.Empty();
  scriptrecord_apppath.Empty();
  scriptrecord_windowtitle.Empty();
  scriptrecord_textpending.Empty();
  memset(scriptrecord_keywasdown, 0, sizeof(scriptrecord_keywasdown));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_Toggle()
* @brief      ScriptRecord_Toggle
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_Toggle()
{
  if(scriptrecord_active)
    {
      if(scriptrecord_appselected)
        {
          ScriptRecord_Print(__L("[ScriptRecord] Finishing session: flushing text..."));
          ScriptRecord_FlushText(false);
          ScriptRecord_Print(__L("[ScriptRecord] Finishing session: writing script..."));
          if(ScriptRecord_WriteScript())
            {
              ScriptRecord_Print(__L("[ScriptRecord] Saved script and left record mode."));
            }
           else
            {
              ScriptRecord_Print(__L("[ScriptRecord] ERROR writing script; left record mode anyway."));
            }
        }
       else
        {
          ScriptRecord_Print(__L("[ScriptRecord] Left record mode (no app selected)."));
        }

      scriptrecord_active = false;
      ScriptRecord_Reset();
      Test_RestoreDefaultGraphics();
      return true;
    }

  ScriptRecord_Reset();
  scriptrecord_active = true;
  ScriptRecord_PrepareRecordedLayout();
  ScriptRecord_PrepareExisting();

  XSTRING basename;
  ScriptRecord_GetRecordedBaseName(basename);
  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  ScriptRecord_Print(__L("[ScriptRecord] ON (F1/ESC finish, F5 discard+delete Tests_Recorded)."));
  ScriptRecord_Print(__L("[ScriptRecord] 1) Left-click a window of the app under test."));
  ScriptRecord_Print(__L("[ScriptRecord] 2) Left-click UI targets (mouse click steps)."));
  ScriptRecord_Print(__L("[ScriptRecord] 3) Right-click a zone to wait/assert it appears on screen."));
  ScriptRecord_Print(__L("[ScriptRecord] 4) Type text (login/password); ENTER/TAB/BACKSPACE recorded as keys."));
  ScriptRecord_Print(__L("[ScriptRecord] Output: Tests/%s/%s"), basename.Get(), outname ? outname->Get() : __L("Tests_Recorded.js"));
  ScriptRecord_Print(__L("[ScriptRecord] Session function: Recorded_%03d  (next bitmap %03d)"), (int)scriptrecord_sessionindex, (int)scriptrecord_nextbitmapindex);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ClearRecorded()
* @brief      F5: leave record mode without saving, then delete Tests_Recorded (and contents)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ClearRecorded()
{
  if(scriptrecord_active)
    {
      scriptrecord_active = false;
      ScriptRecord_Reset();
      Test_RestoreDefaultGraphics();
      ScriptRecord_Print(__L("[ScriptRecord] Record mode cancelled (F5)."));
    }

  XSTRING basename;
  if(!ScriptRecord_GetRecordedBaseName(basename))
    {
      ScriptRecord_Print(__L("[ScriptRecord] ERROR resolving recorded test name."));
      return false;
    }

  XPATH testroot;
  if(!Test_GetRootPath(basename, testroot))
    {
      ScriptRecord_Print(__L("[ScriptRecord] ERROR resolving Tests/%s path."), basename.Get());
      return false;
    }

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir)
    {
      ScriptRecord_Print(__L("[ScriptRecord] ERROR creating XDIR."));
      return false;
    }

  bool status = true;
  if(xdir->Exist(testroot))
    {
      status = xdir->Delete(testroot, true);
      if(status)
        {
          ScriptRecord_Print(__L("[ScriptRecord] Deleted Tests/%s (directory and files)."), basename.Get());
        }
       else
        {
          ScriptRecord_Print(__L("[ScriptRecord] ERROR deleting Tests/%s."), basename.Get());
        }
    }
   else
    {
      ScriptRecord_Print(__L("[ScriptRecord] Nothing to delete: Tests/%s (missing)."), basename.Get());
    }

  GEN_XFACTORY.Delete_Dir(xdir);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_Update()
* @brief      ScriptRecord_Update — poll left mouse button while recording
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_Update()
{
  #ifndef WINDOWS
  return false;
  #else

  bool ldown = ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
  bool rdown = ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0);

  if(ldown && !scriptrecord_mousewasdown)
    {
      POINT pt;

      if(GetCursorPos(&pt))
        {
          if(!scriptrecord_appselected)
            {
              ScriptRecord_SelectApp(pt.x, pt.y);
            }
           else
            {
              ScriptRecord_CaptureClick(pt.x, pt.y, ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK);
            }
        }
    }

  if(rdown && !scriptrecord_rbuttonwasdown)
    {
      POINT pt;

      if(scriptrecord_appselected && GetCursorPos(&pt))
        {
          ScriptRecord_CaptureClick(pt.x, pt.y, ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT);
        }
    }

  scriptrecord_mousewasdown   = ldown;
  scriptrecord_rbuttonwasdown = rdown;

  if(scriptrecord_appselected)
    {
      ScriptRecord_UpdateKeys();
    }

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_EscapeForJS(XSTRING& source, XSTRING& target)
* @brief      Escape a string for embedding in a JavaScript double-quoted literal
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_EscapeForJS(XSTRING& source, XSTRING& target)
{
  target.Empty();

  for(XDWORD c=0; c<source.GetSize(); c++)
    {
      XCHAR ch = source.Get()[c];
      switch(ch)
        {
          case __C('\\') : target.Add(__L("\\\\")); break;
          case __C('\"') : target.Add(__L("\\\"")); break;
          case __C('\n') : target.Add(__L("\\n"));  break;
          case __C('\r') : target.Add(__L("\\r"));  break;
          case __C('\t') : target.Add(__L("\\t"));  break;
          // '%' must not reach XSTRING::Format / Printf masks (FormatArg can hang on "%...").
          case __C('%')  : target.Add(__L("_"));    break;
                default  : target.Add(ch);          break;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_FlushText()
* @brief      Flush pending typed characters as a text step
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_FlushText(bool writescript)
{
  if(scriptrecord_textpending.IsEmpty()) return true;

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_STEP();
  if(!step) return false;

  step->type = ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT;
  step->text = scriptrecord_textpending;
  scriptrecord_steps.Add(step);

  ScriptRecord_Print(__L("[ScriptRecord] Step %d: text \"%s\" (%d chars)"), (int)scriptrecord_steps.GetSize(), scriptrecord_textpending.Get(), (int)scriptrecord_textpending.GetSize());
  scriptrecord_textpending.Empty();
  if(writescript) ScriptRecord_WriteScript();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_AddKeyLiteral(XCHAR* literal)
* @brief      Append a special-key step (ENTER, TAB, ...)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_AddKeyLiteral(XCHAR* literal)
{
  if(!literal || !literal[0]) return false;

  ScriptRecord_FlushText();

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_STEP();
  if(!step) return false;

  step->type = ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY;
  step->text = literal;
  scriptrecord_steps.Add(step);

  ScriptRecord_Print(__L("[ScriptRecord] Step %d: key %s"), (int)scriptrecord_steps.GetSize(), literal);
  ScriptRecord_WriteScript();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_UpdateKeys()
* @brief      Poll keyboard edges while recording (text + special keys)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_UpdateKeys()
{
  #ifndef WINDOWS
  return false;
  #else

  if(!scriptrecord_appselected) return false;

  for(int vk = 1; vk < 256; vk++)
    {
      // Mouse buttons and pure modifiers are not recorded as key steps.
      if((vk >= VK_LBUTTON && vk <= VK_XBUTTON2) ||
         (vk == VK_SHIFT) || (vk == VK_CONTROL) || (vk == VK_MENU) ||
         (vk == VK_LSHIFT) || (vk == VK_RSHIFT) ||
         (vk == VK_LCONTROL) || (vk == VK_RCONTROL) ||
         (vk == VK_LMENU) || (vk == VK_RMENU) ||
         (vk == VK_LWIN) || (vk == VK_RWIN) ||
         (vk == VK_CAPITAL) || (vk == VK_NUMLOCK) || (vk == VK_SCROLL))
        {
          scriptrecord_keywasdown[vk] = ((GetAsyncKeyState(vk) & 0x8000) != 0);
          continue;
        }

      // F1 / ESC control the recorder itself.
      if((vk == VK_F1) || (vk == VK_ESCAPE) || (vk >= VK_F2 && vk <= VK_F24))
        {
          scriptrecord_keywasdown[vk] = ((GetAsyncKeyState(vk) & 0x8000) != 0);
          continue;
        }

      bool down = ((GetAsyncKeyState(vk) & 0x8000) != 0);
      bool was  = scriptrecord_keywasdown[vk];
      scriptrecord_keywasdown[vk] = down;

      if(!(down && !was)) continue;

      XCHAR* literal = NULL;
      switch(vk)
        {
          case VK_RETURN : literal = __L("ENTER");      break;
          case VK_TAB    : literal = __L("TAB");        break;
          case VK_BACK   : literal = __L("BACKSPACE");  break;
          case VK_DELETE : literal = __L("DEL");        break;
          case VK_LEFT   : literal = __L("LEFT ARROW"); break;
          case VK_RIGHT  : literal = __L("RIGHT ARROW");break;
          case VK_UP     : literal = __L("UP ARROW");   break;
          case VK_DOWN   : literal = __L("DOWN ARROW"); break;
          case VK_HOME   : literal = __L("HOME");       break;
          case VK_END    : literal = __L("END");        break;
          case VK_PRIOR  : literal = __L("PAGE UP");    break;
          case VK_NEXT   : literal = __L("PAGE DOWN");  break;
          case VK_INSERT : literal = __L("INS");        break;
          case VK_SPACE  : // Space goes into typed text (login fields).
                           break;
                 default : break;
        }

      if(literal)
        {
          ScriptRecord_AddKeyLiteral(literal);
          continue;
        }

      BYTE  keystate[256];
      WCHAR chars[8];
      UINT  scancode;

      memset(keystate, 0, sizeof(keystate));
      for(int i=0; i<256; i++)
        {
          SHORT s = GetAsyncKeyState(i);
          keystate[i] = (BYTE)(((s & 0x8000) ? 0x80 : 0) | ((s & 0x0001) ? 0x01 : 0));
        }

      scancode = MapVirtualKey((UINT)vk, MAPVK_VK_TO_VSC);
      int n = ToUnicode((UINT)vk, scancode, keystate, chars, 8, 0);
      if(n <= 0) continue;

      for(int c=0; c<n; c++)
        {
          WCHAR ch = chars[c];
          if(ch < 32 && ch != 9) continue; // skip controls except TAB (handled above)
          scriptrecord_textpending.Add((XCHAR)ch);
        }

      if(!scriptrecord_textpending.IsEmpty())
        {
          // Keep buffering; flush on special key, click, or end of recording.
          ScriptRecord_Print(__L("[ScriptRecord] Typing... (%d chars)"), (int)scriptrecord_textpending.GetSize());
        }
    }

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_SelectApp(int screenx, int screeny)
* @brief      Identify target application from window under cursor
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_SelectApp(int screenx, int screeny)
{
  #ifndef WINDOWS
  return false;
  #else

  POINT pt;
  pt.x = screenx;
  pt.y = screeny;

  HWND hwnd = WindowFromPoint(pt);
  if(!hwnd)
    {
      ScriptRecord_Print(__L("[ScriptRecord] No window under cursor."));
      return false;
    }

  hwnd = GetAncestor(hwnd, GA_ROOT);
  if(!hwnd) return false;

  // Resolve process from HWND directly (avoid Application_GetRunningList — very slow with browsers).
  DWORD pid = 0;
  GetWindowThreadProcessId(hwnd, &pid);
  if(!pid)
    {
      ScriptRecord_Print(__L("[ScriptRecord] Cannot get process id for window."));
      return false;
    }

  XCHAR pathbuf[MAX_PATH + 1];
  pathbuf[0] = 0;

  HANDLE hproc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
  if(!hproc)
    {
      hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    }

  if(hproc)
    {
      DWORD pathsize = MAX_PATH;
      if(!QueryFullProcessImageName(hproc, 0, pathbuf, &pathsize))
        {
          pathbuf[0] = 0;
        }
      CloseHandle(hproc);
    }

  if(!pathbuf[0])
    {
      ScriptRecord_Print(__L("[ScriptRecord] Cannot resolve executable path for window."));
      return false;
    }

  XPATH  fullpath = pathbuf;
  XSTRING appname;
  fullpath.GetNamefileExt(appname);
  if(appname.IsEmpty())
    {
      ScriptRecord_Print(__L("[ScriptRecord] Cannot resolve executable name for window."));
      return false;
    }

  if(appname.Find(APPLICATION_NAMEFILE, true) != XSTRING_NOTFOUND)
    {
      ScriptRecord_Print(__L("[ScriptRecord] Ignored click on ActionScriptQA itself."));
      return false;
    }

  scriptrecord_windowhandle = (void*)hwnd;
  scriptrecord_appname      = appname.Get();
  scriptrecord_apppath      = fullpath.Get();
  ScriptRecord_PathForScript(scriptrecord_apppath);

  // Browsers: never call GetWindowText (can hang) and use a stable short title for Screen_* Find().
  if(ScriptRecord_IsBrowserApp(scriptrecord_appname, scriptrecord_apppath))
    {
      ScriptRecord_BrowserWindowTitle(scriptrecord_appname, scriptrecord_apppath, scriptrecord_windowtitle);
    }
   else
    {
      if(!ScriptRecord_GetWindowTextSafe(hwnd, scriptrecord_windowtitle, 300))
        {
          scriptrecord_windowtitle.Empty();
        }
    }

  scriptrecord_appselected = true;

  ScriptRecord_Print(__L("[ScriptRecord] App selected: %s"), scriptrecord_appname.Get());
  ScriptRecord_Print(__L("[ScriptRecord] Path: %s"), scriptrecord_apppath.Get());
  ScriptRecord_Print(__L("[ScriptRecord] Window: %s"), scriptrecord_windowtitle.Get());
  if(ScriptRecord_IsBrowserApp(scriptrecord_appname, scriptrecord_apppath))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Browser detected: Ensure will use OpenURL (edit url in script)."));
    }
  ScriptRecord_Print(__L("[ScriptRecord] Click UI controls to capture bitmaps..."));

  ScriptRecord_WriteScript();

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_CaptureClick(int screenx, int screeny, ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype)
* @brief      Capture bitmap around cursor: CLICK (LMB) or WAIT/assert (RMB)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_CaptureClick(int screenx, int screeny, ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype)
{
  #ifndef WINDOWS
  return false;
  #else

  if(!scriptrecord_appselected) return false;
  if((steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK) && (steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT)) return false;

  ScriptRecord_FlushText();

  HWND hwnd = (HWND)scriptrecord_windowhandle;
  if(!hwnd || !IsWindow(hwnd))
    {
      // Re-resolve by title/name without Application_GetRunningList (browsers make that very slow).
      ACTIONSCRIPTQA_FINDWND data;
      data.appname     = &scriptrecord_appname;
      data.windowtitle = &scriptrecord_windowtitle;
      data.found       = NULL;

      EnumWindows(ActionScriptQA_EnumFindWindow, (LPARAM)&data);

      if(!data.found)
        {
          ScriptRecord_Print(__L("[ScriptRecord] Target window no longer valid."));
          return false;
        }

      scriptrecord_windowhandle = (void*)data.found;
      hwnd = data.found;
      ScriptRecord_Print(__L("[ScriptRecord] Target window handle refreshed."));
    }

  RECT  winrect;
  RECT  clientrect;
  POINT ptclient;

  if(!GetWindowRect(hwnd, &winrect)) return false;
  if(!GetClientRect(hwnd, &clientrect)) return false;

  // CaptureContent/GetDC use client-area coordinates.
  ptclient.x = screenx;
  ptclient.y = screeny;
  if(!ScreenToClient(hwnd, &ptclient))
    {
      ScriptRecord_Print(__L("[ScriptRecord] ScreenToClient failed."));
      return false;
    }

  int clientw = clientrect.right  - clientrect.left;
  int clienth = clientrect.bottom - clientrect.top;

  if((clientw <= 0) || (clienth <= 0) ||
     (ptclient.x < 0) || (ptclient.y < 0) ||
     (ptclient.x >= clientw) || (ptclient.y >= clienth))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Click outside client area (ignored)."));
      return false;
    }

  int captw = APPFLOW_CFG.ScriptRecord_GetCaptureWidth();
  int capth = APPFLOW_CFG.ScriptRecord_GetCaptureHeight();
  if(captw < 8)  captw = 8;
  if(capth < 8)  capth = 8;
  if(captw > clientw) captw = clientw;
  if(capth > clienth) capth = clienth;

  int halfw = captw / 2;
  int halfh = capth / 2;

  GRPRECTINT rect;
  rect.x1 = ptclient.x - halfw;
  rect.y1 = ptclient.y - halfh;

  if(rect.x1 < 0) rect.x1 = 0;
  if(rect.y1 < 0) rect.y1 = 0;
  if((rect.x1 + captw) > clientw) rect.x1 = clientw - captw;
  if((rect.y1 + capth) > clienth) rect.y1 = clienth - capth;

  rect.x2 = rect.x1 + captw;
  rect.y2 = rect.y1 + capth;

  // Layout fallback uses outer window origin (Screen_GetPosXY without bitmap).
  int layoutx = screenx - winrect.left;
  int layouty = screeny - winrect.top;

  GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
  if(!screen)
    {
      ScriptRecord_Print(__L("[ScriptRecord] CreateScreen failed."));
      return false;
    }

  screen->SetHandle((void*)hwnd);
  screen->SetWidth(clientw);
  screen->SetHeight(clienth);

  GRPBITMAP* bitmap = screen->CaptureContent(&rect, (void*)hwnd);
  GEN_GRPFACTORY.DeleteScreen(screen);

  if(!bitmap)
    {
      ScriptRecord_Print(__L("[ScriptRecord] CaptureContent failed."));
      return false;
    }

  XSTRING* prefix = APPFLOW_CFG.ScriptRecord_GetBitmapPrefix();
  XSTRING  bmpname;
  bmpname.Format(__L("%s%03d.png"), prefix ? prefix->Get() : __L("rec_"), (int)scriptrecord_nextbitmapindex);

  XPATH xpathbmp;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbmp);

  XDIR* xdirgraphics = GEN_XFACTORY.Create_Dir();
  if(xdirgraphics)
    {
      if(!xdirgraphics->Exist(xpathbmp))
        {
          xdirgraphics->Make(xpathbmp, true);
        }
      GEN_XFACTORY.Delete_Dir(xdirgraphics);
    }

  xpathbmp.Slash_Add();
  xpathbmp += bmpname.Get();

  GRPBITMAPFILE bmpfile;
  bool saved = bmpfile.Save(xpathbmp, bitmap, 100);
  GEN_GRPFACTORY.DeleteBitmap(bitmap);

  if(!saved)
    {
      #ifndef GRP_BITMAP_FILE_PNG_ACTIVE
      ScriptRecord_Print(__L("[ScriptRecord] Failed to save %s (PNG support not compiled)."), xpathbmp.Get());
      #else
      ScriptRecord_Print(__L("[ScriptRecord] Failed to save %s"), xpathbmp.Get());
      #endif
      return false;
    }

  scriptrecord_nextbitmapindex++;

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_STEP();
  if(!step) return false;

  step->type       = steptype;
  step->bitmapname = bmpname;
  step->layoutx    = layoutx;
  step->layouty    = layouty;
  scriptrecord_steps.Add(step);

  if(steptype == ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT)
    {
      ScriptRecord_Print(__L("[ScriptRecord] Step %d: WAIT %s capt %dx%d (assert appears)"),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth);
    }
   else
    {
      ScriptRecord_Print(__L("[ScriptRecord] Step %d: CLICK %s capt %dx%d at client %d,%d layout %d,%d"),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth, ptclient.x, ptclient.y, layoutx, layouty);
    }

  ScriptRecord_WriteScript();

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_PathForScript(XPATH& path)
* @brief      Normalize path separators to '/' for portable JS strings
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_PathForScript(XPATH& path)
{
  if(!path.IsEmpty()) path.Slash_Normalize(false);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_PrepareExisting()
* @brief      Load existing record script so new sessions append Recorded_NNN
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_PrepareExisting()
{
  scriptrecord_existinglines.DeleteContents();
  scriptrecord_existinglines.DeleteAll();
  scriptrecord_sessions.DeleteContents();
  scriptrecord_sessions.DeleteAll();
  scriptrecord_ensurekeyswritten.DeleteContents();
  scriptrecord_ensurekeyswritten.DeleteAll();
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
  scriptrecord_readymapwritten  = false;

  XPATH xpath;
  if(!ScriptRecord_ResolveScriptPath(xpath)) return false;

  XSTRING* prefix = APPFLOW_CFG.ScriptRecord_GetBitmapPrefix();
  XCHAR*   pref   = prefix && !prefix->IsEmpty() ? prefix->Get() : __L("rec_");
  int      prefsize = 0;
  while(pref[prefsize]) prefsize++;

  XFILETXT file;
  if(file.Open(xpath, true))
    {
      if(file.ReadAllFile())
        {
          int  mainstart   = -1;
          int  maxrecorded = 0;
          bool hasrecorded = false;

          for(int c=0; c<file.GetNLines(); c++)
            {
              XSTRING* line = file.GetLine(c);
              if(!line) continue;

              int posrec = line->Find(__L("function Recorded_"), false);
              if(posrec != XSTRING_NOTFOUND)
                {
                  // checkvalidchars=false: line has letters; only digits at the index matter.
                  int n = line->ConvertToInt(posrec + 18, NULL, false);
                  if(n > 0)
                    {
                      hasrecorded = true;
                      if(n > maxrecorded) maxrecorded = n;

                      ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_SESSION();
                      if(session)
                        {
                          session->id = (XDWORD)n;

                          for(int d=c+1; d<file.GetNLines(); d++)
                            {
                              XSTRING* look = file.GetLine(d);
                              if(!look) continue;
                              if(look->Find(__L("function "), false) != XSTRING_NOTFOUND) break;

                              XSTRING value;
                              if(ScriptRecord_ParseQuotedAssign(look, __L("appname"), value))
                                {
                                  session->appname = value.Get();
                                  ScriptRecord_MakeAppKey(session->appname, session->appkey);
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, __L("apppath"), value))
                                {
                                  session->apppath = value.Get();
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, __L("windowtitle"), value))
                                {
                                  session->windowtitle = value.Get();
                                }

                              if(!session->appname.IsEmpty() && !session->apppath.IsEmpty() && !session->windowtitle.IsEmpty())
                                break;
                            }

                          scriptrecord_sessions.Add(session);
                        }
                    }
                }

              if(mainstart < 0)
                {
                  if(line->Find(__L("function main("), false) != XSTRING_NOTFOUND)
                    {
                      mainstart = c;
                    }
                }

              int posbmp = line->Find(pref, false);
              if(posbmp != XSTRING_NOTFOUND)
                {
                  int n = line->ConvertToInt(posbmp + prefsize, NULL, false);
                  if(n >= (int)scriptrecord_nextbitmapindex)
                    {
                      scriptrecord_nextbitmapindex = (XDWORD)(n + 1);
                    }
                }
            }

          if(hasrecorded)
            {
              int last = (mainstart >= 0) ? mainstart : file.GetNLines();
              for(int c=0; c<last; c++)
                {
                  XSTRING* line = file.GetLine(c);
                  if(!line) continue;
                  XSTRING* copy = GEN_NEW XSTRING();
                  if(!copy) continue;
                  (*copy) = line->Get();
                  scriptrecord_existinglines.Add(copy);
                }

              scriptrecord_sessionindex = (XDWORD)(maxrecorded + 1);
            }
           else if(mainstart >= 0)
            {
              // Legacy single-main script: keep helpers and rename main -> Recorded_001
              for(int c=0; c<mainstart; c++)
                {
                  XSTRING* line = file.GetLine(c);
                  if(!line) continue;
                  XSTRING* copy = GEN_NEW XSTRING();
                  if(!copy) continue;
                  (*copy) = line->Get();
                  scriptrecord_existinglines.Add(copy);
                }

              XSTRING* renamed = GEN_NEW XSTRING();
              if(renamed)
                {
                  (*renamed) = __L("function Recorded_001()");
                  scriptrecord_existinglines.Add(renamed);
                }

              for(int c=mainstart+1; c<file.GetNLines(); c++)
                {
                  XSTRING* line = file.GetLine(c);
                  if(!line) continue;
                  XSTRING* copy = GEN_NEW XSTRING();
                  if(!copy) continue;
                  (*copy) = line->Get();
                  scriptrecord_existinglines.Add(copy);
                }

              ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_SESSION();
              if(session)
                {
                  session->id = 1;
                  scriptrecord_sessions.Add(session);
                }
              scriptrecord_sessionindex = 2;
              ScriptRecord_Print(__L("[ScriptRecord] Existing main() migrated to Recorded_001."));
            }
           else
            {
              for(int c=0; c<file.GetNLines(); c++)
                {
                  XSTRING* line = file.GetLine(c);
                  if(!line) continue;
                  XSTRING* copy = GEN_NEW XSTRING();
                  if(!copy) continue;
                  (*copy) = line->Get();
                  scriptrecord_existinglines.Add(copy);
                }
            }

          if(scriptrecord_existinglines.GetSize())
            {
              ScriptRecord_Print(__L("[ScriptRecord] Appending to existing script (%d kept lines, %d sessions)."), (int)scriptrecord_existinglines.GetSize(), (int)scriptrecord_sessions.GetSize());
            }
        }

      file.Close();
    }

  // Also advance bitmap index from files already on disk.
  XPATH xpathgraphics;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathgraphics);

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(xdir)
    {
      XDIRELEMENT element;
      XSTRING     pattern;
      pattern.Format(__L("%s*.png"), pref);

      if(xdir->FirstSearch(xpathgraphics, pattern, &element))
        {
          do
            {
              XPATH* name = element.GetNameFile();
              if(name && !name->IsEmpty())
                {
                  int pos = name->Find(pref, false);
                  if(pos != XSTRING_NOTFOUND)
                    {
                      int n = name->ConvertToInt(pos + prefsize, NULL, false);
                      if(n >= (int)scriptrecord_nextbitmapindex)
                        {
                          scriptrecord_nextbitmapindex = (XDWORD)(n + 1);
                        }
                    }
                }
            }
          while(xdir->NextSearch(&element));
        }

      GEN_XFACTORY.Delete_Dir(xdir);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasHelpers()
* @brief      True if kept script lines already include evidence helpers
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasHelpers()
{
  for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
    {
      XSTRING* line = scriptrecord_existinglines.Get(c);
      if(!line) continue;
      if(line->Find(__L("function EvidenceStart"), false) != XSTRING_NOTFOUND) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasReadyMap()
* @brief      True if kept script lines already declare __qa_application_ready
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasReadyMap()
{
  for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
    {
      XSTRING* line = scriptrecord_existinglines.Get(c);
      if(!line) continue;
      if(line->Find(__L("__qa_application_ready"), false) != XSTRING_NOTFOUND) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasEnsureAppKey(XSTRING& appkey)
* @brief      True if EnsureApplication_<appkey> already exists in kept lines
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ExistingHasEnsureAppKey(XSTRING& appkey)
{
  if(appkey.IsEmpty()) return false;

  XSTRING needle;
  needle.Format(__L("function EnsureApplication_%s"), appkey.Get());

  for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
    {
      XSTRING* line = scriptrecord_existinglines.Get(c);
      if(!line) continue;
      if(line->Find(needle.Get(), false) != XSTRING_NOTFOUND) return true;
    }

  for(XDWORD c=0; c<scriptrecord_ensurekeyswritten.GetSize(); c++)
    {
      XSTRING* key = scriptrecord_ensurekeyswritten.Get(c);
      if(key && key->Compare(appkey) == 0) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ParseQuotedAssign(XSTRING* line, XCHAR* varname, XSTRING& outvalue)
* @brief      Parse  var varname = "value";  from a script line
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ParseQuotedAssign(XSTRING* line, XCHAR* varname, XSTRING& outvalue)
{
  outvalue.Empty();
  if(!line || !varname) return false;

  XSTRING pattern;
  pattern.Format(__L("var %s"), varname);
  int pos = line->Find(pattern.Get(), false);
  if(pos == XSTRING_NOTFOUND) return false;

  int q1 = line->FindCharacter(__C('\"'), (XDWORD)pos);
  if(q1 < 0) return false;
  int q2 = line->FindCharacter(__C('\"'), (XDWORD)(q1 + 1));
  if(q2 < 0 || q2 <= q1 + 1) return false;

  outvalue.Set(line->Get() + q1 + 1, (XDWORD)(q2 - q1 - 1));
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_MakeAppKey(XSTRING& appname, XSTRING& appkey)
* @brief      Build a JS-safe EnsureApplication_<key> suffix from the exe name
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_MakeAppKey(XSTRING& appname, XSTRING& appkey)
{
  appkey.Empty();

  XSTRING probe;
  for(XDWORD c=0; c<appname.GetSize(); c++)
    {
      XCHAR ch = appname.Get()[c];
      if(probe.Character_IsAlpha(ch) || probe.Character_IsNumber(ch, false))
        {
          appkey.Add(ch);
        }
       else if(appkey.GetSize() && appkey.Character_GetLast() != __C('_'))
        {
          appkey.Add(__C('_'));
        }
    }

  while(appkey.GetSize() && appkey.Character_GetLast() == __C('_'))
    {
      appkey.DeleteLastCharacter();
    }

  if(appkey.IsEmpty()) appkey = __L("app");
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_IsBrowserApp(XSTRING& appname, XPATH& apppath)
* @brief      True if the captured process looks like Chrome, Edge or Firefox
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_IsBrowserApp(XSTRING& appname, XPATH& apppath)
{
  XSTRING probe;

  probe = appname.Get();
  probe.ToLowerCase();
  if(probe.Find(__L("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Compare(__L("edge.exe"), false) == 0) return true;
  if(probe.Compare(__L("edge"), false) == 0) return true;

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(__L("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("\\edge\\"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(__L("/edge/"), false) != XSTRING_NOTFOUND) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_BrowserWindowTitle(...)
* @brief      Stable short title for browsers (page titles change and may contain '%' / crash FormatArg)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_BrowserWindowTitle(XSTRING& appname, XPATH& apppath, XSTRING& windowtitle)
{
  XSTRING probe = appname.Get();
  probe.ToLowerCase();

  if(probe.Find(__L("msedge"), false) != XSTRING_NOTFOUND || probe.Compare(__L("edge.exe"), false) == 0)
    {
      windowtitle = __L("Microsoft Edge");
      return;
    }

  if(probe.Find(__L("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = __L("Mozilla Firefox");
      return;
    }

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(__L("msedge"), false) != XSTRING_NOTFOUND || probe.Find(__L("\\edge\\"), false) != XSTRING_NOTFOUND || probe.Find(__L("/edge/"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = __L("Microsoft Edge");
      return;
    }

  if(probe.Find(__L("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = __L("Mozilla Firefox");
      return;
    }

  // Default browser family (chrome / chromium).
  windowtitle = __L("Google Chrome");
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void ACTIONSCRIPTQA::ScriptRecord_AddJSVarString(...)
* @brief      Emit  var name = "value";  without XSTRING::Format (avoids FormatArg hangs)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA::ScriptRecord_AddJSVarString(XFILETXT& file, XCHAR* varname, XSTRING& value)
{
  if(!varname) return;

  XSTRING escaped;
  ScriptRecord_EscapeForJS(value, escaped);

  XSTRING line;
  line  = __L("  var ");
  line += varname;
  line += __L("     = \"");
  line += escaped;
  line += __L("\";");
  file.AddLine(line);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_GetWindowTextSafe(...)
* @brief      Read window title with timeout (GetWindowText can hang on browser UI threads)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_GetWindowTextSafe(void* hwndvoid, XSTRING& title, XDWORD timeoutms)
{
  title.Empty();

  #ifndef WINDOWS
  return false;
  #else

  HWND hwnd = (HWND)hwndvoid;
  if(!hwnd || !IsWindow(hwnd)) return false;

  DWORD_PTR lenresult = 0;
  if(!SendMessageTimeout(hwnd, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, timeoutms, &lenresult))
    {
      return false;
    }

  int len = (int)lenresult;
  if(len <= 0) return false;
  if(len > 1023) len = 1023;

  XCHAR titlebuf[1024];
  titlebuf[0] = 0;

  DWORD_PTR getresult = 0;
  if(!SendMessageTimeout(hwnd, WM_GETTEXT, (WPARAM)1024, (LPARAM)titlebuf, SMTO_ABORTIFHUNG | SMTO_BLOCK, timeoutms, &getresult))
    {
      return false;
    }

  titlebuf[1023] = 0;
  if(!titlebuf[0]) return false;

  title = titlebuf;
  // Neutralize '%' so later Format/Printf of derived messages cannot hang FormatArg.
  title.Character_Change(__C('%'), __C('_'));
  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteEnsureApplicationForApp(...)
* @brief      Write EnsureApplication_<key>() — launch once, focus, leave ready for tests
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteEnsureApplicationForApp(XFILETXT& file, XSTRING& appname, XPATH& apppath, XSTRING& windowtitle, XSTRING& appkey)
{
  XSTRING line;
  XPATH   pathscript = apppath;
  ScriptRecord_PathForScript(pathscript);

  bool isbrowser = ScriptRecord_IsBrowserApp(appname, apppath);

  line.Format(__L("function EnsureApplication_%s()"), appkey.Get());
  file.AddLine(line);
  file.AddLine(__L("{"));
  ScriptRecord_AddJSVarString(file, __L("appname"), appname);
  {
    XSTRING pathstr = pathscript.Get();
    ScriptRecord_AddJSVarString(file, __L("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, __L("windowtitle"), windowtitle);
  if(isbrowser)
    {
      file.AddLine(__L("  // Browser detected: URL cannot be captured - edit this value before running."));
      file.AddLine(__L("  var url         = \"https://EDIT_ME/\";"));
    }
  file.AddLine(__L("  var key         = appname;"));
  file.AddLine(__L(""));
  file.AddLine(__L("  // Already prepared in this script run: only restore focus."));
  file.AddLine(__L("  if(__qa_application_ready[key])"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(__L("      Sleep(200);"));
  file.AddLine(__L("      return true;"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  if(WaitWindow(appname, windowtitle, 1500))"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Console_Printf(\"[EnsureApplication] Already running: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(__L("    }"));
  file.AddLine(__L("   else"));
  file.AddLine(__L("    {"));
  if(isbrowser)
    {
      file.AddLine(__L("      if(!OpenURL(url))"));
      file.AddLine(__L("        {"));
      file.AddLine(__L("          Console_Printf(\"[EnsureApplication] FAIL OpenURL %s\\n\", url);"));
      file.AddLine(__L("          return false;"));
      file.AddLine(__L("        }"));
      file.AddLine(__L(""));
      file.AddLine(__L("      if(!WaitWindow(appname, windowtitle, 30000))"));
      file.AddLine(__L("        {"));
      file.AddLine(__L("          Console_Printf(\"[EnsureApplication] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(__L("          TerminateAplication(appname);"));
      file.AddLine(__L("          return false;"));
      file.AddLine(__L("        }"));
      file.AddLine(__L(""));
      file.AddLine(__L("      Console_Printf(\"[EnsureApplication] Opened URL %s\\n\", url);"));
    }
   else
    {
      file.AddLine(__L("      if(!ExecApplication(apppath))"));
      file.AddLine(__L("        {"));
      file.AddLine(__L("          Console_Printf(\"[EnsureApplication] FAIL ExecApplication %s\\n\", apppath);"));
      file.AddLine(__L("          return false;"));
      file.AddLine(__L("        }"));
      file.AddLine(__L(""));
      file.AddLine(__L("      if(!WaitWindow(appname, windowtitle, 30000))"));
      file.AddLine(__L("        {"));
      file.AddLine(__L("          Console_Printf(\"[EnsureApplication] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(__L("          TerminateAplication(appname);"));
      file.AddLine(__L("          return false;"));
      file.AddLine(__L("        }"));
      file.AddLine(__L(""));
      file.AddLine(__L("      Console_Printf(\"[EnsureApplication] Started %s\\n\", apppath);"));
    }
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(__L("  Sleep(300);"));
  file.AddLine(__L("  __qa_application_ready[key] = true;"));
  file.AddLine(__L("  Console_Printf(\"[EnsureApplication] Ready: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(__L("  return true;"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteCommonHelpers(XFILETXT& file)
* @brief      Write shared WaitWindow / naming / evidence helpers for recorded scripts
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteCommonHelpers(XFILETXT& file)
{
  file.AddLine(__L("var __qa_application_ready = {};"));
  file.AddLine(__L(""));
  file.AddLine(__L("function WaitWindow(appname, windowtitle, timeoutms)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  var outx = { value: 0 };"));
  file.AddLine(__L("  var outy = { value: 0 };"));
  file.AddLine(__L("  var waited = 0;"));
  file.AddLine(__L("  while(waited < timeoutms)"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Sleep(500);"));
  file.AddLine(__L("      waited = waited + 500;"));
  file.AddLine(__L("      if(Screen_GetPosXY(appname, windowtitle, outx, outy) == 0) return true;"));
  file.AddLine(__L("    }"));
  file.AddLine(__L("  return false;"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function ScriptBaseName(scriptname)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  var base = scriptname;"));
  file.AddLine(__L("  var dot  = -1;"));
  file.AddLine(__L("  if(base == \"\" || base == null || base == undefined) base = GetNameScript();"));
  file.AddLine(__L("  dot = base.indexOf(\".\");"));
  file.AddLine(__L("  if(dot >= 0) base = base.substring(0, dot);"));
  file.AddLine(__L("  return base;"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function ListTestFileName(scriptname)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  return ScriptBaseName(scriptname) + \"_ListTest.json\";"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function EvidenceFileName(scriptname)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  return ScriptBaseName(scriptname) + \"_\" + FileCSV_GetShortDateTime() + \".csv\";"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function EvidenceStart(scriptname)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  var dir  = GetPathScript() + \"evidences\";"));
  file.AddLine(__L("  var path = dir + \"\\\\\" + EvidenceFileName(scriptname);"));
  file.AddLine(__L("  MakeDir(dir);"));
  file.AddLine(__L("  if(!FileCSV_Create(path)) return false;"));
  file.AddLine(__L("  if(!FileCSV_SetHeader(\"ID\", \"Description\", \"Result\")) return false;"));
  file.AddLine(__L("  if(!FileCSV_Save()) return false;"));
  file.AddLine(__L("  return true;"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function EvidenceAdd(id, description, result)"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  var idstr = \"\";"));
  file.AddLine(__L("  if(id !== \"\" && id !== null && id !== undefined) idstr = \"\" + (id | 0);"));
  file.AddLine(__L("  return FileCSV_AddRecord(idstr, description, result);"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));
  file.AddLine(__L("function EvidenceEnd()"));
  file.AddLine(__L("{"));
  file.AddLine(__L("  return FileCSV_Close();"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteMain(XFILETXT& file, ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* currentsession)
* @brief      Write main(): EnsureApplication_<app> before each app's tests (once per app)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteMain(XFILETXT& file, ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* currentsession)
{
  XSTRING line;
  XSTRING lastkey;

  file.AddLine(__L("function main()"));
  file.AddLine(__L("{"));

  for(XDWORD c=0; c<scriptrecord_sessions.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = scriptrecord_sessions.Get(c);
      if(!session) continue;

      if(!session->appkey.IsEmpty() && session->appkey.Compare(lastkey) != 0)
        {
          line.Format(__L("  if(!EnsureApplication_%s()) return;"), session->appkey.Get());
          file.AddLine(line);
          lastkey = session->appkey.Get();
        }

      line.Format(__L("  Recorded_%03d();"), (int)session->id);
      file.AddLine(line);
    }

  if(currentsession)
    {
      if(!currentsession->appkey.IsEmpty() && currentsession->appkey.Compare(lastkey) != 0)
        {
          line.Format(__L("  if(!EnsureApplication_%s()) return;"), currentsession->appkey.Get());
          file.AddLine(line);
        }

      line.Format(__L("  Recorded_%03d();"), (int)currentsession->id);
      file.AddLine(line);
    }

  file.AddLine(__L("}"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteScript()
* @brief      Write / refresh the recorded .js under scripts/ (append session if file exists)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteScript()
{
  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  if(!outname || outname->IsEmpty()) return false;

  ScriptRecord_PrepareRecordedLayout();

  XPATH xpath;
  if(!ScriptRecord_ResolveScriptPath(xpath)) return false;

  XPATH apppathscript = scriptrecord_apppath;
  ScriptRecord_PathForScript(apppathscript);

  XSTRING appkey;
  ScriptRecord_MakeAppKey(scriptrecord_appname, appkey);

  ACTIONSCRIPTQA_SCRIPTRECORD_SESSION currentsession;
  currentsession.id          = scriptrecord_sessionindex;
  currentsession.appname     = scriptrecord_appname.Get();
  currentsession.apppath     = apppathscript.Get();
  currentsession.windowtitle = scriptrecord_windowtitle.Get();
  currentsession.appkey      = appkey.Get();

  XFILETXT file;
  if(!file.Create(xpath))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Cannot create %s"), xpath.Get());
      return false;
    }

  XSTRING line;
  bool    needhelpers = true;

  // Full rewrite (no prior kept lines): allow Ensure helpers to be emitted again.
  if(!scriptrecord_existinglines.GetSize())
    {
      scriptrecord_ensurekeyswritten.DeleteContents();
      scriptrecord_ensurekeyswritten.DeleteAll();
      scriptrecord_readymapwritten = false;
    }

  if(scriptrecord_existinglines.GetSize())
    {
      for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
        {
          XSTRING* existing = scriptrecord_existinglines.Get(c);
          if(existing) file.AddLine(*existing);
        }

      needhelpers = !ScriptRecord_ExistingHasHelpers();

      // Ensure a blank separator before helpers / new session function.
      XSTRING* last = scriptrecord_existinglines.Get(scriptrecord_existinglines.GetSize()-1);
      if(!last || !last->IsEmpty()) file.AddLine(__L(""));

      if(needhelpers)
        {
          ScriptRecord_WriteCommonHelpers(file);
          scriptrecord_readymapwritten = true;
        }
       else if(!ScriptRecord_ExistingHasReadyMap() && !scriptrecord_readymapwritten)
        {
          file.AddLine(__L("var __qa_application_ready = {};"));
          file.AddLine(__L(""));
          scriptrecord_readymapwritten = true;
        }
    }
   else
    {
      file.AddLine(__L("// ----------------------------------------------------------------------------"));
      line.Format(__L("// %s - auto-generated by ActionScriptQA script recorder (F1)"), outname->Get());
      file.AddLine(line);
      file.AddLine(__L("// Evidence CSV: evidences/<GetNameScript()>_<datetime>.csv"));
      file.AddLine(__L("// Optional ListTest: <GetNameScript()>_ListTest.json (loaded if present)."));
      file.AddLine(__L("// Layout: assets/Tests/<scriptbase>/{graphics,evidences}/ + <scriptbase>.js"));
      file.AddLine(__L("// App launch: EnsureApplication_<app>() in main() before that app's tests."));
      file.AddLine(__L("// New recording sessions append function Recorded_NNN() and refresh the entry point."));
      file.AddLine(__L("// ----------------------------------------------------------------------------"));
      file.AddLine(__L(""));
      ScriptRecord_WriteCommonHelpers(file);
      scriptrecord_readymapwritten = true;
    }

  // EnsureApplication_<key> for every known app that does not already have one.
  ScriptRecord_Print(__L("[ScriptRecord] WriteScript: ensuring application helpers..."));
  for(XDWORD c=0; c<scriptrecord_sessions.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = scriptrecord_sessions.Get(c);
      if(!session || session->appkey.IsEmpty()) continue;
      if(ScriptRecord_ExistingHasEnsureAppKey(session->appkey)) continue;

      // Prefer stable browser titles when rewriting Ensure from older sessions.
      if(ScriptRecord_IsBrowserApp(session->appname, session->apppath))
        {
          ScriptRecord_BrowserWindowTitle(session->appname, session->apppath, session->windowtitle);
        }

      ScriptRecord_WriteEnsureApplicationForApp(file, session->appname, session->apppath, session->windowtitle, session->appkey);

      XSTRING* keycopy = GEN_NEW XSTRING();
      if(keycopy)
        {
          (*keycopy) = session->appkey.Get();
          scriptrecord_ensurekeyswritten.Add(keycopy);
        }
    }

  if(!appkey.IsEmpty() && !ScriptRecord_ExistingHasEnsureAppKey(appkey))
    {
      ScriptRecord_WriteEnsureApplicationForApp(file, scriptrecord_appname, apppathscript, scriptrecord_windowtitle, appkey);

      XSTRING* keycopy = GEN_NEW XSTRING();
      if(keycopy)
        {
          (*keycopy) = appkey.Get();
          scriptrecord_ensurekeyswritten.Add(keycopy);
        }
    }

  ScriptRecord_Print(__L("[ScriptRecord] WriteScript: writing Recorded_%03d (%d steps)..."), (int)scriptrecord_sessionindex, (int)scriptrecord_steps.GetSize());
  line.Format(__L("function Recorded_%03d()"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  file.AddLine(__L("{"));

  file.AddLine(__L("  var scriptname  = GetNameScript();"));
  line.Format(__L("  var session     = \"Recorded_%03d\";"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  ScriptRecord_AddJSVarString(file, __L("appname"), scriptrecord_appname);
  {
    XSTRING pathstr = apppathscript.Get();
    ScriptRecord_AddJSVarString(file, __L("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, __L("windowtitle"), scriptrecord_windowtitle);

  file.AddLine(__L("  var outx = { value: 0 };"));
  file.AddLine(__L("  var outy = { value: 0 };"));
  file.AddLine(__L("  var i = 0;"));
  file.AddLine(__L("  var status = 1;"));
  file.AddLine(__L("  var stepresult = \"PASS\";"));
  file.AddLine(__L("  var stepdesc = \"\";"));
  file.AddLine(__L("  var catalogpath = \"\";"));
  file.AddLine(__L("  var listloaded = false;"));
  file.AddLine(__L(""));
  file.AddLine(__L("  Console_Printf(\"[%s] Start %s\\n\", scriptname, session);"));
  file.AddLine(__L(""));
  file.AddLine(__L("  if(!EvidenceStart(scriptname))"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Console_Printf(\"[%s] FAIL EvidenceStart/FileCSV\\n\", scriptname);"));
  file.AddLine(__L("      return;"));
  file.AddLine(__L("    }"));
  file.AddLine(__L("  Console_Printf(\"[%s] Evidence CSV %s\\n\", scriptname, FileCSV_GetPath());"));
  file.AddLine(__L(""));
  file.AddLine(__L("  catalogpath = GetPathScript() + ListTestFileName(scriptname);"));
  file.AddLine(__L("  if(IsItExists(catalogpath))"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      if(TraceTests_Load(catalogpath))"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          listloaded = true;"));
  file.AddLine(__L("          EvidenceAdd(\"\", \"TraceTests_Load\", \"PASS\");"));
  file.AddLine(__L("          Console_Printf(\"[%s] ListTest loaded %s\\n\", scriptname, catalogpath);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("       else"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          EvidenceAdd(\"\", \"TraceTests_Load\", \"FAIL\");"));
  file.AddLine(__L("          Console_Printf(\"[%s] FAIL TraceTests_Load %s\\n\", scriptname, catalogpath);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("    }"));
  file.AddLine(__L("   else"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      EvidenceAdd(\"\", \"TraceTests_Load\", \"SKIP\");"));
  file.AddLine(__L("      Console_Printf(\"[%s] ListTest not found (optional) %s\\n\", scriptname, catalogpath);"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  // Application is started/focused from main() via EnsureApplication_<app>() before this test."));
  file.AddLine(__L("  Screen_SetBmpFindCFG(12, 40);"));
  file.AddLine(__L("  Sleep(300);"));
  file.AddLine(__L(""));

  file.AddLine(__L("  var actions = ["));
  for(XDWORD c=0; c<scriptrecord_steps.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = scriptrecord_steps.Get(c);
      if(!step) continue;

      XSTRING escaped;
      XCHAR*  comma = (c + 1 < scriptrecord_steps.GetSize()) ? __L(",") : __L("");

      switch(step->type)
        {
          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(__L("    { type: \"text\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(__L("    { type: \"key\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT :
                line.Format(__L("    { type: \"wait\", bmp: \"%s\", timeoutms: %d, intervalms: %d }%s"),
                            step->bitmapname.Get(),
                            APPFLOW_CFG.ScriptRecord_GetWaitTimeoutMs(),
                            APPFLOW_CFG.ScriptRecord_GetWaitIntervalMs(),
                            comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK :
          default :
                line.Format(__L("    { type: \"click\", bmp: \"%s\", x: %d, y: %d }%s"),
                            step->bitmapname.Get(), step->layoutx, step->layouty, comma);
                break;
        }

      file.AddLine(line);
    }
  file.AddLine(__L("  ];"));
  file.AddLine(__L(""));
  file.AddLine(__L("  for(i = 0; i < actions.length; i++)"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(__L("      Sleep(80);"));
  file.AddLine(__L("      stepresult = \"PASS\";"));
  file.AddLine(__L("      stepdesc = actions[i].type;"));
  file.AddLine(__L(""));
  file.AddLine(__L("      if(actions[i].type == \"click\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          outx = { value: 0 };"));
  file.AddLine(__L("          outy = { value: 0 };"));
  file.AddLine(__L("          stepdesc = \"click \" + actions[i].bmp;"));
  file.AddLine(__L("          status = Screen_GetPosXY(appname, windowtitle, actions[i].bmp, outx, outy);"));
  file.AddLine(__L("          if(status != 0)"));
  file.AddLine(__L("            {"));
  file.AddLine(__L("              var winx = { value: 0 };"));
  file.AddLine(__L("              var winy = { value: 0 };"));
  file.AddLine(__L("              if(Screen_GetPosXY(appname, windowtitle, winx, winy) == 0)"));
  file.AddLine(__L("                {"));
  file.AddLine(__L("                  outx.value = winx.value + actions[i].x;"));
  file.AddLine(__L("                  outy.value = winy.value + actions[i].y;"));
  file.AddLine(__L("                  status = 0;"));
  file.AddLine(__L("                }"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("          if(status == 0)"));
  file.AddLine(__L("            {"));
  file.AddLine(__L("              InpSim_Mouse_Click(outx.value, outy.value);"));
  file.AddLine(__L("              Console_Printf(\"[%s] Click %s at %d,%d\\n\", scriptname, actions[i].bmp, outx.value, outy.value);"));
  file.AddLine(__L("              Sleep(400);"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("           else"));
  file.AddLine(__L("            {"));
  file.AddLine(__L("              stepresult = \"FAIL\";"));
  file.AddLine(__L("              Console_Printf(\"[%s] FAIL find %s\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("      else if(actions[i].type == \"wait\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          stepdesc = \"wait \" + actions[i].bmp;"));
  file.AddLine(__L("          if(Screen_WaitBitmap(appname, windowtitle, actions[i].bmp, actions[i].timeoutms, actions[i].intervalms))"));
  file.AddLine(__L("            {"));
  file.AddLine(__L("              Console_Printf(\"[%s] PASS wait %s\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("           else"));
  file.AddLine(__L("            {"));
  file.AddLine(__L("              stepresult = \"FAIL\";"));
  file.AddLine(__L("              Console_Printf(\"[%s] FAIL wait %s (timeout)\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("      else if(actions[i].type == \"text\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          stepdesc = \"text\";"));
  file.AddLine(__L("          InpSim_Key_ClickByText(actions[i].value, 40);"));
  file.AddLine(__L("          Console_Printf(\"[%s] Type text (%d chars)\\n\", scriptname, actions[i].value.length);"));
  file.AddLine(__L("          Sleep(200);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("      else if(actions[i].type == \"key\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          stepdesc = \"key \" + actions[i].value;"));
  file.AddLine(__L("          InpSim_Key_ClickByLiteral(actions[i].value, 40);"));
  file.AddLine(__L("          Console_Printf(\"[%s] Key %s\\n\", scriptname, actions[i].value);"));
  file.AddLine(__L("          Sleep(200);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L(""));
  file.AddLine(__L("      EvidenceAdd(i + 1, stepdesc, stepresult);"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  if(listloaded) TraceTests_DeleteAll();"));
  file.AddLine(__L("  EvidenceEnd();"));
  file.AddLine(__L("  Console_Printf(\"[%s] End %s\\n\", scriptname, session);"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));

  ScriptRecord_WriteMain(file, &currentsession);

  ScriptRecord_Print(__L("[ScriptRecord] WriteScript: flushing file to disk..."));
  if(!file.WriteAllFile())
    {
      file.Close();
      ScriptRecord_Print(__L("[ScriptRecord] WriteAllFile failed for %s"), xpath.Get());
      return false;
    }

  file.Close();
  ScriptRecord_Print(__L("[ScriptRecord] WriteScript: done."));
  return true;
}

#pragma endregion
