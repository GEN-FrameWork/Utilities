/**-------------------------------------------------------------------------------------------------------------------
*
* @file       Unit1.h
*
* @class      UNIT1
* @brief      Unit1
* @ingroup    XUTILS
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

#ifndef _UNIT1_H_
#define _UNIT1_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>

#include <vector>

#include "XBase.h"
#include "XPath.h"
#include "XString.h"
#include "XVector.h"

#include "XTraceMonitor_CFG.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XTRACEMONITOR_CFGNAMEFILE	 	 			                _L("XTraceMonitor")
#define XTRACEMONITOR_VERSIONLABEL                        _L("Version 7.6.2")
#define XTRACEMONITOR_ROOTDIR                             _L("assets")

#define XTRACEMONITOR_MASKLEVELBLACK                      0x00000001
#define XTRACEMONITOR_MASKLEVELPURPLE                     0x00000002
#define XTRACEMONITOR_MASKLEVELGREEN                      0x00000004
#define XTRACEMONITOR_MASKLEVELBLUE                       0x00000008
#define XTRACEMONITOR_MASKLEVELRED                        0x00000010
#define XTRACEMONITOR_MASKLEVELGRAY                       0x00000020

#define XTRACEMONITOR_MAXSIZEINKUDPBUFFER                 1024
#define XTRACEMONITOR_MAXSIZEINKUARTBUFFER                1024

#define XTRACEMONITOR_MINBLOCKADDMSG                      100000

#define XTRACEMONITOR_MINPUBLICIPFILTERWIDTH              0
#define XTRACEMONITOR_MAXPUBLICIPFILTERWIDTH              450

#define XTRACEMONITOR_MINPUBLICSTATUSMSGWIDTH             200
#define XTRACEMONITOR_MAXPUBLICSTATUSMSGWIDTH             350

#define XTRACEMONITOR_NODEORIGINDEFAULTNAME              _L("IP")
#define XTRACEMONITOR_NODEORIGINDEFAULTNAMENOTPACKET     _L("IP*")

#define XTRACEMONITOR_GRPVIEWPORT_ID_MAIN                _L("mainviewport")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSFACTORY;
class XWINDOWSTHREAD;
class XSTRING;
class XMUTEX;
class XTHREADCOLLECTED;
class DIOWINDOWSFACTORY;
class DIOSTREAMUDPCONFIG;
class	DIOSTREAMUDP;
class DIOSTREAMUARTCONFIG;
class DIOSTREAMUART;
class	DIOCHECKTCPIPCONNECTIONS;
class DIOCHECKINTERNETCONNECTION;
class GRPSCREEN;
class GRPWINDOWSSCREEN;
class GRP2DCANVAS;


struct MONITOR_RECTS
{
    std::vector<RECT>   rcMonitors;
    RECT                rcCombined;

    static BOOL CALLBACK MonitorEnum(HMONITOR hMon,HDC hdc,LPRECT lprcMonitor,LPARAM pData)
    {
        MONITOR_RECTS* pThis = reinterpret_cast<MONITOR_RECTS*>(pData);
        pThis->rcMonitors.push_back(*lprcMonitor);
        UnionRect(&pThis->rcCombined, &pThis->rcCombined, lprcMonitor);
        return TRUE;
    }

    MONITOR_RECTS()
    {
        SetRectEmpty(&rcCombined);
        EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
    }
};


class ORIGIN
{
  public:


                                  ORIGIN                        ()    { Clean();                                  }
    virtual							 	       ~ORIGIN                        ()    { Clean();                                  }

    bool                          showip;
    XSTRING                       name;
    XSTRING                       URL;
    XSTRING                       publicIP;
    XSTRING                       localIP;
    bool                          isuart;  
    TTreeNode*                    node;
    TRichEdit*                    ritchtext;
    TValueListEditor*             valuelisteditor;

    XTRACE_STATUS_MSGS            status_msgs;

  private:

    void                          Clean()
                                  {
                                    showip          = false;
                                    node            = NULL;
                                    isuart          = false;
                                    ritchtext       = NULL;
                                    valuelisteditor = NULL;
                                  }
};


class DBGMESSAGE
{
   public:
                                  DBGMESSAGE                    ()    { Clean();                                  }
    virtual							 	       ~DBGMESSAGE                    ()    { Clean();                                  }

    bool                          iserase;
    XDWORD                        publicIP;
    bool                          publicIPnopacket;
    XDWORD                        localIP;
    XBYTE                         level;
    XDWORD                        sequence;
    XDATETIME                     xtime;
    XSTRING                       string;

  private:

    void                          Clean()
                                  {
                                    iserase          = false;
                                    publicIP         = 0;
                                    publicIPnopacket = false;
                                    localIP          = 0;
                                    level            = 0;
                                    sequence         = 0;
                                  }
};


class TMainForm : public TForm
{
  __published:	// IDE-managed Components

    TPanel*                       ToolbarPanel;
    TImage*                       IMGMain;
    TBitBtn*                      ButtonClear;
    TBitBtn*                      ButtonStop;
    TBitBtn*                      ButtonExit;
    TBitBtn*                      ButtonMinimize;
    TBitBtn*                      ButtonRun;
    TLabel*                       VersionLabel;
    TCheckBox*                    TextTracingCheckBox;
    TBitBtn*                      ButtonCopy;
    TEdit*                        FilterEdit;
    TCheckBox*                    FilterCheckBox;
    TBitBtn*                      ButtonDNSUpdate;
    TCheckBox*                    AddTimeMarkCheckBox;
    TTimer*                       ResolvedOriginTimer;
    TImage*                       InternetImage;
    TImageList*                   InternetImageList;
    TLabel*                       InternetLabel;
    TPanel*                       InternetPanel;
    TTimer*                       InternetUpdateTimer;
    TPanel*                       OriginsPanel;
    TTreeView*                    OriginsTreeView;
    TPanel*                       TextPanel;
    TPanel*                       StatusPanel;
    TRichEdit*                    StatusRichEdit;
    TCheckBox*                    CheckLevelFilterBlack;
    TCheckBox*                    CheckLevelFilterPurple;
    TCheckBox*                    CheckLevelFilterGreen;
    TCheckBox*                    CheckLevelFilterBlue;
    TCheckBox*                    CheckLevelFilterRed;
    TCheckBox*                    CheckLevelFilterGray;
    TLabel*                       CheckLevelFilterLabel;
    TPanel*                       PanelLevelFilter;
    TCheckBox*                    ShowIPsCheckBox;
    TBitBtn*                      ButtonSave;
    TPanel*                       BufferStatusPanel;
    TProgressBar*                 UDPBufferStatusProgressBar;
    TProgressBar*                 UARTBufferStatusProgressBar;
    TLabel*                       UDPBufferStatusLabel;
    TLabel*                       UARTBufferStatusLabel;
    TTimer*                       AddDBMsgTimer;
    TImageList*                   IPFilterImageList;
    TImage*                       IPFilterImage;
    TLabel*                       IPFilterLabel;
    TComboBox*                    ServerIPComboBox;
    TSplitter*                    SplitterIPFilter;
    TComboBox*                    ServerUARTComboBox;
    TPanel*                       ServerPanel;
    TLabel*                       ServerUDPLabel;
    TLabel*                       ServerUARTLabel;
    TSplitter*                    SplitterTop;
    TPanel*                       StatusMSGPanel;
    TSplitter*                    SplitterStatusMsg;
    TCheckBox*                    CheckBoxMsgStatus;
    TBitBtn*                      ButtonClearStatus;
    TTimer*                       TimerStatusMSG;

    void __fastcall               FormCreate                    (TObject *Sender);
    void __fastcall               FormClose                     (TObject *Sender, TCloseAction &Action);
    void __fastcall               ActivarMensajeria1Click       (TObject *Sender);
    void __fastcall               FormCanResize                 (TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
    void __fastcall               ButtonClearClick              (TObject *Sender);
    void __fastcall               ButtonStopClick               (TObject *Sender);
    void __fastcall               ButtonMinimizeClick           (TObject *Sender);
    void __fastcall               ButtonExitClick               (TObject *Sender);
    void __fastcall               Clearmessages1Click           (TObject *Sender);
    void __fastcall               Exit1Click                    (TObject *Sender);
    void __fastcall               Minimizewindow1Click          (TObject *Sender);
    void __fastcall               ButtonCopyClick               (TObject *Sender);
    bool __fastcall               GenerateTab                   (int ntab, AnsiString& tab);
    void __fastcall               FilterCheckBoxClick           (TObject *Sender);
    bool __fastcall               CheckFilterLine               (AnsiString& line);
    void __fastcall               ButtonDNSUpdateClick          (TObject *Sender);
    void __fastcall               ResolvedOriginTimerTimer      (TObject *Sender);
    void __fastcall               InternetUpdateTimerTimer      (TObject *Sender);
    void __fastcall               OriginsTreeViewChange         (TObject *Sender, TTreeNode *Node);
    void __fastcall               CheckLevelFilterBlackClick    (TObject *Sender);
    void __fastcall               CheckLevelFilterBlueClick     (TObject *Sender);
    void __fastcall               CheckLevelFilterGreenClick    (TObject *Sender);
    void __fastcall               CheckLevelFilterPurpleClick   (TObject *Sender);
    void __fastcall               CheckLevelFilterRedClick      (TObject *Sender);
    void __fastcall               CheckLevelFilterGrayClick     (TObject *Sender);
    void __fastcall               PanelLevelFilterClick         (TObject *Sender);
    void __fastcall               AddTimeMarkCheckBoxClick      (TObject *Sender);
    void __fastcall               TextTracingCheckBoxClick      (TObject *Sender);
    void __fastcall               ShowIPsCheckBoxClick          (TObject *Sender);
    void __fastcall               FormActivate                  (TObject *Sender);
    void __fastcall               ButtonSaveClick               (TObject *Sender);
    void __fastcall               AddDBMsgTimerTimer            (TObject *Sender);
    void __fastcall               IPFilterImageClick            (TObject *Sender);
    void __fastcall               SplitterIPFilterMoved         (TObject *Sender);
    void __fastcall               ServerIPComboBoxChange        (TObject *Sender);
    void __fastcall               ServerUARTComboBoxChange      (TObject *Sender);
    void __fastcall               SplitterStatusMsgMoved        (TObject *Sender);
    void __fastcall               CheckBoxMsgStatusClick        (TObject *Sender);
    void __fastcall               ButtonClearStatusClick        (TObject *Sender);
    void __fastcall               ValueListEditorDrawCell       (TObject *Sender, int ACol, int ARow, const TRect &Rect, TGridDrawState State);
    void __fastcall               TimerStatusMSGTimer           (TObject *Sender);

  public:

    void                          AddDBGmessageManager          ();

  private:	// User declarations

    __fastcall                    TMainForm                     (TComponent* Owner);
    void __fastcall               CheckCopyData                 (Messages::TMessage &Message);
    void __fastcall               Minimize                      (TObject *Sender);

    bool                          UpdateInternetStatus          ();
    bool                          ActivateButtons               (bool active);
    bool                          RefreshStatusTextFilter       ();

    TRichEdit*                    CreateRitchText               ();
    TValueListEditor*             CreateValueListEditor         ();
    ORIGIN*                       CreateOrigin                  (ORIGIN* father, XTRACEMONITOR_ORIGINCFG* origincfg);
    bool                          CreateOriginsFromConfig       (bool addUART);
    ORIGIN*                       CreateOriginsFromIP           (XDWORD publicIP, bool publicIPnopacket, XDWORD localIP);
    ORIGIN*                       GetOriginFromIP               (XDWORD publicIP, XDWORD localIP);
    bool                          CreateOriginFromUART          ();
    bool                          DeleteOriginFromUART          ();
    ORIGIN*                       GetOriginFromTreeNode         (TTreeNode* node);
    ORIGIN*                       GetActualOrigin               ();
    void                          ResolvedAllURLOrigins         ();

    bool                          AddLineTrace                  (ORIGIN* origin, DBGMESSAGE* DBGmessage);
    void                          UpdateRunStopButton           (bool run);
    bool                          AddDBGMessage                 (XDWORD publicIP, bool publicIPnopacket, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& string);

    void                          GenerateIPString              (XDWORD IP, XSTRING& IPstring);
    void                          UpdateLevelFilterStatus       ();
    void                          CheckLevelFilter              (TCheckBox* checkbox, XDWORD mask);
    bool                          IsActiveLevelFilter           (int level);

    void                          UpdateIPFilterImage           ();

    bool                          PrintStatus                   (XCHAR* mask, ...);
    bool                          GetNameNode                   (ORIGIN* origin, XSTRING& namenode);

    void                          InitServerList                ();

    bool                          OpenUDPServer                 (XSTRING* servercfg);
    bool                          CloseUDPServer                ();

    bool                          OpenUARTServer                (XSTRING* servercfg);
    bool                          CloseUARTServer               ();

    bool                          RedrawStatusMsgList           (ORIGIN* origin);

    bool                          IsWindowOutsideExtendedDesktop(HWND hwnd);

    static void									  ThreadReadUDPFunction			    (void* param);
    static void									  ThreadReadUARTFunction		    (void* param);

    void                          Clean                         ();


		DIOCHECKINTERNETCONNECTION*		checkinternetconnection;
    bool                          haveinternet;

    bool                          run;

    bool                          exitapp;
    bool                          changeorigin;

    XTRACEMONITOR_CFG*		 		    cfg;

    DIOSTREAMUDPCONFIG*           diostreamudpcfg;
		DIOSTREAMUDP*						      diostreamudp;
    bool                          openudp;

    DIOSTREAMUARTCONFIG*          diostreamuartcfg;
		DIOSTREAMUART*						    diostreamuart;
    bool                          openuart;

    GRPWINDOWSSCREEN*             grp_screen;
    GRP2DCANVAS*                    grp_canvas;

    ORIGIN*                       rootorigin;
    ORIGIN*                       localnointernetorigin;
    bool                          noreadudp;

    ORIGIN*                       UARTorigin;
    bool                          noreaduart;

    XDATETIME*                    xdatetime;

    XVECTOR<ORIGIN*>              origins;

    XMUTEX*												xmutexaddDBGmessage;
    XMUTEX*												xmutexaddorigin;
    
    XVECTOR<DBGMESSAGE*>          DBGmessages;

		XTHREADCOLLECTED*							xthreadreadUDP;
		XTHREADCOLLECTED*							xthreadreadUART;

    int                           timerID;  
};


/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

extern PACKAGE TMainForm *MainForm;


void CALLBACK AddDBGmessageTimerProc(unsigned int uID, unsigned int uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);


#endif
