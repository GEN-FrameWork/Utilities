/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       TranslateScan_Manager.h
* 
* @class      TRANSLATESCAN_MANAGER
* @brief      Translate Scan Manager class
* @ingroup    APPLICATION
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
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XPath.h"
#include "XFileTXT.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef bool (*TRANSLANTESCAN_FOUNDINFILE_PTRFUNC)(XPATH* pathfile, XFILETXT* filetxt, int line, int index, XSTRING& result);
typedef bool (*TRANSLANTESCAN_CHANGEINFILE_PTRFUNC)(XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring,int line, int index, XSTRING& result);

enum TRANSLATESCAN_LITERALMACRO
{
  TRANSLATESCAN_LITERALMACRO_NONE = 0 ,
  TRANSLATESCAN_LITERALMACRO_L        ,   // _L("...")  → candidate to translate
  TRANSLATESCAN_LITERALMACRO_NL       ,   // NL("...")  → explicitly not translatable
  TRANSLATESCAN_LITERALMACRO_FL       ,   // FL("...")  → translate with fixed length
};



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class TRANSLATESCAN_FILETARGET
{
  public:
                             TRANSLATESCAN_FILETARGET                   ();
    virtual                 ~TRANSLATESCAN_FILETARGET                   ();

    XPATH*                   GetXPathFile                               ();
  
  private:

    void                     Clean                                      ();
    XPATH                    xpathfile;
};



class TRANSLATESCAN_MANAGER
{
  public:
                             TRANSLATESCAN_MANAGER                      ();
    virtual                 ~TRANSLATESCAN_MANAGER                      ();

    bool                     SearchFilesTarget                          (XCHAR* inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget);
    bool                     SearchFilesTarget                          (XPATH& inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget);

    bool                     SearchInFile                               (XPATH& pathfile, XCHAR* searchstring, TRANSLANTESCAN_FOUNDINFILE_PTRFUNC foundinfileptrfunc, TRANSLANTESCAN_CHANGEINFILE_PTRFUNC changeinfileptrfunc = NULL);

    bool                     Operation_Remark_Brief                     (XPATH& operationdir);
    static bool              Operation_Remark_Brief_SearchLine          (XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& brief);
    static bool              Operation_Remark_Brief_ChangeLine          (XPATH* pathfile, XFILETXT* filetxt, XSTRING& seachstring, int nline, int index, XSTRING& result);
    static bool              Operation_Remark_Brief_Construct           (XSTRING& brief_origin, XSTRING& brief_target);

    bool                     Operation_Remark_InGroup                   (XPATH& operationdir);
    static bool              Operation_Remark_InGroup_SearchLine        (XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result);
    static bool              Operation_Remark_InGroup_ChangeLine        (XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring, int nline, int index, XSTRING& result);

    bool                     ModifierGEN_Active                     (bool isactive);
    bool                     ModifierGEN_IsActive                   ();

    bool                     Operation_TranslateGENInternal         (XPATH& operationdir);
    bool                     Operation_AddLanguageGEN               (XPATH& operationdir, XSTRING& codealpha2);

    bool                     ModifierAPP_Active                     (bool isactive);
    bool                     ModifierAPP_IsActive                   ();

    bool                     Operation_TranslateAPP                 (XPATH& operationdir);
    bool                     Operation_AddLanguageAPP               (XPATH& operationdir, XSTRING& codealpha2);

    bool                     ModifierVIEW_Active                    (bool isactive);
    bool                     ModifierVIEW_IsActive                  ();

    bool                     Operation_ViewLiteralStrings           (XPATH& operationdir);

    bool                     Operation_LiteralStrings                   (XPATH& operationdir);
    static bool              Operation_LiteralStrings_SearchLine        (XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result);
    static TRANSLATESCAN_LITERALMACRO Operation_LiteralStrings_GetMacroAt(XSTRING* line, int index);
    static bool              Operation_LiteralStrings_IsApplicationName (XSTRING* line, int index);
    static bool              Operation_LiteralStrings_HasTranslatableText(XSTRING& literal);
    static bool              Operation_LiteralStrings_IsGENInternal      (XSTRING& literal, XDWORD& out_ID); // full literal + all ENG/SPA/FRE/GER/CHI
    static int               Operation_LiteralStrings_FindStringEnd     (XCHAR* text, int size, int start);
    static int               Operation_LiteralStrings_SkipPrintfMask   (XCHAR* text, int size, int pos);
    static int               Operation_LiteralStrings_SkipEscape        (XCHAR* text, int size, int pos);

  private:

    void                     Clean                                      ();

    bool                     modifiergen;
    bool                     modifierapp;
    bool                     modifierview;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






