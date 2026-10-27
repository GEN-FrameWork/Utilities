/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA_CFG.cpp
* 
* @class      ACTIONSCRIPTQA_CFG
* @brief      Locomotive Operations for Kinetic Inspections (Configuration)
* @ingroup    
* 
* @author     Abraham J. Velez 
* @date       24/07/2023 7:47:17
* 
* @copyright  CAF Signalling  All rights reserved.
* 
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ----------------------------------------------------------------------------------------*/
#pragma region PRECOMPILATION_INCLUDES

#include "GEN_Defines.h"
#include "XTranslation.h"
#include "ActionScriptQA_TranslateID.h"

#pragma endregion


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
#pragma region INCLUDES

#include "ActionScriptQA_CFG.h"

#include "XLog.h"

#include "ActionScriptQA.h"

#include "XMemory_Control.h"


#pragma endregion


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/
#pragma region GENERAL_VARIABLE

ACTIONSCRIPTQA_CFG* ACTIONSCRIPTQA_CFG::instance = NULL;

#pragma endregion


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/
#pragma region CLASS_MEMBERS


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA_CFG::GetIsInstanced()
* @brief      GetIsInstanced
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA_CFG::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ACTIONSCRIPTQA_CFG& ACTIONSCRIPTQA_CFG::GetInstance(bool ini)
* @brief      GetInstance
* @ingroup    
* 
* @param[in]  ini : 
* 
* @return     ACTIONSCRIPTQA_CFG& : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_CFG& ACTIONSCRIPTQA_CFG::GetInstance(bool ini)
{
  if(!instance) instance = new ACTIONSCRIPTQA_CFG(ini?APPLICATION_NAMEFILE:NULL);

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA_CFG::DelInstance()
* @brief      DelInstance
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA_CFG::DelInstance()
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
* @fn         bool ACTIONSCRIPTQA_CFG::DoVariableMapping()
* @brief      DoVariableMapping
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA_CFG::DoVariableMapping()
{
  if(!APPFLOWCFG::DoVariableMapping())
    {
      return false;
    }

  AddRemark(ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, NL("--------------------------------------------------------------------------------------------------------------------------------------------"), 0, 1);
  AddRemark(ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_SCRIPT_RECORDER_F1), 0, 2);

  AddValue(XFILECFG_VALUETYPE_STRING , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_OUTPUT   , &scriptrecord_outputscript   , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_OUTPUT_JS_UNDER_TESTS_BASE)  , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_STRING , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_BMPPREFIX, &scriptrecord_bitmapprefix   , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_BITMAP_FILE_NAME_PREFIX)         , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT    , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_CAPTUREW , &scriptrecord_capturewidth   , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_CAPTURE_WIDTH_PIXELS)          , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT    , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_CAPTUREH , &scriptrecord_captureheight  , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_CAPTURE_HEIGHT_PIXELS)         , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT    , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_WAITTO   , &scriptrecord_waittimeoutms  , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_WAITBITMAP_TIMEOUT_MS)         , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);
  AddValue(XFILECFG_VALUETYPE_INT    , ACTIONSCRIPTQACFG_SECTION_SCRIPTRECORD, ACTIONSCRIPTQACFG_SCRIPTRECORD_WAITIV   , &scriptrecord_waitintervalms , TL(ACTIONSCRIPTQA_TRANSLATEID_ACTIONSCRIPTQA_CFG_WAITBITMAP_INTERVAL_MS)        , APPFLOW_CFG_DEFAULT_REMARK_COLUMN);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ACTIONSCRIPTQA_CFG::DoDefault()
