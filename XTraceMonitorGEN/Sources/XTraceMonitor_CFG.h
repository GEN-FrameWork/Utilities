/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_CFG.h
*
* @class      XTRACEMONITOR_CFG
* @brief      GEN XTraceMonitor Config class
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
*---------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "APPFlowCFG.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XTRACEMONITOR_CFG_SECTION_APPLICATION           NL("application")
#define XTRACEMONITOR_CFG_COLORTHEME                    NL("colortheme")

// Persisted integer values for [application] colortheme (extensible: add 3, 4, … later).
#define XTRACEMONITOR_CFG_COLORTHEME_INVALID            0
#define XTRACEMONITOR_CFG_COLORTHEME_DARK               1
#define XTRACEMONITOR_CFG_COLORTHEME_LIGHT              2

#define XTRACEMONITOR_CFG_COLORTHEME_DEFAULT            XTRACEMONITOR_CFG_COLORTHEME_DARK

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR_CFG : public APPFLOWCFG
{
  public:

    static bool                     GetIsInstanced                          ();
    static XTRACEMONITOR_CFG&       GetInstance                             (bool ini = true);
    static bool                     DelInstance                             ();

    bool                            DoVariableMapping                       ();
    bool                            DoDefault                               ();

    int                             GetColorTheme                           ();
    void                            SetColorTheme                           (int colortheme);
    bool                            IsColorThemeValid                       (int colortheme);
    int                             ResolveColorTheme                       (int colortheme);

  private:

                                    XTRACEMONITOR_CFG                       (XCHAR* namefile);
                                    XTRACEMONITOR_CFG                       (XTRACEMONITOR_CFG const&);
    virtual                        ~XTRACEMONITOR_CFG                       ();

    void                            operator =                              (XTRACEMONITOR_CFG const&);

    void                            Clean                                   ();

    static XTRACEMONITOR_CFG*       instance;

    int                             colortheme;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
