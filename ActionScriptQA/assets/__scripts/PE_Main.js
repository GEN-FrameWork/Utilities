 


function main()
{   
  
  if(!Log_Ini(scriptlog, "ScriptsExample"))
  {
    return;
  }
  
  Log_CFG_SetLimit(30000, 3);
  Log_CFG_SetFilters("Script, OK, KO", 0x000F);
  Log_CFG_SetBackup(true, 10, true);     
   
  TraceClearScreen(true);
  TraceClearMsgsStatus(true);

  TracePrintColor(TRACE_COLOR.BLUE, "Start script...");
  
  if(PE_Manager_Ini())
    {

      //PE_Manager_UserLogout();
      //Sleep(1000);
      //PE_Manager_UserLogin();

      TracePrintColor(1, "TSR List:");

      for(c=0; c<TSRs_list.length; c++)
      {

          const TSR = TSRs_list[c];
             
          Log_AddEntry(LOG_LEVEL.INFO, "OK", "Testing values [%s, %s, %s, %s, %s, %s, %s, %s, %s]" , TSR.name
                                                                                                   , TSR.pkInicio
                                                                                                   , TSR.viaInicio
                                                                                                   , TSR.pkFin
                                                                                                   , TSR.viaFin
                                                                                                   , TSR.velocidad
                                                                                                   , TSR.desvio
                                                                                                   , TSR.causa
                                                                                                   , TSR.responsable);

          PE_Manager_EstablishTSR(TSRs_list[c]);

          Sleep(3000);
      
        }

      PE_Manager_End();
    }
   
  TracePrintColor(TRACE_COLOR.BLUE, "End Script..."); 
}
