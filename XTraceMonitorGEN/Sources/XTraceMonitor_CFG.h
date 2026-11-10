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

#include "XString.h"

#include "APPFlowCFG.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XTRACEMONITOR_CFG_SECTION_APPLICATION           NL("application")
#define XTRACEMONITOR_CFG_COLORTHEME                    NL("colortheme")
#define XTRACEMONITOR_CFG_SAVESCREENPOSITION            NL("savescreenposition")
#define XTRACEMONITOR_CFG_SAVESCREENSIZE                NL("savescreensize")
#define XTRACEMONITOR_CFG_SAVEDSCREENPOSITION           NL("savedscreenposition")
#define XTRACEMONITOR_CFG_SAVEDSCREENSIZE               NL("savedscreensize")
#define XTRACEMONITOR_CFG_UDPLISTENPORT                 NL("udpport")
#define XTRACEMONITOR_CFG_UARTPORT                      NL("uartport")

#define XTRACEMONITOR_CFG_SECTION_FILTERS               NL("filters")

#define XTRACEMONITOR_CFG_SECTION_CONSOLE               NL("console")
#define XTRACEMONITOR_CFG_ADDDATETIMECONSOLE            NL("adddatetimeconsole")
#define XTRACEMONITOR_CFG_ADDIPCONSOLE                  NL("addipconsole")
#define XTRACEMONITOR_CFG_TEXTTRACKING                  NL("texttracking")

#define XTRACEMONITOR_CFG_SECTION_DYNDNS                NL("dyndns")
#define XTRACEMONITOR_CFG_DYNDNS_ACTIVE                 NL("active")
#define XTRACEMONITOR_CFG_DYNDNS_LOGIN                  NL("login")
#define XTRACEMONITOR_CFG_DYNDNS_PASSWORD               NL("password")

#define XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT         false

#define XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT    false
#define XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT          false
#define XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT          false

// Persisted integer values for [application] colortheme (extensible: add 3, 4, … later).
#define XTRACEMONITOR_CFG_COLORTHEME_INVALID            0
#define XTRACEMONITOR_CFG_COLORTHEME_DARK               1
#define XTRACEMONITOR_CFG_COLORTHEME_LIGHT              2

#define XTRACEMONITOR_CFG_COLORTHEME_DEFAULT            XTRACEMONITOR_CFG_COLORTHEME_DARK

#define XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT    true
#define XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT        true

#define XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT         10001
#define XTRACEMONITOR_CFG_UDPLISTENPORT_MIN             1
#define XTRACEMONITOR_CFG_UDPLISTENPORT_MAX             65535

// UART serial line: Port,Baudrate,DataBits,Parity,StopBits,Flow
// (DIOSTREAMUARTCONFIG::SetFromString rejects port 0; use COM index >= 1).
#define XTRACEMONITOR_CFG_UARTPORT_DEFAULT              NL("1,115200,8,N,1,NONE")

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

    bool                            GetSaveScreenPosition                   ();
    void                            SetSaveScreenPosition                   (bool enable);

    bool                            GetSaveScreenSize                       ();
    void                            SetSaveScreenSize                       (bool enable);

    bool                            GetAddDateTimeConsole                   ();
    void                            SetAddDateTimeConsole                   (bool enable);

    bool                            GetAddIPConsole                         ();
    void                            SetAddIPConsole                         (bool enable);

    bool                            GetTextTracking                         ();
    void                            SetTextTracking                         (bool enable);

    XSTRING*                        GetSavedScreenPosition                  ();
    void                            SetSavedScreenPosition                  (int x, int y);
    void                            SetSavedScreenPosition                  (XCHAR* value);
    bool                            ParseSavedScreenPosition                (int& x, int& y);

    XSTRING*                        GetSavedScreenSize                      ();
    void                            SetSavedScreenSize                      (int width, int height);
    void                            SetSavedScreenSize                      (XCHAR* value);
    bool                            ParseSavedScreenSize                    (int& width, int& height);

    int                             GetUDPListenPort                        ();
    void                            SetUDPListenPort                        (int port);
    int                             ResolveUDPListenPort                    (int port);

    XSTRING*                        GetUARTPort                             ();
    void                            SetUARTPort                             (XCHAR* value);
    void                            SetUARTPort                             (XSTRING* value);
    void                            ResolveUARTPort                         (XSTRING& value);
    bool                            IsUARTPortValid                         (XCHAR* value);
    bool                            IsUARTPortValid                         (XSTRING* value);
    bool                            IsUARTPortValid                         (XSTRING& value);

    bool                            GetDynDNSActive                         ();
    void                            SetDynDNSActive                         (bool active);

    XSTRING*                        GetDynDNSLogin                          ();
    void                            SetDynDNSLogin                          (XCHAR* value);
    void                            SetDynDNSLogin                          (XSTRING* value);

    XSTRING*                        GetDynDNSPassword                       ();
    void                            SetDynDNSPassword                       (XCHAR* value);
    void                            SetDynDNSPassword                       (XSTRING* value);

  private:

                                    XTRACEMONITOR_CFG                       (XCHAR* namefile);
                                    XTRACEMONITOR_CFG                       (XTRACEMONITOR_CFG const&);
    virtual                        ~XTRACEMONITOR_CFG                       ();

    void                            operator =                              (XTRACEMONITOR_CFG const&);

    bool                            ParseIntPair                            (XSTRING* value, int& a, int& b);

    void                            Clean                                   ();

    static XTRACEMONITOR_CFG*       instance;

    int                             colortheme;
    bool                            savescreenposition;
    bool                            savescreensize;
    XSTRING                         savedscreenposition;
    XSTRING                         savedscreensize;
    int                             udplistenport;
    XSTRING                         uartport;

    bool                            adddatetimeconsole;
    bool                            addipconsole;
    bool                            texttracking;

    bool                            dyndns_active;
    XSTRING                         dyndns_login;
    XSTRING                         dyndns_password;   // ciphertext in INI; plaintext after Form decrypt load
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/
