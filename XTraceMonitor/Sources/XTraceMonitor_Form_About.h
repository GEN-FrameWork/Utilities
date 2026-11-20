/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Form_About.h
*
* @class      XTRACEMONITOR_FORM_ABOUT
* @brief      About modal: program info; Accept closes. Opened from the GEN Framework brand icon.
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

#define XTRACEMONITOR_ABOUTMODAL_W                      520.0
#define XTRACEMONITOR_ABOUTMODAL_H                      490.0
#define XTRACEMONITOR_ABOUTMODAL_ICON_W                 96.0
#define XTRACEMONITOR_ABOUTMODAL_ICON_H                 96.0
#define XTRACEMONITOR_ABOUTMODAL_ICON_ALPHA             90
#define XTRACEMONITOR_ABOUTMODAL_GEN_W                  84.0
#define XTRACEMONITOR_ABOUTMODAL_GEN_H                  70.0
#define XTRACEMONITOR_ABOUTMODAL_GEN_ALPHA              95
/* Light blue matching brand/toolbar.png GEN mark on the main toolbar. */
#define XTRACEMONITOR_ABOUTMODAL_VERSION_R              102
#define XTRACEMONITOR_ABOUTMODAL_VERSION_G              153
#define XTRACEMONITOR_ABOUTMODAL_VERSION_B              255
#define XTRACEMONITOR_GENFRAMEWORK_URL_TEXT             NL("http://www.genframework.com")
#define XTRACEMONITOR_GENFRAMEWORK_URL_OPEN             NL("http://www.genframework.com/")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRACEMONITOR;
class UI_ELEMENT;
class UI_ELEMENT_FORM;
class UI_ELEMENT_TEXT;
class UI_ELEMENT_IMAGE;
class XTIMER;

class XTRACEMONITOR_FORM_ABOUT
{
  public:
                                    XTRACEMONITOR_FORM_ABOUT                ();
    virtual                        ~XTRACEMONITOR_FORM_ABOUT                ();

    bool                            SetOwner                                (XTRACEMONITOR* owner);

    bool                            Resolve                                 ();
    bool                            ApplyGeometry                           ();
    bool                            Show                                    (bool show);
    bool                            IsVisible                               ();
    bool                            Tick                                    ();
    bool                            ApplyTheme                              (bool light);

    bool                            HandleElementSelected                   (UI_ELEMENT* element);
    bool                            BrandToolbar_HitTest                    (int screen_x, int screen_y);
    bool                            GenUrl_HitTest                          (int screen_x, int screen_y);
    bool                            OpenGenFrameworkPage                    ();

  private:

    bool                            RefreshContent                          ();
    bool                            OnAccept                                ();
    bool                            HoldPressVisual                         (UI_ELEMENT* btn);
    bool                            ArmCloseAfterPress                      (UI_ELEMENT* btn);

    void                            Clean                                   ();

    XTRACEMONITOR*                  owner;

    UI_ELEMENT_FORM*                about_modal;
    UI_ELEMENT*                     about_modal_title;
    UI_ELEMENT*                     about_modal_btn_accept;
    UI_ELEMENT_IMAGE*               about_brand;
    UI_ELEMENT_IMAGE*               about_gen_dark;
    UI_ELEMENT_IMAGE*               about_gen_light;
    UI_ELEMENT_IMAGE*               brand_toolbar;
    UI_ELEMENT_TEXT*                about_app_name;
    UI_ELEMENT_TEXT*                about_version;
    UI_ELEMENT_TEXT*                about_gen_tagline;
    UI_ELEMENT_TEXT*                about_gen_url;
    UI_ELEMENT_TEXT*                about_copyright;

    XTIMER*                         close_timer;
    bool                            close_armed;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
