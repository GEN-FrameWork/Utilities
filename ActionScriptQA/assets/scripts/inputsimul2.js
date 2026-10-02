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
 
  //Screen_SetPosition(appname, windowtitle, 10, 10);
  //Screen_Resize(appname, windowtitle, 700, 250);
  //Screen_SetFocus(appname, windowtitle);

  Screen_SetBmpFindCFG(0, 10);

  var outx = { value: 0 };
  var outy = { value: 0 };
  var status = Screen_GetPosXY(appname, windowtitle, maskbitmapname, maskbitmapname2, outx, outy);
  var posx = outx.value;
  var posy = outy.value;

  TracePrintColor(1, "Position of %s %d, %d (status=%d)", appname, posx, posy, status);

  Screen_Minimize(appname, windowtitle, false);

  if(status == 0)
    {
      InpSim_Mouse_Click(posx, posy);

      
    }
   else 
    {
      TracePrintColor(4, "Error in Position of %s status=%d !!!", appname, status);
    }

  Log_AddEntry(1, "Script", "[script %s] Terminate application: %s", scriptname, appname);

  Sleep(5000);

  TerminateAplicationWithWindow(appname, windowtitle);  

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
  
}

