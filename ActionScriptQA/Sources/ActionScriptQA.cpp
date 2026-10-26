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

#include "INPManager.h"
#include "INPFactory.h"
#include "INPDevice.h"
#include "INPButton.h"

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void ActionScriptQA_FreeRunningList(XVECTOR<XPROCESS*>& applist)
* @brief      Release process entries returned by Application_GetRunningList.
*
*---------------------------------------------------------------------------------------------------------------------*/
static void ActionScriptQA_FreeRunningList(XVECTOR<XPROCESS*>& applist)
{
  applist.DeleteContents();
  applist.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static XPROCESS* ActionScriptQA_FindRunningByHandle(XVECTOR<XPROCESS*>& applist, void* hwnd)
* @brief      Locate a running process entry by native window handle.
*
*---------------------------------------------------------------------------------------------------------------------*/
static XPROCESS* ActionScriptQA_FindRunningByHandle(XVECTOR<XPROCESS*>& applist, void* hwnd)
{
  if(!hwnd) return NULL;

  for(XDWORD c=0; c<applist.GetSize(); c++)
    {
      XPROCESS* process = applist.Get(c);
      if(process && (process->GetWindowHandle() == hwnd)) return process;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static XPROCESS* ActionScriptQA_FindRunningByNameTitle(XVECTOR<XPROCESS*>& applist, XSTRING& appname, XSTRING& windowtitle)
* @brief      Locate a running process by executable name and optional window title.
*
*---------------------------------------------------------------------------------------------------------------------*/
static XPROCESS* ActionScriptQA_FindRunningByNameTitle(XVECTOR<XPROCESS*>& applist, XSTRING& appname, XSTRING& windowtitle)
{
  if(appname.IsEmpty()) return NULL;

  for(XDWORD c=0; c<applist.GetSize(); c++)
    {
      XPROCESS* process = applist.Get(c);
      if(!process || !process->GetWindowHandle()) continue;
      if(!process->GetName() || process->GetName()->Compare(appname, true) != 0) continue;

      if(windowtitle.IsEmpty()) return process;
      if(process->GetWindowTitle() && (process->GetWindowTitle()->Compare(windowtitle, true) == 0)) return process;
      if(process->GetWindowTitle() && (process->GetWindowTitle()->Find(windowtitle.Get(), true) != XSTRING_NOTFOUND)) return process;
    }

  return NULL;
}


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

  // Mouse/keyboard for ScriptRecord (console app has no APPFlowGraphics screen owner).
  ScriptRecord_EnsureInputDevices();

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
          if((!autorun.Compare(_L("1"), true)) ||
             (!autorun.Compare(_L("si"), true)) ||
             (!autorun.Compare(_L("yes"), true)) ||
             (!autorun.Compare(_L("true"), true)))
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

  {
    INPDEVICE* mouse = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
    if(mouse) GEN_INPMANAGER.DelDevice(mouse);

    INPDEVICE* keyboard = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_KEYBOARD);
    if(keyboard) GEN_INPMANAGER.DelDevice(keyboard);
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

  if((character<32) || (character>127)) character = _C('?');
  APPFLOW_LOG_ENTRY(XLOGLEVEL_WARNING, APPFLOW_CFG_LOG_SECTIONID_STATUSAPP, false, _L("Key pressed: 0x%02X [%c]"), key, character);

  if(console) console->Printf(_L("\r    \r"));

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
      console->Printf(_L("Total working time %s"), string.Get());                                                      
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

  XPATH entry(scriptentry);
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
  graphics += _L("graphics");
  if(!xdir->Exist(graphics)) xdir->Make(graphics, true);

  XPATH evidences = testroot;
  evidences.Slash_Add();
  evidences += _L("evidences");
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
  section.Format(_L("%s/%s/graphics"), APPFLOW_DEFAULT_DIRECTORY_TESTS, basename.Get());
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
  XPATH entrypath(scriptentry);
  XSTRING nameext;
  entrypath.GetNamefileExt(nameext);
  if(nameext.IsEmpty())
    {
      nameext = basename.Get();
      nameext += _L(".js");
    }
  xpath += nameext.Get();

  SCRIPT* script = SCRIPT::Create(nameext.Get());
  if(!script)
    {
      ScriptRecord_Print(_L("[Tests] Cannot create script engine for %s"), nameext.Get());
      Test_RestoreDefaultGraphics();
      return false;
    }

  AdjustLibraries(script);

  if(!script->Load(xpath))
    {
      ScriptRecord_Print(_L("[Tests] Cannot load %s"), xpath.Get());
      GEN_DELETE script;
      Test_RestoreDefaultGraphics();
      return false;
    }

  ScriptRecord_Print(_L("[Tests] Running %s"), xpath.Get());
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

  string  = _L("Memoria total");
  string2.Format(_L("%d Kb, libre %d Kb (el %d%%%%)"), total, free, GEN_XSYSTEM.GetFreeMemoryPercent());
  Show_Line(string, string2);

  XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
  if(datetime)
    {
      datetime->Read();

      string  = _L("Fecha ");
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

  string  = _L("Script Status");
  string2.Format(_L("%s"), script->GetNameScript()->Get());

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
      case SCRIPT_XEVENT_TYPE_ERROR    : XTRACE_PRINTCOLOR(4,_L("Script ERROR [%d]: %s line %d -> \"%s\""), event->GetError(), event->GetErrorText()->Get(), event->GetNLine(), event->GetCurrentToken()->Get());
                                         break;

      case SCRIPT_XEVENT_TYPE_BREAK    : XTRACE_PRINTCOLOR(4,_L("Script BREAK: line %d -> \"%s\""), event->GetNLine(), event->GetCurrentToken()->Get());
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
  memset(scriptrecord_btnwasdown, 0, sizeof(scriptrecord_btnwasdown));
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
      basename = _L("Tests_Recorded");
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
      XPATH entry(*outname);
      XSTRING nameext;
      entry.GetNamefileExt(nameext);
      if(nameext.IsEmpty())
        {
          nameext = basename.Get();
          nameext += _L(".js");
        }
      xpath += nameext.Get();
    }
   else
    {
      xpath += _L("Tests_Recorded.js");
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

  outstring.Add(_L("\n"));
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
  memset(scriptrecord_btnwasdown, 0, sizeof(scriptrecord_btnwasdown));

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
          ScriptRecord_Print(_L("[ScriptRecord] Finishing session: flushing text..."));
          ScriptRecord_FlushText(false);
          ScriptRecord_Print(_L("[ScriptRecord] Finishing session: writing script..."));
          if(ScriptRecord_WriteScript())
            {
              ScriptRecord_Print(_L("[ScriptRecord] Saved script and left record mode."));
            }
           else
            {
              ScriptRecord_Print(_L("[ScriptRecord] ERROR writing script; left record mode anyway."));
            }
        }
       else
        {
          ScriptRecord_Print(_L("[ScriptRecord] Left record mode (no app selected)."));
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
  ScriptRecord_Print(_L("[ScriptRecord] ON (F1/ESC finish, F5 discard+delete Tests_Recorded)."));
  ScriptRecord_Print(_L("[ScriptRecord] 1) Left-click a window of the app under test."));
  ScriptRecord_Print(_L("[ScriptRecord] 2) Left-click UI targets (mouse click steps)."));
  ScriptRecord_Print(_L("[ScriptRecord] 3) Right-click a zone to wait/assert it appears on screen."));
  ScriptRecord_Print(_L("[ScriptRecord] 4) Type text (login/password); ENTER/TAB/BACKSPACE recorded as keys."));
  ScriptRecord_Print(_L("[ScriptRecord] Output: Tests/%s/%s"), basename.Get(), outname ? outname->Get() : _L("Tests_Recorded.js"));
  ScriptRecord_Print(_L("[ScriptRecord] Session function: Recorded_%03d  (next bitmap %03d)"), (int)scriptrecord_sessionindex, (int)scriptrecord_nextbitmapindex);

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
      ScriptRecord_Print(_L("[ScriptRecord] Record mode cancelled (F5)."));
    }

  XSTRING basename;
  if(!ScriptRecord_GetRecordedBaseName(basename))
    {
      ScriptRecord_Print(_L("[ScriptRecord] ERROR resolving recorded test name."));
      return false;
    }

  XPATH testroot;
  if(!Test_GetRootPath(basename, testroot))
    {
      ScriptRecord_Print(_L("[ScriptRecord] ERROR resolving Tests/%s path."), basename.Get());
      return false;
    }

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir)
    {
      ScriptRecord_Print(_L("[ScriptRecord] ERROR creating XDIR."));
      return false;
    }

  bool status = true;
  if(xdir->Exist(testroot))
    {
      status = xdir->Delete(testroot, true);
      if(status)
        {
          ScriptRecord_Print(_L("[ScriptRecord] Deleted Tests/%s (directory and files)."), basename.Get());
        }
       else
        {
          ScriptRecord_Print(_L("[ScriptRecord] ERROR deleting Tests/%s."), basename.Get());
        }
    }
   else
    {
      ScriptRecord_Print(_L("[ScriptRecord] Nothing to delete: Tests/%s (missing)."), basename.Get());
    }

  GEN_XFACTORY.Delete_Dir(xdir);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_EnsureInputDevices()
* @brief      Create mouse/keyboard INP devices for console recording (no GRPSCREEN owner).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_EnsureInputDevices()
{
  if(!GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE))
    {
      INPDEVICE* mouse = INPFACTORY::GetInstance().CreateDevice(INPDEVICE_TYPE_MOUSE, NULL);
      if(mouse) GEN_INPMANAGER.AddDevice(mouse);
    }

  if(!GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_KEYBOARD))
    {
      INPDEVICE* keyboard = INPFACTORY::GetInstance().CreateDevice(INPDEVICE_TYPE_KEYBOARD, NULL);
      if(keyboard) GEN_INPMANAGER.AddDevice(keyboard);
    }

  return (GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE) != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_Update()
* @brief      ScriptRecord_Update — poll mouse via INPDEVICE; cursor via GRPSCREEN (GEN).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_Update()
{
  if(!ScriptRecord_EnsureInputDevices()) return false;

  INPDEVICE* mouse = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(!mouse) return false;

  bool ldown = false;
  bool rdown = false;
  INPBUTTON* lbtn = mouse->GetButton(INPBUTTON_ID_MOUSE_LEFT);
  INPBUTTON* rbtn = mouse->GetButton(INPBUTTON_ID_MOUSE_RIGHT);
  if(lbtn) ldown = lbtn->IsPressed();
  if(rbtn) rdown = rbtn->IsPressed();

  int  cursorx = 0;
  int  cursory = 0;
  bool havecursor = false;

  {
    GRPSCREEN* cursorscreen = GEN_GRPFACTORY.CreateScreen();
    if(cursorscreen)
      {
        havecursor = cursorscreen->GetCursorDesktopPosition(cursorx, cursory);
        GEN_GRPFACTORY.DeleteScreen(cursorscreen);
      }
  }

  if(ldown && !scriptrecord_mousewasdown && havecursor)
    {
      if(!scriptrecord_appselected)
        {
          ScriptRecord_SelectApp(cursorx, cursory);
        }
       else
        {
          ScriptRecord_CaptureClick(cursorx, cursory, ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK);
        }
    }

  if(rdown && !scriptrecord_rbuttonwasdown && scriptrecord_appselected && havecursor)
    {
      ScriptRecord_CaptureClick(cursorx, cursory, ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT);
    }

  scriptrecord_mousewasdown   = ldown;
  scriptrecord_rbuttonwasdown = rdown;

  if(scriptrecord_appselected)
    {
      ScriptRecord_UpdateKeys();
    }

  return true;
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
          case _C('\\') : target.Add(_L("\\\\")); break;
          case _C('\"') : target.Add(_L("\\\"")); break;
          case _C('\n') : target.Add(_L("\\n"));  break;
          case _C('\r') : target.Add(_L("\\r"));  break;
          case _C('\t') : target.Add(_L("\\t"));  break;
          // '%' must not reach XSTRING::Format / Printf masks (FormatArg can hang on "%...").
          case _C('%')  : target.Add(_L("_"));    break;
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

  ScriptRecord_Print(_L("[ScriptRecord] Step %d: text \"%s\" (%d chars)"), (int)scriptrecord_steps.GetSize(), scriptrecord_textpending.Get(), (int)scriptrecord_textpending.GetSize());
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

  ScriptRecord_Print(_L("[ScriptRecord] Step %d: key %s"), (int)scriptrecord_steps.GetSize(), literal);
  ScriptRecord_WriteScript();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_UpdateKeys()
* @brief      Poll keyboard edges via INPDEVICE (specials + layout Unicode text).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_UpdateKeys()
{
  if(!scriptrecord_appselected) return false;
  if(!ScriptRecord_EnsureInputDevices()) return false;

  INPDEVICE* keyboard = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_KEYBOARD);
  if(!keyboard) return false;

  bool shift = false;
  INPBUTTON* shiftr = keyboard->GetButton(INPBUTTON_ID_SHIFT_RIGHT);
  INPBUTTON* shiftl = keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT);
  if(shiftr && shiftr->IsPressed()) shift = true;
  if(shiftl && shiftl->IsPressed()) shift = true;

  for(int id = (int)INPBUTTON_ID_NOBUTTON + 1; id < (int)INPBUTTON_ID_LAST; id++)
    {
      // Skip mouse / console recorder controls / pure modifiers.
      if((id == (int)INPBUTTON_ID_MOUSE_LEFT)   ||
         (id == (int)INPBUTTON_ID_MOUSE_RIGHT)  ||
         (id == (int)INPBUTTON_ID_MOUSE_MIDDLE) ||
         (id == (int)INPBUTTON_ID_SHIFT_LEFT)   ||
         (id == (int)INPBUTTON_ID_SHIFT_RIGHT)  ||
         (id == (int)INPBUTTON_ID_CONTROL_LEFT) ||
         (id == (int)INPBUTTON_ID_CONTROL_RIGHT)||
         (id == (int)INPBUTTON_ID_ALT_LEFT)     ||
         (id == (int)INPBUTTON_ID_ALT_RIGHT)    ||
         (id == (int)INPBUTTON_ID_LWIN)         ||
         (id == (int)INPBUTTON_ID_RWIN)         ||
         (id == (int)INPBUTTON_ID_CAPS_LOCK)    ||
         (id == (int)INPBUTTON_ID_NUMLOCK)      ||
         (id == (int)INPBUTTON_ID_SCROLL_LOCK)  ||
         (id == (int)INPBUTTON_ID_ESCAPE)       ||
         ((id >= (int)INPBUTTON_ID_F1) && (id <= (int)INPBUTTON_ID_F12)))
        {
          INPBUTTON* skipbtn = keyboard->GetButton((INPBUTTON_ID)id);
          scriptrecord_btnwasdown[id] = (skipbtn && skipbtn->IsPressed());
          continue;
        }

      INPBUTTON* button = keyboard->GetButton((INPBUTTON_ID)id);
      if(!button) continue;

      bool down = button->IsPressed();
      bool was  = scriptrecord_btnwasdown[id];
      scriptrecord_btnwasdown[id] = down;

      if(!(down && !was)) continue;

      XCHAR* literal = NULL;
      switch(id)
        {
          case INPBUTTON_ID_RETURN    :
          case INPBUTTON_ID_INTRO     : literal = _L("ENTER");       break;
          case INPBUTTON_ID_TAB       : literal = _L("TAB");         break;
          case INPBUTTON_ID_BACK_SPACE: literal = _L("BACKSPACE");   break;
          case INPBUTTON_ID_DELETE    : literal = _L("DEL");         break;
          case INPBUTTON_ID_LEFT      : literal = _L("LEFT ARROW");  break;
          case INPBUTTON_ID_RIGHT     : literal = _L("RIGHT ARROW"); break;
          case INPBUTTON_ID_UP        : literal = _L("UP ARROW");    break;
          case INPBUTTON_ID_DOWN      : literal = _L("DOWN ARROW");  break;
          case INPBUTTON_ID_HOME      : literal = _L("HOME");        break;
          case INPBUTTON_ID_END       : literal = _L("END");         break;
          case INPBUTTON_ID_PAGE_UP   : literal = _L("PAGE UP");     break;
          case INPBUTTON_ID_PAGE_DOWN : literal = _L("PAGE DOWN");   break;
          case INPBUTTON_ID_INSERT    : literal = _L("INS");         break;
                              default : break;
        }

      if(literal)
        {
          ScriptRecord_AddKeyLiteral(literal);
          continue;
        }

      XSTRING translated;
      if(keyboard->TranslateButtonToUnicode(button, translated) && !translated.IsEmpty())
        {
          // Skip pure control codes from layout translation (keep printable + tab).
          bool hasprintable = false;
          for(XDWORD c=0; c<translated.GetSize(); c++)
            {
              XCHAR ch = translated.Get()[c];
              if((ch >= 32) || (ch == 9))
                {
                  hasprintable = true;
                  break;
                }
            }
          if(!hasprintable) continue;

          scriptrecord_textpending.Add(translated);
        }
       else
        {
          // Fallback: static CreateAllButtons symbol + Shift case-fold.
          XCHAR ch = button->GetSymbol();
          if(!ch) continue;
          if((ch >= _C('A')) && (ch <= _C('Z')) && !shift) ch = (XCHAR)(ch - _C('A') + _C('a'));
          if((ch < 32) && (ch != 9)) continue;
          scriptrecord_textpending.Add(ch);
        }

      if(!scriptrecord_textpending.IsEmpty())
        {
          // Keep buffering; flush on special key, click, or end of recording.
          ScriptRecord_Print(_L("[ScriptRecord] Typing... (%d chars)"), (int)scriptrecord_textpending.GetSize());
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_SelectApp(int screenx, int screeny)
* @brief      Identify target application from window under cursor (GEN FindWindowAtPoint).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_SelectApp(int screenx, int screeny)
{
  XPROCESS process;

  if(!GEN_XPROCESSMANAGER.Application_FindWindowAtPoint(screenx, screeny, process))
    {
      ScriptRecord_Print(_L("[ScriptRecord] No window under cursor."));
      return false;
    }

  if(!process.GetWindowHandle())
    {
      ScriptRecord_Print(_L("[ScriptRecord] No window under cursor."));
      return false;
    }

  if(!process.GetName() || process.GetName()->IsEmpty())
    {
      ScriptRecord_Print(_L("[ScriptRecord] Cannot resolve executable name for window."));
      return false;
    }

  if(process.GetName()->Find(APPLICATION_NAMEFILE, true) != XSTRING_NOTFOUND)
    {
      ScriptRecord_Print(_L("[ScriptRecord] Ignored click on ActionScriptQA itself."));
      return false;
    }

  scriptrecord_windowhandle = process.GetWindowHandle();
  scriptrecord_appname      = process.GetName()->Get();
  if(process.GetPath() && !process.GetPath()->IsEmpty())
    {
      scriptrecord_apppath = process.GetPath()->Get();
    }
   else
    {
      scriptrecord_apppath = scriptrecord_appname;
    }
  ScriptRecord_PathForScript(scriptrecord_apppath);

  // Browsers: use a stable short title for Screen_* Find() (dynamic tab titles break matching).
  if(ScriptRecord_IsBrowserApp(scriptrecord_appname, scriptrecord_apppath))
    {
      ScriptRecord_BrowserWindowTitle(scriptrecord_appname, scriptrecord_apppath, scriptrecord_windowtitle);
    }
   else if(process.GetWindowTitle())
    {
      scriptrecord_windowtitle = process.GetWindowTitle()->Get();
      scriptrecord_windowtitle.Character_Change(_C('%'), _C('_'));
    }
   else
    {
      scriptrecord_windowtitle.Empty();
    }

  scriptrecord_appselected = true;

  ScriptRecord_Print(_L("[ScriptRecord] App selected: %s"), scriptrecord_appname.Get());
  ScriptRecord_Print(_L("[ScriptRecord] Path: %s"), scriptrecord_apppath.Get());
  ScriptRecord_Print(_L("[ScriptRecord] Window: %s"), scriptrecord_windowtitle.Get());
  if(ScriptRecord_IsBrowserApp(scriptrecord_appname, scriptrecord_apppath))
    {
      ScriptRecord_Print(_L("[ScriptRecord] Browser detected: Ensure will use OpenURL (edit url in script)."));
    }
  ScriptRecord_Print(_L("[ScriptRecord] Click UI controls to capture bitmaps..."));

  ScriptRecord_WriteScript();

  return true;
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
  if(!scriptrecord_appselected) return false;
  if((steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK) && (steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT)) return false;

  ScriptRecord_FlushText();

  XVECTOR<XPROCESS*> applist;
  XPROCESS*          process = NULL;

  if(!GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      ScriptRecord_Print(_L("[ScriptRecord] Application_GetRunningList failed."));
      return false;
    }

  process = ActionScriptQA_FindRunningByHandle(applist, scriptrecord_windowhandle);
  if(!process)
    {
      process = ActionScriptQA_FindRunningByNameTitle(applist, scriptrecord_appname, scriptrecord_windowtitle);
      if(process && process->GetWindowHandle())
        {
          scriptrecord_windowhandle = process->GetWindowHandle();
          ScriptRecord_Print(_L("[ScriptRecord] Target window handle refreshed."));
        }
    }

  if(!process || !process->GetWindowHandle() || !process->GetWindowRect())
    {
      ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(_L("[ScriptRecord] Target window no longer valid."));
      return false;
    }

  void*      hwnd    = process->GetWindowHandle();
  GRPRECTINT winrect = (*process->GetWindowRect());
  int        border  = process->GetWindowBorderWidth();
  int        titleh  = process->GetWindowTitleHeight();

  GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
  if(!screen)
    {
      ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(_L("[ScriptRecord] CreateScreen failed."));
      return false;
    }

  screen->SetHandle(hwnd);

  int clientw = 0;
  int clienth = 0;
  if(!screen->GetClientSize(clientw, clienth) || (clientw <= 0) || (clienth <= 0))
    {
      // Fallback from outer rect minus chrome metrics already on XPROCESS.
      clientw = (int)winrect.GetWidth()  - (border * 2);
      clienth = (int)winrect.GetHeight() - titleh - border;
    }

  // CaptureContent uses client-area coordinates (same convention as Script_Lib_Screen).
  int ptclientx = screenx - (winrect.x1 + border);
  int ptclienty = screeny - (winrect.y1 + titleh);

  if((clientw <= 0) || (clienth <= 0) ||
     (ptclientx < 0) || (ptclienty < 0) ||
     (ptclientx >= clientw) || (ptclienty >= clienth))
    {
      GEN_GRPFACTORY.DeleteScreen(screen);
      ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(_L("[ScriptRecord] Click outside client area (ignored)."));
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
  rect.x1 = ptclientx - halfw;
  rect.y1 = ptclienty - halfh;

  if(rect.x1 < 0) rect.x1 = 0;
  if(rect.y1 < 0) rect.y1 = 0;
  if((rect.x1 + captw) > clientw) rect.x1 = clientw - captw;
  if((rect.y1 + capth) > clienth) rect.y1 = clienth - capth;

  rect.x2 = rect.x1 + captw;
  rect.y2 = rect.y1 + capth;

  // Layout fallback uses outer window origin (Screen_GetPosXY without bitmap).
  int layoutx = screenx - winrect.x1;
  int layouty = screeny - winrect.y1;

  screen->SetWidth(clientw);
  screen->SetHeight(clienth);

  GRPBITMAP* bitmap = screen->CaptureContent(&rect, hwnd);
  GEN_GRPFACTORY.DeleteScreen(screen);
  ActionScriptQA_FreeRunningList(applist);

  if(!bitmap)
    {
      ScriptRecord_Print(_L("[ScriptRecord] CaptureContent failed."));
      return false;
    }

  XSTRING* prefix = APPFLOW_CFG.ScriptRecord_GetBitmapPrefix();
  XSTRING  bmpname;
  bmpname.Format(_L("%s%03d.png"), prefix ? prefix->Get() : _L("rec_"), (int)scriptrecord_nextbitmapindex);

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
      ScriptRecord_Print(_L("[ScriptRecord] Failed to save %s (PNG support not compiled)."), xpathbmp.Get());
      #else
      ScriptRecord_Print(_L("[ScriptRecord] Failed to save %s"), xpathbmp.Get());
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
      ScriptRecord_Print(_L("[ScriptRecord] Step %d: WAIT %s capt %dx%d (assert appears)"),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth);
    }
   else
    {
      ScriptRecord_Print(_L("[ScriptRecord] Step %d: CLICK %s capt %dx%d at client %d,%d layout %d,%d"),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth, ptclientx, ptclienty, layoutx, layouty);
    }

  ScriptRecord_WriteScript();

  return true;
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
  XCHAR*   pref   = prefix && !prefix->IsEmpty() ? prefix->Get() : _L("rec_");
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

              int posrec = line->Find(_L("function Recorded_"), false);
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
                              if(look->Find(_L("function "), false) != XSTRING_NOTFOUND) break;

                              XSTRING value;
                              if(ScriptRecord_ParseQuotedAssign(look, _L("appname"), value))
                                {
                                  session->appname = value.Get();
                                  ScriptRecord_MakeAppKey(session->appname, session->appkey);
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, _L("apppath"), value))
                                {
                                  session->apppath = value.Get();
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, _L("windowtitle"), value))
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
                  if(line->Find(_L("function main("), false) != XSTRING_NOTFOUND)
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
                  (*renamed) = _L("function Recorded_001()");
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
              ScriptRecord_Print(_L("[ScriptRecord] Existing main() migrated to Recorded_001."));
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
              ScriptRecord_Print(_L("[ScriptRecord] Appending to existing script (%d kept lines, %d sessions)."), (int)scriptrecord_existinglines.GetSize(), (int)scriptrecord_sessions.GetSize());
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
      pattern.Format(_L("%s*.png"), pref);

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
      if(line->Find(_L("function EvidenceStart"), false) != XSTRING_NOTFOUND) return true;
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
      if(line->Find(_L("__qa_application_ready"), false) != XSTRING_NOTFOUND) return true;
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
  needle.Format(_L("function EnsureApplication_%s"), appkey.Get());

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
  pattern.Format(_L("var %s"), varname);
  int pos = line->Find(pattern.Get(), false);
  if(pos == XSTRING_NOTFOUND) return false;

  int q1 = line->FindCharacter(_C('\"'), (XDWORD)pos);
  if(q1 < 0) return false;
  int q2 = line->FindCharacter(_C('\"'), (XDWORD)(q1 + 1));
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
       else if(appkey.GetSize() && appkey.Character_GetLast() != _C('_'))
        {
          appkey.Add(_C('_'));
        }
    }

  while(appkey.GetSize() && appkey.Character_GetLast() == _C('_'))
    {
      appkey.DeleteLastCharacter();
    }

  if(appkey.IsEmpty()) appkey = _L("app");
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
  if(probe.Find(_L("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Compare(_L("edge.exe"), false) == 0) return true;
  if(probe.Compare(_L("edge"), false) == 0) return true;

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(_L("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("\\edge\\"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(_L("/edge/"), false) != XSTRING_NOTFOUND) return true;

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

  if(probe.Find(_L("msedge"), false) != XSTRING_NOTFOUND || probe.Compare(_L("edge.exe"), false) == 0)
    {
      windowtitle = _L("Microsoft Edge");
      return;
    }

  if(probe.Find(_L("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = _L("Mozilla Firefox");
      return;
    }

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(_L("msedge"), false) != XSTRING_NOTFOUND || probe.Find(_L("\\edge\\"), false) != XSTRING_NOTFOUND || probe.Find(_L("/edge/"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = _L("Microsoft Edge");
      return;
    }

  if(probe.Find(_L("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = _L("Mozilla Firefox");
      return;
    }

  // Default browser family (chrome / chromium).
  windowtitle = _L("Google Chrome");
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
  line  = _L("  var ");
  line += varname;
  line += _L("     = \"");
  line += escaped;
  line += _L("\";");
  file.AddLine(line);
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

  line.Format(_L("function EnsureApplication_%s()"), appkey.Get());
  file.AddLine(line);
  file.AddLine(_L("{"));
  ScriptRecord_AddJSVarString(file, _L("appname"), appname);
  {
    XSTRING pathstr = pathscript.Get();
    ScriptRecord_AddJSVarString(file, _L("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, _L("windowtitle"), windowtitle);
  if(isbrowser)
    {
      file.AddLine(_L("  // Browser detected: URL cannot be captured - edit this value before running."));
      file.AddLine(_L("  var url         = \"https://EDIT_ME/\";"));
    }
  file.AddLine(_L("  var key         = appname;"));
  file.AddLine(_L(""));
  file.AddLine(_L("  // Already prepared in this script run: only restore focus."));
  file.AddLine(_L("  if(__qa_application_ready[key])"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(_L("      Sleep(200);"));
  file.AddLine(_L("      return true;"));
  file.AddLine(_L("    }"));
  file.AddLine(_L(""));
  file.AddLine(_L("  if(WaitWindow(appname, windowtitle, 1500))"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      Console_Printf(\"[EnsureApplication] Already running: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(_L("    }"));
  file.AddLine(_L("   else"));
  file.AddLine(_L("    {"));
  if(isbrowser)
    {
      file.AddLine(_L("      if(!OpenURL(url))"));
      file.AddLine(_L("        {"));
      file.AddLine(_L("          Console_Printf(\"[EnsureApplication] FAIL OpenURL %s\\n\", url);"));
      file.AddLine(_L("          return false;"));
      file.AddLine(_L("        }"));
      file.AddLine(_L(""));
      file.AddLine(_L("      if(!WaitWindow(appname, windowtitle, 30000))"));
      file.AddLine(_L("        {"));
      file.AddLine(_L("          Console_Printf(\"[EnsureApplication] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(_L("          TerminateAplication(appname);"));
      file.AddLine(_L("          return false;"));
      file.AddLine(_L("        }"));
      file.AddLine(_L(""));
      file.AddLine(_L("      Console_Printf(\"[EnsureApplication] Opened URL %s\\n\", url);"));
    }
   else
    {
      file.AddLine(_L("      if(!ExecApplication(apppath))"));
      file.AddLine(_L("        {"));
      file.AddLine(_L("          Console_Printf(\"[EnsureApplication] FAIL ExecApplication %s\\n\", apppath);"));
      file.AddLine(_L("          return false;"));
      file.AddLine(_L("        }"));
      file.AddLine(_L(""));
      file.AddLine(_L("      if(!WaitWindow(appname, windowtitle, 30000))"));
      file.AddLine(_L("        {"));
      file.AddLine(_L("          Console_Printf(\"[EnsureApplication] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(_L("          TerminateAplication(appname);"));
      file.AddLine(_L("          return false;"));
      file.AddLine(_L("        }"));
      file.AddLine(_L(""));
      file.AddLine(_L("      Console_Printf(\"[EnsureApplication] Started %s\\n\", apppath);"));
    }
  file.AddLine(_L("    }"));
  file.AddLine(_L(""));
  file.AddLine(_L("  Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(_L("  Sleep(300);"));
  file.AddLine(_L("  __qa_application_ready[key] = true;"));
  file.AddLine(_L("  Console_Printf(\"[EnsureApplication] Ready: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(_L("  return true;"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));

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
  file.AddLine(_L("var __qa_application_ready = {};"));
  file.AddLine(_L(""));
  file.AddLine(_L("function WaitWindow(appname, windowtitle, timeoutms)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  var outx = { value: 0 };"));
  file.AddLine(_L("  var outy = { value: 0 };"));
  file.AddLine(_L("  var waited = 0;"));
  file.AddLine(_L("  while(waited < timeoutms)"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      Sleep(500);"));
  file.AddLine(_L("      waited = waited + 500;"));
  file.AddLine(_L("      if(Screen_GetPosXY(appname, windowtitle, outx, outy) == 0) return true;"));
  file.AddLine(_L("    }"));
  file.AddLine(_L("  return false;"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function ScriptBaseName(scriptname)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  var base = scriptname;"));
  file.AddLine(_L("  var dot  = -1;"));
  file.AddLine(_L("  if(base == \"\" || base == null || base == undefined) base = GetNameScript();"));
  file.AddLine(_L("  dot = base.indexOf(\".\");"));
  file.AddLine(_L("  if(dot >= 0) base = base.substring(0, dot);"));
  file.AddLine(_L("  return base;"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function ListTestFileName(scriptname)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  return ScriptBaseName(scriptname) + \"_ListTest.json\";"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function EvidenceFileName(scriptname)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  return ScriptBaseName(scriptname) + \"_\" + FileCSV_GetShortDateTime() + \".csv\";"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function EvidenceStart(scriptname)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  var dir  = GetPathScript() + \"evidences\";"));
  file.AddLine(_L("  var path = dir + \"\\\\\" + EvidenceFileName(scriptname);"));
  file.AddLine(_L("  MakeDir(dir);"));
  file.AddLine(_L("  if(!FileCSV_Create(path)) return false;"));
  file.AddLine(_L("  if(!FileCSV_SetHeader(\"ID\", \"Description\", \"Result\")) return false;"));
  file.AddLine(_L("  if(!FileCSV_Save()) return false;"));
  file.AddLine(_L("  return true;"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function EvidenceAdd(id, description, result)"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  var idstr = \"\";"));
  file.AddLine(_L("  if(id !== \"\" && id !== null && id !== undefined) idstr = \"\" + (id | 0);"));
  file.AddLine(_L("  return FileCSV_AddRecord(idstr, description, result);"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));
  file.AddLine(_L("function EvidenceEnd()"));
  file.AddLine(_L("{"));
  file.AddLine(_L("  return FileCSV_Close();"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));

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

  file.AddLine(_L("function main()"));
  file.AddLine(_L("{"));

  for(XDWORD c=0; c<scriptrecord_sessions.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = scriptrecord_sessions.Get(c);
      if(!session) continue;

      if(!session->appkey.IsEmpty() && session->appkey.Compare(lastkey) != 0)
        {
          line.Format(_L("  if(!EnsureApplication_%s()) return;"), session->appkey.Get());
          file.AddLine(line);
          lastkey = session->appkey.Get();
        }

      line.Format(_L("  Recorded_%03d();"), (int)session->id);
      file.AddLine(line);
    }

  if(currentsession)
    {
      if(!currentsession->appkey.IsEmpty() && currentsession->appkey.Compare(lastkey) != 0)
        {
          line.Format(_L("  if(!EnsureApplication_%s()) return;"), currentsession->appkey.Get());
          file.AddLine(line);
        }

      line.Format(_L("  Recorded_%03d();"), (int)currentsession->id);
      file.AddLine(line);
    }

  file.AddLine(_L("}"));

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
      ScriptRecord_Print(_L("[ScriptRecord] Cannot create %s"), xpath.Get());
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
      if(!last || !last->IsEmpty()) file.AddLine(_L(""));

      if(needhelpers)
        {
          ScriptRecord_WriteCommonHelpers(file);
          scriptrecord_readymapwritten = true;
        }
       else if(!ScriptRecord_ExistingHasReadyMap() && !scriptrecord_readymapwritten)
        {
          file.AddLine(_L("var __qa_application_ready = {};"));
          file.AddLine(_L(""));
          scriptrecord_readymapwritten = true;
        }
    }
   else
    {
      file.AddLine(_L("// ----------------------------------------------------------------------------"));
      line.Format(_L("// %s - auto-generated by ActionScriptQA script recorder (F1)"), outname->Get());
      file.AddLine(line);
      file.AddLine(_L("// Evidence CSV: evidences/<GetNameScript()>_<datetime>.csv"));
      file.AddLine(_L("// Optional ListTest: <GetNameScript()>_ListTest.json (loaded if present)."));
      file.AddLine(_L("// Layout: assets/Tests/<scriptbase>/{graphics,evidences}/ + <scriptbase>.js"));
      file.AddLine(_L("// App launch: EnsureApplication_<app>() in main() before that app's tests."));
      file.AddLine(_L("// New recording sessions append function Recorded_NNN() and refresh the entry point."));
      file.AddLine(_L("// ----------------------------------------------------------------------------"));
      file.AddLine(_L(""));
      ScriptRecord_WriteCommonHelpers(file);
      scriptrecord_readymapwritten = true;
    }

  // EnsureApplication_<key> for every known app that does not already have one.
  ScriptRecord_Print(_L("[ScriptRecord] WriteScript: ensuring application helpers..."));
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

  ScriptRecord_Print(_L("[ScriptRecord] WriteScript: writing Recorded_%03d (%d steps)..."), (int)scriptrecord_sessionindex, (int)scriptrecord_steps.GetSize());
  line.Format(_L("function Recorded_%03d()"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  file.AddLine(_L("{"));

  file.AddLine(_L("  var scriptname  = GetNameScript();"));
  line.Format(_L("  var session     = \"Recorded_%03d\";"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  ScriptRecord_AddJSVarString(file, _L("appname"), scriptrecord_appname);
  {
    XSTRING pathstr = apppathscript.Get();
    ScriptRecord_AddJSVarString(file, _L("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, _L("windowtitle"), scriptrecord_windowtitle);

  file.AddLine(_L("  var outx = { value: 0 };"));
  file.AddLine(_L("  var outy = { value: 0 };"));
  file.AddLine(_L("  var i = 0;"));
  file.AddLine(_L("  var status = 1;"));
  file.AddLine(_L("  var stepresult = \"PASS\";"));
  file.AddLine(_L("  var stepdesc = \"\";"));
  file.AddLine(_L("  var catalogpath = \"\";"));
  file.AddLine(_L("  var listloaded = false;"));
  file.AddLine(_L(""));
  file.AddLine(_L("  Console_Printf(\"[%s] Start %s\\n\", scriptname, session);"));
  file.AddLine(_L(""));
  file.AddLine(_L("  if(!EvidenceStart(scriptname))"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      Console_Printf(\"[%s] FAIL EvidenceStart/FileCSV\\n\", scriptname);"));
  file.AddLine(_L("      return;"));
  file.AddLine(_L("    }"));
  file.AddLine(_L("  Console_Printf(\"[%s] Evidence CSV %s\\n\", scriptname, FileCSV_GetPath());"));
  file.AddLine(_L(""));
  file.AddLine(_L("  catalogpath = GetPathScript() + ListTestFileName(scriptname);"));
  file.AddLine(_L("  if(IsItExists(catalogpath))"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      if(TraceTests_Load(catalogpath))"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          listloaded = true;"));
  file.AddLine(_L("          EvidenceAdd(\"\", \"TraceTests_Load\", \"PASS\");"));
  file.AddLine(_L("          Console_Printf(\"[%s] ListTest loaded %s\\n\", scriptname, catalogpath);"));
  file.AddLine(_L("        }"));
  file.AddLine(_L("       else"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          EvidenceAdd(\"\", \"TraceTests_Load\", \"FAIL\");"));
  file.AddLine(_L("          Console_Printf(\"[%s] FAIL TraceTests_Load %s\\n\", scriptname, catalogpath);"));
  file.AddLine(_L("        }"));
  file.AddLine(_L("    }"));
  file.AddLine(_L("   else"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      EvidenceAdd(\"\", \"TraceTests_Load\", \"SKIP\");"));
  file.AddLine(_L("      Console_Printf(\"[%s] ListTest not found (optional) %s\\n\", scriptname, catalogpath);"));
  file.AddLine(_L("    }"));
  file.AddLine(_L(""));
  file.AddLine(_L("  // Application is started/focused from main() via EnsureApplication_<app>() before this test."));
  file.AddLine(_L("  Screen_SetBmpFindCFG(12, 40);"));
  file.AddLine(_L("  Sleep(300);"));
  file.AddLine(_L(""));

  file.AddLine(_L("  var actions = ["));
  for(XDWORD c=0; c<scriptrecord_steps.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = scriptrecord_steps.Get(c);
      if(!step) continue;

      XSTRING escaped;
      XCHAR*  comma = (c + 1 < scriptrecord_steps.GetSize()) ? _L(",") : _L("");

      switch(step->type)
        {
          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(_L("    { type: \"text\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(_L("    { type: \"key\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT :
                line.Format(_L("    { type: \"wait\", bmp: \"%s\", timeoutms: %d, intervalms: %d }%s"),
                            step->bitmapname.Get(),
                            APPFLOW_CFG.ScriptRecord_GetWaitTimeoutMs(),
                            APPFLOW_CFG.ScriptRecord_GetWaitIntervalMs(),
                            comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK :
          default :
                line.Format(_L("    { type: \"click\", bmp: \"%s\", x: %d, y: %d }%s"),
                            step->bitmapname.Get(), step->layoutx, step->layouty, comma);
                break;
        }

      file.AddLine(line);
    }
  file.AddLine(_L("  ];"));
  file.AddLine(_L(""));
  file.AddLine(_L("  for(i = 0; i < actions.length; i++)"));
  file.AddLine(_L("    {"));
  file.AddLine(_L("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(_L("      Sleep(80);"));
  file.AddLine(_L("      stepresult = \"PASS\";"));
  file.AddLine(_L("      stepdesc = actions[i].type;"));
  file.AddLine(_L(""));
  file.AddLine(_L("      if(actions[i].type == \"click\")"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          outx = { value: 0 };"));
  file.AddLine(_L("          outy = { value: 0 };"));
  file.AddLine(_L("          stepdesc = \"click \" + actions[i].bmp;"));
  file.AddLine(_L("          status = Screen_GetPosXY(appname, windowtitle, actions[i].bmp, outx, outy);"));
  file.AddLine(_L("          if(status != 0)"));
  file.AddLine(_L("            {"));
  file.AddLine(_L("              var winx = { value: 0 };"));
  file.AddLine(_L("              var winy = { value: 0 };"));
  file.AddLine(_L("              if(Screen_GetPosXY(appname, windowtitle, winx, winy) == 0)"));
  file.AddLine(_L("                {"));
  file.AddLine(_L("                  outx.value = winx.value + actions[i].x;"));
  file.AddLine(_L("                  outy.value = winy.value + actions[i].y;"));
  file.AddLine(_L("                  status = 0;"));
  file.AddLine(_L("                }"));
  file.AddLine(_L("            }"));
  file.AddLine(_L("          if(status == 0)"));
  file.AddLine(_L("            {"));
  file.AddLine(_L("              InpSim_Mouse_Click(outx.value, outy.value);"));
  file.AddLine(_L("              Console_Printf(\"[%s] Click %s at %d,%d\\n\", scriptname, actions[i].bmp, outx.value, outy.value);"));
  file.AddLine(_L("              Sleep(400);"));
  file.AddLine(_L("            }"));
  file.AddLine(_L("           else"));
  file.AddLine(_L("            {"));
  file.AddLine(_L("              stepresult = \"FAIL\";"));
  file.AddLine(_L("              Console_Printf(\"[%s] FAIL find %s\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(_L("            }"));
  file.AddLine(_L("        }"));
  file.AddLine(_L("      else if(actions[i].type == \"wait\")"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          stepdesc = \"wait \" + actions[i].bmp;"));
  file.AddLine(_L("          if(Screen_WaitBitmap(appname, windowtitle, actions[i].bmp, actions[i].timeoutms, actions[i].intervalms))"));
  file.AddLine(_L("            {"));
  file.AddLine(_L("              Console_Printf(\"[%s] PASS wait %s\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(_L("            }"));
  file.AddLine(_L("           else"));
  file.AddLine(_L("            {"));
  file.AddLine(_L("              stepresult = \"FAIL\";"));
  file.AddLine(_L("              Console_Printf(\"[%s] FAIL wait %s (timeout)\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(_L("            }"));
  file.AddLine(_L("        }"));
  file.AddLine(_L("      else if(actions[i].type == \"text\")"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          stepdesc = \"text\";"));
  file.AddLine(_L("          InpSim_Key_ClickByText(actions[i].value, 40);"));
  file.AddLine(_L("          Console_Printf(\"[%s] Type text (%d chars)\\n\", scriptname, actions[i].value.length);"));
  file.AddLine(_L("          Sleep(200);"));
  file.AddLine(_L("        }"));
  file.AddLine(_L("      else if(actions[i].type == \"key\")"));
  file.AddLine(_L("        {"));
  file.AddLine(_L("          stepdesc = \"key \" + actions[i].value;"));
  file.AddLine(_L("          InpSim_Key_ClickByLiteral(actions[i].value, 40);"));
  file.AddLine(_L("          Console_Printf(\"[%s] Key %s\\n\", scriptname, actions[i].value);"));
  file.AddLine(_L("          Sleep(200);"));
  file.AddLine(_L("        }"));
  file.AddLine(_L(""));
  file.AddLine(_L("      EvidenceAdd(i + 1, stepdesc, stepresult);"));
  file.AddLine(_L("    }"));
  file.AddLine(_L(""));
  file.AddLine(_L("  if(listloaded) TraceTests_DeleteAll();"));
  file.AddLine(_L("  EvidenceEnd();"));
  file.AddLine(_L("  Console_Printf(\"[%s] End %s\\n\", scriptname, session);"));
  file.AddLine(_L("}"));
  file.AddLine(_L(""));

  ScriptRecord_WriteMain(file, &currentsession);

  ScriptRecord_Print(_L("[ScriptRecord] WriteScript: flushing file to disk..."));
  if(!file.WriteAllFile())
    {
      file.Close();
      ScriptRecord_Print(_L("[ScriptRecord] WriteAllFile failed for %s"), xpath.Get());
      return false;
    }

  file.Close();
  ScriptRecord_Print(_L("[ScriptRecord] WriteScript: done."));
  return true;
}

#pragma endregion
