/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_Console.cpp
*
* @class      XTRACEMONITOR_GUI_CONSOLE
* @brief      Trace console panel (resolve, seed, layout)
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
*---------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTraceMonitor_GUI_Console.h"

#include "UI_BoundaryLine.h"
#include "UI_Element_Console.h"
#include "UI_Manager.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_CFG.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_CONSOLE::XTRACEMONITOR_GUI_CONSOLE()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_CONSOLE::XTRACEMONITOR_GUI_CONSOLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_CONSOLE::~XTRACEMONITOR_GUI_CONSOLE()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_CONSOLE::~XTRACEMONITOR_GUI_CONSOLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds this module to the application instance.
* @ingroup    UTILITIES
*
* @param[in]  owner : Application owner.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_CONSOLE* XTRACEMONITOR_GUI_CONSOLE::GetElement()
* @brief      Returns the resolved console UI element.
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_CONSOLE* : Console element, or NULL if unresolved.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_CONSOLE* XTRACEMONITOR_GUI_CONSOLE::GetElement()
{
  return consoleelement;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::Resolve()
* @brief      Resolves the layout console element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::Resolve()
{
  consoleelement = (UI_ELEMENT_CONSOLE*)GEN_USERINTERFACE.Element_Get(NL("log_console"), UI_ELEMENT_TYPE_CONSOLE);
  if(!consoleelement) return false;

  // XML defaults autoscroll=true; align with CFG texttracking (often off).
  ApplyTextTracking();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SeedStartupLines()
* @brief      Clears the graphical console at startup (live lines arrive from the server pump).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SeedStartupLines()
{
  if(!consoleelement) return false;

  consoleelement->Clear();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::Detach()
* @brief      Clears the resolved UI pointer (UI tear-down).
* @ingroup    UTILITIES
*
* @return     bool : true always.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::Detach()
{
  consoleelement = NULL;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::ApplyLayoutGeometry(double panel_w, double console_h, double console_bot)
* @brief      Applies flexible-resize geometry to the console panel.
* @ingroup    UTILITIES
*
* @param[in]  panel_w     : Console width.
* @param[in]  console_h   : Console height.
* @param[in]  console_bot : Console bottom Y (EdgeBottom).
*
* @return     bool : true if the console was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::ApplyLayoutGeometry(double panel_w, double console_h, double console_bot)
{
  if(!consoleelement || !consoleelement->GetBoundaryLine()) return false;

  consoleelement->GetBoundaryLine()->width  = panel_w;
  consoleelement->GetBoundaryLine()->height = console_h;
  consoleelement->GetBoundaryLine()->y      = console_bot;
  consoleelement->SetIntrinsicWidth(panel_w);
  consoleelement->SetIntrinsicHeight(console_h);
  consoleelement->SetYPosition(console_bot);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::SetToRedraw()
* @brief      Marks the console element dirty for the next paint.
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::SetToRedraw()
{
  if(!consoleelement) return false;
  GEN_USERINTERFACE.Elements_SetToRedraw(consoleelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_CONSOLE::ApplyTextTracking()
* @brief      Mirrors CFG texttracking onto console autoscroll (follow live tail).
* @ingroup    UTILITIES
*
* @return     bool : true if the console exists; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_CONSOLE::ApplyTextTracking()
{
  if(!consoleelement) return false;

  // OFF: keep displacement (user/manual scroll). ON: pin to live tail each paint.
  consoleelement->SetAutoscroll(XTRACEMONITOR_CFG::GetInstance(false).GetTextTracking());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_CONSOLE::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_CONSOLE::Clean()
{
  owner          = NULL;
  consoleelement = NULL;
}
