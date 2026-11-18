/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_Console.h
*
* @class      XTRACEMONITOR_GUI_CONSOLE
* @brief      Trace console panel (resolve, seed, layout)
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

#include "XBase.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT_CONSOLE;

class XTRACEMONITOR_GUI_CONSOLE
{
  public:
                                    XTRACEMONITOR_GUI_CONSOLE               ();
    virtual                        ~XTRACEMONITOR_GUI_CONSOLE               ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            SeedStartupLines                        ();
    bool                            Detach                                  ();

    bool                            ApplyLayoutGeometry                     (double panel_w, double console_h, double console_bot);
    bool                            SetToRedraw                             ();
    bool                            ApplyTextTracking                       ();

    UI_ELEMENT_CONSOLE*             GetElement                              ();

  private:

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;
    UI_ELEMENT_CONSOLE*             consoleelement;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
