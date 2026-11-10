/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_CFG.cpp
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_CFG.h"

#include "XLog.h"

#include "DIOStreamUARTConfig.h"

#include "XTraceMonitor.h"


#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XTRACEMONITOR_CFG* XTRACEMONITOR_CFG::instance = NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetIsInstanced()
* @brief      Checks if the singleton instance is created.
* @ingroup    UTILITIES
*
* @return     bool : true if the condition is met; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG& XTRACEMONITOR_CFG::GetInstance(bool ini)
* @brief      Gets the singleton instance.
* @ingroup    UTILITIES
*
* @param[in]  ini : true to initialize configuration from file; false to use the current values.
*
* @return     XTRACEMONITOR_CFG& : Reference to the requested object.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG& XTRACEMONITOR_CFG::GetInstance(bool ini)
{
  if(!instance) instance = GEN_NEW XTRACEMONITOR_CFG(ini?APPLICATION_NAMEFILE:NULL);

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DelInstance()
* @brief      Deletes the singleton instance.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DelInstance()
{
  if(instance)
    {
      delete instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DoVariableMapping()
* @brief      Maps configuration variables.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DoVariableMapping()
{
  if(!APPFLOWCFG::DoVariableMapping())
    {
      return false;
    }

  AddRemark(XTRACEMONITOR_CFG_SECTION_APPLICATION, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_APPLICATION, NL(" Application section of configuration"), 0, 2);

  AddValue(XFILECFG_VALUETYPE_INT, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_COLORTHEME, &colortheme,
           NL("Color theme (0=invalid, 1=Dark, 2=Light)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_SAVESCREENPOSITION, &savescreenposition,
           NL("Save main window position on exit"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_SAVESCREENSIZE, &savescreensize,
           NL("Save main window size on exit"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_SAVEDSCREENPOSITION, &savedscreenposition,
           NL("Last main window position (x, y)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_SAVEDSCREENSIZE, &savedscreensize,
           NL("Last main window size (width, height)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_UDPLISTENPORT, &udplistenport,
           NL("UDP listen port for incoming XTrace datagrams"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_APPLICATION, XTRACEMONITOR_CFG_UARTPORT, &uartport,
           NL("UART serial line (Port,Baudrate,DataBits,Parity,StopBits,Flow)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  // Filters options (XTraceMonitor_Form_Filters) — keys added here as the modal gains controls.
  AddRemark(XTRACEMONITOR_CFG_SECTION_FILTERS, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_FILTERS, NL(" Filters section of configuration"), 0, 2);

  AddRemark(XTRACEMONITOR_CFG_SECTION_CONSOLE, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_CONSOLE, NL(" Console line prefix options"), 0, 2);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_CONSOLE, XTRACEMONITOR_CFG_ADDDATETIMECONSOLE, &adddatetimeconsole,
           NL("Prefix each console line with date/time"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_CONSOLE, XTRACEMONITOR_CFG_ADDIPCONSOLE, &addipconsole,
           NL("Prefix each console line with source IP"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_CONSOLE, XTRACEMONITOR_CFG_TEXTTRACKING, &texttracking,
           NL("Follow / auto-scroll console text"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  AddRemark(XTRACEMONITOR_CFG_SECTION_DYNDNS, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_DYNDNS, NL(" DynDNS access (password stored AES+Base64)"), 0, 2);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_DYNDNS, XTRACEMONITOR_CFG_DYNDNS_ACTIVE, &dyndns_active,
           NL("Enable DynDNS toolbar and credentials"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_DYNDNS, XTRACEMONITOR_CFG_DYNDNS_LOGIN, &dyndns_login,
           NL("DynDNS login (empty when inactive)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_DYNDNS, XTRACEMONITOR_CFG_DYNDNS_PASSWORD, &dyndns_password,
           NL("DynDNS password AES+Base64 (empty when inactive)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DoDefault()
* @brief      Loads default configuration values.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DoDefault()
{
  if(!APPFLOWCFG::DoDefault())
    {
      return false;
    }

  //------------------------------------------------------------------------------

  GEN_XTRACE_NET_CFG_DEFAULT_01

  //------------------------------------------------------------------------------

  log_isactive                            = true;
  log_backupisactive                      = true;
  log_backupmaxfiles                      = 10;
  log_backupiscompress                    = true;

  log_activesectionsID.Empty();

  log_activesectionsID                  += APPFLOW_CFG_LOG_SECTIONID_INITIATION;
  log_activesectionsID                  += NL(",");
  log_activesectionsID                  += APPFLOW_CFG_LOG_SECTIONID_GENERIC;
  log_activesectionsID                  += NL(",");
  log_activesectionsID                  += APPFLOW_CFG_LOG_SECTIONID_STATUSAPP;
  log_activesectionsID                  += NL(",");
  log_activesectionsID                  += APPFLOW_CFG_LOG_SECTIONID_ENDING;

  log_levelmask                         = XLOGLEVEL_ALL;
  log_maxsize                           = 3000;
  log_reductionpercent                  = 10;

  //------------------------------------------------------------------------------

  colortheme                            = XTRACEMONITOR_CFG_COLORTHEME_DEFAULT;
  savescreenposition                    = XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT;
  savescreensize                        = XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT;
  SetSavedScreenPosition(0, 0);
  SetSavedScreenSize(XTRACEMONITOR_WINDOW_WIDTH, XTRACEMONITOR_WINDOW_HEIGHT);
  udplistenport                         = XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  uartport                              = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;

  adddatetimeconsole                    = XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT;
  addipconsole                          = XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT;
  texttracking                          = XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT;

  dyndns_active                         = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login.Empty();
  dyndns_password.Empty();

  //------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetColorTheme()
* @brief      Returns the persisted color theme integer (may be INVALID).
* @ingroup    UTILITIES
*
* @return     int : Color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetColorTheme()
{
  return colortheme;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetColorTheme(int colortheme)
* @brief      Stores the color theme integer (persisted on Save).
* @ingroup    UTILITIES
*
* @param[in]  colortheme : Color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetColorTheme(int colortheme)
{
  this->colortheme = colortheme;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsColorThemeValid(int colortheme)
* @brief      True when colortheme is a known non-invalid theme id.
* @ingroup    UTILITIES
*
* @param[in]  colortheme : Color theme value.
*
* @return     bool : true if valid; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsColorThemeValid(int colortheme)
{
  switch(colortheme)
    {
      case XTRACEMONITOR_CFG_COLORTHEME_DARK  :
      case XTRACEMONITOR_CFG_COLORTHEME_LIGHT : return true;
                                      default : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::ResolveColorTheme(int colortheme)
* @brief      Maps INVALID / unknown values to the factory default theme.
* @ingroup    UTILITIES
*
* @param[in]  colortheme : Color theme value.
*
* @return     int : Valid color theme value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::ResolveColorTheme(int colortheme)
{
  if(IsColorThemeValid(colortheme)) return colortheme;
  return XTRACEMONITOR_CFG_COLORTHEME_DEFAULT;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetSaveScreenPosition()
* @brief      True when the main window position should be persisted on exit.
* @ingroup    UTILITIES
*
* @return     bool : true if save-on-exit is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetSaveScreenPosition()
{
  return savescreenposition;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSaveScreenPosition(bool enable)
* @brief      Enables or disables persisting the main window position on exit.
* @ingroup    UTILITIES
*
* @param[in]  enable : true to save position on exit.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSaveScreenPosition(bool enable)
{
  savescreenposition = enable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetSaveScreenSize()
* @brief      True when the main window size should be persisted on exit.
* @ingroup    UTILITIES
*
* @return     bool : true if save-on-exit is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetSaveScreenSize()
{
  return savescreensize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSaveScreenSize(bool enable)
* @brief      Enables or disables persisting the main window size on exit.
* @ingroup    UTILITIES
*
* @param[in]  enable : true to save size on exit.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSaveScreenSize(bool enable)
{
  savescreensize = enable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetAddDateTimeConsole()
* @brief      True when console lines should be prefixed with date/time.
* @ingroup    UTILITIES
*
* @return     bool : true if date/time prefix is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetAddDateTimeConsole()
{
  return adddatetimeconsole;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetAddDateTimeConsole(bool enable)
* @brief      Enables or disables date/time prefix on console lines.
* @ingroup    UTILITIES
*
* @param[in]  enable : true to prefix date/time.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetAddDateTimeConsole(bool enable)
{
  adddatetimeconsole = enable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetAddIPConsole()
* @brief      True when console lines should be prefixed with source IP.
* @ingroup    UTILITIES
*
* @return     bool : true if IP prefix is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetAddIPConsole()
{
  return addipconsole;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetAddIPConsole(bool enable)
* @brief      Enables or disables source IP prefix on console lines.
* @ingroup    UTILITIES
*
* @param[in]  enable : true to prefix source IP.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetAddIPConsole(bool enable)
{
  addipconsole = enable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetTextTracking()
* @brief      True when console text tracking / follow is enabled.
* @ingroup    UTILITIES
*
* @return     bool : true if text tracking is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetTextTracking()
{
  return texttracking;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetTextTracking(bool enable)
* @brief      Enables or disables console text tracking / follow.
* @ingroup    UTILITIES
*
* @param[in]  enable : true to follow console text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetTextTracking(bool enable)
{
  texttracking = enable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetSavedScreenPosition()
* @brief      Raw "x, y" string for the last saved main window position.
* @ingroup    UTILITIES
*
* @return     XSTRING* : Saved position string.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetSavedScreenPosition()
{
  return &savedscreenposition;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSavedScreenPosition(int x, int y)
* @brief      Stores the main window position as "x, y".
* @ingroup    UTILITIES
*
* @param[in]  x : Desktop X.
* @param[in]  y : Desktop Y.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSavedScreenPosition(int x, int y)
{
  savedscreenposition.Format(NL("%d, %d"), x, y);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSavedScreenPosition(XCHAR* value)
* @brief      Stores the raw saved-position string.
* @ingroup    UTILITIES
*
* @param[in]  value : "x, y" text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSavedScreenPosition(XCHAR* value)
{
  if(value) savedscreenposition = value;
       else savedscreenposition.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::ParseSavedScreenPosition(int& x, int& y)
* @brief      Parses savedscreenposition into integers when the value is valid.
* @ingroup    UTILITIES
*
* @param[out] x : Desktop X.
* @param[out] y : Desktop Y.
*
* @return     bool : true if parsed successfully.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::ParseSavedScreenPosition(int& x, int& y)
{
  return ParseIntPair(&savedscreenposition, x, y);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetSavedScreenSize()
* @brief      Raw "width, height" string for the last saved main window size.
* @ingroup    UTILITIES
*
* @return     XSTRING* : Saved size string.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetSavedScreenSize()
{
  return &savedscreensize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSavedScreenSize(int width, int height)
* @brief      Stores the main window size as "width, height".
* @ingroup    UTILITIES
*
* @param[in]  width  : Client width.
* @param[in]  height : Client height.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSavedScreenSize(int width, int height)
{
  savedscreensize.Format(NL("%d, %d"), width, height);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetSavedScreenSize(XCHAR* value)
* @brief      Stores the raw saved-size string.
* @ingroup    UTILITIES
*
* @param[in]  value : "width, height" text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetSavedScreenSize(XCHAR* value)
{
  if(value) savedscreensize = value;
       else savedscreensize.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::ParseSavedScreenSize(int& width, int& height)
* @brief      Parses savedscreensize into integers when the value is valid (>= design minimum).
* @ingroup    UTILITIES
*
* @param[out] width  : Client width.
* @param[out] height : Client height.
*
* @return     bool : true if parsed successfully and size is usable.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::ParseSavedScreenSize(int& width, int& height)
{
  if(!ParseIntPair(&savedscreensize, width, height)) return false;
  if(width  < XTRACEMONITOR_WINDOW_WIDTH)  return false;
  if(height < XTRACEMONITOR_WINDOW_HEIGHT) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetUDPListenPort()
* @brief      Returns the configured UDP listen port (may be out of range until Resolve).
* @ingroup    UTILITIES
*
* @return     int : Port value.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetUDPListenPort()
{
  return udplistenport;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetUDPListenPort(int port)
* @brief      Sets the UDP listen port (clamped to 1..65535).
* @ingroup    UTILITIES
*
* @param[in]  port : Port value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetUDPListenPort(int port)
{
  udplistenport = ResolveUDPListenPort(port);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::ResolveUDPListenPort(int port)
* @brief      Clamps port into the valid TCP/UDP range; invalid -> default.
* @ingroup    UTILITIES
*
* @param[in]  port : Candidate port.
*
* @return     int : Usable port.
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::ResolveUDPListenPort(int port)
{
  if(port < XTRACEMONITOR_CFG_UDPLISTENPORT_MIN) return XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  if(port > XTRACEMONITOR_CFG_UDPLISTENPORT_MAX) return XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetUARTPort()
* @brief      Returns the UART serial-line configuration string.
* @ingroup    UTILITIES
*
* @return     XSTRING* : Pointer to uartport; NULL if unavailable.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetUARTPort()
{
  return &uartport;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetUARTPort(XCHAR* value)
* @brief      Sets the UART serial-line string (empty resolves to default).
* @ingroup    UTILITIES
*
* @param[in]  value : UART config string.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetUARTPort(XCHAR* value)
{
  if(value) uartport = value;
       else uartport.Empty();
  ResolveUARTPort(uartport);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetUARTPort(XSTRING* value)
* @brief      Sets the UART serial-line string (empty resolves to default).
* @ingroup    UTILITIES
*
* @param[in]  value : UART config string.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetUARTPort(XSTRING* value)
{
  if(value) uartport = (*value);
       else uartport.Empty();
  ResolveUARTPort(uartport);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::ResolveUARTPort(XSTRING& value)
* @brief      Ensures a non-empty UART line; field-level validation comes later.
* @ingroup    UTILITIES
*
* @param[in,out] value : Candidate UART config string.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::ResolveUARTPort(XSTRING& value)
{
  // Empty only — invalid non-empty strings stay visible so the field can show error state.
  if(value.IsEmpty()) value = XTRACEMONITOR_CFG_UARTPORT_DEFAULT;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsUARTPortValid(XCHAR* value)
* @brief      True when DIOSTREAMUARTCONFIG can decode the UART config string.
* @ingroup    UTILITIES
*
* @param[in]  value : Candidate UART line (Port,Baudrate,DataBits,Parity,StopBits,Flow).
*
* @return     bool : true if valid for DIOSTREAMUART.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsUARTPortValid(XCHAR* value)
{
  if(!value) return false;

  DIOSTREAMUARTCONFIG cfg;
  return cfg.SetFromString(value);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsUARTPortValid(XSTRING* value)
* @brief      True when DIOSTREAMUARTCONFIG can decode the UART config string.
* @ingroup    UTILITIES
*
* @param[in]  value : Candidate UART line.
*
* @return     bool : true if valid for DIOSTREAMUART.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsUARTPortValid(XSTRING* value)
{
  if(!value) return false;
  return IsUARTPortValid(value->Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsUARTPortValid(XSTRING& value)
* @brief      True when DIOSTREAMUARTCONFIG can decode the UART config string.
* @ingroup    UTILITIES
*
* @param[in]  value : Candidate UART line.
*
* @return     bool : true if valid for DIOSTREAMUART.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsUARTPortValid(XSTRING& value)
{
  return IsUARTPortValid(value.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetDynDNSActive()
* @brief      DynDNS enable flag from [dyndns] active.
* @ingroup    UTILITIES
*
* @return     bool : true when DynDNS is enabled.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetDynDNSActive()
{
  return dyndns_active;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetDynDNSActive(bool active)
* @brief      Sets DynDNS enable flag.
* @ingroup    UTILITIES
*
* @param[in]  active : Enable value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetDynDNSActive(bool active)
{
  dyndns_active = active;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetDynDNSLogin()
* @brief      DynDNS login string (empty when inactive on disk).
* @ingroup    UTILITIES
*
* @return     XSTRING* : Login pointer.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetDynDNSLogin()
{
  return &dyndns_login;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetDynDNSLogin(XCHAR* value)
* @brief      Sets DynDNS login.
* @ingroup    UTILITIES
*
* @param[in]  value : Login text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetDynDNSLogin(XCHAR* value)
{
  if(value) dyndns_login = value;
       else dyndns_login.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetDynDNSLogin(XSTRING* value)
* @brief      Sets DynDNS login.
* @ingroup    UTILITIES
*
* @param[in]  value : Login text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetDynDNSLogin(XSTRING* value)
{
  if(value) dyndns_login = (*value);
       else dyndns_login.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetDynDNSPassword()
* @brief      DynDNS password (AES+Base64 on disk; plaintext after Form decrypt).
* @ingroup    UTILITIES
*
* @return     XSTRING* : Password pointer.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetDynDNSPassword()
{
  return &dyndns_password;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetDynDNSPassword(XCHAR* value)
* @brief      Sets DynDNS password string (cipher or plain depending on caller).
* @ingroup    UTILITIES
*
* @param[in]  value : Password text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetDynDNSPassword(XCHAR* value)
{
  if(value) dyndns_password = value;
       else dyndns_password.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetDynDNSPassword(XSTRING* value)
* @brief      Sets DynDNS password string (cipher or plain depending on caller).
* @ingroup    UTILITIES
*
* @param[in]  value : Password text.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetDynDNSPassword(XSTRING* value)
{
  if(value) dyndns_password = (*value);
       else dyndns_password.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::ParseIntPair(XSTRING* value, int& a, int& b)
* @brief      Parses a "a, b" integer pair from a CFG string.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  value : Source string.
* @param[out] a : First integer.
* @param[out] b : Second integer.
*
* @return     bool : true if both integers were parsed.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::ParseIntPair(XSTRING* value, int& a, int& b)
{
  if(!value || value->IsEmpty()) return false;

  int comma = value->FindCharacter((XCHAR)',', 0);
  if(comma < 0) return false;

  XSTRING left;
  XSTRING right;
  value->Copy(0, comma, left);
  value->Copy(comma + 1, value->GetSize(), right);

  left.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_ALLSTRING);
  left.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_ALLSTRING);
  right.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_ALLSTRING);
  right.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_ALLSTRING);
  if(left.IsEmpty() || right.IsEmpty()) return false;

  // ConvertToInt validates against integer characters (optional leading '-').
  XSTRING digitchars;
  digitchars = XSTRING_VALIDCHARSINT;
  if(!left.AreValidCharacters(digitchars.Get()))  return false;
  if(!right.AreValidCharacters(digitchars.Get())) return false;

  a = left.ConvertToInt(0, NULL, false);
  b = right.ConvertToInt(0, NULL, false);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG::XTRACEMONITOR_CFG(XCHAR* namefile)
* @brief      Constructor.
* @ingroup    UTILITIES
*
* @param[in]  namefile : Configuration file name.
*
* @return     Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG::XTRACEMONITOR_CFG(XCHAR* namefile) : APPFLOWCFG(namefile)
{
  Clean();

  if(namefile)
    {
      Ini<XTRACEMONITOR_CFG>();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG::~XTRACEMONITOR_CFG()
* @brief      Destructor.
* @note       VIRTUAL
* @ingroup    UTILITIES
*
* @return     Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG::~XTRACEMONITOR_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Clean()
{
  colortheme           = XTRACEMONITOR_CFG_COLORTHEME_INVALID;
  savescreenposition   = XTRACEMONITOR_CFG_SAVESCREENPOSITION_DEFAULT;
  savescreensize       = XTRACEMONITOR_CFG_SAVESCREENSIZE_DEFAULT;
  savedscreenposition.Empty();
  savedscreensize.Empty();
  udplistenport        = XTRACEMONITOR_CFG_UDPLISTENPORT_DEFAULT;
  uartport.Empty();
  adddatetimeconsole   = XTRACEMONITOR_CFG_ADDDATETIMECONSOLE_DEFAULT;
  addipconsole         = XTRACEMONITOR_CFG_ADDIPCONSOLE_DEFAULT;
  texttracking         = XTRACEMONITOR_CFG_TEXTTRACKING_DEFAULT;
  dyndns_active        = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login.Empty();
  dyndns_password.Empty();
}
