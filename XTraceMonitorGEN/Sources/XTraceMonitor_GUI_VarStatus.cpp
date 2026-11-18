/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_GUI_VarStatus.cpp
*
* @class      XTRACEMONITOR_GUI_VARSTATUS
* @brief      Status-variable property list (valor): resolve, seed, splitter, layout
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

#include "XTraceMonitor_GUI_VarStatus.h"

#include "GRPScreen.h"
#include "INPButton.h"
#include "INPCursor.h"

#include "UI_BoundaryLine.h"
#include "UI_Element_PropertyList.h"
#include "UI_Manager.h"

#include "XTraceMonitor.h"
#include "XTraceMonitor_TranslateID.h"

#include "XTranslation.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_VARSTATUS::XTRACEMONITOR_GUI_VARSTATUS()
* @brief      Constructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_VARSTATUS::XTRACEMONITOR_GUI_VARSTATUS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTRACEMONITOR_GUI_VARSTATUS::~XTRACEMONITOR_GUI_VARSTATUS()
* @brief      Destructor
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
XTRACEMONITOR_GUI_VARSTATUS::~XTRACEMONITOR_GUI_VARSTATUS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::SetOwner(XTRACEMONITOR* owner)
* @brief      Binds this module to the application instance.
* @ingroup    UTILITIES
*
* @param[in]  owner : Application owner.
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::SetOwner(XTRACEMONITOR* owner)
{
  this->owner = owner;
  return (owner != NULL);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         UI_ELEMENT_PROPERTYLIST* XTRACEMONITOR_GUI_VARSTATUS::GetElement()
* @brief      Returns the resolved valor property-list element.
* @ingroup    UTILITIES
*
* @return     UI_ELEMENT_PROPERTYLIST* : Valor element, or NULL if unresolved.
*
*---------------------------------------------------------------------------------------------------------------------*/
UI_ELEMENT_PROPERTYLIST* XTRACEMONITOR_GUI_VARSTATUS::GetElement()
{
  return valorelement;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::Resolve()
* @brief      Resolves the layout "valor" property list element pointer.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::Resolve()
{
  valorelement = (UI_ELEMENT_PROPERTYLIST*)GEN_USERINTERFACE.Element_Get(NL("valor"), UI_ELEMENT_TYPE_PROPERTYLIST);
  if(!valorelement) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::Seed()
* @brief      Clears the "valor" property list (no sample rows — filled by live status later).
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::ApplyHeaderCaptions()
{
  if(!valorelement) return false;

  valorelement->SetHeaderCaptions(TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_PROPERTY_NAME),
                                  TL(XTRACEMONITOR_TRANSLATEID_XTRACEMONITOR_PROPERTY_VALUE));
  GEN_USERINTERFACE.Elements_SetToRedraw(valorelement);
  return true;
}


bool XTRACEMONITOR_GUI_VARSTATUS::Seed()
{
  if(!valorelement) return false;

  valorelement->Clear();
  ApplyHeaderCaptions();
  GEN_USERINTERFACE.Elements_SetToRedraw(valorelement);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::Detach()
* @brief      Clears the resolved UI pointer (UI tear-down).
* @ingroup    UTILITIES
*
* @return     bool : true always.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::Detach()
{
  valorelement      = NULL;
  valor_prevpressed = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::UpdateSplitter()
* @brief      Drives the "valor" column splitter drag from the mouse button state.
* @ingroup    UTILITIES
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::UpdateSplitter()
{
  if(!owner || !valorelement || !owner->cursor || !owner->button[XTRACEMONITOR_BUTTON_MOUSE]) return false;

  if(GEN_USERINTERFACE.Element_ModalBlocksInteraction(valorelement))
    {
      if(valorelement->IsSplitDragging()) valorelement->Split_EndDrag();
      valor_prevpressed = owner->button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();
      return true;
    }

  bool   pressed  = owner->button[XTRACEMONITOR_BUTTON_MOUSE]->IsPressed();
  int    cursor_x = (int)owner->cursor->GetX();
  int    cursor_y = (int)owner->GetMainScreen()->GetHeight() - (int)owner->cursor->GetY();
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
* @fn         bool XTRACEMONITOR_GUI_VARSTATUS::ApplyLayoutGeometry(double panel_w, double valor_h, double valor_y)
* @brief      Applies flexible-resize geometry to the valor property list.
* @ingroup    UTILITIES
*
* @param[in]  panel_w : Valor width.
* @param[in]  valor_h : Valor height.
* @param[in]  valor_y : Valor bottom Y (EdgeBottom).
*
* @return     bool : true if the valor was updated; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRACEMONITOR_GUI_VARSTATUS::ApplyLayoutGeometry(double panel_w, double valor_h, double valor_y)
{
  if(!valorelement || !valorelement->GetBoundaryLine()) return false;

  valorelement->GetBoundaryLine()->width  = panel_w;
  valorelement->GetBoundaryLine()->height = valor_h;
  valorelement->GetBoundaryLine()->y      = valor_y;
  valorelement->SetIntrinsicWidth(panel_w);
  valorelement->SetIntrinsicHeight(valor_h);
  valorelement->SetYPosition(valor_y);
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XTRACEMONITOR_GUI_VARSTATUS::Clean()
* @brief      Cleans the object internal state.
* @note       INTERNAL
* @ingroup    UTILITIES
*
*---------------------------------------------------------------------------------------------------------------------*/
void XTRACEMONITOR_GUI_VARSTATUS::Clean()
{
  owner             = NULL;
  valorelement      = NULL;
  valor_prevpressed = false;
}
