/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ActionScriptQA_CFG.h
* 
* @class      ACTIONSCRIPTQA_CFG
* @brief      Locomotive Operations for Kinetic Inspections (Configuration)
* @ingroup    
* 
* @author     Abraham J. Velez 
* @date       24/07/2023 7:48:40
* 
* @copyright  CAF Signalling  All rights reserved.
* 
* --------------------------------------------------------------------------------------------------------------------*/

#ifndef _ACTIONSCRIPTQA_CFG_H_
#define _ACTIONSCRIPTQA_CFG_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/
#pragma region INCLUDES

#include "APPFlowCFG.h"

#pragma endregion


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/
#pragma region DEFINES_ENUMS

#define ACTIONSCRIPTQACFG_SECTIONGENERAL     __L("general")

#pragma endregion


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/
#pragma region CLASS


class ACTIONSCRIPTQA_CFG : public APPFLOWCFG
{
  public:

    static bool                     GetIsInstanced                          ();
    static ACTIONSCRIPTQA_CFG&                GetInstance                             (bool ini = true);
    static bool                     DelInstance                             ();

    bool                            DoVariableMapping                       (); 
    bool                            DoDefault                               ();

  private:
                                    ACTIONSCRIPTQA_CFG                                (XCHAR* namefile);
                                    ACTIONSCRIPTQA_CFG                                (ACTIONSCRIPTQA_CFG const&);        // Don't implement
    virtual                        ~ACTIONSCRIPTQA_CFG                                ();

    void                            operator =                              (ACTIONSCRIPTQA_CFG const&);        // Don't implement

    void                            Clean                                   ();

    static ACTIONSCRIPTQA_CFG*                instance;
};



#pragma endregion


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
#pragma region FUNCTIONS_PROTOTYPES


#pragma endregion


#endif

