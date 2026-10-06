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

#include "XFactory.h"
#include "XDir.h"
#include "XMap.h"
#include "XTrace.h"
#include "XTranslation_GEN.h"




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
              SearchInFile((*path), _L("_L("), Operation_LiteralStrings_SearchLine, NULL);
              SearchInFile((*path), _L("NL("), Operation_LiteralStrings_SearchLine, NULL);
            }
        }
    }
 
  filestarget.DeleteContents();
  filestarget.DeleteAll();  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRANSLATESCAN_LITERALMACRO TRANSLATESCAN_MANAGER::Operation_LiteralStrings_GetMacroAt(XSTRING* line, int index)
* @brief      Identify a standalone _L( or NL( macro at line[index] (rejects __L(, foo_L(, fooNL(, etc.).
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

  // _L( / NL( then optional spaces, then opening quote of the string literal.
  int start = index + 3;
  XCHAR* text = searchline->Get();

  while((start < (int)searchline->GetSize()) && ((text[start] == _C(' ')) || (text[start] == _C('\t'))))
    {
      start++;
    }

  if((start >= (int)searchline->GetSize()) || (text[start] != _C('"')))
    {
      return false;
    }

  start++;
  if(start >= (int)searchline->GetSize())
    {
      return false;
    }

  int end = searchline->Find(_L("\""), true, start);
  if(end == XSTRING_NOTFOUND)
    {
      return false;
    }

  searchline->Copy(start, end, result);
  if(result.IsEmpty())
    {
      return false;
    }

  // NL("...") is explicitly non-translatable by macro choice.
  if(macro == TRANSLATESCAN_LITERALMACRO_NL)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: NL macro] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  // Classify _L("..."): green = candidate to translate; red/blue = mark as not translatable (for now).
  if(Operation_LiteralStrings_IsApplicationName(searchline, index))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: application name] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  if(!Operation_LiteralStrings_HasTranslatableText(result))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L(" %s(%d) | [NO TRANSLATE: format/symbols] \"%s\""), pathfile->Get(), nline + 1, result.Get());
      return true;
    }

  {
    XDWORD gen_ID = 0;
    if(Operation_LiteralStrings_IsGENInternal(result, gen_ID))
      {
        XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L(" %s(%d) | [NO TRANSLATE: GEN internal ID=%d] \"%s\""), pathfile->Get(), nline + 1, gen_ID, result.Get());
        return true;
      }
  }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, _L(" %s(%d) | \"%s\""), pathfile->Get(), nline + 1, result.Get());

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

}




