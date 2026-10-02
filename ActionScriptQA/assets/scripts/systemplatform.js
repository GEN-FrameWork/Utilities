// ----------------------------------------------------------------------------
// systemplatform.js — smoke System_* platform adaptation APIs
// ----------------------------------------------------------------------------

function main()
{
  var scriptname = "systemplatform.js";
  var platform   = System_GetType();
  var hardware   = System_GetHardwareType();
  var osid       = System_GetOperativeSystemID();
  var user       = System_GetUser();
  var mem        = System_GetFreeMemoryPercent();

  Log_Ini("scripts.log", "ActionScript");
  Log_CFG_SetFilters("Script", 7);

  Log_AddEntry(1, "Script", "[script %s] Platform=%s Hardware=%s MemFree=%d%%", scriptname, platform, hardware, mem);
  Log_AddEntry(1, "Script", "[script %s] OSID=%s User=%s", scriptname, osid, user);
  Log_AddEntry(1, "Script", "[script %s] IsWindows=%d IsLinux=%d IsAndroid=%d", scriptname, System_IsWindows(), System_IsLinux(), System_IsAndroid());
  TracePrintColor(1, "[%s] %s / %s / win=%d linux=%d", scriptname, platform, hardware, System_IsWindows(), System_IsLinux());

  if(System_IsWindows())
    {
      Log_AddEntry(1, "Script", "[script %s] Windows branch OK", scriptname);
    }
  else if(System_IsLinux())
    {
      Log_AddEntry(1, "Script", "[script %s] Linux branch OK (skip Word-like tools)", scriptname);
    }

  Log_AddEntry(1, "Script", "[script %s] End script.", scriptname);
}
