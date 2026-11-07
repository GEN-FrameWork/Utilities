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
#include "UI_Skin.h"
#include "UI_SkinCanvas.h"
#include "UI_BoundaryLine.h"
#include "UI_Colors.h"
#include "UI_Style.h"
#include "UI_StyleSheet.h"
#include "GRP2DColor.h"

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

  static int cursor_x = 0;
  static int cursor_y = 0;

  int  now_cursor_x = 0;
  int  now_cursor_y = 0;
  bool bar_owned    = false;

  if(cursor)
    {
      now_cursor_x = (int)cursor->GetX();
      now_cursor_y = (int)height - (int)cursor->GetY();

      // Motion gestures (keep historical path). Click/release handling below must NOT live only
      // inside this branch: IsPressedWithRelease needs to observe press frames, and IsReadyToTest
      // would otherwise swallow short clicks (including opstatus double-click).
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

          // Overlay scrollbar thumb drag (tree / propertylist / console): capture on press over the
          // track, follow while held, release without firing a content SELECCTION.
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

      // Always sample mouse click/release (independent of motion-buffer gating).
      if(button[XTRACEMONITOR_BUTTON_MOUSE] && button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressedWithRelease())
        {
          int click_x = now_cursor_x;
          int click_y = now_cursor_y;

          // While a GEN modal form is up, do not route clicks to foot/opstatus under it.
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

  // Re-assert foot geometry after UI layout writeback (intrinsic/boundary can be restored mid-frame).
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

      // Until line metrics are known, keep text dirty so ContentHeight is measured on draw.
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
  screen->SetWidth(XTRACEMONITOR_WINDOW_WIDTH);
  screen->SetHeight(XTRACEMONITOR_WINDOW_HEIGHT);

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

  bool status = screen->CreateViewport(GRPVIEWPORT_ID_MAIN, 0.0f, 0.0f, (float)screen->GetWidth(), (float)screen->GetHeight(), 0, 0, screen->GetWidth(), screen->GetHeight());
  if(status)
    {
      GRPVIEWPORT* viewport = screen->GetViewport(GRPVIEWPORT_ID_MAIN);
      if(viewport)
        {
          // Min = design size (1200×699). Max allows growth; chrome buttons track via UpdateCFGChromesButtonsPosition.
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

  if(!BrandMonitor_Resolve())
    {
      return false;
    }

  if(!OpStatus_Resolve())
    {
      return false;
    }

  form_cfg.SetOwner(this);
  if(!form_cfg.Resolve())
    {
      return false;
    }

  // Remap app tokens + chrome title/caption for the persisted theme before the first frame.
  ApplyColorTheme(XTRACEMONITOR_CFG::GetInstance(false).GetColorTheme());

  // Default: workingmode on → Stop visible / Run hidden. Apply once after first Layout_PutBackground
  // so ConvertToBoolean-fixed XML visibility is enforced without painting into an empty canvas.
  // (Calling Apply before any compose used to discard legend TextBackdrop into a gray plate.)
  workingmode = true;
  Toolbar_ApplyRunStopVisibility();

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

  // Belt-and-suspenders: CHANGESIZE can race with canvas recreate; re-apply flexible layout whenever
  // the live client size diverges from the last geometry we laid out, then force a full redraw.
  int live_w = (int)screen->GetWidth();
  int live_h = (int)screen->GetHeight();
  bool size_relaid = false;
  if((live_w != layout_applied_w) || (live_h != layout_applied_h))
    {
      Layout_ApplyFlexibleResize(true);
      size_relaid = true;
      resize_recover_frames = 3;
      // Fresh buffer: drop any canvas-level rebuild snapshots before ReleaseDrawFramerate/Rebuild.
      canvas->DeleteAllRebuildAreas();
      canvas->SetClipBox(0.0, 0.0, (double)(live_w > 0 ? live_w - 1 : 0), (double)(live_h > 0 ? live_h - 1 : 0));
    }
  else if(resize_recover_frames > 0)
    {
      // UpdateSize compose can lose to GLES surface recreate / Overlay dirt clear on the first
      // frames after maximize/extend — force a few full page wipe + redraw passes.
      GEN_USERINTERFACE.Layout_PutBackground();
      GEN_USERINTERFACE.Elements_SetToRedraw();
      size_relaid = true;
      resize_recover_frames--;
    }

  // Chrome layout may appear only after the first canvas/LoadCFGChromesLayout — finish Light/Dark
  // caption title ink + cache invalidate once it exists.
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

  // Bottom-left: 5px from left and bottom (DrawFramerate y is the text baseline; +2 is box bottom).
  canvas->DrawFramerate(screen, 5.0, (double)(screen->GetHeight() - 7));

  GEN_USERINTERFACE.Elements_RebuildDrawAreas();
  if(size_relaid) GEN_USERINTERFACE.Elements_SetToRedraw();
  GEN_USERINTERFACE.Update();

  // Chrome paints after content and its right-side button rebuild can wipe the brand top.
  // Re-blit after chrome (same-frame) so a larger brand can sit flush under the caption.
  BrandMonitor_RepaintAfterChrome();

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

  // Use the LIVE canvas size only. Clamping W/H up to the design minimum while GetWidth() was still
  // a few pixels short (startup client/canvas desync) parked brand_monitor and right-edge panels past
  // the present edge. Window min size is enforced by the viewport / WM_GETMINMAXINFO path instead.
  double W = (double)screen->GetWidth();
  double H = (double)screen->GetHeight();
  if((W < 1.0) || (H < 1.0)) return false;

  // Keep design size in sync for CSS @media / rem resolution. UIScale stays inactive at scale 1.0
  // without autofit (see UI_LAYOUT::IsUIScaleActive) so flexible layout paints 1:1 on the live canvas.
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
  const double brand_w  = XTRACEMONITOR_LAYOUT_BRAND_W;
  const double brand_h  = XTRACEMONITOR_LAYOUT_BRAND_H;
  const double brand_gap_valor = XTRACEMONITOR_LAYOUT_BRAND_GAP_VALOR;

  double panel_w = W - margin - panels_x;
  if(panel_w < 1.0) panel_w = 1.0;

  double foot_y      = H - margin;
  double console_bot = foot_y - foot_h - gap;
  double valor_y     = valor_top + valor_h;
  double console_top = valor_y + gap;
  double console_h   = console_bot - console_top;
  if(console_h < 1.0) console_h = 1.0;

  // Keep opstatus text baseline glued to the foot when the window height changes.
  if((opstatus_foot_y > 0.0) && (foot_y != opstatus_foot_y) && (opstatus_last_baseline > 0.0))
    {
      opstatus_last_baseline += (foot_y - opstatus_foot_y);
    }

  opstatus_foot_y = foot_y;

  // Tree: fixed width/xpos; height tracks window so top stays with valor and bottom with console.
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
      // Root elements: boundaryline.y is the absolute bottom edge used if CalculePosition re-runs.
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

  if(brand_monitor && brand_monitor->GetBoundaryLine())
    {
      double brand_x = W - margin - brand_w;
      double brand_y = valor_top - brand_gap_valor;   // bottom edge; keeps 2px air above valor
      if(brand_y < brand_h) brand_y = brand_h;
      brand_monitor->SetXPosition(brand_x);
      brand_monitor->SetYPosition(brand_y);
      brand_monitor->GetBoundaryLine()->x      = brand_x;
      brand_monitor->GetBoundaryLine()->y      = brand_y;
      brand_monitor->GetBoundaryLine()->width  = brand_w;
      brand_monitor->GetBoundaryLine()->height = brand_h;
      brand_monitor->SetIntrinsicWidth(brand_w);
      brand_monitor->SetIntrinsicHeight(brand_h);
    }

  layout_applied_w = (int)W;
  layout_applied_h = (int)H;

  if(form_cfg.IsVisible())
    {
      form_cfg.ApplyGeometry();
    }

  if(!compose) return true;

  // Canvas was recreated empty on UpdateSize / live drag: wipe background and dirty EVERY layout
  // element so tree / toolbar / flex panels all repaint at the new geometry.
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

  // GEN modal: property-list splitter under the dialog must not steal the pointer.
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

  // Optional per-node icons (PNG): drawn at height == sizefont in front of the label.
  UI_ELEMENT_TREE_NODE* all = treeelement->AddNode(NULL, NL("All"), NL("tree_icon_all.png"));
  if(!all) return false;

  treeelement->AddNode(all, NL("# Local (no internet)"), NL("tree_icon_local.png"));
  treeelement->AddNode(all, NL("Home [88.14.204.122]"), NL("tree_icon_home.png"));

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
      UI_ELEMENT_TREE_NODE* ip = treeelement->AddNode(all, (XCHAR*)ips[i], NL("tree_icon_ip.png"));
      if(!ip) continue;

      UI_ELEMENT_TREE_NODE* trace = treeelement->AddNode(ip, NL("Trace A — uplink via edge-router.primary"), NL("tree_icon_trace.png"));
      treeelement->AddNode(ip, NL("Trace B — secondary path (failover)"), NL("tree_icon_trace.png"));

      if(trace)
        {
          UI_ELEMENT_TREE_NODE* sess = treeelement->AddNode(trace, NL("Session peer [192.168.1.24] NAT"), NL("tree_icon_session.png"));
          treeelement->AddNode(trace, NL("Session peer [10.0.0.7] NAT"), NL("tree_icon_session.png"));
          treeelement->AddNode(trace, NL("Session peer [172.16.4.88] DMZ"), NL("tree_icon_session.png"));

          if(sess)
            {
              UI_ELEMENT_TREE_NODE* flow = treeelement->AddNode(sess, NL("Flow TCP/443 → cdn.endorasoft.local"), NL("tree_icon_flow.png"));
              treeelement->AddNode(sess, NL("Flow UDP/1194 → vpn.endorasoft.local"), NL("tree_icon_flow.png"));

              if(flow)
                {
                  treeelement->AddNode(flow, NL("Packet sample #001 — SYN/ACK handshake complete"), NL("tree_icon_packet.png"));
                  treeelement->AddNode(flow, NL("Packet sample #002 — TLS ClientHello (SNI)"), NL("tree_icon_packet.png"));
                  treeelement->Expand(flow);
                }

              treeelement->Expand(sess);
            }

          treeelement->Expand(trace);
        }

      treeelement->Expand(ip);
    }

  treeelement->AddNode(all, NL("UART STM32 F0"), NL("tree_icon_uart.png"));
  treeelement->AddNode(all, NL("UART STM32 F4 (long label overflow check)"), NL("tree_icon_uart.png"));

  treeelement->Expand(all);
  treeelement->SetSelected(all);

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
  if(!brand_monitor) return false;

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

  // Opaque page wipe under the brand AABB so repeated alpha blits do not compound.
  // Never paint into the valor band (top = VALOR_TOP): that was erasing the propertylist header.
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
  int wi = (int)w; if(wi < 1) wi = 1;
  int hi = (int)(bottom - top); if(hi < 1) hi = 1;
  for(int py = 0; py < hi; py++)
    {
      for(int px = 0; px < wi; px++)
        {
          canvas->PutPixel(left + (double)px, top + (double)py, (const GRP2DCOLOR*)&pagecol);
        }
    }

  brand_monitor->SetMustReDraw(true);
  return skin_canvas->Draw(brand_monitor);
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
bool XTRACEMONITOR::Toolbar_SlotSetVisible(UI_ELEMENT* element, bool visible)
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
                      if(kid && kid->GetType() == UI_ELEMENT_TYPE_ANIMATION) skin->DiscardAnimBackdrop(kid);
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
              UI_ELEMENT* legend = GEN_USERINTERFACE.Element_Get(NL("runstop-lbl"), UI_ELEMENT_TYPE_FORM);
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

  GEN_USERINTERFACE.Elements_SetToRedraw(element, true);

  return true;
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
                                      !layout->GetNameID()->Compare(NL("config_modal") , true));
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

  // Chrome zip often loads after Ini (first canvas). Retry from DrawFrame until RefreshChrome sticks.
  chrome_theme_dirty = true;
  ApplyColorTheme_RefreshChrome(light);

  GEN_USERINTERFACE.Layout_PutBackground();
  GEN_USERINTERFACE.Elements_SetToRedraw();
  BrandMonitor_RepaintAfterChrome();

  OpStatus_Log(light ? NL("Color Theme: Light.") : NL("Color Theme: Dark."));
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
          value.Replace(NL("70,80,95")    , NL("140,150,165"));
          value.Replace(NL("160,170,185") , NL("140,150,165")); // prior Light token
          value.Replace(NL("200,208,220") , NL("40,44,52"));
        }
       else
        {
          value.Replace(NL("236,238,242") , NL("24,28,36"));
          value.Replace(NL("250,250,252") , NL("12,14,18"));
          value.Replace(NL("140,150,165") , NL("70,80,95"));
          value.Replace(NL("160,170,185") , NL("70,80,95"));
          value.Replace(NL("40,44,52")    , NL("200,208,220"));
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
      sheet->Variables().Set(NL("--page-bg")   , NL("236,238,242"));
      sheet->Variables().Set(NL("--console-bg"), NL("250,250,252"));
      sheet->Variables().Set(NL("--frame-line"), NL("140,150,165"));
      sheet->Variables().Set(NL("--label-ink") , NL("40,44,52"));
    }
   else
    {
      sheet->Variables().Set(NL("--page-bg")   , NL("24,28,36"));
      sheet->Variables().Set(NL("--console-bg"), NL("12,14,18"));
      sheet->Variables().Set(NL("--frame-line"), NL("70,80,95"));
      sheet->Variables().Set(NL("--label-ink") , NL("200,208,220"));
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

  // Config modal (Color Theme draft / Accept / Cancel / Predeterminado).
  if(form_cfg.HandleElementSelected(element)) return true;

  if(!name->Compare(NL("stop-btn"), true))
    {
      Toolbar_SetWorkingMode(false);
      OpStatus_Log(NL("workingmode off (paused)."));
      return true;
    }

  if(!name->Compare(NL("run-btn"), true))
    {
      Toolbar_SetWorkingMode(true);
      OpStatus_Log(NL("workingmode on."));
      return true;
    }

  if(!name->Compare(NL("clear-btn"), true))
    {
      if(consoleelement) consoleelement->Clear();
      OpStatus_Log(NL("Clear."));
      return true;
    }

  if(!name->Compare(NL("delstatus-btn"), true))
    {
      OpStatus_Log(NL("Del status (TODO)."));
      return true;
    }

  if(!name->Compare(NL("filter-btn"), true))
    {
      OpStatus_Log(NL("Filter (TODO)."));
      return true;
    }

  if(!name->Compare(NL("dyndns-btn"), true))
    {
      OpStatus_Log(NL("DynDNS (TODO)."));
      return true;
    }

  if(!name->Compare(NL("save-btn"), true))
    {
      OpStatus_Log(NL("Save (TODO)."));
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
  tree_prevpressed       = false;
  opstatus_form          = NULL;
  opstatus_text          = NULL;
  opstatus_count         = 0;
  opstatus_foot_y        = 0.0f;
  opstatus_last_baseline = 0.0f;
  opstatus_line_h        = 0.0f;
  brand_monitor             = NULL;
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

  for(int c=0; c<XTRACEMONITOR_BUTTON_MAX; c++)
    {
      button[c] = NULL;
    }
}
