/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       TranslateScan_Manager.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include "TranslateScan_Manager.h"

#include <stdarg.h>

#include "XFactory.h"
#include "XDir.h"
#include "XFile.h"
#include "XBuffer.h"
#include "XMap.h"
#include "XTrace.h"
#include "XConsole.h"
#include "XTranslation_GEN.h"
#include "XLanguage_ISO_639_3.h"
#include "APPFlowConsole.h"
#include "APPFlowExtended.h"

#ifdef DIO_SCRAPERWEB_TRANSLATION_ACTIVE
#include "DIOScraperWebTranslation.h"
#endif




/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_FILETARGET::TRANSLATESCAN_FILETARGET()
* @brief      Constructor of class.
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_FILETARGET::TRANSLATESCAN_FILETARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_FILETARGET::~TRANSLATESCAN_FILETARGET()
* @brief      Destructor of class.
* @note       VIRTUAL
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_FILETARGET::~TRANSLATESCAN_FILETARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XPATH* TRANSLATESCAN_FILETARGET::GetXPathFile()
* @brief      Get the target file path.
* @ingroup    APPLICATION
*
* @return     XPATH* : pointer to the target file path.
*
*---------------------------------------------------------------------------------------------------------------------*/
XPATH* TRANSLATESCAN_FILETARGET::GetXPathFile()
{
  return &xpathfile;
}

  
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TRANSLATESCAN_FILETARGET::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TRANSLATESCAN_FILETARGET::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_MANAGER::TRANSLATESCAN_MANAGER()
* @brief      Constructor of class.
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_MANAGER::TRANSLATESCAN_MANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_MANAGER::~TRANSLATESCAN_MANAGER()
* @brief      Destructor of class.
* @note       VIRTUAL
* @ingroup    APPLICATION
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_MANAGER::~TRANSLATESCAN_MANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::SearchFilesTarget(XCHAR* inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget)
* @brief      Search source files from a path and add them to the target list.
* @ingroup    APPLICATION
*
* @param[in]  inipath : initial directory path to scan.
* @param[out]  filestarget : list filled with the source files found.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::SearchFilesTarget(XCHAR* inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget)
{
  XCHAR* extensions[] = { _L(".c"),
                          _L(".cpp"),
                          _L(".h"),
                          _L(".hpp"),
                        };  

  XDIR*         xdir = NULL;
  XDIRELEMENT   searchelement;

  if(!inipath)
    {
      return false;
    }

  if(!filestarget)
    {
      return false;
    }

  xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir)
    {   
      return false;
    }

  if(xdir->FirstSearch(inipath, _L("*"), &searchelement))
    {
      do{
          if(searchelement.GetType() == XDIRELEMENTTYPE_DIR)
            {
              XPATH inisubpath;

              inisubpath = inipath;                            
              inisubpath.Slash_Add();
              inisubpath.Add(searchelement.GetNameFile()->Get());

              SearchFilesTarget(inisubpath.Get(), filestarget);
            }

          if(searchelement.GetType() == XDIRELEMENTTYPE_FILE)
            { 
              XSTRING ext;
              bool    valid = false;  
   
              searchelement.GetNameFile()->GetExt(ext); 
 
              for(XDWORD c=0; c<sizeof(extensions)/sizeof(XCHAR*); c++)
                {
                  if(!ext.Compare(extensions[c],true))
                    {
                      valid = true;    
                    }
                }
            
              if(valid)  
                {
                  TRANSLATESCAN_FILETARGET* filetarget = GEN_NEW TRANSLATESCAN_FILETARGET();
                  if(filestarget)
                    {
                      filetarget->GetXPathFile()->Set(inipath);
                      filetarget->GetXPathFile()->Slash_Add();
                      filetarget->GetXPathFile()->Add(searchelement.GetNameFile()->Get()); 

                      filestarget->Add(filetarget);
                    }
                }
            }
  
        } while(xdir->NextSearch(&searchelement));      
    }
    
  GEN_XFACTORY.Delete_Dir(xdir);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::SearchFilesTarget(XPATH& inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget)
