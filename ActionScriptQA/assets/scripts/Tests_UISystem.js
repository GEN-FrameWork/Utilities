// ----------------------------------------------------------------------------
// Tests_UISystem.js — launch UI_System, click sidebar via image find (layout
// fallback), assert XTrace feedback through TraceServer (UDP 10001).
// ----------------------------------------------------------------------------


function ResolveUISystemPath()
{
  return [
    "..\\..\\..\\..\\..\\..\\Examples\\Graphics\\UI_System\\CMake\\Build\\Windows\\intel64\\ui_system.exe",
    "..\\..\\..\\..\\..\\..\\Examples\\Graphics\\UI_System\\CMake\\Build\\Windows\\intel32\\ui_system.exe"
  ];
}


function WaitWindow(appname, windowtitle, timeoutms)
{
  var outx   = { value: 0 };
  var outy   = { value: 0 };
  var status = 1;
  var waited = 0;
  var step   = 500;

  while(waited < timeoutms)
    {
      Sleep(step);
      waited = waited + step;
      status = Screen_GetPosXY(appname, windowtitle, outx, outy);
      if(status == 0)
        {
          return true;
        }
    }

  return false;
}


function DumpTraces(scriptname, tag)
{
  var count = TraceServer_GetCount();
  var i     = 0;
  var line  = "";

  Log_AddEntry(1, "Script", "[script %s] DumpTraces %s count=%d", scriptname, tag, count);
  for(i = 0; i < count; i++)
    {
      line = TraceServer_Peek(i);
      Log_AddEntry(1, "Script", "[script %s]   [%d] %s", scriptname, i, line);
    }
}


function WaitTraceContains(needle, timeoutms, scriptname)
{
  var waited = 0;
  var step   = 200;
  var got    = "";

  while(waited < timeoutms)
    {
      got = TraceServer_Get(-1, needle, 1);
      if(got != "")
        {
          return got;
        }

      Sleep(step);
      waited = waited + step;
    }

  DumpTraces(scriptname, needle);
  return "";
}


function ClickNav(appname, windowtitle, bmpname, layoutx, layouty, outx, outy, scriptname)
{
  var status = 1;
  var winx = { value: 0 };
  var winy = { value: 0 };
  var imgstatus = 1;
  var clickstatus = false;

  Screen_SetFocus(appname, windowtitle);
  Sleep(120);

  status = Screen_GetPosXY(appname, windowtitle, bmpname, outx, outy);
  if(status == 0)
    {
      Screen_SetFocus(appname, windowtitle);
      Sleep(80);
      clickstatus = InpSim_Mouse_Click(outx.value, outy.value);
      Log_AddEntry(1, "Script", "[script %s] Image click %s at %d,%d status=%d", scriptname, bmpname, outx.value, outy.value, clickstatus);
      return true;
    }

  // Fallback: re-read window origin (after SetPosition) + layout design coordinates.
  imgstatus = status;
  status = Screen_GetPosXY(appname, windowtitle, winx, winy);
  if(status != 0)
    {
      Log_AddEntry(4, "Script", "[script %s] FAIL find %s imgstatus=%d and window origin status=%d", scriptname, bmpname, imgstatus, status);
      return false;
    }

  outx.value = winx.value + layoutx;
  outy.value = winy.value + layouty;
  Screen_SetFocus(appname, windowtitle);
  Sleep(80);
  clickstatus = InpSim_Mouse_Click(outx.value, outy.value);
  Log_AddEntry(1, "Script", "[script %s] Layout fallback click %s at %d,%d (win=%d,%d layout=%d,%d imgstatus=%d click=%d)", scriptname, bmpname, outx.value, outy.value, winx.value, winy.value, layoutx, layouty, imgstatus, clickstatus);
  return true;
}


