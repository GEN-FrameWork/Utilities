/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA.cpp
* 
* @class      ACTIONSCRIPTQA
* @brief      Locomotive Operations for Kinetic Inspections
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

/*---- PRECOMPILATION INCLUDES ----------------------------------------------------------------------------------------*/
#pragma region PRECOMPILATION_INCLUDES

#include "GEN_Defines.h"
#include "ActionScriptQA_TranslateID.h"

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
#include "XThread.h"
#include "XTrace.h"
#include "XObserver.h"
#include "XProcessManager.h"

#ifdef WINDOWS
#include <windows.h>
#endif

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

#include "XPathsManager.h"

#include "ActionScriptQA_CFG.h"

#include "INPManager.h"
#include "INPFactory.h"
#include "INPDevice.h"
#include "INPButton.h"
#include "INPSimulate.h"

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
  id   = 0;
  winx = 0;
  winy = 0;
  winw = 0;
  winh = 0;
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

  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS ,  APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS  ,  APPFLOW_DEFAULT_DIRECTORY_SCRIPTS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_TESTS    ,  APPFLOW_DEFAULT_DIRECTORY_TESTS);

  GEN_XPATHSMANAGER.CreateAllPathSectionOnDisk();

  //--------------------------------------------------------------------------------------------------

  InitFSMachine();

  //--------------------------------------------------------------------------------------

  xmutexshowallstatus = GEN_XFACTORY.Create_Mutex();
  if(!xmutexshowallstatus) return false;

  //--------------------------------------------------------------------------------------------------
  // Load .lng before first APPFLOW_CFG access (constructor Ini/DoVariableMapping may use TL).

  { XPATH xpathsection;
    GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathsection);
    xpath.Create(3 , xpathsection.Get(), APPLICATION_NAMEFILE, XTRANSLATION_NAMEFILEEXT);
    if(!GEN_XTRANSLATION.Ini(xpath))
      {
        //return false;
      }
  }

  //--------------------------------------------------------------------------------------

  APPFLOW_CFG_SETAUTOMATICTRACETARGETS

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

  if(!scriptsautorundone)
    {
      char* envautorun = getenv("ACTIONSCRIPTQA_AUTORUN");
      if(envautorun)
        {
          XSTRING autorun;

          autorun.Set(envautorun);
          if((!autorun.Compare(NL("1"), true)) ||
             (!autorun.Compare(NL("si"), true)) ||
             (!autorun.Compare(NL("yes"), true)) ||
             (!autorun.Compare(NL("true"), true)))
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
          if((scancode == 0x3C) || (scancode == 0x3D)) // F2 / F3 — ScriptRecord_Update via INPDEVICE
            {
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
  APPFLOW_LOG_ENTRY(XLOGLEVEL_WARNING, APPFLOW_CFG_LOG_SECTIONID_STATUSAPP, false, TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_KEY_PRESSED_0X), key, character);

  if(console) console->Printf(NL("\r    \r"));

  switch(key)
    {
      case 0x20 : if(ScriptRecord_IsActive())
                    {
                      ScriptRecord_Print(NL("[Tests Exec] Recording is ON — press ESC/F1 to finish before running scripts."));
                    }
                   else
                    {
                      ExecScripts();
                    }
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
  // Screen_SetFocus / UI automation steals WM focus from the host terminal. On Linux that
  // leaves stdin (ESC/Space) dead until the terminal is focused again _ capture and restore.
  void* inputfocus = NULL;
  if(console) console->Get_InputFocus(inputfocus);

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

  if(console)
    {
      if(inputfocus) console->Set_InputFocus(inputfocus);
                else console->Set_Focus();
    }
                                                 
  Show_BlankLine();
                                                  
  if(xtimerglobal)
    { 
      XSTRING string;
                                                      
      xtimerglobal->GetMeasureString(string, true);
      console->Printf(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TOTAL_WORKING_TIME), string.Get());                                                      
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
  graphics += NL("graphics");
  if(!xdir->Exist(graphics)) xdir->Make(graphics, true);

  XPATH evidences = testroot;
  evidences.Slash_Add();
  evidences += NL("evidences");
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
  section.Format(NL("%s/%s/graphics"), APPFLOW_DEFAULT_DIRECTORY_TESTS, basename.Get());
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
  XSTRING entry = scriptentry.Get();
  SCRIPT::EliminateExtraChars(&entry);
  if(entry.IsEmpty()) return false;

  XSTRING basename;
  if(!Test_GetBaseName(entry, basename)) return false;

  // Keep original extension from CFG entry when present; default to .js
  XPATH   entrypath(entry);
  XSTRING nameext;
  entrypath.GetNamefileExt(nameext);
  if(nameext.IsEmpty())
    {
      nameext = basename.Get();
      nameext += NL(".js");
    }

  // Prefer assets/Tests/<base>/<file> (recorder layout). Fall back to assets/scripts/<file>
  // so CFG entries still run if the Tests copy was deleted (F5) or not yet migrated.
  XPATH xpath;
  XPATH xpathscripts;
  bool  found = false;

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(Test_GetRootPath(basename, xpath))
    {
      xpath.Slash_Add();
      xpath += nameext.Get();
      found = xdir->Exist(xpath);
    }

  if(!found)
    {
      if(GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS, xpathscripts))
        {
          xpathscripts.Slash_Add();
          xpathscripts += nameext.Get();
          if(xdir->Exist(xpathscripts))
            {
              xpath = xpathscripts;
              found = true;
              ScriptRecord_Print(NL("[Tests Exec] Using legacy scripts path: %s"), xpath.Get());
            }
        }
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  if(!found)
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TESTS_CANNOT_LOAD), nameext.Get());
      ScriptRecord_Print(NL("[Tests Exec] Expected: assets/Tests/%s/%s (or assets/scripts/%s)"), basename.Get(), nameext.Get(), nameext.Get());
      return false;
    }

  if(!Test_EnsureLayout(basename)) return false;
  if(!Test_BindGraphics(basename)) return false;

  SCRIPT* script = SCRIPT::Create(nameext.Get());
  if(!script)
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TESTS_CANNOT_CREATE_SCRIPT_ENGINE_FOR), nameext.Get());
      Test_RestoreDefaultGraphics();
      return false;
    }

  // Surface SyntaxError / runtime errors on the ASQ console.
  SubscribeEvent(SCRIPT_XEVENT_TYPE_ERROR, script);
  SubscribeEvent(SCRIPT_XEVENT_TYPE_BREAK, script);

  AdjustLibraries(script);

  if(!script->Load(xpath))
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TESTS_CANNOT_LOAD), xpath.Get());
      UnSubscribeEvent(SCRIPT_XEVENT_TYPE_ERROR, script);
      UnSubscribeEvent(SCRIPT_XEVENT_TYPE_BREAK, script);
      GEN_DELETE script;
      Test_RestoreDefaultGraphics();
      return false;
    }

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TESTS_RUNNING), xpath.Get());
  script->Run();
  UnSubscribeEvent(SCRIPT_XEVENT_TYPE_ERROR, script);
  UnSubscribeEvent(SCRIPT_XEVENT_TYPE_BREAK, script);
  GEN_DELETE script;

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

  string  = TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPT_STATUS);
  string2.Format(NL("%s"), script->GetNameScript()->Get());

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
  if(!event) return;

  switch(event->GetEventType())
    {
      case SCRIPT_XEVENT_TYPE_ERROR :
        // Console (not XTRACE): ASQ user-facing output goes through the console.
        ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPT_ERROR_LINE),
                           event->GetError(),
                           event->GetErrorText() ? event->GetErrorText()->Get() : NL(""),
                           event->GetNLine(),
                           event->GetCurrentToken() ? event->GetCurrentToken()->Get() : NL(""));
        break;

      case SCRIPT_XEVENT_TYPE_BREAK :
        ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPT_BREAK_LINE),
                           event->GetNLine(),
                           event->GetCurrentToken() ? event->GetCurrentToken()->Get() : NL(""));
        break;

      default :
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
  scriptrecord_hotkeybusy       = false;
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
  scriptrecord_readymapwritten  = false;
  scriptrecord_winx             = 0;
  scriptrecord_winy             = 0;
  scriptrecord_winw             = 0;
  scriptrecord_winh             = 0;
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
      basename = NL("Tests_Recorded");
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
          nameext += NL(".js");
        }
      xpath += nameext.Get();
    }
   else
    {
      xpath += NL("Tests_Recorded.js");
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

  outstring.Add(NL("\n"));
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
  scriptrecord_hotkeybusy       = false;
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
  scriptrecord_readymapwritten  = false;
  scriptrecord_winx             = 0;
  scriptrecord_winy             = 0;
  scriptrecord_winw             = 0;
  scriptrecord_winh             = 0;
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
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_FINISHING_SESSION_FLUSHING_TEXT));
          ScriptRecord_FlushText(false);
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_FINISHING_SESSION_WRITING_SCRIPT));
          if(ScriptRecord_WriteScript())
            {
              ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_SAVED_SCRIPT_AND_LEFT_RECORD_MODE));
            }
           else
            {
              ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ERROR_WRITING_SCRIPT_LEFT_RECORD_MO));
            }
        }
       else
        {
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_LEFT_RECORD_MODE_NO_APP_SELECTED));
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
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ON_F1_ESC_FINISH_F5_DISCARD_DELETE));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_1_LEFT_CLICK_A_WINDOW_OF_THE_APP_UN));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_2_LEFT_CLICK_UI_TARGETS_MOUSE_CLICK));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_3_RIGHT_CLICK_A_ZONE_TO_WAIT_ASSERT));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_4_TYPE_TEXT_LOGIN_PASSWORD_ENTER_TA));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_OUTPUT_TESTS), basename.Get(), outname ? outname->Get() : TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_TESTS_RECORDED_JS));
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_SESSION_FUNCTION_RECORDED_NEXT_BITM), (int)scriptrecord_sessionindex, (int)scriptrecord_nextbitmapindex);

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
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_RECORD_MODE_CANCELLED_F5));
    }

  XSTRING basename;
  if(!ScriptRecord_GetRecordedBaseName(basename))
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ERROR_RESOLVING_RECORDED_TEST_NAME));
      return false;
    }

  XPATH testroot;
  if(!Test_GetRootPath(basename, testroot))
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ERROR_RESOLVING_TESTS_PATH), basename.Get());
      return false;
    }

  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir)
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ERROR_CREATING_XDIR));
      return false;
    }

  bool status = true;
  if(xdir->Exist(testroot))
    {
      status = xdir->Delete(testroot, true);
      if(status)
        {
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_DELETED_TESTS_DIRECTORY_AND_FILES), basename.Get());
        }
       else
        {
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_ERROR_DELETING_TESTS), basename.Get());
        }
    }
   else
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_NOTHING_TO_DELETE_TESTS_MISSING), basename.Get());
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
* @brief      ScriptRecord_Update — LMB selects app; F2 click-capture; F3 wait-capture.
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_Update()
{
  if(!ScriptRecord_EnsureInputDevices()) return false;

  INPDEVICE* mouse = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(!mouse) return false;

  bool ldown = false;
  INPBUTTON* lbtn = mouse->GetButton(INPBUTTON_ID_MOUSE_LEFT);
  if(lbtn) ldown = lbtn->IsPressed();

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

  // Left click only selects the target app (not capture — that is F2/F3).
  if(!scriptrecord_hotkeybusy && ldown && !scriptrecord_mousewasdown && havecursor && !scriptrecord_appselected)
    {
      ScriptRecord_SelectApp(cursorx, cursory);
    }

  scriptrecord_mousewasdown = ldown;

  if(scriptrecord_appselected)
    {
      INPDEVICE* keyboard = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_KEYBOARD);
      if(keyboard)
        {
          INPBUTTON* f2btn = keyboard->GetButton(INPBUTTON_ID_F2);
          INPBUTTON* f3btn = keyboard->GetButton(INPBUTTON_ID_F3);
          bool f2down = (f2btn && f2btn->IsPressed());
          bool f3down = (f3btn && f3btn->IsPressed());

          if(!scriptrecord_hotkeybusy)
            {
              if(f2down && !scriptrecord_btnwasdown[INPBUTTON_ID_F2])
                {
                  ScriptRecord_HotkeyCapture(ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK);
                }
               else if(f3down && !scriptrecord_btnwasdown[INPBUTTON_ID_F3])
                {
                  ScriptRecord_HotkeyCapture(ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT);
                }
            }

          // Keep F2/F3 edge state even if a capture just held the key down for hundreds of ms.
          scriptrecord_btnwasdown[INPBUTTON_ID_F2] = f2down;
          scriptrecord_btnwasdown[INPBUTTON_ID_F3] = f3down;
        }

      if(!scriptrecord_hotkeybusy)
        {
          ScriptRecord_UpdateKeys();
        }
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
          case _C('\\') : target.Add(NL("\\\\")); break;
          case _C('\"') : target.Add(NL("\\\"")); break;
          case _C('\n') : target.Add(NL("\\n"));  break;
          case _C('\r') : target.Add(NL("\\r"));  break;
          case _C('\t') : target.Add(NL("\\t"));  break;
          // '%' must not reach XSTRING::Format / Printf masks (FormatArg can hang on "%...").
          case _C('%')  : target.Add(NL("_"));    break;
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

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT_CHARS), (int)scriptrecord_steps.GetSize(), scriptrecord_textpending.Get(), (int)scriptrecord_textpending.GetSize());
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

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY), (int)scriptrecord_steps.GetSize(), literal);
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
          case INPBUTTON_ID_INTRO     : literal = NL("ENTER");       break;
          case INPBUTTON_ID_TAB       : literal = NL("TAB");         break;
          case INPBUTTON_ID_BACK_SPACE: literal = NL("BACKSPACE");   break;
          case INPBUTTON_ID_DELETE    : literal = NL("DEL");         break;
          case INPBUTTON_ID_LEFT      : literal = NL("LEFT ARROW");  break;
          case INPBUTTON_ID_RIGHT     : literal = NL("RIGHT ARROW"); break;
          case INPBUTTON_ID_UP        : literal = NL("UP ARROW");    break;
          case INPBUTTON_ID_DOWN      : literal = NL("DOWN ARROW");  break;
          case INPBUTTON_ID_HOME      : literal = NL("HOME");        break;
          case INPBUTTON_ID_END       : literal = NL("END");         break;
          case INPBUTTON_ID_PAGE_UP   : literal = NL("PAGE UP");     break;
          case INPBUTTON_ID_PAGE_DOWN : literal = NL("PAGE DOWN");   break;
          case INPBUTTON_ID_INSERT    : literal = NL("INS");         break;
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
          ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_TYPING_CHARS), (int)scriptrecord_textpending.GetSize());
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
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_NO_WINDOW_UNDER_CURSOR));
      return false;
    }

  if(!process.GetWindowHandle())
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_NO_WINDOW_UNDER_CURSOR));
      return false;
    }

  if(!process.GetName() || process.GetName()->IsEmpty())
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CANNOT_RESOLVE_EXECUTABLE_NAME_FOR));
      return false;
    }

  if(process.GetName()->Find(APPLICATION_NAMEFILE, true) != XSTRING_NOTFOUND)
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_IGNORED_CLICK_ON_ACTIONSCRIPTQA_ITS));
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

  // Capture geometry for playback: outer desktop position + CLIENT size.
  // Screen_SetPosition uses outer top-left; Screen_Resize expects client WxH
  // (it expands to outer via ClientSizeToWindowSize). Storing GetWindowRect
  // width/height here would make playback recreate a larger window.
  scriptrecord_winx = 0;
  scriptrecord_winy = 0;
  scriptrecord_winw = 0;
  scriptrecord_winh = 0;
  {
    GRPSCREEN* geoscreen = GEN_GRPFACTORY.CreateScreen();
    if(geoscreen)
      {
        geoscreen->SetHandle(scriptrecord_windowhandle);
        int posx = 0;
        int posy = 0;
        if(geoscreen->Get_Position(posx, posy))
          {
            scriptrecord_winx = posx;
            scriptrecord_winy = posy;
          }
         else if(process.GetWindowRect() && !process.GetWindowRect()->IsEmpty())
          {
            scriptrecord_winx = process.GetWindowRect()->x1;
            scriptrecord_winy = process.GetWindowRect()->y1;
          }

        int clientw = 0;
        int clienth = 0;
        if(geoscreen->GetClientSize(clientw, clienth) && (clientw > 0) && (clienth > 0))
          {
            scriptrecord_winw = clientw;
            scriptrecord_winh = clienth;
          }
         else if(process.GetWindowRect() && !process.GetWindowRect()->IsEmpty())
          {
            // Fallback: derive client from outer using border/title metadata.
            int border = process.GetWindowBorderWidth();
            int titleh = process.GetWindowTitleHeight();
            scriptrecord_winw = (int)process.GetWindowRect()->GetWidth()  - (border * 2);
            scriptrecord_winh = (int)process.GetWindowRect()->GetHeight() - titleh - border;
            if(scriptrecord_winw < 0) scriptrecord_winw = 0;
            if(scriptrecord_winh < 0) scriptrecord_winh = 0;
          }

        GEN_GRPFACTORY.DeleteScreen(geoscreen);
      }
     else if(process.GetWindowRect() && !process.GetWindowRect()->IsEmpty())
      {
        scriptrecord_winx = process.GetWindowRect()->x1;
        scriptrecord_winy = process.GetWindowRect()->y1;
        int border = process.GetWindowBorderWidth();
        int titleh = process.GetWindowTitleHeight();
        scriptrecord_winw = (int)process.GetWindowRect()->GetWidth()  - (border * 2);
        scriptrecord_winh = (int)process.GetWindowRect()->GetHeight() - titleh - border;
        if(scriptrecord_winw < 0) scriptrecord_winw = 0;
        if(scriptrecord_winh < 0) scriptrecord_winh = 0;
      }
  }

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

  // Same app / append session: refresh EnsureApplication_<app> geometry in kept
  // lines (do NOT delete the function — later WriteScript passes would otherwise
  // drop it when ensurekeyswritten still said it was already emitted).
  {
    XSTRING appkey;
    ScriptRecord_MakeAppKey(scriptrecord_appname, appkey);
    if(!ScriptRecord_UpdateEnsureGeometryInExisting(appkey, scriptrecord_winx, scriptrecord_winy, scriptrecord_winw, scriptrecord_winh))
      {
        // Not in kept lines yet (first capture for this app, or a prior broken write).
        // Clear write-pass bookkeeping so WriteScript emits EnsureApplication again.
        for(XDWORD c=0; c<scriptrecord_ensurekeyswritten.GetSize(); )
          {
            XSTRING* key = scriptrecord_ensurekeyswritten.Get(c);
            if(key && key->Compare(appkey) == 0)
              {
                scriptrecord_ensurekeyswritten.Delete(key);
                delete key;
                continue;
              }
            c++;
          }
      }
  }

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_APP_SELECTED), scriptrecord_appname.Get());
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_PATH), scriptrecord_apppath.Get());
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WINDOW), scriptrecord_windowtitle.Get());
  if((scriptrecord_winw > 0) && (scriptrecord_winh > 0))
    {
      ScriptRecord_Print(NL("[Script Rec] Window geometry (pos + client size): %d,%d %dx%d"), scriptrecord_winx, scriptrecord_winy, scriptrecord_winw, scriptrecord_winh);
    }
   else
    {
      ScriptRecord_Print(NL("[Script Rec] WARNING: could not capture window geometry for playback."));
    }
  if(ScriptRecord_IsBrowserApp(scriptrecord_appname, scriptrecord_apppath))
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_BROWSER_DETECTED_ENSURE_WILL_USE_OP));
    }
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CLICK_UI_CONTROLS_TO_CAPTURE_BITMAP));

  ScriptRecord_WriteScript();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ParkCursorBottomRight(int& outx, int& outy)
