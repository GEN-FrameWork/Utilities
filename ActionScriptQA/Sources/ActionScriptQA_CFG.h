/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA_CFG.h
* 
* @class      ACTIONSCRIPTQA_CFG
* @brief      Locomotive Operations for Kinetic Inspections (Configuration)
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
#pragma region INCLUDES

#include "APPFlowCFG.h"

#pragma endregion


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/
#pragma region DEFINES_ENUMS

#define ACTIONSCRIPTQACFG_SECTIONGENERAL          NL("general")

#define ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD    NL("scriptrecord")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_OUTPUT     NL("outputscript")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_BMPPREFIX  NL("bitmapprefix")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_CAPTUREW   NL("capturewidth")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_CAPTUREH   NL("captureheight")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_WAITTO     NL("waittimeoutms")
#define ACTIONSCRIPTQACFG_SCRIPTRECORD_WAITIV     NL("waitintervalms")

#pragma endregion


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/
#pragma region CLASS


class ACTIONSCRIPTQA_CFG : public APPFLOWCFG
{
  public:

    static bool                     GetIsInstanced                          ();
    static ACTIONSCRIPTQA_CFG&      GetInstance                             (bool ini = true);
    static bool                     DelInstance                             ();

    bool                            DoVariableMapping                       (); 
    bool                            DoDefault                               ();

    XSTRING*                        ScriptRecord_GetOutputScript            ();
    XSTRING*                        ScriptRecord_GetBitmapPrefix            ();
    int                             ScriptRecord_GetCaptureWidth            ();
    int                             ScriptRecord_GetCaptureHeight           ();
    int                             ScriptRecord_GetWaitTimeoutMs           ();
    int                             ScriptRecord_GetWaitIntervalMs          ();

  private:
                                    ACTIONSCRIPTQA_CFG                      (XCHAR* namefile);
                                    ACTIONSCRIPTQA_CFG                      (ACTIONSCRIPTQA_CFG const&);        // Don't implement
    virtual                        ~ACTIONSCRIPTQA_CFG                      ();

    void                            operator =                              (ACTIONSCRIPTQA_CFG const&);        // Don't implement

    void                            Clean                                   ();

    static ACTIONSCRIPTQA_CFG*      instance;

    XSTRING                         scriptrecord_outputscript;
    XSTRING                         scriptrecord_bitmapprefix;
    int                             scriptrecord_capturewidth;
    int                             scriptrecord_captureheight;
    int                             scriptrecord_waittimeoutms;
    int                             scriptrecord_waitintervalms;
};



#pragma endregion


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
#pragma region FUNCTIONS_PROTOTYPES


#pragma endregion
