/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XTraceMonitor_CFG.h
*
* @class       XTRACEMONITOR_CFG
* @brief       eXtended Trace Monitor Config
* @ingroup     UTILITIES
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

#ifndef _XTRACEMONITOR_CFG_H_
#define _XTRACEMONITOR_CFG_H_
 
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XTrace.h"
#include "XFileCFG.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XTRACEMONITOR_CFG_TYPESERVER
{
  XTRACEMONITOR_CFG_TYPESERVER_UNKWOWN    = 0 ,
  XTRACEMONITOR_CFG_TYPESERVER_UDP            ,
  XTRACEMONITOR_CFG_TYPESERVER_UART             
};


#define XTRACEMONITOR_CFG_SECTION_GENERAL				        _L("general")
#define XTRACEMONITOR_CFG_XPOS									        _L("xpos")
#define XTRACEMONITOR_CFG_YPOS									        _L("ypos")
#define XTRACEMONITOR_CFG_WIDTH								          _L("width")
#define XTRACEMONITOR_CFG_HEIGHT								        _L("height")
#define XTRACEMONITOR_CFG_ISCHECKINTERNETAVAILABLE	    _L("ischeckinternetavailable")
#define XTRACEMONITOR_CFG_ISADDTIMEMARK					        _L("isaddtimemark")
#define XTRACEMONITOR_CFG_ISTEXTTRACING					        _L("istexttracing")
#define XTRACEMONITOR_CFG_ISSHOWIPS     				        _L("isshowips")
#define XTRACEMONITOR_CFG_ISSHOWSTATUSMSG 			        _L("isshowstatemsg")
#define XTRACEMONITOR_CFG_STATUSMSGREFRESH 			        _L("statemsgrefresh")
#define XTRACEMONITOR_CFG_ISSHOWCOMMANDMSG              _L("isshowcommandmsg")
#define XTRACEMONITOR_CFG_ROOTORIGINNAME               	_L("rootoriginname")
#define XTRACEMONITOR_CFG_LOCALNOINTERNETORIGINNAME   	_L("localnointernetoriginname")
#define XTRACEMONITOR_CFG_WINDOWSMESSAGEORIGINNAME     	_L("windowsmessageoriginname")
#define XTRACEMONITOR_CFG_UARTORIGINNAME   	            _L("uartoriginname")
#define XTRACEMONITOR_CFG_MAXTEXTSIZE          	        _L("maxtextsize")
#define XTRACEMONITOR_CFG_SAVEDIR              	        _L("savedir")
#define XTRACEMONITOR_CFG_SERVERIPSELECTED      	      _L("serveripselected")
#define XTRACEMONITOR_CFG_SERVERUARTSELECTED      	    _L("serveruartselected")

#define XTRACEMONITOR_CFG_SECTION_FILTERS			          _L("filters")
#define XTRACEMONITOR_CFG_FILTERS_ISTEXTFILTERACTIVE	  _L("istextfilteractive")
#define XTRACEMONITOR_CFG_FILTERS_TEXTFILTER					  _L("textfilter")
#define XTRACEMONITOR_CFG_FILTERS_LEVELFILTER					  _L("levelfilter")
#define XTRACEMONITOR_CFG_FILTERS_ISTREEVISIBLE     	  _L("istreevisible")
#define XTRACEMONITOR_CFG_FILTERS_TREEWIDTH	            _L("treewidth")
#define XTRACEMONITOR_CFG_FILTERS_STATUSMSGWIDTH	      _L("statusmsgwidth")

#define XTRACEMONITOR_CFG_SECTION_DYNDNS    		        _L("dyndns")
#define XTRACEMONITOR_CFG_DYNDNS_ISCHANGEAVAILABLE      _L("ischangeavailable")
#define XTRACEMONITOR_CFG_DYNDNS_LOGIN         	        _L("login")
#define XTRACEMONITOR_CFG_DYNDNS_PASSWORD       	      _L("password")

#define XTRACEMONITOR_CFG_SECTION_SERVERS 		          _L("server")
#define XTRACEMONITOR_CFG_SERVERS_TYPE        	        _L("type")
#define XTRACEMONITOR_CFG_SERVERS_NAME		              _L("name")     
#define XTRACEMONITOR_CFG_SERVERS_CONFIG                _L("config")

#define XTRACEMONITOR_CFG_MAXSERVERS                    10

#define XTRACEMONITOR_CFG_SECTION_ORIGINS				        _L("origins")
#define XTRACEMONITOR_CFG_ORIGINS_ISACTIVE				      _L("isactive")
#define XTRACEMONITOR_CFG_ORIGINS_SHOWIP  				      _L("showip")
#define XTRACEMONITOR_CFG_ORIGINS_NAME						      _L("name")
#define XTRACEMONITOR_CFG_ORIGINS_URL						        _L("url")
#define XTRACEMONITOR_CFG_ORIGINS_IP							      _L("ip")

