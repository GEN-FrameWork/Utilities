// ----------------------------------------------------------------------------
// Tests_UISystem.js — launch UI_System, click sidebar via image find (layout
// fallback), assert #[TESTS_RESULT]# feedback through TraceServer (UDP 10001).
// Catalog / evidence names follow GetNameScript() at runtime:
//   <base>_ListTest.json
//   evidence/<base>_<YYYYMMDD_HHMMSS>.csv
// Script progress goes to ActionScriptQA console (not XTrace).
// ----------------------------------------------------------------------------


function ScriptBaseName(scriptname)
{
  var base = scriptname;
  var dot  = -1;

  if(base == "" || base == null || base == undefined)
    {
      base = GetNameScript();
    }

  dot = base.indexOf(".");
  if(dot >= 0)
    {
      base = base.substring(0, dot);
    }

  return base;
}


function ListTestFileName(scriptname)
{
  return ScriptBaseName(scriptname) + "_ListTest.json";
}


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


function ParseTestsResultError(line, id)
{
  var marker = "#[TESTS_RESULT]# :," + id + ",";
  var idx    = -1;
  var rest   = "";
  var end    = -1;
  var i      = 0;
  var ch     = "";
  var num    = "";

  if(line == "") return -999;

  idx = line.indexOf(marker);
  if(idx < 0) return -999;

  rest = line.substring(idx + marker.length);
  end  = rest.indexOf(",");
  if(end >= 0)
    {
      rest = rest.substring(0, end);
    }

  for(i = 0; i < rest.length; i++)
    {
      ch = rest.substring(i, i + 1);
      if((ch >= "0" && ch <= "9") || (ch == "-" && i == 0))
        {
          num = num + ch;
        }
       else
        {
          break;
        }
    }

  if(num == "" || num == "-") return -999;

  return parseInt(num, 10);
}


function WaitTestResult(id, timeoutms, scriptname)
{
  var waited      = 0;
  var step        = 200;
  var got         = "";
  var needle      = "#[TESTS_RESULT]# :," + id + ",";
  var description = TraceTests_GetDescription(id);
  var error       = -999;

  while(waited < timeoutms)
    {
      got = TraceServer_Get(-1, needle, 1);
      if(got != "")
        {
          error = ParseTestsResultError(got, id);
          Log_AddEntry(1, "Script", "[script %s] TESTS_RESULT id=%d error=%d desc=%s line=%s", scriptname, id, error, description, got);
          return error;
        }

      Sleep(step);
      waited = waited + step;
    }

  DumpTraces(scriptname, needle);
  Log_AddEntry(4, "Script", "[script %s] TIMEOUT TESTS_RESULT id=%d desc=%s", scriptname, id, description);
  return -999;
}


function ReleaseTestsCatalog(scriptname)
{
  TraceTests_DeleteAll();
  Log_AddEntry(1, "Script", "[script %s] TraceTests_DeleteAll", scriptname);
}


function EvidenceFileName(scriptname)
{
  return ScriptBaseName(scriptname) + "_" + FileCSV_GetShortDateTime() + ".csv";
}


function EvidenceStart(scriptname)
{
  var dir  = GetPathScript() + "evidence";
  var path = dir + "\\" + EvidenceFileName(scriptname);

  MakeDir(dir);

  if(!FileCSV_Create(path)) return false;
  if(!FileCSV_SetHeader("ID", "Description", "Result")) return false;
  if(!FileCSV_Save()) return false;

  return true;
}


function EvidenceAdd(id, description, result)
{
  var idstr = "";

  if(id !== "" && id !== null && id !== undefined)
    {
      idstr = "" + (id | 0);
    }

  return FileCSV_AddRecord(idstr, description, result);
}