function main()
{
  var scriptname  = "Tests_UISystem.js";
  var appname     = "ui_system.exe";
  var windowtitle = "Monitor del Sistema";
  var okcount     = 0;
  var failcount   = 0;
  var paths       = ResolveUISystemPath();
  var launched    = false;
  var apppath     = "";
  var i           = 0;
  var outx        = { value: 0 };
  var outy        = { value: 0 };
  var winx        = 0;
  var winy        = 0;
  var status      = 1;
  var got         = "";

  // layoutx/layouty = center of hit target (root ypos is BOTTOM edge).
  var navitems = [
    { bmp: "uisys_nav_resumen.png"      , eid: "nav-resumen-btn"      , x: 105, y: 94  },
    { bmp: "uisys_nav_cpu.png"          , eid: "nav-cpu-btn"          , x: 105, y: 150 },
    { bmp: "uisys_nav_memoria.png"      , eid: "nav-memoria-btn"      , x: 105, y: 206 },
    { bmp: "uisys_nav_red.png"          , eid: "nav-red-btn"          , x: 105, y: 262 },
    { bmp: "uisys_nav_disco.png"        , eid: "nav-disco-btn"        , x: 105, y: 318 },
    { bmp: "uisys_nav_procesos.png"     , eid: "nav-procesos-btn"     , x: 105, y: 374 },
    { bmp: "uisys_nav_alertas.png"      , eid: "nav-alertas-btn"      , x: 105, y: 430 },
    { bmp: "uisys_nav_configuracion.png", eid: "nav-configuracion-btn", x: 105, y: 486 }
  ];

  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);

  Log_AddEntry(1, "Script", "[script %s] Start UI_System TraceServer feedback test", scriptname);
  TracePrintColor(1, "[%s] Start UI_System TraceServer feedback test", scriptname);

  if(!System_IsWindows())
    {
      Log_AddEntry(1, "Script", "[script %s] SKIP on non-Windows (%s)", scriptname, System_GetType());
      TracePrintColor(1, "[%s] SKIP on %s", scriptname, System_GetType());
      return;
    }

  if(!TraceServer_Ini(10001))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL TraceServer_Ini(10001)", scriptname);
      TracePrintColor(4, "[%s] FAIL TraceServer_Ini", scriptname);
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      return;
    }

  okcount = okcount + 1;
  Log_AddEntry(1, "Script", "[script %s] PASS TraceServer_Ini", scriptname);
  TraceServer_Clear();

  for(i = 0; i < paths.length; i++)
    {
      apppath = paths[i];
      if(ExecApplication(apppath))
        {
          launched = true;
          break;
        }
    }

  if(!launched)
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL ExecApplication (tried relative UI_System builds)", scriptname);
      TracePrintColor(4, "[%s] FAIL ExecApplication", scriptname);
      TraceServer_End();
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      return;
    }

  okcount = okcount + 1;
  Log_AddEntry(1, "Script", "[script %s] PASS ExecApplication %s", scriptname, apppath);
  TracePrintColor(1, "[%s] Launched %s", scriptname, apppath);

  if(!WaitWindow(appname, windowtitle, 30000))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL window '%s' not found", scriptname, windowtitle);
      TracePrintColor(4, "[%s] FAIL window not found", scriptname);
      TerminateAplication(appname);
      TraceServer_End();
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      return;
    }

  status = Screen_GetPosXY(appname, windowtitle, outx, outy);
  winx = outx.value;
  winy = outy.value;
  okcount = okcount + 1;
  Log_AddEntry(1, "Script", "[script %s] PASS window pos=%d,%d", scriptname, winx, winy);
  TracePrintColor(1, "[%s] Window '%s' pos=%d,%d", scriptname, windowtitle, winx, winy);

  Screen_SetPosition(appname, windowtitle, 40, 40);
  Sleep(500);
  Screen_SetFocus(appname, windowtitle);
  Sleep(700);

  status = Screen_GetPosXY(appname, windowtitle, outx, outy);
  if(status == 0)
    {
      winx = outx.value;
      winy = outy.value;
      Log_AddEntry(1, "Script", "[script %s] After SetPosition window pos=%d,%d", scriptname, winx, winy);
    }

  Screen_SetBmpFindCFG(12, 40);
  TraceServer_Clear();
  Sleep(300);

  for(i = 0; i < navitems.length; i++)
    {
      outx = { value: 0 };
      outy = { value: 0 };

      TraceServer_Clear();
      Sleep(150);

      if(!ClickNav(appname, windowtitle, navitems[i].bmp, navitems[i].x, navitems[i].y, outx, outy, scriptname))
        {
          failcount = failcount + 1;
          TracePrintColor(4, "[%s] FAIL click %s", scriptname, navitems[i].bmp);
          continue;
        }

      TracePrintColor(1, "[%s] Clicked %s at %d,%d", scriptname, navitems[i].bmp, outx.value, outy.value);

      got = WaitTraceContains("UI Element [" + navitems[i].eid + "]: Selected!", 5000, scriptname);
      if(got != "")
        {
          okcount = okcount + 1;
          Log_AddEntry(1, "Script", "[script %s] PASS trace: %s", scriptname, navitems[i].eid);
          TracePrintColor(1, "[%s] PASS trace %s", scriptname, navitems[i].eid);
        }
       else
        {
          failcount = failcount + 1;
          Log_AddEntry(4, "Script", "[script %s] FAIL missing trace: %s (count=%d)", scriptname, navitems[i].eid, TraceServer_GetCount());
          TracePrintColor(4, "[%s] FAIL missing trace %s", scriptname, navitems[i].eid);
        }

      Sleep(450);
    }

  TraceServer_Clear();
  Sleep(150);
  outx = { value: 0 };
  outy = { value: 0 };

  // Close button center ~ (1414, 24) in design canvas.
  if(!ClickNav(appname, windowtitle, "uisys_btn_chrome_close.png", 1414, 24, outx, outy, scriptname))
    {
      failcount = failcount + 1;
      TracePrintColor(4, "[%s] FAIL click close", scriptname);
      TerminateAplicationWithWindow(appname, windowtitle);
    }
   else
    {
      TracePrintColor(1, "[%s] Clicked close at %d,%d", scriptname, outx.value, outy.value);

      got = WaitTraceContains("UI Element [btn_chrome_close]: Selected!", 5000, scriptname);
      if(got != "")
        {
          okcount = okcount + 1;
          Log_AddEntry(1, "Script", "[script %s] PASS trace close btn_chrome_close", scriptname);
          TracePrintColor(1, "[%s] PASS trace btn_chrome_close", scriptname);
        }
       else
        {
          failcount = failcount + 1;
          Log_AddEntry(4, "Script", "[script %s] FAIL missing close trace (count=%d)", scriptname, TraceServer_GetCount());
          TracePrintColor(4, "[%s] FAIL missing close trace", scriptname);
          TerminateAplicationWithWindow(appname, windowtitle);
        }
    }

  Sleep(1000);
  TraceServer_End();

  Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
  TracePrintColor(1, "[%s] Result ok=%d fail=%d", scriptname, okcount, failcount);

  if(failcount != 0)
    {
      TracePrintColor(4, "[%s] UI_System TraceServer feedback FAILED", scriptname);
      Log_AddEntry(4, "Script", "[script %s] UI_System TraceServer feedback FAILED", scriptname);
    }
   else
    {
      TracePrintColor(1, "[%s] UI_System TraceServer feedback OK", scriptname);
      Log_AddEntry(1, "Script", "[script %s] UI_System TraceServer feedback OK", scriptname);
    }

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
}
