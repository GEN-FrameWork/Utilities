/**-------------------------------------------------------------------------------------------------------------------
*
* @file       Unit1.cpp
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include <vcl.h>
#pragma hdrstop

#include <winsock2.h>
#include <windows.h>

#include <iostream.h>
#include <mmsystem.h>


#include "XVector.h"
#include "XBuffer.h"
#include "XPathsManager.h"
#include "XWINDOWSFactory.h"
#include "XWINDOWSSleep.h"
#include "XWINDOWSSystem.h"
#include "XWINDOWSThread.h"
#include "XWINDOWSDateTime.h"
#include "XWINDOWSTrace.h"
#include "XFileCFG.h"
#include "XString.h"
#include "XTrace.h"
#include "XDir.h"
#include "XFile.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XLanguage_ISO_639_3.h"

#include "DIODynDNS.h"
#include "DIOCheckTCPIPConnections.h"
#include "DIOCheckInternetConnection.h"
#include "DIOWINDOWSFactory.h"
#include "DIODNSResolver.h"
#include "DIOStream.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"
#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamUART.h"
#include "DIOWebClient.h"
#include "DIODynDNS.h"


#include "Unit1.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#pragma package(smart_init)

#pragma resource "*.dfm"


#ifdef XTRACE_ACTIVE
XWINDOWSTRACE           windowsdebugtrace;
#endif


TMainForm *MainForm;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::FormCreate(TObject*Sender)
* @brief      FormCreate
* @ingroup    XUTILS
*
* @param[in]  TObject* Sender :
*
* @return     void : does not return anything.
*
* ---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::FormCreate(TObject* Sender)
{
  XWORD		versionrequested = WINSOCK_VERSION;
	WSADATA data;
	int     error;
  bool    status;

  haveinternet = false;

  exitapp      = false;
  changeorigin = false; 

  Caption = XTRACE_DEFAULT_SPECIALAIM;

  DBGmessages.SetAddInLimit(XTRACEMONITOR_MINBLOCKADDMSG);

  InternetImageList->GetBitmap(0,InternetImage->Picture->Bitmap);
  InternetLabel->Caption = "Internet [x]";

  WindowProc = CheckCopyData;

  error = WSAStartup(versionrequested, &data);
	if(error) WSACleanup();

	if(data.wVersion != versionrequested) WSACleanup();

  XFACTORY::SetInstance(new XWINDOWSFACTORY());
  if(!XFACTORY::GetIsInstanced()) return;

  XSLEEP::SetInstance(new XWINDOWSSLEEP());
  if(!XSLEEP::GetIsInstanced()) return;

  XSYSTEM::SetInstance(new XWINDOWSSYSTEM());
  if(!XSYSTEM::GetIsInstanced()) return;

  //XBUFFER::SetHardwareUseLittleEndian(GEN_XSYSTEM.HardwareUseLittleEndian());

 	DIOFACTORY::SetInstance(new DIOWINDOWSFACTORY());
  if(!DIOFACTORY::GetIsInstanced()) return;


  XTRANSLATION_GEN::GetInstance().Sentences_AddToTranslation(XLANGUAGE_ISO_639_3_CODE_ENG);

  //---------------------------------------------------------------

  AnsiString exefile = Application->ExeName;
  AnsiString exepath = ExtractFilePath(exefile);
  XPATH      xpath;

  xpath  = exepath.c_str();
  xpath += XTRACEMONITOR_ROOTDIR;

  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);

  cfg  = new XTRACEMONITOR_CFG(XTRACEMONITOR_CFGNAMEFILE);
	if(!cfg) return;


  FilterEdit->Text                = cfg->Filters_GetText()->Get();
  FilterCheckBox->Checked         = cfg->Filters_IsTextActive();

  AddTimeMarkCheckBox->Checked    = cfg->IsAddTimemark();
	TextTracingCheckBox->Checked    = cfg->IsTextTracing();
  ShowIPsCheckBox->Checked        = cfg->IsShowIPs();
  CheckBoxMsgStatus->Checked      = cfg->IsShowStatusMsg();

  RefreshStatusTextFilter();

  //--------------------------------------------------------------------------------------

  FormActivate(NULL);

  //--------------------------------------------------------------------------------------

  if(cfg->IsCheckInternetAvailable())
    {
      checkinternetconnection	= new DIOCHECKINTERNETCONNECTION();
    	if(!checkinternetconnection) return;

    	haveinternet = checkinternetconnection->Check();
    }

  //--------------------------------------------------------------------------------------

  xdatetime = GEN_XFACTORY.CreateDateTime();
  if(!xdatetime) return;

  //--------------------------------------------------------------------------------------

  xmutexaddDBGmessage = GEN_XFACTORY.Create_Mutex();
  if(!xmutexaddDBGmessage) return;

  xmutexaddorigin = GEN_XFACTORY.Create_Mutex();
  if(!xmutexaddorigin) return;

  //--------------------------------------------------------------------------------------

  xthreadreadUDP = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, _L("TMainForm::ReadUDP"), ThreadReadUDPFunction, (void*)this);
  if(xthreadreadUDP)
    {
      //xthreadreadUDP->SetPriority(XTHREADPRIORITY_REALTIME);
      xthreadreadUDP->Ini();
    }


  xthreadreadUART = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, _L("TMainForm::ReadUART"), ThreadReadUARTFunction, (void*)this);
  if(xthreadreadUART)
    {
      xthreadreadUART->Ini();
    }
  //--------------------------------------------------------------------------------------

  UpdateInternetStatus();

  //--------------------------------------------------------------------------------------

  InitServerList();

  ServerIPComboBoxChange(NULL);

  ServerUARTComboBoxChange(NULL);

  //--------------------------------------------------------------------------------------

  UpdateLevelFilterStatus();

  //--------------------------------------------------------------------------------------

  InternetUpdateTimer->Enabled  = true;
  AddDBMsgTimer->Enabled        = true;

  if(cfg->DynDNS_IsChangeAvailable())
    {
      ResolvedOriginTimer->Enabled  = true;
    }
   else ButtonDNSUpdate->Enabled  = false;

  //--------------------------------------------------------------------------------------

  if(cfg->Filters_IsTreeVisible())
    {
      //SplitterIPFilter->Left = cfg->Filters_GetTreeWidth();
      OriginsPanel->Width    = cfg->Filters_GetTreeWidth();
    }
   else
    {
      //SplitterIPFilter->Left =  XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
      OriginsPanel->Width    =  XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
    }

  SplitterIPFilterMoved(NULL);

  //SplitterStatusMsg->Left = SplitterIPFilter->Left + cfg->Filters_GetStatusMsgWidth();
  StatusMSGPanel->Width   = cfg->Filters_GetStatusMsgWidth();

  SplitterStatusMsgMoved(NULL);

  //--------------------------------------------------------------------------------------

  if(cfg->GetStatusMsgRefresh())
    {
      TimerStatusMSG->Interval  = ((cfg->GetStatusMsgRefresh()*1000)/2);
      TimerStatusMSG->Enabled   = true;
    }
   else
    {
      TimerStatusMSG->Enabled   = false;
    }

  //--------------------------------------------------------------------------------------

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::FormClose(TObject*Sender, TCloseAction &Action)
* @brief      FormClose
* @ingroup    XUTILS
*
* @param[in]  TObject*Sender :
* @param[in]  &Action :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  noreadudp  = true;
  noreaduart = true;

  InternetUpdateTimer->Enabled  = false;
  AddDBMsgTimer->Enabled        = false;
  ResolvedOriginTimer->Enabled  = false;

  Application->OnIdle = NULL;

  if(xthreadreadUDP)
		{
			xthreadreadUDP->End();

			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadreadUDP);
			xthreadreadUDP = NULL;
		}

  if(xthreadreadUART)
		{
			xthreadreadUART->End();

			DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadreadUART);
			xthreadreadUART = NULL;
		}


  if(xmutexaddDBGmessage)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexaddDBGmessage);
      xmutexaddDBGmessage = NULL;
    }

  if(xmutexaddorigin)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexaddorigin);
      xmutexaddorigin = NULL;
    }

  if(xdatetime)
    {
      GEN_XFACTORY.DeleteDateTime(xdatetime);
      xdatetime = NULL;
    }

  cfg->SetXPos(Left);
  cfg->SetYPos(Top);
  cfg->SetWidth(Width);
  cfg->SetHeight(Height);

  cfg->Filters_GetText()->Set(FilterEdit->Text.c_str());
  cfg->Filters_SetIsTextActive(FilterCheckBox->Checked);

  //cfg->Filters_SetPublicIPWidth((SplitterIPFilter->Left==XTRACEMONITOR_MINPUBLICIPFILTERWIDTH)?cfg->Filters_GetPublicIPWidth()*-1:cfg->Filters_GetPublicIPWidth());
  //cfg->Filters_SetPublicIPWidth(SplitterIPFilter->Left);

  cfg->SetIsAddTimemark(AddTimeMarkCheckBox->Checked);
	cfg->SetIsTextTracing(TextTracingCheckBox->Checked);

  cfg->SetIsShowStatusMsg(CheckBoxMsgStatus->Checked);

  cfg->Save();

  if(checkinternetconnection)
		{
			if(checkinternetconnection)
				{
 					delete checkinternetconnection;
					checkinternetconnection = NULL;
				}
		}

  WindowProc = WndProc;

  XPATHSMANAGER::DelInstance();

  CloseUDPServer();

  if(diostreamuart)
    {
      GEN_DIOFACTORY.DeleteStreamIO(diostreamuart);
      delete diostreamuartcfg;
    }


  if(DIOFACTORY::GetIsInstanced()) DIOFACTORY::DelInstance();


  if(XSLEEP::GetIsInstanced()) XSLEEP::DelInstance();

  if(XSYSTEM::GetIsInstanced()) XSYSTEM::DelInstance();

  if(XFACTORY::GetIsInstanced()) XFACTORY::DelInstance();

  origins.DeleteContents();
  origins.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ActivarMensajeria1Click(TObject*Sender)
* @brief      ActivarMensajeria1Click
* @ingroup    XUTILS
*
* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ActivarMensajeria1Click(TObject *Sender)
{
  ButtonStopClick(Sender);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::FormCanResize(TObject*Sender, int &NewWidth, int &NewHeight, bool &Resize)
* @brief      FormCanResize
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
* @param[in]  &NewWidth :
* @param[in]  &NewHeight :
* @param[in]  &Resize :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize)
{
  if(NewWidth  <= 1580) NewWidth  = 1580;
  if(NewHeight <=  132) NewHeight = 300;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonClearClick(TObject*Sender)
* @brief      ButtonClearClick
* @ingroup    XUTILS
*
* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonClearClick(TObject *Sender)
{
  ORIGIN*origin  = GetActualOrigin();
  if(!origin) return;

  origin->ritchtext->Lines->Clear();

  XSTRING namenode;
  GetNameNode(origin, namenode);

  PrintStatus(_L("Clear screen from %s."), namenode.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonStopClick(TObject*Sender)
* @brief      ButtonStopClick
* @ingroup    XUTILS
*
* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonStopClick(TObject *Sender)
{
  run=!run;

  UpdateRunStopButton(run);

  XSTRING string;

  if(run)
         string.Format(_L("Start trace lines."));
   else  string.Format(_L("Stop trace lines."));

  PrintStatus(string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonMinimizeClick(TObject*Sender)
* @brief      ButtonMinimizeClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonMinimizeClick(TObject *Sender)
{
  Application->Minimize();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonExitClick(TObject*Sender)
* @brief      ButtonExitClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonExitClick(TObject *Sender)
{
  exitapp = true;

  Close();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::Clearmessages1Click(TObject*Sender)
* @brief      Clearmessages1Click
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::Clearmessages1Click(TObject *Sender)
{
  ButtonClearClick(Sender);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::Exit1Click(TObject*Sender)
* @brief      Exit1Click
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::Exit1Click(TObject *Sender)
{
  ButtonExitClick(Sender);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::Minimizewindow1Click(TObject*Sender)
* @brief      Minimizewindow1Click
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::Minimizewindow1Click(TObject *Sender)
{
  ButtonMinimizeClick(Sender);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonCopyClick(TObject*Sender)
* @brief      ButtonCopyClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonCopyClick(TObject *Sender)
{
  ORIGIN* origin  = GetActualOrigin();
  if(!origin) return;

  origin->ritchtext->CopyToClipboard();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool __fastcall TMainForm::GenerateTab(int ntab,AnsiString& tab)
* @brief      GenerateTab
* @ingroup    XUTILS
*

* @param[in]  ntab :
* @param[in]  tab :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool __fastcall TMainForm::GenerateTab(int ntab,AnsiString& tab)
{
	tab = "";

	for(int c=0;c<ntab;c++)
		{
			tab += "  ";
		}

	return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::FilterCheckBoxClick(TObject*Sender)
* @brief      FilterCheckBoxClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::FilterCheckBoxClick(TObject *Sender)
{
  RefreshStatusTextFilter();

  cfg->Filters_SetIsTextActive(FilterCheckBox->Checked);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool __fastcall TMainForm::CheckFilterLine(AnsiString& line)
* @brief      CheckFilterLine
* @ingroup    XUTILS
*

* @param[in]  line :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool __fastcall TMainForm::CheckFilterLine(AnsiString& line)
{
  if(!FilterCheckBox->Checked)    return true;
  if(FilterEdit->Text.IsEmpty())  return true;

  int position = line.Pos(FilterEdit->Text);

  return (!position)?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonDNSUpdateClick(TObject*Sender)
* @brief      ButtonDNSUpdateClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonDNSUpdateClick(TObject *Sender)
{
  DIOURL								url;
	DIOIP*								newip;
	XSTRING								login;
	XSTRING								password;
  bool                  needchange  = false;
	DIODYNDNS_STATUSCODE	statusupdate;

  if(!diostreamudpcfg) return;

  ORIGIN* origin  = GetActualOrigin();
  if(!origin) return;

  origin->ritchtext->SelAttributes ->Color = clBlack;

  PrintStatus(_L("Resolving DNS change of %s ..."), diostreamudpcfg->GetRemoteURL()->Get());

  origin->ritchtext->Update();
  origin->valuelisteditor->Update();

	DIODYNDNS dyndns;

	url.Set(diostreamudpcfg->GetRemoteURL()->Get());

	newip = new DIOIP();
	if(!newip) return;

	dyndns.GetLogin()->Set(cfg->DynDNS_GetLogin()->Get());
	dyndns.GetPassword()->Set(cfg->DynDNS_GetPassword()->Get());

  ActivateButtons(false);

	if(dyndns.IsChangedPublicIP(url, needchange, newip, 5))
		{
      if(needchange)
        {
          XSTRING newIPstring;

          newip->GetXString(newIPstring);

    			statusupdate = dyndns.Update(url, (*newip), 5);
    			if(statusupdate == DIODYNDNS_STATUSCODE_GOOD)
            {
              PrintStatus(_L("DNS changed to Public IP: %s."), newIPstring.Get());

              ResolvedAllURLOrigins();
            }
           else
            {
              PrintStatus(_L("Failed to change the DNS."));
            }
        }
       else
        {
          PrintStatus(_L("DNS was already changed."));
        }
		}
   else
    {
      PrintStatus(_L("Error to obtain the local IP."));
    }


  ActivateButtons(true);

	delete newip;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ResolvedOriginTimerTimer(TObject*Sender)
* @brief      ResolvedOriginTimerTimer
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ResolvedOriginTimerTimer(TObject *Sender)
{
  if(!haveinternet) return;

  ResolvedAllURLOrigins();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::InternetUpdateTimerTimer(TObject*Sender)
* @brief      InternetUpdateTimerTimer
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::InternetUpdateTimerTimer(TObject *Sender)
{
  UpdateInternetStatus();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::OriginsTreeViewChange(TObject* sender, TTreeNode* node)
* @brief      OriginsTreeViewChange
* @ingroup    XUTILS
*

* @param[in]  sender :
* @param[in]  node :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::OriginsTreeViewChange(TObject* sender, TTreeNode* node)
{
  xmutexaddorigin->Lock();

  ORIGIN* selectorigin = GetOriginFromTreeNode(node);
  if(!selectorigin)
    {
      xmutexaddorigin->UnLock();
      return;
    }

  if(selectorigin->ritchtext)
    {
      selectorigin->ritchtext->Visible        = true;
      selectorigin->ritchtext->SelStart       = 0;
      selectorigin->ritchtext->Perform(EM_SCROLLCARET,0,0);
      selectorigin->ritchtext->SelStart       = selectorigin->ritchtext->GetTextLen();
      selectorigin->ritchtext->Perform(EM_SCROLLCARET,0,0);

      selectorigin->valuelisteditor->Visible  = true;

    }

  for(int c=0; c<(int)origins.GetSize(); c++)
    {
      ORIGIN* origin = origins.Get(c);
      if(origin != selectorigin)
        {
          if(origin->ritchtext)       origin->ritchtext->Visible = false;
          if(origin->valuelisteditor) origin->valuelisteditor->Visible = false;
        }
    }

  xmutexaddorigin->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterBlackClick(TObject*Sender)
* @brief      CheckLevelFilterBlackClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterBlackClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterBlack, XTRACEMONITOR_MASKLEVELBLACK);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterBlueClick(TObject*Sender)
* @brief      CheckLevelFilterBlueClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterBlueClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterBlue, XTRACEMONITOR_MASKLEVELBLUE);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterGreenClick(TObject*Sender)
* @brief      CheckLevelFilterGreenClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterGreenClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterGreen, XTRACEMONITOR_MASKLEVELGREEN);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterPurpleClick(TObject*Sender)
* @brief      CheckLevelFilterPurpleClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterPurpleClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterPurple, XTRACEMONITOR_MASKLEVELPURPLE);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterRedClick(TObject*Sender)
* @brief      CheckLevelFilterRedClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterRedClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterRed, XTRACEMONITOR_MASKLEVELRED);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckLevelFilterGrayClick(TObject*Sender)
* @brief      CheckLevelFilterGrayClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckLevelFilterGrayClick(TObject *Sender)
{
  CheckLevelFilter(CheckLevelFilterGray, XTRACEMONITOR_MASKLEVELGRAY);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::PanelLevelFilterClick(TObject*Sender)
* @brief      PanelLevelFilterClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::PanelLevelFilterClick(TObject *Sender)
{
  static bool active = false;

  if(active)
          cfg->Filters_SetLevels(0xFF);
    else  cfg->Filters_SetLevels(0);

  UpdateLevelFilterStatus();

  active = !active;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::AddTimeMarkCheckBoxClick(TObject*Sender)
* @brief      AddTimeMarkCheckBoxClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::AddTimeMarkCheckBoxClick(TObject *Sender)
{
  cfg->SetIsAddTimemark(AddTimeMarkCheckBox->Checked);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::TextTracingCheckBoxClick(TObject*Sender)
* @brief      TextTracingCheckBoxClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::TextTracingCheckBoxClick(TObject *Sender)
{
  cfg->SetIsTextTracing(TextTracingCheckBox->Checked);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ShowIPsCheckBoxClick(TObject*Sender)
* @brief      ShowIPsCheckBoxClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ShowIPsCheckBoxClick(TObject *Sender)
{
  cfg->SetIsShowIPs(ShowIPsCheckBox->Checked);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::FormActivate(TObject*Sender)
* @brief      FormActivate
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::FormActivate(TObject *Sender)
{
  Left       = cfg->GetXPos();
  Top        = cfg->GetYPos();
  Width      = cfg->GetWidth();
  Height     = cfg->GetHeight();

  if(IsWindowOutsideExtendedDesktop(MainForm->Handle))
    {
      Left    = 613;
      Top     = 349;
      Width   = 694;
      Height  = 381;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonSaveClick(TObject*Sender)
* @brief      ButtonSaveClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonSaveClick(TObject *Sender)
{
  xmutexaddDBGmessage->Lock();

  ActivateButtons(false);

  ORIGIN* origin = GetActualOrigin();
  if(origin)
    {
      if(origin->ritchtext)
        {
          XDIR* xdir = GEN_XFACTORY.Create_Dir();
          if(xdir)
            {
              XPATH    xpath;
              XSTRING  namefile;
              XSTRING  extfile;
              bool     status = false;

              extfile = _L(".rtf");

              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);

              xpath.Slash_Add();
              xpath += cfg->GetSaveDir()->Get();
              xpath.Slash_Add();

              xdir->Make(xpath);

              XSTRING namenode;
              GetNameNode(origin, namenode);

              xpath += namenode.Get();
              xpath += extfile.Get();

              XFILE* xfile = GEN_XFACTORY.Create_File();
              if(xfile)
                {
                   if(xfile->Exist(xpath))
                     {
                       if(xfile->Erase(xpath))
                        {
                          origin->ritchtext->Lines->SaveToFile(xpath.Get());
                          status = true;
                        }

                     }
                    else
                     {
                       origin->ritchtext->Lines->SaveToFile(xpath.Get());
                       status = true;
                     }

                  GEN_XFACTORY.Delete_File(xfile);
                }

              PrintStatus(_L("Save node \"%s\" in [%s] file : %s"), namenode.Get(), extfile.Get(), status?_L("Ok."):_L("Error!!!"));

              GEN_XFACTORY.Delete_Dir(xdir);
           }
        }
    }

  ActivateButtons(true);

  xmutexaddDBGmessage->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::AddDBMsgTimerTimer(TObject*Sender)
* @brief      AddDBMsgTimerTimer
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::AddDBMsgTimerTimer(TObject *Sender)
{
  AddDBGmessageManager();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::IPFilterImageClick(TObject*Sender)
* @brief      IPFilterImageClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::IPFilterImageClick(TObject *Sender)
{
  if(OriginsPanel->Width)
    {
      //SplitterIPFilter->Left = XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
      OriginsPanel->Width    = XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
      cfg->Filters_SetIsTreeVisible(false);
    }
   else
    {
      //SplitterIPFilter->Left = cfg->Filters_GetTreeWidth();
      OriginsPanel->Width    = cfg->Filters_GetTreeWidth();
      cfg->Filters_SetIsTreeVisible(true);
    }

  ServerIPComboBox->Width  = OriginsPanel->Width;

  UpdateIPFilterImage();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::SplitterIPFilterMoved(TObject*Sender)
* @brief      SplitterIPFilterMoved
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::SplitterIPFilterMoved(TObject *Sender)
{
  if(OriginsPanel->Width > XTRACEMONITOR_MAXPUBLICIPFILTERWIDTH)
    {
      //SplitterIPFilter->Left = XTRACEMONITOR_MAXPUBLICIPFILTERWIDTH;
      OriginsPanel->Width    = XTRACEMONITOR_MAXPUBLICIPFILTERWIDTH;
    }

  if(OriginsPanel->Width  < XTRACEMONITOR_MINPUBLICIPFILTERWIDTH)
    {
      //SplitterIPFilter->Left = XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
      OriginsPanel->Width    = XTRACEMONITOR_MINPUBLICIPFILTERWIDTH;
    }

  //ServerIPComboBox->Width  = OriginsPanel->Width;

  if(OriginsPanel->Width) cfg->Filters_SetTreeWidth(OriginsPanel->Width);

  UpdateIPFilterImage();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ServerIPComboBoxChange(TObject*Sender)
* @brief      ServerIPComboBoxChange
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ServerIPComboBoxChange(TObject *Sender)
{
  cfg->SetServerIPSelected(ServerIPComboBox->ItemIndex);

  if(!cfg->GetServer(ServerIPComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UDP))
    {
      return;
    }

  changeorigin = true;

  bool _run = run;

  run = false;

  OriginsPanel->Update();
  OriginsTreeView->Update();
  StatusMSGPanel->Update();
  TextPanel->Update();

  StatusMSGPanel->Hide();
  TextPanel->Hide();
  OriginsPanel->Hide();
  OriginsTreeView->Hide();


  XSTRING servercfg;
  servercfg = cfg->GetServer(ServerIPComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UDP)->config;

  ServerUDPLabel->Caption = servercfg.Get();

  CloseUDPServer();

  CreateOriginsFromConfig(Sender?false:true);

  if(haveinternet)
    {
      ResolvedAllURLOrigins();
    }

  OpenUDPServer(&servercfg);

  cfg->Save();

  OriginsPanel->Show();
  OriginsTreeView->Show();
  TextPanel->Show();
  StatusMSGPanel->Show();
 

  run = _run;

  changeorigin = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ServerUARTComboBoxChange(TObject*Sender)
* @brief      ServerUARTComboBoxChange
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ServerUARTComboBoxChange(TObject *Sender)
{
  cfg->SetServerUARTSelected(ServerUARTComboBox->ItemIndex);

  if(!cfg->GetServer(ServerUARTComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UART))
    {
      return;
    }

  changeorigin =true;

  Sleep(1000);

  XSTRING servercfg;
  servercfg  = _L("COM");
  servercfg += cfg->GetServer(ServerUARTComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UART)->config;

  ServerUARTLabel->Caption = servercfg.Get();

  CloseUARTServer();
  OpenUARTServer(&servercfg);

  DeleteOriginFromUART();
  CreateOriginFromUART();

  XSTRING nameUART;

  nameUART = cfg->GetServer(ServerUARTComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UART)->name;

  PrintStatus(_L("%s %s: %s"), nameUART.Get(), servercfg.Get(), openuart?_L("Open"):_L("Close"));

  UARTBufferStatusLabel->Enabled       = openuart;
  UARTBufferStatusProgressBar->Enabled = openuart;

  cfg->Save();

  changeorigin = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::SplitterStatusMsgMoved(TObject *Sender)
* @brief      SplitterStatusMsgMoved
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::SplitterStatusMsgMoved(TObject *Sender)
{
  if(StatusMSGPanel->Width > XTRACEMONITOR_MAXPUBLICSTATUSMSGWIDTH)
    {
      //SplitterStatusMsg->Left = SplitterIPFilter->Left + XTRACEMONITOR_MAXPUBLICSTATUSMSGWIDTH;
      StatusMSGPanel->Width      = XTRACEMONITOR_MAXPUBLICSTATUSMSGWIDTH;
    }

  if(StatusMSGPanel->Width < XTRACEMONITOR_MINPUBLICSTATUSMSGWIDTH)
    {
      //SplitterStatusMsg->Left = SplitterIPFilter->Left + XTRACEMONITOR_MINPUBLICSTATUSMSGWIDTH;
      StatusMSGPanel->Width   = XTRACEMONITOR_MINPUBLICSTATUSMSGWIDTH;
    }

  if(StatusMSGPanel->Width) cfg->Filters_SetStatusMsgWidth(StatusMSGPanel->Width);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckBoxMsgStatusClick(TObject *Sender)
* @brief      CheckBoxMsgStatusClick
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckBoxMsgStatusClick(TObject *Sender)
{
  if(CheckBoxMsgStatus->Checked)
    {
      StatusMSGPanel->Visible = true;
      SplitterStatusMsg->Visible = true;
      SplitterStatusMsg->Left = StatusMSGPanel->Left + StatusMSGPanel->Width;
    }
   else
    {
      StatusMSGPanel->Visible = false;
      SplitterStatusMsg->Visible = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ButtonClearStatusClick(TObject*Sender)
* @brief      ButtonClearClick
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ButtonClearStatusClick(TObject *Sender)
{
  ORIGIN*origin  = GetActualOrigin();
  if(!origin) return;

  origin->valuelisteditor->Strings->Clear();

  origin->status_msgs.StatusMsg_DeleteAll();

  XSTRING namenode;
  GetNameNode(origin, namenode);

  PrintStatus(_L("Clear status from %s."), namenode.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::ValueListEditorDrawCell(TObject*Sender, int ACol, int ARow, const TRect &Rect, TGridDrawState State)
* @brief      ValueListEditorDrawCell
* @ingroup    XUTILS
*

* @param[in]  TObject*Sender :
* @param[in]  ACol :
* @param[in]  ARow :
* @param[in]  TRect &Rect :
* @param[in]  State :
*
* @return     void : does not return anything.
*
* ---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::ValueListEditorDrawCell(TObject *Sender, int ACol, int ARow, const TRect &Rect, TGridDrawState State)
{
  TValueListEditor*   valuelisteditor = (TValueListEditor*)Sender;
  ORIGIN*             select_origin   = NULL;
  XTRACE_STATUS_MSG*  status_msg      = NULL;
  bool                changenow       = false;


  select_origin   =  GetActualOrigin();
  if(!select_origin) return;

  if(ARow >= 1)
    {
      status_msg = select_origin->status_msgs.StatusMsg_Get(ARow-1);
    }

  if(!status_msg) return;

  changenow = (status_msg->GetXTimerLastUpdate()->GetMeasureSeconds()<=(XQWORD)cfg->GetStatusMsgRefresh())?true:false;

  if(!ACol &&  ARow >= 1)
    {
      valuelisteditor->Canvas->Brush->Color   = clWhite;
      valuelisteditor->Canvas->FillRect(Rect);

      valuelisteditor->Canvas->Brush->Color   = clWhite;
      valuelisteditor->Canvas->Font->Color    = (changenow?clRed:clBlack);

      valuelisteditor->Canvas->TextOut(Rect.Left + 1, Rect.Top, valuelisteditor->Cells[ACol][ARow]);
    }

  if(ACol &&  ARow >= 1)
    {
      TRect rect = Rect;

      valuelisteditor->Canvas->Brush->Color   = clWhite;
      valuelisteditor->Canvas->FillRect(Rect);

      if(status_msg->GetType() == XTRACE_TYPE_STATUS_MSG_RGBCOLOR)
        {
          rect.Right = rect.Left + 30;

          valuelisteditor->Canvas->Brush->Color   = (TColor)RGB(status_msg->Value_GetColor()[0], status_msg->Value_GetColor()[1], status_msg->Value_GetColor()[2]);
          valuelisteditor->Canvas->FillRect(rect);

          valuelisteditor->Canvas->Brush->Color   = clWhite;
          valuelisteditor->Canvas->Font->Color    = (changenow?clRed:clBlack);

          valuelisteditor->Canvas->TextOut(Rect.Left + 1 + 34, Rect.Top + 1, valuelisteditor->Cells[ACol][ARow]);
        }
       else
        {
          valuelisteditor->Canvas->Brush->Color   = clWhite;
          valuelisteditor->Canvas->Font->Color    = (changenow?clRed:clBlack);

          valuelisteditor->Canvas->TextOut(Rect.Left + 1, Rect.Top + 1, valuelisteditor->Cells[ACol][ARow]);
        }

    }

}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::TimerStatusMSGTimer(TObject *Sender)
* @brief      TimerStatusMSGTimer
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::TimerStatusMSGTimer(TObject *Sender)
{
   ORIGIN* origin = GetActualOrigin();

   if(origin) RedrawStatusMsgList(origin);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::AddDBGmessageManager()
* @brief      AddDBGmessageManager
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::AddDBGmessageManager()
{
  xmutexaddDBGmessage->Lock();

  if(openudp)
    {
      int sizebuffer =  (diostreamudp->GetInXBuffer()->GetSize()/1024);

      UDPBufferStatusProgressBar->Position = ((sizebuffer*100)/XTRACEMONITOR_MAXSIZEINKUDPBUFFER);

      if(sizebuffer >= XTRACEMONITOR_MAXSIZEINKUDPBUFFER)
        {
          diostreamudp->GetInXBuffer()->Delete();
          PrintStatus(_L("Buffer UDP messages overlapped: Buffers deleted! "));

          DBGmessages.DeleteContents();
          DBGmessages.DeleteAll();
        }
    }

  if(openuart)
    {
      int sizebuffer =  (diostreamuart->GetInXBuffer()->GetSize()/1024);

      UARTBufferStatusProgressBar->Position = ((sizebuffer*100)/XTRACEMONITOR_MAXSIZEINKUDPBUFFER);

      if(sizebuffer >= XTRACEMONITOR_MAXSIZEINKUARTBUFFER)
        {
          diostreamuart->GetInXBuffer()->Delete();
          PrintStatus(_L("Buffer UART messages overlapped: Buffers deleted! "));

          DBGmessages.DeleteContents();
          DBGmessages.DeleteAll();
        }
    }

  for(XDWORD c=0; c<DBGmessages.GetSize(); c++)
    {
      DBGMESSAGE* DBGmessage = DBGmessages.Get(c);
      if(!DBGmessage) break;

      if(run)
        {
          ORIGIN* origin;

          if((DBGmessage->publicIP == 0xFFFFFFFF) && (DBGmessage->localIP == 0xFFFFFFFF))
            {
              origin = UARTorigin;
              if(origin) AddLineTrace(origin, DBGmessage);
            }
           else
            {
              origin = GetOriginFromIP(0, 0);
              if(origin) AddLineTrace(origin, DBGmessage);

              if(DBGmessage->publicIP && DBGmessage->localIP)
                {
                  origin = GetOriginFromIP(DBGmessage->publicIP, DBGmessage->localIP);
                  if(!origin)
                    {
    				    			xmutexaddorigin->Lock();

                      origin = GetOriginFromIP(DBGmessage->publicIP, 0);
                      if(!origin)
                        {
                          origin = CreateOriginsFromIP(DBGmessage->publicIP, DBGmessage->publicIPnopacket, 0);
                          if(origin)
                            {
                              OriginsTreeView->AlphaSort();
                              origins.Add(origin);
                            }
                        }

                      origin = CreateOriginsFromIP(DBGmessage->publicIP, DBGmessage->publicIPnopacket, DBGmessage->localIP);
                      if(origin)
                        {
                          OriginsTreeView->AlphaSort();
                          origins.Add(origin);
                        }

                      xmutexaddorigin->UnLock();

                      origin = GetOriginFromIP(DBGmessage->publicIP, DBGmessage->localIP);
                      if(origin) AddLineTrace(origin, DBGmessage);

                    } else AddLineTrace(origin, DBGmessage);
                }

              if(DBGmessage->publicIP)
                {
                  origin = GetOriginFromIP(DBGmessage->publicIP, 0);
                  if(origin) AddLineTrace(origin, DBGmessage);
                }

              if(!DBGmessage->publicIP && DBGmessage->localIP)
                {
                  AddLineTrace(localnointernetorigin, DBGmessage);

                  origin = GetOriginFromIP(DBGmessage->publicIP, DBGmessage->localIP);
                  if(!origin)
                    {
    				    			xmutexaddorigin->Lock();

                      origin = CreateOriginsFromIP(DBGmessage->publicIP, DBGmessage->publicIPnopacket, DBGmessage->localIP);
                      if(origin)
                        {
                          OriginsTreeView->AlphaSort();
                          origins.Add(origin);

                          AddLineTrace(origin, DBGmessage);
                        }

                      xmutexaddorigin->UnLock();

                    } else AddLineTrace(origin, DBGmessage);
                }

              if(!DBGmessage->publicIP && !DBGmessage->localIP)
                {
                  XSTRING string;

                  string.Format(_L("Origin of the message Unknown!"));
                  PrintStatus(string.Get());
               }
            }
        }
    }

  DBGmessages.DeleteContents();
  DBGmessages.DeleteAll();

  xmutexaddDBGmessage->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         __fastcall TMainForm::TMainForm(TComponent* Owner)
* @brief      TMainForm
* @ingroup    XUTILS
*

* @param[in]  Owner :
*
* @return     __fastcall :
*
*---------------------------------------------------------------------------------------------------------------------*/
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
  Clean();

  run = true;

  UpdateRunStopButton(run);

  ButtonStop->Top       = ButtonRun->Top;
  ButtonStop->Left      = ButtonRun->Left;

  VersionLabel->Caption = XTRACEMONITOR_VERSIONLABEL;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void __fastcall TMainForm::CheckCopyData(Messages::TMessage &Message)
