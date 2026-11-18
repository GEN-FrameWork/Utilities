/**-------------------------------------------------------------------------------------------------------------------
*
* @file       APPUpdateCreator_CFG.cpp
*
* @class      APPUPDATECREATOR_CFG
* @brief      Application Update Creator configuration class
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

#include "XLog.h"

#include "APPUpdateCreator.h"

#include "APPUpdateCreator_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

APPUPDATECREATOR_CFG* APPUPDATECREATOR_CFG::instance = NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR_CFG::GetIsInstanced()
* @brief      Check if the singleton instance is created.
* @ingroup    UTILS
*
* @return     bool : true if the instance exists.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR_CFG::GetIsInstanced()
{
  return instance!=NULL;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         APPUPDATECREATOR_CFG& APPUPDATECREATOR_CFG::GetInstance()
* @brief      Get the singleton instance of the configuration.
* @ingroup    UTILS
*
* @return     APPUPDATECREATOR_CFG& : reference to the singleton instance.
*
*---------------------------------------------------------------------------------------------------------------------*/
APPUPDATECREATOR_CFG& APPUPDATECREATOR_CFG::GetInstance()
{
  if(!instance) instance = GEN_NEW APPUPDATECREATOR_CFG(APPLICATION_NAMEFILE);

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR_CFG::DelInstance()
* @brief      Delete the singleton instance of the configuration.
* @ingroup    UTILS
*
* @return     bool : true if the instance has been deleted.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR_CFG::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool APPUPDATECREATOR_CFG::Default()
* @brief      Set the default configuration parameters.
* @ingroup    UTILS
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool APPUPDATECREATOR_CFG::Default()
{

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         APPUPDATECREATOR_CFG::APPUPDATECREATOR_CFG(XCHAR* namefile)
* @brief      Constructor of class.
* @ingroup    UTILS
*
* @param[in]  namefile : name of the configuration file.
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
APPUPDATECREATOR_CFG::APPUPDATECREATOR_CFG(XCHAR* namefile) : APPFLOWCFG(namefile)
{
  XSTRING section;
  XSTRING key;

  Clean();

  Default();

  Ini<APPUPDATECREATOR_CFG>();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         APPUPDATECREATOR_CFG::~APPUPDATECREATOR_CFG()
* @brief      Destructor of class.
* @note       VIRTUAL
* @ingroup    UTILS
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
APPUPDATECREATOR_CFG::~APPUPDATECREATOR_CFG()
{
  End();

  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void APPUPDATECREATOR_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    UTILS
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void APPUPDATECREATOR_CFG::Clean()
{

}






