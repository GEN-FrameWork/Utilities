// ----------------------------------------------------------------------------
// Example Scrip + Input Simulate
// ----------------------------------------------------------------------------


function main()
{
  var scriptname      = "inputsimul.js";
  //var appname         = "wordpad.exe";
  //var apppath         = "C:\\Program Files (x86)\\Windows NT\\Accessories\\" + appname;
  //var windowtitle     = "WordPad";


  var appname         = "canvas2D.exe";  
  var apppath         = "D:\\Projects\\GENFrameWork\\Examples\\Graphics\\Canvas2D\\Platforms\\Windows\\x64\\" + appname;
  //var apppath         = "/mnt/d/Projects/GENFrameWork/Examples/Graphics/Canvas2D/Platforms/Linux/x64/" + appname;
  //var appname         = "canvas2D";
  var windowtitle     = "Canvas 2D";
  var maskbitmapname  = "inputsimul.png";
  var maskbitmapname2 = "inputsimul2.png";

  Log_AddEntry(1, "Script", "[script %s] Iniciado Test...", scriptname);

  ExecApplication(apppath);

  Log_AddEntry(1, "Script", "[script %s] Exec application: %s", scriptname, appname);
 
  //Window_SetPosition(appname, windowtitle, 10, 10);
  //Window_Resize(appname, windowtitle, 700, 250);
  //Window_SetFocus(appname, windowtitle);

  Window_SetBmpFindCFG(0, 10);

  var posx = Window_GetPosX(appname, windowtitle, maskbitmapname, maskbitmapname2);
  var posy = Window_GetPosY(appname, windowtitle, maskbitmapname, maskbitmapname2);

  TracePrintColor(1, "Position of %s %d, %d %08X, %08X", appname, posx, posy, posx, posy);

  Window_Minimize(appname, windowtitle, false);

  if(posx != 0x00FFFFFE)
    {
      InpSim_Mouse_Click(posx, posy);

      
    }
   else 
    {
      TracePrintColor(4, "Error in Position of %s %d, %d %08X, %08X !!!", appname, posx, posy, posx, posy);
    }

  Log_AddEntry(1, "Script", "[script %s] Terminate application: %s", scriptname, appname);

  Sleep(5000);

  TerminateAplicationWithWindow(appname, windowtitle);  

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
  
}