* @brief      Desktop coords of the target window outer bottom-right (inside by 2px).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ParkCursorBottomRight(int& outx, int& outy)
{
  outx = 0;
  outy = 0;
  if(!scriptrecord_windowhandle) return false;

  GRPRECTINT winrect;
  bool       got = false;

  #ifdef WINDOWS
  if(IsWindow((HWND)scriptrecord_windowhandle))
    {
      RECT outer;
      if(GetWindowRect((HWND)scriptrecord_windowhandle, &outer))
        {
          winrect.x1 = outer.left;
          winrect.y1 = outer.top;
          winrect.x2 = outer.right;
          winrect.y2 = outer.bottom;
          got = true;
        }
    }
  #endif

  if(!got)
    {
      XVECTOR<XPROCESS*> applist;
      if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
        {
          XPROCESS* process = ActionScriptQA_FindRunningByHandle(applist, scriptrecord_windowhandle);
          if(process && process->GetWindowRect() && !process->GetWindowRect()->IsEmpty())
            {
              winrect = (*process->GetWindowRect());
              got = true;
            }
          ActionScriptQA_FreeRunningList(applist);
        }
    }

  if(!got)
    {
      // Last resort: recorded outer pos + client size (approximate).
      if((scriptrecord_winw > 0) && (scriptrecord_winh > 0))
        {
          winrect.x1 = scriptrecord_winx;
          winrect.y1 = scriptrecord_winy;
          winrect.x2 = scriptrecord_winx + scriptrecord_winw;
          winrect.y2 = scriptrecord_winy + scriptrecord_winh;
          got = true;
        }
    }

  if(!got) return false;

  outx = winrect.x2 - 2;
  outy = winrect.y2 - 2;
  if(outx < winrect.x1) outx = winrect.x1;
  if(outy < winrect.y1) outy = winrect.y1;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_HotkeyCapture(ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype)
* @brief      F2/F3 capture: anchor cursor → park bottom-right → capture → verify (+ click on F2).
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_HotkeyCapture(ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype)
{
  if(!scriptrecord_appselected) return false;
  if((steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK) && (steptype != ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT)) return false;
  if(scriptrecord_hotkeybusy) return false;

  scriptrecord_hotkeybusy = true;

  int screenx = 0;
  int screeny = 0;
  bool havecursor = false;
  {
    GRPSCREEN* cursorscreen = GEN_GRPFACTORY.CreateScreen();
    if(cursorscreen)
      {
        havecursor = cursorscreen->GetCursorDesktopPosition(screenx, screeny);
        GEN_GRPFACTORY.DeleteScreen(cursorscreen);
      }
  }

  if(!havecursor)
    {
      ScriptRecord_Print(NL("[Script Rec] ERROR: cannot read cursor position for F2/F3 capture."));
      scriptrecord_hotkeybusy = false;
      return false;
    }

  ScriptRecord_Print(NL("[Script Rec] Hotkey %s at desktop %d,%d — parking cursor, capturing..."),
                     (steptype == ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK) ? NL("F2/click") : NL("F3/wait"),
                     screenx, screeny);

  int parkx = 0;
  int parky = 0;
  INPSIMULATE* sim = GEN_INPFACTORY.CreateSimulator();
  if(sim && ScriptRecord_ParkCursorBottomRight(parkx, parky))
    {
      sim->Mouse_SetPos(parkx, parky);
      GEN_XSLEEP.MilliSeconds(220);
    }
   else
    {
      ScriptRecord_Print(NL("[Script Rec] WARNING: could not park cursor at window corner; capturing with cursor in place."));
    }

  bool captured = ScriptRecord_CaptureClick(screenx, screeny, steptype);
  if(!captured)
    {
      if(sim) sim->Mouse_SetPos(screenx, screeny);
      if(sim) GEN_INPFACTORY.DeleteSimulator(sim);
      scriptrecord_hotkeybusy = false;
      return false;
    }

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = scriptrecord_steps.Get(scriptrecord_steps.GetSize() - 1);
  if(step)
    {
      ScriptRecord_VerifyAndAct(steptype, step->bitmapname, step->layoutx, step->layouty, screenx, screeny);
    }
   else if(sim && (steptype == ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT))
    {
      sim->Mouse_SetPos(screenx, screeny);
    }

  if(sim) GEN_INPFACTORY.DeleteSimulator(sim);
  scriptrecord_hotkeybusy = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_VerifyAndAct(ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype, ...)
* @brief      Second pass while recording: find saved bitmap; F2 clicks, F3 only asserts.
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_VerifyAndAct(ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype, XSTRING& bmpname, int layoutx, int layouty, int anchordesktopx, int anchordesktopy)
{
  if(bmpname.IsEmpty() || !scriptrecord_windowhandle) return false;

  XVECTOR<XPROCESS*> applist;
  XPROCESS*          process = NULL;
  bool               havelist = false;
  int                border   = 0;
  int                titleh   = 0;
  GRPRECTINT         winrect;

  #ifdef WINDOWS
  const bool hwndstillvalid = (scriptrecord_windowhandle && IsWindow((HWND)scriptrecord_windowhandle));
  #else
  const bool hwndstillvalid = (scriptrecord_windowhandle != NULL);
  #endif

  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      havelist = true;
      process = ActionScriptQA_FindRunningByHandle(applist, scriptrecord_windowhandle);
    }

  void* hwnd = hwndstillvalid ? scriptrecord_windowhandle
                              : (process ? process->GetWindowHandle() : scriptrecord_windowhandle);
  if(!hwnd)
    {
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(NL("[Script Rec] VERIFY FAIL: target window invalid for %s"), bmpname.Get());
      return false;
    }

  if(process && process->GetWindowRect())
    {
      winrect = (*process->GetWindowRect());
      border  = process->GetWindowBorderWidth();
      titleh  = process->GetWindowTitleHeight();
    }

  #ifdef WINDOWS
  {
    RECT outer;
    if(GetWindowRect((HWND)hwnd, &outer))
      {
        winrect.x1 = outer.left;
        winrect.y1 = outer.top;
        winrect.x2 = outer.right;
        winrect.y2 = outer.bottom;
      }
  }
  #endif

  GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
  if(!screen)
    {
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      return false;
    }

  screen->SetHandle(hwnd);

  int clientw = 0;
  int clienth = 0;
  if(!screen->GetClientSize(clientw, clienth) || (clientw <= 0) || (clienth <= 0))
    {
      clientw = (int)winrect.GetWidth()  - (border * 2);
      clienth = (int)winrect.GetHeight() - titleh - border;
    }

  screen->SetWidth(clientw);
  screen->SetHeight(clienth);

  GRPBITMAP* bitmapscreen = screen->CaptureContent(NULL, hwnd);
  GEN_GRPFACTORY.DeleteScreen(screen);

  XPATH xpathbmp;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, xpathbmp);
  xpathbmp.Slash_Add();
  xpathbmp += bmpname.Get();

  GRPBITMAPFILE bmpfile(xpathbmp);
  GRPBITMAP*    bitmapref = bmpfile.Load();

  int  foundx = 0;
  int  foundy = 0;
  bool found  = false;

  // Same defaults as generated script Screen_SetBmpFindCFG(8, 25).
  if(bitmapscreen && bitmapref)
    {
      found = bitmapscreen->FindSubBitmap(bitmapref, foundx, foundy, 8, 25);
    }

  int clickx = anchordesktopx;
  int clicky = anchordesktopy;

  if(found && bitmapref)
    {
      clickx = winrect.x1 + foundx + ((int)bitmapref->GetWidth()  / 2) + border;
      clicky = winrect.y1 + foundy + ((int)bitmapref->GetHeight() / 2) + titleh;
      ScriptRecord_Print(NL("[Script Rec] VERIFY PASS: found %s at desktop %d,%d (client %d,%d)"),
                         bmpname.Get(), clickx, clicky, foundx, foundy);
    }
   else
    {
      // Layout fallback (same idea as playback GetPosXY miss → window pos + layout).
      clickx = winrect.x1 + layoutx;
      clicky = winrect.y1 + layouty;
      ScriptRecord_Print(NL("[Script Rec] VERIFY WARN: %s not found after capture — using layout fallback %d,%d"),
                         bmpname.Get(), clickx, clicky);
    }

  if(bitmapref)    GEN_GRPFACTORY.DeleteBitmap(bitmapref);
  if(bitmapscreen) GEN_GRPFACTORY.DeleteBitmap(bitmapscreen);
  if(havelist)     ActionScriptQA_FreeRunningList(applist);

  INPSIMULATE* sim = GEN_INPFACTORY.CreateSimulator();
  if(!sim) return found;

  if(steptype == ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK)
    {
      sim->Mouse_Click(clickx, clicky);
      ScriptRecord_Print(NL("[Script Rec] Replay click at %d,%d"), clickx, clicky);
      GEN_XSLEEP.MilliSeconds(400);
    }
   else
    {
      // Wait step: restore cursor to the anchored point so the user can continue aiming.
      sim->Mouse_SetPos(anchordesktopx, anchordesktopy);
    }

  GEN_INPFACTORY.DeleteSimulator(sim);
  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_CaptureClick(int screenx, int screeny, ACTIONSCRIPTQA_SCRIPTRECORD_STEPTYPE steptype)
* @brief      Capture bitmap around anchored desktop point: CLICK (F2) or WAIT/assert (F3)
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
  bool               havelist = false;

  // Prefer the HWND captured at SelectApp. GetRunningList only refreshes metadata (border/title).
  // Never replace a still-valid SelectApp HWND with FindByNameTitle — browsers share chrome.exe /
  // "Google Chrome" across many windows and the first match is often a blank/black frame.
  #ifdef WINDOWS
  const bool hwndstillvalid = (scriptrecord_windowhandle && IsWindow((HWND)scriptrecord_windowhandle));
  #else
  const bool hwndstillvalid = (scriptrecord_windowhandle != NULL);
  #endif

  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      havelist = true;
      process = ActionScriptQA_FindRunningByHandle(applist, scriptrecord_windowhandle);
      if(!process && !hwndstillvalid)
        {
          process = ActionScriptQA_FindRunningByNameTitle(applist, scriptrecord_appname, scriptrecord_windowtitle);
          if(process && process->GetWindowHandle())
            {
              scriptrecord_windowhandle = process->GetWindowHandle();
              ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_TARGET_WINDOW_HANDLE_REFRESHED));
            }
        }
    }
   else
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_APPLICATION_GETRUNNINGLIST_FAILED));
    }

  void* hwnd = hwndstillvalid ? scriptrecord_windowhandle
                              : (process ? process->GetWindowHandle() : scriptrecord_windowhandle);
  if(!hwnd)
    {
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_TARGET_WINDOW_NO_LONGER_VALID));
      return false;
    }
  #ifdef WINDOWS
  if(!IsWindow((HWND)hwnd))
    {
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_TARGET_WINDOW_NO_LONGER_VALID));
      return false;
    }
  #endif

  GRPRECTINT winrect;
  int        border = 0;
  int        titleh = 0;

  if(process && process->GetWindowRect())
    {
      winrect = (*process->GetWindowRect());
      border  = process->GetWindowBorderWidth();
      titleh  = process->GetWindowTitleHeight();
    }

  GRPSCREEN* screen = GEN_GRPFACTORY.CreateScreen();
  if(!screen)
    {
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CREATESCREEN_FAILED));
      return false;
    }

  screen->SetHandle(hwnd);

  int clientw = 0;
  int clienth = 0;
  if(!screen->GetClientSize(clientw, clienth) || (clientw <= 0) || (clienth <= 0))
    {
      clientw = (int)winrect.GetWidth()  - (border * 2);
      clienth = (int)winrect.GetHeight() - titleh - border;
    }

  // CaptureContent uses client-area coordinates (same convention as Script_Lib_Screen).
  int ptclientx = 0;
  int ptclienty = 0;

  #ifdef WINDOWS
  {
    POINT pt;
    pt.x = screenx;
    pt.y = screeny;
    if(ScreenToClient((HWND)hwnd, &pt))
      {
        ptclientx = (int)pt.x;
        ptclienty = (int)pt.y;
      }
     else
      {
        ptclientx = screenx - (winrect.x1 + border);
        ptclienty = screeny - (winrect.y1 + titleh);
      }

    RECT outer;
    if(GetWindowRect((HWND)hwnd, &outer))
      {
        winrect.x1 = outer.left;
        winrect.y1 = outer.top;
        winrect.x2 = outer.right;
        winrect.y2 = outer.bottom;
      }
  }
  #else
  ptclientx = screenx - (winrect.x1 + border);
  ptclienty = screeny - (winrect.y1 + titleh);
  #endif

  if((clientw <= 0) || (clienth <= 0) ||
     (ptclientx < 0) || (ptclienty < 0) ||
     (ptclientx >= clientw) || (ptclienty >= clienth))
    {
      GEN_GRPFACTORY.DeleteScreen(screen);
      if(havelist) ActionScriptQA_FreeRunningList(applist);
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CLICK_OUTSIDE_CLIENT_AREA_IGNORED));
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
  if(havelist) ActionScriptQA_FreeRunningList(applist);

  if(!bitmap)
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CAPTURECONTENT_FAILED));
      return false;
    }

  XSTRING* prefix = APPFLOW_CFG.ScriptRecord_GetBitmapPrefix();
  XSTRING  bmpname;
  bmpname.Format(NL("%s%03d.png"), prefix ? prefix->Get() : NL("rec_"), (int)scriptrecord_nextbitmapindex);

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
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_FAILED_TO_SAVE_PNG_SUPPORT_NOT_COMP), xpathbmp.Get());
      #else
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_FAILED_TO_SAVE), xpathbmp.Get());
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
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT_CAPT_X_ASSERT_APPEARS),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth);
    }
   else
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK_CAPT_X_AT_CLIENT_LAYOUT),
                         (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth, ptclientx, ptclienty, layoutx, layouty);
    }

  if(!ScriptRecord_WriteScript())
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITEALLFILE_FAILED_FOR),
                         NL("Tests_Recorded.js"));
      return false;
    }

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
  XCHAR*   pref   = prefix && !prefix->IsEmpty() ? prefix->Get() : NL("rec_");
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

              int posrec = line->Find(NL("function Recorded_"), false);
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
                              if(look->Find(NL("function "), false) != XSTRING_NOTFOUND) break;

                              XSTRING value;
                              if(ScriptRecord_ParseQuotedAssign(look, NL("appname"), value))
                                {
                                  session->appname = value.Get();
                                  ScriptRecord_MakeAppKey(session->appname, session->appkey);
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, NL("apppath"), value))
                                {
                                  session->apppath = value.Get();
                                }
                              else if(ScriptRecord_ParseQuotedAssign(look, NL("windowtitle"), value))
                                {
                                  session->windowtitle = value.Get();
                                }
                              else
                                {
                                  int ivalue = 0;
                                  if(ScriptRecord_ParseIntAssign(look, NL("winx"), ivalue)) session->winx = ivalue;
                                  else if(ScriptRecord_ParseIntAssign(look, NL("winy"), ivalue)) session->winy = ivalue;
                                  else if(ScriptRecord_ParseIntAssign(look, NL("winw"), ivalue)) session->winw = ivalue;
                                  else if(ScriptRecord_ParseIntAssign(look, NL("winh"), ivalue)) session->winh = ivalue;
                                }

                              if(!session->appname.IsEmpty() && !session->apppath.IsEmpty() && !session->windowtitle.IsEmpty() &&
                                 (session->winw > 0) && (session->winh > 0))
                                break;
                            }

                          scriptrecord_sessions.Add(session);
                        }
                    }
                }

              if(mainstart < 0)
                {
                  if(line->Find(NL("function main("), false) != XSTRING_NOTFOUND)
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
                  (*renamed) = NL("function Recorded_001()");
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
              ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_EXISTING_MAIN_MIGRATED_TO_RECORDED));
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
              ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_APPENDING_TO_EXISTING_SCRIPT_KEPT_L), (int)scriptrecord_existinglines.GetSize(), (int)scriptrecord_sessions.GetSize());
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
      pattern.Format(NL("%s*.png"), pref);

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
      if(line->Find(NL("function EvidenceStart"), false) != XSTRING_NOTFOUND) return true;
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
      if(line->Find(NL("__qa_application_ready"), false) != XSTRING_NOTFOUND) return true;
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
  needle.Format(NL("function EnsureApplication_%s"), appkey.Get());

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
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_UpdateEnsureGeometryInExisting(XSTRING& appkey, int winx, int winy, int winw, int winh)
* @brief      Update winx/winy/winw/winh inside an existing EnsureApplication_<appkey>() kept for append.
* @ingroup
*
* @return     true if the Ensure function was found and updated (or geometry already matched).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_UpdateEnsureGeometryInExisting(XSTRING& appkey, int winx, int winy, int winw, int winh)
{
  if(appkey.IsEmpty()) return false;
  if((winw <= 0) || (winh <= 0)) return false;

  XSTRING needle;
  needle.Format(NL("function EnsureApplication_%s"), appkey.Get());

  bool found    = false;
  int  depth    = 0;
  bool seenopen = false;

  for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
    {
      XSTRING* line = scriptrecord_existinglines.Get(c);
      if(!line) continue;

      if(!found)
        {
          if(line->Find(needle.Get(), false) == XSTRING_NOTFOUND) continue;
          found    = true;
          depth    = 0;
          seenopen = false;
        }

      {
        XSTRING pattern;
        pattern.Format(NL("var %s"), NL("winx"));
        if(line->Find(pattern.Get(), false) != XSTRING_NOTFOUND) line->Format(NL("  var winx        = %d;"), winx);
        pattern.Format(NL("var %s"), NL("winy"));
        if(line->Find(pattern.Get(), false) != XSTRING_NOTFOUND) line->Format(NL("  var winy        = %d;"), winy);
        pattern.Format(NL("var %s"), NL("winw"));
        if(line->Find(pattern.Get(), false) != XSTRING_NOTFOUND) line->Format(NL("  var winw        = %d;"), winw);
        pattern.Format(NL("var %s"), NL("winh"));
        if(line->Find(pattern.Get(), false) != XSTRING_NOTFOUND) line->Format(NL("  var winh        = %d;"), winh);
      }

      for(XDWORD i=0; i<line->GetSize(); i++)
        {
          XCHAR ch = line->Get()[i];
          if(ch == _C('{'))
            {
              depth++;
              seenopen = true;
            }
           else if(ch == _C('}'))
            {
              depth--;
            }
        }

      if(found && seenopen && (depth <= 0)) break;
    }

  if(found)
    {
      ScriptRecord_Print(NL("[Script Rec] Updated EnsureApplication_%s geometry: %d,%d %dx%d"), appkey.Get(), winx, winy, winw, winh);
    }

  return found;
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
  pattern.Format(NL("var %s"), varname);
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
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_ParseIntAssign(XSTRING* line, XCHAR* varname, int& outvalue)
* @brief      Parse  var varname = 123;  from a script line
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_ParseIntAssign(XSTRING* line, XCHAR* varname, int& outvalue)
{
  outvalue = 0;
  if(!line || !varname) return false;

  XSTRING pattern;
  pattern.Format(NL("var %s"), varname);
  int pos = line->Find(pattern.Get(), false);
  if(pos == XSTRING_NOTFOUND) return false;

  int eq = line->FindCharacter(_C('='), (XDWORD)pos);
  if(eq < 0) return false;

  outvalue = line->ConvertToInt(eq + 1, NULL, false);
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

  if(appkey.IsEmpty()) appkey = NL("app");
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
  if(probe.Find(NL("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Compare(NL("edge.exe"), false) == 0) return true;
  if(probe.Compare(NL("edge"), false) == 0) return true;

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(NL("chrome"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("msedge"), false)  != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("firefox"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("\\edge\\"), false) != XSTRING_NOTFOUND) return true;
  if(probe.Find(NL("/edge/"), false) != XSTRING_NOTFOUND) return true;

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

  if(probe.Find(NL("msedge"), false) != XSTRING_NOTFOUND || probe.Compare(NL("edge.exe"), false) == 0)
    {
      windowtitle = NL("Microsoft Edge");
      return;
    }

  if(probe.Find(NL("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = NL("Mozilla Firefox");
      return;
    }

  probe = apppath.Get();
  probe.ToLowerCase();
  if(probe.Find(NL("msedge"), false) != XSTRING_NOTFOUND || probe.Find(NL("\\edge\\"), false) != XSTRING_NOTFOUND || probe.Find(NL("/edge/"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = NL("Microsoft Edge");
      return;
    }

  if(probe.Find(NL("firefox"), false) != XSTRING_NOTFOUND)
    {
      windowtitle = NL("Mozilla Firefox");
      return;
    }

  // Default browser family (chrome / chromium).
  windowtitle = NL("Google Chrome");
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
  line  = NL("  var ");
  line += varname;
  line += NL("     = \"");
  line += escaped;
  line += NL("\";");
  file.AddLine(line);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteEnsureApplicationForApp(...)
* @brief      Write EnsureApplication_<key>() _ launch once, focus, leave ready for tests
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteEnsureApplicationForApp(XFILETXT& file, XSTRING& appname, XPATH& apppath, XSTRING& windowtitle, XSTRING& appkey, int winx, int winy, int winw, int winh)
{
  XSTRING line;
  XPATH   pathscript = apppath;
  ScriptRecord_PathForScript(pathscript);

  bool isbrowser = ScriptRecord_IsBrowserApp(appname, apppath);
  bool hasgeometry = ((winw > 0) && (winh > 0));

  line.Format(NL("function EnsureApplication_%s()"), appkey.Get());
  file.AddLine(line);
  file.AddLine(NL("{"));
  ScriptRecord_AddJSVarString(file, NL("appname"), appname);
  {
    XSTRING pathstr = pathscript.Get();
    ScriptRecord_AddJSVarString(file, NL("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, NL("windowtitle"), windowtitle);
  if(isbrowser)
    {
      file.AddLine(NL("  // Browser detected: URL cannot be captured - edit this value before running."));
      file.AddLine(NL("  var url         = \"https://EDIT_ME/\";"));
    }
  line.Format(NL("  var winx        = %d;"), winx);
  file.AddLine(line);
  line.Format(NL("  var winy        = %d;"), winy);
  file.AddLine(line);
  line.Format(NL("  var winw        = %d;"), winw);
  file.AddLine(line);
  line.Format(NL("  var winh        = %d;"), winh);
  file.AddLine(line);
  file.AddLine(NL("  var key         = appname;"));
  file.AddLine(NL(""));
  file.AddLine(NL("  // Already prepared in this script run: restore geometry + focus."));
  file.AddLine(NL("  if(__qa_application_ready[key])"));
  file.AddLine(NL("    {"));
  if(hasgeometry)
    {
      file.AddLine(NL("      Screen_SetPosition(appname, windowtitle, winx, winy);"));
      file.AddLine(NL("      Screen_Resize(appname, windowtitle, winw, winh);"));
      file.AddLine(NL("      Sleep(200);"));
    }
  file.AddLine(NL("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("      Sleep(200);"));
  file.AddLine(NL("      return true;"));
  file.AddLine(NL("    }"));
  file.AddLine(NL(""));
  // Short probe only: do not burn __qa_timeout_ms before OpenURL/Exec.
  file.AddLine(NL("  var already = WaitWindow(appname, windowtitle, 2000);"));
  file.AddLine(NL("  if(already)"));
  file.AddLine(NL("    {"));
  file.AddLine(NL("      Console_Printf(\"[Tests] Already running: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(NL("    }"));
  file.AddLine(NL(""));
  if(isbrowser)
    {
      // Always navigate: a reused Chrome on the wrong page makes the sequence look random.
      file.AddLine(NL("  if(!OpenURL(url))"));
      file.AddLine(NL("    {"));
      file.AddLine(NL("      Console_Printf(\"[Tests] FAIL OpenURL %s\\n\", url);"));
      file.AddLine(NL("      return false;"));
      file.AddLine(NL("    }"));
      file.AddLine(NL(""));
      file.AddLine(NL("  if(!WaitWindow(appname, windowtitle, __qa_timeout_ms))"));
      file.AddLine(NL("    {"));
      file.AddLine(NL("      Console_Printf(\"[Tests] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(NL("      TerminateAplication(appname);"));
      file.AddLine(NL("      return false;"));
      file.AddLine(NL("    }"));
      file.AddLine(NL(""));
      file.AddLine(NL("  Console_Printf(\"[Tests] Opened URL %s\\n\", url);"));
      file.AddLine(NL("  Sleep(800);"));
    }
   else
    {
      file.AddLine(NL("  if(!already)"));
      file.AddLine(NL("    {"));
      file.AddLine(NL("      if(!ExecApplication(apppath))"));
      file.AddLine(NL("        {"));
      file.AddLine(NL("          Console_Printf(\"[Tests] FAIL ExecApplication %s\\n\", apppath);"));
      file.AddLine(NL("          return false;"));
      file.AddLine(NL("        }"));
      file.AddLine(NL(""));
      file.AddLine(NL("      if(!WaitWindow(appname, windowtitle, __qa_timeout_ms))"));
      file.AddLine(NL("        {"));
      file.AddLine(NL("          Console_Printf(\"[Tests] FAIL window not found: %s\\n\", windowtitle);"));
      file.AddLine(NL("          TerminateAplication(appname);"));
      file.AddLine(NL("          return false;"));
      file.AddLine(NL("        }"));
      file.AddLine(NL(""));
      file.AddLine(NL("      Console_Printf(\"[Tests] Started %s\\n\", apppath);"));
      file.AddLine(NL("    }"));
    }
  file.AddLine(NL(""));
  // Focus as soon as the window exists — before geometry / bitmap search / keys.
  file.AddLine(NL("  Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("  Sleep(200);"));
  file.AddLine(NL(""));
  if(hasgeometry)
    {
      file.AddLine(NL("  // Restore layout from SelectApp: outer position + client size."));
      file.AddLine(NL("  Screen_SetPosition(appname, windowtitle, winx, winy);"));
      file.AddLine(NL("  Screen_Resize(appname, windowtitle, winw, winh);"));
      file.AddLine(NL("  Sleep(200);"));
      file.AddLine(NL("  Console_Printf(\"[Tests] Geometry pos %d,%d client %dx%d\\n\", winx, winy, winw, winh);"));
    }
  file.AddLine(NL("  Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("  Sleep(300);"));
  file.AddLine(NL("  __qa_application_ready[key] = true;"));
  file.AddLine(NL("  Console_Printf(\"[Tests] Ready: %s / %s\\n\", appname, windowtitle);"));
  file.AddLine(NL("  return true;"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));

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
  file.AddLine(NL("var __qa_application_ready = {};"));
  file.AddLine(NL("// Global timeout (ms) for Screen_GetPosXY / WaitWindow / bitmap waits."));
  file.AddLine(NL("var __qa_timeout_ms = 8000;"));
  file.AddLine(NL(""));
  file.AddLine(NL("function WaitWindow(appname, windowtitle, timeoutms)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  var outx = { value: 0 };"));
  file.AddLine(NL("  var outy = { value: 0 };"));
  file.AddLine(NL("  if(timeoutms === \"\" || timeoutms === null || timeoutms === undefined) timeoutms = __qa_timeout_ms;"));
  file.AddLine(NL("  return (Screen_GetPosXY(appname, windowtitle, outx, outy, timeoutms) == 0);"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function ScriptBaseName(scriptname)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  var base = scriptname;"));
  file.AddLine(NL("  var dot  = -1;"));
  file.AddLine(NL("  if(base == \"\" || base == null || base == undefined) base = GetNameScript();"));
  file.AddLine(NL("  dot = base.indexOf(\".\");"));
  file.AddLine(NL("  if(dot >= 0) base = base.substring(0, dot);"));
  file.AddLine(NL("  return base;"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function ListTestFileName(scriptname)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  return ScriptBaseName(scriptname) + \"_ListTest.json\";"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function EvidenceFileName(scriptname)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  return ScriptBaseName(scriptname) + \"_\" + FileCSV_GetShortDateTime() + \".csv\";"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function EvidenceStart(scriptname)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  var dir  = GetPathScript() + \"evidences\";"));
  file.AddLine(NL("  var path = dir + \"\\\\\" + EvidenceFileName(scriptname);"));
  file.AddLine(NL("  MakeDir(dir);"));
  file.AddLine(NL("  if(!FileCSV_Create(path)) return false;"));
  file.AddLine(NL("  if(!FileCSV_SetHeader(\"ID\", \"Description\", \"Result\")) return false;"));
  file.AddLine(NL("  if(!FileCSV_Save()) return false;"));
  file.AddLine(NL("  return true;"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function EvidenceAdd(id, description, result)"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  var idstr = \"\";"));
  file.AddLine(NL("  if(id !== \"\" && id !== null && id !== undefined) idstr = \"\" + (id | 0);"));
  file.AddLine(NL("  return FileCSV_AddRecord(idstr, description, result);"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));
  file.AddLine(NL("function EvidenceEnd()"));
  file.AddLine(NL("{"));
  file.AddLine(NL("  return FileCSV_Close();"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));

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

  file.AddLine(NL("function main()"));
  file.AddLine(NL("{"));

  for(XDWORD c=0; c<scriptrecord_sessions.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_SESSION* session = scriptrecord_sessions.Get(c);
      if(!session) continue;

      if(!session->appkey.IsEmpty() && session->appkey.Compare(lastkey) != 0)
        {
          line.Format(NL("  if(!EnsureApplication_%s()) return;"), session->appkey.Get());
          file.AddLine(line);
          lastkey = session->appkey.Get();
        }

      line.Format(NL("  Recorded_%03d();"), (int)session->id);
      file.AddLine(line);
    }

  if(currentsession)
    {
      if(!currentsession->appkey.IsEmpty() && currentsession->appkey.Compare(lastkey) != 0)
        {
          line.Format(NL("  if(!EnsureApplication_%s()) return;"), currentsession->appkey.Get());
          file.AddLine(line);
        }

      line.Format(NL("  Recorded_%03d();"), (int)currentsession->id);
      file.AddLine(line);
    }

  file.AddLine(NL("}"));

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
  currentsession.winx        = scriptrecord_winx;
  currentsession.winy        = scriptrecord_winy;
  currentsession.winw        = scriptrecord_winw;
  currentsession.winh        = scriptrecord_winh;

  XFILETXT file;
  if(!file.Create(xpath))
    {
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_CANNOT_CREATE), xpath.Get());
      return false;
    }

  XSTRING line;
  bool    needhelpers = true;

  // ensurekeyswritten only dedupes EnsureApplication within THIS WriteScript pass.
  // Clearing each pass is required when appending Recorded_NNN: otherwise a prior
  // pass marks the app as "written" while kept lines no longer contain the function
  // (missing EnsureApplication_* at runtime).
  scriptrecord_ensurekeyswritten.DeleteContents();
  scriptrecord_ensurekeyswritten.DeleteAll();
  if(!scriptrecord_existinglines.GetSize())
    {
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
      if(!last || !last->IsEmpty()) file.AddLine(NL(""));

      if(needhelpers)
        {
          ScriptRecord_WriteCommonHelpers(file);
          scriptrecord_readymapwritten = true;
        }
       else if(!ScriptRecord_ExistingHasReadyMap() && !scriptrecord_readymapwritten)
        {
          file.AddLine(NL("var __qa_application_ready = {};"));
          file.AddLine(NL(""));
          scriptrecord_readymapwritten = true;
        }
    }
   else
    {
      file.AddLine(NL("// ----------------------------------------------------------------------------"));
      line.Format(NL("// %s - auto-generated by ActionScriptQA script recorder (F1)"), outname->Get());
      file.AddLine(line);
      file.AddLine(NL("// Evidence CSV: evidences/<GetNameScript()>_<datetime>.csv"));
      file.AddLine(NL("// Optional ListTest: <GetNameScript()>_ListTest.json (loaded if present)."));
      file.AddLine(NL("// Layout: assets/Tests/<scriptbase>/{graphics,evidences}/ + <scriptbase>.js"));
      file.AddLine(NL("// App launch: EnsureApplication_<app>() in main() before that app's tests."));
      file.AddLine(NL("// New recording sessions append function Recorded_NNN() and refresh the entry point."));
      file.AddLine(NL("// ----------------------------------------------------------------------------"));
      file.AddLine(NL(""));
      ScriptRecord_WriteCommonHelpers(file);
      scriptrecord_readymapwritten = true;
    }

  // EnsureApplication_<key> for every known app that does not already have one.
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITESCRIPT_ENSURING_APPLICATION_HE));
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

      ScriptRecord_WriteEnsureApplicationForApp(file, session->appname, session->apppath, session->windowtitle, session->appkey, session->winx, session->winy, session->winw, session->winh);

      XSTRING* keycopy = GEN_NEW XSTRING();
      if(keycopy)
        {
          (*keycopy) = session->appkey.Get();
          scriptrecord_ensurekeyswritten.Add(keycopy);
        }
    }

  if(!appkey.IsEmpty() && !ScriptRecord_ExistingHasEnsureAppKey(appkey))
    {
      ScriptRecord_WriteEnsureApplicationForApp(file, scriptrecord_appname, apppathscript, scriptrecord_windowtitle, appkey, scriptrecord_winx, scriptrecord_winy, scriptrecord_winw, scriptrecord_winh);

      XSTRING* keycopy = GEN_NEW XSTRING();
      if(keycopy)
        {
          (*keycopy) = appkey.Get();
          scriptrecord_ensurekeyswritten.Add(keycopy);
        }
    }

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITESCRIPT_WRITING_RECORDED_STEPS), (int)scriptrecord_sessionindex, (int)scriptrecord_steps.GetSize());
  line.Format(NL("function Recorded_%03d()"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  file.AddLine(NL("{"));

  file.AddLine(NL("  var scriptname  = GetNameScript();"));
  line.Format(NL("  var session     = \"Recorded_%03d\";"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  ScriptRecord_AddJSVarString(file, NL("appname"), scriptrecord_appname);
  {
    XSTRING pathstr = apppathscript.Get();
    ScriptRecord_AddJSVarString(file, NL("apppath"), pathstr);
  }
  ScriptRecord_AddJSVarString(file, NL("windowtitle"), scriptrecord_windowtitle);
  line.Format(NL("  var winx        = %d;"), scriptrecord_winx);
  file.AddLine(line);
  line.Format(NL("  var winy        = %d;"), scriptrecord_winy);
  file.AddLine(line);
  line.Format(NL("  var winw        = %d;"), scriptrecord_winw);
  file.AddLine(line);
  line.Format(NL("  var winh        = %d;"), scriptrecord_winh);
  file.AddLine(line);

  file.AddLine(NL("  var outx = { value: 0 };"));
  file.AddLine(NL("  var outy = { value: 0 };"));
  file.AddLine(NL("  var i = 0;"));
  file.AddLine(NL("  var status = 1;"));
  file.AddLine(NL("  var stepresult = \"PASS\";"));
  file.AddLine(NL("  var stepdesc = \"\";"));
  file.AddLine(NL("  var catalogpath = \"\";"));
  file.AddLine(NL("  var listloaded = false;"));
  file.AddLine(NL(""));
  file.AddLine(NL("  Console_Printf(\"[Tests] Start %s / %s\\n\", scriptname, session);"));
  file.AddLine(NL(""));
  file.AddLine(NL("  if(!EvidenceStart(scriptname))"));
  file.AddLine(NL("    {"));
  file.AddLine(NL("      Console_Printf(\"[Tests] FAIL EvidenceStart/FileCSV\\n\");"));
  file.AddLine(NL("      return;"));
  file.AddLine(NL("    }"));
  file.AddLine(NL("  Console_Printf(\"[Tests] Evidence CSV %s\\n\", FileCSV_GetPath());"));
  file.AddLine(NL(""));
  file.AddLine(NL("  catalogpath = GetPathScript() + ListTestFileName(scriptname);"));
  file.AddLine(NL("  if(IsItExists(catalogpath))"));
  file.AddLine(NL("    {"));
  file.AddLine(NL("      if(TraceTests_Load(catalogpath))"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          listloaded = true;"));
  file.AddLine(NL("          EvidenceAdd(\"\", \"TraceTests_Load\", \"PASS\");"));
  file.AddLine(NL("          Console_Printf(\"[Tests] ListTest loaded %s\\n\", catalogpath);"));
  file.AddLine(NL("        }"));
  file.AddLine(NL("       else"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          EvidenceAdd(\"\", \"TraceTests_Load\", \"FAIL\");"));
  file.AddLine(NL("          Console_Printf(\"[Tests] FAIL TraceTests_Load %s\\n\", catalogpath);"));
  file.AddLine(NL("        }"));
  file.AddLine(NL("    }"));
  file.AddLine(NL("   else"));
  file.AddLine(NL("    {"));
  file.AddLine(NL("      EvidenceAdd(\"\", \"TraceTests_Load\", \"SKIP\");"));
  file.AddLine(NL("      Console_Printf(\"[Tests] ListTest not found (optional) %s\\n\", catalogpath);"));
  file.AddLine(NL("    }"));
  file.AddLine(NL(""));
  file.AddLine(NL("  // Application is started/focused from main() via EnsureApplication_<app>() before this test."));
  if((scriptrecord_winw > 0) && (scriptrecord_winh > 0))
    {
      file.AddLine(NL("  // Re-apply geometry captured at SelectApp (safe if Ensure already did it)."));
      file.AddLine(NL("  Screen_SetPosition(appname, windowtitle, winx, winy);"));
      file.AddLine(NL("  Screen_Resize(appname, windowtitle, winw, winh);"));
      file.AddLine(NL("  Sleep(200);"));
    }
  file.AddLine(NL("  // Take focus before any GetPosXY / WaitBitmap / key or text input."));
  file.AddLine(NL("  Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("  Sleep(200);"));
  file.AddLine(NL("  // Tighter match reduces false positives on small placeholder / icon crops."));
  file.AddLine(NL("  Screen_SetBmpFindCFG(8, 25);"));
  file.AddLine(NL("  Sleep(300);"));
  file.AddLine(NL(""));

  file.AddLine(NL("  var actions = ["));
  for(XDWORD c=0; c<scriptrecord_steps.GetSize(); c++)
    {
      ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = scriptrecord_steps.Get(c);
      if(!step) continue;

      XSTRING escaped;
      XCHAR*  comma = (c + 1 < scriptrecord_steps.GetSize()) ? NL(",") : NL("");

      switch(step->type)
        {
          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(NL("    { type: \"text\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_KEY :
                ScriptRecord_EscapeForJS(step->text, escaped);
                line.Format(NL("    { type: \"key\", value: \"%s\" }%s"), escaped.Get(), comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_WAIT :
                line.Format(NL("    { type: \"wait\", bmp: \"%s\", timeoutms: %d, intervalms: %d }%s"),
                            step->bitmapname.Get(),
                            APPFLOW_CFG.ScriptRecord_GetWaitTimeoutMs(),
                            APPFLOW_CFG.ScriptRecord_GetWaitIntervalMs(),
                            comma);
                break;

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK :
          default :
                line.Format(NL("    { type: \"click\", bmp: \"%s\", x: %d, y: %d }%s"),
                            step->bitmapname.Get(), step->layoutx, step->layouty, comma);
                break;
        }

      file.AddLine(line);
    }
  file.AddLine(NL("  ];"));
  file.AddLine(NL(""));
  file.AddLine(NL("  for(i = 0; i < actions.length; i++)"));
  file.AddLine(NL("    {"));
  file.AddLine(NL("      Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("      Sleep(120);"));
  file.AddLine(NL("      stepresult = \"PASS\";"));
  file.AddLine(NL("      stepdesc = actions[i].type;"));
  file.AddLine(NL(""));
  file.AddLine(NL("      if(actions[i].type == \"click\")"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          var foundby = \"bmp\";"));
  file.AddLine(NL("          outx = { value: 0 };"));
  file.AddLine(NL("          outy = { value: 0 };"));
  file.AddLine(NL("          stepdesc = \"click \" + actions[i].bmp;"));
  file.AddLine(NL("          status = Screen_GetPosXY(appname, windowtitle, actions[i].bmp, outx, outy, __qa_timeout_ms);"));
  file.AddLine(NL("          var layoutwinx = { value: 0 };"));
  file.AddLine(NL("          var layoutwiny = { value: 0 };"));
  file.AddLine(NL("          var havewin = (Screen_GetPosXY(appname, windowtitle, layoutwinx, layoutwiny, __qa_timeout_ms) == 0);"));
  file.AddLine(NL("          // Reject false-positive bitmap hits far from the recorded layout anchor."));
  file.AddLine(NL("          if((status == 0) && havewin)"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              var expx = layoutwinx.value + actions[i].x;"));
  file.AddLine(NL("              var expy = layoutwiny.value + actions[i].y;"));
  file.AddLine(NL("              var dx = outx.value - expx;"));
  file.AddLine(NL("              var dy = outy.value - expy;"));
  file.AddLine(NL("              if(dx < 0) dx = -dx;"));
  file.AddLine(NL("              if(dy < 0) dy = -dy;"));
  file.AddLine(NL("              if((dx > 120) || (dy > 120))"));
  file.AddLine(NL("                {"));
  file.AddLine(NL("                  Console_Printf(\"[Tests] WARN %s bmp hit far from layout (dx=%d dy=%d at %d,%d) — using layout %d,%d\\n\","));
  file.AddLine(NL("                                actions[i].bmp, dx, dy, outx.value, outy.value, expx, expy);"));
  file.AddLine(NL("                  outx.value = expx;"));
  file.AddLine(NL("                  outy.value = expy;"));
  file.AddLine(NL("                  foundby = \"layout-sanity\";"));
  file.AddLine(NL("                }"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("          if(status != 0)"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              if(havewin)"));
  file.AddLine(NL("                {"));
  file.AddLine(NL("                  outx.value = layoutwinx.value + actions[i].x;"));
  file.AddLine(NL("                  outy.value = layoutwiny.value + actions[i].y;"));
  file.AddLine(NL("                  status = 0;"));
  file.AddLine(NL("                  foundby = \"layout\";"));
  file.AddLine(NL("                  Console_Printf(\"[Tests] WARN %s not found — layout fallback %d,%d\\n\", actions[i].bmp, outx.value, outy.value);"));
  file.AddLine(NL("                }"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("          if(status == 0)"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("              Sleep(80);"));
  file.AddLine(NL("              InpSim_Mouse_Click(outx.value, outy.value);"));
  file.AddLine(NL("              Console_Printf(\"[Tests] Click %s at %d,%d (%s)\\n\", actions[i].bmp, outx.value, outy.value, foundby);"));
  file.AddLine(NL("              Sleep(600);"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("           else"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              stepresult = \"FAIL\";"));
  file.AddLine(NL("              Console_Printf(\"[Tests] FAIL find %s\\n\", actions[i].bmp);"));
  file.AddLine(NL("              EvidenceAdd(i + 1, stepdesc, stepresult);"));
  file.AddLine(NL("              if(listloaded) TraceTests_DeleteAll();"));
  file.AddLine(NL("              EvidenceEnd();"));
  file.AddLine(NL("              Console_Printf(\"[Tests] FAIL %s / %s (Screen_GetPosXY)\\n\", scriptname, session);"));
  file.AddLine(NL("              return;"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("        }"));
  file.AddLine(NL("      else if(actions[i].type == \"wait\")"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          stepdesc = \"wait \" + actions[i].bmp;"));
  file.AddLine(NL("          if(Screen_WaitBitmap(appname, windowtitle, actions[i].bmp, __qa_timeout_ms, actions[i].intervalms))"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              Console_Printf(\"[Tests] PASS wait %s\\n\", actions[i].bmp);"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("           else"));
  file.AddLine(NL("            {"));
  file.AddLine(NL("              stepresult = \"FAIL\";"));
  file.AddLine(NL("              Console_Printf(\"[Tests] FAIL wait %s (timeout)\\n\", actions[i].bmp);"));
  file.AddLine(NL("              EvidenceAdd(i + 1, stepdesc, stepresult);"));
  file.AddLine(NL("              if(listloaded) TraceTests_DeleteAll();"));
  file.AddLine(NL("              EvidenceEnd();"));
  file.AddLine(NL("              Console_Printf(\"[Tests] FAIL %s / %s (Screen_WaitBitmap)\\n\", scriptname, session);"));
  file.AddLine(NL("              return;"));
  file.AddLine(NL("            }"));
  file.AddLine(NL("        }"));
  file.AddLine(NL("      else if(actions[i].type == \"text\")"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          stepdesc = \"text\";"));
  file.AddLine(NL("          Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("          Sleep(150);"));
  file.AddLine(NL("          InpSim_Key_ClickByText(actions[i].value, 40);"));
  file.AddLine(NL("          Console_Printf(\"[Tests] Type text (%d chars)\\n\", actions[i].value.length);"));
  file.AddLine(NL("          Sleep(250);"));
  file.AddLine(NL("        }"));
  file.AddLine(NL("      else if(actions[i].type == \"key\")"));
  file.AddLine(NL("        {"));
  file.AddLine(NL("          stepdesc = \"key \" + actions[i].value;"));
  file.AddLine(NL("          Screen_SetFocus(appname, windowtitle);"));
  file.AddLine(NL("          Sleep(100);"));
  file.AddLine(NL("          InpSim_Key_ClickByLiteral(actions[i].value, 40);"));
  file.AddLine(NL("          Console_Printf(\"[Tests] Key %s\\n\", actions[i].value);"));
  file.AddLine(NL("          Sleep(200);"));
  file.AddLine(NL("        }"));
  file.AddLine(NL(""));
  file.AddLine(NL("      EvidenceAdd(i + 1, stepdesc, stepresult);"));
  file.AddLine(NL("    }"));
  file.AddLine(NL(""));
  file.AddLine(NL("  if(listloaded) TraceTests_DeleteAll();"));
  file.AddLine(NL("  EvidenceEnd();"));
  file.AddLine(NL("  Console_Printf(\"[Tests] End %s / %s\\n\", scriptname, session);"));
  file.AddLine(NL("}"));
  file.AddLine(NL(""));

  ScriptRecord_WriteMain(file, &currentsession);

  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITESCRIPT_FLUSHING_FILE_TO_DISK));
  if(!file.WriteAllFile())
    {
      file.Close();
      ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITEALLFILE_FAILED_FOR), xpath.Get());
      return false;
    }

  file.Close();
  ScriptRecord_Print(TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_SCRIPTRECORD_WRITESCRIPT_DONE));
  return true;
}

#pragma endregion
