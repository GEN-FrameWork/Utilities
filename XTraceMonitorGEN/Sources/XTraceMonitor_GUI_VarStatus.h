/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_VarStatus.h
*
* @class      XTRACEMONITOR_GUI_VARSTATUS
* @brief      Status-variable property list (valor): resolve, seed, splitter, layout
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
class UI_ELEMENT_PROPERTYLIST;

class XTRACEMONITOR_GUI_VARSTATUS
{
  public:
                                    XTRACEMONITOR_GUI_VARSTATUS             ();
    virtual                        ~XTRACEMONITOR_GUI_VARSTATUS             ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            Seed                                    ();
    bool                            Detach                                  ();

    bool                            UpdateSplitter                          ();
    bool                            ApplyLayoutGeometry                     (double panel_w, double valor_h, double valor_y);

    UI_ELEMENT_PROPERTYLIST*        GetElement                              ();

  private:

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;
    UI_ELEMENT_PROPERTYLIST*        valorelement;
    bool                            valor_prevpressed;
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