function EvidenceEnd()
{
  return FileCSV_Close();
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
  var scriptname  = GetNameScript();
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
  var error       = -999;
  var catalogpath = "";
  var description = "";

  // layoutx/layouty = center of hit target (root ypos is BOTTOM edge).
  // testid matches <scriptname>_ListTest.json in this same scripts folder.
  var navitems = [
    { bmp: "uisys_nav_resumen.png"      , eid: "nav-resumen-btn"      , x: 105, y: 94  , testid: 1001 },
    { bmp: "uisys_nav_cpu.png"          , eid: "nav-cpu-btn"          , x: 105, y: 150 , testid: 1002 },
    { bmp: "uisys_nav_memoria.png"      , eid: "nav-memoria-btn"      , x: 105, y: 206 , testid: 1003 },
    { bmp: "uisys_nav_red.png"          , eid: "nav-red-btn"          , x: 105, y: 262 , testid: 1004 },
    { bmp: "uisys_nav_disco.png"        , eid: "nav-disco-btn"        , x: 105, y: 318 , testid: 1005 },
    { bmp: "uisys_nav_procesos.png"     , eid: "nav-procesos-btn"     , x: 105, y: 374 , testid: 1006 },
    { bmp: "uisys_nav_alertas.png"      , eid: "nav-alertas-btn"      , x: 105, y: 430 , testid: 1007 },
    { bmp: "uisys_nav_configuracion.png", eid: "nav-configuracion-btn", x: 105, y: 486 , testid: 1008 }
  ];

  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);

  Log_AddEntry(1, "Script", "[script %s] Start UI_System TESTS_RESULT feedback test", scriptname);
  Console_Printf("[%s] Start UI_System TESTS_RESULT feedback test\n", scriptname);

  if(!EvidenceStart(scriptname))
    {
      Log_AddEntry(4, "Script", "[script %s] FAIL EvidenceStart/FileCSV", scriptname);
      Console_Printf("[%s] FAIL EvidenceStart/FileCSV\n", scriptname);
      return;
    }

  Log_AddEntry(1, "Script", "[script %s] Evidence CSV %s", scriptname, FileCSV_GetPath());
  Console_Printf("[%s] Evidence CSV %s\n", scriptname, FileCSV_GetPath());

  if(!System_IsWindows())
    {
      Log_AddEntry(1, "Script", "[script %s] SKIP on non-Windows (%s)", scriptname, System_GetType());
      Console_Printf("[%s] SKIP on %s\n", scriptname, System_GetType());
      EvidenceAdd("", "Platform check", "SKIP");
      EvidenceEnd();
      return;
    }

  catalogpath = GetPathScript() + ListTestFileName(scriptname);
  if(!TraceTests_Load(catalogpath))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL TraceTests_Load(%s)", scriptname, catalogpath);
      Console_Printf("[%s] FAIL TraceTests_Load\n", scriptname);
      EvidenceAdd("", "TraceTests_Load", "FAIL");
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      EvidenceEnd();
      return;
    }

  okcount = okcount + 1;
  EvidenceAdd("", "TraceTests_Load", "PASS");
  Log_AddEntry(1, "Script", "[script %s] PASS TraceTests_Load %s", scriptname, catalogpath);

  for(i = 0; i < navitems.length; i++)
    {
      if(!TraceTests_Exists(navitems[i].testid))
        {
          failcount = failcount + 1;
          Log_AddEntry(4, "Script", "[script %s] FAIL catalog missing id=%d", scriptname, navitems[i].testid);
        }
    }

  if(!TraceTests_Exists(1099))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL catalog missing id=1099", scriptname);
    }

  if(!TraceServer_Ini(10001))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL TraceServer_Ini(10001)", scriptname);
      Console_Printf("[%s] FAIL TraceServer_Ini\n", scriptname);
      EvidenceAdd("", "TraceServer_Ini", "FAIL");
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      ReleaseTestsCatalog(scriptname);
      EvidenceEnd();
      return;
    }

  okcount = okcount + 1;
  EvidenceAdd("", "TraceServer_Ini", "PASS");
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
      Console_Printf("[%s] FAIL ExecApplication\n", scriptname);
      EvidenceAdd("", "ExecApplication", "FAIL");
      TraceServer_End();
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      ReleaseTestsCatalog(scriptname);
      EvidenceEnd();
      return;
    }

  okcount = okcount + 1;
  EvidenceAdd("", "ExecApplication", "PASS");
  Log_AddEntry(1, "Script", "[script %s] PASS ExecApplication %s", scriptname, apppath);
  Console_Printf("[%s] Launched %s\n", scriptname, apppath);

  if(!WaitWindow(appname, windowtitle, 30000))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL window '%s' not found", scriptname, windowtitle);
      Console_Printf("[%s] FAIL window not found\n", scriptname);
      EvidenceAdd("", "WaitWindow", "FAIL");
      TerminateAplication(appname);
      TraceServer_End();
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      ReleaseTestsCatalog(scriptname);
      EvidenceEnd();
      return;
    }

  status = Screen_GetPosXY(appname, windowtitle, outx, outy);
  winx = outx.value;
  winy = outy.value;
  okcount = okcount + 1;
  EvidenceAdd("", "WaitWindow", "PASS");
  Log_AddEntry(1, "Script", "[script %s] PASS window pos=%d,%d", scriptname, winx, winy);
  Console_Printf("[%s] Window '%s' pos=%d,%d\n", scriptname, windowtitle, winx, winy);

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
      description = TraceTests_GetDescription(navitems[i].testid);

      TraceServer_Clear();
      Sleep(150);

      if(!ClickNav(appname, windowtitle, navitems[i].bmp, navitems[i].x, navitems[i].y, outx, outy, scriptname))
        {
          failcount = failcount + 1;
          EvidenceAdd(navitems[i].testid, description, "FAIL");
          Console_Printf("[%s] FAIL click %s\n", scriptname, navitems[i].bmp);
          continue;
        }

      Console_Printf("[%s] Clicked %s at %d,%d (id=%d %s)\n", scriptname, navitems[i].bmp, outx.value, outy.value, navitems[i].testid, description);

      error = WaitTestResult(navitems[i].testid, 5000, scriptname);
      if(error == 0)
        {
          okcount = okcount + 1;
          EvidenceAdd(navitems[i].testid, description, "PASS");
          Log_AddEntry(1, "Script", "[script %s] PASS TESTS_RESULT id=%d %s", scriptname, navitems[i].testid, description);
          Console_Printf("[%s] PASS id=%d %s\n", scriptname, navitems[i].testid, description);
        }
       else
        {
          failcount = failcount + 1;
          EvidenceAdd(navitems[i].testid, description, "FAIL");
          Log_AddEntry(4, "Script", "[script %s] FAIL TESTS_RESULT id=%d error=%d %s (count=%d)", scriptname, navitems[i].testid, error, description, TraceServer_GetCount());
          Console_Printf("[%s] FAIL id=%d error=%d\n", scriptname, navitems[i].testid, error);
        }

      Sleep(450);
    }

  TraceServer_Clear();
  Sleep(150);
  outx = { value: 0 };
  outy = { value: 0 };
  description = TraceTests_GetDescription(1099);

  // Close button center ~ (1414, 24) in design canvas.
  if(!ClickNav(appname, windowtitle, "uisys_btn_chrome_close.png", 1414, 24, outx, outy, scriptname))
    {
      failcount = failcount + 1;
      EvidenceAdd(1099, description, "FAIL");
      Console_Printf("[%s] FAIL click close\n", scriptname);
      TerminateAplicationWithWindow(appname, windowtitle);
    }
   else
    {
      Console_Printf("[%s] Clicked close at %d,%d (id=1099 %s)\n", scriptname, outx.value, outy.value, description);

      error = WaitTestResult(1099, 5000, scriptname);
      if(error == 0)
        {
          okcount = okcount + 1;
          EvidenceAdd(1099, description, "PASS");
          Log_AddEntry(1, "Script", "[script %s] PASS TESTS_RESULT id=1099 %s", scriptname, description);
          Console_Printf("[%s] PASS id=1099 %s\n", scriptname, description);
        }
       else
        {
          failcount = failcount + 1;
          EvidenceAdd(1099, description, "FAIL");
          Log_AddEntry(4, "Script", "[script %s] FAIL TESTS_RESULT id=1099 error=%d (count=%d)", scriptname, error, TraceServer_GetCount());
          Console_Printf("[%s] FAIL id=1099 error=%d\n", scriptname, error);
          TerminateAplicationWithWindow(appname, windowtitle);
        }
    }

  Sleep(1000);
  TraceServer_End();

  Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
  Console_Printf("[%s] Result ok=%d fail=%d\n", scriptname, okcount, failcount);

  if(failcount != 0)
    {
      Console_Printf("[%s] UI_System TraceServer feedback FAILED\n", scriptname);
      Log_AddEntry(4, "Script", "[script %s] UI_System TraceServer feedback FAILED", scriptname);
    }
   else
    {
      Console_Printf("[%s] UI_System TraceServer feedback OK\n", scriptname);
      Log_AddEntry(1, "Script", "[script %s] UI_System TraceServer feedback OK", scriptname);
    }

  ReleaseTestsCatalog(scriptname);
  EvidenceEnd();
  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
}
