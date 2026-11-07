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
  colortheme = XTRACEMONITOR_CFG_COLORTHEME_INVALID;
}
