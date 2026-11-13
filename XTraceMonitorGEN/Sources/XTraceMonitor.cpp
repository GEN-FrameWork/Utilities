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

#include <stdio.h>
#ifdef WINDOWS
#include <windows.h>
#endif
#include "GRPXEvent.h"

#include "INPManager.h"

#include "UI_XEvent.h"
#include "UI_Manager.h"
#include "UI_Layout.h"
#include "UI_Background.h"
#include "UI_Color.h"
#include "UI_Element.h"
#include "UI_Element_Text.h"
#include "UI_Element_TextBox.h"
#include "UI_Element_Form.h"
#include "UI_Element_RadioGroup.h"
#include "UI_Element_RadioButton.h"
#include "UI_Element_Console.h"
#include "UI_Element_PropertyList.h"
#include "UI_Element_Tree.h"
#include "UI_Element_Image.h"
#include "UI_Element_Option.h"
#include "UI_Element_Animation.h"
#include "UI_Skin.h"
#include "UI_SkinCanvas.h"
#include "UI_BoundaryLine.h"
#include "UI_Colors.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "GRP2DColor.h"

#include "APPFlowLog.h"
#include "APPFlowExtended.h"
#ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
#include "APPFlowExtended_InternetStatus.h"
#include "APPFlowInternetServices.h"
#endif

