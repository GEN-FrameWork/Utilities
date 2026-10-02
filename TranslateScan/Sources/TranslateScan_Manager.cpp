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
  XCHAR* extensions[] = { __L(".c"),
                          __L(".cpp"),
                          __L(".h"),
                          __L(".hpp"),
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

  if(xdir->FirstSearch(inipath, __L("*"), &searchelement))
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
              SearchInFile((*path), __L("* @brief      "), Operation_Remark_Brief_SearchLine, Operation_Remark_Brief_ChangeLine);
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

  functionstr = __L("* @fn         ");
  int start = functionline->Find(functionstr.Get(), false, 0);
  if(start == XSTRING_NOTFOUND)
    {
      // Not remark function.
      return false;
    }

  start += functionstr.GetSize();

  int startmember = functionline->Find(__L("::"), true, start);
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

  int indexlastspace = nameclass.FindCharacter(__C(' '), 0, true);
  if(indexlastspace >= 1)
    {
      nameclass.DeleteCharacters(0, indexlastspace+1); 
    }

  startmember+=2;

  int endmember = functionline->Find(__L("("), true, startmember);
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

  if(namemember.FindCharacter(__C('~'), 0, false) != XSTRING_NOTFOUND)
    {
      isdestructor = true;
      result       = __L("Destructor of class");
    }
   else 
    {
      if(!nameclass.Compare(namemember))
        {
          isconstructor = true;
          result        = __L("Constructor of class");
        }
       else
        {
          if(!namemember.Compare(__L("Clean"), false))
            {
              result = __L("Clean the attributes of the class: Default initialize");
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
   
  XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, __L(" %-48s - %-64s | %s"), nameclass.Get(), namemember.Get(), result.Get());

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
          
      if(brief_target.Get()[index-1] != __C(' ') && (nchar > 1))
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
              brief_target.Insert(__L(" "), index);
              shift++;
              index++;
            }

          if(brief_target.GetSize() != index + nchar)
            {
              if(brief_target.GetSize() >= index + nchar - 1)
                {
                  brief_target.Insert(__L(" "), (index + nchar -1));             
                  shift++;
                  index++;
                }
            }
        }

      if(index)
        {
          if(brief_target.Get()[index-1] != __C(' ') && (nchar == 1))
            {
              brief_target.Get()[index] = brief_target.Character_ToLower(brief_target.Get()[index]);
              brief_target.Insert(__L(" "), index);
              shift++;
              index++;
            }
        }        
    }
  
  brief_target.DeleteCharacter(__C('_'));

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
              SearchInFile((*path), __L("* @ingroup    "), Operation_Remark_InGroup_SearchLine, Operation_Remark_InGroup_ChangeLine);
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

  result = __L("");

  if((pathfile->Find(__L("/GEN/")           , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(__L("\\GEN\\")         , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(__L("/GENFrameWork/")  , true) != XSTRING_NOTFOUND) ||
     (pathfile->Find(__L("\\GENFrameWork\\"), true) != XSTRING_NOTFOUND))
    {
      isGEN = true;
    }
     
  if(isGEN)
    {       
      if(pathfile->Find(__L("AppFlow")                                , true) != XSTRING_NOTFOUND) result = __L("APPFLOW");
      if(pathfile->Find(__L("Cipher")                                 , true) != XSTRING_NOTFOUND) result = __L("CIPHER");
      if(pathfile->Find(__L("Common")                                 , true) != XSTRING_NOTFOUND) result = __L("COMMON");
      if(pathfile->Find(__L("Compress")                               , true) != XSTRING_NOTFOUND) result = __L("COMPRESS");
      if(pathfile->Find(__L("Databases")                              , true) != XSTRING_NOTFOUND) result = __L("DATABASE");
      if(pathfile->Find(__L("DataIO")                                 , true) != XSTRING_NOTFOUND) result = __L("DATAIO");
      if(pathfile->Find(__L("Graphic")                                , true) != XSTRING_NOTFOUND) result = __L("GRAPHIC");
      if(pathfile->Find(__L("Input")                                  , true) != XSTRING_NOTFOUND) result = __L("INPUT");
      if(pathfile->Find(__L("MainProc")                               , true) != XSTRING_NOTFOUND) result = __L("MAIN_PROCEDURE");
      if(pathfile->Find(__L("Script")                                 , true) != XSTRING_NOTFOUND) result = __L("SCRIPT");
      if(pathfile->Find(__L("Sound")                                  , true) != XSTRING_NOTFOUND) result = __L("SOUND");
      if(pathfile->Find(__L("UserInterface")                          , true) != XSTRING_NOTFOUND) result = __L("USERINTERFACE");
      if(pathfile->Find(__L("XUtils")                                 , true) != XSTRING_NOTFOUND) result = __L("XUTILS");

      if(pathfile->Find(__L("Examples")                               , true) != XSTRING_NOTFOUND) result = __L("EXAMPLES");
      if(pathfile->Find(__L("Tests")                                  , true) != XSTRING_NOTFOUND) result = __L("TESTS");
      if(pathfile->Find(__L("UnitTests")                              , true) != XSTRING_NOTFOUND) result = __L("UNIT_TESTS");
                                                                      
      if(pathfile->Find(__L("Platforms/Windows")                      , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_WINDOWS");
      if(pathfile->Find(__L("Platforms/Linux")                        , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_LINUX");
      if(pathfile->Find(__L("Platforms/Android")                      , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_ANDROID");
      if(pathfile->Find(__L("Platforms/Microcontrollers/STM32")       , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_STM32");
      if(pathfile->Find(__L("Platforms/Microcontrollers/ESP32")       , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_ESP32");
      if(pathfile->Find(__L("Platforms/Microcontrollers/SAMD5xE5x")   , true) != XSTRING_NOTFOUND) result = __L("PLATFORM_SAMD5XE5X");           
    }

  if(result.IsEmpty())                                
    {
      result = __L("APPLICATION");
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_GREEN, __L(" %s | %s"), pathfile->Get(), result.Get());

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




