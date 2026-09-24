# --------------------------------------------------------------------
# actionscriptqa: Action Script QA  
# GEN FrameWork.
# --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)

project(actionscriptqa)


set(GEN_DIRECTORY "../../../") 

# --------------------------------------------------------------------
# Activate features/Defines

option(DEBUG_FEATURE                                            "Debug mode"                                              ON )
option(XTRACE_FEATURE                                           "XTrace"                                                  ON )
option(XTRACE_NOINTERNET_FEATURE                                "No need for trace dependencies with Internet"            ON )                  
option(XMEMORY_CONTROL_FEATURE                                  "XMemory Control"                                         ON )
                
option(APPFLOW_FEATURE                                          "Application Flow modules"                                ON )
option(APPFLOW_CONSOLE_FEATURE                                  "Application Flow Console"                                ON )
option(APPFLOW_LOG_FEATURE                                      "Application Flow Log"                                    ON )
option(APPFLOW_EXTENDED_FEATURE                                 "Application Flow Extended"                               ON )  
option(APPFLOW_EXTENDED_APPLICATIONHEADER_FEATURE               "Application Flow Extended Aplication Header"             ON )  
option(APPFLOW_EXTENDED_APPLICATIONSTATUS_FEATURE               "Application Flow Extended Aplication Status"             ON )  

option(APPFLOW_CFG_GENERAL_FEATURE                              "Application CFG General"                                 ON )
option(APPFLOW_CFG_LOG_FEATURE                                  "Application CFG LOG"                                     ON )
option(APPFLOW_CFG_SCRIPTS_FEATURE                              "Application Flow Scripts List"                           ON ) 
               
option(DIO_FEATURE                                              "Data Input/Output"                                       ON )
option(DIO_STREAMTLS_FEATURE                                    "Stream TLS"                                              ON )    
option(DIO_CHECKCONNECTIONS_FEATURE                             "Check Connections"                                       ON )
option(DIO_NETWORKTIMEPROTOCOL_FEATURE                          "Network Time Protocol"                                   ON )
option(DIO_DYNDNS_FEATURE                                       "DynDNS"                                                  ON )
option(DIO_PCAP_FEATURE                                         "PCap Library"                                            ON )
                                                                                                                          
option(SCRIPT_FEATURE                                           "Activate Scripts"                                        ON )
option(SCRIPT_G_FEATURE                                         "Activate Script G Language"                              ON )
option(SCRIPT_LUA_FEATURE                                       "Activate Script LUA Language"                            ON )
option(SCRIPT_JAVASCRIPT_FEATURE                                "Activate Script Javascript Language"                     ON )
option(SCRIPT_LIB_SYSTEM_FEATURE                                "Script Lib System"                                       ON )    
option(SCRIPT_LIB_PROCESS_FEATURE                               "Script Lib Process"                                      ON )    
option(SCRIPT_LIB_LOG_FEATURE                                   "Script Lib Log"                                          ON )    
option(SCRIPT_LIB_CONSOLE_FEATURE                               "Script Lib Console"                                      ON ) 
option(SCRIPT_LIB_WINDOW_FEATURE                                "Script Lib Window"                                       ON ) 
option(SCRIPT_LIB_INPUTSIMULATE_FEATURE                         "Script Lib Input Simulated"                              ON )   
 

# --------------------------------------------------------------------
# Config Application Macro

 set(APP_CFG_ACTIONSCRIPTQA "-DAPPFLOW_CFG=ACTIONSCRIPTQA_CFG::GetInstance()")
 add_definitions(${APP_CFG_ACTIONSCRIPTQA})
                   

# --------------------------------------------------------------------
# Directorys of the Application

set(APP_DIRECTORY_APPLICATION                                       "../Sources")
list(APPEND GEN_INCLUDES_DIR_LIST                                   ${APP_DIRECTORY_APPLICATION})


# --------------------------------------------------------------------
# Source code of the GEN in this Application

include("${GEN_DIRECTORY}/Common/CMake/GEN_Main.cmake") 

# --------------------------------------------------------------------
# Source code of the Application

list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/ActionScriptQA.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/ActionScriptQA_CFG.cpp")
 

# --------------------------------------------------------------------
# End Proyect 

include("${GEN_DIRECTORY}/Common/CMake/GEN_End.cmake")