* @brief      DoDefault
* @ingroup    
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ACTIONSCRIPTQA_CFG::DoDefault()
{
  if(!APPFLOWCFG::DoDefault()) 
    {
      return false;
    }
  
  //------------------------------------------------------------------------------

  log_isactive                          = true;
  log_backupisactive                    = true;
  log_backupmaxfiles                    = 10;
  log_backupiscompress                  = true;

  log_activesectionsID.Empty();

  log_activesectionsID                 += APPFLOW_CFG_LOG_SECTIONID_INITIATION;
  log_activesectionsID                 += NL(",");
  log_activesectionsID                 += APPFLOW_CFG_LOG_SECTIONID_GENERIC;
  log_activesectionsID                 += NL(",");
  log_activesectionsID                 += APPFLOW_CFG_LOG_SECTIONID_STATUSAPP;
  log_activesectionsID                 += NL(",");
  log_activesectionsID                 += APPFLOW_CFG_LOG_SECTIONID_ENDING;

  log_levelmask                         = XLOGLEVEL_ALL;
  log_maxsize                           = 3000;
  log_reductionpercent                  = 10;

  scriptrecord_outputscript             = NL("Tests_Recorded.js");
  scriptrecord_bitmapprefix             = NL("rec_");
  scriptrecord_capturewidth             = 96;
  scriptrecord_captureheight            = 32;
  scriptrecord_waittimeoutms            = 10000;
  scriptrecord_waitintervalms           = 500;

  //------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* ACTIONSCRIPTQA_CFG::ScriptRecord_GetOutputScript()
* @brief      ScriptRecord_GetOutputScript
* @ingroup    
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* ACTIONSCRIPTQA_CFG::ScriptRecord_GetOutputScript()
{
  return &scriptrecord_outputscript;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* ACTIONSCRIPTQA_CFG::ScriptRecord_GetBitmapPrefix()
* @brief      ScriptRecord_GetBitmapPrefix
* @ingroup    
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* ACTIONSCRIPTQA_CFG::ScriptRecord_GetBitmapPrefix()
{
  return &scriptrecord_bitmapprefix;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int ACTIONSCRIPTQA_CFG::ScriptRecord_GetCaptureWidth()
* @brief      ScriptRecord_GetCaptureWidth
* @ingroup    
* 
* --------------------------------------------------------------------------------------------------------------------*/
int ACTIONSCRIPTQA_CFG::ScriptRecord_GetCaptureWidth()
{
  return scriptrecord_capturewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int ACTIONSCRIPTQA_CFG::ScriptRecord_GetCaptureHeight()
* @brief      ScriptRecord_GetCaptureHeight
* @ingroup    
* 
* --------------------------------------------------------------------------------------------------------------------*/
int ACTIONSCRIPTQA_CFG::ScriptRecord_GetCaptureHeight()
{
  return scriptrecord_captureheight;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int ACTIONSCRIPTQA_CFG::ScriptRecord_GetWaitTimeoutMs()
* @brief      ScriptRecord_GetWaitTimeoutMs
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
int ACTIONSCRIPTQA_CFG::ScriptRecord_GetWaitTimeoutMs()
{
  return scriptrecord_waittimeoutms;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int ACTIONSCRIPTQA_CFG::ScriptRecord_GetWaitIntervalMs()
* @brief      ScriptRecord_GetWaitIntervalMs
* @ingroup
*
* --------------------------------------------------------------------------------------------------------------------*/
int ACTIONSCRIPTQA_CFG::ScriptRecord_GetWaitIntervalMs()
{
  return scriptrecord_waitintervalms;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ACTIONSCRIPTQA_CFG::ACTIONSCRIPTQA_CFG(XCHAR* namefile) : APPCFG(namefile)
* @brief      Constructor
* @ingroup    
* 
* @param[in]  XCHAR* : 
* 
* @return     Does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_CFG::ACTIONSCRIPTQA_CFG(XCHAR* namefile) : APPFLOWCFG(namefile)
{
  Clean();

  if(namefile)
    {
      Ini<ACTIONSCRIPTQA_CFG>();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         ACTIONSCRIPTQA_CFG::~ACTIONSCRIPTQA_CFG()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    
* 
* @return     Does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
ACTIONSCRIPTQA_CFG::~ACTIONSCRIPTQA_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ACTIONSCRIPTQA_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    
* 
* @return     void : does not return anything. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ACTIONSCRIPTQA_CFG::Clean()
{
  scriptrecord_outputscript.Empty();
  scriptrecord_bitmapprefix.Empty();

  scriptrecord_capturewidth   = 96;
  scriptrecord_captureheight  = 32;
  scriptrecord_waittimeoutms  = 10000;
  scriptrecord_waitintervalms = 500;
}

#pragma endregion
