/**-------------------------------------------------------------------------------------------------------------------
*
* @file       APPUpdateCreator.cpp
*
* @class      APPUPDATECREATOR
* @brief      Application Update Creator class
* @ingroup    UTILS
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

#include "APPUpdateCreator.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "VersionFrameWork.h"

#include "XBase.h"
#include "XPath.h"
#include "XFactory.h"
#include "XRand.h"
#include "XDir.h"
#include "XString.h"
#include "XSystem.h"
#include "XLog.h"
#include "XConsole.h"
#include "XFileTXT.h"
#include "XFileINI.h"
#include "XScheduler.h"
#include "XTranslation.h"
#include "XTrace.h"
#include "XThread.h"

#include "DIOFactory.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamEnumDevices.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"
#include "DIONTP.h"
#include "DIOSMTP.h"
#include "DIOURL.h"
#include "DIOApplicationUpdate.h"
#include "DIOWebClient_XEvent.h"
#include "DIOWebClient.h"
#include "DIOCheckInternetConnection.h"
#include "DIOScraperWeb.h"
#include "DIOScraperWebPublicIP.h"
#include "DIOScraperWebGeolocationIP.h"
#include "DIOAlerts.h"

#include "HashCRC32.h"

#include "APPFlowLog.h"
#include "APPFlowExtended.h"

#include "APPUpdateCreator_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPLICATIONCREATEINSTANCE(APPUPDATECREATOR, appupdatecreator)

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         APPUPDATECREATOR::APPUPDATECREATOR()
* @brief      Constructor of class.
* @ingroup    UTILS
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
APPUPDATECREATOR::APPUPDATECREATOR():  XFSMACHINE(0)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         APPUPDATECREATOR::~APPUPDATECREATOR()
* @brief      Destructor of class.
* @note       VIRTUAL
* @ingroup    UTILS
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
APPUPDATECREATOR::~APPUPDATECREATOR()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::InitFSMachine()
* @brief      Initialize the finite state machine.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::InitFSMachine()
{
  if(!AddState( APPUPDATECREATOR_XFSMSTATE_NONE   ,
                APPUPDATECREATOR_XFSMEVENT_INI      , APPUPDATECREATOR_XFSMSTATE_INI          ,
                APPUPDATECREATOR_XFSMEVENT_END      , APPUPDATECREATOR_XFSMSTATE_END          ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( APPUPDATECREATOR_XFSMSTATE_INI      ,
                APPUPDATECREATOR_XFSMEVENT_RUN      , APPUPDATECREATOR_XFSMSTATE_RUN          ,
                APPUPDATECREATOR_XFSMEVENT_END      , APPUPDATECREATOR_XFSMSTATE_END          ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( APPUPDATECREATOR_XFSMSTATE_RUN      ,
                APPUPDATECREATOR_XFSMEVENT_END      , APPUPDATECREATOR_XFSMSTATE_END          ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( APPUPDATECREATOR_XFSMSTATE_END      ,
                APPUPDATECREATOR_XFSMEVENT_INI      , APPUPDATECREATOR_XFSMSTATE_INI          ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::AppProc_Ini()
* @brief      Initialize the application process.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::AppProc_Ini()
{
  XSTRING string;
  XSTRING stringresult;
  
  //-------------------------------------------------------------------------------------------------

  Application_GetName()->Set(APPLICATION_NAMEAPP);

 
  GEN_SET_VERSION(APPLICATION_NAMEAPP, APPLICATION_NAMEFILE, APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR, APPLICATION_OWNER, APPLICATION_YEAROFCREATION)
  Application_GetName()->Set(APPLICATION_NAMEAPP);

  XTRACE_SETAPPLICATIONNAME((*Application_GetName()));
  XTRACE_SETAPPLICATIONVERSION(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
  XTRACE_SETAPPLICATIONID(string);


  //--------------------------------------------------------------------------------------------------

  GEN_XPATHSMANAGER.AdjustRootPathDefault(APPFLOW_DEFAULT_DIRECTORY_ROOT);

  GEN_XPATHSMANAGER.CreateAllPathSectionOnDisk();

  //-------------------------------------------------------------------------------------------------

  InitFSMachine();  

  //--------------------------------------------------------------------------------------------------
 
  APPFLOW_CFG_SETAUTOMATICTRACETARGETS

  //--------------------------------------------------------------------------------------

  /*
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathsection);
  xpath.Create(3 , xpathsection.Get(), POS_VOMBE_LNG_NAMEFILE, XTRANSLATION_NAMEFILEEXT);

  if(!GEN_XTRANSLATION.Ini(xpath))
    {
      //return false;
    }
  */

  GEN_XTRANSLATION.SetActual( XLANGUAGE_ISO_639_3_CODE_SPA);

  //--------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPStart(&APPFLOW_CFG, this);

  //------------------------------------------------------------------------------------


  SetEvent(APPUPDATECREATOR_XFSMEVENT_INI);

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::AppProc_FirstUpdate()
* @brief      Execute the first application update callback.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::AppProc_FirstUpdate()
{
  waitkeyatend = true;

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::AppProc_Update()
* @brief      Execute the application update callback.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::AppProc_Update()
{
  if(GetEvent()==APPUPDATECREATOR_XFSMEVENT_NONE) // Not GEN_NEW event
    {
      switch(GetCurrentState())
        {
          case APPUPDATECREATOR_XFSMSTATE_INI       :
          case APPUPDATECREATOR_XFSMSTATE_RUN       :
          case APPUPDATECREATOR_XFSMSTATE_END       : break;

        }
    }
   else //  New event
    {
      if(GetEvent()<APPUPDATECREATOR_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case APPUPDATECREATOR_XFSMSTATE_INI   : { XPATH                             xpath;
                                                        DIOAPPLICATIONUPDATE_VERSIONDATA  applicationversiondata;
                                                        XDWORD                            nfiles;
                                                        XDWORD                            sizetotal;
                                                        bool                              status;

                                                        if(GetApplicationParam(xpath, applicationversiondata))
                                                          {
                                                            console->Printf(_L("   Version        : %d.\n"), applicationversiondata.GetVersion());
                                                            console->Printf(_L("   Subversion     : %d.\n"), applicationversiondata.GetSubVersion());
                                                            console->Printf(_L("   Error Control  : %d.\n"), applicationversiondata.GetSubVersionError());
                                                            console->Printf(_L("   Ini Sistema    : %s.\n"), applicationversiondata.SystemMustBeInit()?_L("Si"):_L("No"));
                                                            console->Printf(_L("\n"));
                                                            console->Printf(_L("   Creando %s en [%s] ...\n"), DIOAPPLICATIONUPDATE_INIFILE, xpath.Get());

                                                            status = GenerateUpdateFile(xpath, applicationversiondata, nfiles, sizetotal);

                                                            console->Printf(_L("   Fichero de actualizacion: %s\n"), status?_L("Ok."):_L("Error!"));
                                                            console->Printf(_L("   Numero de ficheros      : %d\n"), nfiles);
                                                            console->Printf(_L("   Tamano de actualizacion : %dk\n"), sizetotal /1024);
                                                          }
                                                         else
                                                          {
                                                            console->Printf(_L("   Error! Argumentos erroneos o invalidos.\n"));
                                                          }

                                                        console->Printf(_L("\n"));

                                                        SetExitType(APPFLOWBASE_EXITTYPE_BY_APPLICATION);
                                                        SetEvent(APPUPDATECREATOR_XFSMEVENT_END);
                                                      }
              case APPUPDATECREATOR_XFSMSTATE_RUN   : break;

              case APPUPDATECREATOR_XFSMSTATE_END   : break;
            }
        }
    }

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::AppProc_End()
* @brief      Finalize the application process.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::AppProc_End()
{
  XSTRING string;
  XSTRING stringresult;

  //--------------------------------------------------------------------------------------

  SetCurrentState(APPUPDATECREATOR_XFSMSTATE_END);

  //--------------------------------------------------------------------------------------

  if(waitkeyatend)
    {
      console->PrintMessage(_L(" "),0,false,true);
      console->WaitKey(_L("  Pulsa una tecla para continuar... (%d)"), 1, false, 5);
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
* @fn         bool APPUPDATECREATOR::GetApplicationParam(XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata)
* @brief      Read update parameters from the command line.
* @ingroup    UTILS
*
* @param[out]  xpath : application path obtained from the command line.
* @param[out]  applicationversiondata : version data obtained from the command line.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::GetApplicationParam(XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata)
{
  typedef struct
  {
    XCHAR* cmd;
    int    code;

  } COMANDINLINE;

  XSTRING       actualcmd;
  COMANDINLINE  commandinline[] = { { _L("path")     , 1 },
                                    { _L("version")  , 2 },
                                    { _L("subver")   , 3 },
                                    { _L("vererr")   , 4 },
                                    { _L("boot")     , 5 },
                                    { _L("nokey")    , 6 }
                                  };
  int           codecmd;
  XDWORD        c = 0;

  xpath.Empty();
  applicationversiondata.Clean();

  if(!GetExecParams()) return false;

  do{ codecmd = -1;

      if(!GetExecParams()->Get(c))        return false;
      if(!GetExecParams()->Get(c)->Get()) return false;

      actualcmd = GetExecParams()->Get(c)->Get();
      actualcmd.ToLowerCase();
      actualcmd.DeleteCharacter(_C(' '));

      if(actualcmd.Get()[0]==_C('-'))
        {
          actualcmd.DeleteCharacters(0, 1);

          for(int d=0; d<sizeof(commandinline)/sizeof(COMANDINLINE); d++)
            {
              if(!actualcmd.Compare(commandinline[d].cmd))
                {
                  codecmd = commandinline[d].code;
                  break;
                }
            }

          if(codecmd!=-1)
            {
              c++;

              switch(codecmd)
                {
                  case 1: xpath = GetExecParams()->Get(c)->Get();                                                     break;
                  case 2: applicationversiondata.SetVersion(GetExecParams()->Get(c)->ConvertToInt());                 break;
                  case 3: applicationversiondata.SetSubVersion(GetExecParams()->Get(c)->ConvertToInt());              break;
                  case 4: applicationversiondata.SetSubVersionError(GetExecParams()->Get(c)->ConvertToInt());         break;
                  case 5: applicationversiondata.SetSystemMustBeInit(GetExecParams()->Get(c)->ConvertToBoolean());    break;
                  case 6: waitkeyatend = false;                                                                       break;
                }

            } else return false;

        } else return false;

      c++;

    } while(c < GetExecParams()->GetSize());

  if(xpath.IsEmpty()) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::CreateListOfFiles(XPATH& xpath, XVECTOR<XPATH*>* updatefiles)
* @brief      Create the recursive list of files to include in the update package.
* @ingroup    UTILS
*
* @param[in]  xpath : root path to scan.
* @param[out]  updatefiles : list filled with the file paths found.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::CreateListOfFiles(XPATH& xpath, XVECTOR<XPATH*>* updatefiles)
{
  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir)  return false;

  XDIRELEMENT xdirelement;

  if(xdir->FirstSearch(xpath, _L("*") ,&xdirelement))
    {
      do{ if(xdirelement.GetNameFile()->Compare(DIOAPPLICATIONUPDATE_INIFILE, true))
            {
              XPATH* xpathnext = GEN_NEW XPATH();
              if(xpathnext)
                {
                  xpathnext->Set(xpath);
                  xpathnext->Slash_Add();
                  xpathnext->Add(xdirelement.GetNameFile()->Get());

                  if(xdirelement.GetType()==XDIRELEMENTTYPE_DIR)
                    {
                      CreateListOfFiles((*xpathnext), updatefiles);
                      GEN_DELETE xpathnext;
                    }
                   else updatefiles->Add(xpathnext);
                }
            }
        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR::GenerateUpdateFile(XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, XDWORD& nfiles, XDWORD& sizetotal)
* @brief      Generate the application update descriptor file.
* @ingroup    UTILS
*
* @param[in]  xpath : root path used to generate the update descriptor.
* @param[in]  applicationversiondata : version data written to the descriptor.
* @param[out]  nfiles : number of files included in the descriptor.
* @param[out]  sizetotal : total size of the included files.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR::GenerateUpdateFile(XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, XDWORD& nfiles, XDWORD& sizetotal)
{
  XVECTOR<XPATH*> updatefiles;
  XPATH           xpathupdatefile;
  bool            status = false;

  nfiles    = 0;
  sizetotal = 0;

  CreateListOfFiles(xpath, &updatefiles);

  xpathupdatefile = xpath;
  xpathupdatefile.Slash_Add();
  xpathupdatefile += DIOAPPLICATIONUPDATE_INIFILE;

  XFILEINI xfileini;
  if(xfileini.Create(xpathupdatefile))
    {
      XSTRING key;
      XSTRING value;

      value.ConvertFromInt(applicationversiondata.GetVersion());                  xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION            , value);
      value.ConvertFromInt(applicationversiondata.GetSubVersion());               xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION         , value);
      value.ConvertFromInt(applicationversiondata.GetSubVersionError());          xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL       , value);
      value.ConvertFromBoolean(applicationversiondata.SystemMustBeInit());        xfileini.WriteValue(DIOAPPLICATIONUPDATE_GENERALSECTION, DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT   , value);

      HASHCRC32 hashcrc32;

      for(XDWORD c=0; c<updatefiles.GetSize(); c++)
        {
          XPATH* xpathfile = updatefiles.Get(c);
          if(xpathfile)
            {
              XQWORD size = 0;

              hashcrc32.ResetResult();
              hashcrc32.Do((*xpathfile));

              XFILE* xfile = GEN_XFACTORY.Create_File();
              if(xfile)
                {
                  if(xfile->Open(xpathfile->Get()))
                    {
                      size = xfile->GetSize();
                      xfile->Close();
                    }

                  GEN_XFACTORY.Delete_File(xfile);
                }

              nfiles++;
              sizetotal += (XDWORD)size;

              XPATH* namefile = updatefiles.Get(c);
              if(namefile)
                {
                  namefile->DeleteCharacters(0, xpath.GetSize());
                  if((namefile->Get()[0] == _C('/')) || (namefile->Get()[0] == _C('\\'))) namefile->DeleteCharacters(0, 1);
                }

              key.Format(_L("%s%d"), DIOAPPLICATIONUPDATE_FILESSECTION_FILES, c);
              value.Format(_L("%s,%d,%08X"), namefile->Get(), (XDWORD)size, hashcrc32.GetResultCRC32());

              xfileini.WriteValue(DIOAPPLICATIONUPDATE_FILESSECTION, key, value);
            }
        }

      xfileini.Close();

      status =true;
    }

  updatefiles.DeleteContents();
  updatefiles.DeleteAll();

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void APPUPDATECREATOR::HandleEvent(XEVENT* xevent)
* @brief      Handle observer events received by the application.
* @note       INTERNAL
* @ingroup    UTILS
*
* @param[in]  xevent : event to handle.
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void APPUPDATECREATOR::HandleEvent(XEVENT* xevent)
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void APPUPDATECREATOR::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    UTILS
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void APPUPDATECREATOR::Clean()
{
  waitkeyatend    = false;
}

