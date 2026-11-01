/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XDateTime.h"
#include "XFSMachine.h"
#include "XString.h"

#include "APPFlowGraphics.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XTRACEMONITORXFSMEVENTS
{
  XTRACEMONITOR_XFSMEVENT_NONE              = 0 ,
  XTRACEMONITOR_XFSMEVENT_INI                   ,
  XTRACEMONITOR_XFSMEVENT_END                   ,

  XTRACEMONITOR_LASTEVENT
};


enum XTRACEMONITORXFSMSTATES
{
  XTRACEMONITOR_XFSMSTATE_NONE              = 0 ,
  XTRACEMONITOR_XFSMSTATE_INI                   ,
  XTRACEMONITOR_XFSMSTATE_END                   ,

  XTRACEMONITOR_LASTSTATE
};


enum XTRACEMONITOR_BUTTONS
{
  XTRACEMONITOR_BUTTON_ESC                  = 0 ,
  XTRACEMONITOR_BUTTON_MOUSE                    ,
  XTRACEMONITOR_BUTTON_TOUCHSCREEN              ,

  XTRACEMONITOR_BUTTON_MAX
};


#define APPLICATION_VERSION                       0
#define APPLICATION_SUBVERSION                    1
#define APPLICATION_SUBVERSIONERR                 0

#define APPLICATION_NAMEAPP                       NL("XTraceMonitor")
#define APPLICATION_NAMEFILE                      NL("xtracemonitor")

#define APPLICATION_OWNER                         NL("EndoraSoft")

#define APPLICATION_YEAROFCREATION                2026

#define APPLICATION_GUI_XTRACEMONITOR_VIEWPORT_ID NL("xtracemonitor_viewport")



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class INPBUTTON;
class INPCURSOR;
class UI_XEVENT;
class UI_ELEMENT;
class UI_ELEMENT_TEXT;
class UI_ELEMENT_CONSOLE;
class UI_ELEMENT_PROPERTYLIST;
class UI_ELEMENT_TREE;
class GRPXEVENT;
class XTRACEMONITOR_CFG;

class XTRACEMONITOR : public APPFLOWGRAPHICS, public XFSMACHINE
{
  public:
                                    XTRACEMONITOR                           ();
    virtual                        ~XTRACEMONITOR                           ();

    bool                            InitFSMachine                           ();

    bool                            AppProc_Ini                             ();
    bool                            AppProc_FirstUpdate                     ();
    bool                            AppProc_Update                          ();
    bool                            AppProc_End                             ();

  private:

    bool                            UpdateInput                             ();

    bool                            Ini_Graphics                            (GRPSCREEN* screen);
    bool                            Ini_Canvas                              (GRPSCREEN* screen);
    bool                            Ini_UserInterface                       (bool on);

    bool                            DrawFrame                               ();

    bool                            Console_Resolve                         ();
    bool                            Console_SeedStartupLines                ();

    bool                            Valor_Resolve                           ();
    bool                            Valor_Seed                              ();
    bool                            Valor_UpdateSplitter                    ();

    bool                            Tree_Resolve                            ();
    bool                            Tree_Seed                               ();
    bool                            Tree_UpdateInput                        ();

    bool                            Toolbar_SetRunning                      (bool running);
    bool                            Toolbar_ApplyRunStopVisibility          ();

    bool                            UserInterface_ElementSelected           (UI_ELEMENT* element);
    bool                            UserInterface_ChangeLiteralText         (UI_ELEMENT_TEXT* element_text, XSTRING* maskvalue, XSTRING* maskresult);
    bool                            UserInterface_CFGChromes                (GRPSCREEN* screen);

    void                            HandleEvent_UserInterface               (UI_XEVENT* event);
    void                            HandleEvent_Graphics                    (GRPXEVENT* event);
    void                            HandleEvent                             (XEVENT* xevent);

    void                            Clean                                   ();

    XTIMER*                         xtimer;

    INPBUTTON*                      button[XTRACEMONITOR_BUTTON_MAX];
    INPCURSOR*                      cursor;

    UI_ELEMENT_CONSOLE*             consoleelement;
    UI_ELEMENT_PROPERTYLIST*        valorelement;
    bool                            valor_prevpressed;

    UI_ELEMENT_TREE*                treeelement;
    bool                            tree_prevpressed;

    bool                            monitor_running;
    bool                            exitapp;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
