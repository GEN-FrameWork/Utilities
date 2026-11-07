/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_CFG.h
*
* @class      XTRACEMONITOR_FORM_CFG
* @brief      Configuracion modal: draft state, radios, Accept / Cancel / Predeterminado
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

enum XTRACEMONITOR_FORM_CFG_COLORTHEME
{
  XTRACEMONITOR_FORM_CFG_COLORTHEME_INVALID = 0 ,
  XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK        ,
  XTRACEMONITOR_FORM_CFG_COLORTHEME_LIGHT       ,
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT;
class UI_ELEMENT_FORM;
class UI_ELEMENT_RADIOGROUP;
class UI_ELEMENT_RADIOBUTTON;

class XTRACEMONITOR_FORM_CFG
{
  public:
                                    XTRACEMONITOR_FORM_CFG                  ();
    virtual                        ~XTRACEMONITOR_FORM_CFG                  ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            ApplyGeometry                           ();
    bool                            Show                                    (bool show);
    bool                            IsVisible                               ();

    bool                            HandleElementSelected                   (UI_ELEMENT* element);

    int                             GetColorThemeCommitted                  ();
    int                             GetColorThemeDraft                      ();

  private:

    bool                            LoadCommittedFromCFG                    ();
    bool                            SyncRadiosFromDraft                     ();
    int                             ReadDraftFromRadios                     ();
    bool                            SetDraftColorTheme                      (int colortheme);
    bool                            OnAccept                                ();
    bool                            OnCancel                                ();
    bool                            OnDefault                               ();

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;

    UI_ELEMENT_FORM*                config_modal;
    UI_ELEMENT*                     config_modal_title;
    UI_ELEMENT*                     config_modal_btn_accept;
    UI_ELEMENT*                     config_modal_btn_cancel;
    UI_ELEMENT*                     config_modal_btn_default;
    UI_ELEMENT_RADIOGROUP*          color_theme_group;
    UI_ELEMENT_RADIOBUTTON*         theme_light;
    UI_ELEMENT_RADIOBUTTON*         theme_dark;

    int                             colortheme_committed;
    int                             colortheme_draft;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
