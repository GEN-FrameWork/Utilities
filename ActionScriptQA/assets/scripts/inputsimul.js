// ----------------------------------------------------------------------------
// inputsimul Script
// ----------------------------------------------------------------------------


function main()
{
  var scriptname      = "inputsimul.js";
  var appname         = "WINWORD.EXE";  
  var apppath         =  "C:\\Program Files\\Microsoft Office\\root\\Office16\\" + appname;
  var windowtitle     = "Word";
  var maskbitmapname  = "inputsimul.png";
  var maskbitmapname2 = "inputsimul2.png";

  Log_Ini("scripts.log","ActionScript"); 

  Log_CFG_SetFilters("Script", 7); 

  Log_AddEntry(1, "Script", "[script %s] Iniciado Test...", scriptname);
  
  ExecApplication(apppath);
  
  Log_AddEntry(1, "Script", "[script %s] Exec application: %s", scriptname, appname);
 
  Screen_SetPosition(appname, windowtitle, 10, 10);
  Screen_Resize(appname, windowtitle, 700, 250);
  Screen_SetFocus(appname, windowtitle);

  var outx = { value: 0 };
  var outy = { value: 0 };
  var status = Screen_GetPosXY(appname, windowtitle, maskbitmapname, maskbitmapname2, outx, outy);
  var posx = outx.value;
  var posy = outy.value;

  TracePrintColor(1, "Position of %s %d, %d (status=%d)", appname, posx, posy, status);

  Screen_Minimize(appname, windowtitle, true);
  Sleep(1000);

  Screen_Minimize(appname, windowtitle, false);
  Sleep(1000);

  Screen_Maximize(appname, windowtitle, true);
  Sleep(1000);

  Screen_Maximize(appname, windowtitle, false);
  Sleep(1000);

  Screen_SetFocus(appname, windowtitle);


  //InpSim_Mouse_Click(posx, posy);
  //Sleep(3000);
  
  /*
  InpSim_Mouse_Click(posx + 30, posy + 50);

  Sleep(1000);

  InpSim_Mouse_Click(posx + 30, posy + 50);

  Sleep(1000);
  

  InpSim_Mouse_Click(posx + 80, posy + 450);

  Sleep(1000);

  InpSim_Key_ClickByLiteral("A", 100);
  InpSim_Key_ClickByLiteral("ENTER", 100);

  Log_AddEntry(1, "Script", "[script %s] Simulate Key A + Enter.", scriptname)
  
  InpSim_Key_ClickByLiteral("!"  , 1);
  InpSim_Key_ClickByLiteral("@"  , 1);
  InpSim_Key_ClickByLiteral("#"  , 1);
  InpSim_Key_ClickByLiteral("$"  , 1);
  InpSim_Key_ClickByLiteral("%"  , 1);
  InpSim_Key_ClickByLiteral("^"  , 1);
  InpSim_Key_ClickByLiteral("&"  , 1);
  InpSim_Key_ClickByLiteral("*"  , 1);
  InpSim_Key_ClickByLiteral("("  , 1);
  InpSim_Key_ClickByLiteral(")"  , 1);
  InpSim_Key_ClickByLiteral("_"  , 1);
  InpSim_Key_ClickByLiteral("+"  , 1);
  InpSim_Key_ClickByLiteral("-"  , 1);
  InpSim_Key_ClickByLiteral("="  , 1);
  InpSim_Key_ClickByLiteral("["  , 1);
  InpSim_Key_ClickByLiteral("]"  , 1);
  InpSim_Key_ClickByLiteral("{"  , 1);
  InpSim_Key_ClickByLiteral("}"  , 1);
  InpSim_Key_ClickByLiteral("|"  , 1);
  InpSim_Key_ClickByLiteral(";"  , 1);
  InpSim_Key_ClickByLiteral(":"  , 1);
  InpSim_Key_ClickByLiteral("'"  , 1);
  InpSim_Key_ClickByLiteral(","  , 1);
  InpSim_Key_ClickByLiteral("."  , 1);
  InpSim_Key_ClickByLiteral("<"  , 1);   
  InpSim_Key_ClickByLiteral("?"  , 1);
  InpSim_Key_ClickByLiteral("/"  , 1);
  InpSim_Key_ClickByLiteral("\\" , 1);
  InpSim_Key_ClickByLiteral("\"" , 1);
  
  //InpSim_Key_ClickByLiteral("�"  , 1);
  //InpSim_Key_ClickByLiteral("�"  , 1);
  //InpSim_Key_ClickByLiteral("�"  , 1);
  //InpSim_Key_ClickByLiteral("�"  , 1);
  //InpSim_Key_ClickByLiteral("�"  , 1);
 
  InpSim_Key_ClickByLiteral("ENTER", 1);

  InpSim_Key_ClickByText("Texto De Prueba 0123456789 !@#$%^&*()_+-=[]{}|;:',.<����?/\\\"�", 1);
  */

  /*
  Sleep(1000);

  Log_AddEntry(1, "Script", "[script %s] Terminate application: %s", scriptname, appname);

  TerminateAplicationWithWindow(appname, windowtitle);  

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
  */
  
}