#define XTRACEMONITOR_CFG_MAXORIGINS						        64

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPATHS;
class XSYSTEM;


class XTRACEMONITOR_SERVERCFG
{
	public:
																		XTRACEMONITOR_SERVERCFG                         ();
		virtual												 ~XTRACEMONITOR_SERVERCFG 												();

		int                             type;    
		XSTRING													name;
		XSTRING													config;
		
	private:

		void                            Clean																						();																		
};



class XTRACEMONITOR_ORIGINCFG
{
	public:
																		XTRACEMONITOR_ORIGINCFG                         ();
		virtual												 ~XTRACEMONITOR_ORIGINCFG													();

		bool														isactive;
    bool                            showip;
		XSTRING													name;
		XSTRING													URL;
		XSTRING													IP;

	private:

		void                            Clean																						();
																		
};



class XTRACEMONITOR_CFG : public XFILECFG
{
	public:
                                    XTRACEMONITOR_CFG                               (XCHAR* namefile);
    virtual                        ~XTRACEMONITOR_CFG                               ();

    bool                            DoVariableMapping                               ();
    bool                            DoDefault                                       ();

		int															GetXPos																					();
    void														SetXPos																					(int xpos);
    int															GetYPos																					();
    void														SetYPos																					(int ypos);
		int															GetWidth																				();
    void														SetWidth																				(int width);
    int															GetHeight																				();				
		void														SetHeight																				(int height);		
    bool                            IsCheckInternetAvailable                        ();
    bool														IsAddTimemark																		();
		void														SetIsAddTimemark																(bool isaddtimemark);
		bool														IsTextTracing																		();
		void														SetIsTextTracing																(bool istexttracing);
		bool														IsShowIPs       																();
		void														SetIsShowIPs        														(bool isshowIPs);
  	bool														IsShowStatusMsg  																();
		void														SetIsShowStatusMsg   														(bool isshowstatusmsg);
    int     												GetStatusMsgRefresh 	  		     				        ();
    bool                            IsShowCommandMsg                                ();
    XSTRING*                        GetRootOriginName                               ();
    XSTRING*                        GetLocalNoInternetOriginName                    ();
    XSTRING*                        GetWindowsMessageOriginName                     ();
    XSTRING*                        GetUARTOriginName                               ();
    XDWORD                          GetMaxTextSize                                  ();
    XSTRING*                        GetSaveDir                                      ();
    int                             GetServerIPSelected                             ();
    void                            SetServerIPSelected                             (int serverIPselected);
    int                             GetServerUARTSelected                           ();
    void                            SetServerUARTSelected                           (int serverUARTselected);

    bool                            DynDNS_IsChangeAvailable                        ();
    XSTRING*												DynDNS_GetLogin          	  										();
    XSTRING*												DynDNS_GetPassword        	  								  ();

		bool														Filters_IsTextActive       											();
		void														Filters_SetIsTextActive 												(bool istextactive);
    XSTRING*												Filters_GetText							            				();
 		XDWORD													Filters_GetLevels				        								();
		void														Filters_SetLevels       		 									  (XDWORD levels);
    bool    												Filters_IsTreeVisible			  		        				();
    void    												Filters_SetIsTreeVisible         								(bool treevisible);
    int     												Filters_GetTreeWidth			  		        				();
    void    												Filters_SetTreeWidth			       								(int treewidth);
    int     												Filters_GetStatusMsgWidth			  		     				();
    void    												Filters_SetStatusMsgWidth			        					(int status);

    XTRACEMONITOR_SERVERCFG*        GetServer                                       (int index, XTRACEMONITOR_CFG_TYPESERVER typeserver);
		XTRACEMONITOR_ORIGINCFG*				GetOrigin																				(int index);

	private:

    void                            Clean                                           ();

		int															xpos;
		int															ypos;
		int															width;
		int															height;
    bool                            ischeckinternetavailable;
    bool                            isaddtimemark;
		bool                            istexttracing;
		bool                            isshowIPs;
    bool                            isshowstatusmsg;
    bool                            isshowcommandmsg;
    int                             statusmsgrefresh;
    XSTRING                         rootoriginname;
    XSTRING                         localnointernetoriginname;
    XSTRING                         windowsmessageoriginname;
    XSTRING                         UARToriginname;
    XDWORD                          maxtextsize;
    XSTRING                         savedir;
    int                             serverIPselected;
    int                             serverUARTselected;

    bool														filters_istextactive;
		XSTRING													filters_text;
    XDWORD                          filters_levels;
    bool														filters_istreevisible;
    int                             filters_treewidth;
    int                             filters_statusmsgwidth;


    bool                            dyndns_ischangeavailable;
    XSTRING                         dyndns_login;
    XSTRING                         dyndns_password;

    XTRACEMONITOR_SERVERCFG         servers[XTRACEMONITOR_CFG_MAXSERVERS];
		XTRACEMONITOR_ORIGINCFG				  origins[XTRACEMONITOR_CFG_MAXORIGINS];
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

