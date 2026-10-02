

WND_BMP_STATUS = {   OK: 0,
                     NOWINDOW: 1,
                     NOBITMAP: 2
                 };


var appname                      = "java.exe";
var windowtitle                  = "PCE_ZR";
                                
var user                         = "Supervisor1";
var password                     = "SUPERVISOR1";

var namebmpuserloggedin          = "Supervisor1.png";

var namebmpEstablishTSRpanel     = "establecerLTV.png";
var namebmpEstablishTSRpanel2    = "establecerLTV2.png";

var namebmpEliminateTSRpanel     = "eliminarLTV.png";
var namebmpEliminateTSRpanel2    = "eliminarLTV2.png";

var namebmpActivateTSR           = "activarLTV.png";
var namebmpPurgeTSR              = "purgarLTV.png";
var namebmpTSREstablished        = "establecidaLTV.png";

                                 
var posxEstablishTSRpanel        = 0;
var posyEstablishTSRpanel        = 0;

var posxEliminateTSRpanel        = 0;
var posyEliminateTSRpanel        = 0;
    
function PE_Manager_Ini()
{
  Screen_SetFocus(appname, windowtitle);

  var outEstablishX = { value: 0 };
  var outEstablishY = { value: 0 };
  var statusEstablish = Screen_GetPosX(appname, windowtitle, namebmpEstablishTSRpanel, namebmpEstablishTSRpanel2, outEstablishX);
  Screen_GetPosY(appname, windowtitle, namebmpEstablishTSRpanel, namebmpEstablishTSRpanel2, outEstablishY);
  posxEstablishTSRpanel = outEstablishX.value;
  posyEstablishTSRpanel = outEstablishY.value;
  TracePrintColor(1, "x = %d e y = %d", posxEstablishTSRpanel, posyEstablishTSRpanel);

  var outEliminateX = { value: 0 };
  var outEliminateY = { value: 0 };
  Screen_GetPosX(appname, windowtitle, namebmpEliminateTSRpanel2, outEliminateX);
  Screen_GetPosY(appname, windowtitle, namebmpEliminateTSRpanel2, outEliminateY);
  posxEliminateTSRpanel = outEliminateX.value;
  posyEliminateTSRpanel = outEliminateY.value;
  TracePrintColor(1, "x = %d e y = %d", posxEliminateTSRpanel, posyEliminateTSRpanel);

  var outWinX = { value: 0 };
  var outWinY = { value: 0 };
  Screen_GetPosX(appname, windowtitle, outWinX);
  Screen_GetPosY(appname, windowtitle, outWinY);
  posx = outWinX.value;
  posy = outWinY.value;
  TracePrintColor(1, "x = %d e y = %d", posx, posy);

  if(statusEstablish != WND_BMP_STATUS.OK)
    {
      TracePrintColor(1, "The TSR establishmet panel position bitmap position value has returned an error -> status %d", statusEstablish);
      Log_AddEntry(LOG_LEVEL.WARNING, "KO", "[Script %s] The script can�t resolve the TSR establishmet panel position (status %d)", scriptname, statusEstablish);

      return false;
    }

  return true;
}

function PE_Manager_UserLogout() {

  Screen_SetFocus(appname, windowtitle);

  InpSim_Mouse_Click(posx, posy);
  InpSim_Mouse_Click(posx + 70, posy + 40);
  TracePrintColor(1, "x = %d e y = %d", posx, posy);

  InpSim_Key_ClickByLiteral("TAB", 100);
  InpSim_Key_ClickByLiteral("TAB", 100);
  InpSim_Key_ClickByLiteral("SPACEBAR", 100);

  Sleep(2000);
}

function PE_Manager_UserLogin() {

  InpSim_Mouse_Click(posx, posy);
  InpSim_Mouse_Click(posx + 70, posy + 40);

  InpSim_Key_ClickByLiteral("TAB", 1000);
  InpSim_Key_ClickByLiteral("SPACEBAR", 1);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByText(user, 100);
  TracePrintColor(1, "Entered user value '%s'", user);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByText(password, 100);
  TracePrintColor(1, "Entered password value '%s'", password);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("ENTER", 1);

}

function PE_Manager_CleanTSR() {

  InpSim_Mouse_Click(posxEstablishTSRpanel, posyEstablishTSRpanel);

  for (i = 0; i < 9; i++) {
    InpSim_Key_ClickByLiteral("TAB", 1);
  }

  InpSim_Key_ClickByLiteral("SPACEBAR", 1);

}

