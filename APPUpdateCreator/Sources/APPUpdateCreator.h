/**-------------------------------------------------------------------------------------------------------------------
*
* @file       APPUpdateCreator.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"

#include "XString.h"
#include "XScheduler.h"
#include "XDir.h"

#include "DIOStream.h"
#include "DIOURL.h"
#include "DIOApplicationUpdate_XEvent.h"
#include "DIOApplicationUpdate.h"

#include "APPFlowConsole.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum APPUPDATEXFSMEVENTS
{
  APPUPDATECREATOR_XFSMEVENT_NONE                     = 0 ,
  APPUPDATECREATOR_XFSMEVENT_INI                          ,
  APPUPDATECREATOR_XFSMEVENT_RUN                          ,
  APPUPDATECREATOR_XFSMEVENT_END                          ,

  APPUPDATECREATOR_LASTEVENT
};


enum APPUPDATEXFSMSTATES
{
  APPUPDATECREATOR_XFSMSTATE_NONE                     = 0 ,
  APPUPDATECREATOR_XFSMSTATE_INI                          ,
  APPUPDATECREATOR_XFSMSTATE_RUN                          ,
  APPUPDATECREATOR_XFSMSTATE_END                          ,

  APPUPDATECREATOR_LASTSTATE
};


#define APPLICATION_VERSION                       0
#define APPLICATION_SUBVERSION                    1
#define APPLICATION_SUBVERSIONERR                 0

#define APPLICATION_NAMEAPP                       _L("APP Update Creator")
#define APPLICATION_NAMEFILE                      _L("appupdatecreator")

#define APPLICATION_OWNER                         _L("EndoraSoft")

#define APPLICATION_YEAROFCREATION                2018


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSCHEDULER;

class APPUPDATECREATOR : public APPFLOWCONSOLE, public XFSMACHINE
{
  public:
                                      APPUPDATECREATOR                    ();
    virtual                          ~APPUPDATECREATOR                    ();

    bool                              InitFSMachine                       ();

    bool                              AppProc_Ini                         ();
    bool                              AppProc_FirstUpdate                 ();
    bool                              AppProc_Update                      ();
    bool                              AppProc_End                         ();

    bool                              GetApplicationParam                 (XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata);
    bool                              CreateListOfFiles                   (XPATH& xpath, XVECTOR<XPATH*>* updatefiles);
    bool                              GenerateUpdateFile                  (XPATH& xpath, DIOAPPLICATIONUPDATE_VERSIONDATA& applicationversiondata, XDWORD& nfiles, XDWORD& sizetotal);

  private:

    void                              HandleEvent                         (XEVENT* xevent);

    void                              Clean                               ();

    bool                              waitkeyatend;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/