* @brief      CheckCopyData
* @ingroup    XUTILS
*

* @param[in]  &Message :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void __fastcall TMainForm::CheckCopyData(Messages::TMessage &Message)
{
  if((Message.Msg == WM_COPYDATA)&&(run))
    {
      COPYDATASTRUCT* cs = (COPYDATASTRUCT*)Message.LParam;
      XBUFFER         data;

      data.Add((XBYTE*)cs->lpData, cs->cbData);

      XDWORD     publicIP   = 0;
      XDWORD     localIP    = 0;
      XBYTE      level      = 0;
      XDWORD     sequence   = 0;
      XDATETIME  xtime;
      XSTRING    string;

      xtime.SetToZero();
      string.Empty();

      XDWORD error = XTRACE::instance->GetTraceFromXBuffer(data, publicIP, localIP, level, sequence, &xtime, data);
      if(!error)
        {
          XTRACE::instance->SetTraceDataToText(data, string);

          AddDBGMessage(publicIP, false, localIP, level, sequence, xtime, string);
        }
       else
        {
          switch(error)
           {
             case 1  : break;
             default : { XSTRING string;

                         string.Format(_L("Message lost by packet error [code %d]."), error);
                         PrintStatus(string.Get());
                       }
                       break;
           }
        }
    }
   else WndProc(Message);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::UpdateInternetStatus()
* @brief      UpdateInternetStatus
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::UpdateInternetStatus()
{
  if(!checkinternetconnection) return false;

  InternetLabel->Caption = "Check Internet";

  InternetImageList->GetBitmap(1,InternetImage->Picture->Bitmap);
  InternetImage->Repaint();

  if(checkinternetconnection)
    {
      haveinternet = checkinternetconnection->Check();
    }

  InternetImageList->GetBitmap(haveinternet?2:0,InternetImage->Picture->Bitmap);
  InternetImage->Repaint();

  if(haveinternet) InternetLabel->Caption = "ON";  else InternetLabel->Caption = "OFF";

  //PrintStatus(_L("Check Internet status: %s "), (haveinternet)?_L("Connected"):_L("Disconnected"));

  return haveinternet;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::ActivateButtons(bool active)
* @brief      ActivateButtons
* @ingroup    XUTILS
*

* @param[in]  active :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::ActivateButtons(bool active)
{
  ButtonClear->Enabled      = active;
  ButtonStop->Enabled       = active;
  ButtonExit->Enabled       = active;
  ButtonMinimize->Enabled   = active;
  ButtonRun->Enabled        = active;
  ButtonCopy->Enabled       = active;
  if(cfg->DynDNS_IsChangeAvailable())   ButtonDNSUpdate->Enabled  = active;
  ButtonSave->Enabled       = active;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::RefreshStatusTextFilter()
* @brief      RefreshStatusTextFilter
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::RefreshStatusTextFilter()
{
  XSTRING string;
  XSTRING filter = FilterEdit->Text.c_str();

  if(FilterCheckBox->Checked)
        string.Format(_L("Activated filter with \"%s\"."), filter.Get());
   else string.Format(_L("Deactivated filter.") );

  PrintStatus(string.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TRichEdit* TMainForm::CreateRitchText()
* @brief      CreateRitchText
* @ingroup    XUTILS
*

* @return     TRichEdit* :
*
*---------------------------------------------------------------------------------------------------------------------*/
TRichEdit* TMainForm::CreateRitchText()
{
  TRichEdit* ritchtext = new TRichEdit(TextPanel);
  if(!ritchtext) return NULL;

  ritchtext->Visible        = false;

  ritchtext->Align          = alClient;
  ritchtext->Parent         = TextPanel;
  ritchtext->BorderStyle    = bsNone;
  ritchtext->BevelInner     = bvNone;
  ritchtext->BevelOuter     = bvNone;
  ritchtext->Ctl3D          = false;

  ritchtext->ScrollBars     = ssBoth;
  ritchtext->ReadOnly       = true;
  ritchtext->WordWrap       = false;

  ritchtext->Font->Name     = _L("Courier New");
  ritchtext->Font->Height   = -11;

  ritchtext->HideSelection  = true;

  ritchtext->MaxLength      = ((cfg->GetMaxTextSize()+4)*1204);

  return ritchtext;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         TValueListEditor* TMainForm::CreateValueListEditor()
* @brief      CreateValueListEditor
* @ingroup    XUTILS
*

* @return     TValueListEditor* :
*
*---------------------------------------------------------------------------------------------------------------------*/
TValueListEditor* TMainForm::CreateValueListEditor()
{
  TValueListEditor* valuelisteditor = new TValueListEditor(StatusMSGPanel);
  if(!valuelisteditor) return NULL;

  TGridOptions options;

  options = TGridOptions() << goVertLine << goHorzLine << goFixedVertLine << goFixedHorzLine << goColSizing; // << goEditing;

  valuelisteditor->OnDrawCell     = ValueListEditorDrawCell;

  valuelisteditor->Visible        = false;

  valuelisteditor->Options        = options;

  valuelisteditor->Align          = alClient;
  valuelisteditor->Parent         = StatusMSGPanel;

  valuelisteditor->BorderStyle    = bsNone;
  //valuelisteditor->BevelOuter     = bvNone;
  valuelisteditor->Ctl3D          = false;

  valuelisteditor->TitleCaptions->Clear();
  valuelisteditor->TitleCaptions->Add(_L("Name"));
  valuelisteditor->TitleCaptions->Add(_L("Value"));

  valuelisteditor->ScrollBars     = ssBoth;

  valuelisteditor->Font->Name     = _L("Courier New");
  valuelisteditor->Font->Height   = -11;

  return valuelisteditor;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ORIGIN* TMainForm::CreateOrigin(ORIGIN* father, XTRACEMONITOR_ORIGINCFG* origincfg)
* @brief      CreateOrigin
* @ingroup    XUTILS
*

* @param[in]  father :
* @param[in]  origincfg :
*
* @return     ORIGIN* :
*
*---------------------------------------------------------------------------------------------------------------------*/
ORIGIN* TMainForm::CreateOrigin(ORIGIN* father, XTRACEMONITOR_ORIGINCFG* origincfg)
{
  if(!origincfg)                  return NULL;
  if(!origincfg->isactive)        return NULL;
  //if(!origincfg->name.GetSize())  return NULL;

  XSTRING   string;
  XSTRING   string2;
  ORIGIN*   origin;
  XSTRING   defaultURLstring;

  origin = new ORIGIN();
  if(!origin) return NULL;

  if(origincfg->URL.GetSize() && haveinternet)
    {
      DIOURL URL;
      URL.Set(origincfg->URL);
    }

  GenerateIPString(0 , origin->publicIP);
  GenerateIPString(0 , origin->localIP);

  origin->showip    = origincfg->showip;
  origin->name      = origincfg->name;
  origin->URL       = origincfg->URL;


  string = origin->name.Get();

  if(origin->showip)
    {
      string2.Format(_L(" [%s]"), origincfg->IP.Get());
      if(origincfg->IP.GetSize()) string += string2.Get();
    }

  if(origin->URL.GetSize())
    {
      if(origincfg->IP.GetSize()) origin->publicIP  = origincfg->IP;
    }
   else
    {
       if(origincfg->IP.GetSize()) origin->localIP  = origincfg->IP;
    }

  if(father)
        origin->node = OriginsTreeView->Items->AddChild(father->node, string.Get());
   else origin->node = OriginsTreeView->Items->Add(NULL, string.Get());

  if(origin->node)
    {
      origin->ritchtext       = CreateRitchText();
      origin->valuelisteditor = CreateValueListEditor();
    }

  return origin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::CreateOriginsFromConfig()
* @brief      CreateOriginsFromConfig
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::CreateOriginsFromConfig(bool addUART)
{
  bool status = true;

  if(xmutexaddorigin) xmutexaddorigin->Lock();

  OriginsTreeView->RowSelect      = false;
  OriginsTreeView->HideSelection  = false;

  if(OriginsTreeView->Items->GetFirstNode())  OriginsTreeView->Items->Delete(OriginsTreeView->Items->GetFirstNode());

  XTRACEMONITOR_ORIGINCFG rootorigincfg;

  rootorigincfg.isactive = true;
  rootorigincfg.showip   = false;
  rootorigincfg.name     = cfg->GetRootOriginName()->Get();
  rootorigincfg.URL      = _L("");
  rootorigincfg.IP       = _L("");


  XTRACEMONITOR_ORIGINCFG localnointernetorigincfg;

  localnointernetorigincfg.isactive = true;
  localnointernetorigincfg.showip   = false;
  localnointernetorigincfg.name     = _L("# ");
  localnointernetorigincfg.name    += cfg->GetLocalNoInternetOriginName()->Get();
  localnointernetorigincfg.URL      = _L("");
  localnointernetorigincfg.IP       = _L("");

  int c=0;
  while(c<(int)origins.GetSize())
    {
      ORIGIN* origin = origins.Get(c);
      if(origin)
        {
          if(!origin->isuart)
            {
              origins.Delete(origin);
              delete origin;

            } else c++;

        } else break;
    }

  rootorigin = CreateOrigin(NULL, &rootorigincfg);
  if(rootorigin)
    {
      origins.Add(rootorigin);

      localnointernetorigin = CreateOrigin(rootorigin, &localnointernetorigincfg);
      if(localnointernetorigin)
        {
          origins.Add(localnointernetorigin);
        }

      for(int c=0; c<XTRACEMONITOR_CFG_MAXORIGINS; c++)
        {
          XTRACEMONITOR_ORIGINCFG* origincfg = cfg->GetOrigin(c);
          if(origincfg)
            {
              if(origincfg->isactive)
                {
                  ORIGIN* origin;

                  origin = CreateOrigin(rootorigin, origincfg);
                  if(origin) origins.Add(origin);
                }
            }
        }

      if(addUART) CreateOriginFromUART();

      rootorigin->node->Expand(true);
    }

 	cfg->Save();
  if(rootorigin->node)
    {
      OriginsTreeView->Selected =  rootorigin->node;
      rootorigin->node->Focused = true;
    }

  OriginsTreeView->AlphaSort();

  noreadudp = false;

  if(xmutexaddorigin) xmutexaddorigin->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ORIGIN* TMainForm::CreateOriginsFromIP(XDWORD publicIP, bool publicIPnopacket, XDWORD localIP)
* @brief      CreateOriginsFromIP
* @ingroup    XUTILS
*

* @param[in]  publicIP :
* @param[in]  publicIPnopacket :
* @param[in]  localIP :
*
* @return     ORIGIN* :
*
*---------------------------------------------------------------------------------------------------------------------*/
ORIGIN* TMainForm::CreateOriginsFromIP(XDWORD publicIP, bool publicIPnopacket, XDWORD localIP)
{
  XTRACEMONITOR_ORIGINCFG nodeorigincfg;
  ORIGIN*                 fatherorigin  = NULL;
  ORIGIN*                 origin;

  if(!publicIP && !localIP)
    {
      fatherorigin = rootorigin;
    }
   else
    {
      if(!publicIP && localIP) fatherorigin = localnointernetorigin;
    }

  if(!fatherorigin) fatherorigin= GetOriginFromIP(publicIP, 0);
  if(!fatherorigin)
    {
      fatherorigin = rootorigin;

      XSTRING publicIPstring;
      XSTRING localIPstring;

      GenerateIPString(publicIP  , publicIPstring);
      GenerateIPString(localIP   , localIPstring);

      nodeorigincfg.isactive = true;
      nodeorigincfg.showip   = true;
      
      if(publicIPnopacket)
            nodeorigincfg.name     = XTRACEMONITOR_NODEORIGINDEFAULTNAMENOTPACKET;
       else nodeorigincfg.name     = XTRACEMONITOR_NODEORIGINDEFAULTNAME;

      nodeorigincfg.URL      = _L("");
      nodeorigincfg.IP       = publicIPstring.Get();

      origin = CreateOrigin(fatherorigin, &nodeorigincfg);
      if(origin)
        {
          XSTRING publicIPstring;
          GenerateIPString(publicIP , publicIPstring);

          origin->publicIP  = publicIPstring.Get();
          origin->localIP   = localIPstring.Get();
          origin->isuart    = false;

          origins.Add(origin);
        }
    }
   else
    {
      XSTRING localIPstring;

      GenerateIPString(localIP  , localIPstring);

      nodeorigincfg.isactive = true;
      nodeorigincfg.showip   = true;

      if(localIPstring.Compare(_L("1.1.1.1"), true))
        {
          nodeorigincfg.name     = _L("");
          nodeorigincfg.URL      = _L("");
          nodeorigincfg.IP       = localIPstring.Get();
        }
       else
        {
          nodeorigincfg.name     = cfg->GetWindowsMessageOriginName()->Get();
          nodeorigincfg.URL      = _L("");
          nodeorigincfg.IP       = _L("");
        }

      origin = CreateOrigin(fatherorigin, &nodeorigincfg);
      if(origin)
        {
          XSTRING publicIPstring;
          GenerateIPString(publicIP , publicIPstring);

          origin->publicIP  = publicIPstring.Get();
          origin->localIP   = localIPstring.Get();
          origin->isuart    = false;

          origins.Add(rootorigin);
        }
    }

  return origin;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::CreateOriginsFromUART()
* @brief      CreateOriginsFromUART
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::CreateOriginFromUART()
{
  XTRACEMONITOR_ORIGINCFG UARTorigincfg;
  XSTRING                 servername;

  if(cfg->GetServer(ServerUARTComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UART))
        servername = cfg->GetServer(ServerUARTComboBox->ItemIndex, XTRACEMONITOR_CFG_TYPESERVER_UART)->name;
   else servername = cfg->GetUARTOriginName()->Get();

  UARTorigincfg.isactive = true;
  UARTorigincfg.showip   = false;
  UARTorigincfg.name     = servername.Get();
  UARTorigincfg.URL      = servername.Get();
  UARTorigincfg.IP       = _L("255.255.255.255");

  UARTorigin = CreateOrigin(NULL, &UARTorigincfg);
  if(UARTorigin)
    {
      UARTorigin->publicIP = _L("255.255.255.255");
      UARTorigin->localIP  = _L("255.255.255.255");
      UARTorigin->isuart   = true;

      origins.Add(UARTorigin);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::DeleteOriginFromUART()
* @brief      DeleteOriginFromUART
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::DeleteOriginFromUART()
{
  ORIGIN* uartorigin = GetOriginFromIP(0xffffffff, 0xffffffff);
  if(uartorigin)
    {
      OriginsTreeView->Items->Delete(uartorigin->node);
      origins.Delete(uartorigin);

      delete uartorigin;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ORIGIN* TMainForm::GetOriginFromIP(XDWORD publicIP, XDWORD localIP)
* @brief      GetOriginFromIP
* @ingroup    XUTILS
*

* @param[in]  publicIP :
* @param[in]  localIP :
*
* @return     ORIGIN* :
*
*---------------------------------------------------------------------------------------------------------------------*/
ORIGIN* TMainForm::GetOriginFromIP(XDWORD publicIP, XDWORD localIP)
{
  XSTRING publicIPstring;
  XSTRING localIPstring;

  GenerateIPString(publicIP , publicIPstring);
  GenerateIPString(localIP  , localIPstring);

  for(int c=0; c<(int)origins.GetSize(); c++)
    {
      ORIGIN* origin = origins.Get(c);
      if(origin)
        {
          if((!origin->publicIP.Compare(publicIPstring)) && (!origin->localIP.Compare(localIPstring)))
            {
              return origin;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ORIGIN* TMainForm::GetOriginFromTreeNode(TTreeNode* node)
* @brief      GetOriginFromTreeNode
* @ingroup    XUTILS
*

* @param[in]  node :
*
* @return     ORIGIN* :
*
*---------------------------------------------------------------------------------------------------------------------*/
ORIGIN* TMainForm::GetOriginFromTreeNode(TTreeNode* node)
{
  if(!node) return NULL;

  for(int c=0; c<(int)origins.GetSize(); c++)
    {
      ORIGIN* origin = origins.Get(c);
      if(origin)
        {
          if(origin->node == node)
            {
              return origin;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         ORIGIN* TMainForm::GetActualOrigin()
* @brief      GetActualOrigin
* @ingroup    XUTILS
*

* @return     ORIGIN* :
*
*---------------------------------------------------------------------------------------------------------------------*/
ORIGIN* TMainForm::GetActualOrigin()
{
  if(!OriginsTreeView)            return NULL;
  if(!OriginsTreeView->Selected)  return NULL;

  return GetOriginFromTreeNode(OriginsTreeView->Selected);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::ResolvedAllURLOrigins()
* @brief      ResolvedAllURLOrigins
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::ResolvedAllURLOrigins()
{
  if(!haveinternet) return;

  ActivateButtons(false);

  if(xmutexaddorigin)     xmutexaddorigin->Lock();
  if(xmutexaddDBGmessage) xmutexaddDBGmessage->Lock();

  //PrintStatus(_L("Resolve URL from nodes ...   "));

  for(int c=0; c<XTRACEMONITOR_CFG_MAXORIGINS; c++)
    {
      ORIGIN* origin = origins.Get(c);
      if(origin)
        {
          XSTRING string;
          XSTRING string2;

          if(!origin->isuart)
            {
              if(origin->URL.GetSize())
                {
                  if(origin->URL.Compare(_L("UART")))
                    {
                      DIOURL URL;

                      URL.Set(origin->URL);
                      URL.ResolveURL(origin->publicIP);

                      string = origin->name.Get();

                      if(origin->showip)
                        {
                          string2.Format(_L(" [%s]"), origin->publicIP.Get());
                          string += string2.Get();
                        }

                      origin->node->Text = string.Get();
                   }
                }
            }
        }
    }

  OriginsTreeView->AlphaSort();

  if(xmutexaddDBGmessage) xmutexaddDBGmessage->UnLock();
  if(xmutexaddorigin) xmutexaddorigin->UnLock();

  ActivateButtons(true);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::AddLineTrace(ORIGIN* origin, DBGMESSAGE* DBGmessage)
* @brief      AddLineTrace
* @ingroup    XUTILS
*

* @param[in]  origin :
* @param[in]  DBGmessage :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::AddLineTrace(ORIGIN* origin, DBGMESSAGE* DBGmessage)
{
  if(!origin)     return false;
  if(!DBGmessage) return false;
  if(!xdatetime)  return false;

  XSTRING     line;
  TColor      color                 = clBlack;
  XSTRING     datetimestring;
  int         maxsize               = ((cfg->GetMaxTextSize())*1024);
  int         sizetext              =  origin->ritchtext->GetTextLen();
  int         indexcmdscreenclear   = DBGmessage->string.Find(XTRACE_IDMSGCLEARSCREEN  , false);
  int         indexcmdchangestatus  = DBGmessage->string.Find(XTRACE_IDMSGSTATUS       , false);
  int         indexcmdclearstatus   = DBGmessage->string.Find(XTRACE_IDMSGSTATUSCLEAR  , false);
  XSTRING     publicIP;
  XSTRING     localIP;
  bool        eraseorigin           = false;
  bool        erasemsgstatus        = false;

  if(indexcmdscreenclear != XSTRING_NOTFOUND)
    {
      XSTRING namenode;
      GetNameNode(origin, namenode);

      if(!DBGmessage->level)
        {
          if(namenode.Find(_L("_"), true) != XSTRING_NOTFOUND)
            {
              eraseorigin   = true;
            }
        }
       else
        {
          eraseorigin   = true;
        }
    }

  if(sizetext >= maxsize)
    {
      eraseorigin = true;
    }

  if(eraseorigin)
    {
      bool visible = origin->ritchtext->Visible;

      origin->ritchtext->Visible = false;
      origin->ritchtext->Lines->Clear();
      origin->ritchtext->Visible = visible;

      XSTRING line;
      XSTRING namenode;
      GetNameNode(origin, namenode);

      line.Format(_L("Clear screen from %s"), namenode.Get());

      if(sizetext >= maxsize)                     line.AddFormat(_L(" for overlapped size %dk > %dk"), (sizetext/1024), (maxsize/1024));
      if(indexcmdscreenclear != XSTRING_NOTFOUND) line.AddFormat(_L(" for command clear screen"));

      PrintStatus(line.Get());
    }

  if(indexcmdclearstatus != XSTRING_NOTFOUND)
    {
      XSTRING namenode;
      GetNameNode(origin, namenode);

      if(!DBGmessage->level)
        {
          if(namenode.Find(_L("_"), true) != XSTRING_NOTFOUND)
            {
              erasemsgstatus   = true;
            }
        }
       else
        {
          erasemsgstatus   = true;
        }
    }

  if(erasemsgstatus)
    {
      XSTRING line;
      XSTRING namenode;

      GetNameNode(origin, namenode);

      line.Format(_L("Clear Msg status from %s for command"), namenode.Get());

      origin->valuelisteditor->Strings->Clear();
      origin->status_msgs.StatusMsg_DeleteAll();
    }


  if(cfg->IsShowIPs())
    {
      XSTRING publicIP;
      XSTRING localIP;

      publicIP.Format(_L("[%03d.%03d.%03d.%03d]"), (DBGmessage->publicIP&0xFF000000)>>24
                                                  , (DBGmessage->publicIP&0x00FF0000)>>16
                                                  , (DBGmessage->publicIP&0x0000FF00)>>8
                                                  , (DBGmessage->publicIP&0x000000FF));

      localIP.Format(_L("[%03d.%03d.%03d.%03d]") , (DBGmessage->localIP&0xFF000000)>>24
                                                  , (DBGmessage->localIP&0x00FF0000)>>16
                                                  , (DBGmessage->localIP&0x0000FF00)>>8
                                                  , (DBGmessage->localIP&0x000000FF));

      if(origin == rootorigin)
        {
          line += publicIP.Get();
          line += localIP.Get();
        }
       else
        {
          if(origin->node)
            {
              if(origin->node->Parent == rootorigin->node)  line += localIP.Get();
            }
        }
    }


  if(cfg->IsAddTimemark())
    {
      xdatetime->Read();

      xdatetime->GetDateTimeToString((XDATETIME_FORMAT_STANDARD | XDATETIME_FORMAT_TIMEWITHMILLISECONDS), datetimestring);

      line += _L("[");
      line +=  datetimestring.Get();
      line += _L("] ");
    }

  line += DBGmessage->string.Get();

  if(DBGmessage->level)
	  {
    	if((DBGmessage->level & XTRACE_LEVEL_WITHCOLOR) == XTRACE_LEVEL_WITHCOLOR)
		 		{
          if(!IsActiveLevelFilter(DBGmessage->level & 0x0f)) return false;

          switch(DBGmessage->level & 0x0f)
           {
             case 0 : color = clBlack;   break;
             case 1 : color = clBlue;    break;
             case 2 : color = clGreen;   break;
             case 3 : color = clPurple;  break;
             case 4 : color = clRed;     break;
             case 5 : color = clGray;    break;
           }
  			}
   	}

  if(origin)
    {
      if(origin->ritchtext)
        {
          origin->ritchtext->SelAttributes->Color = color;

          XTRACE_STATUS_MSG* status_msg = origin->status_msgs.StatusMsg_Add(&line);
          if(status_msg)
            {
              status_msg->GetXTimerLastUpdate()->Reset();
              RedrawStatusMsgList(origin);

              if(!cfg->IsShowCommandMsg()) return true;
            }

          if((indexcmdscreenclear != XSTRING_NOTFOUND) || (indexcmdchangestatus != XSTRING_NOTFOUND) || (indexcmdclearstatus  != XSTRING_NOTFOUND))
            {
              if(!cfg->IsShowCommandMsg())
                {
                  return true;
                }
            }

          AnsiString  line_builder;

          line_builder += line.Get();

          if(CheckFilterLine(line_builder))
            {
              origin->ritchtext->Lines->Add(line_builder);

              if(cfg->IsTextTracing())
                {
                  origin->ritchtext->SelStart = origin->ritchtext->GetTextLen();
                  origin->ritchtext->Perform(EM_SCROLLCARET,0,0);
                }
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::UpdateRunStopButton(bool run)
* @brief      UpdateRunStopButton
* @ingroup    XUTILS
*

* @param[in]  run :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::UpdateRunStopButton(bool run)
{
  ButtonRun->Visible = !run;
  ButtonStop->Visible = run;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::AddDBGMessage(XDWORD publicIP, bool publicIPnopacket, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& string)
* @brief      AddDBGMessage
* @ingroup    XUTILS
*

* @param[in]  publicIP :
* @param[in]  localIP :
* @param[in]  level :
* @param[in]  sequence :
* @param[in]  xtime :
* @param[in]  string :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::AddDBGMessage(XDWORD publicIP, bool publicIPnopacket, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& string)
{
  DBGMESSAGE* DBGmessage = new DBGMESSAGE();
  if(!DBGmessage) return false;

  xmutexaddDBGmessage->Lock();

  DBGmessage->publicIP          = publicIP;
  DBGmessage->publicIPnopacket  = publicIPnopacket;
  DBGmessage->localIP           = localIP;
  DBGmessage->level             = level;
  DBGmessage->sequence          = sequence;
  DBGmessage->xtime             = xtime;
  DBGmessage->string            = string;

  DBGmessages.Add(DBGmessage);

  xmutexaddDBGmessage->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::GenerateIPString(XDWORD IP, XSTRING& IPstring)
* @brief      GenerateIPString
* @ingroup    XUTILS
*

* @param[in]  IP :
* @param[in]  IPstring :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::GenerateIPString(XDWORD IP, XSTRING& IPstring)
{
  IPstring.Format(_L("%d.%d.%d.%d"), (IP&0xFF000000)>>24, (IP&0x00FF0000)>>16, (IP&0x0000FF00)>>8, (IP&0x000000FF));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::UpdateLevelFilterStatus()
* @brief      UpdateLevelFilterStatus
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::UpdateLevelFilterStatus()
{
  CheckLevelFilterBlack->Checked   = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELBLACK) ? true:false;
  CheckLevelFilterBlue->Checked    = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELBLUE)  ? true:false;
  CheckLevelFilterGreen->Checked   = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELGREEN) ? true:false;
  CheckLevelFilterPurple->Checked  = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELPURPLE)? true:false;
  CheckLevelFilterRed->Checked     = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELRED)   ? true:false;
  CheckLevelFilterGray->Checked    = (cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELGRAY)  ? true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::CheckLevelFilter(TCheckBox* checkbox, XDWORD mask)
* @brief      CheckLevelFilter
* @ingroup    XUTILS
*

* @param[in]  checkbox :
* @param[in]  mask :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::CheckLevelFilter(TCheckBox* checkbox, XDWORD mask)
{
  XDWORD levelfilter = cfg->Filters_GetLevels();

  if(checkbox->Checked)
          levelfilter |=  mask;
     else levelfilter &= ~mask;

  cfg->Filters_SetLevels(levelfilter);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::IsActiveLevelFilter(int level)
* @brief      IsActiveLevelFilter
* @ingroup    XUTILS
*

* @param[in]  level :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::IsActiveLevelFilter(int level)
{
  switch(level)
    {
      case 0 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELBLACK)   return true;
                break;

      case 1 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELBLUE)    return true;
                break;

      case 2 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELGREEN)   return true;
                break;

      case 3 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELPURPLE)  return true;
                break;

      case 4 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELRED)     return true;
                break;

      case 5 :  if(cfg->Filters_GetLevels() & XTRACEMONITOR_MASKLEVELGRAY)    return true;
                break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::UpdateIPFilterImage()
* @brief      UpdateIPFilterImage
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::UpdateIPFilterImage()
{
  if(OriginsPanel->Width)
    {
      IPFilterImageList->GetBitmap(0, IPFilterImage->Picture->Bitmap);
      IPFilterImage->Repaint();
    }
   else
    {
      IPFilterImageList->GetBitmap(1, IPFilterImage->Picture->Bitmap);
      IPFilterImage->Repaint();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::PrintStatus(XCHAR* mask, ...)
* @brief      PrintStatus
* @ingroup    XUTILS
*

* @param[in]  mask :
* @param[in]  ... :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::PrintStatus(XCHAR* mask, ...)
{
  XSTRING   string;
  va_list   arg;

	va_start(arg, mask);

	string.FormatArg(mask, &arg);

	va_end(arg);

  int  sizetext = StatusRichEdit->GetTextLen();

  if(sizetext >= (5*1024)) StatusRichEdit->Lines->Clear();

  StatusRichEdit->Lines->Add(string.Get());

  StatusRichEdit->SelStart = StatusRichEdit->GetTextLen();
  StatusRichEdit->Perform(EM_SCROLLCARET,0,0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::GetNameNode(ORIGIN* origin, XSTRING& namenode)
* @brief      GetNameNode
* @ingroup    XUTILS
*

* @param[in]  origin :
* @param[in]  namenode :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::GetNameNode(ORIGIN* origin, XSTRING& namenode)
{
  namenode.Empty();

  namenode += origin->name;

  if(origin != rootorigin)
    {
      if(origin->node->Parent != rootorigin->node)
        {
          ORIGIN* originfather = GetOriginFromTreeNode(origin->node->Parent);

          namenode.Empty();

          if(originfather) namenode = originfather->name.Get();

          namenode += _L("_");
          namenode += origin->publicIP.Get();

          if(origin->localIP.Compare(_L("255.255.255.255")))
            {
              namenode += _L("_#_");
              namenode += origin->localIP.Get();
            }
       }
      else
       {
         if(origin->localIP.Compare(_L("255.255.255.255")) && origin->localIP.Compare(_L("0.0.0.0")))
           {
             namenode += _L("_");
             namenode += origin->publicIP.Get();
           }
       }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::InitServerList()
* @brief      InitServerList
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::InitServerList()
{
  for(XDWORD c=0; c<XTRACEMONITOR_CFG_MAXSERVERS; c++)
    {
      XTRACEMONITOR_SERVERCFG* server = cfg->GetServer(c, XTRACEMONITOR_CFG_TYPESERVER_UNKWOWN);
      if(server)
        {
          if(!server->config.IsEmpty())
            {
              if(server->type == XTRACEMONITOR_CFG_TYPESERVER_UDP)
                {
                  ServerIPComboBox->Items->Add(server->name.Get());
                }

              if(server->type == XTRACEMONITOR_CFG_TYPESERVER_UART)
                {
                  ServerUARTComboBox->Items->Add(server->name.Get());
                }
            }
        }
    }

  ServerIPComboBox->ItemIndex   = cfg->GetServerIPSelected();
  ServerUARTComboBox->ItemIndex = cfg->GetServerUARTSelected();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::OpenUDPServer(XSTRING* servercfg)
* @brief      OpenUDPServer
* @ingroup    XUTILS
*

* @param[in]  servercfg :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::OpenUDPServer(XSTRING* servercfg)
{
  if(openudp)
    {
      CloseUDPServer();
    }

  if(openudp) return false;

  if(!diostreamudpcfg) diostreamudpcfg = new DIOSTREAMUDPCONFIG();

	if(diostreamudpcfg)
		{
      diostreamudpcfg->SetFromString(servercfg);
			diostreamudpcfg->SetMode(DIOSTREAMMODE_SERVER);
      diostreamudpcfg->SetIsUsedDatagrams(true);
      diostreamudpcfg->SetSizeBufferSO(10000*1024);
      diostreamudpcfg->SetThreadPriority(XTHREADPRIORITY_REALTIME);
      diostreamudpcfg->SetThreadWaitYield(0);

			diostreamudp = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO(diostreamudpcfg);
			if(diostreamudp)
				{
					if(diostreamudp->Open())
						{
              openudp = true;
              xthreadreadUDP->Run(true);
						}
				}
    }

  return openudp;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::CloseUDPServer()
* @brief      CloseUDPServer
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::CloseUDPServer()
{
  if(!diostreamudp) return false;

  openudp = false;
  if(xthreadreadUDP) xthreadreadUDP->Run(openudp);

  diostreamudp->Disconnect();
  diostreamudp->WaitToDisconnected(10);
  diostreamudp->Close();

  GEN_DIOFACTORY.DeleteStreamIO(diostreamudp);
  delete diostreamudpcfg;

  diostreamudp      = NULL;
  diostreamudpcfg   = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::OpenUARTServer(XSTRING* servercfg)
* @brief      OpenUARTServer
* @ingroup    XUTILS
*

* @param[in]  servercfg :
*
* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::OpenUARTServer(XSTRING* servercfg)
{
  if(openuart)
    {
      CloseUARTServer();
    }

  if(openuart) return false;

  if(!diostreamuartcfg) diostreamuartcfg = new DIOSTREAMUARTCONFIG();

	if(diostreamuartcfg)
		{
      diostreamuartcfg->SetFromString(servercfg);

			diostreamuart = (DIOSTREAMUART*)GEN_DIOFACTORY.CreateStreamIO(diostreamuartcfg);
			if(diostreamuart)
				{
					if(diostreamuart->Open())
						{
              openuart = true;
              xthreadreadUART->Run(true);
						}
				}
    }

  return openuart;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::CloseUARTServer()
* @brief      CloseUARTServer
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::CloseUARTServer()
{
  if(!diostreamuart) return false;

  openuart = false;
  if(xthreadreadUART) xthreadreadUART->Run(openuart);

  diostreamuart->Close();

  GEN_DIOFACTORY.DeleteStreamIO(diostreamuart);
  delete diostreamuartcfg;

  diostreamuart     = NULL;
  diostreamuartcfg  = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::RedrawStatusMsgList()
* @brief      RedrawStatusMsgList
* @ingroup    XUTILS
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::RedrawStatusMsgList(ORIGIN* origin)
{
  XVECTOR<XTRACE_STATUS_MSG*>* statusmsgs = origin->status_msgs.StatusMsg_GetAll();
  if(!statusmsgs) return false;

  origin->valuelisteditor->Strings->Clear();

  for(XDWORD c=0; c<statusmsgs->GetSize(); c++)
    {
      XTRACE_STATUS_MSG* statusmsg = statusmsgs->Get(c);
      if(statusmsg)
        {
          AnsiString string = statusmsg->GetName()->Get();
          XSTRING    valuestr;

          switch(statusmsg->GetType())
            {
              case XTRACE_TYPE_STATUS_MSG_UNKNOWN     : break;

              case XTRACE_TYPE_STATUS_MSG_BOOLEAN     : valuestr.Format(_L("%s"),statusmsg->Value_GetBoolean()?_L("true"):_L("false"));
                                                        break;

              case XTRACE_TYPE_STATUS_MSG_INTEGER     : valuestr.Format(_L("%d"), statusmsg->Value_GetInteger());
                                                        break;

              case XTRACE_TYPE_STATUS_MSG_STRING      : valuestr = statusmsg->Value_GetString()->Get();
                                                        break;

              case XTRACE_TYPE_STATUS_MSG_XDWORD      : valuestr.Format(_L("%d"), statusmsg->Value_GetDword());
                                                        break;

              case XTRACE_TYPE_STATUS_MSG_FLOAT       : valuestr.Format(_L("%f"), statusmsg->Value_GetFloat());
                                                        break;

              case XTRACE_TYPE_STATUS_MSG_RGBCOLOR    : valuestr.Format(_L("#%02X%02X%02X"), statusmsg->Value_GetColor()[0],statusmsg->Value_GetColor()[1],statusmsg->Value_GetColor()[2]);
                                                        break;

            }

          string += _L("=");
          string += valuestr.Get();

          origin->valuelisteditor->Strings->Add(string);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool TMainForm::IsWindowOutsideExtendedDesktop(HWND hwnd)
* @brief      IsWindowOutsideExtendedDesktop
* @ingroup
*

* @return     bool : true if is succesful.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool TMainForm::IsWindowOutsideExtendedDesktop(HWND hwnd)
{
  MONITOR_RECTS monitorrect;
  RECT          windowrect;

  if(GetWindowRect(hwnd, &windowrect))
    {
      //int windowWidth = windowrect.right - windowrect.left;
      //int windowHeight = windowrect.bottom - windowrect.top;

      if(windowrect.left   < monitorrect.rcCombined.left     ||
         windowrect.right  > monitorrect.rcCombined.right    ||
         windowrect.top    < monitorrect.rcCombined.top      ||
         windowrect.bottom > monitorrect.rcCombined.bottom)
        {
          return true; // Window is partially or fully outside the extended desktop
        }
    }

  return false; // Window is within the extended desktop
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::ThreadReadUDPFunction(void* param)
* @brief      ThreadReadUDPFunction
* @ingroup    XUTILS
*

* @param[in]  param :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::ThreadReadUDPFunction(void* param)
{
	TMainForm* mainform = (TMainForm*)param;
	if(!mainform) return;

  bool addmessage = true;

  if(mainform->exitapp)
    {
      return;
    }

  if(mainform->changeorigin)
    {
      return;
    }

  while(addmessage)
    {
      XDWORD     publicIP   = 0;
      XDWORD     localIP    = 0;
      XBYTE      level      = 0;
      XDWORD     sequence   = 0;
      XDATETIME  xtime;
      XBUFFER    data;
      XSTRING    string;

      if(mainform->exitapp)
        {
          break;
        }

      if(mainform->changeorigin)
        {
          break;
        }

      xtime.SetToZero();
      string.Empty();

      if(mainform->openudp)
        {
         //XDWORD error = XTRACE::instance->GetTraceFromXBuffer((*mainform->diostreamudp->GetInXBuffer()), publicIP, localIP, level, sequence, &xtime, data);

          XSTRING  address;
          XWORD    port;
          XBUFFER  data;
          bool     publicIPnopacket = false;

          mainform->diostreamudp->ReadDatagram(address, port, data);
          XDWORD error = XTRACE::instance->GetTraceFromXBuffer(data, publicIP, localIP, level, sequence, &xtime, data);

          if(!error)
            {
              if(!mainform->run) return;


              if((!publicIP) && (!address.IsEmpty()))
                {
                  DIOIP pIP;

                  pIP.Set(address);
                  if(!pIP.IsLocal() && !pIP.IsAPIPA())
                    {
                      publicIP = (pIP.Get()[0]<<24) | (pIP.Get()[1]<<16) | (pIP.Get()[2]<<8) | (pIP.Get()[3]);

                      if(publicIP == localIP)
                        {
                          publicIP = 0;
                        }
                       else
                        {
                          publicIPnopacket = true;
                        }
                    }
                }


              XTRACE::instance->SetTraceDataToText(data, string);

              mainform->AddDBGMessage(publicIP, publicIPnopacket, localIP, level, sequence, xtime, string);
            }
           else
            {
              switch(error)
                {
                    case 1  : break;
                    default : {
                                XSTRING string;

                                string.Format(_L("Message lost by packet error [code %d]."), error);
                                mainform->PrintStatus(string.Get());
                              }
                              break;
                }

              addmessage = false;
            }

        } else addmessage = false;
   }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::ThreadReadUARTFunction(void* param)
* @brief      ThreadReadUARTFunction
* @ingroup    XUTILS
*

* @param[in]  param :
*
* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::ThreadReadUARTFunction(void* param)
{
	TMainForm* mainform = (TMainForm*)param;
	if(!mainform) return;

  bool addmessage = true;

  if(mainform->exitapp)
    {
      return;
    }

  if(mainform->changeorigin)
    {
      return;
    }

  while(addmessage)
    {
      XDWORD     publicIP   = 0;
      XDWORD     localIP    = 0;
      XBYTE      level      = 0;
      XDWORD     sequence   = 0;
      XDATETIME  xtime;
      XBUFFER    data;
      XSTRING    string;

      if(mainform->exitapp)
        {
          break;
        }

      if(mainform->changeorigin)
        {
          break;
        }

      xtime.SetToZero();
      string.Empty();

      if(mainform->openuart)
        {
          XDWORD error = XTRACE::instance->GetTraceFromXBuffer((*mainform->diostreamuart->GetInXBuffer()), publicIP, localIP, level, sequence, &xtime, data);
          if(!error)
            {
              if(!mainform->run) return;

              XTRACE::instance->SetTraceDataToText(data, string);

              mainform->AddDBGMessage(0xFFFFFFFF, false,  0xFFFFFFFF, level, sequence, xtime, string);
            }
           else
            {
              switch(error)
                {
                  case 1  : break;
                  default : {
                              XSTRING string;

                              string.Format(_L("Message lost by packet error [code %d]."), error);
                              mainform->PrintStatus(string.Get());
                            }
                            break;
                }

              addmessage = false;
            }

        } else addmessage = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void TMainForm::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    XUTILS
*

* @return     void : does not return anything.
*
*---------------------------------------------------------------------------------------------------------------------*/
void TMainForm::Clean()
{
  checkinternetconnection  = NULL;
  haveinternet             = false;

  run                      = false;

  cfg                      = NULL;

  diostreamudpcfg          = NULL;
  diostreamudp             = NULL;
  openudp                  = false;

  diostreamuartcfg         = NULL;
  diostreamuart            = NULL;
  openuart                 = false;

  grp_screen               = NULL;
  grp_canvas               = NULL;

  rootorigin               = NULL;
  localnointernetorigin    = NULL;
  noreadudp                = false;

  xdatetime                = NULL;

  UARTorigin               = NULL;
  noreaduart               = false;

  xmutexaddDBGmessage      = NULL;
  xmutexaddorigin          = NULL;

  xthreadreadUDP           = NULL;
  xthreadreadUART          = NULL;

  timerID                  = 0;
}



