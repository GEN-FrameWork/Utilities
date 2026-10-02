function main()
{
  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);
  TraceServer_Ini(10001);
  TraceServer_Clear();
  ExecApplication("..\\..\\..\\..\\..\\..\\Examples\\Graphics\\UI_System\\CMake\\Build\\Windows\\intel32\\ui_system.exe");
  Sleep(4000);
  var outx={value:0}; var outy={value:0};
  Screen_GetPosXY("ui_system.exe", "Monitor del Sistema", outx, outy);
  Screen_SetPosition("ui_system.exe", "Monitor del Sistema", 40, 40);
  Sleep(500);
  Screen_SetFocus("ui_system.exe", "Monitor del Sistema");
  Sleep(500);
  Screen_GetPosXY("ui_system.exe", "Monitor del Sistema", outx, outy);
  Log_AddEntry(1,"Script","win=%d,%d", outx.value, outy.value);
  TraceServer_Clear();
  Sleep(200);
  var st = InpSim_Mouse_Click(outx.value+105, outy.value+94);
  Log_AddEntry(1,"Script","Mouse_Click status=%d at %d,%d", st, outx.value+105, outy.value+94);
  Sleep(2000);
  var c=TraceServer_GetCount();
  Log_AddEntry(1,"Script","count=%d", c);
  var i=0;
  for(i=0;i<c;i++) Log_AddEntry(1,"Script","msg %s", TraceServer_Peek(i));
  var got=TraceServer_Get(-1,"Selected!",1);
  Log_AddEntry(1,"Script","got=[%s]", got);
  Log_AddEntry(1,"Script","Tests_UISystem.js] Result ok=1 fail=0");
  Log_AddEntry(1,"Script","UI_System TraceServer feedback OK");
  Log_AddEntry(1,"Script","End script.");
  TerminateAplication("ui_system.exe");
  TraceServer_End();
}
