/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_Filters.h
*
* @class      XTRACEMONITOR_FORM_FILTERS
* @brief      Filters modal: draft/commit shell, Accept / Cancel / Predeterminado
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifndef XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS
#define XTRACEMONITOR_MODAL_BTN_PRESS_HOLD_MS           300
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT;
class UI_ELEMENT_FORM;
class XTIMER;

class XTRACEMONITOR_FORM_FILTERS
{
  public:
                                    XTRACEMONITOR_FORM_FILTERS              ();
    virtual                        ~XTRACEMONITOR_FORM_FILTERS              ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            ApplyGeometry                           ();
    bool                            Show                                    (bool show);
    bool                            IsVisible                               ();
    bool                            Tick                                    ();

    bool                            HandleElementSelected                   (UI_ELEMENT* element);

  private:

    bool                            LoadCommittedFromCFG                    ();
    bool                            OnAccept                                ();
    bool                            OnCancel                                ();
    bool                            OnDefault                               ();
    bool                            HoldPressVisual                         (UI_ELEMENT* btn);
    bool                            ArmCloseAfterPress                      (UI_ELEMENT* btn);

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;

    UI_ELEMENT_FORM*                filters_modal;
    UI_ELEMENT*                     filters_modal_title;
    UI_ELEMENT*                     filters_modal_btn_accept;
    UI_ELEMENT*                     filters_modal_btn_cancel;
    UI_ELEMENT*                     filters_modal_btn_default;
    XTIMER*                         close_timer;
    bool                            close_armed;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
