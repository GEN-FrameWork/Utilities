// ----------------------------------------------------------------------------
// keyclickbytext.js — verify InpSim_Key_ClickByText return + visible typing
// ----------------------------------------------------------------------------


function main()
{
  var scriptname  = "keyclickbytext.js";
  var appname     = "WINWORD.EXE";
  var apppath     = "C:\\Program Files\\Microsoft Office\\root\\Office16\\" + appname;
  // Open a real document (not the Word Home/start screen) so typing has a caret target.
  var docpath     = "E:\\Projects\\GEN_FrameWork\\Utilities\\ActionScriptQA\\assets\\scripts\\keyclickbytext_blank.txt";
  var windowtitle = "Word";
  var okcount     = 0;
  var failcount   = 0;
  var focused     = false;
  var i           = 0;

  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);

  Log_AddEntry(1, "Script", "[script %s] Iniciado test Key_ClickByText...", scriptname);
  Log_AddEntry(1, "Script", "[script %s] Platform=%s Hardware=%s", scriptname, System_GetType(), System_GetHardwareType());

  // Word is a Windows-only target; skip on Linux/Android/etc.
  if(!System_IsWindows())
    {
      Log_AddEntry(1, "Script", "[script %s] SKIP Word test on non-Windows (%s)", scriptname, System_GetType());
      TracePrintColor(1, "[%s] SKIP Word test on %s", scriptname, System_GetType());
      return;
    }

  if(!ExecApplication(apppath, docpath))
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL ExecApplication(%s)", scriptname, apppath);
      TracePrintColor(4, "[%s] FAIL ExecApplication", scriptname);
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      return;
    }

  Log_AddEntry(1, "Script", "[script %s] Exec application: %s %s", scriptname, appname, docpath);

  var targetx    = 40;
  var targety    = 40;
  var targetw    = 1000;
  var targeth    = 700;
  var outx       = { value: 0 };
  var outy       = { value: 0 };
  var posx       = 0;
  var posy       = 0;
  var posok      = false;
  var status     = 1;

  for(i = 0; i < 40; i++)
    {
      Sleep(500);

      // Detect Word window position before moving it.
      status = Screen_GetPosXY(appname, windowtitle, outx, outy);
      if(status == 0)
        {
          posx = outx.value;
          posy = outy.value;

          if(!posok)
            {
              Log_AddEntry(1, "Script", "[script %s] Detected Word pos=%d,%d", scriptname, posx, posy);
              TracePrintColor(1, "[%s] Detected Word pos=%d,%d", scriptname, posx, posy);
              posok = true;
            }

          Screen_SetPosition(appname, windowtitle, targetx, targety);
          Screen_Resize(appname, windowtitle, targetw, targeth);
          Sleep(200);

          status = Screen_GetPosXY(appname, windowtitle, outx, outy);
          if(status == 0)
            {
              posx = outx.value;
              posy = outy.value;
              Log_AddEntry(1, "Script", "[script %s] After move/resize pos=%d,%d size=%dx%d", scriptname, posx, posy, targetw, targeth);
            }

          if(Screen_SetFocus(appname, windowtitle))
            {
              focused = true;
              break;
            }
        }
    }

  if(!focused)
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL Screen_SetFocus after wait", scriptname);
      TracePrintColor(4, "[%s] FAIL Screen_SetFocus — keys would go to wrong window", scriptname);
      TerminateAplicationWithWindow(appname, windowtitle);
      Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
      return;
    }

  okcount = okcount + 1;
  Log_AddEntry(1, "Script", "[script %s] PASS Screen detect/move/resize/focus (pos=%d,%d)", scriptname, posx, posy);
  TracePrintColor(1, "[%s] PASS Screen detect/move/resize/focus", scriptname);

  // Click inside the document body relative to the window origin.
  var clickx = posx + 480;
  var clicky = posy + 380;

  // Ensure caret is in the document body (not ribbon / search).
  InpSim_Key_ClickByLiteral("ESC", 30);
  Sleep(200);
  Screen_SetFocus(appname, windowtitle);
  InpSim_Mouse_Click(clickx, clicky);
  Sleep(250);
  InpSim_Mouse_Click(clickx, clicky);
  Sleep(250);
  Screen_SetFocus(appname, windowtitle);
  Sleep(200);

  // Ctrl+End then type on a new line for a clean visible result.
  InpSim_Key_Press(0x11);
  InpSim_Key_ClickByLiteral("END", 30);
  InpSim_Key_UnPress(0x11);
  Sleep(100);
  InpSim_Key_ClickByLiteral("ENTER", 30);
  Sleep(100);

  var status = InpSim_Key_ClickByText("Qa Test 0123", 40);
  if(status)
    {
      okcount = okcount + 1;
      Log_AddEntry(1, "Script", "[script %s] PASS supported text returned true", scriptname);
      TracePrintColor(1, "[%s] PASS supported text -> true (check Word document)", scriptname);
    }
  else
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL supported text returned false", scriptname);
      TracePrintColor(4, "[%s] FAIL supported text -> false", scriptname);
    }

  status = InpSim_Key_ClickByText("", 1);
  if(status)
    {
      okcount = okcount + 1;
      Log_AddEntry(1, "Script", "[script %s] PASS empty text returned true", scriptname);
      TracePrintColor(1, "[%s] PASS empty text -> true", scriptname);
    }
  else
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL empty text returned false", scriptname);
      TracePrintColor(4, "[%s] FAIL empty text -> false", scriptname);
    }

  status = InpSim_Key_ClickByText("A\tB", 1);
  if(!status)
    {
      okcount = okcount + 1;
      Log_AddEntry(1, "Script", "[script %s] PASS unsupported char returned false", scriptname);
      TracePrintColor(1, "[%s] PASS unsupported char -> false", scriptname);
    }
  else
    {
      failcount = failcount + 1;
      Log_AddEntry(4, "Script", "[script %s] FAIL unsupported char returned true", scriptname);
      TracePrintColor(4, "[%s] FAIL unsupported char -> true", scriptname);
    }

  Sleep(5000);
  TerminateAplicationWithWindow(appname, windowtitle);

  Log_AddEntry(1, "Script", "[script %s] Result ok=%d fail=%d", scriptname, okcount, failcount);
  TracePrintColor(1, "[%s] Result ok=%d fail=%d", scriptname, okcount, failcount);

  if(failcount != 0)
    {
      TracePrintColor(4, "[%s] Key_ClickByText verification FAILED", scriptname);
    }
  else
    {
      TracePrintColor(1, "[%s] Key_ClickByText verification OK", scriptname);
    }

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
}
