/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_CFG.h
*
* @class      XTRACEMONITOR_FORM_CFG
* @brief      Configuracion modal: draft state, radios/checks, Accept / Cancel / Predeterminado
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

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XTRACEMONITOR_FORM_CFG_COLORTHEME
{
  XTRACEMONITOR_FORM_CFG_COLORTHEME_INVALID = 0 ,
  XTRACEMONITOR_FORM_CFG_COLORTHEME_DARK        ,
  XTRACEMONITOR_FORM_CFG_COLORTHEME_LIGHT       ,
};

// Symmetric passphrase for DynDNS password at-rest (AES-128). Change here if rotation is needed.
#define XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD            NL("XTM-DynDNS-AES128-Key!")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT;
class UI_ELEMENT_FORM;
class UI_ELEMENT_RADIOGROUP;
class UI_ELEMENT_RADIOBUTTON;
class UI_ELEMENT_CHECKBOX;
class UI_ELEMENT_EDITTEXT;
class UI_ELEMENT_MULTIOPTION;

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
    bool                            HandleElementChangeContents             (UI_ELEMENT* element);

    int                             GetColorThemeCommitted                  ();
    int                             GetColorThemeDraft                      ();

    // DynDNS password: AES-CBC + Base64 using XTRACEMONITOR_DYNDNS_CIPHER_PASSWORD.
    static bool                     EncryptDynDNSPassword                   (XSTRING& plain, XSTRING& out_b64);
    static bool                     DecryptDynDNSPassword                   (XSTRING& in_b64, XSTRING& out_plain);

  private:

    bool                            LoadCommittedFromCFG                    ();
    bool                            SyncRadiosFromDraft                     ();
    bool                            SyncChecksFromDraft                     ();
    bool                            SyncPortFromDraft                       ();
    bool                            SyncUARTFromDraft                       ();
    bool                            SyncDynDNSFromDraft                     ();
    int                             ReadDraftFromRadios                     ();
    bool                            ReadDraftFromChecks                     ();
    bool                            ReadDraftFromPort                       ();
    bool                            ReadDraftFromUART                       ();
    bool                            ReadDraftFromDynDNS                     ();
    bool                            ApplyUARTFieldVisual                    ();
    bool                            ApplyDynDNSFieldsEnabled                ();
    bool                            ApplyDynDNSToolbarButton                (bool active);
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
    UI_ELEMENT_CHECKBOX*            chk_savescreenposition;
    UI_ELEMENT_CHECKBOX*            chk_savescreensize;
    UI_ELEMENT_CHECKBOX*            chk_adddatetimeconsole;
    UI_ELEMENT_CHECKBOX*            chk_addipconsole;
    UI_ELEMENT_CHECKBOX*            chk_texttracking;
    UI_ELEMENT*                     lbl_udplistenport;
    UI_ELEMENT_EDITTEXT*            edit_udplistenport;
    UI_ELEMENT*                     lbl_uartport;
    UI_ELEMENT_EDITTEXT*            edit_uartport;

    UI_ELEMENT_RADIOGROUP*          dyndns_group;
    UI_ELEMENT_CHECKBOX*            chk_dyndns_active;
    UI_ELEMENT*                     lbl_dyndns_login;
    UI_ELEMENT_EDITTEXT*            edit_dyndns_login;
    UI_ELEMENT*                     lbl_dyndns_password;
    UI_ELEMENT_EDITTEXT*            edit_dyndns_password;
    UI_ELEMENT_MULTIOPTION*         dyndns_btn;

    int                             colortheme_committed;
    int                             colortheme_draft;

    bool                            savescreenposition_committed;
    bool                            savescreenposition_draft;
    bool                            savescreensize_committed;
    bool                            savescreensize_draft;
    bool                            adddatetimeconsole_committed;
    bool                            adddatetimeconsole_draft;
    bool                            addipconsole_committed;
    bool                            addipconsole_draft;
    bool                            texttracking_committed;
    bool                            texttracking_draft;

    XSTRING                         savedscreenposition_committed;
    XSTRING                         savedscreenposition_draft;
    XSTRING                         savedscreensize_committed;
    XSTRING                         savedscreensize_draft;

    int                             udplistenport_committed;
    int                             udplistenport_draft;

    XSTRING                         uartport_committed;
    XSTRING                         uartport_draft;
    bool                            uartport_valid;

    bool                            dyndns_active_committed;
    bool                            dyndns_active_draft;
    XSTRING                         dyndns_login_committed;
    XSTRING                         dyndns_login_draft;
    XSTRING                         dyndns_password_committed;   // plaintext in memory
    XSTRING                         dyndns_password_draft;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