* @brief      Search source files from a path and add them to the target list.
* @ingroup    APPLICATION
*
* @param[in]  inipath : initial directory path to scan.
* @param[out]  filestarget : list filled with the source files found.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::SearchFilesTarget(XPATH& inipath, XVECTOR<TRANSLATESCAN_FILETARGET*>* filestarget)
{
  return SearchFilesTarget(inipath.Get(), filestarget);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::SearchInFile(XPATH& pathfile, XCHAR* searchstring, TRANSLANTESCAN_FOUNDINFILE_PTRFUNC foundinfileptrfunc, TRANSLANTESCAN_CHANGEINFILE_PTRFUNC changeinfileptrfunc)
* @brief      Search a text pattern in a file and optionally change matching lines.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path to open and scan.
* @param[in]  searchstring : text pattern to search.
* @param[in]  foundinfileptrfunc : callback executed when a match is found.
* @param[in]  changeinfileptrfunc : optional callback used to modify a matching line.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::SearchInFile(XPATH& pathfile, XCHAR* searchstring, TRANSLANTESCAN_FOUNDINFILE_PTRFUNC foundinfileptrfunc, TRANSLANTESCAN_CHANGEINFILE_PTRFUNC changeinfileptrfunc)
{
  if(!searchstring)
    {
      return false;
    }

  XSTRING _searchstring = searchstring;
  bool status = false;

  XFILETXT* filetxt = GEN_NEW XFILETXT();
  if(!filetxt)
    {
      return false;
    }

  if(filetxt->Open(pathfile.Get(), false))
    {
      bool change = false;

      filetxt->ReadAllFile();

      for(int c=0; c<(int)filetxt->GetNLines(); c++)
        {
          XSTRING* line = filetxt->GetLine(c);
          if(line)
            {
              int index = 0;

              do{ index = line->Find(searchstring, false, index);

                  if(index != XSTRING_NOTFOUND)   
                    {                    
                      if(foundinfileptrfunc)
                        {
                          XSTRING result;

                          foundinfileptrfunc(&pathfile, filetxt, c, index, result);

                          if(changeinfileptrfunc)
                            {
                              if(changeinfileptrfunc(&pathfile, filetxt, _searchstring, c, index, result))
                                {
                                  change = true;
                                  break;  
                                }
                            }
                        }

                      index += _searchstring.GetSize(); 
                    }                  
             
                } while(index != XSTRING_NOTFOUND);

            }
        }

      if(change)
        {
          filetxt->WriteAllFile();
        }
      
      filetxt->Close();

      status = true;
    } 

  GEN_DELETE filetxt;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief(XPATH& operationdir)
* @brief      Update brief remarks in the source files of a directory.
* @ingroup    APPLICATION
*
* @param[in]  operationdir : directory where the operation is executed.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief(XPATH& operationdir)
{
  XVECTOR<TRANSLATESCAN_FILETARGET*> filestarget;

  if(!SearchFilesTarget(operationdir.Get(), &filestarget))
    {
      filestarget.DeleteContents();
      filestarget.DeleteAll();  

      return false;
    }

  for(XDWORD c=0; c<filestarget.GetSize(); c++)
    {
      TRANSLATESCAN_FILETARGET* filetarget = filestarget.Get(c);
      if(filetarget)
        {
          XPATH* path = filetarget->GetXPathFile();
          if(path)
            {            
              SearchInFile((*path), _L("* @brief      "), Operation_Remark_Brief_SearchLine, Operation_Remark_Brief_ChangeLine);
            }
        }
   }
 
  filestarget.DeleteContents();
  filestarget.DeleteAll();  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
* @brief      Build the replacement brief text for a matched function remark line.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path being scanned.
* @param[in]  filetxt : text file being scanned.
* @param[in]  nline : line number where the match was found.
* @param[in]  index : character index where the match was found.
* @param[out]  result : brief text generated for the line.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
{
  if(!pathfile)
    {
      return false;
    }

  if(!filetxt)
    { 
      return false;
    }

  if(nline < 0)
    {
      return false;
    }

  XSTRING* searchline = filetxt->GetLine(nline);
  if(!searchline)
    {
      return false;
    }

  XSTRING* functionline = filetxt->GetLine(nline-1);
  if(!functionline)
    {
      return false;
    }

  XSTRING functionstr;

  functionstr = _L("* @fn         ");
  int start = functionline->Find(functionstr.Get(), false, 0);
  if(start == XSTRING_NOTFOUND)
    {
      // Not remark function.
      return false;
    }

  start += functionstr.GetSize();

  int startmember = functionline->Find(_L("::"), true, start);
  if(startmember == XSTRING_NOTFOUND)
    {
      return false;
    }

  XSTRING nameclass;
  bool    isconstructor = false;
  bool    isdestructor  = false;

  functionline->Copy(start, startmember, nameclass);

  if(nameclass.IsEmpty())
    {
      return false;
    }

  int indexlastspace = nameclass.FindCharacter(_C(' '), 0, true);
  if(indexlastspace >= 1)
    {
      nameclass.DeleteCharacters(0, indexlastspace+1); 
    }

  startmember+=2;

  int endmember = functionline->Find(_L("("), true, startmember);
  if(endmember == XSTRING_NOTFOUND)
    {
      return false;
    }

  XSTRING namemember;
 
  functionline->Copy(startmember, endmember, namemember);
  if(namemember.IsEmpty())
    {
      return false;
    }

  if(namemember.FindCharacter(_C('~'), 0, false) != XSTRING_NOTFOUND)
    {
      isdestructor = true;
      result       = _L("Destructor of class");
    }
   else 
    {
      if(!nameclass.Compare(namemember))
        {
          isconstructor = true;
          result        = _L("Constructor of class");
        }
       else
        {
          if(!namemember.Compare(_L("Clean"), false))
            {
              result = _L("Clean the attributes of the class: Default initialize");
            }
           else     
            {
              Operation_Remark_Brief_Construct(namemember, result);        
            }
        } 
    }

  if(result.IsEmpty())
    {
      return false;
    }
   
  XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, _L(" %-48s - %-64s | %s"), nameclass.Get(), namemember.Get(), result.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_ChangeLine(XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring, int nline, int index, XSTRING& result)
* @brief      Replace a matched brief remark line with the generated text.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path being modified.
* @param[in,out]  filetxt : text file where the line is replaced.
* @param[in]  searchstring : text pattern that was matched.
* @param[in]  nline : line number to replace.
* @param[in]  index : character index where the matched text starts.
* @param[in]  result : brief text to write.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_ChangeLine(XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring, int nline, int index, XSTRING& result)
{
  if(!pathfile)
    {
      return false;
    }

  if(!filetxt)
    { 
      return false;
    }

  if(nline < 0)
    {
      return false;
    }

  if(result.IsEmpty())
    {
      return false;
    }

  XSTRING searchline = filetxt->GetLine(nline)->Get();

  if(!searchline.IsEmpty())
    {
      searchline.DeleteCharactersToEnd(index +  searchstring.GetSize());
      searchline.Add(result);


      filetxt->DeleteLine(nline);
      filetxt->InsertLine(nline, searchline);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_Construct(XSTRING& brief_origin, XSTRING& brief_target)
* @brief      Construct a readable brief text from a member name.
* @ingroup    APPLICATION
*
* @param[in]  brief_origin : original member name used as source text.
* @param[out]  brief_target : readable brief text generated from the source text.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_Brief_Construct(XSTRING& brief_origin, XSTRING& brief_target)
{
  brief_target = brief_origin;

  if(brief_target.IsEmpty())
    {
      return false;
    }  
  
  int             startindexupper = 0;
  int             ncharupper      = 0;
  bool            startupper      = false; 
  XMAP<int, int>  range;

  for(XDWORD c=0; c<brief_target.GetSize(); c++)
    {  
      if(brief_target.Character_IsUpperCase(brief_target.Get()[c]) && !startupper)
        {
          startupper      = true;
          startindexupper = c;
          ncharupper      = 1;
        } 
       else
        {
          if((brief_target.Character_IsLowerCase(brief_target.Get()[c]) || (c>=brief_target.GetSize())) && startupper)
            {
              range.Add(startindexupper, ncharupper);

              startupper      = false; 
              startindexupper = 0;
              ncharupper      = 0;
            }
           else
            {
              ncharupper++;    
            }
        }        
    }

  if(startupper)
    {
      range.Add(startindexupper, ncharupper);

      startupper      = false; 
      startindexupper = 0;
      ncharupper      = 0;
    }

  int shift = 0;

  for(XDWORD c=0; c<range.GetSize(); c++)
    {
      int index = range.GetKey(c) + shift;
      int nchar = range.GetElement(c);
          
      if(brief_target.Get()[index-1] != _C(' ') && (nchar > 1))
        {
          if(brief_target.GetSize() != index + nchar)
            {
              if(brief_target.GetSize() >= index + nchar - 1)
                {
                  brief_target.Get()[(index + nchar - 1)] = brief_target.Character_ToLower(brief_target.Get()[(index + nchar - 1)]);
                }
            }

          if(index)
            {
              brief_target.Insert(_L(" "), index);
              shift++;
              index++;
            }

          if(brief_target.GetSize() != index + nchar)
            {
              if(brief_target.GetSize() >= index + nchar - 1)
                {
                  brief_target.Insert(_L(" "), (index + nchar -1));             
                  shift++;
                  index++;
                }
            }
        }

      if(index)
        {
          if(brief_target.Get()[index-1] != _C(' ') && (nchar == 1))
            {
              brief_target.Get()[index] = brief_target.Character_ToLower(brief_target.Get()[index]);
              brief_target.Insert(_L(" "), index);
              shift++;
              index++;
            }
        }        
    }
  
  brief_target.DeleteCharacter(_C('_'));

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup(XPATH& operationdir)
* @brief      Update ingroup remarks in the source files of a directory.
* @ingroup    APPLICATION
*
* @param[in]  operationdir : directory where the operation is executed.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup(XPATH& operationdir)
{
  XVECTOR<TRANSLATESCAN_FILETARGET*> filestarget;

  if(!SearchFilesTarget(operationdir.Get(), &filestarget))
    {
      filestarget.DeleteContents();
      filestarget.DeleteAll();  

      return false;
    }

  for(XDWORD c=0; c<filestarget.GetSize(); c++)
    {
      TRANSLATESCAN_FILETARGET* filetarget = filestarget.Get(c);
      if(filetarget)
        {
          XPATH* path = filetarget->GetXPathFile();
          if(path)
            {            
              SearchInFile((*path), _L("* @ingroup    "), Operation_Remark_InGroup_SearchLine, Operation_Remark_InGroup_ChangeLine);
            }
        }
   }
 
  filestarget.DeleteContents();
  filestarget.DeleteAll();  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
* @brief      Build the replacement ingroup text for a matched remark line.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path being scanned.
* @param[in]  filetxt : text file being scanned.
* @param[in]  nline : line number where the match was found.
* @param[in]  index : character index where the match was found.
* @param[out]  result : ingroup text generated for the line.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
{
  if(!pathfile)
    {
      return false;
    }

  if(!filetxt)
    { 
      return false;
    }

  if(nline < 0)
    {
      return false;
    }

  XSTRING* searchline = filetxt->GetLine(nline);
  if(!searchline)
    {
      return false;
    }

  bool isGEN = false;

  result = _L("");

  if((pathfile->Find(_L("/GEN/")           , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(_L("\\GEN\\")         , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(_L("/GENFrameWork/")  , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(_L("\\GENFrameWork\\"), true) != XSTRING_NOTFOUND))
    {
      isGEN = true;
    }
     
  if(isGEN)
    {       
      if(pathfile->Find(_L("AppFlow")                                , true) != XSTRING_NOTFOUND) result = _L("APPFLOW");
      if(pathfile->Find(_L("Cipher")                                 , true) != XSTRING_NOTFOUND) result = _L("CIPHER");
      if(pathfile->Find(_L("Common")                                 , true) != XSTRING_NOTFOUND) result = _L("COMMON");
      if(pathfile->Find(_L("Compress")                               , true) != XSTRING_NOTFOUND) result = _L("COMPRESS");
      if(pathfile->Find(_L("Databases")                              , true) != XSTRING_NOTFOUND) result = _L("DATABASE");
      if(pathfile->Find(_L("DataIO")                                 , true) != XSTRING_NOTFOUND) result = _L("DATAIO");
      if(pathfile->Find(_L("Graphic")                                , true) != XSTRING_NOTFOUND) result = _L("GRAPHIC");
      if(pathfile->Find(_L("Input")                                  , true) != XSTRING_NOTFOUND) result = _L("INPUT");
      if(pathfile->Find(_L("MainProc")                               , true) != XSTRING_NOTFOUND) result = _L("MAIN_PROCEDURE");
      if(pathfile->Find(_L("Script")                                 , true) != XSTRING_NOTFOUND) result = _L("SCRIPT");
      if(pathfile->Find(_L("Sound")                                  , true) != XSTRING_NOTFOUND) result = _L("SOUND");
      if(pathfile->Find(_L("UserInterface")                          , true) != XSTRING_NOTFOUND) result = _L("USERINTERFACE");
      if(pathfile->Find(_L("XUtils")                                 , true) != XSTRING_NOTFOUND) result = _L("XUTILS");

      if(pathfile->Find(_L("Examples")                               , true) != XSTRING_NOTFOUND) result = _L("EXAMPLES");
      if(pathfile->Find(_L("Tests")                                  , true) != XSTRING_NOTFOUND) result = _L("TESTS");
      if(pathfile->Find(_L("UnitTests")                              , true) != XSTRING_NOTFOUND) result = _L("UNIT_TESTS");
                                                                      
      if(pathfile->Find(_L("Platforms/Windows")                      , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_WINDOWS");
      if(pathfile->Find(_L("Platforms/Linux")                        , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_LINUX");
      if(pathfile->Find(_L("Platforms/Android")                      , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_ANDROID");
      if(pathfile->Find(_L("Platforms/Microcontrollers/STM32")       , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_STM32");
      if(pathfile->Find(_L("Platforms/Microcontrollers/ESP32")       , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_ESP32");
      if(pathfile->Find(_L("Platforms/Microcontrollers/SAMD5xE5x")   , true) != XSTRING_NOTFOUND) result = _L("PLATFORM_SAMD5XE5X");           
    }

  if(result.IsEmpty())                                
    {
      result = _L("APPLICATION");
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, _L(" %s | %s"), pathfile->Get(), result.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup_ChangeLine(XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring, int nline, int index, XSTRING& result)
* @brief      Replace a matched ingroup remark line with the generated text.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path being modified.
* @param[in,out]  filetxt : text file where the line is replaced.
* @param[in]  searchstring : text pattern that was matched.
* @param[in]  nline : line number to replace.
* @param[in]  index : character index where the matched text starts.
* @param[in]  result : ingroup text to write.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_Remark_InGroup_ChangeLine(XPATH* pathfile, XFILETXT* filetxt, XSTRING& searchstring, int nline, int index, XSTRING& result)
{
  if(nline < 0)
    {
      return false;
    }

  if(result.IsEmpty())
    {
      return false;
    }

  XSTRING searchline = filetxt->GetLine(nline)->Get();

  if(!searchline.IsEmpty())
    {
      searchline.DeleteCharactersToEnd(index +  searchstring.GetSize());
      searchline.Add(result);


      filetxt->DeleteLine(nline);
      filetxt->InsertLine(nline, searchline);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierGEN_Active(bool isactive)
* @brief      Mark that extracted strings belong to the GEN internal catalogue.
* @ingroup    APPLICATION
*
* @param[in]  isactive : true when the directory is the GEN tree.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierGEN_Active(bool isactive)
{
  modifiergen = isactive;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierGEN_IsActive()
* @brief      True when extracted strings belong to the GEN internal catalogue.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierGEN_IsActive()
{
  return modifiergen;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierAPP_Active(bool isactive)
* @brief      Mark that extracted strings belong to the application catalogue.
* @ingroup    APPLICATION
*
* @param[in]  isactive : true when the directory is an application.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierAPP_Active(bool isactive)
{
  modifierapp = isactive;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierAPP_IsActive()
* @brief      True when extracted strings belong to the application catalogue.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierAPP_IsActive()
{
  return modifierapp;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierVIEW_Active(bool isactive)
* @brief      Mark that literals are only listed, never rewritten.
* @ingroup    APPLICATION
*
* @param[in]  isactive : true when VIEW is the active modifier.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierVIEW_Active(bool isactive)
{
  modifierview = isactive;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::ModifierVIEW_IsActive()
* @brief      True when literals are only listed, never rewritten.
* @ingroup    APPLICATION
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::ModifierVIEW_IsActive()
{
  return modifierview;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_IsCatalogueFile(XPATH& pathfile)
* @brief      True for the GEN translation sources, which must not be rewritten as call sites.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_IsCatalogueFile(XPATH& pathfile)
{
  if(pathfile.Find(NL("XTranslation_GEN"), true) != XSTRING_NOTFOUND) return true;
  if(pathfile.Find(NL("XTranslation.cpp"), true) != XSTRING_NOTFOUND) return true;
  if(pathfile.Find(NL("XTranslation.h"), true) != XSTRING_NOTFOUND)   return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_AppendIdentifierChar(XSTRING& name, XCHAR character)
* @brief      Append one uppercase identifier character, collapsing separators to a single underscore.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_AppendIdentifierChar(XSTRING& name, XCHAR character)
{
  bool alnum = name.Character_IsAlpha(character) || name.Character_IsNumber(character, false);

  if(alnum)
    {
      name.Add(name.Character_ToUpper(character));
      return;
    }

  if(name.IsEmpty()) return;
  if(name.Character_GetLast() == _C('_')) return;

  name.Add(_C('_'));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_BuildIDName(XPATH& pathfile, XSTRING& literal, XSTRING& idname)
* @brief      Build XTRANSLATION_GEN_ID_<file>_<words> from the source file name and the literal.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_BuildIDName(XPATH& pathfile, XSTRING& literal, XSTRING& idname)
{
  XSTRING module;
  XSTRING words;
  XCHAR*  text = literal.Get();
  int     size = (int)literal.GetSize();

  pathfile.GetNamefile(module);
  module.ToUpperCase();

  idname = NL("XTRANSLATION_GEN_ID_");

  for(XDWORD c=0; c<module.GetSize(); c++)
    {
      TRANSLATESCAN_AppendIdentifierChar(idname, module.Get()[c]);
    }

  if((idname.GetSize() > 0) && (idname.Character_GetLast() != _C('_')))
    {
      idname.Add(_C('_'));
    }

  for(int c=0; text && (c<size); )
    {
      if(text[c] == _C('%'))
        {
          c = TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipPrintfMask(text, size, c);
          TRANSLATESCAN_AppendIdentifierChar(words, _C('_'));
          continue;
        }

      if(text[c] == _C('\\'))
        {
          c = TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipEscape(text, size, c);
          continue;
        }

      TRANSLATESCAN_AppendIdentifierChar(words, text[c]);
      c++;
    }

  if(!words.IsEmpty() && (words.Character_GetLast() == _C('_')))
    {
      words.DeleteLastCharacter();
    }

  if(words.IsEmpty())
    {
      words = NL("TEXT");
    }

  if(words.GetSize() > 48)
    {
      words.DeleteCharactersToEnd(48);
      if(words.Character_GetLast() == _C('_')) words.DeleteLastCharacter();
    }

  if(words.Character_IsNumber(words.Character_GetFirst(), false))
    {
      idname.Add(_C('N'));
    }

  idname.Add(words);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_EscapeForC(XSTRING& text, XSTRING& escaped)
* @brief      Escape a decoded translation so it can sit inside NL("...").
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_EscapeForC(XSTRING& text, XSTRING& escaped)
{
  escaped.Empty();

  for(XDWORD c=0; c<text.GetSize(); c++)
    {
      XCHAR character = text.Get()[c];

      if(character == _C('\\') || character == _C('"'))
        {
          escaped.Add(_C('\\'));
          escaped.Add(character);
          continue;
        }

      if(character == _C('\n')) { escaped.Add(NL("\\n")); continue; }
      if(character == _C('\r')) { escaped.Add(NL("\\r")); continue; }
      if(character == _C('\t')) { escaped.Add(NL("\\t")); continue; }

      if((character < 32) || (character > 126))
        {
          XSTRING unit;
          unit.Format(NL("\\u%04X"), (unsigned)character);
          escaped.Add(unit);
          continue;
        }

      escaped.Add(character);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int TRANSLATESCAN_HexDigit(XCHAR character)
* @brief      Hex value of one character, or -1.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static int TRANSLATESCAN_HexDigit(XCHAR character)
{
  if((character >= _C('0')) && (character <= _C('9'))) return (int)(character - _C('0'));
  if((character >= _C('a')) && (character <= _C('f'))) return (int)(character - _C('a')) + 10;
  if((character >= _C('A')) && (character <= _C('F'))) return (int)(character - _C('A')) + 10;

  return -1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_Unescape(XSTRING& raw, XSTRING& text)
* @brief      Turn the raw contents of a C literal into the text sent to the translator.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_Unescape(XSTRING& raw, XSTRING& text)
{
  XCHAR* source = raw.Get();
  int    size   = (int)raw.GetSize();

  text.Empty();

  for(int c=0; source && (c<size); c++)
    {
      if((source[c] != _C('\\')) || ((c + 1) >= size))
        {
          text.Add(source[c]);
          continue;
        }

      XCHAR escaped = source[++c];

      if(escaped == _C('n'))  { text.Add(_C('\n')); continue; }
      if(escaped == _C('r'))  { text.Add(_C('\r')); continue; }
      if(escaped == _C('t'))  { text.Add(_C('\t')); continue; }
      if(escaped == _C('"') || escaped == _C('\\') || escaped == _C('\''))
        {
          text.Add(escaped);
          continue;
        }

      if((escaped == _C('u')) && ((c + 4) < size))
        {
          int code = 0;
          int hexok = true;

          for(int h=1; h<=4; h++)
            {
              int value = TRANSLATESCAN_HexDigit(source[c + h]);
              if(value < 0)
                {
                  hexok = false;
                  break;
                }
              code = (code << 4) | value;
            }

          if(hexok)
            {
              text.Add((XCHAR)code);
              c += 4;
              continue;
            }
        }

      text.Add(_C('\\'));
      text.Add(escaped);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_ShieldPrintfMasks(XSTRING& text, XSTRING& shielded, XVECTOR<XSTRING*>& masks)
* @brief      Replace each C/C++ printf mask with {{n}} so the translator cannot rewrite it.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_ShieldPrintfMasks(XSTRING& text, XSTRING& shielded, XVECTOR<XSTRING*>& masks)
{
  XCHAR* source = text.Get();
  int    size   = (int)text.GetSize();

  shielded.Empty();

  for(int c=0; source && (c<size); )
    {
      if(source[c] == _C('%'))
        {
          int next = TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipPrintfMask(source, size, c);

          if(next > (c + 1))
            {
              XSTRING* mask = GEN_NEW XSTRING();
              XSTRING  token;

              if(mask)
                {
                  text.Copy(c, next, (*mask));
                  token.Format(NL("{{%d}}"), (int)masks.GetSize());
                  masks.Add(mask);
                  shielded.Add(token);
                  c = next;
                  continue;
                }
            }
        }

      shielded.Add(source[c]);
      c++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_RestorePrintfMasks(XSTRING& text, XVECTOR<XSTRING*>& masks)
* @brief      Put the original printf masks back. Fails if a placeholder did not survive the translation.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_RestorePrintfMasks(XSTRING& text, XVECTOR<XSTRING*>& masks)
{
  for(int index=(int)masks.GetSize()-1; index>=0; index--)
    {
      XSTRING* mask = masks.Get((XDWORD)index);
      XSTRING  token[4];
      bool     found = false;

      if(!mask) return false;

      token[0].Format(NL("{{%d}}"), index);
      token[1].Format(NL("{{ %d }}"), index);
      token[2].Format(NL("{%d}"), index);
      token[3].Format(NL("{ %d }"), index);

      for(int pattern=0; pattern<4; pattern++)
        {
          if(text.Find(token[pattern].Get(), false) == XSTRING_NOTFOUND) continue;

          text.ReplaceFirst(token[pattern].Get(), mask->Get());
          found = true;
          break;
        }

      if(!found) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_IDIsUsed(XSTRING& name, XSTRING& headertext)
* @brief      True when the identifier is already an enumerator in the GEN id header.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_IDIsUsed(XSTRING& name, XSTRING& headertext)
{
  XSTRING token;

  token = name;
  token.Add(NL(" ,"));
  if(headertext.Find(token.Get(), false) != XSTRING_NOTFOUND) return true;

  token = name;
  token.Add(NL(","));

  return headertext.Find(token.Get(), false) != XSTRING_NOTFOUND;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int TRANSLATESCAN_CatalogueLanguageIndex(XCHAR* detected)
* @brief      Map a detected ISO code onto ENG, SPA, FRE, GER, CHI. Returns -1 for any other language.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static int TRANSLATESCAN_CatalogueLanguageIndex(XCHAR* detected)
{
  XSTRING code;

  if(!detected || !detected[0]) return -1;

  code = detected;

  if(code.Compare(NL("en"), true) == 0) return 0;
  if(code.Compare(NL("es"), true) == 0) return 1;
  if(code.Compare(NL("fr"), true) == 0) return 2;
  if(code.Compare(NL("de"), true) == 0) return 3;
  if(code.Compare(NL("zh"), true) == 0) return 4;
  if(code.Find(NL("zh"), true) == 0)    return 4;

  return -1;
}


class TRANSLATESCAN_GENTEXT
{
  public:
    XPATH   pathfile;
    XSTRING literal;
    XSTRING idname;
    XSTRING sentence[5];
    int     fixed;
    bool    ready;
};


class TRANSLATESCAN_GENPLACE
{
  public:
    XPATH pathfile;
    int   nline;
    int   index;
    int   endquote;
    int   textindex;
};


class TRANSLATESCAN_LTONLPLACE
{
  public:
    XPATH   pathfile;
    int     nline;
    int     index;
    XSTRING literal;
};


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static XCONSOLE* TRANSLATESCAN_Console()
* @brief      Application console, when the flow has already created it.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static XCONSOLE* TRANSLATESCAN_Console()
{
  APPFLOWCONSOLE* appconsole = APPFLOW_EXTENDED.GetConsole();

  if(!appconsole) return NULL;

  return appconsole->GetConsole();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_Report(int color, XCHAR* mask, ...)
* @brief      Same operation detail on the console and on XTRACE (Extended Trace Monitor).
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_Report(int color, XCHAR* mask, ...)
{
  XSTRING   line;
  XCONSOLE* console;
  va_list   arg;

  if(!mask) return;

  va_start(arg, mask);
  line.FormatArg(mask, &arg);
  va_end(arg);

  console = TRANSLATESCAN_Console();
  if(console) console->Printf(NL("%s\n"), line.Get());

  XTRACE_PRINTCOLOR(color, NL("%s"), line.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_ApplyLToNLPlaces(XVECTOR<TRANSLATESCAN_LTONLPLACE*>& places, bool& status)
* @brief      Rewrite _L("...") with no translatable text as NL("...") in the collected sites.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_ApplyLToNLPlaces(XVECTOR<TRANSLATESCAN_LTONLPLACE*>& places, bool& status)
{
  XVECTOR<XPATH*> paths;

  if(places.IsEmpty()) return;

  for(XDWORD p=0; p<places.GetSize(); p++)
    {
      TRANSLATESCAN_LTONLPLACE* place = places.Get(p);
      bool                      found = false;

      if(!place) continue;

      for(XDWORD u=0; u<paths.GetSize(); u++)
        {
          if(paths.Get(u) && (paths.Get(u)->Compare(place->pathfile.Get(), true) == 0))
            {
              found = true;
              break;
            }
        }

      if(!found)
        {
          XPATH* path = GEN_NEW XPATH();
          if(path)
            {
              (*path) = place->pathfile;
              paths.Add(path);
            }
        }
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   converting %d _L to NL (no translatable text)"), (int)places.GetSize());

  for(XDWORD u=0; u<paths.GetSize(); u++)
    {
      XPATH*   path = paths.Get(u);
      XFILETXT filetxt;
      bool     changed = false;

      if(!path) continue;
      if(!filetxt.Open(*path, false) || !filetxt.ReadAllFile()) continue;

      for(int p=(int)places.GetSize()-1; p>=0; p--)
        {
          TRANSLATESCAN_LTONLPLACE* place = places.Get((XDWORD)p);
          XSTRING*                  line  = NULL;
          XSTRING                   head;
          XSTRING                   rebuilt;
          XSTRING                   tail;

          if(!place || (place->pathfile.Compare(path->Get(), true) != 0)) continue;

          line = filetxt.GetLine(place->nline);
          if(!line) continue;

          line->Copy(place->index, place->index + 3, head);
          if(head.Compare(NL("_L("), false) != 0) continue;

          line->Copy(0, place->index, rebuilt);
          rebuilt.Add(NL("NL("));
          line->Copy(place->index + 3, tail);
          rebuilt.Add(tail);

          filetxt.DeleteLine(place->nline);
          filetxt.InsertLine(place->nline, rebuilt);
          changed = true;

          TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   replace %d  _L->NL \"%s\""), place->nline + 1, place->literal.Get());
        }

      if(changed)
        {
          if(!filetxt.WriteAllFile())
            {
              status = false;
              TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   write failed  %s"), path->Get());
            }
           else
            {
              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   updated  %s"), path->Get());
            }
        }

      filetxt.Close();
    }

  paths.DeleteContents();
  paths.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_TranslateGENInternal(XPATH& operationdir)
* @brief      Extract missing GEN literals, translate them to the five GEN languages and replace them with GEN_TL.
* @ingroup    APPLICATION
*
* @param[in]  operationdir : GEN source directory.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_TranslateGENInternal(XPATH& operationdir)
{
  if(!modifiergen)           return false;
  if(operationdir.IsEmpty()) return false;

  #ifndef DIO_SCRAPERWEB_TRANSLATION_ACTIVE
  TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN translation scraper is not compiled in."));
  return false;
  #else

  XVECTOR<TRANSLATESCAN_FILETARGET*> files;
  XVECTOR<TRANSLATESCAN_GENTEXT*>    texts;
  XVECTOR<TRANSLATESCAN_GENPLACE*>   places;
  XVECTOR<TRANSLATESCAN_LTONLPLACE*> ltonl;
  XPATH                              idheader;
  XPATH                              idcpp;
  XFILETXT                           catalogue;
  XSTRING                            cataloguetext;
  static const XCHAR*                codename[5] = { NL("ENG"), NL("SPA"), NL("FRE"), NL("GER"), NL("CHI") };
  static const XCHAR*                alpha2[5]   = { NL("en"), NL("es"), NL("fr"), NL("de"), NL("zh") };
  bool                               status = true;
  int                                failed = 0;

  if(!SearchFilesTarget(operationdir, &files))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN no source files under %s"), operationdir.Get());
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN scan %s"), operationdir.Get());

  for(XDWORD f=0; f<files.GetSize(); f++)
    {
      TRANSLATESCAN_FILETARGET* filetarget = files.Get(f);
      XPATH*                    pathfile   = filetarget ? filetarget->GetXPathFile() : NULL;
      XFILETXT                  filetxt;

      if(!pathfile || TRANSLATESCAN_IsCatalogueFile(*pathfile)) continue;
      if(!filetxt.Open(*pathfile, true)) continue;
      if(!filetxt.ReadAllFile())
        {
          filetxt.Close();
          continue;
        }

      TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL(" > %s"), pathfile->Get());

      for(int nline=0; nline<(int)filetxt.GetNLines(); nline++)
        {
          XSTRING* line = filetxt.GetLine(nline);
          if(!line) continue;

          XCHAR* search[2] = { NL("_L("), NL("FL(") };

          for(int s=0; s<2; s++)
            {
              int index = 0;

              do{ index = line->Find(search[s], false, index);
                  if(index == XSTRING_NOTFOUND) break;

                  XSTRING literal;
                  TRANSLATESCAN_LITERALMACRO macro = Operation_LiteralStrings_GetMacroAt(line, index);

                  if((macro == TRANSLATESCAN_LITERALMACRO_L) || (macro == TRANSLATESCAN_LITERALMACRO_FL))
                    {
                      XCHAR* text = line->Get();
                      int    size = (int)line->GetSize();
                      int    start = index + 3;

                      while((start < size) && ((text[start] == _C(' ')) || (text[start] == _C('\t')))) start++;

                      if((start < size) && (text[start] == _C('"')))
                        {
                          int end = Operation_LiteralStrings_FindStringEnd(text, size, start + 1);

                          if(end != XSTRING_NOTFOUND)
                            {
                              line->Copy(start + 1, end, literal);

                              if(!Operation_LiteralStrings_IsApplicationName(line, index))
                                {
                                  if(!Operation_LiteralStrings_HasTranslatableText(literal))
                                    {
                                      if(macro == TRANSLATESCAN_LITERALMACRO_L)
                                        {
                                          TRANSLATESCAN_LTONLPLACE* place = GEN_NEW TRANSLATESCAN_LTONLPLACE();
                                          if(place)
                                            {
                                              place->pathfile = pathfile->Get();
                                              place->nline    = nline;
                                              place->index    = index;
                                              place->literal  = literal;
                                              ltonl.Add(place);

                                              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  [NOTEXT->_NL] \"%s\""), nline + 1, literal.Get());
                                            }
                                        }
                                    }
                                  else
                                    {
                                      XDWORD existing = 0;

                                      if(!Operation_LiteralStrings_IsGENInternal(literal, existing))
                                        {
                                          int textindex = -1;

                                          for(XDWORD t=0; t<texts.GetSize(); t++)
                                            {
                                              if(texts.Get(t)->literal.Compare(literal, false) == 0)
                                                {
                                                  textindex = (int)t;
                                                  break;
                                                }
                                            }

                                          if(textindex < 0)
                                            {
                                              TRANSLATESCAN_GENTEXT* entry = GEN_NEW TRANSLATESCAN_GENTEXT();
                                              if(entry)
                                                {
                                                  entry->pathfile = *pathfile;
                                                  entry->literal  = literal;
                                                  entry->fixed    = (macro == TRANSLATESCAN_LITERALMACRO_FL) ? (int)literal.GetSize() : 0;
                                                  entry->ready    = false;
                                                  // Provisional name from the source literal; rebuilt from English after translation.
                                                  TRANSLATESCAN_BuildIDName(*pathfile, literal, entry->idname);
                                                  textindex = (int)texts.GetSize();
                                                  texts.Add(entry);
                                                }
                                            }

                                          if(textindex >= 0)
                                            {
                                              TRANSLATESCAN_GENPLACE* place = GEN_NEW TRANSLATESCAN_GENPLACE();
                                              if(place)
                                                {
                                                  int tailindex = end + 1;

                                                  while((tailindex < size) && ((text[tailindex] == _C(' ')) || (text[tailindex] == _C('\t')))) tailindex++;
                                                  if((tailindex < size) && (text[tailindex] == _C(')'))) tailindex++;

                                                  place->pathfile  = pathfile->Get();
                                                  place->nline     = nline;
                                                  place->index     = index;
                                                  place->endquote  = tailindex - 1;
                                                  place->textindex = textindex;
                                                  places.Add(place);

                                                  {
                                                    TRANSLATESCAN_GENTEXT* shown = texts.Get((XDWORD)textindex);

                                                    if(shown)
                                                      {
                                                        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  %s  \"%s\""), nline + 1, shown->idname.Get(), literal.Get());
                                                      }
                                                  }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                  index += 3;
                } while(index != XSTRING_NOTFOUND);
            }
        }

      filetxt.Close();
    }

  idheader = operationdir;
  idheader.Slash_Add();
  idheader.Add(NL("XUtils"));
  idheader.Slash_Add();
  idheader.Add(NL("XTranslation_GEN_ID.h"));

  idcpp = operationdir;
  idcpp.Slash_Add();
  idcpp.Add(NL("XUtils"));
  idcpp.Slash_Add();
  idcpp.Add(NL("XTranslation_GEN.cpp"));

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN  %s"), idheader.Get());
  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN  %s"), idcpp.Get());

  if(catalogue.Open(idcpp, true) && catalogue.ReadAllFile())
    {
      for(int c=0; c<(int)catalogue.GetNLines(); c++)
        {
          XSTRING* line = catalogue.GetLine(c);
          if(line) cataloguetext.Add(line->Get());
        }
      catalogue.Close();
    }

  {
    DIOSCRAPERWEBTRANSLATION scraper;
    int                      pending = 0;
    int                      current = 0;

    for(XDWORD t=0; t<texts.GetSize(); t++)
      {
        TRANSLATESCAN_GENTEXT* entry = texts.Get(t);
        XSTRING                marker;

        if(!entry) continue;

        marker = NL("NL(\"");
        marker.Add(entry->literal);
        marker.Add(NL("\")"));

        if(cataloguetext.Find(marker.Get(), false) != XSTRING_NOTFOUND) continue;

        pending++;
      }

    if(pending > 0)
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN translating %d string(s). Each one needs several web requests."), pending);
      }
     else
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN no new strings to translate."));
      }

    for(XDWORD t=0; t<texts.GetSize(); t++)
      {
        TRANSLATESCAN_GENTEXT* entry = texts.Get(t);
        XSTRING                marker;
        DIOTRANSLATION_RESULT  detected;
        int                    origin = -1;

        XSTRING                human;
        XSTRING                autolang;
        XSTRING                english;
        XSTRING                english_for_id;

        if(!entry) continue;

        marker = NL("NL(\"");
        marker.Add(entry->literal);
        marker.Add(NL("\")"));

        if(cataloguetext.Find(marker.Get(), false) != XSTRING_NOTFOUND)
          {
            continue;
          }

        XVECTOR<XSTRING*>      masks;
        XSTRING                shielded;

        current++;
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   translate %d/%d  %s  \"%s\""), current, pending, entry->idname.Get(), entry->literal.Get());

        TRANSLATESCAN_Unescape(entry->literal, human);
        if(human.IsEmpty()) human = entry->literal;

        TRANSLATESCAN_ShieldPrintfMasks(human, shielded, masks);

        autolang = NL("auto");
        english  = NL("en");

        if(!scraper.Get(shielded, autolang, english, detected))
          {
            failed++;
            masks.DeleteContents();
            masks.DeleteAll();
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   failed  \"%s\""), entry->literal.Get());
            continue;
          }

        origin = TRANSLATESCAN_CatalogueLanguageIndex(detected.GetSourceLanguage());
        if(origin == 0) english_for_id = human;

        for(int lang=0; lang<5; lang++)
          {
            XSTRING targetcode;

            if(lang == origin)
              {
                entry->sentence[lang] = entry->literal;
                continue;
              }

            if((origin < 0) && (lang == 0))
              {
                XSTRING decoded;

                decoded = detected.GetTranslation();
                if(!TRANSLATESCAN_RestorePrintfMasks(decoded, masks))
                  {
                    failed++;
                    entry->ready = false;
                    TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   mask lost  \"%s\""), entry->literal.Get());
                    break;
                  }

                english_for_id = decoded;
                TRANSLATESCAN_EscapeForC(decoded, entry->sentence[lang]);
                continue;
              }

            targetcode = alpha2[lang];

            XSTRING translation;
            XSTRING sourcecode;

            sourcecode = (origin >= 0) ? alpha2[origin] : detected.GetSourceLanguage();
            if(sourcecode.IsEmpty()) sourcecode = english;

            TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("     -> %s"), targetcode.Get());

            if(!scraper.Get(shielded, sourcecode, targetcode, translation) ||
               !TRANSLATESCAN_RestorePrintfMasks(translation, masks))
              {
                failed++;
                entry->ready = false;
                TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   mask lost  \"%s\""), entry->literal.Get());
                break;
              }

            if(lang == 0) english_for_id = translation;
            TRANSLATESCAN_EscapeForC(translation, entry->sentence[lang]);
          }

        masks.DeleteContents();
        masks.DeleteAll();

        if(entry->sentence[0].IsEmpty() || entry->sentence[1].IsEmpty() || entry->sentence[2].IsEmpty() ||
           entry->sentence[3].IsEmpty() || entry->sentence[4].IsEmpty())
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   incomplete  \"%s\""), entry->literal.Get());
            continue;
          }

        if(english_for_id.IsEmpty()) english_for_id = human;
        TRANSLATESCAN_BuildIDName(entry->pathfile, english_for_id, entry->idname);

        entry->ready = true;
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   ready  %s  \"%s\""), entry->idname.Get(), entry->literal.Get());
      }
  }

  {
    int ready = 0;
    for(XDWORD t=0; t<texts.GetSize(); t++)
      {
        if(texts.Get(t) && texts.Get(t)->ready) ready++;
      }

    if(ready == 0)
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN finished with no catalogue changes. failed=%d"), failed);
        TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);
        TRANSLATESCAN_Report(status ? XTRACE_COLOR_GREEN : XTRACE_COLOR_RED, NL("GEN done. status=%d failed=%d"), status ? 1 : 0, failed);
        texts.DeleteContents();
        texts.DeleteAll();
        places.DeleteContents();
        places.DeleteAll();
        ltonl.DeleteContents();
        ltonl.DeleteAll();
        files.DeleteContents();
        files.DeleteAll();
        return (failed == 0) && status;
      }

    TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN writing catalogue (%d ready)."), ready);
  }

  {
    XFILETXT header;

    if(!header.Open(idheader, false) || !header.ReadAllFile())
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN could not open %s"), idheader.Get());
        TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);
        texts.DeleteContents();
        texts.DeleteAll();
        places.DeleteContents();
        places.DeleteAll();
        ltonl.DeleteContents();
        ltonl.DeleteAll();
        files.DeleteContents();
        files.DeleteAll();
        return false;
      }

    int last = -1;
    XSTRING headertext;

    for(int c=0; c<(int)header.GetNLines(); c++)
      {
        XSTRING* line = header.GetLine(c);
        if(!line) continue;

        headertext.Add(line->Get());
        headertext.Add(_C('\n'));

        if(line->Find(NL("XTRANSLATION_GEN_ID_LAST"), false) != XSTRING_NOTFOUND) last = c;
      }

    if(last < 0)
      {
        header.Close();
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN LAST enumerator not found in %s"), idheader.Get());
        TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);
        texts.DeleteContents();
        texts.DeleteAll();
        places.DeleteContents();
        places.DeleteAll();
        ltonl.DeleteContents();
        ltonl.DeleteAll();
        files.DeleteContents();
        files.DeleteAll();
        return false;
      }

    for(XDWORD t=0; t<texts.GetSize(); t++)
      {
        TRANSLATESCAN_GENTEXT* entry = texts.Get(t);
        XSTRING                base;
        int                    suffix = 2;

        if(!entry || !entry->ready) continue;

        base = entry->idname;

        while(TRANSLATESCAN_IDIsUsed(entry->idname, headertext))
          {
            XSTRING extra;

            extra.Format(NL("_%d"), suffix);
            entry->idname = base;
            entry->idname.Add(extra);
            suffix++;
          }

        headertext.Add(entry->idname);
        headertext.Add(NL(" ,"));
      }

    for(int t=(int)texts.GetSize()-1; t>=0; t--)
      {
        TRANSLATESCAN_GENTEXT* entry = texts.Get(t);
        XSTRING                line;

        if(!entry || !entry->ready) continue;

        line = NL("  ");
        line.Add(entry->idname);
        line.Add(NL(" ,"));
        header.InsertLine(last, line);
      }

    if(!header.WriteAllFile())
      {
        status = false;
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN write failed  %s"), idheader.Get());
      }
     else
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN updated  %s"), idheader.Get());
      }
    header.Close();
  }

  {
    XFILETXT source;

    if(!source.Open(idcpp, false) || !source.ReadAllFile())
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN could not open %s"), idcpp.Get());
        TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);
        texts.DeleteContents();
        texts.DeleteAll();
        places.DeleteContents();
        places.DeleteAll();
        ltonl.DeleteContents();
        ltonl.DeleteAll();
        files.DeleteContents();
        files.DeleteAll();
        return false;
      }

    int last = -1;
    for(int c=0; c<(int)source.GetNLines(); c++)
      {
        XSTRING* line = source.GetLine(c);
        if(line && (line->Find(NL("Sentence_Add("), false) != XSTRING_NOTFOUND)) last = c;
      }

    if(last < 0)
      {
        source.Close();
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN Sentence_Add insert point not found in %s"), idcpp.Get());
        TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);
        texts.DeleteContents();
        texts.DeleteAll();
        places.DeleteContents();
        places.DeleteAll();
        ltonl.DeleteContents();
        ltonl.DeleteAll();
        files.DeleteContents();
        files.DeleteAll();
        return false;
      }

    {
      XSTRING comment = NL("");
      source.InsertLine(last + 1, comment);
      last++;
      comment = NL("  //--- TRANSLATESCAN ------------------------------------------------------------------------------------------");
      source.InsertLine(last + 1, comment);
      last++;
      comment.Empty();
      source.InsertLine(last + 1, comment);
      last++;
    }

    for(XDWORD t=0; t<texts.GetSize(); t++)
      {
        TRANSLATESCAN_GENTEXT* entry = texts.Get(t);
        if(!entry || !entry->ready) continue;

        // One identifier, then ENG, SPA, FRE, GER and CHI. Same layout as the .lng entry and XTranslation_GEN.cpp.
        for(int lang=0; lang<5; lang++)
          {
            XSTRING line;
            line.Format(NL("  Sentence_Add(%s, XLANGUAGE_ISO_639_3_CODE_%s, NL(\"%s\"), %d);"),
                        entry->idname.Get(), codename[lang], entry->sentence[lang].Get(), entry->fixed);
            last++;
            source.InsertLine(last, line);
          }

        XSTRING blank = NL("");
        last++;
        source.InsertLine(last, blank);
      }

    if(!source.WriteAllFile())
      {
        status = false;
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN write failed  %s"), idcpp.Get());
      }
     else
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN updated  %s"), idcpp.Get());
      }
    source.Close();
  }

  for(XDWORD f=0; f<files.GetSize(); f++)
    {
      XPATH*   pathfile = files.Get(f) ? files.Get(f)->GetXPathFile() : NULL;
      XFILETXT filetxt;
      bool     changed = false;

      if(!pathfile) continue;

      if(!filetxt.Open(*pathfile, false) || !filetxt.ReadAllFile()) continue;

      for(int p=(int)places.GetSize()-1; p>=0; p--)
        {
          TRANSLATESCAN_GENPLACE* place = places.Get(p);
          TRANSLATESCAN_GENTEXT*  entry = NULL;
          XSTRING*                line  = NULL;
          XSTRING                 rebuilt;
          XSTRING                 tail;

          if(!place || (place->textindex < 0) || (place->textindex >= (int)texts.GetSize())) continue;
          entry = texts.Get(place->textindex);
          if(!entry || !entry->ready) continue;
          if(place->pathfile.Compare(pathfile->Get(), true) != 0) continue;

          line = filetxt.GetLine(place->nline);
          if(!line) continue;

          line->Copy(0, place->index, rebuilt);
          rebuilt.Add(NL("GEN_TL("));
          rebuilt.Add(entry->idname);
          rebuilt.Add(NL(")"));
          line->Copy(place->endquote + 1, tail);
          rebuilt.Add(tail);

          filetxt.DeleteLine(place->nline);
          filetxt.InsertLine(place->nline, rebuilt);
          changed = true;

          TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   replace %d  GEN_TL(%s)"), place->nline + 1, entry->idname.Get());
        }

      if(changed)
        {
          bool hasinclude = false;
          int  insertat   = 0;

          for(int c=0; c<(int)filetxt.GetNLines(); c++)
            {
              XSTRING* line = filetxt.GetLine(c);
              if(!line) continue;
              if(line->Find(NL("XTranslation_GEN.h"), false) != XSTRING_NOTFOUND) hasinclude = true;
              if(line->Find(NL("#include \"GEN_Defines.h\""), false) != XSTRING_NOTFOUND) insertat = c + 1;
            }

          if(!hasinclude)
            {
              XSTRING include = NL("#include \"XTranslation_GEN.h\"");
              filetxt.InsertLine(insertat, include);
              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   include XTranslation_GEN.h"));
            }

          if(!filetxt.WriteAllFile())
            {
              status = false;
              TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("GEN write failed  %s"), pathfile->Get());
            }
           else
            {
              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("GEN updated  %s"), pathfile->Get());
            }
        }

      filetxt.Close();
    }

  TRANSLATESCAN_ApplyLToNLPlaces(ltonl, status);

  TRANSLATESCAN_Report(status ? XTRACE_COLOR_GREEN : XTRACE_COLOR_RED, NL("GEN done. status=%d failed=%d"), status ? 1 : 0, failed);

  texts.DeleteContents();
  texts.DeleteAll();
  places.DeleteContents();
  places.DeleteAll();
  ltonl.DeleteContents();
  ltonl.DeleteAll();
  files.DeleteContents();
  files.DeleteAll();

  return status;

  #endif
}


class TRANSLATESCAN_APPTEXT
{
  public:
    TRANSLATESCAN_APPTEXT()
    {
      fixed      = 0;
      ready      = false;
      addtolng   = false;
      addtoenum  = false;
      idnumber   = 0;
    }

    XPATH   pathfile;
    XSTRING literal;
    XSTRING human;
    XSTRING idname;
    XSTRING sentence[5];
    int     fixed;
    bool    ready;
    bool    addtolng;
    bool    addtoenum;
    int     idnumber;
};


class TRANSLATESCAN_APPPLACE
{
  public:
    XPATH pathfile;
    int   nline;
    int   index;
    int   endquote;
    int   textindex;
};


class TRANSLATESCAN_APPENUM
{
  public:
    TRANSLATESCAN_APPENUM()
    {
      idnumber = 0;
    }

    int     idnumber;
    XSTRING idname;
};


class TRANSLATESCAN_APPEXISTING
{
  public:
    TRANSLATESCAN_APPEXISTING()
    {
      idnumber = 0;
      ntext    = 0;
    }

    int     idnumber;
    XSTRING idname;
    XSTRING text[16];
    int     ntext;
};


class TRANSLATESCAN_APPSCOPE
{
  public:
    XVECTOR<TRANSLATESCAN_FILETARGET*> files;
    XVECTOR<TRANSLATESCAN_APPTEXT*>    texts;
    XVECTOR<TRANSLATESCAN_APPPLACE*>   places;
    XVECTOR<TRANSLATESCAN_APPENUM*>    enums;
    XVECTOR<TRANSLATESCAN_APPEXISTING*> existing;
    XVECTOR<XPATH*>                    lngfiles;
    XVECTOR<TRANSLATESCAN_LTONLPLACE*>  ltonl;

    ~TRANSLATESCAN_APPSCOPE()
    {
      files.DeleteContents();
      texts.DeleteContents();
      places.DeleteContents();
      ltonl.DeleteContents();
      enums.DeleteContents();
      existing.DeleteContents();
      lngfiles.DeleteContents();
    }
};


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_CollectLng(XCHAR* inipath, XVECTOR<XPATH*>* found)
* @brief      Collect .lng files under an application directory.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_CollectLng(XCHAR* inipath, XVECTOR<XPATH*>* found)
{
  XDIR*       xdir = NULL;
  XDIRELEMENT element;

  if(!inipath || !found) return;

  xdir = GEN_XFACTORY.Create_Dir();
  if(!xdir) return;

  if(xdir->FirstSearch(inipath, _L("*"), &element))
    {
      do{
          if(!element.GetNameFile()) continue;
          if(!element.GetNameFile()->Compare(NL("."), true))  continue;
          if(!element.GetNameFile()->Compare(NL(".."), true)) continue;

          if(element.GetType() == XDIRELEMENTTYPE_DIR)
            {
              XPATH subpath;

              subpath = inipath;
              subpath.Slash_Add();
              subpath.Add(element.GetNameFile()->Get());
              TRANSLATESCAN_CollectLng(subpath.Get(), found);
            }

          if(element.GetType() == XDIRELEMENTTYPE_FILE)
            {
              XSTRING ext;

              element.GetNameFile()->GetExt(ext);
              if(!ext.Compare(NL(".lng"), true))
                {
                  XPATH* path = GEN_NEW XPATH();
                  if(path)
                    {
                      path->Set(inipath);
                      path->Slash_Add();
                      path->Add(element.GetNameFile()->Get());
                      found->Add(path);
                    }
                }
            }

        } while(xdir->NextSearch(&element));
    }

  GEN_XFACTORY.Delete_Dir(xdir);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_EndsWith(XSTRING& text, XCHAR* suffix)
* @brief      True when text ends with suffix.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_EndsWith(XSTRING& text, XCHAR* suffix)
{
  XSTRING tail;
  XSTRING mark;
  int     marks = 0;

  if(!suffix) return false;

  mark  = suffix;
  marks = (int)mark.GetSize();
  if((int)text.GetSize() < marks) return false;

  text.Copy((int)text.GetSize() - marks, tail);

  return tail.Compare(mark, true) == 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_HeaderHasIdentifier(XSTRING& name, XSTRING& headertext)
* @brief      True when name is already a whole identifier in the header text.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_HeaderHasIdentifier(XSTRING& name, XSTRING& headertext)
{
  int from = 0;

  if(name.IsEmpty() || !headertext.Get()) return false;

  while(from < (int)headertext.GetSize())
    {
      int    at    = headertext.Find(name.Get(), false, from);
      XCHAR* text  = headertext.Get();
      int    size  = (int)headertext.GetSize();
      int    after = 0;
      bool   beforeok;
      bool   afterok;

      if((at == XSTRING_NOTFOUND) || !text) return false;

      after = at + (int)name.GetSize();

      beforeok = (at == 0) ||
                 ((text[at - 1] != _C('_')) &&
                  !headertext.Character_IsAlpha(text[at - 1]) &&
                  !headertext.Character_IsNumber(text[at - 1], false));

      afterok = (after >= size) ||
                ((text[after] != _C('_')) &&
                 !headertext.Character_IsAlpha(text[after]) &&
                 !headertext.Character_IsNumber(text[after], false));

      if(beforeok && afterok) return true;

      from = at + 1;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_ReadCIdentifier(XSTRING& line, int start, XSTRING& ident, int& next)
* @brief      Read one C identifier from a line.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_ReadCIdentifier(XSTRING& line, int start, XSTRING& ident, int& next)
{
  XCHAR* text = line.Get();
  int    size = (int)line.GetSize();
  int    c    = start;

  ident.Empty();
  next = start;
  if(!text) return false;

  while((c < size) && ((text[c] == _C(' ')) || (text[c] == _C('\t')))) c++;
  if(c >= size) return false;
  if(!line.Character_IsAlpha(text[c]) && (text[c] != _C('_'))) return false;

  while((c < size) && (line.Character_IsAlpha(text[c]) || line.Character_IsNumber(text[c], false) || (text[c] == _C('_'))))
    {
      ident.Add(text[c]);
      c++;
    }

  next = c;

  return !ident.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_ReadIntAfter(XSTRING& line, int start, int& value)
* @brief      Read an integer that follows an enumerator assignment.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_ReadIntAfter(XSTRING& line, int start, int& value)
{
  XCHAR* text = line.Get();
  int    size = (int)line.GetSize();
  int    c    = start;
  bool   neg  = false;

  value = 0;
  if(!text) return false;

  while((c < size) && ((text[c] == _C(' ')) || (text[c] == _C('\t')) || (text[c] == _C('=')))) c++;
  if((c < size) && (text[c] == _C('-')))
    {
      neg = true;
      c++;
    }

  if((c >= size) || !line.Character_IsNumber(text[c], false)) return false;

  while((c < size) && line.Character_IsNumber(text[c], false))
    {
      value = (value * 10) + (int)(text[c] - _C('0'));
      c++;
    }

  if(neg) value = -value;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int TRANSLATESCAN_ReadJSONString(XSTRING& line, int start, XSTRING& raw)
* @brief      Read the next JSON string on a line. Returns the index after the closing quote.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static int TRANSLATESCAN_ReadJSONString(XSTRING& line, int start, XSTRING& raw)
{
  XCHAR* text = line.Get();
  int    size = (int)line.GetSize();
  int    c    = start;

  raw.Empty();
  if(!text) return XSTRING_NOTFOUND;

  while((c < size) && (text[c] != _C('"'))) c++;
  if(c >= size) return XSTRING_NOTFOUND;

  c++;

  while(c < size)
    {
      if((text[c] == _C('\\')) && ((c + 1) < size))
        {
          raw.Add(_C('\\'));
          raw.Add(text[c + 1]);
          c += 2;
          continue;
        }

      if(text[c] == _C('"')) return c + 1;

      raw.Add(text[c]);
      c++;
    }

  return XSTRING_NOTFOUND;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_AllDigits(XSTRING& text)
* @brief      True when every character is a decimal digit.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_AllDigits(XSTRING& text)
{
  if(text.IsEmpty() || !text.Get()) return false;

  for(XDWORD c=0; c<text.GetSize(); c++)
    {
      if(!text.Character_IsNumber(text.Get()[c], false)) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_NextIs(XSTRING& line, int index, XCHAR expected)
* @brief      True when the next non-space character is expected.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_NextIs(XSTRING& line, int index, XCHAR expected)
{
  XCHAR* text = line.Get();
  int    size = (int)line.GetSize();

  if(!text) return false;

  while((index < size) && ((text[index] == _C(' ')) || (text[index] == _C('\t')))) index++;

  return (index < size) && (text[index] == expected);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static XCHAR TRANSLATESCAN_FirstChar(XSTRING* line)
* @brief      First character that is not a space or a tab.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static XCHAR TRANSLATESCAN_FirstChar(XSTRING* line)
{
  XCHAR* text;
  int    size;
  int    c = 0;

  if(!line || !line->Get()) return 0;

  text = line->Get();
  size = (int)line->GetSize();

  while((c < size) && ((text[c] == _C(' ')) || (text[c] == _C('\t')))) c++;
  if(c >= size) return 0;

  return text[c];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int TRANSLATESCAN_Indent(XSTRING* line)
* @brief      Count of leading spaces.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static int TRANSLATESCAN_Indent(XSTRING* line)
{
  int c    = 0;
  int size = 0;

  if(!line || !line->Get()) return 0;

  size = (int)line->GetSize();
  while((c < size) && (line->Get()[c] == _C(' '))) c++;

  return c;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_Pad(XSTRING& line, int spaces)
* @brief      Replace a line with a run of spaces.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_Pad(XSTRING& line, int spaces)
{
  line.Empty();
  if(spaces < 0) spaces = 0;

  for(int c=0; c<spaces; c++) line.Add(_C(' '));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static int TRANSLATESCAN_AppLanguageSlot(XSTRING& name)
* @brief      Map a .lng language name onto ENG, SPA, FRE, GER, CHI. Returns -1 otherwise.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static int TRANSLATESCAN_AppLanguageSlot(XSTRING& name)
{
  if(name.Find(NL("Chinese"), true) != XSTRING_NOTFOUND) return 4;
  if(name.Find(NL("Chino"), true)   != XSTRING_NOTFOUND) return 4;
  if(name.Find(NL("French"), true)  != XSTRING_NOTFOUND) return 2;
  if(name.Find(NL("Franc"), true)   != XSTRING_NOTFOUND) return 2;
  if(name.Find(NL("German"), true)  != XSTRING_NOTFOUND) return 3;
  if(name.Find(NL("Deutsch"), true) != XSTRING_NOTFOUND) return 3;
  if(name.Find(NL("Alem"), true)    != XSTRING_NOTFOUND) return 3;
  if(name.Find(NL("Spanish"), true) != XSTRING_NOTFOUND) return 1;
  if(name.Find(NL("Espa"), true)    != XSTRING_NOTFOUND) return 1;
  if(name.Find(NL("English"), true) != XSTRING_NOTFOUND) return 0;
  if(name.Find(NL("Ingl"), true)    != XSTRING_NOTFOUND) return 0;

  return -1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_EscapeForJSON(XSTRING& text, XSTRING& escaped)
* @brief      Escape a sentence so it can sit inside a JSON string. Unicode stays as characters.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_EscapeForJSON(XSTRING& text, XSTRING& escaped)
{
  escaped.Empty();

  for(XDWORD c=0; c<text.GetSize(); c++)
    {
      XCHAR character = text.Get()[c];

      if((character == _C('\\')) || (character == _C('"')))
        {
          escaped.Add(_C('\\'));
          escaped.Add(character);
          continue;
        }

      if(character == _C('\n')) { escaped.Add(NL("\\n")); continue; }
      if(character == _C('\r')) { escaped.Add(NL("\\r")); continue; }
      if(character == _C('\t')) { escaped.Add(NL("\\t")); continue; }
      if(character < 0x20) continue;

      escaped.Add(character);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_BuildAppIDName(XSTRING& prefix, XPATH& pathfile, XSTRING& literal, XSTRING& idname)
* @brief      Build <enum>_<file>_<words> with the same rules as a GEN identifier.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_BuildAppIDName(XSTRING& prefix, XPATH& pathfile, XSTRING& literal, XSTRING& idname)
{
  XSTRING module;
  XSTRING words;
  XCHAR*  text = literal.Get();
  int     size = (int)literal.GetSize();

  pathfile.GetNamefile(module);
  module.ToUpperCase();

  idname = prefix;

  for(XDWORD c=0; c<module.GetSize(); c++)
    {
      TRANSLATESCAN_AppendIdentifierChar(idname, module.Get()[c]);
    }

  if((idname.GetSize() > 0) && (idname.Character_GetLast() != _C('_')))
    {
      idname.Add(_C('_'));
    }

  for(int c=0; text && (c<size); )
    {
      if(text[c] == _C('%'))
        {
          c = TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipPrintfMask(text, size, c);
          TRANSLATESCAN_AppendIdentifierChar(words, _C('_'));
          continue;
        }

      if(text[c] == _C('\\'))
        {
          c = TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipEscape(text, size, c);
          continue;
        }

      TRANSLATESCAN_AppendIdentifierChar(words, text[c]);
      c++;
    }

  if(!words.IsEmpty() && (words.Character_GetLast() == _C('_')))
    {
      words.DeleteLastCharacter();
    }

  if(words.IsEmpty())
    {
      words = NL("TEXT");
    }

  if(words.GetSize() > 48)
    {
      words.DeleteCharactersToEnd(48);
      if(words.Character_GetLast() == _C('_')) words.DeleteLastCharacter();
    }

  if(words.Character_IsNumber(words.Character_GetFirst(), false))
    {
      idname.Add(_C('N'));
    }

  idname.Add(words);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_MakeUniqueAppID(XSTRING& idname, XSTRING& headertext)
* @brief      Append _2, _3... until the identifier is free, then reserve it.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_MakeUniqueAppID(XSTRING& idname, XSTRING& headertext)
{
  XSTRING base = idname;
  int     suffix = 2;

  while(TRANSLATESCAN_HeaderHasIdentifier(idname, headertext))
    {
      XSTRING extra;

      extra.Format(NL("_%d"), suffix);
      idname = base;
      idname.Add(extra);
      suffix++;
    }

  headertext.Add(idname);
  headertext.Add(NL(" ,"));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_WriteLinesUTF8(XPATH& path, XFILETXT& text)
* @brief      Write text lines as UTF-8 without a BOM, using CR LF.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_WriteLinesUTF8(XPATH& path, XFILETXT& text)
{
  XFILE*  file = NULL;
  XBUFFER all;
  bool    ok   = true;

  for(int c=0; c<(int)text.GetNLines(); c++)
    {
      XSTRING* line = text.GetLine(c);

      if(line && !line->IsEmpty())
        {
          XBUFFER xbuffer;
          int     nbytes = ((int)line->GetSize() * 4) + 8;

          if(nbytes < 8) nbytes = 8;
          xbuffer.Resize((XDWORD)nbytes);

          // addzeroatend stays true: the returned size is the UTF-8 byte count and a NUL sits after it.
          if(!line->ConvertToUTF8(xbuffer.Get(), nbytes) || (nbytes <= 0)) return false;
          if(!all.Add(xbuffer.Get(), (XDWORD)nbytes)) return false;
        }

      if(!all.Add((XBYTE)0x0D) || !all.Add((XBYTE)0x0A)) return false;
    }

  file = GEN_XFACTORY.Create_File();
  if(!file) return false;

  if(!file->Create(path))
    {
      GEN_XFACTORY.Delete_File(file);
      return false;
    }

  if(all.GetSize() && !file->Write(all)) ok = false;

  file->Close();
  GEN_XFACTORY.Delete_File(file);

  return ok;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_EnsureDirectory(XPATH& path)
* @brief      Create a directory tree when it is missing.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_EnsureDirectory(XPATH& path)
{
  XDIR* xdir = GEN_XFACTORY.Create_Dir();
  bool  ok   = false;

  if(!xdir) return false;

  if(xdir->Exist(path)) ok = true;
   else
    {
      ok = xdir->Make(path, true);
      if(ok) TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP created directory  %s"), path.Get());
       else  TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not create directory  %s"), path.Get());
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return ok;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_AppResolveLayout(XPATH& operationdir, XSTRING& appname, XPATH& approot, XPATH& sourcesdir, XPATH& assetsdir)
* @brief      Resolve application name plus Sources/ and assets/ from the scan root.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_AppResolveLayout(XPATH& operationdir, XSTRING& appname, XPATH& approot, XPATH& sourcesdir, XPATH& assetsdir)
{
  XPATH   root = operationdir;
  XSTRING leaf;
  XDIR*   xdir = NULL;

  if(root.Slash_HaveAtLast()) root.Slash_Delete();
  root.GetNamefile(leaf);

  if(leaf.Compare(NL("Sources"), true) == 0)
    {
      XSTRING parent;

      sourcesdir = root;
      root.GetDriveAndPath(parent);
      approot = parent;
      if(approot.Slash_HaveAtLast()) approot.Slash_Delete();
      approot.GetNamefile(appname);
    }
   else
    {
      appname  = leaf;
      approot  = root;
      sourcesdir = root;
      sourcesdir.Slash_Add();
      sourcesdir.Add(NL("Sources"));

      xdir = GEN_XFACTORY.Create_Dir();
      if(xdir && !xdir->Exist(sourcesdir)) sourcesdir = root;
      if(xdir) GEN_XFACTORY.Delete_Dir(xdir);
    }

  if(appname.IsEmpty()) appname = leaf;

  assetsdir = approot;
  assetsdir.Slash_Add();
  assetsdir.Add(NL("assets"));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static void TRANSLATESCAN_AppBuildEnumName(XSTRING& appname, XSTRING& enumname)
* @brief      Build APP_TRANSLATEID-style enum name from the application folder name.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static void TRANSLATESCAN_AppBuildEnumName(XSTRING& appname, XSTRING& enumname)
{
  XSTRING upper = appname;

  upper.ToUpperCase();
  enumname.Empty();

  for(XDWORD c=0; c<upper.GetSize(); c++)
    {
      XCHAR character = upper.Get()[c];

      if(((character >= _C('A')) && (character <= _C('Z'))) ||
         ((character >= _C('0')) && (character <= _C('9'))))
        {
          enumname.Add(character);
          continue;
        }

      if(enumname.IsEmpty() || (enumname.Character_GetLast() == _C('_'))) continue;
      enumname.Add(_C('_'));
    }

  if(!enumname.IsEmpty() && (enumname.Character_GetLast() == _C('_'))) enumname.DeleteLastCharacter();
  if(enumname.IsEmpty()) enumname = NL("APP");
  enumname.Add(NL("_TRANSLATEID"));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_CreateMinTranslateIDHeader(XPATH& idheader, XSTRING& appname)
* @brief      Write a minimal TranslateID header so APP can start a catalogue.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_CreateMinTranslateIDHeader(XPATH& idheader, XSTRING& appname)
{
  XFILETXT text;
  XSTRING  enumname;
  XSTRING  line;
  XSTRING  unknown;
  XSTRING  last;

  TRANSLATESCAN_AppBuildEnumName(appname, enumname);

  unknown = enumname;
  unknown.Add(NL("_UNKNOWN"));
  last = enumname;
  last.Add(NL("_LAST"));

  line.Format(NL("/**-------------------------------------------------------------------------------------------------------------------"));
  text.AddLine(line);
  line.Format(NL("*"));
  text.AddLine(line);
  line.Format(NL("* @file       %s_TranslateID.h"), appname.Get());
  text.AddLine(line);
  line.Format(NL("*"));
  text.AddLine(line);
  line.Format(NL("* @brief      Identifiers of the %s translation sentences"), appname.Get());
  text.AddLine(line);
  line.Format(NL("* @ingroup    APPLICATION"));
  text.AddLine(line);
  line.Format(NL("*"));
  text.AddLine(line);
  line.Format(NL("* --------------------------------------------------------------------------------------------------------------------*/"));
  text.AddLine(line);
  text.AddLine(NL(""));
  text.AddLine(NL("#pragma once"));
  text.AddLine(NL(""));
  text.AddLine(NL("/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/"));
  text.AddLine(NL(""));
  text.AddLine(NL(""));
  text.AddLine(NL(""));
  text.AddLine(NL("/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/"));
  text.AddLine(NL(""));
  line.Format(NL("enum %s"), enumname.Get());
  text.AddLine(line);
  text.AddLine(NL("{"));
  line.Format(NL("  %s                                         = 0 ,"), unknown.Get());
  text.AddLine(line);
  text.AddLine(NL(""));
  line.Format(NL("  %s"), last.Get());
  text.AddLine(line);
  text.AddLine(NL("};"));
  text.AddLine(NL(""));
  text.AddLine(NL(""));
  text.AddLine(NL(""));
  text.AddLine(NL("/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/"));

  if(!TRANSLATESCAN_WriteLinesUTF8(idheader, text))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not create  %s"), idheader.Get());
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP created  %s"), idheader.Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_CreateMinLng(XPATH& lngpath)
* @brief      Write a minimal five-language .lng catalogue.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_CreateMinLng(XPATH& lngpath)
{
  XFILETXT text;

  text.AddLine(NL("{"));
  text.AddLine(NL("  \"languages\"       : [ \"Spanish\" ,"));
  text.AddLine(NL("                        \"English\" ,"));
  text.AddLine(NL("                        \"French\"  ,"));
  text.AddLine(NL("                        \"German\"  ,"));
  text.AddLine(NL("                        \"Chinese\""));
  text.AddLine(NL("                      ],"));
  text.AddLine(NL(""));
  text.AddLine(NL("  \"translations\"    : {"));
  text.AddLine(NL("                      }"));
  text.AddLine(NL("}"));

  if(!TRANSLATESCAN_WriteLinesUTF8(lngpath, text))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not create  %s"), lngpath.Get());
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP created  %s"), lngpath.Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         static bool TRANSLATESCAN_UpgradeAppLngLanguages(XPATH& lngpath)
* @brief      Ensure the .lng languages list includes ENG, SPA, FRE, GER and CHI.
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
static bool TRANSLATESCAN_UpgradeAppLngLanguages(XPATH& lngpath)
{
  XFILETXT lng;
  int      translationsline = -1;
  int      firstlangline    = -1;
  int      lastlangline     = -1;
  int      nlang            = 0;
  bool     langseen[5];
  bool     inlanguages      = false;
  bool     languagesnamed   = false;

  for(int c=0; c<5; c++) langseen[c] = false;

  if(!lng.Open(lngpath, false) || !lng.ReadAllFile()) return false;

  for(int c=0; c<(int)lng.GetNLines(); c++)
    {
      XSTRING* line = lng.GetLine(c);
      if(line && (line->Find(NL("\"translations\""), false) != XSTRING_NOTFOUND))
        {
          translationsline = c;
          break;
        }
    }

  if(translationsline < 0)
    {
      lng.Close();
      return false;
    }

  for(int c=0; c<translationsline; c++)
    {
      XSTRING* line   = lng.GetLine(c);
      int      cursor = 0;

      if(!line) continue;
      if(!inlanguages && (line->Find(NL("\"languages\""), false) != XSTRING_NOTFOUND))
        {
          inlanguages   = true;
          firstlangline = c;
        }
      if(!inlanguages) continue;

      do{ XSTRING raw;
          int     next = TRANSLATESCAN_ReadJSONString(*line, cursor, raw);

          if(next == XSTRING_NOTFOUND) break;

          if(!languagesnamed && (raw.Compare(NL("languages"), true) == 0))
            {
              languagesnamed = true;
            }
          else if(languagesnamed)
            {
              int slot = TRANSLATESCAN_AppLanguageSlot(raw);
              if((slot >= 0) && (slot < 5)) langseen[slot] = true;
              nlang++;
            }

          cursor = next;
        } while(cursor != XSTRING_NOTFOUND);

      if(TRANSLATESCAN_FirstChar(line) == _C(']'))
        {
          lastlangline = c;
          break;
        }
    }

  {
    bool complete = true;

    for(int c=0; c<5; c++)
      {
        if(!langseen[c]) complete = false;
      }

    if(complete && (nlang >= 5))
      {
        lng.Close();
        return true;
      }
  }

  if((firstlangline < 0) || (lastlangline < 0) || (lastlangline < firstlangline))
    {
      lng.Close();
      return false;
    }

  for(int c=lastlangline; c>=firstlangline; c--) lng.DeleteLine(c);

  lng.InsertLine(firstlangline    , NL("  \"languages\"       : [ \"Spanish\" ,"));
  lng.InsertLine(firstlangline + 1, NL("                        \"English\" ,"));
  lng.InsertLine(firstlangline + 2, NL("                        \"French\"  ,"));
  lng.InsertLine(firstlangline + 3, NL("                        \"German\"  ,"));
  lng.InsertLine(firstlangline + 4, NL("                        \"Chinese\""));
  lng.InsertLine(firstlangline + 5, NL("                      ],"));

  lng.Close();

  if(!TRANSLATESCAN_WriteLinesUTF8(lngpath, lng))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not update languages in %s"), lngpath.Get());
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP updated languages in  %s"), lngpath.Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_TranslateAPP(XPATH& operationdir)
* @brief      Extract application literals, translate them and replace them with TL().
* @ingroup    APPLICATION
*
* @param[in]  operationdir : application directory.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_TranslateAPP(XPATH& operationdir)
{
  if(!modifierapp)           return false;
  if(operationdir.IsEmpty()) return false;

  #ifndef DIO_SCRAPERWEB_TRANSLATION_ACTIVE
  TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP translation scraper is not compiled in."));
  return false;
  #else

  TRANSLATESCAN_APPSCOPE scope;
  XPATH                  idheader;
  XPATH                  lngpath;
  XSTRING                enumname;
  XSTRING                idprefix;
  XSTRING                headertext;
  XSTRING                dirname;
  XSTRING                appname;
  XPATH                  approot;
  XPATH                  sourcesdir;
  XPATH                  assetsdir;
  XPATH                  root;
  int                    nheaders   = 0;
  int                    enumnext   = 0;
  int                    enumlast   = -1;
  int                    nlang      = 0;
  int                    langorder[16];
  bool                   langseen[5];
  int                    keyindent      = -1;
  int                    sentenceindent = -1;
  int                    bracketindent  = -1;
  int                    lastbracket    = -1;
  int                    translationsline = -1;
  int                    failed = 0;
  bool                   status = true;
  static const XCHAR*    alpha2[5] = { NL("en"), NL("es"), NL("fr"), NL("de"), NL("zh") };

  for(int c=0; c<5; c++) langseen[c] = false;
  for(int c=0; c<16; c++) langorder[c] = -1;

  if(!SearchFilesTarget(operationdir, &scope.files)) return false;

  root = operationdir;
  if(root.Slash_HaveAtLast()) root.Slash_Delete();
  TRANSLATESCAN_AppResolveLayout(root, appname, approot, sourcesdir, assetsdir);
  dirname = appname;

  for(XDWORD f=0; f<scope.files.GetSize(); f++)
    {
      XPATH*  pathfile = scope.files.Get(f) ? scope.files.Get(f)->GetXPathFile() : NULL;
      XSTRING namefile;
      XSTRING ext;
      XSTRING expected;

      if(!pathfile) continue;

      pathfile->GetNamefile(namefile);
      pathfile->GetExt(ext);
      if(ext.Compare(NL(".h"), true) != 0) continue;
      if(namefile.Find(NL("_TranslateID"), true) == XSTRING_NOTFOUND) continue;

      nheaders++;
      idheader = pathfile->Get();

      expected = dirname;
      expected.Add(NL("_TranslateID"));
      if(!dirname.IsEmpty() && (namefile.Compare(expected, true) == 0))
        {
          idheader = pathfile->Get();
          nheaders = 1;
          break;
        }
    }

  if(idheader.IsEmpty() || (nheaders == 0))
    {
      XSTRING filename;

      if(!TRANSLATESCAN_EnsureDirectory(sourcesdir)) return false;

      filename = appname;
      filename.Add(NL("_TranslateID.h"));
      idheader = sourcesdir;
      idheader.Slash_Add();
      idheader.Add(filename);

      if(!TRANSLATESCAN_CreateMinTranslateIDHeader(idheader, appname)) return false;
      nheaders = 1;
    }

  if((nheaders > 1) && !dirname.IsEmpty())
    {
      XSTRING namefile;
      XSTRING expected;

      idheader.GetNamefile(namefile);
      expected = dirname;
      expected.Add(NL("_TranslateID"));
      if(namefile.Compare(expected, true) != 0)
        {
          TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP found more than one TranslateID header under %s"), operationdir.Get());
          return false;
        }
    }

  {
    XSTRING lngname;
    int     cut = XSTRING_NOTFOUND;

    idheader.GetNamefile(lngname);
    cut = lngname.Find(NL("_TranslateID"), true);
    if(cut != XSTRING_NOTFOUND) lngname.DeleteCharactersToEnd((XDWORD)cut);
    lngname.ToLowerCase();

    TRANSLATESCAN_CollectLng(approot.Get(), &scope.lngfiles);

    for(XDWORD c=0; c<scope.lngfiles.GetSize(); c++)
      {
        XPATH*  path = scope.lngfiles.Get(c);
        XSTRING name;

        if(!path) continue;
        path->GetNamefile(name);
        name.ToLowerCase();
        if(name.Compare(lngname, true) == 0)
          {
            lngpath = path->Get();
            break;
          }
      }

    if(lngpath.IsEmpty() && (scope.lngfiles.GetSize() == 1) && scope.lngfiles.Get(0))
      {
        lngpath = scope.lngfiles.Get(0)->Get();
      }

    if(lngpath.IsEmpty())
      {
        XSTRING filename;

        if(!TRANSLATESCAN_EnsureDirectory(assetsdir)) return false;

        filename = lngname;
        filename.Add(NL(".lng"));
        lngpath = assetsdir;
        lngpath.Slash_Add();
        lngpath.Add(filename);

        if(!TRANSLATESCAN_CreateMinLng(lngpath)) return false;
      }
  }

  if(lngpath.IsEmpty())
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP language file was not found under %s"), operationdir.Get());
      return false;
    }

  if(!TRANSLATESCAN_UpgradeAppLngLanguages(lngpath))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP language file must list English, Spanish, French, German and Chinese."));
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP scan %s"), operationdir.Get());
  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP  %s"), idheader.Get());
  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP  %s"), lngpath.Get());

  {
    XFILETXT header;
    bool     inenum = false;

    if(!header.Open(idheader, true) || !header.ReadAllFile()) return false;

    for(int c=0; c<(int)header.GetNLines(); c++)
      {
        XSTRING* line = header.GetLine(c);
        XSTRING  ident;
        int      next = 0;
        int      at   = XSTRING_NOTFOUND;

        if(!line) continue;

        headertext.Add(line->Get());
        headertext.Add(_C('\n'));

        if(!inenum)
          {
            at = line->Find(NL("enum"), false);
            if(at != XSTRING_NOTFOUND)
              {
                XCHAR* text     = line->Get();
                int    size     = (int)line->GetSize();
                int    enumend  = at + 4;
                bool   boundary = true;

                if(text && (at > 0) &&
                   ((text[at - 1] == _C('_')) || line->Character_IsAlpha(text[at - 1]) || line->Character_IsNumber(text[at - 1], false)))
                  {
                    boundary = false;
                  }

                if(text && (enumend < size) &&
                   ((text[enumend] == _C('_')) || line->Character_IsAlpha(text[enumend]) || line->Character_IsNumber(text[enumend], false)))
                  {
                    boundary = false;
                  }

                if(boundary && TRANSLATESCAN_ReadCIdentifier(*line, enumend, ident, next)) enumname = ident;
              }

            if(line->Find(NL("{"), false) != XSTRING_NOTFOUND) inenum = !enumname.IsEmpty();
            continue;
          }

        if(!TRANSLATESCAN_ReadCIdentifier(*line, 0, ident, next)) continue;

        if(TRANSLATESCAN_EndsWith(ident, NL("_LAST")) || (ident.Compare(NL("LAST"), true) == 0))
          {
            enumlast = c;
            break;
          }

        {
          TRANSLATESCAN_APPENUM* enumerator = GEN_NEW TRANSLATESCAN_APPENUM();
          int                    id         = enumnext;

          if(TRANSLATESCAN_NextIs(*line, next, _C('=')))
            {
              int assigned = 0;
              if(TRANSLATESCAN_ReadIntAfter(*line, next, assigned))
                {
                  id = assigned;
                  enumnext = assigned + 1;
                }
               else enumnext++;
            }
           else enumnext++;

          if(enumerator)
            {
              enumerator->idnumber = id;
              enumerator->idname   = ident;
              scope.enums.Add(enumerator);
            }
        }
      }

    header.Close();
  }

  if(enumname.IsEmpty() || (enumlast < 0))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP translation enum was not found in %s"), idheader.Get());
      return false;
    }

  idprefix = enumname;
  idprefix.Add(_C('_'));

  {
    XFILETXT lng;
    bool     inlanguages = false;

    if(!lng.Open(lngpath, true) || !lng.ReadAllFile()) return false;

    for(int c=0; c<(int)lng.GetNLines(); c++)
      {
        XSTRING* line = lng.GetLine(c);
        if(line && (line->Find(NL("\"translations\""), false) != XSTRING_NOTFOUND))
          {
            translationsline = c;
            break;
          }
      }

    if(translationsline < 0)
      {
        lng.Close();
        TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP language file has no translations in %s"), lngpath.Get());
        return false;
      }

    bool languagesnamed = false;

    for(int c=0; c<translationsline; c++)
      {
        XSTRING* line   = lng.GetLine(c);
        int      cursor = 0;

        if(!line) continue;
        if(!inlanguages && (line->Find(NL("\"languages\""), false) != XSTRING_NOTFOUND)) inlanguages = true;
        if(!inlanguages) continue;

        do{ XSTRING raw;
            int     next = TRANSLATESCAN_ReadJSONString(*line, cursor, raw);

            if(next == XSTRING_NOTFOUND) break;

            if(!languagesnamed && (raw.Compare(NL("languages"), true) == 0))
              {
                languagesnamed = true;
              }
            else if(languagesnamed && (nlang < 16))
              {
                int slot = TRANSLATESCAN_AppLanguageSlot(raw);
                langorder[nlang] = slot;
                if((slot >= 0) && (slot < 5)) langseen[slot] = true;
                nlang++;
              }

            cursor = next;
          } while(cursor != XSTRING_NOTFOUND);

        if(TRANSLATESCAN_FirstChar(line) == _C(']')) break;
      }

    for(int c=0; c<5; c++)
      {
        if(!langseen[c])
          {
            lng.Close();
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP language file must list English, Spanish, French, German and Chinese."));
            return false;
          }
      }

    {
      TRANSLATESCAN_APPEXISTING* current = NULL;

      for(int c=translationsline; c<(int)lng.GetNLines(); c++)
        {
          XSTRING* line   = lng.GetLine(c);
          int      cursor = 0;
          bool     gotkey = false;
          XCHAR    first;

          if(!line) continue;

          first = TRANSLATESCAN_FirstChar(line);
          if(first == _C('}')) break;

          do{ XSTRING raw;
              int     next = TRANSLATESCAN_ReadJSONString(*line, cursor, raw);
              bool    iskey;

              if(next == XSTRING_NOTFOUND) break;

              iskey = TRANSLATESCAN_AllDigits(raw) && TRANSLATESCAN_NextIs(*line, next, _C(':'));
              if(iskey)
                {
                  current = GEN_NEW TRANSLATESCAN_APPEXISTING();
                  gotkey  = true;
                  if(current)
                    {
                      current->idnumber = (int)raw.ConvertToDWord();
                      for(XDWORD e=0; e<scope.enums.GetSize(); e++)
                        {
                          TRANSLATESCAN_APPENUM* enumerator = scope.enums.Get(e);
                          if(enumerator && (enumerator->idnumber == current->idnumber))
                            {
                              current->idname = enumerator->idname;
                              break;
                            }
                        }
                      scope.existing.Add(current);
                    }
                }
              else if(current && (current->ntext < 16))
                {
                  TRANSLATESCAN_Unescape(raw, current->text[current->ntext]);
                  current->ntext++;
                }

              cursor = next;
            } while(cursor != XSTRING_NOTFOUND);

          if(gotkey && (c != translationsline) && (keyindent < 0)) keyindent = TRANSLATESCAN_Indent(line);
          if(current && (current->ntext > 0) && (sentenceindent < 0) && !gotkey) sentenceindent = TRANSLATESCAN_Indent(line);

          if(first == _C(']'))
            {
              lastbracket = c;
              if(bracketindent < 0) bracketindent = TRANSLATESCAN_Indent(line);
              current = NULL;
            }
        }
    }

    lng.Close();
  }

  if(keyindent < 0)      keyindent = 24;
  if(sentenceindent < 0) sentenceindent = keyindent + 12;
  if(bracketindent < 0)  bracketindent = keyindent + 10;

  for(XDWORD f=0; f<scope.files.GetSize(); f++)
    {
      TRANSLATESCAN_FILETARGET* filetarget = scope.files.Get(f);
      XPATH*                    pathfile   = filetarget ? filetarget->GetXPathFile() : NULL;
      XFILETXT                  filetxt;

      if(!pathfile || TRANSLATESCAN_IsCatalogueFile(*pathfile)) continue;
      if(pathfile->Find(NL("_TranslateID.h"), true) != XSTRING_NOTFOUND) continue;
      if(!filetxt.Open(*pathfile, true)) continue;
      if(!filetxt.ReadAllFile())
        {
          filetxt.Close();
          continue;
        }

      TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL(" > %s"), pathfile->Get());

      for(int nline=0; nline<(int)filetxt.GetNLines(); nline++)
        {
          XSTRING* line = filetxt.GetLine(nline);
          if(!line) continue;

          XCHAR* search[2] = { NL("_L("), NL("FL(") };

          for(int s=0; s<2; s++)
            {
              int index = 0;

              do{ index = line->Find(search[s], false, index);
                  if(index == XSTRING_NOTFOUND) break;

                  XSTRING literal;
                  TRANSLATESCAN_LITERALMACRO macro = Operation_LiteralStrings_GetMacroAt(line, index);

                  if((macro == TRANSLATESCAN_LITERALMACRO_L) || (macro == TRANSLATESCAN_LITERALMACRO_FL))
                    {
                      XCHAR* text  = line->Get();
                      int    size  = (int)line->GetSize();
                      int    start = index + 3;

                      while((start < size) && ((text[start] == _C(' ')) || (text[start] == _C('\t')))) start++;

                      if((start < size) && (text[start] == _C('"')))
                        {
                          int end = Operation_LiteralStrings_FindStringEnd(text, size, start + 1);

                          if(end != XSTRING_NOTFOUND)
                            {
                              line->Copy(start + 1, end, literal);

                              if(!Operation_LiteralStrings_IsApplicationName(line, index))
                                {
                                  if(!Operation_LiteralStrings_HasTranslatableText(literal))
                                    {
                                      if(macro == TRANSLATESCAN_LITERALMACRO_L)
                                        {
                                          TRANSLATESCAN_LTONLPLACE* place = GEN_NEW TRANSLATESCAN_LTONLPLACE();
                                          if(place)
                                            {
                                              place->pathfile = pathfile->Get();
                                              place->nline    = nline;
                                              place->index    = index;
                                              place->literal  = literal;
                                              scope.ltonl.Add(place);

                                              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  [NOTEXT->_NL] \"%s\""), nline + 1, literal.Get());
                                            }
                                        }
                                    }
                                  else
                                    {
                                      int     textindex = -1;
                                      XSTRING human;

                                  TRANSLATESCAN_Unescape(literal, human);
                                  if(human.IsEmpty()) human = literal;

                                  for(XDWORD t=0; t<scope.texts.GetSize(); t++)
                                    {
                                      if(scope.texts.Get(t) && (scope.texts.Get(t)->literal.Compare(literal, false) == 0))
                                        {
                                          textindex = (int)t;
                                          break;
                                        }
                                    }

                                  if(textindex < 0)
                                    {
                                      for(XDWORD e=0; e<scope.existing.GetSize(); e++)
                                        {
                                          TRANSLATESCAN_APPEXISTING* catalogue = scope.existing.Get(e);
                                          bool                        same = false;

                                          if(!catalogue || catalogue->idname.IsEmpty()) continue;

                                          for(int n=0; n<catalogue->ntext; n++)
                                            {
                                              if((catalogue->text[n].Compare(human, false) == 0) ||
                                                 (catalogue->text[n].Compare(literal, false) == 0))
                                                {
                                                  same = true;
                                                  break;
                                                }
                                            }

                                          if(!same) continue;

                                          TRANSLATESCAN_APPTEXT* entry = GEN_NEW TRANSLATESCAN_APPTEXT();
                                          if(entry)
                                            {
                                              entry->literal    = literal;
                                              entry->human      = human;
                                              entry->idname     = catalogue->idname;
                                              entry->idnumber   = catalogue->idnumber;
                                              entry->fixed      = (macro == TRANSLATESCAN_LITERALMACRO_FL) ? (int)literal.GetSize() : 0;
                                              entry->ready      = true;
                                              entry->addtolng   = false;
                                              entry->addtoenum  = false;
                                              textindex = (int)scope.texts.GetSize();
                                              scope.texts.Add(entry);
                                            }
                                          break;
                                        }
                                    }

                                  if(textindex < 0)
                                    {
                                      TRANSLATESCAN_APPTEXT* entry = GEN_NEW TRANSLATESCAN_APPTEXT();
                                      if(entry)
                                        {
                                          entry->pathfile  = *pathfile;
                                          entry->literal   = literal;
                                          entry->human     = human;
                                          entry->fixed     = (macro == TRANSLATESCAN_LITERALMACRO_FL) ? (int)literal.GetSize() : 0;
                                          entry->ready     = false;
                                          entry->addtolng  = false;
                                          entry->addtoenum = false;
                                          // Provisional name from the source literal; rebuilt from English after translation.
                                          TRANSLATESCAN_BuildAppIDName(idprefix, *pathfile, literal, entry->idname);
                                          textindex = (int)scope.texts.GetSize();
                                          scope.texts.Add(entry);
                                        }
                                    }

                                  if(textindex >= 0)
                                    {
                                      TRANSLATESCAN_APPPLACE* place = GEN_NEW TRANSLATESCAN_APPPLACE();
                                      if(place)
                                        {
                                          int tailindex = end + 1;

                                          while((tailindex < size) && ((text[tailindex] == _C(' ')) || (text[tailindex] == _C('\t')))) tailindex++;
                                          if((tailindex < size) && (text[tailindex] == _C(')'))) tailindex++;

                                          place->pathfile  = pathfile->Get();
                                          place->nline     = nline;
                                          place->index     = index;
                                          place->endquote  = tailindex - 1;
                                          place->textindex = textindex;
                                          scope.places.Add(place);

                                          {
                                            TRANSLATESCAN_APPTEXT* shown = scope.texts.Get((XDWORD)textindex);

                                            if(shown)
                                              {
                                                TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  %s  \"%s\""), nline + 1, shown->idname.Get(), literal.Get());
                                              }
                                          }
                                        }
                                    }
                                    }
                                }
                            }
                        }
                    }

                  index += 3;
                } while(index != XSTRING_NOTFOUND);
            }
        }

      filetxt.Close();
    }

  {
    DIOSCRAPERWEBTRANSLATION scraper;
    int                      pending = 0;
    int                      current = 0;

    for(XDWORD t=0; t<scope.texts.GetSize(); t++)
      {
        if(scope.texts.Get(t) && !scope.texts.Get(t)->ready) pending++;
      }

    if(pending > 0)
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP translating %d string(s). Each one needs several web requests."), pending);
      }
     else
      {
        int reused = 0;

        for(XDWORD t=0; t<scope.texts.GetSize(); t++)
          {
            if(scope.texts.Get(t) && scope.texts.Get(t)->ready && !scope.texts.Get(t)->addtolng) reused++;
          }

        if(reused > 0)
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP reusing %d existing catalogue string(s)."), reused);
          }
         else
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP no new strings to translate."));
          }
      }

    for(XDWORD t=0; t<scope.texts.GetSize(); t++)
      {
        TRANSLATESCAN_APPTEXT* entry = scope.texts.Get(t);
        DIOTRANSLATION_RESULT  detected;
        XVECTOR<XSTRING*>      masks;
        XSTRING                shielded;
        XSTRING                autolang;
        XSTRING                english;
        int                    origin = -1;

        if(!entry || entry->ready) continue;

        current++;
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   translate %d/%d  %s  \"%s\""), current, pending, entry->idname.Get(), entry->literal.Get());

        TRANSLATESCAN_ShieldPrintfMasks(entry->human, shielded, masks);

        autolang = NL("auto");
        english  = NL("en");

        if(!scraper.Get(shielded, autolang, english, detected))
          {
            failed++;
            masks.DeleteContents();
            masks.DeleteAll();
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   failed  \"%s\""), entry->literal.Get());
            continue;
          }

        origin = TRANSLATESCAN_CatalogueLanguageIndex(detected.GetSourceLanguage());

        for(int lang=0; lang<5; lang++)
          {
            XSTRING targetcode;

            if(lang == origin)
              {
                entry->sentence[lang] = entry->human;
                continue;
              }

            if((origin < 0) && (lang == 0))
              {
                XSTRING decoded;

                decoded = detected.GetTranslation();
                if(!TRANSLATESCAN_RestorePrintfMasks(decoded, masks))
                  {
                    failed++;
                    entry->ready = false;
                    TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   mask lost  \"%s\""), entry->literal.Get());
                    break;
                  }

                entry->sentence[lang] = decoded;
                continue;
              }

            targetcode = alpha2[lang];

            {
              XSTRING translation;
              XSTRING sourcecode;

              sourcecode = (origin >= 0) ? alpha2[origin] : detected.GetSourceLanguage();
              if(sourcecode.IsEmpty()) sourcecode = english;

              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("     -> %s"), targetcode.Get());

              if(!scraper.Get(shielded, sourcecode, targetcode, translation) ||
                 !TRANSLATESCAN_RestorePrintfMasks(translation, masks))
                {
                  failed++;
                  entry->ready = false;
                  TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   mask lost  \"%s\""), entry->literal.Get());
                  break;
                }

              entry->sentence[lang] = translation;
            }
          }

        masks.DeleteContents();
        masks.DeleteAll();

        if(entry->sentence[0].IsEmpty() || entry->sentence[1].IsEmpty() || entry->sentence[2].IsEmpty() ||
           entry->sentence[3].IsEmpty() || entry->sentence[4].IsEmpty())
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("   incomplete  \"%s\""), entry->literal.Get());
            continue;
          }

        // Enum IDs always come from the English sentence, even when the source literal is not English.
        TRANSLATESCAN_BuildAppIDName(idprefix, entry->pathfile, entry->sentence[0], entry->idname);
        TRANSLATESCAN_MakeUniqueAppID(entry->idname, headertext);

        entry->ready     = true;
        entry->addtolng  = true;
        entry->addtoenum = true;

        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   ready  %s  \"%s\""), entry->idname.Get(), entry->literal.Get());
      }
  }

  {
    int ready = 0;

    for(XDWORD t=0; t<scope.texts.GetSize(); t++)
      {
        if(scope.texts.Get(t) && scope.texts.Get(t)->ready) ready++;
      }

    if(ready == 0)
      {
        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP finished with no catalogue changes. failed=%d"), failed);
        TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
        TRANSLATESCAN_Report(status ? XTRACE_COLOR_GREEN : XTRACE_COLOR_RED, NL("APP done. status=%d failed=%d"), status ? 1 : 0, failed);
        return (failed == 0) && status;
      }

    TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP applying %d ready string(s)."), ready);
  }

  {
    int nextid = enumnext;
    int maxlng = 0;
    int newlng = 0;

    for(XDWORD e=0; e<scope.existing.GetSize(); e++)
      {
        TRANSLATESCAN_APPEXISTING* catalogue = scope.existing.Get(e);
        if(catalogue && (catalogue->idnumber > maxlng)) maxlng = catalogue->idnumber;
      }

    if((maxlng + 1) > nextid) nextid = maxlng + 1;

    for(XDWORD t=0; t<scope.texts.GetSize(); t++)
      {
        TRANSLATESCAN_APPTEXT* entry = scope.texts.Get(t);
        if(!entry || !entry->ready || !entry->addtolng) continue;
        entry->idnumber = nextid;
        nextid++;
        newlng++;
      }

    if(newlng > 0)
      {
        XFILETXT header;

        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP writing %d new catalogue entries."), newlng);

        if(!header.Open(idheader, false) || !header.ReadAllFile())
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not open %s"), idheader.Get());
            TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
            return false;
          }

        enumlast = -1;
        for(int c=0; c<(int)header.GetNLines(); c++)
          {
            XSTRING* line = header.GetLine(c);
            XSTRING  lastname;

            if(!line) continue;
            lastname = enumname;
            lastname.Add(NL("_LAST"));
            if(line->Find(lastname.Get(), false) != XSTRING_NOTFOUND) enumlast = c;
          }

        if(enumlast < 0)
          {
            header.Close();
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP LAST enumerator not found in %s"), idheader.Get());
            TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
            return false;
          }

        for(int t=(int)scope.texts.GetSize()-1; t>=0; t--)
          {
            TRANSLATESCAN_APPTEXT* entry = scope.texts.Get((XDWORD)t);
            XSTRING                line;
            bool                   first = true;

            if(!entry || !entry->ready || !entry->addtoenum) continue;

            for(int u=0; u<t; u++)
              {
                TRANSLATESCAN_APPTEXT* earlier = scope.texts.Get((XDWORD)u);
                if(earlier && earlier->ready && earlier->addtoenum) first = false;
              }

            line = NL("  ");
            line.Add(entry->idname);
            if(first && (entry->idnumber != enumnext))
              {
                XSTRING assigned;
                assigned.Format(NL(" = %d"), entry->idnumber);
                line.Add(assigned);
              }
            line.Add(NL(" ,"));
            header.InsertLine(enumlast, line);
            TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   enum  %s"), entry->idname.Get());
          }

        if(!header.WriteAllFile())
          {
            status = false;
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP write failed  %s"), idheader.Get());
          }
         else
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP updated  %s"), idheader.Get());
          }
        header.Close();
        if(!status)
          {
            TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
            return false;
          }
      }

    if((newlng > 0) && status)
      {
        XFILETXT lng;
        int      produced = 0;

        if(!lng.Open(lngpath, false) || !lng.ReadAllFile())
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP could not open %s"), lngpath.Get());
            TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
            return false;
          }

        lastbracket = -1;
        for(int c=translationsline; c<(int)lng.GetNLines(); c++)
          {
            XSTRING* line = lng.GetLine(c);
            if(!line) continue;
            if(TRANSLATESCAN_FirstChar(line) == _C(']')) lastbracket = c;
            if(TRANSLATESCAN_FirstChar(line) == _C('}')) break;
          }

        if(lastbracket >= 0)
          {
            XSTRING* bracket = lng.GetLine(lastbracket);
            XSTRING  updated;

            if(bracket) updated = bracket->Get();
            if(updated.Find(NL(","), false) == XSTRING_NOTFOUND) updated.Add(_C(','));
            lng.DeleteLine(lastbracket);
            lng.InsertLine(lastbracket, updated);
          }

        {
          int insertat = (lastbracket >= 0) ? (lastbracket + 1) : (translationsline + 1);

          for(XDWORD t=0; t<scope.texts.GetSize(); t++)
            {
              TRANSLATESCAN_APPTEXT* entry = scope.texts.Get(t);
              XSTRING                key;
              XSTRING                line;
              XSTRING                blank;
              bool                   lastentry;

              if(!entry || !entry->ready || !entry->addtolng) continue;

              produced++;
              lastentry = (produced == newlng);

              blank.Empty();
              lng.InsertLine(insertat, blank);
              insertat++;

              key.Format(NL("%05d"), entry->idnumber);
              TRANSLATESCAN_Pad(line, keyindent);
              {
                XSTRING rest;
                rest.Format(NL("\"%s\" : [ %d,"), key.Get(), entry->fixed);
                line.Add(rest);
              }
              lng.InsertLine(insertat, line);
              insertat++;

              for(int n=0; n<nlang; n++)
                {
                  int     slot = langorder[n];
                  XSTRING sentence;
                  XSTRING escaped;

                  if((slot >= 0) && (slot < 5)) sentence = entry->sentence[slot];
                  else sentence = entry->sentence[0];

                  TRANSLATESCAN_EscapeForJSON(sentence, escaped);
                  TRANSLATESCAN_Pad(line, sentenceindent);
                  line.Add(_C('"'));
                  line.Add(escaped);
                  if((n + 1) < nlang) line.Add(NL("\" ,"));
                  else                line.Add(_C('"'));
                  lng.InsertLine(insertat, line);
                  insertat++;
                }

              TRANSLATESCAN_Pad(line, bracketindent);
              line.Add(_C(']'));
              if(!lastentry) line.Add(_C(','));
              lng.InsertLine(insertat, line);
              insertat++;

              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   lng  %s  %s"), key.Get(), entry->idname.Get());
            }
        }

        lng.Close();
        if(!TRANSLATESCAN_WriteLinesUTF8(lngpath, lng))
          {
            TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP write failed  %s"), lngpath.Get());
            TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
            return false;
          }

        TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP updated  %s"), lngpath.Get());
      }
  }

  if(!status)
    {
      TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);
      return false;
    }

  for(XDWORD f=0; f<scope.files.GetSize(); f++)
    {
      XPATH*   pathfile = scope.files.Get(f) ? scope.files.Get(f)->GetXPathFile() : NULL;
      XFILETXT filetxt;
      bool     changed = false;

      if(!pathfile) continue;
      if(pathfile->Find(NL("_TranslateID.h"), true) != XSTRING_NOTFOUND) continue;

      if(!filetxt.Open(*pathfile, false) || !filetxt.ReadAllFile()) continue;

      for(int p=(int)scope.places.GetSize()-1; p>=0; p--)
        {
          TRANSLATESCAN_APPPLACE* place = scope.places.Get((XDWORD)p);
          TRANSLATESCAN_APPTEXT*  entry = NULL;
          XSTRING*                line  = NULL;
          XSTRING                 rebuilt;
          XSTRING                 tail;

          if(!place || (place->textindex < 0) || (place->textindex >= (int)scope.texts.GetSize())) continue;
          entry = scope.texts.Get((XDWORD)place->textindex);
          if(!entry || !entry->ready) continue;
          if(place->pathfile.Compare(pathfile->Get(), true) != 0) continue;

          line = filetxt.GetLine(place->nline);
          if(!line) continue;

          line->Copy(0, place->index, rebuilt);
          rebuilt.Add(NL("TL("));
          rebuilt.Add(entry->idname);
          rebuilt.Add(NL(")"));
          line->Copy(place->endquote + 1, tail);
          rebuilt.Add(tail);

          filetxt.DeleteLine(place->nline);
          filetxt.InsertLine(place->nline, rebuilt);
          changed = true;

          TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   replace %d  TL(%s)"), place->nline + 1, entry->idname.Get());
        }

      if(changed)
        {
          bool hasinclude = false;
          int  insertat   = 0;

          for(int c=0; c<(int)filetxt.GetNLines(); c++)
            {
              XSTRING* line = filetxt.GetLine(c);
              if(!line) continue;
              if(line->Find(NL("XTranslation.h"), false) != XSTRING_NOTFOUND) hasinclude = true;
              if(line->Find(NL("#include \"GEN_Defines.h\""), false) != XSTRING_NOTFOUND) insertat = c + 1;
            }

          if(!hasinclude)
            {
              XSTRING include = NL("#include \"XTranslation.h\"");
              filetxt.InsertLine(insertat, include);
              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   include XTranslation.h"));
            }

          if(!filetxt.WriteAllFile())
            {
              status = false;
              TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("APP write failed  %s"), pathfile->Get());
            }
           else
            {
              TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("APP updated  %s"), pathfile->Get());
            }
        }

      filetxt.Close();
    }

  TRANSLATESCAN_ApplyLToNLPlaces(scope.ltonl, status);

  TRANSLATESCAN_Report(status ? XTRACE_COLOR_GREEN : XTRACE_COLOR_RED, NL("APP done. status=%d failed=%d"), status ? 1 : 0, failed);

  return status;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_ViewLiteralStrings(XPATH& operationdir)
* @brief      List translation candidates under a directory without changing any file.
* @ingroup    APPLICATION
*
* @param[in]  operationdir : directory where the operation is executed.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_ViewLiteralStrings(XPATH& operationdir)
{
  XVECTOR<TRANSLATESCAN_FILETARGET*> files;
  int                                ncandidate = 0;
  int                                nfl        = 0;
  int                                nnl        = 0;
  int                                nappname   = 0;
  int                                nnotext    = 0;
  int                                ngen       = 0;
  int                                nfiles     = 0;

  if(operationdir.IsEmpty()) return false;

  if(!SearchFilesTarget(operationdir, &files))
    {
      TRANSLATESCAN_Report(XTRACE_COLOR_RED, NL("VIEW no source files under %s"), operationdir.Get());
      return false;
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("VIEW scan %s"), operationdir.Get());

  for(XDWORD f=0; f<files.GetSize(); f++)
    {
      TRANSLATESCAN_FILETARGET* filetarget = files.Get(f);
      XPATH*                    pathfile   = filetarget ? filetarget->GetXPathFile() : NULL;
      XFILETXT                  filetxt;
      bool                      reported   = false;

      if(!pathfile) continue;
      if(!filetxt.Open(*pathfile, true)) continue;
      if(!filetxt.ReadAllFile())
        {
          filetxt.Close();
          continue;
        }

      nfiles++;

      for(int nline=0; nline<(int)filetxt.GetNLines(); nline++)
        {
          XSTRING* line = filetxt.GetLine(nline);
          if(!line) continue;

          XCHAR* search[3] = { NL("_L("), NL("NL("), NL("FL(") };

          for(int s=0; s<3; s++)
            {
              int index = 0;

              do{ index = line->Find(search[s], false, index);
                  if(index == XSTRING_NOTFOUND) break;

                  TRANSLATESCAN_LITERALMACRO macro = Operation_LiteralStrings_GetMacroAt(line, index);
                  XSTRING                    literal;
                  XCHAR*                     text;
                  int                        size;
                  int                        start;

                  if(macro == TRANSLATESCAN_LITERALMACRO_NONE)
                    {
                      index += 3;
                      continue;
                    }

                  text  = line->Get();
                  size  = (int)line->GetSize();
                  start = index + 3;

                  while((start < size) && ((text[start] == _C(' ')) || (text[start] == _C('\t')))) start++;

                  if((start >= size) || (text[start] != _C('"')))
                    {
                      index += 3;
                      continue;
                    }

                  {
                    int end = Operation_LiteralStrings_FindStringEnd(text, size, start + 1);

                    if(end == XSTRING_NOTFOUND)
                      {
                        index += 3;
                        continue;
                      }

                    line->Copy(start + 1, end, literal);
                  }

                  if(!reported)
                    {
                      TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL(" > %s"), pathfile->Get());
                      reported = true;
                    }

                  if(macro == TRANSLATESCAN_LITERALMACRO_NL)
                    {
                      nnl++;
                      TRANSLATESCAN_Report(XTRACE_COLOR_GRAY, NL("   %d  [NL] \"%s\""), nline + 1, literal.Get());
                    }
                  else if(Operation_LiteralStrings_IsApplicationName(line, index))
                    {
                      nappname++;
                      TRANSLATESCAN_Report(XTRACE_COLOR_GRAY, NL("   %d  [APPNAME] \"%s\""), nline + 1, literal.Get());
                    }
                  else if(!Operation_LiteralStrings_HasTranslatableText(literal))
                    {
                      nnotext++;
                      TRANSLATESCAN_Report(XTRACE_COLOR_GRAY, NL("   %d  [NOTEXT] \"%s\""), nline + 1, literal.Get());
                    }
                  else
                    {
                      XDWORD gen_ID = 0;

                      if(Operation_LiteralStrings_IsGENInternal(literal, gen_ID))
                        {
                          ngen++;
                          TRANSLATESCAN_Report(XTRACE_COLOR_PURPLE, NL("   %d  [GEN ID=%d] \"%s\""), nline + 1, gen_ID, literal.Get());
                        }
                      else if(macro == TRANSLATESCAN_LITERALMACRO_FL)
                        {
                          ncandidate++;
                          nfl++;
                          TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  [FL] \"%s\""), nline + 1, literal.Get());
                        }
                       else
                        {
                          ncandidate++;
                          TRANSLATESCAN_Report(XTRACE_COLOR_GREEN, NL("   %d  [L] \"%s\""), nline + 1, literal.Get());
                        }
                    }

                  index += 3;
                } while(index != XSTRING_NOTFOUND);
            }
        }

      filetxt.Close();
    }

  TRANSLATESCAN_Report(XTRACE_COLOR_GREEN,
                       NL("VIEW done. files=%d candidates=%d (FL=%d) gen=%d nl=%d appname=%d notext=%d"),
                       nfiles, ncandidate, nfl, ngen, nnl, nappname, nnotext);

  files.DeleteContents();
  files.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings(XPATH& operationdir)
* @brief      Scan source files for _L("...") and NL("...") strings and classify them.
* @ingroup    APPLICATION
*
* @param[in]  operationdir : directory where the operation is executed.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings(XPATH& operationdir)
{
  bool previous = modifierview;
  bool status;

  ModifierVIEW_Active(true);
  status = Operation_ViewLiteralStrings(operationdir);
  ModifierVIEW_Active(previous);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_LITERALMACRO TRANSLATESCAN_MANAGER::Operation_LiteralStrings_GetMacroAt(XSTRING* line, int index)
* @brief      Identify a standalone _L(, NL( or FL( macro at line[index] (rejects __L(, foo_L(, fooNL(, etc.).
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
TRANSLATESCAN_LITERALMACRO TRANSLATESCAN_MANAGER::Operation_LiteralStrings_GetMacroAt(XSTRING* line, int index)
{
  if(!line || index < 0)
    {
      return TRANSLATESCAN_LITERALMACRO_NONE;
    }

  XCHAR* text = line->Get();
  if(!text)
    {
      return TRANSLATESCAN_LITERALMACRO_NONE;
    }

  if(index > 0)
    {
      XCHAR previous = text[index - 1];

      // Reject identifier/_ continuation so "__L(" / "name_L(" / "fooNL(" are ignored.
      if((previous == _C('_')) ||
         ((previous >= _C('A')) && (previous <= _C('Z'))) ||
         ((previous >= _C('a')) && (previous <= _C('z'))) ||
         ((previous >= _C('0')) && (previous <= _C('9'))))
        {
          return TRANSLATESCAN_LITERALMACRO_NONE;
        }
    }

  if((index + 3) > (int)line->GetSize())
    {
      return TRANSLATESCAN_LITERALMACRO_NONE;
    }

  if((text[index] == _C('_')) && (text[index + 1] == _C('L')) && (text[index + 2] == _C('(')))
    {
      return TRANSLATESCAN_LITERALMACRO_L;
    }

  if((text[index] == _C('N')) && (text[index + 1] == _C('L')) && (text[index + 2] == _C('(')))
    {
      return TRANSLATESCAN_LITERALMACRO_NL;
    }

  if((text[index] == _C('F')) && (text[index + 1] == _C('L')) && (text[index + 2] == _C('(')))
    {
      return TRANSLATESCAN_LITERALMACRO_FL;
    }

  return TRANSLATESCAN_LITERALMACRO_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_IsApplicationName(XSTRING* line, int index)
* @brief      True when the _L( at index is the APPLICATION_NAMEAPP / APPLICATION_NAMEFILE definition.
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_IsApplicationName(XSTRING* line, int index)
{
  if(!line || index < 0)
    {
      return false;
    }

  // Only the left side of the line (before this _L) can define the macro name.
  XSTRING prefix;
  line->Copy(0, index, prefix);
  if(prefix.IsEmpty())
    {
      return false;
    }

  if(prefix.Find(_L("APPLICATION_NAMEAPP"), true) != XSTRING_NOTFOUND)
    {
      return true;
    }

  if(prefix.Find(_L("APPLICATION_NAMEFILE"), true) != XSTRING_NOTFOUND)
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_IsGENInternal(XSTRING& literal, XDWORD& out_ID)
* @brief      True when the literal already exists in XTRANSLATION_GEN internal sentences.
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_IsGENInternal(XSTRING& literal, XDWORD& out_ID)
{
  out_ID = 0;

  if(literal.IsEmpty())
    {
      return false;
    }

  #ifdef XTRANSLATION_GEN_ACTIVE
  XTRANSLATION_GEN_SENTENCE* sentence = XTRANSLATION_GEN::GetInstance().Sentence_FindByText(literal.Get(), false);
  if(!sentence)
    {
      return false;
    }

  out_ID = sentence->ID;
  return true;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipPrintfMask(XCHAR* text, int size, int pos)
* @brief      Skip one C/C++ printf conversion starting at '%' (pos). Returns index after the mask.
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipPrintfMask(XCHAR* text, int size, int pos)
{
  int c = pos + 1;

  if(!text || pos < 0 || pos >= size || text[pos] != _C('%'))
    {
      return pos + 1;
    }

  if(c >= size)
    {
      return c;
    }

  // "%%"
  if(text[c] == _C('%'))
    {
      return c + 1;
    }

  // flags: - + # 0 space
  while((c < size) &&
        ((text[c] == _C('-')) || (text[c] == _C('+')) || (text[c] == _C('#')) ||
         (text[c] == _C('0')) || (text[c] == _C(' '))))
    {
      c++;
    }

  // width: * or digits
  if((c < size) && (text[c] == _C('*')))
    {
      c++;
    }
  else
    {
      while((c < size) && (text[c] >= _C('0')) && (text[c] <= _C('9')))
        {
          c++;
        }
    }

  // precision: .[*|digits]
  if((c < size) && (text[c] == _C('.')))
    {
      c++;
      if((c < size) && (text[c] == _C('*')))
        {
          c++;
        }
      else
        {
          while((c < size) && (text[c] >= _C('0')) && (text[c] <= _C('9')))
            {
              c++;
            }
        }
    }

  // length modifier
  if(c < size)
    {
      if(text[c] == _C('h'))
        {
          c++;
          if((c < size) && (text[c] == _C('h')))
            {
              c++;
            }
        }
      else if(text[c] == _C('l'))
        {
          c++;
          if((c < size) && (text[c] == _C('l')))
            {
              c++;
            }
        }
      else if((text[c] == _C('L')) || (text[c] == _C('z')) || (text[c] == _C('j')) || (text[c] == _C('t')))
        {
          c++;
        }
    }

  // conversion specifier
  if(c < size)
    {
      XCHAR conversion = text[c];
      if((conversion == _C('d')) || (conversion == _C('i')) || (conversion == _C('o')) ||
         (conversion == _C('u')) || (conversion == _C('x')) || (conversion == _C('X')) ||
         (conversion == _C('e')) || (conversion == _C('E')) || (conversion == _C('f')) ||
         (conversion == _C('F')) || (conversion == _C('g')) || (conversion == _C('G')) ||
         (conversion == _C('a')) || (conversion == _C('A')) || (conversion == _C('c')) ||
         (conversion == _C('s')) || (conversion == _C('p')) || (conversion == _C('n')) ||
         (conversion == _C('%')))
        {
          return c + 1;
        }
    }

  // Not a recognizable mask: consume only '%'.
  return pos + 1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_FindStringEnd(XCHAR* text, int size, int start)
* @brief      Find the closing " of a C string literal starting at start (first char after opening ").
*             Honors escapes so \" does not end the literal; \\ " does.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @return     int : index of the closing quote, or XSTRING_NOTFOUND.
*
*---------------------------------------------------------------------------------------------------------------------*/
int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_FindStringEnd(XCHAR* text, int size, int start)
{
  if(!text || start < 0)
    {
      return XSTRING_NOTFOUND;
    }

  for(int c = start; c < size; )
    {
      if(text[c] == _C('\\'))
        {
          c = Operation_LiteralStrings_SkipEscape(text, size, c);
          continue;
        }

      if(text[c] == _C('"'))
        {
          return c;
        }

      c++;
    }

  return XSTRING_NOTFOUND;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipEscape(XCHAR* text, int size, int pos)
* @brief      Skip one C escape sequence starting at '\\' (pos). Returns index after the escape.
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
int TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SkipEscape(XCHAR* text, int size, int pos)
{
  int c = pos + 1;

  if(!text || pos < 0 || pos >= size || text[pos] != _C('\\'))
    {
      return pos + 1;
    }

  if(c >= size)
    {
      return c;
    }

  XCHAR escape = text[c];

  // Simple escapes: \n \r \t \0 \\ \" \' \a \b \f \v
  if((escape == _C('n')) || (escape == _C('r')) || (escape == _C('t')) || (escape == _C('0')) ||
     (escape == _C('\\')) || (escape == _C('"')) || (escape == _C('\'')) || (escape == _C('a')) ||
     (escape == _C('b')) || (escape == _C('f')) || (escape == _C('v')))
    {
      return c + 1;
    }

  // \xHH...
  if(escape == _C('x'))
    {
      c++;
      while((c < size) &&
            ((((text[c] >= _C('0')) && (text[c] <= _C('9'))) ||
              ((text[c] >= _C('a')) && (text[c] <= _C('f'))) ||
              ((text[c] >= _C('A')) && (text[c] <= _C('F'))))))
        {
          c++;
        }
      return c;
    }

  // \uHHHH / \UHHHHHHHH
  if((escape == _C('u')) || (escape == _C('U')))
    {
      int hexdigits = (escape == _C('u')) ? 4 : 8;
      c++;
      for(int d=0; (d < hexdigits) && (c < size); d++, c++)
        {
          if(!(((text[c] >= _C('0')) && (text[c] <= _C('9'))) ||
               ((text[c] >= _C('a')) && (text[c] <= _C('f'))) ||
               ((text[c] >= _C('A')) && (text[c] <= _C('F')))))
            {
              break;
            }
        }
      return c;
    }

  // Octal \ooo
  if((escape >= _C('0')) && (escape <= _C('7')))
    {
      int digits = 0;
      while((c < size) && (digits < 3) && (text[c] >= _C('0')) && (text[c] <= _C('7')))
        {
          c++;
          digits++;
        }
      return c;
    }

  return c + 1;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_HasTranslatableText(XSTRING& literal)
* @brief      True when the literal has human-language letters after removing printf masks / escapes / symbols.
* @note       INTERNAL
* @ingroup    APPLICATION
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_HasTranslatableText(XSTRING& literal)
{
  XCHAR* text = literal.Get();
  int    size = (int)literal.GetSize();
  XSTRING probe;

  if(!text || size <= 0)
    {
      return false;
    }

  for(int c=0; c<size; )
    {
      XCHAR character = text[c];

      if(character == _C('%'))
        {
          c = Operation_LiteralStrings_SkipPrintfMask(text, size, c);
          continue;
        }

      if(character == _C('\\'))
        {
          c = Operation_LiteralStrings_SkipEscape(text, size, c);
          continue;
        }

      // ASCII letter, or non-ASCII (accented / other scripts) counts as language content.
      if(probe.Character_IsAlpha(character) || ((XWORD)character > 127))
        {
          return true;
        }

      c++;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
* @brief      Extract one _L("...") literal and report whether it looks translatable.
* @ingroup    APPLICATION
*
* @param[in]  pathfile : file path being scanned.
* @param[in]  filetxt : text file being scanned.
* @param[in]  nline : line number where the match was found.
* @param[in]  index : character index where the match was found.
* @param[out]  result : literal string extracted from the line.
*
* @return     bool : true if it is successful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TRANSLATESCAN_MANAGER::Operation_LiteralStrings_SearchLine(XPATH* pathfile, XFILETXT* filetxt, int nline, int index, XSTRING& result)
{
  if(!pathfile)
    {
      return false;
    }

  if(!filetxt)
    { 
      return false;
    }

  if(nline < 0)
    {
      return false;
    }

  XSTRING* searchline = filetxt->GetLine(nline);
  if(!searchline)
    {
      return false;
    }

  TRANSLATESCAN_LITERALMACRO macro = Operation_LiteralStrings_GetMacroAt(searchline, index);
  if(macro == TRANSLATESCAN_LITERALMACRO_NONE)
    {
      return false;
    }

  // _L( / NL( / FL( then optional spaces, then opening quote of the string literal.
  int start = index + 3;
  XCHAR* text = searchline->Get();
  int    size = (int)searchline->GetSize();

  while((start < size) && ((text[start] == _C(' ')) || (text[start] == _C('\t'))))
    {
      start++;
    }

  if((start >= size) || (text[start] != _C('"')))
    {
      return false;
    }

  start++;
  int end = Operation_LiteralStrings_FindStringEnd(text, size, start);
  if(end == XSTRING_NOTFOUND)
    {
      return false;
    }

  // Full macro argument, including escaped quotes such as \".
  searchline->Copy(start, end, result);

  // NL("...") is explicitly non-translatable by macro choice.
  if(macro == TRANSLATESCAN_LITERALMACRO_NL)
    {
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: NL macro] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  // Classify _L("...") / FL("..."): green = candidate; red/blue = not for app translation catalog.
  if(Operation_LiteralStrings_IsApplicationName(searchline, index))
    {
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: application name] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  if(!Operation_LiteralStrings_HasTranslatableText(result))
    {
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: format/symbols] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  {
    XDWORD gen_ID = 0;
    if(Operation_LiteralStrings_IsGENInternal(result, gen_ID))
      {
        //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: already translated ID=%d] \"%s\""), pathfile->Get(), nline + 1, gen_ID, result.Get());
        return true;
      }
  }

  if(macro == TRANSLATESCAN_LITERALMACRO_FL)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, _L(" %s(%d) | [FL] \"%s\""), pathfile->Get(), nline + 1, result.Get());
    }
  else
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, _L(" %s(%d) | \"%s\""), pathfile->Get(), nline + 1, result.Get());
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TRANSLATESCAN_MANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    APPLICATION
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TRANSLATESCAN_MANAGER::Clean()
{
  modifiergen  = false;
  modifierapp  = false;
  modifierview = false;
}




