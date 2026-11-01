/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor.cpp
*
* @class      XTRACEMONITOR
* @brief      GEN graphical eXtended Trace Monitor
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
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor.h"

#include "VersionFrameWork.h"

#include "XPath.h"
#include "XTimer.h"
#include "XFactory.h"
#include "XString.h"
#include "XSystem.h"
#include "XLog.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XTrace.h"
#include "XFSMachine.h"

#include "GRP2DCanvas.h"
#include "GRPScreenCFGChromes.h"
#include "GRPScreen.h"
#include "GRPViewPort.h"
#include "GRPXEvent.h"

#include "INPManager.h"

#include "UI_XEvent.h"
#include "UI_Manager.h"
#include "UI_Layout.h"
#include "UI_Element.h"
#include "UI_Element_Text.h"
#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Element_Tree.h"

#include "APPFlowLog.h"
#include "APPFlowExtended.h"

#include "XTraceMonitor_CFG.h"
#include "XTraceMonitor_TranslateID.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

 APPLICATIONCREATEINSTANCE(XTRACEMONITOR, xtracemonitor)



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR::XTRACEMONITOR()
* @brief      Constructor.
* @ingroup    UTILITIES
*
* @return     Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR::XTRACEMONITOR() :  XFSMACHINE(0)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR::~XTRACEMONITOR()
* @brief      Destructor.
* @ingroup    UTILITIES
*
* @return     Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR::~XTRACEMONITOR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::InitFSMachine()
* @brief      Initializes the finite state machine.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::InitFSMachine()
{
  if(!AddState( XTRACEMONITOR_XFSMSTATE_NONE        ,
                XTRACEMONITOR_XFSMEVENT_INI         , XTRACEMONITOR_XFSMSTATE_INI       ,
                XTRACEMONITOR_XFSMEVENT_END         , XTRACEMONITOR_XFSMSTATE_END       ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( XTRACEMONITOR_XFSMSTATE_INI         ,
                XTRACEMONITOR_XFSMEVENT_NONE        , XTRACEMONITOR_XFSMSTATE_NONE      ,
                XTRACEMONITOR_XFSMEVENT_END         , XTRACEMONITOR_XFSMSTATE_END       ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;


  if(!AddState( XTRACEMONITOR_XFSMSTATE_END         ,
                XTRACEMONITOR_XFSMEVENT_NONE        , XTRACEMONITOR_XFSMSTATE_NONE      ,
                XTRACEMONITOR_XFSMEVENT_INI         , XTRACEMONITOR_XFSMSTATE_INI       ,
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::AppProc_Ini()
* @brief      Initializes the application process.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::AppProc_Ini()
{
  XSTRING string;

  //-------------------------------------------------------------------------------------------------

  GEN_SET_VERSION(APPLICATION_NAMEAPP, APPLICATION_NAMEFILE, APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR, APPLICATION_OWNER, APPLICATION_YEAROFCREATION)

  Application_GetName()->Set(APPLICATION_NAMEAPP);

  SetInitOptions(APPFLOWGRAPHICS_INIOPTION_CREATEMAINSCREEN | APPFLOWGRAPHICS_INIOPTION_SHOWMAINSCREEN | APPFLOWGRAPHICS_INIOPTION_INPUT);

  //--------------------------------------------------------------------------------------------------

  XTRACE_SETAPPLICATIONNAME((*Application_GetName()));
  XTRACE_SETAPPLICATIONVERSION(APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);
  XTRACE_SETAPPLICATIONID(string);

  //--------------------------------------------------------------------------------------------------

  GEN_XPATHSMANAGER.AdjustRootPathDefault(APPFLOW_DEFAULT_DIRECTORY_ROOT);

  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS, APPFLOW_DEFAULT_DIRECTORY_GRAPHICS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_FONTS, APPFLOW_DEFAULT_DIRECTORY_FONTS);
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_UI_LAYOUTS, APPFLOW_DEFAULT_DIRECTORY_UI_LAYOUTS);

  GEN_XPATHSMANAGER.CreateAllPathSectionOnDisk();

  //--------------------------------------------------------------------------------------------------

  InitFSMachine();

  //--------------------------------------------------------------------------------------

  xtimer = GEN_XFACTORY.CreateTimer();
  if(!xtimer) return false;

  //--------------------------------------------------------------------------------------

  { XPATH xpath;

    GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
    xpath.Slash_Add();
    xpath.Add(APPLICATION_NAMEFILE);
    xpath.Add(XTRANSLATION_NAMEFILEEXT);

    if(!GEN_XTRANSLATION.Ini(xpath))
      {
        return false;
      }
  }

  //--------------------------------------------------------------------------------------

  APPFLOW_CFG_SETAUTOMATICTRACETARGETS

  //--------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPStart(&APPFLOW_CFG);

  //--------------------------------------------------------------------------------------

  SetEvent(XTRACEMONITOR_XFSMEVENT_INI);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::AppProc_FirstUpdate()
* @brief      Executes the first application update.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::AppProc_FirstUpdate()
{
  //--------------------------------------------------------------------------------------

  INPDEVICE* inpdevice;

  inpdevice = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_KEYBOARD);
  if(inpdevice)
    {
      button[XTRACEMONITOR_BUTTON_ESC] = inpdevice->GetButton(INPBUTTON_ID_ESCAPE);
    }

  inpdevice = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);
  if(inpdevice)
    {
      button[XTRACEMONITOR_BUTTON_MOUSE] = inpdevice->GetButton(INPBUTTON_ID_MOUSE_LEFT);
      cursor = inpdevice->GetCursor(0);
    }

  inpdevice = GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_TOUCHSCREEN);
  if(inpdevice)
    {
      button[XTRACEMONITOR_BUTTON_TOUCHSCREEN] = inpdevice->GetButton(INPBUTTON_ID_TOUCHSCREEN);
      cursor = inpdevice->GetCursor(0);
    }

  //--------------------------------------------------------------------------------------

  if(!Ini_UserInterface(true))
    {
      return false;
    }

  //--------------------------------------------------------------------------------------

  if(xtimer) xtimer->Reset();

  //--------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::AppProc_Update()
* @brief      Executes the application update cycle.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::AppProc_Update()
{
  if(GetEvent()==XTRACEMONITOR_XFSMEVENT_NONE)
    {
      switch(GetCurrentState())
        {
          case XTRACEMONITOR_XFSMSTATE_NONE   : break;

          case XTRACEMONITOR_XFSMSTATE_INI    : UpdateInput();
                                                DrawFrame();
                                                GetMainScreen()->UpdateViewports();

                                                if(exitapp)
                                                  {
                                                    SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                                                    exitapp = false;
                                                  }

                                                break;

          case XTRACEMONITOR_XFSMSTATE_END    : break;

        }
    }
   else
    {
      if(GetEvent()<XTRACEMONITOR_LASTEVENT)
        {
          CheckTransition();

          switch(GetCurrentState())
            {
              case XTRACEMONITOR_XFSMSTATE_NONE  : break;

              case XTRACEMONITOR_XFSMSTATE_INI   : break;

              case XTRACEMONITOR_XFSMSTATE_END   : break;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::AppProc_End()
* @brief      Ends the application process.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::AppProc_End()
{
  //--------------------------------------------------------------------------------------

  SetCurrentState(XTRACEMONITOR_XFSMSTATE_END);

  //--------------------------------------------------------------------------------------

  Ini_UserInterface(false);

  //--------------------------------------------------------------------------------------

  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
      xtimer = NULL;
    }

  //--------------------------------------------------------------------------------------

  APPFLOW_EXTENDED.APPEnd();
  APPFLOW_EXTENDED.DelInstance();
  APPFLOW_CFG.DelInstance();

  //--------------------------------------------------------------------------------------

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::UpdateInput()
* @brief      Updates the application input state.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::UpdateInput()
{
  int height = GetMainScreen()->GetHeight();

  if(cursor)
    {
      if(cursor->GetMotion()->IsReadyToTest(50))
        {
          cursor->GetMotion()->InvertYAxis(height);

          GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOTION, cursor->GetMotion());

          cursor->GetMotion()->Reset();
        }
       else
        {
          if(cursor->GetMotion()->GetNPoints() < 50)
            {
              static int   cursor_x       = 0;
              static int   cursor_y       = 0;
              int          now_cursor_x   = (int)cursor->GetX();
              int          now_cursor_y   = (int)height - (int)cursor->GetY();
              bool         changed        = false;

              if(cursor_x != now_cursor_x)
                {
                  cursor_x = now_cursor_x;
                  changed  = true;
                }

              if(cursor_y != now_cursor_y)
                {
                  cursor_y = now_cursor_y;
                  changed  = true;
                }

              if(changed)
                {
                  GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOVE, cursor_x, cursor_y);
                }

              if(button[XTRACEMONITOR_BUTTON_MOUSE] && button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressedWithRelease())
                {
                  GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_SELECCTION, cursor_x, cursor_y);

                  cursor->GetMotion()->Reset();
                }

              if(button[XTRACEMONITOR_BUTTON_TOUCHSCREEN])
                {
                  if(button[XTRACEMONITOR_BUTTON_TOUCHSCREEN]->IsPressed())
                    {
                      GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOVE, cursor_x, cursor_y);
                      GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_SELECCTION, cursor_x, cursor_y);

                      cursor->GetMotion()->Reset();
                    }
                }
            }
        }
    }

  Valor_UpdateSplitter();
  Tree_UpdateInput();

  if(button[XTRACEMONITOR_BUTTON_ESC])
    {
      if(button[XTRACEMONITOR_BUTTON_ESC]->IsPressedWithRelease())
        {
          SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Ini_Graphics(GRPSCREEN* screen)
* @brief      Initializes the graphics subsystem.
* @ingroup    UTILITIES
*
* @param[in]  screen : Screen where the graphics or user interface resources are created.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Ini_Graphics(GRPSCREEN* screen)
{
  if(!screen)
    {
      return false;
    }

  {
    XSTRING title;

    title  = TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_WINDOW_TITLE);
    title += NL(" ");
    title.AddFormat(NL("%d.%d.%d"), APPLICATION_VERSION, APPLICATION_SUBVERSION, APPLICATION_SUBVERSIONERR);

    screen->GetTitle()->Set(title.Get());
  }

  screen->SetDesktopScreenSelected(GRPSCREENTYPE_DESKTOP_MAIN);

  screen->SetPosition(100, 100);
  screen->SetWidth(1220);
  screen->SetHeight(800);

  // Custom chrome (borderless GEN caption). Must run before the native window is created.
  UserInterface_CFGChromes(screen);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Ini_Canvas(GRPSCREEN* screen)
* @brief      Initializes the canvas resources.
* @ingroup    UTILITIES
*
* @param[in]  screen : Screen where the graphics or user interface resources are created.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Ini_Canvas(GRPSCREEN* screen)
{
  if(!screen)
    {
      return false;
    }

  bool status = screen->CreateViewport(APPLICATION_GUI_XTRACEMONITOR_VIEWPORT_ID, 0.0f, 0.0f, (float)screen->GetWidth(), (float)screen->GetHeight(), 0, 0, screen->GetWidth(), screen->GetHeight());
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Ini_UserInterface(bool on)
* @brief      Initializes or ends the user interface.
* @ingroup    UTILITIES
*
* @param[in]  on : true to enable the operation; false to disable it.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Ini_UserInterface(bool on)
{
  if(!on)
    {
      consoleelement = NULL;
      valorelement   = NULL;
      treeelement    = NULL;

      GEN_USERINTERFACE.SubscribeOutputEvents(false, this, &GEN_USERINTERFACE.GetInstance());
      GEN_USERINTERFACE.SubscribeInputEvents(false);
      GEN_USERINTERFACE.DelInstance();

      return true;
    }

  GRPSCREEN*    screen    = NULL;
  GRPVIEWPORT*  viewport  = NULL;
  GRP2DCANVAS*  canvas    = NULL;
  XPATH         xpath;

  screen = GetMainScreen();
  if(!screen)
    {
      return false;
    }

  viewport = screen->GetViewport(0);
  if(!viewport)
    {
      return false;
    }

  canvas = viewport->GetCanvas();
  if(!canvas)
    {
      return false;
    }

  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_UI_LAYOUTS, xpath);
  xpath.Slash_Add();
  xpath.Add(NL("xtracemonitor/xtracemonitor.xml"));

  if(!GEN_USERINTERFACE.Load(xpath, screen, 0))
    {
      return false;
    }

  GEN_USERINTERFACE.SubscribeInputEvents(true);
  GEN_USERINTERFACE.SubscribeOutputEvents(true, this, &GEN_USERINTERFACE.GetInstance());

  GEN_USERINTERFACE.Layout_PutBackground();
  GEN_USERINTERFACE.Elements_SetToRedraw();

  if(!Console_Resolve())
    {
      return false;
    }

  if(!Console_SeedStartupLines())
    {
      return false;
    }

  if(!Valor_Resolve())
    {
      return false;
    }

  if(!Valor_Seed())
    {
      return false;
    }

  if(!Tree_Resolve())
    {
      return false;
    }

  if(!Tree_Seed())
    {
      return false;
    }

  // Default visible control is Stop (monitor considered running until the user stops it).
  Toolbar_SetRunning(true);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::DrawFrame()
* @brief      Draws the current frame.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::DrawFrame()
{
  GRPSCREEN* screen = GetMainScreen();
  if(!screen) return false;

  GRPVIEWPORT* viewport = NULL;
  GRP2DCANVAS* canvas   = NULL;
  bool         status   = false;

  viewport = screen->GetViewport(APPLICATION_GUI_XTRACEMONITOR_VIEWPORT_ID);
  if(viewport)
    {
      canvas = viewport->GetCanvas();
      if(canvas)
        {
          status = true;
        }
    }

  if(!status)
    {
      return false;
    }

  if((screen->GetWidth() < 32) || (screen->GetHeight() < 32))
    {
      return true;
    }

  canvas->ReleaseDrawFramerate();

  canvas->RebuildAllAreas();

  // Bottom-left: 5px from left and bottom (DrawFramerate y is the text baseline; +2 is box bottom).
  canvas->DrawFramerate(screen, 5.0, (double)(screen->GetHeight() - 7));

  GEN_USERINTERFACE.Elements_RebuildDrawAreas();
  GEN_USERINTERFACE.Update();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Console_Resolve()
* @brief      Resolves the layout console element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Console_Resolve()
{
  consoleelement = (UI_ELEMENT_CONSOLE*)GEN_USERINTERFACE.Element_Get(NL("log_console"), UI_ELEMENT_TYPE_CONSOLE);
  if(!consoleelement) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Console_SeedStartupLines()
* @brief      Seeds initial lines into the graphical console.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Console_SeedStartupLines()
{
  if(!consoleelement) return false;

  consoleelement->Clear();

  consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GREEN, NL("XTraceMonitor ready."));
  consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY,  NL("------------------------------------------------"));
  consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_BLUE,  NL("UI_ELEMENT_CONSOLE layout loaded."));
  consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY,  NL("Next: wire XTrace feed and remaining UI."));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Valor_Resolve()
* @brief      Resolves the layout "valor" property list element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Valor_Resolve()
{
  valorelement = (UI_ELEMENT_PROPERTYLIST*)GEN_USERINTERFACE.Element_Get(NL("valor"), UI_ELEMENT_TYPE_PROPERTYLIST);
  if(!valorelement) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Valor_Seed()
* @brief      Seeds sample status rows into the "valor" property list.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Valor_Seed()
{
  if(!valorelement) return false;

  valorelement->Clear();

  valorelement->Add(NL("S.O. version")       , NL("Windows 11 Pro 10.0.26300 (x64)"));
  valorelement->Add(NL("Public IP")          , NL("203.0.113.42"));
  valorelement->Add(NL("Serial")             , NL("XTM-2026-000417"));
  valorelement->Add(NL("Local IPs")          , NL("192.168.1.24, 10.0.0.7"));
  valorelement->Add(NL("User")               , NL("SirGraham"));
  valorelement->Add(NL("Domain user")        , NL("ENDORASOFT\\sirgraham"));
  valorelement->Add(NL("Manager server")     , NL("manager.endorasoft.local:8443"));
  valorelement->Add(NL("Operability Status") , NL("Operative"));
  valorelement->Add(NL("APP version")        , NL("1.0.0"));
  valorelement->Add(NL("Last report")        , NL("2026-10-31 17:54:00"));

  GEN_USERINTERFACE.Elements_SetToRedraw(valorelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Valor_UpdateSplitter()
* @brief      Drives the "valor" column splitter drag from the mouse button state.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Valor_UpdateSplitter()
{
  if(!valorelement || !cursor || !button[XTRACEMONITOR_BUTTON_MOUSE]) return false;

  bool   pressed = button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();
  int    cursor_x = (int)cursor->GetX();
  int    cursor_y = (int)GetMainScreen()->GetHeight() - (int)cursor->GetY();
  double local_x  = 0.0f;
  double local_y  = 0.0f;

  if(!valorelement->ScreenToLocal(cursor_x, cursor_y, local_x, local_y)) return false;

  if(valorelement->IsSplitDragging())
    {
      if(pressed)
        {
          double before = valorelement->GetNameColumnWidth();

          valorelement->Split_DragTo(local_x);

          if(before != valorelement->GetNameColumnWidth()) GEN_USERINTERFACE.Elements_SetToRedraw(valorelement);
        }
       else
        {
          valorelement->Split_EndDrag();
        }
    }
   else
    {
      // Grab only on the press transition, so dragging INTO the splitter from elsewhere does not grab it.
      if(pressed && !valor_prevpressed)
        {
          if(valorelement->HitTestSplitter(local_x, local_y, valorelement->GetBoundaryLine()->width, valorelement->GetBoundaryLine()->height))
            {
              valorelement->Split_BeginDrag();
            }
        }
    }

  valor_prevpressed = pressed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Tree_Resolve()
* @brief      Resolves the layout "tree" element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_Resolve()
{
  treeelement = (UI_ELEMENT_TREE*)GEN_USERINTERFACE.Element_Get(NL("tree"), UI_ELEMENT_TYPE_TREE);
  if(!treeelement) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Tree_Seed()
* @brief      Seeds a sample hierarchy into the "tree" element.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_Seed()
{
  if(!treeelement) return false;

  treeelement->Clear();

  UI_ELEMENT_TREE_NODE* all = treeelement->AddNode(NULL, NL("All"));
  if(!all) return false;

  treeelement->AddNode(all, NL("# Local (no internet)"));
  treeelement->AddNode(all, NL("Home [88.14.204.122]"));

  // Rich sample: many IPs + deep children with long labels to exercise generic H/V scroll.
  static const XCHAR* ips[] =
    {
      NL("IP [37.189.128.248]"),
      NL("IP [46.189.241.182]"),
      NL("IP [79.117.197.73]"),
      NL("IP [83.38.7.165]"),
      NL("IP [83.56.150.31]"),
      NL("IP [85.85.131.24]"),
      NL("IP* [83.38.38.127]"),
      NL("IP* [85.192.82.189]"),
      NL("IP [91.200.12.44]"),
      NL("IP [103.21.244.17]"),
      NL("IP [151.80.33.210]"),
      NL("IP [185.199.108.153]"),
      NL("IP [198.51.100.77]"),
      NL("IP [203.0.113.90]"),
      NL("IP [212.64.18.5]"),
    };

  for(XDWORD i=0; i<(XDWORD)(sizeof(ips) / sizeof(ips[0])); i++)
    {
      UI_ELEMENT_TREE_NODE* ip = treeelement->AddNode(all, (XCHAR*)ips[i]);
      if(!ip) continue;

      UI_ELEMENT_TREE_NODE* trace = treeelement->AddNode(ip, NL("Trace A — uplink via edge-router.primary"));
      treeelement->AddNode(ip, NL("Trace B — secondary path (failover)"));

      if(trace)
        {
          UI_ELEMENT_TREE_NODE* sess = treeelement->AddNode(trace, NL("Session peer [192.168.1.24] NAT"));
          treeelement->AddNode(trace, NL("Session peer [10.0.0.7] NAT"));
          treeelement->AddNode(trace, NL("Session peer [172.16.4.88] DMZ"));

          if(sess)
            {
              UI_ELEMENT_TREE_NODE* flow = treeelement->AddNode(sess, NL("Flow TCP/443 → cdn.endorasoft.local"));
              treeelement->AddNode(sess, NL("Flow UDP/1194 → vpn.endorasoft.local"));

              if(flow)
                {
                  treeelement->AddNode(flow, NL("Packet sample #001 — SYN/ACK handshake complete"));
                  treeelement->AddNode(flow, NL("Packet sample #002 — TLS ClientHello (SNI)"));
                  treeelement->Expand(flow);
                }

              treeelement->Expand(sess);
            }

          treeelement->Expand(trace);
        }

      treeelement->Expand(ip);
    }

  treeelement->AddNode(all, NL("UART STM32 F0"));
  treeelement->AddNode(all, NL("UART STM32 F4 (long label overflow check)"));

  treeelement->Expand(all);
  treeelement->SetSelected(all);

  GEN_USERINTERFACE.Elements_SetToRedraw(treeelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Tree_UpdateInput()
* @brief      Drives tree expand/collapse and selection from the mouse button press transition.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_UpdateInput()
{
  if(!treeelement || !cursor || !button[XTRACEMONITOR_BUTTON_MOUSE]) return false;

  bool pressed = button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();

  // Act only on the press transition.
  if(pressed && !tree_prevpressed)
    {
      int    cursor_x = (int)cursor->GetX();
      int    cursor_y = (int)GetMainScreen()->GetHeight() - (int)cursor->GetY();
      double local_x  = 0.0f;
      double local_y  = 0.0f;

      if(treeelement->ScreenToLocal(cursor_x, cursor_y, local_x, local_y))
        {
          double box_w = treeelement->GetBoundaryLine()->width;
          double box_h = treeelement->GetBoundaryLine()->height;

          if((local_x >= 0.0f) && (local_x < box_w) && (local_y >= 0.0f) && (local_y < box_h))
            {
              if(treeelement->ClickAt(local_x, local_y))
                {
                  GEN_USERINTERFACE.Elements_SetToRedraw(treeelement);
                }
            }
        }
    }

  tree_prevpressed = pressed;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::UserInterface_ElementSelected(UI_ELEMENT* element)
* @brief      Processes the selected user interface element.
* @ingroup    UTILITIES
*
* @param[in]  element : User interface element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Toolbar_SetRunning(bool running)
* @brief      Sets the run/stop toolbar state (shared slot: one visible at a time).
* @ingroup    UTILITIES
*
* @param[in]  running : true → show Stop; false → show Run.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_SetRunning(bool running)
{
  monitor_running = running;
  return Toolbar_ApplyRunStopVisibility();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Toolbar_ApplyRunStopVisibility()
* @brief      Shows Stop or Run in the shared toolbar slot according to monitor_running.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_ApplyRunStopVisibility()
{
  UI_ELEMENT* stop_btn = GEN_USERINTERFACE.Element_Get(NL("stop-btn"), UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT* run_btn  = GEN_USERINTERFACE.Element_Get(NL("run-btn") , UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT* stop_lbl = GEN_USERINTERFACE.Element_Get(NL("stop-lbl"), UI_ELEMENT_TYPE_FORM);
  UI_ELEMENT* run_lbl  = GEN_USERINTERFACE.Element_Get(NL("run-lbl") , UI_ELEMENT_TYPE_FORM);

  if(stop_btn)
    {
      stop_btn->SetVisible(monitor_running);
      GEN_USERINTERFACE.Elements_SetToRedraw(stop_btn);
    }

  if(stop_lbl)
    {
      stop_lbl->SetVisible(monitor_running);
      GEN_USERINTERFACE.Elements_SetToRedraw(stop_lbl);
    }

  if(run_btn)
    {
      run_btn->SetVisible(!monitor_running);
      GEN_USERINTERFACE.Elements_SetToRedraw(run_btn);
    }

  if(run_lbl)
    {
      run_lbl->SetVisible(!monitor_running);
      GEN_USERINTERFACE.Elements_SetToRedraw(run_lbl);
    }

  // Drop hover/press left on the button that just hid (shared toolbar slot).
  GEN_USERINTERFACE.ResetPreselect();

  return (stop_btn != NULL) || (run_btn != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::UserInterface_ElementSelected(UI_ELEMENT* element)
* @brief      Processes the selected user interface element.
* @ingroup    UTILITIES
*
* @param[in]  element : User interface element to process.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::UserInterface_ElementSelected(UI_ELEMENT* element)
{
  if(!element) return false;

  // Custom chrome caption buttons (defaultwindowschromes.zip). Role may be on the
  // multioption or on the pressed button child — walk fathers until a chrome role hits.
  {
    UI_ELEMENT* chrome_el = element;
    while(chrome_el)
      {
        switch(chrome_el->GetChromeRole())
          {
            case UI_ELEMENT_CHROMEROLE_MINIMIZE : if(GetMainScreen()) GetMainScreen()->Minimize(true);
                                                  return true;

            case UI_ELEMENT_CHROMEROLE_CLOSE    : SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                                                  return true;

            case UI_ELEMENT_CHROMEROLE_NONE     : chrome_el = chrome_el->GetFather();
                                                  continue;

            default                            : return true;   // other chrome roles consumed
          }
      }
  }

  XSTRING* name = element->GetName();
  if(!name) return false;

  if(!name->Compare(NL("stop-btn"), true))
    {
      Toolbar_SetRunning(false);
      if(consoleelement) consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY, NL("Stop: monitor paused."));
      return true;
    }

  if(!name->Compare(NL("run-btn"), true))
    {
      Toolbar_SetRunning(true);
      if(consoleelement) consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GREEN, NL("Run: monitor running."));
      return true;
    }

  if(!name->Compare(NL("clear-btn"), true))
    {
      if(consoleelement)
        {
          consoleelement->Clear();
          consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY, NL("Clear."));
        }
      return true;
    }

  if(!name->Compare(NL("delstatus-btn"), true))
    {
      if(consoleelement) consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY, NL("Del status (TODO)."));
      return true;
    }

  if(!name->Compare(NL("dyndns-btn"), true))
    {
      if(consoleelement) consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY, NL("DynDNS (TODO)."));
      return true;
    }

  if(!name->Compare(NL("save-btn"), true))
    {
      if(consoleelement) consoleelement->PrintColor(UI_ELEMENT_CONSOLE_COLOR_GRAY, NL("Save (TODO)."));
      return true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::UserInterface_ChangeLiteralText(UI_ELEMENT_TEXT* element_text, XSTRING* maskvalue, XSTRING* maskresolved)
* @brief      Resolves a user interface literal text.
* @ingroup    UTILITIES
*
* @param[in]  element_text : Text element that contains the literal to resolve.
* @param[in]  maskvalue : Mask value to resolve.
* @param[in]  maskresolved : Resolved mask output value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::UserInterface_ChangeLiteralText(UI_ELEMENT_TEXT* element_text, XSTRING* maskvalue, XSTRING* maskresolved)
{
  if(!element_text)   return false;
  if(!maskvalue)      return false;
  if(!maskresolved)   return false;

  if(!maskvalue->Compare(NL("TITLE"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_LAYOUT_TITLE));
    }

  if(!maskvalue->Compare(NL("BTN_STOP"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_STOP));
    }

  if(!maskvalue->Compare(NL("BTN_RUN"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_RUN));
    }

  if(!maskvalue->Compare(NL("BTN_CLEAR"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_CLEAR));
    }

  if(!maskvalue->Compare(NL("BTN_DELSTATUS"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_DELSTATUS));
    }

  if(!maskvalue->Compare(NL("BTN_DYNDNS"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_DYNDNS));
    }

  if(!maskvalue->Compare(NL("BTN_SAVE"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_SAVE));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::UserInterface_CFGChromes(GRPSCREEN* screen)
* @brief      Configures GEN custom / native window chromes.
* @ingroup    UTILITIES
*
* @param[in]  screen : Screen that receives the chromes configuration.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::UserInterface_CFGChromes(GRPSCREEN* screen)
{
  if(!screen) return false;

  GRPSCREENCFGCHROMES cfgchromes;

  #ifdef GRP_SCREEN_CUSTOMCHROMES_ACTIVE

  // Default bundle: ui_layouts/defaultwindowschromes.zip (layout name "chrome").
  // Auto-hide off: caption stays visible (no automatic hide / unhide cycle).
  cfgchromes.SetCustomAutoHide(0);

  #else

  cfgchromes.SetNativeCaptionActive(true);
  cfgchromes.SetNativeIconActive(false);
  cfgchromes.SetNativeTitleActive(true);
  cfgchromes.SetNativeMinimizeActive(true);
  cfgchromes.SetNativeMaximizeActive(false);
  cfgchromes.SetNativeCloseActive(true);

  #endif

  cfgchromes.SetResizeActive(false);

  screen->SetCFGChromes(cfgchromes);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR::HandleEvent_UserInterface(UI_XEVENT* event)
* @brief      Handles user interface events.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @param[in]  event : Event information to process.
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR::HandleEvent_UserInterface(UI_XEVENT* event)
{
  switch(event->GetEventType())
    {
      case UI_XEVENT_TYPE_OUTPUT_SELECTED        : { UI_ELEMENT* element = event->GetElement();
                                                     if(element)
                                                       {
                                                         UserInterface_ElementSelected(element);
                                                       }
                                                   }
                                                   break;

      case UI_XEVENT_TYPE_OUTPUT_TEXTTOCHANGE    : { UI_ELEMENT_TEXT* element_text = (UI_ELEMENT_TEXT*)event->GetElement();

                                                     UserInterface_ChangeLiteralText(element_text, event->GetMaskTextValue(), event->GetMaskTextResolved());
                                                   }
                                                   break;

      case UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS  : break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR::HandleEvent_Graphics(GRPXEVENT* event)
* @brief      Handles graphics events.
* @ingroup    UTILITIES
*
* @param[in]  event : Event information to process.
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR::HandleEvent_Graphics(GRPXEVENT* event)
{
  switch(event->GetEventType())
    {
      case GRPXEVENT_TYPE_SCREEN_CREATING         : { GRPSCREEN* screen  = event->GetScreen();
                                                      if(!screen) break;

                                                      Ini_Graphics(screen);
                                                    }
                                                    break;

      case GRPXEVENT_TYPE_SCREEN_CANVASCREATING   : { GRPSCREEN* screen  = event->GetScreen();
                                                      if(!screen) break;

                                                      Ini_Canvas(screen);
                                                    }
                                                    break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR::HandleEvent(XEVENT* xevent)
* @brief      Handles an event.
* @ingroup    UTILITIES
*
* @param[in]  xevent : Event information to process.
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_GRAPHICS                   : { GRPXEVENT* event = (GRPXEVENT*)xevent;
                                                      if(!event) return;

                                                      HandleEvent_Graphics(event);
                                                    }
                                                    break;

      case XEVENT_TYPE_USERINTERFACE              : { UI_XEVENT* event = (UI_XEVENT*)xevent;
                                                      if(!event) return;

                                                      HandleEvent_UserInterface(event);
                                                    }
                                                    break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
* @return     void : Does not return a value.
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR::Clean()
{
  xtimer          = NULL;
  cursor          = NULL;
  consoleelement  = NULL;
  valorelement    = NULL;
  valor_prevpressed = false;
  treeelement     = NULL;
  tree_prevpressed  = false;
  monitor_running = true;
  exitapp         = false;

  for(int c=0; c<XTRACEMONITOR_BUTTON_MAX; c++)
    {
      button[c] = NULL;
    }
}