#include "DIOIP.h"
#include "DIODNSResolver.h"

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

  netstatus_timer = GEN_XFACTORY.CreateTimer();
  if(!netstatus_timer) return false;

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

  ApplySavedScreenGeometry();

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
  PersistScreenGeometryIfExiting();

  if(GetEvent()==XTRACEMONITOR_XFSMEVENT_NONE)
    {
      switch(GetCurrentState())
        {
          case XTRACEMONITOR_XFSMSTATE_NONE   : break;

          case XTRACEMONITOR_XFSMSTATE_INI    : form_cfg.Tick();
                                                form_filters.Tick();

                                                if(!GEN_USERINTERFACE.Element_IsModalActive())
                                                  {
                                                    InternetStatus_Update(false);
                                                  }

                                                UpdateInput();
                                                DrawFrame();
                                                GetMainScreen()->UpdateViewports();

                                                if(exitapp)
                                                  {
                                                    PersistScreenGeometryToCFG();
                                                    screen_geometry_persisted = true;
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

  PersistScreenGeometryIfExiting();

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

  // Geometry must already be persisted in AppProc_Update: APPFLOWGRAPHICS::End deletes
  // mainscreen before AppProc_End runs.

  //--------------------------------------------------------------------------------------

  Ini_UserInterface(false);

  //--------------------------------------------------------------------------------------

  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
      xtimer = NULL;
    }

  if(netstatus_timer)
    {
      GEN_XFACTORY.DeleteTimer(netstatus_timer);
      netstatus_timer = NULL;
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

  static int cursor_x = 0;
  static int cursor_y = 0;

  int  now_cursor_x = 0;
  int  now_cursor_y = 0;
  bool bar_owned    = false;

  if(cursor)
    {
      now_cursor_x = (int)cursor->GetX();
      now_cursor_y = (int)height - (int)cursor->GetY();

      if(cursor->GetMotion()->IsReadyToTest(50))
        {
          cursor->GetMotion()->InvertYAxis(height);

          GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOTION, cursor->GetMotion());

          cursor->GetMotion()->Reset();
        }
       else if(cursor->GetMotion()->GetNPoints() < 50)
        {
          bool changed = false;

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

          bool mouseheld = (button[XTRACEMONITOR_BUTTON_MOUSE] && button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed());

          if(mouseheld)
            {
              bar_owned = GEN_USERINTERFACE.ScrollBar_OnPointer(now_cursor_x, now_cursor_y, true);
              if(bar_owned) cursor->GetMotion()->Reset();
            }
           else
            {
              bar_owned = GEN_USERINTERFACE.ScrollBar_OnPointer(cursor_x, cursor_y, false);
            }

          if(button[XTRACEMONITOR_BUTTON_TOUCHSCREEN])
            {
              if(button[XTRACEMONITOR_BUTTON_TOUCHSCREEN]->IsPressed())
                {
                  GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_CURSOR_MOVE, cursor_x, cursor_y);
                  if(GEN_USERINTERFACE.ScrollBar_OnPointer(cursor_x, cursor_y, true))
                    {
                      cursor->GetMotion()->Reset();
                    }
                   else
                    {
                      GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_SELECCTION, cursor_x, cursor_y);
                      cursor->GetMotion()->Reset();
                    }
                }
               else
                {
                  GEN_USERINTERFACE.ScrollBar_OnPointer(cursor_x, cursor_y, false);
                }
            }
        }

      if(button[XTRACEMONITOR_BUTTON_MOUSE] && button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressedWithRelease())
        {
          int click_x = now_cursor_x;
          int click_y = now_cursor_y;

          if(GEN_USERINTERFACE.Element_IsModalActive())
            {
              if(!bar_owned)
                {
                  GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_SELECCTION, click_x, click_y);
                }
            }
           else if(OpStatus_HitTest(click_x, click_y))
            {
              OpStatus_OnClick();
            }
           else if(!bar_owned)
            {
              GEN_USERINTERFACE.SendEvent(UI_XEVENT_TYPE_INPUT_SELECCTION, click_x, click_y);
            }

          if(cursor) cursor->GetMotion()->Reset();
        }
    }

  OpStatus_UpdateInput();

  if(opstatus_form)
    {
      XDWORD visible = 1;
      if(opstatus_expanded && (opstatus_count > 1))
        {
          visible = opstatus_count;
          if(visible < 1) visible = 1;
          if(visible > XTRACEMONITOR_OPSTATUS_MAXLINES) visible = XTRACEMONITOR_OPSTATUS_MAXLINES;
        }
      OpStatus_ApplyGeometry(visible);
      if(opstatus_expanded && (opstatus_count > 1)) GEN_USERINTERFACE.Element_PutToLastPositionLayout(opstatus_form);

      if((opstatus_line_h < 1.0f) && opstatus_text)
        {
          opstatus_text->SetMustReDraw(true);
          GEN_USERINTERFACE.Elements_SetToRedraw(opstatus_form, true);
        }
    }

  if(!GEN_USERINTERFACE.ScrollBar_IsDragging())
    {
      Valor_UpdateSplitter();
      Tree_UpdateInput();
    }

  if(button[XTRACEMONITOR_BUTTON_ESC])
    {
      if(button[XTRACEMONITOR_BUTTON_ESC]->IsPressedWithRelease())
        {
          if(GEN_USERINTERFACE.EditText_HasFocus())
            {
              GEN_USERINTERFACE.EditText_ClearFocus(true);
            }
           else
            {
              PersistScreenGeometryToCFG();
              screen_geometry_persisted = true;
              SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
            }
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

  int pos_x = 0;
  int pos_y = 0;
  int size_w = XTRACEMONITOR_WINDOW_WIDTH;
  int size_h = XTRACEMONITOR_WINDOW_HEIGHT;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  int saved_x = 0;
  int saved_y = 0;
  if(cfg.ParseSavedScreenPosition(saved_x, saved_y))
    {
      pos_x = saved_x;
      pos_y = saved_y;
    }

  int saved_w = 0;
  int saved_h = 0;
  if(cfg.ParseSavedScreenSize(saved_w, saved_h))
    {
      size_w = saved_w;
      size_h = saved_h;
    }

  screen->SetPosition(pos_x, pos_y);
  screen->SetWidth((XDWORD)size_w);
  screen->SetHeight((XDWORD)size_h);

  UserInterface_CFGChromes(screen);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplySavedScreenGeometry()
* @brief      Moves/resizes the live main window from CFG savedscreenposition / savedscreensize.
* @ingroup    UTILITIES
*
* @return     bool : true if any geometry was applied; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplySavedScreenGeometry()
{
  GRPSCREEN* screen = GetMainScreen();
  if(!screen) return false;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  bool applied = false;

  int saved_x = 0;
  int saved_y = 0;
  if(cfg.ParseSavedScreenPosition(saved_x, saved_y))
    {
      if(screen->Set_Position(saved_x, saved_y))
        {
          applied = true;
        }
    }

  int saved_w = 0;
  int saved_h = 0;
  if(cfg.ParseSavedScreenSize(saved_w, saved_h))
    {
      int cur_w = (int)screen->GetWidth();
      int cur_h = (int)screen->GetHeight();
      if((cur_w != saved_w) || (cur_h != saved_h))
        {
          if(screen->Resize(saved_w, saved_h))
            {
              applied = true;
              Layout_ApplyFlexibleResize(true);
            }
        }
    }

  return applied;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::PersistScreenGeometryToCFG()
* @brief      Writes live main-window position/size into CFG when the matching save flags are on.
* @note       Must run while GetMainScreen() is still valid. APPFLOWGRAPHICS::End deletes it before AppProc_End.
* @ingroup    UTILITIES
*
* @return     bool : true if CFG was saved; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::PersistScreenGeometryToCFG()
{
  GRPSCREEN* screen = GetMainScreen();
  if(!screen) return false;

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  bool dirty = false;

  if(cfg.GetSaveScreenPosition())
    {      
      int x = screen->GetPositionX();
      int y = screen->GetPositionY();
      int live_x = 0;
      int live_y = 0;

      if(screen->Get_Position(live_x, live_y))
        {         
          if((live_x > -30000) && (live_x < 30000) && (live_y > -30000) && (live_y < 30000))
            {
              x = live_x;
              y = live_y;
            }
        }

      cfg.SetSavedScreenPosition(x, y);
      dirty = true;
    }

  if(cfg.GetSaveScreenSize())
    {
      int w = (int)screen->GetWidth();
      int h = (int)screen->GetHeight();
      int cw = 0;
      int ch = 0;

      if(screen->GetClientSize(cw, ch) && (cw >= XTRACEMONITOR_WINDOW_WIDTH) && (ch >= XTRACEMONITOR_WINDOW_HEIGHT))
        {
          w = cw;
          h = ch;
        }

      cfg.SetSavedScreenSize(w, h);
      dirty = true;
    }

  if(!dirty) return false;
  return cfg.Save();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR::PersistScreenGeometryIfExiting()
* @brief      One-shot persist when exit has been requested and the main screen is still alive.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR::PersistScreenGeometryIfExiting()
{
  if(screen_geometry_persisted) return;
  if(GetExitType() == APPFLOWBASE_EXITTYPE_UNKNOWN) return;

  PersistScreenGeometryToCFG();
  screen_geometry_persisted = true;
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

  bool status = screen->CreateViewport(GRPVIEWPORT_ID_MAIN, 0.0f, 0.0f, (float)screen->GetWidth(), (float)screen->GetHeight(), 0, 0, screen->GetWidth(), screen->GetHeight());
  if(status)
    {
      GRPVIEWPORT* viewport = screen->GetViewport(GRPVIEWPORT_ID_MAIN);
      if(viewport)
        {          
          viewport->SetMinSize((float)XTRACEMONITOR_WINDOW_WIDTH, (float)XTRACEMONITOR_WINDOW_HEIGHT);
          viewport->SetMaxSize(3840.0f, 2160.0f);
        }
    }

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
      consoleelement            = NULL;
      valorelement              = NULL;
      treeelement               = NULL;
      tree_uart_node            = NULL;
      opstatus_form             = NULL;
      opstatus_text             = NULL;
      brand_monitor             = NULL;

      if(opstatus_click_timer)
        {
          GEN_XFACTORY.DeleteTimer(opstatus_click_timer);
          opstatus_click_timer = NULL;
        }

      GRPSCREEN* screen = GetMainScreen();
      if(screen) UnSubscribeEvent(GRPXEVENT_TYPE_SCREEN_CHANGESIZE, screen);

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

  SubscribeEvent(GRPXEVENT_TYPE_SCREEN_CHANGESIZE, screen);

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

  BrandMonitor_Resolve();

  if(!InternetStatus_Resolve())
    {
      return false;
    }

  if(GetMainScreen())
    {
      InternetStatus_ApplyGeometry((double)GetMainScreen()->GetWidth());
    }
  InternetStatus_Update(true);

  if(!OpStatus_Resolve())
    {
      return false;
    }

  form_cfg.SetOwner(this);
  if(!form_cfg.Resolve())
    {
      return false;
    }

  form_filters.SetOwner(this);
  if(!form_filters.Resolve())
    {
      return false;
    }

  ApplyColorTheme(XTRACEMONITOR_CFG::GetInstance(false).GetColorTheme());


  workingmode = true;

  Toolbar_ApplyRunStopVisibility();
  Toolbar_ApplyFilterBtnVisibility();

  OpStatus_Log(NL("XTraceMonitor ready."));

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

  viewport = screen->GetViewport(GRPVIEWPORT_ID_MAIN);
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


  int live_w = (int)screen->GetWidth();
  int live_h = (int)screen->GetHeight();
  bool size_relaid = false;
  bool size_mismatch = ((live_w != layout_applied_w) || (live_h != layout_applied_h));
  if(size_mismatch)
    {
      Layout_ApplyFlexibleResize(true);
      size_relaid = true;
      resize_recover_frames = 3;
     
      canvas->DeleteAllRebuildAreas();
      canvas->SetClipBox(0.0, 0.0, (double)(live_w > 0 ? live_w - 1 : 0), (double)(live_h > 0 ? live_h - 1 : 0));
    }
  else if(resize_recover_frames > 0)
    {
      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();
      size_relaid = true;
      resize_recover_frames--;
    }

  if(chrome_theme_dirty)
    {
      XTRACEMONITOR_CFG& cfgtheme = XTRACEMONITOR_CFG::GetInstance(false);
      const bool light = (cfgtheme.ResolveColorTheme(cfgtheme.GetColorTheme()) == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);
      if(ApplyColorTheme_RefreshChrome(light))
        {
          if(GEN_USERINTERFACE.Layouts_Get(NL("chrome")))
            {
              UI_LAYOUT* chrome = GEN_USERINTERFACE.Layouts_Get(NL("chrome"));
              if(chrome && chrome->GetSkin() && chrome->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
                ((UI_SKINCANVAS*)chrome->GetSkin())->InvalidateCompositionCaches();
              chrome->Elements_SetToRedraw();
            }
          chrome_theme_dirty = false;
        }
    }

  canvas->ReleaseDrawFramerate();

  if(!size_relaid) canvas->RebuildAllAreas();

  canvas->DrawFramerate(screen, 5.0, (double)(screen->GetHeight() - 7));

  GEN_USERINTERFACE.Elements_RebuildDrawAreas();
  if(size_relaid) GEN_USERINTERFACE.Elements_SetToRedraw();
  GEN_USERINTERFACE.Update();

  BrandMonitor_RepaintAfterChrome();
  if(!GEN_USERINTERFACE.Element_IsModalActive())
    {
      InternetStatus_RepaintAfterChrome();
    }

  {
    UI_LAYOUT* xtm = GEN_USERINTERFACE.Layouts_Get(NL("xtracemonitor"));
    if(xtm && xtm->GetSkin() && xtm->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
      {
        UI_SKINCANVAS* skin = (UI_SKINCANVAS*)xtm->GetSkin();
        if(skin && skin->CompactLegendsNeedRepair())
          skin->RepaintCompactLegendForms(xtm);
      }
  }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Layout_ApplyFlexibleResize(bool compose)
* @brief      Applies the flexible resize layout from the current screen size.
*             Grows/shrinks: valor/footer width, console width+height, tree height (bottom-aligned with
*             console; top fixed with valor), brand_monitor horizontal position. Toolbar and other chrome
*             stay at design geometry. Custom chrome buttons track via UpdateCFGChromesButtonsPosition.
* @param[in]  compose : When true (DrawFrame), wipe background and dirty all elements. When false
*             (CHANGESIZE inside UpdateSize), geometry only — UpdateSize already PutBackground+Update.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Layout_ApplyFlexibleResize(bool compose)
{
  GRPSCREEN* screen = GetMainScreen();
  if(!screen) return false;

 
  double W = (double)screen->GetWidth();
  double H = (double)screen->GetHeight();
  if((W < 1.0) || (H < 1.0)) return false;

  UI_LAYOUT* content_layout = GEN_USERINTERFACE.Layouts_Get(NL("xtracemonitor"));
  if(content_layout &&
     ((content_layout->GetDesignWidth() != (XDWORD)W) || (content_layout->GetDesignHeight() != (XDWORD)H)))
    {
      content_layout->SetDesignSize((XDWORD)W, (XDWORD)H);
      if(content_layout->GetStyleSheet())
        {
          content_layout->GetStyleSheet()->SetMediaViewport((int)W, (int)H);
        }
    }

  const double margin   = XTRACEMONITOR_LAYOUT_MARGIN;
  const double gap      = XTRACEMONITOR_LAYOUT_GAP;
  const double panels_x = XTRACEMONITOR_LAYOUT_PANELS_X;
  const double valor_top = XTRACEMONITOR_LAYOUT_VALOR_TOP;
  const double valor_h  = XTRACEMONITOR_LAYOUT_VALOR_H;
  const double foot_h   = XTRACEMONITOR_LAYOUT_FOOT_H;
  double panel_w = W - margin - panels_x;
  if(panel_w < 1.0) panel_w = 1.0;

  double foot_y      = H - margin;
  double console_bot = foot_y - foot_h - gap;
  double valor_y     = valor_top + valor_h;
  double console_top = valor_y + gap;
  double console_h   = console_bot - console_top;
  if(console_h < 1.0) console_h = 1.0;

  
  if((opstatus_foot_y > 0.0) && (foot_y != opstatus_foot_y) && (opstatus_last_baseline > 0.0))
    {
      opstatus_last_baseline += (foot_y - opstatus_foot_y);
    }

  opstatus_foot_y = foot_y;

  double tree_h = console_bot - valor_top;
  if(tree_h < 1.0) tree_h = 1.0;

  if(treeelement && treeelement->GetBoundaryLine())
    {
      treeelement->GetBoundaryLine()->height = tree_h;
      treeelement->GetBoundaryLine()->y      = console_bot;
      treeelement->SetIntrinsicHeight(tree_h);
      treeelement->SetYPosition(console_bot);
    }

  if(valorelement && valorelement->GetBoundaryLine())
    {
      valorelement->GetBoundaryLine()->width  = panel_w;
      valorelement->GetBoundaryLine()->height = valor_h;
      valorelement->GetBoundaryLine()->y      = valor_y;
      valorelement->SetIntrinsicWidth(panel_w);
      valorelement->SetIntrinsicHeight(valor_h);
      valorelement->SetYPosition(valor_y);
    }

  if(consoleelement && consoleelement->GetBoundaryLine())
    {
      consoleelement->GetBoundaryLine()->width  = panel_w;
      consoleelement->GetBoundaryLine()->height = console_h;
      consoleelement->GetBoundaryLine()->y      = console_bot;
      consoleelement->SetIntrinsicWidth(panel_w);
      consoleelement->SetIntrinsicHeight(console_h);
      consoleelement->SetYPosition(console_bot);
    }

  if(opstatus_form && opstatus_form->GetBoundaryLine())
    {
      opstatus_form->GetBoundaryLine()->width = panel_w;
      opstatus_form->GetBoundaryLine()->y     = foot_y;
      opstatus_form->SetIntrinsicWidth(panel_w);
      opstatus_form->SetYPosition(foot_y);

      UI_BOUNDARYLINE* vis = opstatus_form->GetVisibleRect();
      if(vis)
        {
          vis->x     = opstatus_form->GetXPosition();
          vis->y     = foot_y;
          vis->width = panel_w;
        }

      if(opstatus_text && opstatus_text->GetBoundaryLine())
        {
          double text_w = panel_w - (XTRACEMONITOR_LAYOUT_OPSTATUS_TEXT_PAD_X * 2.0);
          if(text_w < 1.0) text_w = 1.0;
          opstatus_text->GetBoundaryLine()->width = text_w;
          opstatus_text->SetIntrinsicWidth(text_w);
        }

      XDWORD visible = 1;
      if(opstatus_expanded && (opstatus_count > 1)) visible = opstatus_count;
      OpStatus_ApplyGeometry(visible);
    }

  layout_applied_w = (int)W;
  layout_applied_h = (int)H;

  if(form_cfg.IsVisible())
    {
      form_cfg.ApplyGeometry();
    }

  if(form_filters.IsVisible())
    {
      form_filters.ApplyGeometry();
    }

  if(!GEN_USERINTERFACE.Element_IsModalActive())
    {
      InternetStatus_ApplyGeometry(W);
    }

  if(!compose) return true;

  GEN_USERINTERFACE.Layout_PutBackground();
  GEN_USERINTERFACE.Elements_SetToRedraw();

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
* @brief      Clears the "valor" property list (no sample rows — filled by live status later).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Valor_Seed()
{
  if(!valorelement) return false;

  valorelement->Clear();
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

  if(GEN_USERINTERFACE.Element_ModalBlocksInteraction(valorelement))
    {
      if(valorelement->IsSplitDragging()) valorelement->Split_EndDrag();
      valor_prevpressed = button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();
      return true;
    }

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
* @brief      Seeds the tree with All, Local (no internet), CFG [origins], and one UART node.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_Seed()
{
  if(!treeelement) return false;

  treeelement->Clear();
  tree_uart_node = NULL;

  UI_ELEMENT_TREE_NODE* all = treeelement->AddNode(NULL, NL("All"), NL("tree_icon_all.png"));
  if(!all) return false;

  treeelement->AddNode(all, NL("# Local (no internet)"), NL("tree_icon_local.png"));

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const int norigins = cfg.GetOriginCount();

  for(int i=0; i<norigins; i++)
    {
      XTRACEMONITOR_CFG_ORIGIN origin;
      if(!cfg.GetOrigin(i, origin)) continue;
      if(origin.name.IsEmpty()) continue;

      XSTRING display_ip = origin.ip;
      if(display_ip.IsEmpty() && !origin.url.IsEmpty())
        {
          DIOIP resolved;
          if(GEN_DIODNSRESOLVER.ResolveURL(origin.url.Get(), resolved))
            {
              resolved.GetXString(display_ip);
            }
        }

      XSTRING label = origin.name;
      if(origin.showip && !display_ip.IsEmpty())
        {
          label.Format(NL("%s (%s)"), origin.name.Get(), display_ip.Get());
        }

     
      XCHAR* icon = NL("tree_icon_ip.png");
      if(!display_ip.IsEmpty())
        {
         
          bool is_local = (display_ip.Find(NL("192.168."), 0, true) == 0) ||
                          (display_ip.Find(NL("10.")     , 0, true) == 0) ||
                          (display_ip.Find(NL("127.")    , 0, true) == 0);
          if(!is_local && (display_ip.Find(NL("172."), 0, true) == 0))
            {            
              XSTRING second;
              int     dot1 = display_ip.Find(NL("."), 0, true);
              int     dot2 = (dot1 >= 0) ? display_ip.Find(NL("."), dot1 + 1, true) : -1;
              if((dot1 >= 0) && (dot2 > dot1))
                {
                  display_ip.Copy(dot1 + 1, dot2, second);
                  int octet = second.ConvertToInt();
                  if((octet >= 16) && (octet <= 31)) is_local = true;
                }
            }
          icon = is_local ? NL("tree_icon_packet.png") : NL("tree_icon_ip.png");
        }

      treeelement->AddNode(all, label.Get(), icon);
    }

  XSTRING uart_label;
  Tree_BuildUARTLabel(uart_label);
  tree_uart_node = treeelement->AddNode(all, uart_label.Get(), NL("tree_icon_uart.png"));

  treeelement->Expand(all);
  treeelement->SetSelected(all);

  GEN_USERINTERFACE.Elements_SetToRedraw(treeelement);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Tree_BuildUARTLabel(XSTRING& out)
* @brief      Builds the single UART tree label from the active uartport vs [target] type UART.
* @ingroup    UTILITIES
*
* @param[out] out : "UART" or "UART <TargetName>" when the serial line matches a Target.
*
* @return     bool : true always (out is always filled).
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_BuildUARTLabel(XSTRING& out)
{
  out = NL("UART");

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  XSTRING* uart = cfg.GetUARTPort();
  if(!uart || uart->IsEmpty()) return true;

  const int ntargets = cfg.GetTargetCount();
  for(int i=0; i<ntargets; i++)
    {
      XTRACEMONITOR_CFG_TARGET target;

      if(!cfg.GetTarget(i, target)) continue;
      if(target.type != XTRACEMONITOR_CFG_TARGETTYPE_UART) continue;
      if(target.name.IsEmpty() || target.uartconfig.IsEmpty()) continue;
     
      if(uart->Compare(target.uartconfig.Get()) != 0) continue;

      out.Format(NL("UART %s"), target.name.Get());
      return true;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Tree_UpdateUARTLabel()
* @brief      Refreshes the single UART tree node label after uartport / Target changes.
* @ingroup    UTILITIES
*
* @return     bool : true if the node was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Tree_UpdateUARTLabel()
{
  if(!treeelement || !tree_uart_node) return false;

  XSTRING label;
  Tree_BuildUARTLabel(label);
  if(!tree_uart_node->text.Compare(label.Get())) return true;

  tree_uart_node->text.Set(label.Get());
  GEN_USERINTERFACE.Elements_SetToRedraw(treeelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::BrandMonitor_Resolve()
* @brief      Resolves the layout brand_monitor image element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::BrandMonitor_Resolve()
{
  brand_monitor = (UI_ELEMENT_IMAGE*)GEN_USERINTERFACE.Element_Get(NL("brand_monitor"), UI_ELEMENT_TYPE_IMAGE);
  // Optional art (removed from main layout); soft-fail so Ini continues.
  if(!brand_monitor) return true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::BrandMonitor_RepaintAfterChrome()
* @brief      Re-blits brand_monitor after chrome so caption-button rebuild cannot crop its top.
* @ingroup    UTILITIES
*
* @return     bool : true if the brand was repainted; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::BrandMonitor_RepaintAfterChrome()
{
  if(!brand_monitor || !brand_monitor->IsVisible() || !brand_monitor->GetBoundaryLine()) return false;

  UI_LAYOUT* layout = brand_monitor->GetLayout();
  if(!layout || !layout->GetSkin()) return false;
  if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin_canvas = (UI_SKINCANVAS*)layout->GetSkin();
  GRP2DCANVAS*   canvas      = skin_canvas->GetCanvas();
  if(!canvas) return false;

  double x = brand_monitor->GetXPosition();
  double y = brand_monitor->GetYPosition();
  double w = brand_monitor->GetBoundaryLine()->width;
  double h = brand_monitor->GetBoundaryLine()->height;
  if((w < 1.0) || (h < 1.0)) return false;

  double left   = UI_BOUNDARYLINE_EdgeLeft(x, w);
  double top    = UI_BOUNDARYLINE_EdgeTop (y, h);
  double bottom = top + h;
  const double valor_top = XTRACEMONITOR_LAYOUT_VALOR_TOP;
  if(bottom > valor_top) bottom = valor_top;
  if(bottom <= top) return false;

  XBYTE pr = 24, pg = 28, pb = 36;
  if(layout->GetBackground() && layout->GetBackground()->GetColor() &&
     layout->GetBackground()->GetColor()->IsValid())
    {
      UI_COLOR* page = layout->GetBackground()->GetColor();
      pr = (XBYTE)page->GetRed();
      pg = (XBYTE)page->GetGreen();
      pb = (XBYTE)page->GetBlue();
    }

  GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
  GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);

  canvas->SetFillColor(&pagecol);
  canvas->SetLineColor(&noline);
  canvas->SetLineWidth(1.0f);
  canvas->Rectangle(left, bottom, left + w, top, true);

  brand_monitor->SetMustReDraw(true);
  return skin_canvas->Draw(brand_monitor);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::InternetStatus_Resolve()
* @brief      Resolves the top-right net-status LED and text elements.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::InternetStatus_Resolve()
{
  netstatus_form  = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("netstatus")      , UI_ELEMENT_TYPE_FORM);
  netstatus_led   = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("netstatus_led")  , UI_ELEMENT_TYPE_FORM);
  netstatus_state = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("netstatus_state"), UI_ELEMENT_TYPE_TEXT);
  netstatus_ip    = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("netstatus_ip")   , UI_ELEMENT_TYPE_TEXT);
  netstatus_udp   = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("netstatus_udp")  , UI_ELEMENT_TYPE_TEXT);

  if(!netstatus_form || !netstatus_led || !netstatus_state || !netstatus_ip || !netstatus_udp) return false;

  // Compact fixed box — never show scroll chrome even if form default overflow is AUTO.
  netstatus_form->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, UI_OVERFLOW_HIDDEN);
  netstatus_form->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  netstatus_form->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  netstatus_form->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  netstatus_form->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  netstatus_form->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  netstatus_form->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);

  // Same chrome as valor/console/tree (XML: roundrect 4, frame_line, console_back).
  netstatus_form->SetRoundRect(4);
  netstatus_form->SetBorderWidth(1.0);
  if(netstatus_form->GetBorderColor())
    {
      netstatus_form->SetBorderColorFromString(NL("70,80,95,255"));
    }
  if(netstatus_form->GetBackgroundColor())
    {
      netstatus_form->GetBackgroundColor()->SetFromString(NL("12,14,18,255"));
      netstatus_form->SetBackgroundColorSet(true);
    }

  // Opaque console_back plate under each line: Draw_Text self-clears glyphs; matches the frame fill.
  auto set_ink_and_plate = [](UI_ELEMENT_TEXT* el)
    {
      if(!el) return;
      if(el->GetColor())
        {
          el->GetColor()->SetRed(220);
          el->GetColor()->SetGreen(226);
          el->GetColor()->SetBlue(235);
          el->GetColor()->SetAlpha(255);
          el->GetColor()->SetValid(true);
        }
      if(el->GetBackgroundColor())
        {
          el->GetBackgroundColor()->SetRed(12);
          el->GetBackgroundColor()->SetGreen(14);
          el->GetBackgroundColor()->SetBlue(18);
          el->GetBackgroundColor()->SetAlpha(255);
          el->GetBackgroundColor()->SetValid(true);
          el->SetBackgroundColorSet(true);
        }
    };
  set_ink_and_plate(netstatus_state);
  set_ink_and_plate(netstatus_ip);
  set_ink_and_plate(netstatus_udp);

  // Opaque LED fill from the start — tiny forms without fill are page-wiped as "legend strips".
  if(netstatus_led->GetBackgroundColor())
    {
      netstatus_led->GetBackgroundColor()->SetRed(120);
      netstatus_led->GetBackgroundColor()->SetGreen(130);
      netstatus_led->GetBackgroundColor()->SetBlue(140);
      netstatus_led->GetBackgroundColor()->SetAlpha(255);
      netstatus_led->GetBackgroundColor()->SetValid(true);
      netstatus_led->SetBackgroundColorSet(true);
    }
  netstatus_led->SetVisible(true);
  netstatus_prev_valid = false;

  if(netstatus_timer) netstatus_timer->Reset();

  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  // GEN already polls connection / public IP on APPFLOW_EXTENDED's background path —
  // tighten cadence a bit so the LED tracks sooner than the default 30s / 3600s.
  {
    APPFLOWEXTENDED_INTERNETSTATUS* inet = APPFLOW_EXTENDED.GetInternetStatus();
    APPFLOWINTERNETSERVICES*        svc  = inet ? inet->GetInternetServices() : NULL;
    if(svc)
      {
        svc->ChangeCadenceCheck(APPFLOWINTERNETSERVICES_TASKID_CHECKCONNECTIONINTERNET, 5, true);
        svc->ChangeCadenceCheck(APPFLOWINTERNETSERVICES_TASKID_GETIPS, 30, true);
      }
  }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::InternetStatus_ApplyGeometry(double canvas_w)
* @brief      Right-aligns the net-status cluster under the caption on the live canvas width.
* @ingroup    UTILITIES
*
* @param[in]  canvas_w : Live canvas width in pixels.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::InternetStatus_WipeCluster(double left, double top, double right, double bottom)
{
  if((right <= left) || (bottom <= top)) return false;

  UI_LAYOUT* layout = netstatus_state ? netstatus_state->GetLayout() : NULL;
  if(!layout || !layout->GetSkin() || layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin   = (UI_SKINCANVAS*)layout->GetSkin();
  GRP2DCANVAS*   canvas = skin->GetCanvas();
  if(!canvas) return false;

  XBYTE pr = 24, pg = 28, pb = 36;
  if(layout->GetBackground() && layout->GetBackground()->GetColor() &&
     layout->GetBackground()->GetColor()->IsValid())
    {
      UI_COLOR* page = layout->GetBackground()->GetColor();
      pr = (XBYTE)page->GetRed();
      pg = (XBYTE)page->GetGreen();
      pb = (XBYTE)page->GetBlue();
    }

  // Opaque page wipe every DrawFrame (after chrome). Use AGG Rectangle — PutPixel over the
  // ~280×70 cluster alone was tens of thousands of copy_pixel calls per frame at idle.
  GRP2DCOLOR_RGBA8 pagecol(pr, pg, pb, 255);
  GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);
  canvas->SetFillColor(&pagecol);
  canvas->SetLineColor(&noline);
  canvas->SetLineWidth(1.0f);
  canvas->Rectangle(left, bottom, right, top, true);
  return true;
}


bool XTRACEMONITOR::InternetStatus_PaintCluster()
{
  if(!netstatus_led || !netstatus_state || !netstatus_ip || !netstatus_udp) return false;

  UI_LAYOUT* layout = netstatus_state->GetLayout();
  if(!layout || !layout->GetSkin() || layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) return false;

  UI_SKINCANVAS* skin = (UI_SKINCANVAS*)layout->GetSkin();

  // After an opaque page wipe, drop TextBackdrop WITHOUT restore so Draw_Text does not
  // PutBitmapNoAlpha a stale (glyph-baked) snapshot over the clean page, then re-captures.
  skin->DiscardTextBackdrop(netstatus_state);
  skin->DiscardTextBackdrop(netstatus_ip);
  skin->DiscardTextBackdrop(netstatus_udp);

  if(netstatus_form) netstatus_form->SetMustReDraw(true);
  netstatus_led->SetMustReDraw(true);
  netstatus_state->SetMustReDraw(true);
  netstatus_ip->SetMustReDraw(true);
  netstatus_udp->SetMustReDraw(true);

  // Frame first (same Draw_Form chrome as valor), then LED + lines on top.
  if(netstatus_form) skin->Draw(netstatus_form);
  skin->Draw(netstatus_led);
  skin->Draw(netstatus_state);
  skin->Draw(netstatus_ip);
  skin->Draw(netstatus_udp);
  return true;
}


bool XTRACEMONITOR::InternetStatus_RepaintAfterChrome()
{
  if(!netstatus_led || !netstatus_state || !netstatus_ip || !netstatus_udp) return false;
  if(!netstatus_prev_valid) return false;
  if(GEN_USERINTERFACE.Element_IsModalActive()) return false;

  // Same contract as BrandMonitor_RepaintAfterChrome: chrome's right-button rebuild can
  // page-wipe this AABB after content Update, which both hid the LED and left partial
  // glyph ink that the next latency redraw alpha-stacked on top of.
  if(!InternetStatus_WipeCluster(netstatus_prev_l, netstatus_prev_t, netstatus_prev_r, netstatus_prev_b))
    return false;
  return InternetStatus_PaintCluster();
}


bool XTRACEMONITOR::InternetStatus_ApplyGeometry(double canvas_w)
{
  if(!netstatus_led || !netstatus_state || !netstatus_ip || !netstatus_udp) return false;
  if(!netstatus_led->GetBoundaryLine() || !netstatus_state->GetBoundaryLine() ||
     !netstatus_ip->GetBoundaryLine() || !netstatus_udp->GetBoundaryLine()) return false;

  // Frame right edge = valor/console right edge (canvas_w - LAYOUT_MARGIN).
  const double margin    = XTRACEMONITOR_LAYOUT_MARGIN;
  const double led       = XTRACEMONITOR_LAYOUT_NETSTATUS_LED;
  const double line_h    = XTRACEMONITOR_LAYOUT_NETSTATUS_LINE_H;
  const double line_gap  = XTRACEMONITOR_LAYOUT_NETSTATUS_LINE_GAP;
  const double led_gap   = XTRACEMONITOR_LAYOUT_NETSTATUS_LED_GAP;
  const double text_pad  = XTRACEMONITOR_LAYOUT_NETSTATUS_TEXT_PAD;
  const double frame_pad = XTRACEMONITOR_LAYOUT_NETSTATUS_FRAME_PAD;

  UI_SKINCANVAS* skin = NULL;
  UI_LAYOUT* layout = netstatus_state->GetLayout();
  if(layout && layout->GetSkin() && layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
    {
      skin = (UI_SKINCANVAS*)layout->GetSkin();
    }

  auto measure = [&](UI_ELEMENT_TEXT* el) -> double
    {
      if(!el || !el->GetText() || el->GetText()->IsEmpty()) return 120.0;
      XDWORD sf = el->GetSizeFont();
      if(sf < 1) sf = 12;
      if(!skin) return (double)el->GetText()->GetSize() * (double)sf * 0.55;
      double tw = skin->GetWidthString(el->GetText()->Get(), sf);
      if(tw < 1.0) tw = 120.0;
      return tw;
    };

  double tw_state = measure(netstatus_state);
  double tw_ip    = measure(netstatus_ip);
  double tw_udp   = measure(netstatus_udp);
  double text_w   = tw_state;
  if(tw_ip  > text_w) text_w = tw_ip;
  if(tw_udp > text_w) text_w = tw_udp;
  text_w += text_pad;

  const double content_w = led + led_gap + text_w;
  const double content_h = (line_h * 3.0) + (line_gap * 2.0);
  const double frame_w   = content_w + (frame_pad * 2.0);
  const double frame_h   = content_h + (frame_pad * 2.0);

  double frame_right = canvas_w - margin;
  if(frame_right < margin + frame_w) frame_right = margin + frame_w;
  const double frame_x = frame_right - frame_w;

  // Same vertical gap as valor↔console (LAYOUT_GAP): frame bottom sits GAP above valor top.
  const double gap          = XTRACEMONITOR_LAYOUT_GAP;
  const double frame_bottom = XTRACEMONITOR_LAYOUT_VALOR_TOP - gap;
  const double frame_top    = frame_bottom - frame_h;
  const double content_top  = frame_top + frame_pad;
  const double state_y      = content_top + line_h;
  const double ip_y         = state_y + line_h + line_gap;
  const double udp_y        = ip_y + line_h + line_gap;
  const double led_y        = state_y - ((line_h - led) * 0.5);

  const double led_x  = frame_x + frame_pad;
  const double text_x = led_x + led + led_gap;

  // Wipe previous/new frame AABB (+1 for border AA).
  const double pad = 2.0;
  const double cluster_l = frame_x - pad;
  const double cluster_t = frame_top - pad;
  const double cluster_r = frame_right + pad;
  const double cluster_b = frame_bottom + pad;

  if(netstatus_prev_valid)
    {
      InternetStatus_WipeCluster(netstatus_prev_l, netstatus_prev_t, netstatus_prev_r, netstatus_prev_b);
    }
  InternetStatus_WipeCluster(cluster_l, cluster_t, cluster_r, cluster_b);

  auto place = [](UI_ELEMENT* el, double abs_x, double abs_y, double cw, double ch)
    {
      if(!el || !el->GetBoundaryLine()) return;
      el->GetBoundaryLine()->x      = abs_x;
      el->GetBoundaryLine()->y      = abs_y;
      el->GetBoundaryLine()->width  = cw;
      el->GetBoundaryLine()->height = ch;
      el->SetIntrinsicWidth(cw);
      el->SetIntrinsicHeight(ch);
      el->SetXPosition(abs_x);
      el->SetYPosition(abs_y);
      if(el->GetType() == UI_ELEMENT_TYPE_FORM)
        {
          UI_BOUNDARYLINE* vis = ((UI_ELEMENT_FORM*)el)->GetVisibleRect();
          if(vis)
            {
              vis->x = abs_x; vis->y = abs_y; vis->width = cw; vis->height = ch;
            }
        }
    };

  if(netstatus_form && netstatus_form->GetBoundaryLine())
    {
      place((UI_ELEMENT*)netstatus_form, frame_x, frame_bottom, frame_w, frame_h);
    }

  place((UI_ELEMENT*)netstatus_led  , led_x , led_y  , led   , led);
  place((UI_ELEMENT*)netstatus_state, text_x, state_y, text_w, line_h);
  place((UI_ELEMENT*)netstatus_ip   , text_x, ip_y   , text_w, line_h);
  place((UI_ELEMENT*)netstatus_udp  , text_x, udp_y  , text_w, line_h);

  if(netstatus_state) netstatus_state->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_LEFT);
  if(netstatus_ip)    netstatus_ip->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_LEFT);
  if(netstatus_udp)   netstatus_udp->SetTextAlign(UI_ELEMENT_TYPE_ALIGN_LEFT);

  // LED must stay an opaque "paintable" form (h<=28 & w<=120): otherwise RepaintCompactLegendForms
  // treats it as a toolbar legend strip and page-wipes it every frame.
  if(netstatus_led->GetBackgroundColor())
    {
      const bool online = netstatus_haveinternet;
      netstatus_led->GetBackgroundColor()->SetRed  (online ? 63  : 248);
      netstatus_led->GetBackgroundColor()->SetGreen(online ? 185 : 81);
      netstatus_led->GetBackgroundColor()->SetBlue (online ? 80  : 73);
      netstatus_led->GetBackgroundColor()->SetAlpha(255);
      netstatus_led->GetBackgroundColor()->SetValid(true);
      netstatus_led->SetBackgroundColorSet(true);
    }
  if(netstatus_led->GetColor())
    {
      netstatus_led->GetBackgroundColor()->CopyTo(netstatus_led->GetColor());
      netstatus_led->GetColor()->SetValid(true);
    }
  netstatus_led->SetVisible(true);

  netstatus_prev_l = cluster_l;
  netstatus_prev_t = cluster_t;
  netstatus_prev_r = cluster_r;
  netstatus_prev_b = cluster_b;
  netstatus_prev_valid = true;

  InternetStatus_PaintCluster();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::InternetStatus_Update(bool force)
* @brief      Paints the net LED from APPFLOW_EXTENDED InternetStatus (GEN-updated) + UDP/target CFG.
* @ingroup    UTILITIES
*
* @param[in]  force : When true, bypass the UI poll timer.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::InternetStatus_Update(bool force)
{
  if(!netstatus_led || !netstatus_state || !netstatus_ip || !netstatus_udp) return false;

  #ifndef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  return false;
  #else

  // Never paint/remasure under an active modal (Draw_Text TextBackdrop path is disabled).
  if(!force && GEN_USERINTERFACE.Element_IsModalActive()) return true;

  if(!force && netstatus_timer &&
     (netstatus_timer->GetMeasureMilliSeconds() < (XDWORD)XTRACEMONITOR_NETSTATUS_POLL_MS))
    {
      return true;
    }

  if(netstatus_timer) netstatus_timer->Reset();

  APPFLOWEXTENDED_INTERNETSTATUS* inet = APPFLOW_EXTENDED.GetInternetStatus();
  APPFLOWINTERNETSERVICES*        svc  = inet ? inet->GetInternetServices() : NULL;
  if(!svc) return false;

  // Live values from GEN InternetServices (scheduler updates connection/latency/public IP off-UI).
  const bool   online   = svc->HaveInternetConnection();
  const XDWORD latency  = svc->GetInternetLatency();
  XSTRING*     publicip = svc->GetPublicIP();

  const bool online_changed = (online != netstatus_haveinternet);
  netstatus_haveinternet = online;

  // LED colour is the connection state; label stays "Conexion a internet" (+ latency when online).
  XSTRING state_line;
  if(online)
    {
      state_line.Format(_L("Conexion a internet · %u ms"), latency);
    }
   else
    {
      state_line.Set(_L("Conexion a internet"));
    }

  XSTRING ip_line;
  if(online && publicip && !publicip->IsEmpty())
    {
      ip_line.Format(_L("IP Publica: %s"), publicip->Get());
    }
   else
    {
      ip_line.Set(_L("IP Publica: --"));
    }

  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  const int udp_port = cfg.ResolveUDPListenPort(cfg.GetUDPListenPort());
  XSTRING target_name;
  const int ntargets = cfg.GetTargetCount();
  for(int i = 0; i < ntargets; i++)
    {
      XTRACEMONITOR_CFG_TARGET target;
      if(!cfg.GetTarget(i, target)) continue;
      if(target.type != XTRACEMONITOR_CFG_TARGETTYPE_UDP) continue;
      if(target.port != udp_port) continue;
      target_name = target.name;
      break;
    }

  XSTRING udp_line;
  if(!target_name.IsEmpty())
    {
      udp_line.Format(_L("UDP Port: %d · %s"), udp_port, target_name.Get());
    }
   else
    {
      udp_line.Format(_L("UDP Port: %d"), udp_port);
    }

  UI_SKINCANVAS* skin = NULL;
  UI_LAYOUT* layout = netstatus_state->GetLayout();
  if(layout && layout->GetSkin() && layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
    skin = (UI_SKINCANVAS*)layout->GetSkin();

  // XSTRING::Compare is strcmp-style: 0 = equal. Skip only when unchanged.
  auto apply_line = [&](UI_ELEMENT_TEXT* el, XSTRING& last, const XSTRING& now, bool force_set) -> bool
    {
      if(!el || !el->GetText()) return false;
      if(!force_set && !last.Compare(now.Get())) return false;
      // Restore TextBackdrop (if any) before swapping the string — GEN Draw_Text alpha-blends.
      if(skin) skin->PrepareTextContentChange(el);
      el->GetText()->Set(now.Get());
      last = now;
      el->SetMustReDraw(true);
      return true;
    };

  bool any = false;
  any |= apply_line(netstatus_state, netstatus_last_state, state_line, force);
  any |= apply_line(netstatus_ip   , netstatus_last_ip   , ip_line   , force);
  any |= apply_line(netstatus_udp  , netstatus_last_udp  , udp_line  , force);

  // LED colour via components (avoid CSV alpha ambiguity). Always refresh when online flips or forced.
  if((online_changed || force || any) && netstatus_led->GetBackgroundColor())
    {
      netstatus_led->GetBackgroundColor()->SetRed  (online ? 63  : 248);
      netstatus_led->GetBackgroundColor()->SetGreen(online ? 185 : 81);
      netstatus_led->GetBackgroundColor()->SetBlue (online ? 80  : 73);
      netstatus_led->GetBackgroundColor()->SetAlpha(255);
      netstatus_led->GetBackgroundColor()->SetValid(true);
      netstatus_led->SetBackgroundColorSet(true);
      if(netstatus_led->GetColor())
        {
          netstatus_led->GetColor()->SetRed  (online ? 63  : 248);
          netstatus_led->GetColor()->SetGreen(online ? 185 : 81);
          netstatus_led->GetColor()->SetBlue (online ? 80  : 73);
          netstatus_led->GetColor()->SetAlpha(255);
          netstatus_led->GetColor()->SetValid(true);
        }
    }

  // Content and/or LED change → remasure + right-pack (UDP/latency/IP widths differ) then repaint.
  if(any || online_changed || force)
    {
      if(GetMainScreen())
        {
          InternetStatus_ApplyGeometry((double)GetMainScreen()->GetWidth());
        }
       else
        {
          InternetStatus_PaintCluster();
        }
    }

  return true;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::OpStatus_Resolve()
* @brief      Resolves the opstatus foot form and textbox, and seeds their geometry.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::OpStatus_Resolve()
{
  opstatus_form = (UI_ELEMENT_FORM*)GEN_USERINTERFACE.Element_Get(NL("opstatus"), UI_ELEMENT_TYPE_FORM);
  opstatus_text = (UI_ELEMENT_TEXTBOX*)GEN_USERINTERFACE.Element_Get(NL("opstatus_text"), UI_ELEMENT_TYPE_TEXTBOX);
  if(!opstatus_form || !opstatus_text) return false;

  if(!opstatus_click_timer)
    {
      opstatus_click_timer = GEN_XFACTORY.CreateTimer();
      if(!opstatus_click_timer) return false;
    }

  opstatus_click_timer->Reset();

  // Lock the foot bottom edge (grows upward from this Y).
  opstatus_foot_y = opstatus_form->GetYPosition();

  // Foot never scrolls (form/textbox default overflow is AUTO and XML overflow is not
  // applied to these element types at load time).
  OpStatus_DisableScroll();

  // Ensure the foot form paints above the console when expanded.
  GEN_USERINTERFACE.Element_PutToLastPositionLayout(opstatus_form);

  // Baseline is frozen on first real ContentHeight (see ApplyGeometry), not here.
  opstatus_last_baseline = 0.0f;
  opstatus_line_h        = 0.0f;

  return OpStatus_Refresh();
}


bool XTRACEMONITOR::OpStatus_Log(XCHAR* message)
{
  if(!message) return false;

  if(opstatus_count < XTRACEMONITOR_OPSTATUS_MAXLINES)
    {
      opstatus_msgs[opstatus_count].Set(message);
      opstatus_count++;
    }
   else
    {
      for(XDWORD c=1; c<XTRACEMONITOR_OPSTATUS_MAXLINES; c++)
        {
          opstatus_msgs[c-1] = opstatus_msgs[c];
        }
      opstatus_msgs[XTRACEMONITOR_OPSTATUS_MAXLINES-1].Set(message);
      opstatus_count = XTRACEMONITOR_OPSTATUS_MAXLINES;
    }

  return OpStatus_Refresh();
}


bool XTRACEMONITOR::OpStatus_Refresh()
{
  if(!opstatus_form || !opstatus_text) return false;

  XDWORD visible = 1;
  if(opstatus_expanded && (opstatus_count > 1))
    {
      visible = opstatus_count;
      if(visible > XTRACEMONITOR_OPSTATUS_MAXLINES) visible = XTRACEMONITOR_OPSTATUS_MAXLINES;
      if(visible < 1) visible = 1;
    }

  XSTRING body;
  if(opstatus_count == 0)
    {
      body.Empty();
    }
   else
    {
      XDWORD start = opstatus_count - visible;
      for(XDWORD c=start; c<opstatus_count; c++)
        {
          if(c > start) body.Add(NL("\n"));
          body.Add(opstatus_msgs[c].Get());
        }
    }

  opstatus_text->GetText()->Set(body.Get());

  OpStatus_ApplyGeometry(visible);

  if(opstatus_expanded && (opstatus_count > 1)) GEN_USERINTERFACE.Element_PutToLastPositionLayout(opstatus_form);

  GEN_USERINTERFACE.Elements_SetToRedraw(opstatus_form);
  if(consoleelement) GEN_USERINTERFACE.Elements_SetToRedraw(consoleelement);

  return true;
}


bool XTRACEMONITOR::OpStatus_DisableScroll()
{
  if(!opstatus_form || !opstatus_text) return false;

  opstatus_form->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, UI_OVERFLOW_HIDDEN);
  opstatus_form->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_form->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  opstatus_form->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_form->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  opstatus_form->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_form->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);

  opstatus_text->Scroll_SetOverflow(UI_PROPERTY_SCROLLEABLE_TYPE_ALL, UI_OVERFLOW_HIDDEN);
  opstatus_text->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_text->Scroll_SetActivate(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  opstatus_text->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_text->Scroll_SetBarShown(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);
  opstatus_text->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_HORIZONTAL, false);
  opstatus_text->Scroll_SetVisible(UI_PROPERTY_SCROLLEABLE_TYPE_VERTICAL  , false);

  return true;
}


bool XTRACEMONITOR::OpStatus_ApplyGeometry(XDWORD visible_lines)
{
  if(!opstatus_form || !opstatus_text) return false;

  OpStatus_DisableScroll();

  // Collapsed always shows exactly one line; ignore stale expanded counts.
  XDWORD visible = visible_lines;
  if(!opstatus_expanded) visible = 1;
  if(visible < 1) visible = 1;
  if(visible > XTRACEMONITOR_OPSTATUS_MAXLINES) visible = XTRACEMONITOR_OPSTATUS_MAXLINES;
  if(visible > opstatus_count && opstatus_count > 0) visible = opstatus_count;

  double foot_y = opstatus_foot_y;
  if(foot_y <= 0.0f) foot_y = opstatus_form->GetYPosition();

  double spacing  = (double)opstatus_text->GetLineSpacing();
  double sizefont = (double)opstatus_text->GetSizeFont();
  if(sizefont < 1.0f) sizefont = 12.0f;

  // Lock TextBox lineheight once from a single-line draw. Never overwrite afterwards
  // (multi-line content_h used to inflate line_h and prevent collapse).
  double content_h = opstatus_text->GetContentHeight();

  double collapsed_h_est = XTRACEMONITOR_OPSTATUS_PAD + XTRACEMONITOR_OPSTATUS_LINEHEIGHT;
  double temp_baseline   = foot_y - (collapsed_h_est * 0.5f) + (sizefont * 0.5f);

  if((opstatus_line_h < 1.0f) && (visible == 1) && (content_h > 1.0f))
    {
      // Preserve the pixel position of the last line while switching from the sizefont
      // estimate to the real TextBox lineheight (font_a + spacing).
      double prev_baseline = (opstatus_last_baseline > 0.0f) ? opstatus_last_baseline : temp_baseline;
      double font_a        = content_h - spacing;
      if(font_a < 1.0f) font_a = sizefont;
      // With estimate text_h=sizefont+spacing, last ≈ prev_baseline - sizefont + font_a.
      opstatus_last_baseline = prev_baseline - sizefont + font_a;
      opstatus_line_h        = content_h;
    }

  double line_h = opstatus_line_h;
  if(line_h < 1.0f) line_h = sizefont + spacing;

  double collapsed_h = collapsed_h_est;
  if(collapsed_h < (line_h + XTRACEMONITOR_OPSTATUS_PAD)) collapsed_h = line_h + XTRACEMONITOR_OPSTATUS_PAD;

  if(opstatus_last_baseline <= 0.0f) opstatus_last_baseline = temp_baseline;
  double last_baseline = opstatus_last_baseline;

  // TextBox: content_h = n*lineheight => last baseline = text_y - spacing (stable for any n).
  double text_h = (double)visible * line_h;
  if(text_h < line_h) text_h = line_h;
  double text_y = last_baseline + spacing;

  // Local Y constant (CalculePosition: abs = father_y - boundaryline.y). Must not depend on height.
  double text_local_y = foot_y - text_y;
  if(text_local_y < 0.0f) text_local_y = 0.0f;

  double box_top = text_y - text_h;
  double top_pad = (collapsed_h - line_h) * 0.5f;
  if(top_pad < 0.0f) top_pad = 0.0f;

  double new_h = collapsed_h;
  if(visible > 1)
    {
      new_h = (foot_y - box_top) + top_pad;
      if(new_h < collapsed_h) new_h = collapsed_h;
    }

  opstatus_form->GetBoundaryLine()->height = new_h;
  opstatus_form->SetIntrinsicHeight(new_h);
  opstatus_form->SetYPosition(foot_y);

  UI_BOUNDARYLINE* vis = opstatus_form->GetVisibleRect();
  if(vis)
    {
      vis->x      = opstatus_form->GetXPosition();
      vis->y      = foot_y;
      vis->width  = opstatus_form->GetBoundaryLine()->width;
      vis->height = new_h;
    }

  opstatus_text->GetBoundaryLine()->height = text_h;
  opstatus_text->SetIntrinsicHeight(text_h);
  opstatus_text->GetBoundaryLine()->y = text_local_y;
  opstatus_text->SetYPosition(text_y);

  return true;
}


bool XTRACEMONITOR::OpStatus_SetExpanded(bool expanded)
{
  opstatus_expanded = expanded;
  return OpStatus_Refresh();
}


bool XTRACEMONITOR::OpStatus_ToggleExpanded()
{
  return OpStatus_SetExpanded(!opstatus_expanded);
}


bool XTRACEMONITOR::OpStatus_HitTest(int screen_x, int screen_y)
{
  if(!opstatus_form || !opstatus_form->GetBoundaryLine()) return false;

  double x = opstatus_form->GetXPosition();
  double y = (opstatus_foot_y > 0.0f) ? opstatus_foot_y : opstatus_form->GetYPosition();
  double w = opstatus_form->GetBoundaryLine()->width;
  double h = opstatus_form->GetBoundaryLine()->height;
  double top = y - h;

  if((double)screen_x < x || (double)screen_x >= (x + w)) return false;
  if((double)screen_y < top || (double)screen_y >= y) return false;

  return true;
}


bool XTRACEMONITOR::OpStatus_OnClick()
{
  if(!opstatus_click_timer) return false;

  XQWORD elapsed = opstatus_click_timer->GetMeasureMilliSeconds();

  if(opstatus_click_armed && (elapsed <= XTRACEMONITOR_OPSTATUS_DBLCLICK_MS))
    {
      OpStatus_ToggleExpanded();
      opstatus_click_armed = false;
    }
   else
    {
      opstatus_click_armed = true;
    }

  opstatus_click_timer->Reset();

  return true;
}


bool XTRACEMONITOR::OpStatus_UpdateInput()
{
  // Double-click is handled on IsPressedWithRelease in UpdateInput (same edge as SELECCTION).
  // Keep this hook for symmetry with tree/valor input helpers.
  return (opstatus_form != NULL);
}


bool XTRACEMONITOR::Tree_UpdateInput()
{
  if(!treeelement || !cursor || !button[XTRACEMONITOR_BUTTON_MOUSE]) return false;

  bool pressed = button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();

  // Act only on the press transition.
  if(pressed && !tree_prevpressed)
    {
      // GEN modal form owns the pointer: content under (tree, etc.) must not select while it is up.
      // Redraw/update of the tree may continue — only interaction is blocked.
      if(GEN_USERINTERFACE.Element_ModalBlocksInteraction(treeelement))
        {
          tree_prevpressed = pressed;
          return true;
        }

      int    cursor_x = (int)cursor->GetX();
      int    cursor_y = (int)GetMainScreen()->GetHeight() - (int)cursor->GetY();
      double local_x  = 0.0f;
      double local_y  = 0.0f;

      // Foot form owns the click when the pointer is over it (including expanded overlap).
      if(OpStatus_HitTest(cursor_x, cursor_y))
        {
          tree_prevpressed = pressed;
          return true;
        }

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
* @fn         bool XTRACEMONITOR::Toolbar_SlotSetVisible(UI_ELEMENT* element, bool visible)
* @brief      Shows or hides a shared-slot toolbar control without fade transition (avoids Stop/Run ghosts).
* @ingroup    UTILITIES
*
* @param[in]  element : Element to show or hide.
* @param[in]  visible : True to show; false to hide and restore the slot background.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_SlotSetVisible(UI_ELEMENT* element, bool visible, XCHAR* legend_element_name)
{
  if(!element) return false;

  // Already in the desired state: leave the slot alone (re-redrawing the visible sibling
  // made the shared legend TextBackdrop re-capture the icon rim as a gray plate).
  if(element->IsVisible() == visible) return true;

  element->SetVisible(visible);

  // Same idiom as UI_System card swap: drop TOSHOW/TOHIDE so alpha fade cannot leave the sibling icon.
  element->SetTransitionStateShow(UI_ELEMENT_TRANSITION_STATE_SHOW_NONE);

  if(!visible)
    {
      // MultiOption has no RestoreOnHide cache (only FORM/MENU do). Layout also has no background
      // bitmap here, so EraseElementWithLayoutBackground is a no-op — paint the page fill over the
      // shared slot AABB or alpha Stop/Run stack (green play over leftover red STOP).
      UI_LAYOUT* layout = element->GetLayout();
      if(layout && layout->GetSkin() && layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
        {
          UI_SKINCANVAS* skin   = (UI_SKINCANVAS*)layout->GetSkin();
          GRP2DCANVAS*   canvas = skin->GetCanvas();

          XVECTOR<UI_ELEMENT*>* compose = element->GetComposeElements();
          if(compose)
            {
              for(XDWORD c=0; c<compose->GetSize(); c++)
                {
                  UI_ELEMENT* option = compose->Get(c);
                  if(!option) continue;

                  XVECTOR<UI_ELEMENT*>* kids = option->GetComposeElements();
                  if(!kids) continue;

                  for(XDWORD k=0; k<kids->GetSize(); k++)
                    {
                      UI_ELEMENT* kid = kids->Get(k);
                      if(kid && kid->GetType() == UI_ELEMENT_TYPE_ANIMATION)
                        {
                          skin->DiscardAnimBackdrop(kid);
                          ((UI_ELEMENT_ANIMATION*)kid)->Stop();
                        }
                      if(kid && kid->GetType() == UI_ELEMENT_TYPE_TEXT)      skin->DiscardTextBackdrop(kid);
                    }
                }
            }

          // Do NOT EraseElementRebuildArea here: its pad intersects runstop-lbl and deletes the
          // legend TextBackdrop without restore (leaves the gray rim plate under "Parar").

          if(canvas && element->GetBoundaryLine())
            {
              double x = element->GetXPosition();
              double y = element->GetYPosition();
              double w = element->GetBoundaryLine()->width;
              double h = element->GetBoundaryLine()->height;
              const double pad = (double)UI_SKINCANVAS_PRESELECT_MAXEDGE;
              double left   = UI_BOUNDARYLINE_EdgeLeft  (x, w) - pad;
              double right  = UI_BOUNDARYLINE_EdgeRight (x, w) + pad;
              double top    = UI_BOUNDARYLINE_EdgeTop   (y, h) - pad;
              double bottom = UI_BOUNDARYLINE_EdgeBottom(y, h) + pad;

              // Cap wipe at the shared legend top so page fill never paints through the caption.
              XCHAR* legend_name = legend_element_name ? legend_element_name : NL("runstop-lbl");
              UI_ELEMENT* legend = GEN_USERINTERFACE.Element_Get(legend_name, UI_ELEMENT_TYPE_FORM);
              if(legend && legend->GetBoundaryLine())
                {
                  double legend_top = UI_BOUNDARYLINE_EdgeTop(legend->GetYPosition(), legend->GetBoundaryLine()->height);
                  if(bottom > legend_top) bottom = legend_top;
                }

              if(bottom > top)
                {
                  // Match active layout page background (Dark/Light Color Theme).
                  XBYTE pr = 24, pg = 28, pb = 36, pa = 255;
                  if(layout->GetBackground() && layout->GetBackground()->GetColor() &&
                     layout->GetBackground()->GetColor()->IsValid())
                    {
                      UI_COLOR* page = layout->GetBackground()->GetColor();
                      pr = page->GetRed(); pg = page->GetGreen(); pb = page->GetBlue(); pa = page->GetAlpha();
                    }
                  GRP2DCOLOR_RGBA8 fill(pr, pg, pb, pa);
                  GRP2DCOLOR_RGBA8 noline(0, 0, 0, 0);
                  canvas->SetFillColor(&fill);
                  canvas->SetLineColor(&noline);
                  canvas->SetLineWidth(1.0f);
                  canvas->Rectangle(left, bottom, right, top, true);
                }
            }
        }

      // Already wiped: do not leave MustReDraw (RestoreOnHide is a no-op for MULTIOPTION).
      element->SetMustReDraw(false);
      return true;
    }

  // Restart sprite loops on the face that becomes visible (Filtro_Figuras twin).
  XVECTOR<UI_ELEMENT*>* compose = element->GetComposeElements();
  if(compose)
    {
      for(XDWORD c=0; c<compose->GetSize(); c++)
        {
          UI_ELEMENT* option = compose->Get(c);
          if(!option) continue;
          XVECTOR<UI_ELEMENT*>* kids = option->GetComposeElements();
          if(!kids) continue;
          for(XDWORD k=0; k<kids->GetSize(); k++)
            {
              UI_ELEMENT* kid = kids->Get(k);
              if(kid && kid->GetType() == UI_ELEMENT_TYPE_ANIMATION)
                {
                  UI_ELEMENT_ANIMATION* anim = (UI_ELEMENT_ANIMATION*)kid;
                  if(anim->GetComposeElements() && anim->GetComposeElements()->GetSize() > 1)
                    anim->Play();
                }
            }
        }
    }

  GEN_USERINTERFACE.Elements_SetToRedraw(element, true);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Filters_IsNonDefaultCommitted()
* @brief      True when committed CFG [filters] differs from factory defaults (text on or any level off).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Filters_IsNonDefaultCommitted()
{
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);

  if(cfg.Filters_IsTextActive()) return true;

  const int used = cfg.Filters_GetLevels() & (XTRACEMONITOR_CFG_FILTER_LEVEL_BLACK  |
                                              XTRACEMONITOR_CFG_FILTER_LEVEL_PURPLE |
                                              XTRACEMONITOR_CFG_FILTER_LEVEL_GREEN  |
                                              XTRACEMONITOR_CFG_FILTER_LEVEL_BLUE   |
                                              XTRACEMONITOR_CFG_FILTER_LEVEL_RED    |
                                              XTRACEMONITOR_CFG_FILTER_LEVEL_GRAY);
  const int all  = (XTRACEMONITOR_CFG_FILTER_LEVEL_BLACK  |
                    XTRACEMONITOR_CFG_FILTER_LEVEL_PURPLE |
                    XTRACEMONITOR_CFG_FILTER_LEVEL_GREEN  |
                    XTRACEMONITOR_CFG_FILTER_LEVEL_BLUE   |
                    XTRACEMONITOR_CFG_FILTER_LEVEL_RED    |
                    XTRACEMONITOR_CFG_FILTER_LEVEL_GRAY);

  return (used != all);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Toolbar_ApplyFilterBtnVisibility()
* @brief      Shows static Filtre or animated Filtro_Figuras twin from committed [filters] state.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_ApplyFilterBtnVisibility()
{
  UI_ELEMENT* filter_btn        = GEN_USERINTERFACE.Element_Get(NL("filter-btn")       , UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT* filter_active_btn = GEN_USERINTERFACE.Element_Get(NL("filter-active-btn"), UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT* lbl               = GEN_USERINTERFACE.Element_Get(NL("filter-lbl")       , UI_ELEMENT_TYPE_FORM);

  const bool custom = Filters_IsNonDefaultCommitted();

  if(custom)
    {
      Toolbar_SlotSetVisible(filter_btn       , false, NL("filter-lbl"));
      Toolbar_SlotSetVisible(filter_active_btn, true , NL("filter-lbl"));
    }
   else
    {
      Toolbar_SlotSetVisible(filter_active_btn, false, NL("filter-lbl"));
      Toolbar_SlotSetVisible(filter_btn       , true , NL("filter-lbl"));
    }

  if(lbl) lbl->SetVisible(true);

  GEN_USERINTERFACE.ResetPreselect();

  return (filter_btn != NULL) || (filter_active_btn != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Toolbar_SetWorkingMode(bool on)
* @brief      Sets workingmode and refreshes the shared Stop/Run toolbar slot (+ legend).
* @ingroup    UTILITIES
*
* @param[in]  on : True = working (Stop); false = paused (Arrancar).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_SetWorkingMode(bool on)
{
  workingmode = on;
  return Toolbar_ApplyRunStopVisibility();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::Toolbar_ApplyRunStopVisibility()
* @brief      Shows Stop or Run in the shared toolbar slot according to workingmode.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::Toolbar_ApplyRunStopVisibility()
{
  UI_ELEMENT*      stop_btn = GEN_USERINTERFACE.Element_Get(NL("stop-btn"), UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT*      run_btn  = GEN_USERINTERFACE.Element_Get(NL("run-btn") , UI_ELEMENT_TYPE_MULTIOPTION);
  UI_ELEMENT*      lbl      = GEN_USERINTERFACE.Element_Get(NL("runstop-lbl"), UI_ELEMENT_TYPE_FORM);
  UI_ELEMENT_TEXT* txt      = (UI_ELEMENT_TEXT*)GEN_USERINTERFACE.Element_Get(NL("runstop-txt"), UI_ELEMENT_TYPE_TEXT);

  // Hide outgoing first (restore slot), then show incoming — no transition fade.
  if(workingmode)
    {
      Toolbar_SlotSetVisible(run_btn , false);
      Toolbar_SlotSetVisible(stop_btn, true);
    }
   else
    {
      Toolbar_SlotSetVisible(stop_btn, false);
      Toolbar_SlotSetVisible(run_btn , true);
    }

  // Shared legend stays visible (after both buttons in XML); only the caption swaps.
  // Skip legend churn when the caption is already correct (Ini SetWorkingMode): re-Discard/
  // wash/redraw was re-centering the glyph box and misaligning vs clear/delstatus/…
  if(lbl) lbl->SetVisible(true);
  if(txt && txt->GetText())
    {
      XCHAR* legend = workingmode ? TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_STOP)
                                  : TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_RUN);

      if(txt->GetText()->Compare(legend))
        {
          UI_LAYOUT* layout = txt->GetLayout();
          if(!layout && lbl) layout = lbl->GetLayout();
          UI_SKINCANVAS* skin = NULL;
          if(layout && layout->GetSkin() && layout->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
            skin = (UI_SKINCANVAS*)layout->GetSkin();

          // Parar↔Arrancar: GEN PrepareTextContentChange restores TextBackdrop (+ father form plate)
          // before the string swap so glyphs never stack. Do not CalculePosition / remasure.
          if(skin) skin->PrepareTextContentChange(txt);

          txt->GetText()->Set(legend);
          if(txt->GetMaskText())
            {
              if(workingmode) txt->GetMaskText()->Set(NL("#[BTN_STOP]"));
                         else txt->GetMaskText()->Set(NL("#[BTN_RUN]"));
            }

          txt->SetMustReDraw(true);
          GEN_USERINTERFACE.Elements_SetToRedraw(txt, true);
          if(lbl) GEN_USERINTERFACE.Elements_SetToRedraw(lbl, true);
        }
    }

  // Drop hover/press left on the button that just hid (shared toolbar slot).
  GEN_USERINTERFACE.ResetPreselect();

  return (stop_btn != NULL) || (run_btn != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplyColorTheme(int colortheme)
* @brief      Applies Dark/Light chrome (page, panels, strokes, fonts) from CFG tokens / CSS :root.
* @ingroup    UTILITIES
*
* @param[in]  colortheme : XTRACEMONITOR_CFG_COLORTHEME_* integer (INVALID resolves to default Dark).
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplyColorTheme(int colortheme)
{
  XTRACEMONITOR_CFG& cfg = XTRACEMONITOR_CFG::GetInstance(false);
  colortheme = cfg.ResolveColorTheme(colortheme);
  const bool light = (colortheme == XTRACEMONITOR_CFG_COLORTHEME_LIGHT);

  // Keep in sync with xtracemonitor.css :root tokens (var() is parse-time; runtime remaps below).
  // UI_COLOR CSV 4th field is alpha PERCENT (0-100), not 0-255.
  XCHAR* page_bg     = light ? NL("236,238,242,100") : NL("24,28,36,100");
  XCHAR* console_bg  = light ? NL("250,250,252,100") : NL("12,14,18,100");
  // Light frame slightly darker than page so panel edges stay visible on white washes.
  XCHAR* frame_line  = light ? NL("140,150,165,100") : NL("70,80,95,100");
  XCHAR* label_ink   = light ? NL("40,44,52,100")    : NL("200,208,220,100");

  auto set_named = [](XCHAR* name, XCHAR* rgba) -> void
    {
      XSTRING* existing = GEN_UI_COLORS.Get(name);
      if(existing) existing->Set(rgba);
       else         GEN_UI_COLORS.Add(name, rgba);
    };

  set_named(NL("label_ink")   , label_ink);
  set_named(NL("frame_line")  , frame_line);
  set_named(NL("console_back"), console_bg);

  // Only the app layouts. Never touch GEN custom window chrome ("chrome" from
  // defaultwindowschromes.zip): caption fill, white title, SVG buttons and app icon.
  XVECTOR<UI_LAYOUT*>* layouts = GEN_USERINTERFACE.Layouts_GetAll();
  if(layouts)
    {
      for(XDWORD i=0; i<layouts->GetSize(); i++)
        {
          UI_LAYOUT* layout = layouts->Get(i);
          if(!layout || !layout->GetNameID()) continue;

          const bool is_app_layout = (!layout->GetNameID()->Compare(NL("xtracemonitor"), true) ||
                                      !layout->GetNameID()->Compare(NL("config_modal") , true) ||
                                      !layout->GetNameID()->Compare(NL("filters_modal"), true));
          if(!is_app_layout) continue;

          if(layout->GetBackground() && layout->GetBackground()->GetColor())
            {
              layout->GetBackground()->GetColor()->SetFromString(page_bg);
              layout->GetBackground()->GetColor()->SetValid(true);
            }

          ApplyColorTheme_PatchStyleSheet(layout->GetStyleSheet(), light);

          XVECTOR<UI_ELEMENT*>* elements = layout->Elements_Get();
          if(!elements) continue;
          for(XDWORD e=0; e<elements->GetSize(); e++)
            {
              ApplyColorTheme_Element(elements->Get(e), light);
            }
        }
    }

  // Drop stale Form/Anim/Option backdrops (black plates under chrome SVGs on Light) then repaint.
  XVECTOR<UI_LAYOUT*>* all_layouts = GEN_USERINTERFACE.Layouts_GetAll();
  if(all_layouts)
    {
      for(XDWORD i=0; i<all_layouts->GetSize(); i++)
        {
          UI_LAYOUT* layout = all_layouts->Get(i);
          if(!layout || !layout->GetSkin()) continue;
          if(layout->GetSkin()->GetDrawMode() != UI_SKIN_DRAWMODE_CANVAS) continue;
          ((UI_SKINCANVAS*)layout->GetSkin())->InvalidateCompositionCaches();
        }
    }

  // Net-status frame + text plates match valor/console (console_back / frame_line / label_ink).
  if(netstatus_form)
    {
      netstatus_form->SetRoundRect(4);
      netstatus_form->SetBorderWidth(1.0);
      netstatus_form->SetBorderColorFromString(light ? NL("140,150,165,255") : NL("70,80,95,255"));
      if(netstatus_form->GetBackgroundColor())
        {
          netstatus_form->GetBackgroundColor()->SetFromString(light ? NL("250,250,252,255") : NL("12,14,18,255"));
          netstatus_form->SetBackgroundColorSet(true);
        }
      netstatus_form->SetMustReDraw(true);
    }

  auto theme_net_ink = [&](UI_ELEMENT_TEXT* el)
    {
      if(!el) return;
      if(el->GetColor())
        {
          if(light)
            {
              el->GetColor()->SetRed(40); el->GetColor()->SetGreen(44); el->GetColor()->SetBlue(52);
            }
           else
            {
              el->GetColor()->SetRed(220); el->GetColor()->SetGreen(226); el->GetColor()->SetBlue(235);
            }
          el->GetColor()->SetAlpha(255);
          el->GetColor()->SetValid(true);
        }
      if(el->GetBackgroundColor())
        {
          // Same fill as the netstatus frame (console_back) so glyphs do not flash a page plate.
          if(light)
            {
              el->GetBackgroundColor()->SetRed(250);
              el->GetBackgroundColor()->SetGreen(250);
              el->GetBackgroundColor()->SetBlue(252);
            }
           else
            {
              el->GetBackgroundColor()->SetRed(12);
              el->GetBackgroundColor()->SetGreen(14);
              el->GetBackgroundColor()->SetBlue(18);
            }
          el->GetBackgroundColor()->SetAlpha(255);
          el->GetBackgroundColor()->SetValid(true);
          el->SetBackgroundColorSet(true);
        }
      el->SetMustReDraw(true);
    };
  theme_net_ink(netstatus_state);
  theme_net_ink(netstatus_ip);
  theme_net_ink(netstatus_udp);

  // Chrome zip often loads after Ini (first canvas). Retry from DrawFrame until RefreshChrome sticks.
  chrome_theme_dirty = true;
  ApplyColorTheme_RefreshChrome(light);

  GEN_USERINTERFACE.Layout_PutBackground();
  GEN_USERINTERFACE.Elements_SetToRedraw();
  BrandMonitor_RepaintAfterChrome();
  if(GetMainScreen())
    InternetStatus_ApplyGeometry((double)GetMainScreen()->GetWidth());
   else
    InternetStatus_RepaintAfterChrome();

  OpStatus_Log(light ? _L("Color Theme: Light.") : _L("Color Theme: Dark."));
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplyColorTheme_RefreshChrome(bool light)
* @brief      Themes GEN custom window chrome caption/title for Dark/Light without touching SVG glyphs.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplyColorTheme_RefreshChrome(bool light)
{
  UI_LAYOUT* chrome = GEN_USERINTERFACE.Layouts_Get(NL("chrome"));
  if(!chrome) return false;

  // Chrome caption bar (= Windows custom chrome toolbar) matches the app page in every scheme.
  // Opaque page fill so SVG soft edges never composite onto a leftover black AnimBackdrop plate.
  // UI_COLOR CSV 4th field is alpha PERCENT (0-100), not 0-255.
  XCHAR* caption_fill = light ? NL("236,238,242,100") : NL("24,28,36,100");

  auto apply_caption = [&](UI_ELEMENT* element) -> void
    {
      if(!element) return;
      if(element->GetColor())
        {
          element->GetColor()->SetFromString(caption_fill);
          element->GetColor()->SetValid(true);
        }
      if(element->GetBackgroundColor())
        {
          element->GetBackgroundColor()->SetFromString(caption_fill);
          element->GetBackgroundColor()->SetValid(true);
        }
      element->SnapshotStyleVisual();
      element->SetMustReDraw(true);
    };

  auto apply_title = [&](UI_ELEMENT* element) -> void
    {
      if(!element) return;
      if(chrome->GetSkin() && chrome->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
        ((UI_SKINCANVAS*)chrome->GetSkin())->PrepareTextContentChange(element);
      if(element->GetColor())
        {
          // Authored #FFFFFFFF: write components (avoid CSV remap). Do not touch chrome SVG icons.
          if(light)
            {
              element->GetColor()->SetRed(40);
              element->GetColor()->SetGreen(44);
              element->GetColor()->SetBlue(52);
              element->GetColor()->SetAlpha(255);
            }
           else
            {
              element->GetColor()->SetRed(255);
              element->GetColor()->SetGreen(255);
              element->GetColor()->SetBlue(255);
              element->GetColor()->SetAlpha(255);
            }
          element->GetColor()->SetValid(true);
        }
      element->SnapshotStyleVisual();
      element->SetMustReDraw(true);
    };

  // Prefer role lookup (covers nested chrome title/caption), then walk as belt-and-suspenders.
  UI_ELEMENT* caption_el = chrome->Elements_Get(UI_ELEMENT_CHROMEROLE_CAPTION);
  UI_ELEMENT* title_el   = chrome->Elements_Get(UI_ELEMENT_CHROMEROLE_TITLE);
  if(!caption_el && !title_el) return false;

  apply_caption(caption_el);
  apply_title(title_el);

  auto walk = [&](UI_ELEMENT* element, auto&& self) -> void
    {
      if(!element) return;

      switch(element->GetChromeRole())
        {
          case UI_ELEMENT_CHROMEROLE_CAPTION : apply_caption(element); break;
          case UI_ELEMENT_CHROMEROLE_TITLE   : apply_title(element);   break;
                                    default : element->SetMustReDraw(true); break;
        }

      if(element->GetComposeElements())
        {
          for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
            self(element->GetComposeElements()->Get(c), self);
        }
    };

  XVECTOR<UI_ELEMENT*>* elements = chrome->Elements_Get();
  if(elements)
    {
      for(XDWORD e=0; e<elements->GetSize(); e++)
        walk(elements->Get(e), walk);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplyColorTheme_PatchStyleBag(UI_STYLE& style, bool light)
* @brief      Remaps baked Dark/Light RGB strings inside a style bag (CSS declarations / :root vars).
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplyColorTheme_PatchStyleBag(UI_STYLE& style, bool light)
{
  XVECTOR<UI_STYLEPROPERTY*>* props = style.GetProperties();
  if(!props) return false;

  for(XDWORD i=0; i<props->GetSize(); i++)
    {
      UI_STYLEPROPERTY* prop = props->Get(i);
      if(!prop) continue;
      XSTRING& value = prop->GetValue();
      if(value.IsEmpty()) continue;

      // Bidirectional so Accept can toggle either way after a previous theme.
      if(light)
        {
          value.Replace(NL("24,28,36")    , NL("236,238,242"));
          value.Replace(NL("12,14,18")    , NL("250,250,252"));
          value.Replace(NL("64,72,84")    , NL("210,214,220")); // edit idle
          value.Replace(NL("8,10,12")     , NL("248,250,252")); // edit focus
          value.Replace(NL("70,80,95")    , NL("140,150,165"));
          value.Replace(NL("160,170,185") , NL("140,150,165")); // prior Light token
          value.Replace(NL("200,208,220") , NL("40,44,52"));
          value.Replace(NL("95,140,190,100") , NL("165,200,235,100")); // title-bar left
          value.Replace(NL("95,140,190,0")   , NL("165,200,235,0"));   // title-bar right (transparent)
          value.Replace(NL("95,140,190")     , NL("165,200,235"));     // title-bar RGB without alpha
          value.Replace(NL("155,190,225")    , NL("210,228,245"));     // prior opaque right
          value.Replace(NL("72,118,168")     , NL("165,200,235"));     // prior title-bar left
          value.Replace(NL("130,170,210")    , NL("210,228,245"));     // prior title-bar right
        }
       else
        {
          value.Replace(NL("236,238,242") , NL("24,28,36"));
          value.Replace(NL("250,250,252") , NL("12,14,18"));
          value.Replace(NL("210,214,220") , NL("64,72,84"));   // edit idle
          value.Replace(NL("248,250,252") , NL("8,10,12"));    // edit focus
          value.Replace(NL("140,150,165") , NL("70,80,95"));
          value.Replace(NL("160,170,185") , NL("70,80,95"));
          value.Replace(NL("40,44,52")    , NL("200,208,220"));
          value.Replace(NL("165,200,235,100") , NL("95,140,190,100")); // title-bar left
          value.Replace(NL("165,200,235,0")   , NL("95,140,190,0"));   // title-bar right (transparent)
          value.Replace(NL("165,200,235")     , NL("95,140,190"));     // title-bar RGB without alpha
          value.Replace(NL("210,228,245")     , NL("155,190,225"));    // prior opaque right
          value.Replace(NL("150,185,225")     , NL("95,140,190"));     // prior light left
          value.Replace(NL("195,220,240")     , NL("155,190,225"));    // prior light right
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplyColorTheme_PatchStyleSheet(UI_STYLESHEET* sheet, bool light)
* @brief      Updates :root variables and every rule declaration to the active theme palette.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplyColorTheme_PatchStyleSheet(UI_STYLESHEET* sheet, bool light)
{
  if(!sheet) return false;

  ApplyColorTheme_PatchStyleBag(sheet->Variables(), light);

  if(light)
    {
      sheet->Variables().Set(NL("--page-bg")        , NL("236,238,242"));
      sheet->Variables().Set(NL("--console-bg")     , NL("250,250,252"));
      sheet->Variables().Set(NL("--frame-line")     , NL("140,150,165"));
      sheet->Variables().Set(NL("--label-ink")      , NL("40,44,52"));
      sheet->Variables().Set(NL("--edit-idle-bg")   , NL("210,214,220"));
      sheet->Variables().Set(NL("--edit-focus-bg")  , NL("248,250,252"));
      sheet->Variables().Set(NL("--title-bar-left") , NL("165,200,235,100"));
      sheet->Variables().Set(NL("--title-bar-right"), NL("165,200,235,0"));
    }
   else
    {
      sheet->Variables().Set(NL("--page-bg")        , NL("24,28,36"));
      sheet->Variables().Set(NL("--console-bg")     , NL("12,14,18"));
      sheet->Variables().Set(NL("--frame-line")     , NL("70,80,95"));
      sheet->Variables().Set(NL("--label-ink")      , NL("200,208,220"));
      sheet->Variables().Set(NL("--edit-idle-bg")   , NL("64,72,84"));
      sheet->Variables().Set(NL("--edit-focus-bg")  , NL("8,10,12"));
      sheet->Variables().Set(NL("--title-bar-left") , NL("95,140,190,100"));
      sheet->Variables().Set(NL("--title-bar-right"), NL("95,140,190,0"));
    }

  XVECTOR<UI_CSSRULE*>* rules = sheet->Rules_GetAll();
  if(rules)
    {
      for(XDWORD r=0; r<rules->GetSize(); r++)
        {
          UI_CSSRULE* rule = rules->Get(r);
          if(rule) ApplyColorTheme_PatchStyleBag(rule->GetDeclarations(), light);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR::ApplyColorTheme_Element(UI_ELEMENT* element, bool light)
* @brief      Recursively applies page/console/frame/ink colors to one element subtree.
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR::ApplyColorTheme_Element(UI_ELEMENT* element, bool light)
{
  if(!element) return false;

  // Window-chrome roles (caption / title / min / max / close / icon / language) keep their own palette.
  if(element->GetChromeRole() != UI_ELEMENT_CHROMEROLE_NONE) return true;

  const UI_ELEMENT_TYPE type = element->GetType();

  // Icon faces (toolbar multioption/button + PNG/SVG brand art): never recolor or
  // SnapshotStyleVisual — that replaced animation plates and hid the glyphs.
  if((type == UI_ELEMENT_TYPE_IMAGE)      ||
     (type == UI_ELEMENT_TYPE_ANIMATION)  ||
     (type == UI_ELEMENT_TYPE_BUTTON)     ||
     (type == UI_ELEMENT_TYPE_MULTIOPTION))
    {
      if(element->GetComposeElements())
        {
          for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
            ApplyColorTheme_Element(element->GetComposeElements()->Get(c), light);
        }
      return true;
    }

  // CSV 4th field is alpha PERCENT (0-100).
  XCHAR* page_bg    = light ? NL("236,238,242,100") : NL("24,28,36,100");
  XCHAR* console_bg = light ? NL("250,250,252,100") : NL("12,14,18,100");
  XCHAR* frame_line = light ? NL("140,150,165,100") : NL("70,80,95,100");
  XCHAR* edit_idle  = light ? NL("210,214,220,100") : NL("64,72,84,100");
  XCHAR* edit_focus = light ? NL("248,250,252,100") : NL("8,10,12,100");

  auto rgb_eq = [](UI_COLOR* c, XBYTE r, XBYTE g, XBYTE b) -> bool
    {
      if(!c) return false;
      return (c->GetRed() == r) && (c->GetGreen() == g) && (c->GetBlue() == b);
    };

  // Only remap colors that already belong to the Dark/Light token set. Leaves white chrome
  // titles, black caption fills, accent inks, and non-theme button washes alone.
  auto is_theme_ink = [&](UI_COLOR* c) -> bool
    {
      if(!c) return false;
      if(rgb_eq(c, 200, 208, 220) || rgb_eq(c, 40, 44, 52)) return true;
      // Named label_ink can drift a few LSB after style snapshots — treat near-tokens as theme ink.
      const int r = (int)c->GetRed();
      const int g = (int)c->GetGreen();
      const int b = (int)c->GetBlue();
      const int lum = r + g + b;
      const bool neutral = (abs(r - g) <= 16) && (abs(g - b) <= 16);
      return neutral && ((lum >= 560 && lum <= 680) || (lum >= 100 && lum <= 180));
    };
  auto is_theme_frame = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 70, 80, 95) || rgb_eq(c, 160, 170, 185) || rgb_eq(c, 140, 150, 165);
    };
  auto is_theme_page = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 24, 28, 36) || rgb_eq(c, 236, 238, 242);
    };
  auto is_theme_console = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 12, 14, 18) || rgb_eq(c, 250, 250, 252);
    };
  auto is_theme_edit_idle = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 64, 72, 84) || rgb_eq(c, 210, 214, 220) || rgb_eq(c, 48, 54, 64) || rgb_eq(c, 220, 224, 230);
    };
  auto is_theme_edit_focus = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 8, 10, 12) || rgb_eq(c, 248, 250, 252);
    };
  auto is_theme_title_bar = [&](UI_COLOR* c) -> bool
    {
      return rgb_eq(c, 95, 140, 190) || rgb_eq(c, 165, 200, 235) ||
             rgb_eq(c, 72, 118, 168) || rgb_eq(c, 150, 185, 225);
    };

  bool changed = false;

  // Toolbar legends + other label_ink texts: write components (PNG/SVG faces are never touched above).
  if(element->IsColorSet() && element->GetColor() && is_theme_ink(element->GetColor()))
    {
      UI_COLOR* ink = element->GetColor();
      if(type == UI_ELEMENT_TYPE_TEXT)
        {
          UI_LAYOUT* lay = element->GetLayout();
          if(lay && lay->GetSkin() && lay->GetSkin()->GetDrawMode() == UI_SKIN_DRAWMODE_CANVAS)
            ((UI_SKINCANVAS*)lay->GetSkin())->PrepareTextContentChange(element);
        }
      if(light)
        {
          ink->SetRed(40); ink->SetGreen(44); ink->SetBlue(52); ink->SetAlpha(255);
        }
       else
        {
          ink->SetRed(200); ink->SetGreen(208); ink->SetBlue(220); ink->SetAlpha(255);
        }
      ink->SetValid(true);
      changed = true;
    }

  if(element->IsBorderColorSet() && element->GetBorderColor() && is_theme_frame(element->GetBorderColor()))
    {
      element->GetBorderColor()->SetFromString(frame_line);
      element->GetBorderColor()->SetValid(true);
      changed = true;
    }

  if(element->IsBackgroundColorSet() && element->GetBackgroundColor() &&
     (element->GetBackgroundColor()->GetAlpha() > 0))
    {
      UI_COLOR* bg = element->GetBackgroundColor();
      const bool panel = ((type == UI_ELEMENT_TYPE_CONSOLE) ||
                          (type == UI_ELEMENT_TYPE_TREE) ||
                          (type == UI_ELEMENT_TYPE_PROPERTYLIST));

      if(panel || is_theme_console(bg))
        {
          bg->SetFromString(console_bg);
          bg->SetValid(true);
          changed = true;
        }
       else if(is_theme_page(bg))
        {
          bg->SetFromString(page_bg);
          bg->SetValid(true);
          changed = true;
        }
       else if(((type == UI_ELEMENT_TYPE_EDITTEXT) || (type == UI_ELEMENT_TYPE_COMBO) ||
                (type == UI_ELEMENT_TYPE_MENU) || (type == UI_ELEMENT_TYPE_OPTION)) &&
               (is_theme_edit_idle(bg) || is_theme_edit_focus(bg)))
        {
          // Keep focus/idle roles across Dark↔Light; :selected reapply will pick the live one.
          // COMBO is an EditText subclass with its own type — Puerto UDP/UART must remap too.
          // MENU/OPTION share the edit idle plate (combo dropdowns created before theme remap).
          const bool focused = element->IsSelected();
          const bool popup   = ((type == UI_ELEMENT_TYPE_MENU) || (type == UI_ELEMENT_TYPE_OPTION));
          bg->SetFromString((focused && !popup) ? edit_focus : edit_idle);
          bg->SetValid(true);
          changed = true;
        }
       else if((type == UI_ELEMENT_TYPE_TEXT) && is_theme_title_bar(bg))
        {
          bg->SetFromString(light ? NL("165,200,235,100") : NL("95,140,190,100")); // plate start (end is transparent via style)
          bg->SetValid(true);
          changed = true;
        }
    }

  if(element->GetComputedStyle())
    {
      ApplyColorTheme_PatchStyleBag(*element->GetComputedStyle(), light);
    }

  if(changed)
    {
      // Pseudo-class restyles restore from this snapshot — only refresh when we actually wrote colors.
      element->SnapshotStyleVisual();
      element->SetMustReDraw(true);
    }

  if(element->GetComposeElements())
    {
      for(XDWORD c=0; c<element->GetComposeElements()->GetSize(); c++)
        {
          ApplyColorTheme_Element(element->GetComposeElements()->Get(c), light);
        }
    }

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

            case UI_ELEMENT_CHROMEROLE_LANGUAGE :
                                                  #ifdef XTRANSLATION_ACTIVE
                                                  if(GEN_XTRANSLATION.IsInitialized() && GEN_XTRANSLATION.SetActual_Next())
                                                    {
                                                      GEN_USERINTERFACE.Layouts_ApplyTranslations();
                                                      #ifdef APPFLOW_CFG_GENERAL_ACTIVE
                                                      APPFLOW_CFG.Language_PersistActual();
                                                      #endif
                                                    }
                                                  #endif
                                                  return true;

            case UI_ELEMENT_CHROMEROLE_MAXIMIZE : if(GetMainScreen())
                                                    {
                                                      // Toggle grow ↔ restore (same contract as UI_System / WindowChromes).
                                                      if(GetMainScreen()->IsClientSizeAtMaximum())
                                                            GetMainScreen()->Maximize(false);
                                                       else GetMainScreen()->Maximize(true);
                                                    }
                                                  return true;

            case UI_ELEMENT_CHROMEROLE_CLOSE    : PersistScreenGeometryToCFG();
                                                  screen_geometry_persisted = true;
                                                  SetExitType(APPFLOWBASE_EXITTYPE_BY_USER);
                                                  return true;

            case UI_ELEMENT_CHROMEROLE_NONE     : chrome_el = chrome_el->GetFather();
                                                  continue;

            default                            : return true;   // other chrome roles consumed
          }
      }
  }

  XSTRING* name = element->GetName();
  if(!name) return false;

  // Config / Filters modals (Accept / Cancel / Predeterminado).
  if(form_cfg.HandleElementSelected(element)) return true;
  if(form_filters.HandleElementSelected(element)) return true;

  if(!name->Compare(NL("stop-btn"), true))
    {
      Toolbar_SetWorkingMode(false);
      OpStatus_Log(_L("Workingmode off (paused)."));
      return true;
    }

  if(!name->Compare(NL("run-btn"), true))
    {
      Toolbar_SetWorkingMode(true);
      OpStatus_Log(_L("Workingmode on."));
      return true;
    }

  if(!name->Compare(NL("clear-btn"), true))
    {
      if(consoleelement) consoleelement->Clear();
      OpStatus_Log(_L("Clear console."));
      return true;
    }

  if(!name->Compare(NL("delstatus-btn"), true))
    {
      OpStatus_Log(_L("Del var status."));
      return true;
    }

  if(!name->Compare(NL("dyndns-btn"), true))
    {
      OpStatus_Log(_L("Ajusta DynDNS (TODO)."));
      return true;
    }

  if(!name->Compare(NL("save-btn"), true))
    {
      OpStatus_Log(_L("Save console."));
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

  if(!maskvalue->Compare(NL("BTN_FILTER"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_FILTER));
    }

  if(!maskvalue->Compare(NL("BTN_CONFIG"), true))
    {
      maskresolved->Set(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_BTN_CONFIG));
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

  cfgchromes.SetResizeActive(true);

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

      case UI_XEVENT_TYPE_OUTPUT_CHANGECONTENTS  : { UI_ELEMENT* element = event->GetElement();
                                                     if(element)
                                                       {
                                                         form_cfg.HandleElementChangeContents(element);
                                                       }
                                                   }
                                                   break;
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

      case GRPXEVENT_TYPE_SCREEN_CHANGESIZE       : { GRPSCREEN* screen = event->GetScreen();
                                                      if(!screen) break;

                                                      // Geometry only. UpdateSize already PutBackground and will
                                                      // SetToRedraw + Update after this observer returns. A compose
                                                      // wipe here raced Overlay_ClearSuppressedContentDirt and left
                                                      // toolbar/panels blank after maximize/extend.
                                                      Layout_ApplyFlexibleResize(false);
                                                      resize_recover_frames = 3;
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
  xtimer                 = NULL;
  cursor                 = NULL;
  consoleelement         = NULL;
  valorelement           = NULL;
  valor_prevpressed      = false;
  treeelement            = NULL;
  tree_uart_node         = NULL;
  tree_prevpressed       = false;
  opstatus_form          = NULL;
  opstatus_text          = NULL;
  opstatus_count         = 0;
  opstatus_foot_y        = 0.0f;
  opstatus_last_baseline = 0.0f;
  opstatus_line_h        = 0.0f;
  brand_monitor             = NULL;
  netstatus_form            = NULL;
  netstatus_led             = NULL;
  netstatus_state           = NULL;
  netstatus_ip              = NULL;
  netstatus_udp             = NULL;
  netstatus_timer           = NULL;
  netstatus_haveinternet    = false;
  netstatus_prev_l          = 0.0;
  netstatus_prev_t          = 0.0;
  netstatus_prev_r          = 0.0;
  netstatus_prev_b          = 0.0;
  netstatus_prev_valid      = false;
  layout_applied_w          = 0;
  layout_applied_h          = 0;
  resize_recover_frames     = 0;
  opstatus_expanded         = false;
  opstatus_prevpressed      = false;
  opstatus_click_armed      = false;
  opstatus_click_timer      = NULL;
  workingmode               = true;
  chrome_theme_dirty        = false;
  exitapp                   = false;
  screen_geometry_persisted = false;

  for(int c=0; c<XTRACEMONITOR_BUTTON_MAX; c++)
    {
      button[c] = NULL;
    }
}
