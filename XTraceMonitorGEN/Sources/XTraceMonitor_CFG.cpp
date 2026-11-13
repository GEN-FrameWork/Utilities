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
* @fn         XTRACEMONITOR_CFG_TARGET::XTRACEMONITOR_CFG_TARGET()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG_TARGET::XTRACEMONITOR_CFG_TARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG_TARGET::~XTRACEMONITOR_CFG_TARGET()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG_TARGET::~XTRACEMONITOR_CFG_TARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG_TARGET::Clean()
* @brief      Clears target fields.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG_TARGET::Clean()
{
  type = XTRACEMONITOR_CFG_TARGETTYPE_UNKNOWN;
  name.Empty();
  host.Empty();
  port = 0;
  uartconfig.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG_ORIGIN::XTRACEMONITOR_CFG_ORIGIN()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG_ORIGIN::XTRACEMONITOR_CFG_ORIGIN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG_ORIGIN::~XTRACEMONITOR_CFG_ORIGIN()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG_ORIGIN::~XTRACEMONITOR_CFG_ORIGIN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG_ORIGIN::Clean()
* @brief      Clears origin fields.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG_ORIGIN::Clean()
{
  isactive = false;
  showip   = false;
  name.Empty();
  url.Empty();
  ip.Empty();
}


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

  AddRemark(XTRACEMONITOR_CFG_SECTION_FILTERS, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_FILTERS, NL(" Filters section of configuration"), 0, 2);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN, XTRACEMONITOR_CFG_SECTION_FILTERS, XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE, &filters_istextactive,
           NL("Enable console text substring filter"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING, XTRACEMONITOR_CFG_SECTION_FILTERS, XTRACEMONITOR_CFG_FILTERS_TEXTFILTER, &filters_text,
           NL("Text filter substring (used when istextfilteractive=yes)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT, XTRACEMONITOR_CFG_SECTION_FILTERS, XTRACEMONITOR_CFG_FILTERS_LEVELFILTER, &filters_levels,
           NL("XTrace level bitmask (bit0=Black … bit5=Gray; 255=all)"), APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

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

  AddRemark(XTRACEMONITOR_CFG_SECTION_TARGET, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_TARGET, NL(" Trace targets (legacy servers): type1=UDP host,port ; type2=UART (port,baud,...)"), 0, 2);
  AddValueSecuence<XSTRING>(XFILECFG_VALUETYPE_STRING,
                            XTRACEMONITOR_CFG_SECTION_TARGET,
                            XTRACEMONITOR_CFG_TARGET_ITEM,
                            XTRACEMONITOR_CFG_TARGET_ITEM_MASK,
                            XTRACEMONITOR_CFG_MINTARGETS,
                            XTRACEMONITOR_CFG_MAXTARGETS,
                            targets,
                            ntargets,
                            NL("Target item (type,name,...)"),
                            APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  AddRemark(XTRACEMONITOR_CFG_SECTION_ORIGINS, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(XTRACEMONITOR_CFG_SECTION_ORIGINS, NL(" Trace origins (legacy origins XX): isactive,showip,name,url,ip"), 0, 2);
  AddValueSecuence<XSTRING>(XFILECFG_VALUETYPE_STRING,
                            XTRACEMONITOR_CFG_SECTION_ORIGINS,
                            XTRACEMONITOR_CFG_ORIGIN_ITEM,
                            XTRACEMONITOR_CFG_ORIGIN_ITEM_MASK,
                            XTRACEMONITOR_CFG_MINORIGINS,
                            XTRACEMONITOR_CFG_MAXORIGINS,
                            origins,
                            norigins,
                            NL("Origin item (isactive,showip,name,url,ip)"),
                            APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SeedDefaultTargetsIfEmpty()
* @brief      Fills empty [target] slots with the legacy server conversion seed.
* @note       INTERNAL — used by DoDefault and first-run migration when the INI has no target lines yet.
* @ingroup    UTILITIES
*
* @return     bool : true if the seed was applied; false if slots already had data.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SeedDefaultTargetsIfEmpty()
{
  bool any = false;
  for(int i=0; i<ntargets; i++)
    {
      XSTRING* line = targets.Get(i);
      if(line && !line->IsEmpty()) { any = true; break; }
    }
  if(any) return false;

  static const XCHAR* seed[] =
    {
      NL("1,GEN FrameWork,xtracegen.dyndns.org,10001"),
      NL("1,EndoraSoft,xtraceendorasoft.dyndns.org,10101"),
      NL("1,IMT,xtraceimt.dyndns.org,10201"),
      NL("1,Bizintek,xtracebizintek.dyndns.org,10301"),
      NL("1,Lantek,xtracelantek.dyndns.org,10401"),
      NL("1,Symplio,xtracesymplio.dyndns.org,10501"),
      NL("2,UART STM32 WB55 0,(7,115200,8,N,1,NONE)"),
      NL("2,UART STM32 F0,(2,115200,8,N,1,NONE)"),
      NL("1,CAF,xtracecaf.dyndns.org,10601"),
      NL("1,IMT 2,xtraceimt.dyndns.org,10701"),
    };

  const int seed_n = (int)(sizeof(seed) / sizeof(seed[0]));
  for(int i=0; i<ntargets; i++)
    {
      XSTRING* line = targets.Get(i);
      if(!line) continue;
      if(i < seed_n) line->Set(seed[i]);
                else line->Empty();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SeedDefaultOriginsIfEmpty()
* @brief      Fills empty [origins] slots with the legacy origin conversion seed.
* @note       INTERNAL — used by DoDefault and first-run migration when the INI has no origin lines yet.
* @ingroup    UTILITIES
*
* @return     bool : true if the seed was applied; false if slots already had data.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SeedDefaultOriginsIfEmpty()
{
  bool any = false;
  for(int i=0; i<norigins; i++)
    {
      XSTRING* line = origins.Get(i);
      if(line && !line->IsEmpty()) { any = true; break; }
    }
  if(any) return false;

  // Legacy DoDefault origins[0]: Local / xdebug.kicks-ass.org (active + showip).
  static const XCHAR* seed[] =
    {
      NL("yes,yes,Local,xdebug.kicks-ass.org,"),
    };

  const int seed_n = (int)(sizeof(seed) / sizeof(seed[0]));
  for(int i=0; i<norigins; i++)
    {
      XSTRING* line = origins.Get(i);
      if(!line) continue;
      if(i < seed_n) line->Set(seed[i]);
                else line->Empty();
    }

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

  filters_istextactive                  = XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE_DEFAULT;
  filters_text.Empty();
  filters_levels                        = XTRACEMONITOR_CFG_FILTERS_LEVELFILTER_DEFAULT;

  dyndns_active                         = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login.Empty();
  dyndns_password.Empty();

  // Force legacy seed on factory defaults (clear first so SeedDefaultTargetsIfEmpty always applies).
  for(int i=0; i<ntargets; i++)
    {
      XSTRING* line = targets.Get(i);
      if(line) line->Empty();
    }
  SeedDefaultTargetsIfEmpty();

  for(int i=0; i<norigins; i++)
    {
      XSTRING* line = origins.Get(i);
      if(line) line->Empty();
    }
  SeedDefaultOriginsIfEmpty();

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
* @fn         bool XTRACEMONITOR_CFG::Filters_IsTextActive()
* @brief      True when the console text substring filter is enabled ([filters]).
* @ingroup    UTILITIES
*
* @return     bool : true if the text filter is active.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::Filters_IsTextActive()
{
  return filters_istextactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetIsTextActive(bool istextactive)
* @brief      Enables or disables the console text substring filter.
* @ingroup    UTILITIES
*
* @param[in]  istextactive : true to apply Filters_GetText() as a substring filter.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetIsTextActive(bool istextactive)
{
  filters_istextactive = istextactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::Filters_GetText()
* @brief      Substring used when Filters_IsTextActive() is true ([filters] textfilter).
* @ingroup    UTILITIES
*
* @return     XSTRING* : Filter text (may be empty).
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::Filters_GetText()
{
  return &filters_text;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetText(XCHAR* value)
* @brief      Sets the text-filter substring.
* @ingroup    UTILITIES
*
* @param[in]  value : Filter text (NULL clears).
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetText(XCHAR* value)
{
  if(value) filters_text.Set(value);
       else filters_text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetText(XSTRING* value)
* @brief      Sets the text-filter substring.
* @ingroup    UTILITIES
*
* @param[in]  value : Filter text (NULL clears).
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetText(XSTRING* value)
{
  if(value) filters_text = (*value);
       else filters_text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::Filters_GetLevels()
* @brief      XTrace level bitmask ([filters] levelfilter).
* @ingroup    UTILITIES
*
* @return     int : Level bits (see XTRACEMONITOR_CFG_FILTER_LEVEL_*).
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::Filters_GetLevels()
{
  return filters_levels;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetLevels(int levels)
* @brief      Sets the XTrace level bitmask persisted under [filters].
* @ingroup    UTILITIES
*
* @param[in]  levels : Level bits (0..255 typical; factory default 255=all).
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetLevels(int levels)
{
  filters_levels = levels;
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
* @fn         int XTRACEMONITOR_CFG::GetTargetCount()
* @brief      Number of mapped [target] item slots.
* @ingroup    UTILITIES
*
* @return     int : Slot count (ntargets).
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetTargetCount()
{
  return ntargets;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetTargetLine(int index)
* @brief      Raw CSV line for a target slot (0-based).
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
*
* @return     XSTRING* : Line pointer; NULL if out of range.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetTargetLine(int index)
{
  if(index < 0) return NULL;
  if(index >= ntargets) return NULL;
  return targets.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetTargetLine(int index, XCHAR* line)
* @brief      Sets the raw CSV line for a target slot.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  line : CSV text (may be NULL to clear).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetTargetLine(int index, XCHAR* line)
{
  XSTRING* slot = GetTargetLine(index);
  if(!slot) return false;
  if(line) slot->Set(line);
      else slot->Empty();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetTargetLine(int index, XSTRING* line)
* @brief      Sets the raw CSV line for a target slot.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  line : CSV text (may be NULL to clear).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetTargetLine(int index, XSTRING* line)
{
  return SetTargetLine(index, line ? line->Get() : NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetTarget(int index, XTRACEMONITOR_CFG_TARGET& out)
* @brief      Parses a target slot into typed fields.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[out] out : Typed target.
*
* @return     bool : true if parsed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetTarget(int index, XTRACEMONITOR_CFG_TARGET& out)
{
  return ParseTargetLine(GetTargetLine(index), out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetTarget(int index, const XTRACEMONITOR_CFG_TARGET& in)
* @brief      Serializes typed fields into a target slot line.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  in : Typed target.
*
* @return     bool : true if written; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetTarget(int index, const XTRACEMONITOR_CFG_TARGET& in)
{
  XSTRING line;
  if(!FormatTargetLine(in, line)) return false;
  return SetTargetLine(index, line.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetOriginCount()
* @brief      Number of mapped [origins] item slots.
* @ingroup    UTILITIES
*
* @return     int : Slot count (norigins).
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetOriginCount()
{
  return norigins;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetOriginLine(int index)
* @brief      Raw CSV line for an origin slot (0-based).
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
*
* @return     XSTRING* : Line pointer; NULL if out of range.
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetOriginLine(int index)
{
  if(index < 0) return NULL;
  if(index >= norigins) return NULL;
  return origins.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetOriginLine(int index, XCHAR* line)
* @brief      Sets the raw CSV line for an origin slot.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  line : CSV text (may be NULL to clear).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetOriginLine(int index, XCHAR* line)
{
  XSTRING* slot = GetOriginLine(index);
  if(!slot) return false;
  if(line) slot->Set(line);
      else slot->Empty();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetOriginLine(int index, XSTRING* line)
* @brief      Sets the raw CSV line for an origin slot.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  line : CSV text (may be NULL to clear).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetOriginLine(int index, XSTRING* line)
{
  return SetOriginLine(index, line ? line->Get() : NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::GetOrigin(int index, XTRACEMONITOR_CFG_ORIGIN& out)
* @brief      Parses an origin slot into typed fields.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[out] out : Typed origin.
*
* @return     bool : true if parsed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::GetOrigin(int index, XTRACEMONITOR_CFG_ORIGIN& out)
{
  return ParseOriginLine(GetOriginLine(index), out);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::SetOrigin(int index, const XTRACEMONITOR_CFG_ORIGIN& in)
* @brief      Serializes typed fields into an origin slot line.
* @ingroup    UTILITIES
*
* @param[in]  index : Slot index.
* @param[in]  in : Typed origin.
*
* @return     bool : true if written; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::SetOrigin(int index, const XTRACEMONITOR_CFG_ORIGIN& in)
{
  XSTRING line;
  if(!FormatOriginLine(in, line)) return false;
  return SetOriginLine(index, line.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::ParseTargetLine(XSTRING* line, XTRACEMONITOR_CFG_TARGET& out)
* @brief      Parses type,name,... with parentheses protecting UART CSV commas.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  line : Source CSV.
* @param[out] out : Typed fields.
*
* @return     bool : true if parsed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::ParseTargetLine(XSTRING* line, XTRACEMONITOR_CFG_TARGET& out)
{
  out.Clean();
  if(!line || line->IsEmpty()) return false;

  XVECTOR<XSTRING*> fields;
  XSTRING           cur;
  int               depth = 0;
  XDWORD            n     = line->GetSize();

  for(XDWORD i=0; i<n; i++)
    {
      XCHAR ch = line->Get()[i];
      if(ch == (XCHAR)'(')
        {
          depth++;
          cur.Add(ch);
          continue;
        }
      if(ch == (XCHAR)')')
        {
          if(depth > 0) depth--;
          cur.Add(ch);
          continue;
        }
      if((ch == (XCHAR)',') && (depth == 0))
        {
          cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_FROM_FIRST);
          cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_TO_END);
          cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_FROM_FIRST);
          cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_TO_END);
          XSTRING* field = GEN_NEW XSTRING();
          if(!field)
            {
              fields.DeleteContents();
              fields.DeleteAll();
              return false;
            }
          (*field) = cur;
          fields.Add(field);
          cur.Empty();
          continue;
        }
      cur.Add(ch);
    }

  cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_FROM_FIRST);
  cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_TO_END);
  cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_FROM_FIRST);
  cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_TO_END);
  {
    XSTRING* field = GEN_NEW XSTRING();
    if(!field)
      {
        fields.DeleteContents();
        fields.DeleteAll();
        return false;
      }
    (*field) = cur;
    fields.Add(field);
  }

  bool ok = false;
  do
    {
      if(fields.GetSize() < 2) break;

      XSTRING* tstr = fields.Get(0);
      XSTRING* name = fields.Get(1);
      if(!tstr || !name || tstr->IsEmpty()) break;

      out.type = tstr->ConvertToInt(0, NULL, false);
      out.name = (*name);

      if(out.type == XTRACEMONITOR_CFG_TARGETTYPE_UDP)
        {
          if(fields.GetSize() < 4) break;
          XSTRING* host = fields.Get(2);
          XSTRING* port = fields.Get(3);
          if(!host || !port || host->IsEmpty() || port->IsEmpty()) break;
          out.host = (*host);
          out.port = port->ConvertToInt(0, NULL, false);
          if(out.port <= 0) break;
          ok = true;
        }
      else if(out.type == XTRACEMONITOR_CFG_TARGETTYPE_UART)
        {
          if(fields.GetSize() < 3) break;
          XSTRING* uart = fields.Get(2);
          if(!uart || uart->IsEmpty()) break;
          out.uartconfig = (*uart);
          // Strip surrounding parentheses used to protect commas in the INI line.
          if((out.uartconfig.GetSize() >= 2) &&
             (out.uartconfig.Get()[0] == (XCHAR)'(') &&
             (out.uartconfig.Get()[out.uartconfig.GetSize()-1] == (XCHAR)')'))
            {
              XSTRING inner;
              out.uartconfig.Copy(1, (int)out.uartconfig.GetSize()-1, inner);
              out.uartconfig = inner;
            }
          ok = !out.uartconfig.IsEmpty();
        }
    }
  while(false);

  fields.DeleteContents();
  fields.DeleteAll();
  if(!ok) out.Clean();
  return ok;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::FormatTargetLine(const XTRACEMONITOR_CFG_TARGET& in, XSTRING& line)
* @brief      Builds the INI CSV line for a typed target.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  in : Typed fields.
* @param[out] line : CSV text.
*
* @return     bool : true if formatted; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::FormatTargetLine(const XTRACEMONITOR_CFG_TARGET& in, XSTRING& line)
{
  line.Empty();

  if(in.type == XTRACEMONITOR_CFG_TARGETTYPE_UDP)
    {
      line.Format(NL("%d,%s,%s,%d"), in.type, in.name.Get(), in.host.Get(), in.port);
      return true;
    }

  if(in.type == XTRACEMONITOR_CFG_TARGETTYPE_UART)
    {
      line.Format(NL("%d,%s,(%s)"), in.type, in.name.Get(), in.uartconfig.Get());
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::ParseOriginLine(XSTRING* line, XTRACEMONITOR_CFG_ORIGIN& out)
* @brief      Parses isactive,showip,name,url,ip CSV (bools via ConvertToBoolean).
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  line : Source CSV.
* @param[out] out : Typed fields.
*
* @return     bool : true if parsed; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::ParseOriginLine(XSTRING* line, XTRACEMONITOR_CFG_ORIGIN& out)
{
  out.Clean();
  if(!line || line->IsEmpty()) return false;

  XVECTOR<XSTRING*> fields;
  XSTRING           cur;
  XDWORD            n = line->GetSize();

  for(XDWORD i=0; i<n; i++)
    {
      XCHAR ch = line->Get()[i];
      if(ch == (XCHAR)',')
        {
          cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_FROM_FIRST);
          cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_TO_END);
          cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_FROM_FIRST);
          cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_TO_END);
          XSTRING* field = GEN_NEW XSTRING();
          if(!field)
            {
              fields.DeleteContents();
              fields.DeleteAll();
              return false;
            }
          (*field) = cur;
          fields.Add(field);
          cur.Empty();
          continue;
        }
      cur.Add(ch);
    }

  cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_FROM_FIRST);
  cur.DeleteCharacter((XCHAR)' ' , XSTRINGCONTEXT_TO_END);
  cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_FROM_FIRST);
  cur.DeleteCharacter((XCHAR)'\t', XSTRINGCONTEXT_TO_END);
  {
    XSTRING* field = GEN_NEW XSTRING();
    if(!field)
      {
        fields.DeleteContents();
        fields.DeleteAll();
        return false;
      }
    (*field) = cur;
    fields.Add(field);
  }

  bool ok = false;
  do
    {
      // Trailing empty ip yields 5 fields (last empty). Require at least name+url present.
      if(fields.GetSize() < 4) break;

      XSTRING* a = fields.Get(0);
      XSTRING* b = fields.Get(1);
      XSTRING* name = fields.Get(2);
      XSTRING* url  = fields.Get(3);
      XSTRING* ip   = (fields.GetSize() >= 5) ? fields.Get(4) : NULL;
      if(!a || !b || !name || !url) break;
      if(a->IsEmpty() || b->IsEmpty()) break;
      if(name->IsEmpty()) break;

      out.isactive = a->ConvertToBoolean();
      out.showip   = b->ConvertToBoolean();
      out.name     = (*name);
      out.url      = (*url);
      if(ip) out.ip = (*ip);
      ok = true;
    }
  while(false);

  fields.DeleteContents();
  fields.DeleteAll();
  if(!ok) out.Clean();
  return ok;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::FormatOriginLine(const XTRACEMONITOR_CFG_ORIGIN& in, XSTRING& line)
* @brief      Builds the INI CSV line for a typed origin (bools as yes/no).
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  in : Typed fields.
* @param[out] line : CSV text.
*
* @return     bool : true if formatted; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::FormatOriginLine(const XTRACEMONITOR_CFG_ORIGIN& in, XSTRING& line)
{
  line.Format(NL("%s,%s,%s,%s,%s"),
              in.isactive ? NL("yes") : NL("no"),
              in.showip   ? NL("yes") : NL("no"),
              in.name.Get(),
              in.url.Get(),
              in.ip.Get());
  return true;
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
      // Existing INIs without [target]/[origins] keep empty slots after load — seed once.
      bool seeded = false;
      if(SeedDefaultTargetsIfEmpty()) seeded = true;
      if(SeedDefaultOriginsIfEmpty()) seeded = true;
      if(seeded)
        {
          Save();
        }
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
  filters_istextactive = XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE_DEFAULT;
  filters_text.Empty();
  filters_levels       = XTRACEMONITOR_CFG_FILTERS_LEVELFILTER_DEFAULT;
  dyndns_active        = XTRACEMONITOR_CFG_DYNDNS_ACTIVE_DEFAULT;
  dyndns_login.Empty();
  dyndns_password.Empty();
  ntargets = 0;
  targets.DeleteContents();
  targets.DeleteAll();
  norigins = 0;
  origins.DeleteContents();
  origins.DeleteAll();
}
