/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XTraceMonitor_CFG.cpp
*
* @class       XTRACEMONITOR_CFG
* @brief       eXtended Trace Monitor Config
* @ingroup     UTILITIES

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

#include "Common_DefaultXTraceCFG.h"

#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/*  XTRACEMONITOR_SERVERCFG                                                                                           */
/*--------------------------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_SERVERCFG::XTRACEMONITOR_SERVERCFG()
* @brief      Constructor
* @ingroup    UTILITIES
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_SERVERCFG::XTRACEMONITOR_SERVERCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_SERVERCFG::~XTRACEMONITOR_SERVERCFG()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_SERVERCFG::~XTRACEMONITOR_SERVERCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_SERVERCFG::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_SERVERCFG::Clean()
{
  type = XTRACEMONITOR_CFG_TYPESERVER_UNKWOWN;
	name.Empty();
	config.Empty();
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*  XTRACEMONITOR_ORIGINCFG                                                                                           */
/*--------------------------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINCFG::XTRACEMONITOR_ORIGINCFG()
* @brief      Constructor
* @ingroup    UTILITIES
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGINCFG::XTRACEMONITOR_ORIGINCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINCFG::~XTRACEMONITOR_ORIGINCFG()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILITIES
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGINCFG::~XTRACEMONITOR_ORIGINCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_ORIGINCFG::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_ORIGINCFG::Clean()
{
	isactive = false;
  showip   = false;
	name.Empty();
	URL.Empty();
  IP.Empty();
}


/*--------------------------------------------------------------------------------------------------------------------*/
/*  XTRACEMONITOR_CFG                                                                                                 */
/*--------------------------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_CFG::XTRACEMONITOR_CFG(XCHAR* namefile) : XFILECFG(namefile)
* @brief      Constructor
* @ingroup
*
* @param[in]  XCHAR* : name of file of config
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG::XTRACEMONITOR_CFG(XCHAR* namefile) : XFILECFG(namefile)
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
* @brief      Destructor
* @note       VIRTUAL
* @ingroup
*
* @return     Does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_CFG::~XTRACEMONITOR_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DoVariableMapping()
* @brief      DoVariableMapping
* @ingroup    APPLICATION
*
* @return     bool : true if is succesful.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DoVariableMapping()
{
  XSTRING section;

  if(!XFILECFG::DoVariableMapping())
    {
      return false;
    }

  //-----------------------------------------------------
	// GENERAL
	AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_XPOS						            , &xpos);
	AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_YPOS					    	        , &ypos);
	AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_WIDTH					    	        , &width);
	AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_HEIGHT				    	        , &height);

  AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISCHECKINTERNETAVAILABLE    , &ischeckinternetavailable);

	AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISADDTIMEMARK	    	        , &isaddtimemark);
	AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISTEXTTRACING	    	        , &istexttracing);
	AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISSHOWIPS                   , &isshowIPs);
	AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISSHOWSTATUSMSG             , &isshowstatusmsg);
	AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ISSHOWCOMMANDMSG            , &isshowcommandmsg);
  AddValue(XFILECFG_VALUETYPE_INT   	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_STATUSMSGREFRESH            , &statusmsgrefresh);

  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_ROOTORIGINNAME              , &rootoriginname);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_LOCALNOINTERNETORIGINNAME   , &localnointernetoriginname);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_WINDOWSMESSAGEORIGINNAME    , &windowsmessageoriginname);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_UARTORIGINNAME              , &UARToriginname);

  AddValue(XFILECFG_VALUETYPE_INT   	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_MAXTEXTSIZE			            , &maxtextsize);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_SAVEDIR                     , &savedir);

  AddValue(XFILECFG_VALUETYPE_INT   	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_SERVERIPSELECTED            , &serverIPselected);
  AddValue(XFILECFG_VALUETYPE_INT   	  , XTRACEMONITOR_CFG_SECTION_GENERAL		, XTRACEMONITOR_CFG_SERVERUARTSELECTED          , &serverUARTselected);

  //-----------------------------------------------------
	// FILTERS

  AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE	, &filters_istextactive);
	AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_TEXTFILTER		    	, &filters_text);
  AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_LEVELFILTER		    	, &filters_levels);
  AddValue(XFILECFG_VALUETYPE_BOOLEAN   , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_ISTREEVISIBLE       , &filters_istreevisible);
  AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_TREEWIDTH           , &filters_treewidth);
  AddValue(XFILECFG_VALUETYPE_INT			  , XTRACEMONITOR_CFG_SECTION_FILTERS		, XTRACEMONITOR_CFG_FILTERS_STATUSMSGWIDTH      , &filters_statusmsgwidth);

  //-----------------------------------------------------
	// DYNDNS
  AddValue(XFILECFG_VALUETYPE_BOOLEAN	  , XTRACEMONITOR_CFG_SECTION_DYNDNS		, XTRACEMONITOR_CFG_DYNDNS_ISCHANGEAVAILABLE    , &dyndns_ischangeavailable);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_DYNDNS		, XTRACEMONITOR_CFG_DYNDNS_LOGIN                , &dyndns_login);
  AddValue(XFILECFG_VALUETYPE_STRING	  , XTRACEMONITOR_CFG_SECTION_DYNDNS		, XTRACEMONITOR_CFG_DYNDNS_PASSWORD             , &dyndns_password);

  //-----------------------------------------------------
	// SERVERS
	for(int c=0; c<XTRACEMONITOR_CFG_MAXSERVERS; c++)
		{
			section.Format(_L("%s %02d"), XTRACEMONITOR_CFG_SECTION_SERVERS, c+1);

			AddValue(XFILECFG_VALUETYPE_INT      	, section.Get()		, XTRACEMONITOR_CFG_SERVERS_TYPE    	,	&servers[c].type);
      AddValue(XFILECFG_VALUETYPE_STRING		, section.Get()		, XTRACEMONITOR_CFG_SERVERS_NAME			,	&servers[c].name);
			AddValue(XFILECFG_VALUETYPE_STRING		, section.Get()   , XTRACEMONITOR_CFG_SERVERS_CONFIG    ,	&servers[c].config);			
		}

	//-----------------------------------------------------
	// ORIGINS
	for(int c=0; c<XTRACEMONITOR_CFG_MAXORIGINS; c++)
		{
			section.Format(_L("%s %02d"), XTRACEMONITOR_CFG_SECTION_ORIGINS, c+1);

			AddValue(XFILECFG_VALUETYPE_BOOLEAN 	, section.Get()		, XTRACEMONITOR_CFG_ORIGINS_ISACTIVE	,	&origins[c].isactive);
      AddValue(XFILECFG_VALUETYPE_BOOLEAN 	, section.Get()		, XTRACEMONITOR_CFG_ORIGINS_SHOWIP    ,	&origins[c].showip);
			AddValue(XFILECFG_VALUETYPE_STRING		, section.Get()		, XTRACEMONITOR_CFG_ORIGINS_NAME			,	&origins[c].name);
			AddValue(XFILECFG_VALUETYPE_STRING		, section.Get()   , XTRACEMONITOR_CFG_ORIGINS_URL				,	&origins[c].URL);
			AddValue(XFILECFG_VALUETYPE_STRING		, section.Get()		, XTRACEMONITOR_CFG_ORIGINS_IP				,	&origins[c].IP);
		}

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DoDefault()
* @brief      DoDefault
* @ingroup    APPLICATION
*
* @return     bool : true if is succesful.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DoDefault()
{
  xpos								          = 613;
	ypos								          = 349;
	width								          = 694;
	height							          = 381;
  ischeckinternetavailable      = false;
	isaddtimemark				          = true;
	istexttracing				          = true;
  isshowIPs                     = true;
  isshowstatusmsg               = true;
  isshowcommandmsg              = false;
  statusmsgrefresh              = 8;
  rootoriginname                = _L("All");
  localnointernetoriginname     = _L("Local (no internet)");
  windowsmessageoriginname      = _L("Windows messages");
  UARToriginname                = _L("UART");
  maxtextsize                   = 512;
  savedir                       = _L("savedir");
  serverIPselected              = 0;
  serverUARTselected            = 0;

  filters_istextactive          = false;
  filters_text                  = _L("");
  filters_levels                = 0x000000FF;
  filters_istreevisible         = false;
  filters_treewidth             = 264;
  filters_statusmsgwidth        = 250;


  dyndns_ischangeavailable      = false;
  dyndns_login                  = _L("");
  dyndns_password               = _L("");

  //defaulturl                    = _L("xdebug.kicks-ass.org");
  //defaultport                   = 10222;
  //defaultuart                   = _L("0,115200,8,N,1,NONE");

  servers[0].type               = XTRACEMONITOR_CFG_TYPESERVER_UDP;
	servers[0].name               = _L("Old Config Kick-Ass");
	servers[0].config             = _L("xdebug.kicks-ass.org:10021");

  servers[1].type               = XTRACEMONITOR_CFG_TYPESERVER_UDP;
	servers[1].name               = _L("GEN Default Config");
	servers[1].config             = GEN_XTRACE_NET_DEFAULT_01;

  servers[2].type               = XTRACEMONITOR_CFG_TYPESERVER_UART;
	servers[2].name               = _L("UART STM32");
	servers[2].config             = _L("0,115200,8,N,1,NONE");


	origins[0].isactive           = true;
  origins[0].showip             = true;
	origins[0].name			          = _L("Local");
	origins[0].URL			          = _L("xdebug.kicks-ass.org");
	origins[0].IP				          = _L("");

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetXPos()
* @brief      GetXPos
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetXPos()
{
  return xpos;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetXPos(int xpos)
* @brief      SetXPos
* @ingroup    UTILITIES
*
* @param[in]  xpos :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetXPos(int xpos)
{
  this->xpos = xpos;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetYPos()
* @brief      GetYPos
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetYPos()
{
  return ypos;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetYPos(int ypos)
* @brief      SetYPos
* @ingroup    UTILITIES
*
* @param[in]  ypos :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetYPos(int ypos)
{
  this->ypos = ypos;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetWidth()
* @brief      GetWidth
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetWidth()
{
  return width;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetWidth(int width)
* @brief      SetWidth
* @ingroup    UTILITIES
*
* @param[in]  width :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetWidth(int width)
{
  this->width = width;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetHeight()
* @brief      GetHeight
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetHeight()
{
  return height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetHeight(int height)
* @brief      SetHeight
* @ingroup    UTILITIES
*
* @param[in]  height :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetHeight(int height)
{
  this->height = height;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsCheckInternetAvailable()
* @brief      IsDNSChangeAvaible
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsCheckInternetAvailable()
{
  return ischeckinternetavailable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsAddTimemark()
* @brief      IsAddTimemark
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsAddTimemark()
{
  return isaddtimemark;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetIsAddTimemark(bool isaddtimemark)
* @brief      SetIsAddTimemark
* @ingroup    UTILITIES
*
* @param[in]  isaddtimemark :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetIsAddTimemark(bool isaddtimemark)
{
  this->isaddtimemark = isaddtimemark;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsTextTracing()
* @brief      IsTextTracing
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsTextTracing()
{
  return istexttracing;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetIsTextTracing(bool istexttracing)
* @brief      SetIsTextTracing
* @ingroup    UTILITIES
*
* @param[in]  istexttracing :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetIsTextTracing(bool istexttracing)
{
  this->istexttracing = istexttracing;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsShowIPs()
* @brief      IsShowIPs
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsShowIPs()
{
  return isshowIPs;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetIsShowIPs(bool isshowIPs)
* @brief      SetIsShowIPs
* @ingroup    UTILITIES
*
* @param[in]  isshowIPs :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetIsShowIPs(bool isshowIPs)
{
  this->isshowIPs = isshowIPs;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsShowStatusMsg()
* @brief      IsShowStatusMsg
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsShowStatusMsg()
{
  return isshowstatusmsg;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetIsShowStatusMsg(bool isshowstatusmsg)
* @brief      SetIsShowStatusMsg
* @ingroup    UTILITIES
*
* @param[in]  isshowIPs :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetIsShowStatusMsg(bool isshowstatusmsg)
{
  this->isshowstatusmsg = isshowstatusmsg;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetStatusMsgRefresh()
* @brief      GetStatusMsgRefresh
* @ingroup    UTILITIES
*
* @return     int  :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetStatusMsgRefresh()
{
  return statusmsgrefresh;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::IsShowCommandMsg()
* @brief      IsShowCommandMsg
* @ingroup    UTILITIES
*
* @return     bool :
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::IsShowCommandMsg()
{
  return this->isshowcommandmsg;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetRootOriginName()
* @brief      GetRootOriginName
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetRootOriginName()
{
  return &rootoriginname;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetLocalNoInternetOriginName()
* @brief      GetLocalNoInternetOriginName
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetLocalNoInternetOriginName()
{
  return &localnointernetoriginname;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetWindowsMessageOriginName()
* @brief      GetLocalNoInternetOriginName
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetWindowsMessageOriginName()
{
  return &windowsmessageoriginname;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetUARTOriginName()
* @brief      GetLocalNoInternetOriginName
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetUARTOriginName()
{
  return &UARToriginname;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_CFG::GetMaxTextSize()
* @brief      GetMaxTextSize
* @ingroup    UTILITIES
*
* @return     XDWORD :
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_CFG::GetMaxTextSize()
{
  return maxtextsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::GetSaveDir()
* @brief      GetSaveDir
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::GetSaveDir()
{
  return &savedir;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetServerIPSelected()
* @brief      GetServerIPSelected
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetServerIPSelected()
{
  return serverIPselected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetServerIPSelected(int serverIPselected)
* @brief      SetServerIPSelected
* @ingroup    UTILITIES
*
* @param[in]  serverselected :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetServerIPSelected(int serverIPselected)
{
  this->serverIPselected = serverIPselected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::GetServerUARTSelected()
* @brief      GetServerUARTSelected
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::GetServerUARTSelected()
{
  return serverUARTselected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::SetServerUARTSelected(int serverUARTselected)
* @brief      SetServerUARTSelected
* @ingroup    UTILITIES
*
* @param[in]  serverUARTselected :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::SetServerUARTSelected(int serverUARTselected)
{
  this->serverUARTselected = serverUARTselected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::Filters_IsTextActive()
* @brief      Filters_IsTextActive
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::Filters_IsTextActive()
{
  return filters_istextactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetIsTextActive(bool istextactive)
* @brief      Filters_SetIsTextActive
* @ingroup    UTILITIES
*
* @param[in]  istextactive :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetIsTextActive(bool istextactive)
{
  this->filters_istextactive = istextactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::Filters_GetText()
* @brief      Filters_GetText
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::Filters_GetText()
{
  return &filters_text;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XTRACEMONITOR_CFG::Filters_GetLevels()
* @brief      Filters_GetLevels
* @ingroup    UTILITIES
*
* @return     XDWORD :
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACEMONITOR_CFG::Filters_GetLevels()
{
  return filters_levels;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetLevels(XDWORD levels)
* @brief      Filters_SetLevels
* @ingroup    UTILITIES
*
* @param[in]  levels :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetLevels(XDWORD levels)
{
  this->filters_levels = levels;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::Filters_IsTreeVisible()
* @brief      Filters_IsTreeVisible
* @ingroup    UTILITIES
*
* @return     bool :
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::Filters_IsTreeVisible()
{
  return filters_istreevisible;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetIsTreeVisible(bool istreevisible)
* @brief      Filters_SetIsTreeVisible
* @ingroup    UTILITIES
*
* @param[in]  istreevisible :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetIsTreeVisible(bool istreevisible)
{
 this->filters_istreevisible = istreevisible;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::Filters_GetTreeWidth()
* @brief      Filters_GetTreeWidth
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::Filters_GetTreeWidth()
{
  return filters_treewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetTreeWidth(int treewidth)
* @brief      Filters_SetTreeWidth
* @ingroup    UTILITIES
*
* @param[in]  treewidth :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetTreeWidth(int treewidth)
{
 filters_treewidth = treewidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XTRACEMONITOR_CFG::Filters_GetStatusMsgWidth()
* @brief      Filters_GetStatusMsgWidth
* @ingroup    UTILITIES
*
* @return     int :
*
*---------------------------------------------------------------------------------------------------------------------*/
int XTRACEMONITOR_CFG::Filters_GetStatusMsgWidth()
{
  return filters_statusmsgwidth;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Filters_SetStatusMsgWidth(int status)
* @brief      Filters_SetStatusMsgWidth
* @ingroup    UTILITIES
*
* @param[in]  status :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Filters_SetStatusMsgWidth(int status)
{
  filters_statusmsgwidth = status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_CFG::DynDNS_IsChangeAvailable()
* @brief      DynDNS_IsChangeAvailable
* @ingroup    UTILITIES
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_CFG::DynDNS_IsChangeAvailable()
{
  return dyndns_ischangeavailable;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::DynDNS_GetLogin()
* @brief      DynDNS_GetLogin
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::DynDNS_GetLogin()
{
  return &dyndns_login;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XTRACEMONITOR_CFG::DynDNS_GetPassword()
* @brief      DynDNS_GetPassword
* @ingroup    UTILITIES
*
* @return     XSTRING* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACEMONITOR_CFG::DynDNS_GetPassword()
{
  return &dyndns_password;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINCFG* XTRACEMONITOR_CFG::GetServer(int index)
* @brief      GetServer
* @ingroup    UTILITIES
*
* @param[in]  index :
*
* @return     XTRACEMONITOR_SERVERCFG* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_SERVERCFG* XTRACEMONITOR_CFG::GetServer(int index, XTRACEMONITOR_CFG_TYPESERVER typeserver)
{
	if(index < 0)															  return NULL;
	if(index >= XTRACEMONITOR_CFG_MAXSERVERS)		return NULL;

  if(typeserver == XTRACEMONITOR_CFG_TYPESERVER_UNKWOWN) return &servers[index];

  int _index;

  for(int c=0; c<XTRACEMONITOR_CFG_MAXSERVERS; c++)
    {
      XTRACEMONITOR_SERVERCFG* server  = &servers[c];
      if(server)
        {
          if(server->type == typeserver)
            {
              if(index == _index) return &servers[c];
              _index++;
            }
        }
	  }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_ORIGINCFG* XTRACEMONITOR_CFG::GetOrigin(int index)
* @brief      GetOrigin
* @ingroup    UTILITIES
*
* @param[in]  index :
*
* @return     XTRACEMONITOR_ORIGINCFG* :
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_ORIGINCFG* XTRACEMONITOR_CFG::GetOrigin(int index)
{
	if(index < 0)															  return NULL;
	if(index >= XTRACEMONITOR_CFG_MAXORIGINS)		return NULL;

	return &origins[index];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_CFG::Clean()
{
	xpos						              = 0;
	ypos					    	          = 0;
	width						              = 0;
	height					              = 0;

  ischeckinternetavailable      = false;

  isaddtimemark		              = false;
	istexttracing		              = false;
	isshowIPs                     = false;
  isshowstatusmsg               = false;
  isshowcommandmsg              = false;

  statusmsgrefresh              = 0;

  serverIPselected              = 0;
  serverUARTselected            = 0;

  dyndns_ischangeavailable      = false;

	filters_istextactive          = false;
  filters_levels                = 0x00;
  filters_istreevisible         = false;
  filters_treewidth             = 0;
  filters_statusmsgwidth        = 0;

}


