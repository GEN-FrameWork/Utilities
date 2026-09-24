// ----------------------------------------------------------------------------
// Example Scrip + Input Simulate
// ----------------------------------------------------------------------------


function main()
{
  var scriptname      = "inputsimul2.js"
  var appname         = "java.exe";
  var windowtitle     = "PCE_ZR";
  var maskbitmapname  = "establecerLTV.png";
  
  TracePrintColor(1, "Start script");
  
  var posx = Window_GetPosX(appname, windowtitle, maskbitmapname);
  var posy = Window_GetPosY(appname, windowtitle, maskbitmapname);

  TracePrintColor(1, "Position of %s %d, %d  %08X, %08X", appname, posx, posy, posx, posy);

  //Sleep(5000);
  
}

