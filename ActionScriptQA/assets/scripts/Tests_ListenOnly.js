function main()
{
  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);
  TraceServer_Ini(10001);
  TraceServer_Clear();
  ExecApplication("..\\..\\..\\..\\..\\..\\Examples\\Graphics\\UI_System\\CMake\\Build\\Windows\\intel32\\ui_system.exe");
  Sleep(6000);
  var count = TraceServer_GetCount();
  Log_AddEntry(1, "Script", "[listen] count=%d", count);
  var i=0;
  for(i=0;i<count;i++)
    {
      Log_AddEntry(1, "Script", "[listen] %s", TraceServer_Peek(i));
    }
  TerminateAplication("ui_system.exe");
  TraceServer_End();
}