function PE_Manager_ActivateTSR()
{
  
  var outActive = { value: 0 };
  var statusActive = Screen_GetPosX(appname, windowtitle, namebmpActivateTSR, outActive);

  //We search the non-acivated version of the bmp
  if(statusActive != WND_BMP_STATUS.NOBITMAP)
  {
   TracePrintColor(1, "The CEM has not returned the TSR values-> status %d", statusActive);
   Log_AddEntry(LOG_LEVEL.INFO, "KO", "[LTV %s] The CEM does not confirm the LTV configuration values (posx returns status %d).", ltvname, statusActive);
   PE_Manager_CleanTSR();
   return false;
  }
  
  TracePrintColor(1, "The activate TSR button is active");

  InpSim_Mouse_Click(posxEstablishTSRpanel, posyEstablishTSRpanel);

  for (i = 0; i < 9; i++) {
    InpSim_Key_ClickByLiteral("TAB", 1);
  }

  InpSim_Key_ClickByLiteral("SPACEBAR", 1);

  return true;

  }

function PE_Manager_PurgeTSR() {

  // ! -> Needs checking
  InpSim_Mouse_Click(posxEstablishTSRpanel + 50, posyEstablishTSRpanel);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("SPACEBAR", 1);

}


function PE_Manager_DeleteTSR() {

  // ! -> Needs checking
  InpSim_Mouse_Click(posxEstablishTSRpanel + 50, posyEstablishTSRpanel);

  //We choose the firt TSR from the dropdown
  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("DOWN ARROW", 1);

  InpSim_Key_ClickByLiteral("TAB", 1);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("SPACEBAR", 1);
  Sleep(1000);

  PE_Manager_PurgeTSR();

}
function PE_Manager_EstablishTSR(TSR)
{
  
  var isEstablished;

  Screen_SetFocus(appname, windowtitle);

  PE_Manager_CleanTSR();

  InpSim_Mouse_Click(posxEstablishTSRpanel, posyEstablishTSRpanel);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByText(TSR.pkInicio, 1);
  TracePrintColor(1, "Entered 'pkInicio' = %s", TSR.pkInicio);

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByText(TSR.pkFin, 1);
  TracePrintColor(1, "Entered 'pkFin' = %s", TSR.pkFin);

  // ! viaInicio -> String to number
  InpSim_Key_ClickByLiteral("TAB", 1);
  for (i = 0; i < 2; i++) {
    InpSim_Key_ClickByLiteral("DOWN ARROW", 1);
  }
  TracePrintColor(1, "Entered 'viaInicio' = 1B");

  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByText(TSR.velocidad, 1);
  TracePrintColor(1, "Entered 'velocidad' %s", TSR.velocidad);

  InpSim_Key_ClickByLiteral("TAB", 1);
  TracePrintColor(1, "Entered 'Sin Desvio'");

  // ! viaFin -> String to number
  InpSim_Key_ClickByLiteral("TAB", 1);
  for (i = 0; i < 2; i++) {
    InpSim_Key_ClickByLiteral("DOWN ARROW", 1);
  }
  TracePrintColor(1, "Entered 'viaFin' = 1B");

  // ! Causa
  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("DOWN ARROW", 1);
  TracePrintColor(1, "Entered viaFin = 'Trabajos en via'");

  // ! Responsable
  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("DOWN ARROW", 1);
  TracePrintColor(1, "Entered viaFin = 'Mantenimiento' ");

  InpSim_Key_ClickByLiteral("TAB", 1);
  // Establish button
  InpSim_Key_ClickByLiteral("TAB", 1);
  InpSim_Key_ClickByLiteral("SPACEBAR", 1);
  TracePrintColor(1, "Established button set");

  Sleep(3000);

  PE_Manager_ActivateTSR();

  TracePrintColor(1, "Established TSR bmp search start");

  var outEstablished = { value: 0 };
  var statusEstablished = Screen_GetPosX(appname, windowtitle, namebmpTSREstablished, outEstablished);
  
  if(statusEstablished == WND_BMP_STATUS.NOBITMAP || statusEstablished != WND_BMP_STATUS.OK)
  {
    TracePrintColor(1, "The 'established TSR' confirmation bitmap has not been found -> status %d", statusEstablished);
    Log_AddEntry(LOG_LEVEL.INFO, "KO", "[LTV %s] The TSR has not been established.", TSR.name);
    return false;
  }

  Log_AddEntry(LOG_LEVEL.INFO, "OK", "[LTV %s] The TSR has been established!", TSR.name);

  PE_Manager_DeleteTSR();

  return true;
}


function PE_Manager_End()
{

  return true;
}

