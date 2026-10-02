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
  
  var outx = { value: 0 };
  var outy = { value: 0 };
  var status = Screen_GetPosX(appname, windowtitle, maskbitmapname, outx);
  Screen_GetPosY(appname, windowtitle, maskbitmapname, outy);

  TracePrintColor(1, "Position of %s %d, %d (status=%d)", appname, outx.value, outy.value, status);

  //Sleep(5000);
  
}

