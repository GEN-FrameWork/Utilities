/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       TranslateScan.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"
#include "TranslateScan_TranslateID.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "TranslateScan.h"

#include <stdio.h>
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

#include "HashMD5.h"

#include "DIOFactory.h"
#include "DIOStreamCipher.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOCoreProtocol_Connection.h"
#include "DIOWebClient_XEvent.h"
#include "DIOWebClient.h"

#include "DIOCheckTCPIPConnections.h"
#include "DIOCheckInternetConnection.h"

#include "DIOScraperWeb.h"
#include "DIOScraperWebPublicIP.h"
#include "DIOScraperWebGeolocationIP.h"
#include "DIOScraperWebUserAgentID.h"

#include "DIOCoreProtocol_ConnectionsManager_XEvent.h"

#include "APPFlowLog.h"
#include "APPFlowInternetServices.h"
#include "APPFlowExtended.h"

#include "TranslateScan_CFG.h"
#include "TranslateScan_Manager.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPLICATIONCREATEINSTANCE(TRANSLATESCAN, translatescan)



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN::TRANSLATESCAN()
* @brief      Constructor of class.
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN::TRANSLATESCAN() : XFSMACHINE(0)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN::~TRANSLATESCAN()
* @brief      Destructor of class.
* @note       VIRTUAL
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN::~TRANSLATESCAN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::InitFSMachine()
* @brief      Initialize the finite state machine.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::InitFSMachine()
{
  if(!AddState( TRANSLATESCAN_XFSMSTATE_NONE            ,
                TRANSLATESCAN_XFSMEVENT_INI             , TRANSLATESCAN_XFSMSTATE_INI             ,
                TRANSLATESCAN_XFSMEVENT_END             , TRANSLATESCAN_XFSMSTATE_END             ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( TRANSLATESCAN_XFSMSTATE_INI             ,
                TRANSLATESCAN_XFSMEVENT_UPDATE          , TRANSLATESCAN_XFSMSTATE_UPDATE          ,
                TRANSLATESCAN_XFSMEVENT_END             , TRANSLATESCAN_XFSMSTATE_END             ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( TRANSLATESCAN_XFSMSTATE_UPDATE          ,                
                TRANSLATESCAN_XFSMEVENT_END             , TRANSLATESCAN_XFSMSTATE_END             ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( TRANSLATESCAN_XFSMSTATE_END             ,
                TRANSLATESCAN_XFSMEVENT_NONE            , TRANSLATESCAN_XFSMSTATE_NONE            ,
                TRANSLATESCAN_XFSMEVENT_INI             , TRANSLATESCAN_XFSMSTATE_INI             ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::AppProc_Ini()
* @brief      Initialize the application process.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::AppProc_Ini()
{
  XSTRING string;
  XSTRING stringresult;
  XPATH   xpathsection;
  XPATH   xpath;
  bool    status = false;

  //-------------------------------------------------------------------------------------------------

  if(GetExecParams())
    {
      for(XDWORD c=0; c<GetExecParams()->GetSize(); c++)
        {
          XSTRING* param = (XSTRING*)GetExecParams()->Get(c);
          if(!param) continue;

          if(ParseAddLanguageParam(param))
            {
              hasexecmodifier = true;
              modifieraddlng  = true;
            }
          else if(IsExecParamModifier(param))
            {
              hasexecmodifier = true;

              if(!param->Compare(TRANSLATESCAN_EXECMODIFIER_GEN, false))
                {
                  modifiergen = true;
                }

              if(!param->Compare(TRANSLATESCAN_EXECMODIFIER_APP, false))
                {
                  modifierapp = true;
                }

              if(!param->Compare(TRANSLATESCAN_EXECMODIFIER_VIEW, false))
                {
                  modifierview = true;
                }
            }
          else if(operationdir.IsEmpty())
            {
              operationdir = param->Get();
            }
        }
    }

  if(operationdir.IsEmpty() && (hasexecmodifier || (GetExecParams() && GetExecParams()->GetSize())))
    {
      XDIR* xdir = GEN_XFACTORY.Create_Dir();
      if(xdir)
        {
          xdir->GetActual(operationdir);

          GEN_XFACTORY.Delete_Dir(xdir);
        }
    }

  Application_GetName()->AddFormat(APPLICATION_NAMEAPP); 

  GEN_SET_VERSION(Application_GetName()->Get(), APPLICATION_NAMEFILE, APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR, APPLICATION_OWNER, APPLICATION_YEAROFCREATION)
  
  //--------------------------------------------------------------------------------------------------

  ACTIVATEXTHREADGROUP(XTHREADGROUPID_SCHEDULER);
  ACTIVATEXTHREADGROUP(XTHREADGROUPID_DIOSTREAM);

  //--------------------------------------------------------------------------------------------------

  XTRACE_SETAPPLICATIONNAME((*Application_GetName()));
  XTRACE_SETAPPLICATIONVERSION(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
  XTRACE_SETAPPLICATIONID(string);

  //--------------------------------------------------------------------------------------------------

  GEN_XPATHSMANAGER.AdjustRootPathDefault(APPFLOW_DEFAULT_DIRECTORY_ROOT);

  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_SCRAPERS, APPFLOW_DEFAULT_DIRECTORY_SCRAPERS);

  GEN_XPATHSMANAGER.CreateAllPathSectionOnDisk();

  //--------------------------------------------------------------------------------------------------

  InitFSMachine();

  //--------------------------------------------------------------------------------------------------

  xmutexshowallstatus = GEN_XFACTORY.Create_Mutex();
  if(!xmutexshowallstatus) return false;

  //--------------------------------------------------------------------------------------------------

  APPFLOW_CFG_SETAUTOMATICTRACETARGETS

  //--------------------------------------------------------------------------------------------------
  
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathsection);
  xpath.Create(3 , xpathsection.Get(), APPLICATION_NAMEFILE, XTRANSLATION_NAMEFILEEXT);

  if(!GEN_XTRANSLATION.Ini(xpath))
    {
      //return false;
    }
  
  //--------------------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPStart(&APPFLOW_CFG, this);

  //--------------------------------------------------------------------------------------------------

  SetEvent(TRANSLATESCAN_XFSMEVENT_INI);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::AppProc_FirstUpdate()
* @brief      Execute the first application update callback.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::AppProc_FirstUpdate()
{
  XSTRING string;
  XSTRING stringresult;
  bool    status = false;

  //--------------------------------------------------------------------------------------------------

  xtimerupdateconsole = GEN_XFACTORY.CreateTimer();
  if(!xtimerupdateconsole) return false;
  
  //--------------------------------------------------------------------------------------------------

  Show_AllStatus();

  //--------------------------------------------------------------------------------------------------
  
  manager = GEN_NEW TRANSLATESCAN_MANAGER();
  if(!manager)
    {
      return false;
    }

  if(hasexecmodifier)
    {
      bool done = RunExecModifier();

      SetExitType(done ? APPFLOWBASE_EXITTYPE_BY_APPLICATION : APPFLOWBASE_EXITTYPE_BY_SERIOUSERROR);

      // Do not redraw the status screen: it would clear the modifier progress lines.
    }
  else
    {
      Show_AllStatus();
    }

  //--------------------------------------------------------------------------------------------------
  
  //console->PrintMessage(NL(" "), 0, false, true);
  //console->WaitKey(TL(TRANSLATESCAN_TRANSLATEID_TRANSLATESCAN_PULSA_UNA_TECLA_PARA_CONTINUAR), 1, false, 5);

  //--------------------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::AppProc_Update()
* @brief      Execute the application update callback.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::AppProc_Update()
{
  if(GetEvent()==TRANSLATESCAN_XFSMEVENT_NONE) // Not GEN_NEW event
    {
      switch(GetCurrentState())
        {
          case TRANSLATESCAN_XFSMSTATE_NONE       : break;

          case TRANSLATESCAN_XFSMSTATE_INI        : break;

          case TRANSLATESCAN_XFSMSTATE_UPDATE     : /*
                                                    if(GetExitType() == APPFLOWBASE_EXITTYPE_UNKNOWN)
                                                      {
                                                        if(xtimerupdateconsole)
                                                          {
                                                            if(xtimerupdateconsole->GetMeasureMilliSeconds() >= 100)
                                                              {
                                                                Show_AllStatus();
                                                                xtimerupdateconsole->Reset(); 
                                                              }

                                                            if(console->KBHit())
                                                              {
                                                                int key = console->GetChar();
                                                                KeyValidSecuences(key);
                                                              }
                                                          }
                                                      }
                                                   */              
                                                   break;

          case TRANSLATESCAN_XFSMSTATE_END       : SetExitType(APPFLOWBASE_EXITTYPE_BY_APPLICATION);
                                                   break;

        }
    }
   else //  New event
    {
      if(GetEvent()<TRANSLATESCAN_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case TRANSLATESCAN_XFSMSTATE_NONE   : break;

              case TRANSLATESCAN_XFSMSTATE_INI    : SetEvent(TRANSLATESCAN_XFSMEVENT_UPDATE);
                                                    break;

              case TRANSLATESCAN_XFSMSTATE_UPDATE : SetEvent(TRANSLATESCAN_XFSMEVENT_END);
                                                    break;

              case TRANSLATESCAN_XFSMSTATE_END    : break;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::AppProc_End()
* @brief      Finalize the application process.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::AppProc_End()
{
  XSTRING string;
  XSTRING stringresult;


  //--------------------------------------------------------------------------------------

  SetEvent(TRANSLATESCAN_XFSMEVENT_END);
  
  //--------------------------------------------------------------------------------------

  if(xmutexshowallstatus)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexshowallstatus);
      xmutexshowallstatus = NULL;
    }

  //--------------------------------------------------------------------------------------

  if(xtimerupdateconsole)
    {
      GEN_XFACTORY.DeleteTimer(xtimerupdateconsole);
      xtimerupdateconsole = NULL;
    }

  //--------------------------------------------------------------------------------------

  if(manager)
    {
      GEN_DELETE manager;
      manager = NULL;
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
* @fn         bool TRANSLATESCAN::KeyValidSecuences(int key)
* @brief      Process a keyboard sequence received by the console.
* @ingroup    APPLICATION
*
* @param[in]  key : key code received from the console.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::KeyValidSecuences(int key)
{
  XCHAR character = (XCHAR)key;

  if((character<32) || (character>127)) character = _C('?');
  APPFLOW_LOG_ENTRY(XLOGLEVEL_WARNING, APPFLOW_CFG_LOG_SECTIONID_STATUSAPP, false, TL(TRANSLATESCAN_TRANSLATEID_TRANSLATESCAN_KEY_PRESSED_0X), key, character);

  console->Printf(NL("\r    \r"));

  switch(key)
    {
      case 0x1B : // ESC Exit application
                  SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                  break;

      /*
      case 'T'  : { if(manager)
                      {
                        manager->Operation_Remark_Brief(operationdir);
                      }
                  }
                  break;   


      case 'Y'  : { if(manager)
                      {
                        manager->Operation_Remark_InGroup(operationdir);
                      }
                  }
                  break;   


      case 'R'  : { if(manager)
                      {
                        XSTRING brief_origin;
                        XSTRING brief_target;

                        brief_origin = NL("Sender_SMTPConfig");

                        manager->Operation_Remark_Brief_Construct(brief_origin, brief_target);
                      }
                  }
                  break;
      */

      case 'L'  : { if(manager)
                      {
                        manager->ModifierVIEW_Active(true);
                        manager->Operation_ViewLiteralStrings(operationdir);
                        manager->ModifierVIEW_Active(false);
                      }
                  }
                  break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_MANAGER* TRANSLATESCAN::GetManager()
* @brief      Get the Translate Scan manager.
* @ingroup    APPLICATION
*
* @return     TRANSLATESCAN_MANAGER* : pointer to the manager instance.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_MANAGER* TRANSLATESCAN::GetManager()
{
  return manager;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::Show_AllStatus()
* @brief      Show the current application status.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::Show_AllStatus()
{
  if(xmutexshowallstatus) xmutexshowallstatus->Lock();

  APPFLOW_EXTENDED.ShowAll();
  
  if(xmutexshowallstatus) xmutexshowallstatus->UnLock();

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TRANSLATESCAN::HandleEvent(XEVENT* xevent)
* @brief      Handle observer events received by the application.
* @ingroup    APPLICATION
*
* @param[in]  xevent : event to handle.
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TRANSLATESCAN::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;
  
  /*
  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_COREPROTOCOLCONNECTIONSMANAGER   : { DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT* event = (DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT*)xevent;
                                                            if(!event) return;

                                                            HandleEvent_CoreProtocolConnectionManager(event);
                                                          }
                                                          break;
    }
  */
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::ModifierGEN_IsActive()
* @brief      True when the GEN execution modifier was present.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::ModifierGEN_IsActive()
{
  return modifiergen;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::ModifierAPP_IsActive()
* @brief      True when the directory is an application and strings go to its catalogue.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::ModifierAPP_IsActive()
{
  return modifierapp;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::ModifierVIEW_IsActive()
* @brief      True when VIEW lists translation candidates without changing files.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::ModifierVIEW_IsActive()
{
  return modifierview;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::ModifierADDLNG_IsActive()
* @brief      True when ADDLNG=<codealpha2> was present.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::ModifierADDLNG_IsActive()
{
  return modifieraddlng;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* TRANSLATESCAN::ModifierADDLNG_GetCodeAlpha2()
* @brief      ISO 639-1 / alpha-2 code requested by ADDLNG.
* @ingroup    APPLICATION
*
* @return     XSTRING* : pointer to the code string.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* TRANSLATESCAN::ModifierADDLNG_GetCodeAlpha2()
{
  return &addlngcodealpha2;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::IsExecParamModifier(XSTRING* param)
* @brief      True when every character of the parameter is an uppercase letter.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @param[in]  param : parameter to test.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::IsExecParamModifier(XSTRING* param)
{
  if(!param)           return false;
  if(param->IsEmpty()) return false;

  XCHAR* text = param->Get();
  if(!text) return false;

  for(XDWORD c=0; c<param->GetSize(); c++)
    {
      if(!param->Character_IsUpperCase(text[c])) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::ParseAddLanguageParam(XSTRING* param)
* @brief      True when param is ADDLNG=<codealpha2> and the alpha-2 code is stored.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @param[in]  param : parameter to test.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::ParseAddLanguageParam(XSTRING* param)
{
  XSTRING prefix;
  XSTRING value;
  int     cut;

  if(!param)           return false;
  if(param->IsEmpty()) return false;

  cut = param->Find(NL("="), true);
  if(cut == XSTRING_NOTFOUND) return false;

  param->Copy(0, cut, prefix);
  param->Copy(cut + 1, value);

  if(prefix.Compare(TRANSLATESCAN_EXECMODIFIER_ADDLNG, false) != 0) return false;
  if(value.IsEmpty()) return false;

  value.ToLowerCase();
  addlngcodealpha2 = value;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN::RunExecModifier()
* @brief      Run the function of the active execution modifier.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN::RunExecModifier()
{
  if(!manager) return false;

  if(modifieraddlng)
    {
      if(addlngcodealpha2.IsEmpty())
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, NL("ADDLNG requires ADDLNG=<codealpha2> (example: ADDLNG=eu)."));
          return false;
        }

      if(modifierapp)
        {
          manager->ModifierAPP_Active(true);
          return manager->Operation_AddLanguageAPP(operationdir, addlngcodealpha2);
        }

      if(modifiergen)
        {
          manager->ModifierGEN_Active(true);
          return manager->Operation_AddLanguageGEN(operationdir, addlngcodealpha2);
        }

      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, NL("ADDLNG requires GEN or APP together with the working directory."));
      return false;
    }

  if(modifierapp)
    {
      manager->ModifierAPP_Active(true);

      return manager->Operation_TranslateAPP(operationdir);
    }

  if(modifiergen)
    {
      manager->ModifierGEN_Active(true);

      return manager->Operation_TranslateGENInternal(operationdir);
    }

  if(modifierview)
    {
      manager->ModifierVIEW_Active(true);

      return manager->Operation_ViewLiteralStrings(operationdir);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TRANSLATESCAN::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TRANSLATESCAN::Clean()
{
  xtimerupdateconsole         = NULL;
  xmutexshowallstatus         = NULL;

  manager                     = NULL;

  hasexecmodifier             = false;
  modifiergen                 = false;
  modifierapp                 = false;
  modifierview                = false;
  modifieraddlng              = false;
  addlngcodealpha2.Empty();
}



