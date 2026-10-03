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

  SCRIPT::LoadScriptAndRun(APPFLOW_CFG.Scripts_GetAll(), AdjustLibraries);
                                                 
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
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
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
  console->Printf(outstring.Get());
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

  scriptrecord_sessionids.DeleteAll();

  scriptrecord_appselected      = false;
  scriptrecord_mousewasdown     = false;
  scriptrecord_windowhandle     = NULL;
  scriptrecord_sessionindex     = 1;
  scriptrecord_nextbitmapindex  = 1;
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
          ScriptRecord_FlushText();
          ScriptRecord_WriteScript();
          ScriptRecord_Print(__L("[ScriptRecord] Saved script and left record mode."));
        }
       else
        {
          ScriptRecord_Print(__L("[ScriptRecord] Left record mode (no app selected)."));
        }

      scriptrecord_active = false;
      ScriptRecord_Reset();
      return true;
    }

  ScriptRecord_Reset();
  scriptrecord_active = true;
  ScriptRecord_PrepareExisting();

  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  ScriptRecord_Print(__L("[ScriptRecord] ON (F1 again or ESC to finish)."));
  ScriptRecord_Print(__L("[ScriptRecord] 1) Left-click a window of the app under test."));
  ScriptRecord_Print(__L("[ScriptRecord] 2) Left-click UI targets to capture bitmaps."));
  ScriptRecord_Print(__L("[ScriptRecord] 3) Type text (login/password); ENTER/TAB/BACKSPACE recorded as keys."));
  if(outname) ScriptRecord_Print(__L("[ScriptRecord] Output script: %s"), outname->Get());
  ScriptRecord_Print(__L("[ScriptRecord] Session function: Recorded_%03d  (next bitmap %03d)"), (int)scriptrecord_sessionindex, (int)scriptrecord_nextbitmapindex);

  return true;
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

  bool down = ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);

  if(down && !scriptrecord_mousewasdown)
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
              ScriptRecord_CaptureClick(pt.x, pt.y);
            }
        }
    }

  scriptrecord_mousewasdown = down;

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
bool ACTIONSCRIPTQA::ScriptRecord_FlushText()
{
  if(scriptrecord_textpending.IsEmpty()) return true;

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_STEP();
  if(!step) return false;

  step->type = ACTIONSCRIPTQA_SCRIPTRECORD_STEP_TEXT;
  step->text = scriptrecord_textpending;
  scriptrecord_steps.Add(step);

  ScriptRecord_Print(__L("[ScriptRecord] Step %d: text \"%s\" (%d chars)"), (int)scriptrecord_steps.GetSize(), scriptrecord_textpending.Get(), (int)scriptrecord_textpending.GetSize());
  scriptrecord_textpending.Empty();
  ScriptRecord_WriteScript();

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

  XVECTOR<XPROCESS*> applist;
  XPROCESS*          found = NULL;

  if(!GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Application_GetRunningList failed."));
      return false;
    }

  for(XDWORD c=0; c<applist.GetSize(); c++)
    {
      XPROCESS* process = applist.Get(c);
      if(!process) continue;
      if(process->GetWindowHandle() == (void*)hwnd)
        {
          found = process;
          break;
        }
    }

  if(!found)
    {
      ScriptRecord_Print(__L("[ScriptRecord] Window not matched in running app list."));
      applist.DeleteContents();
      applist.DeleteAll();
      return false;
    }

  if(found->GetName() && (found->GetName()->Find(APPLICATION_NAMEFILE, true) != XSTRING_NOTFOUND))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Ignored click on ActionScriptQA itself."));
      applist.DeleteContents();
      applist.DeleteAll();
      return false;
    }

  scriptrecord_windowhandle = found->GetWindowHandle();
  scriptrecord_appname      = found->GetName() ? found->GetName()->Get() : __L("");
  scriptrecord_windowtitle  = found->GetWindowTitle() ? found->GetWindowTitle()->Get() : __L("");

  // GetPath() already holds the full executable path when available.
  if(found->GetPath() && !found->GetPath()->IsEmpty())
    {
      scriptrecord_apppath = found->GetPath()->Get();
    }
   else
    {
      scriptrecord_apppath = scriptrecord_appname.Get();
    }

  ScriptRecord_PathForScript(scriptrecord_apppath);

  scriptrecord_appselected = true;

  ScriptRecord_Print(__L("[ScriptRecord] App selected: %s"), scriptrecord_appname.Get());
  ScriptRecord_Print(__L("[ScriptRecord] Path: %s"), scriptrecord_apppath.Get());
  ScriptRecord_Print(__L("[ScriptRecord] Window: %s"), scriptrecord_windowtitle.Get());
  ScriptRecord_Print(__L("[ScriptRecord] Click UI controls to capture bitmaps..."));

  ScriptRecord_WriteScript();

  applist.DeleteContents();
  applist.DeleteAll();

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_CaptureClick(int screenx, int screeny)
* @brief      Capture bitmap around click and append a script step
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_CaptureClick(int screenx, int screeny)
{
  #ifndef WINDOWS
  return false;
  #else

  if(!scriptrecord_appselected) return false;

  ScriptRecord_FlushText();

  HWND hwnd = (HWND)scriptrecord_windowhandle;
  if(!hwnd || !IsWindow(hwnd))
    {
      // Re-resolve by title/name: some apps recreate the HWND after first interaction.
      XVECTOR<XPROCESS*> applist;
      HWND               foundhwnd = NULL;

      if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
        {
          for(XDWORD c=0; c<applist.GetSize(); c++)
            {
              XPROCESS* process = applist.Get(c);
              if(!process || !process->GetWindowHandle()) continue;

              bool namematch  = process->GetName() && !process->GetName()->Compare(scriptrecord_appname, true);
              bool titlematch = process->GetWindowTitle() && !process->GetWindowTitle()->Compare(scriptrecord_windowtitle, true);
              if(namematch || titlematch)
                {
                  foundhwnd = (HWND)process->GetWindowHandle();
                  if(foundhwnd && IsWindow(foundhwnd)) break;
                  foundhwnd = NULL;
                }
            }
        }

      applist.DeleteContents();
      applist.DeleteAll();

      if(!foundhwnd)
        {
          ScriptRecord_Print(__L("[ScriptRecord] Target window no longer valid."));
          return false;
        }

      scriptrecord_windowhandle = (void*)foundhwnd;
      hwnd = foundhwnd;
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
  bmpname.Format(__L("%s%03d.png"), prefix ? prefix->Get() : __L("rec_"), (int)(scriptrecord_nextbitmapindex + scriptrecord_steps.GetSize()));

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

  ACTIONSCRIPTQA_SCRIPTRECORD_STEP* step = GEN_NEW ACTIONSCRIPTQA_SCRIPTRECORD_STEP();
  if(!step) return false;

  step->type       = ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK;
  step->bitmapname = bmpname;
  step->layoutx    = layoutx;
  step->layouty    = layouty;
  scriptrecord_steps.Add(step);

  ScriptRecord_Print(__L("[ScriptRecord] Step %d: %s capt %dx%d at client %d,%d layout %d,%d"),
                     (int)scriptrecord_steps.GetSize(), bmpname.Get(), captw, capth, ptclient.x, ptclient.y, layoutx, layouty);
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
  scriptrecord_sessionids.DeleteAll();
  scriptrecord_sessionindex    = 1;
  scriptrecord_nextbitmapindex = 1;

  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  if(!outname || outname->IsEmpty()) return false;

  XPATH xpath;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS, xpath);
  xpath.Slash_Add();
  xpath += outname->Get();

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
                      scriptrecord_sessionids.Add((XDWORD)n);
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

              scriptrecord_sessionids.Add(1);
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
              ScriptRecord_Print(__L("[ScriptRecord] Appending to existing script (%d kept lines)."), (int)scriptrecord_existinglines.GetSize());
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
* @fn         bool ACTIONSCRIPTQA::ScriptRecord_WriteScript()
* @brief      Write / refresh the recorded .js under scripts/ (append session if file exists)
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA::ScriptRecord_WriteScript()
{
  XSTRING* outname = APPFLOW_CFG.ScriptRecord_GetOutputScript();
  if(!outname || outname->IsEmpty()) return false;

  XPATH xpath;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRIPTS, xpath);
  xpath.Slash_Add();
  xpath += outname->Get();

  XPATH apppathscript = scriptrecord_apppath;
  ScriptRecord_PathForScript(apppathscript);

  XFILETXT file;
  if(!file.Create(xpath))
    {
      ScriptRecord_Print(__L("[ScriptRecord] Cannot create %s"), xpath.Get());
      return false;
    }

  XSTRING line;

  if(scriptrecord_existinglines.GetSize())
    {
      for(XDWORD c=0; c<scriptrecord_existinglines.GetSize(); c++)
        {
          XSTRING* existing = scriptrecord_existinglines.Get(c);
          if(existing) file.AddLine(*existing);
        }

      // Ensure a blank separator before the new session function.
      XSTRING* last = scriptrecord_existinglines.Get(scriptrecord_existinglines.GetSize()-1);
      if(!last || !last->IsEmpty()) file.AddLine(__L(""));
    }
   else
    {
      file.AddLine(__L("// ----------------------------------------------------------------------------"));
      line.Format(__L("// %s - auto-generated by ActionScriptQA script recorder (F1)"), outname->Get());
      file.AddLine(line);
      file.AddLine(__L("// New recording sessions append function Recorded_NNN() and refresh the entry point."));
      file.AddLine(__L("// ----------------------------------------------------------------------------"));
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
    }

  line.Format(__L("function Recorded_%03d()"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  file.AddLine(__L("{"));

  line.Format(__L("  var scriptname  = \"%s\";"), outname->Get());
  file.AddLine(line);
  line.Format(__L("  var session     = \"Recorded_%03d\";"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  line.Format(__L("  var appname     = \"%s\";"), scriptrecord_appname.Get());
  file.AddLine(line);
  line.Format(__L("  var apppath     = \"%s\";"), apppathscript.Get());
  file.AddLine(line);
  line.Format(__L("  var windowtitle = \"%s\";"), scriptrecord_windowtitle.Get());
  file.AddLine(line);

  file.AddLine(__L("  var outx = { value: 0 };"));
  file.AddLine(__L("  var outy = { value: 0 };"));
  file.AddLine(__L("  var i = 0;"));
  file.AddLine(__L("  var status = 1;"));
  file.AddLine(__L(""));
  file.AddLine(__L("  Console_Printf(\"[%s] Start %s\\n\", scriptname, session);"));
  file.AddLine(__L(""));
  file.AddLine(__L("  if(!ExecApplication(apppath))"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Console_Printf(\"[%s] FAIL ExecApplication %s\\n\", scriptname, apppath);"));
  file.AddLine(__L("      return;"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  if(!WaitWindow(appname, windowtitle, 30000))"));
  file.AddLine(__L("    {"));
  file.AddLine(__L("      Console_Printf(\"[%s] FAIL window not found\\n\", scriptname);"));
  file.AddLine(__L("      TerminateAplication(appname);"));
  file.AddLine(__L("      return;"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
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

          case ACTIONSCRIPTQA_SCRIPTRECORD_STEP_CLICK :
          default :
                ScriptRecord_EscapeForJS(step->bitmapname, escaped);
                line.Format(__L("    { type: \"click\", bmp: \"%s\", x: %d, y: %d }%s"), escaped.Get(), step->layoutx, step->layouty, comma);
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
  file.AddLine(__L(""));
  file.AddLine(__L("      if(actions[i].type == \"click\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          outx = { value: 0 };"));
  file.AddLine(__L("          outy = { value: 0 };"));
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
  file.AddLine(__L("              Console_Printf(\"[%s] FAIL find %s\\n\", scriptname, actions[i].bmp);"));
  file.AddLine(__L("            }"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("      else if(actions[i].type == \"text\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          InpSim_Key_ClickByText(actions[i].value, 40);"));
  file.AddLine(__L("          Console_Printf(\"[%s] Type text (%d chars)\\n\", scriptname, actions[i].value.length);"));
  file.AddLine(__L("          Sleep(200);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("      else if(actions[i].type == \"key\")"));
  file.AddLine(__L("        {"));
  file.AddLine(__L("          InpSim_Key_ClickByLiteral(actions[i].value, 40);"));
  file.AddLine(__L("          Console_Printf(\"[%s] Key %s\\n\", scriptname, actions[i].value);"));
  file.AddLine(__L("          Sleep(200);"));
  file.AddLine(__L("        }"));
  file.AddLine(__L("    }"));
  file.AddLine(__L(""));
  file.AddLine(__L("  Console_Printf(\"[%s] End %s\\n\", scriptname, session);"));
  file.AddLine(__L("}"));
  file.AddLine(__L(""));

  file.AddLine(__L("function main()"));
  file.AddLine(__L("{"));
  for(XDWORD c=0; c<scriptrecord_sessionids.GetSize(); c++)
    {
      line.Format(__L("  Recorded_%03d();"), (int)scriptrecord_sessionids.Get(c));
      file.AddLine(line);
    }
  line.Format(__L("  Recorded_%03d();"), (int)scriptrecord_sessionindex);
  file.AddLine(line);
  file.AddLine(__L("}"));

  if(!file.WriteAllFile())
    {
      file.Close();
      ScriptRecord_Print(__L("[ScriptRecord] WriteAllFile failed for %s"), xpath.Get());
      return false;
    }

  file.Close();
  return true;
}

#pragma endregion
