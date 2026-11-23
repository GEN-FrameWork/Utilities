# --------------------------------------------------------------------
# XTraceMonitor : GEN graphical eXtended Trace Monitor (new UI).
# GEN FrameWork
# --------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)

# --------------------------------------------------------------------
# PreProject

set(GEN_DIRECTORY "../../../")

option(USE_CLANG_COMPILER_FEATURE                               "Use Clang Compiler"                                      ON)
include("${GEN_DIRECTORY}/Common/CMake/GEN_PreProject.cmake")

project(xtracemonitor)

# --------------------------------------------------------------------
# features/Defines

#option(DEBUG_FEATURE                                            "Debug mode"                                              ON )
#option(XTRACE_FEATURE                                           "XTrace"                                                  ON )
#option(XTRACE_NOINTERNET_FEATURE                                "No need for trace dependencies with Internet"            ON )
#option(XMEMORY_CONTROL_FEATURE                                  "XMemory Control"                                         ON )


option(GRP_OPENGL_FEATURE                                       "Graphics OpenGL ES required"                             ON )
option(LINUX_X11_FEATURE                                        "Linux X11"                                               ON )

option(USERINTERFACE_FEATURE                                    "User Interace"                                           ON )

option(APPFLOW_FEATURE                                          "Application Flow modules"                                ON )
option(APPFLOW_EXTENDED_FEATURE                                 "Application Flow Extended"                               ON )
option(APPFLOW_EXTENDED_INTERNETSTATUS_FEATURE                  "Application Flow Extended Internet Status"               ON )
option(APPFLOW_LOG_FEATURE                                      "Application Flow Log"                                    ON )
option(APPFLOW_GRAPHICS_FEATURE                                 "Application Flow Graphics"                               ON )
option(APPFLOW_GRAPHICS_NOTCONSOLE_FEATURE                      "Application Flow Graphics (Deactive Console)"            ON )

option(APPFLOW_CFG_GENERAL_FEATURE                              "Application Flow CFG General"                            ON )
option(APPFLOW_CFG_LOG_FEATURE                                  "Application Flow CFG LOG"                                ON )

option(GRP_FEATURE                                              "Graphics"                                                ON )
option(GRP_SCREEN_CUSTOMCHROMES_FEATURE                         "Graphics Screen Custom Chromes"                          ON )
option(GRP_BITMAP_FILE_BMP_FEATURE                              "Graphics Bitmap File BMP"                                ON )
option(GRP_BITMAP_FILE_JPG_FEATURE                              "Graphics Bitmap File JPG"                                ON )
option(GRP_BITMAP_FILE_PNG_FEATURE                              "Graphics Bitmap File PNG"                                ON )
option(GRP_BITMAP_FILE_TGA_FEATURE                              "Graphics Bitmap File TGA"                                ON )
option(GRP_VIDEO_FILE_AVI_FEATURE                               "Graphics Video File AVI"                                 ON )
option(GRP_VECTOR_FILE_SVG_FEATURE                              "Graphics Vector File SVG"                                ON )
option(GRP_2D_FEATURE                                           "Graphics 2D"                                             ON )
option(GRP_STATISTICSCHARS_FEATURE                              "Graphics Statistics Charts"                              ON )

option(INP_FEATURE                                              "Inputs"                                                  ON )

option(DIO_STREAM_FEATURE                                       "Stream"                                                  ON )
option(DIO_STREAMUDP_FEATURE                                    "Stream UDP"                                              ON )
option(DIO_STREAMUART_FEATURE                                   "Stream UART"                                             ON )

option(CIPHER_SYMMETRIC_FEATURE                                 "Cipher Symmetric"                                        ON )
option(CIPHER_SYMMETRIC_AES_FEATURE                             "Cipher Symmetric AES"                                    ON )
option(DIO_DYNDNS_FEATURE                                       "DynDNS protocol (members.dyndns.org)"                    ON )

option(DIO_SCRAPERWEB_GEOLOCATIONIP_FEATURE                     "Scraper Web Geolocation IP"                              ON )
option(DIO_SCRAPERWEB_STATICMAP_FEATURE                         "Scraper Web Static Map"                                  ON )

option(XPROCESSMANAGER_FEATURE                                  "Process Manager"                                         ON )
option(XCLIPBOARD_FEATURE                                       "Clipboard functions (portable text)"                     ON )
option(XTHREADCOLLECTED_FEATURE                                 "Thread Collected"                                        ON )
option(XTRACE_SERVER_UDP_FEATURE                                "XTrace receive server UDP"                               ON )
option(XTRACE_SERVER_UART_FEATURE                               "XTrace receive server UART"                              ON )
option(XTRACE_SERVER_WINDOWSMSG_FEATURE                         "XTrace receive server Windows WM_COPYDATA"               ON )


# --------------------------------------------------------------------
# Config Application Macro

set(APP_CFG_XTRACEMONITOR "-DAPPFLOW_CFG=XTRACEMONITOR_CFG::GetInstance()")
add_definitions(${APP_CFG_XTRACEMONITOR})


# --------------------------------------------------------------------
# Android package settings

set(GEN_ANDROID_APP_LABEL                                  "XTraceMonitor")
set(GEN_ANDROID_APP_ID                                     "com.gen.xtracemonitor")


# --------------------------------------------------------------------
# Main CMakeList of GEN

include("${GEN_DIRECTORY}/Common/CMake/GEN_Main.cmake")


# --------------------------------------------------------------------
# Directorys of the Application

set(APP_DIRECTORY_APPLICATION   "../Sources")
list(APPEND GEN_INCLUDES_DIR_LIST ${APP_DIRECTORY_APPLICATION})


# --------------------------------------------------------------------
# Source code of the Application

list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_CFG.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Form_CFG.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Form_Filters.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Form_About.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Form_DynDNS.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Server.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_Origins.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_GUI_Tree.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_GUI_Console.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_GUI_VarStatus.cpp")
list(APPEND GEN_SOURCES_MODULES_LIST "${APP_DIRECTORY_APPLICATION}/XTraceMonitor_GeoIP.cpp")


# --------------------------------------------------------------------
# Platform packaging (Package/Windows|Android|Linux). Only this app CMake owns it.

set(APP_DIRECTORY_PACKAGE "../Package")
set(APP_DIRECTORY_PACKAGE_LINUX "${APP_DIRECTORY_PACKAGE}/Linux")

if(COMPILE_FOR_WINDOWS)

  set(APP_DIRECTORY_PACKAGE_WINDOWS "${APP_DIRECTORY_PACKAGE}/Windows")
  list(APPEND GEN_INCLUDES_DIR_LIST ${APP_DIRECTORY_PACKAGE_WINDOWS})
  set(APP_RESOURCE_FILE "${APP_DIRECTORY_PACKAGE_WINDOWS}/XTraceMonitor.rc")

endif()


# --------------------------------------------------------------------
# End Proyect

include("${GEN_DIRECTORY}/Common/CMake/GEN_End.cmake")


# --------------------------------------------------------------------
# Linux packaging: copy icons + generate .desktop with absolute Icon=/Exec=
# (Icon=name alone only resolves via system hicolor; build dirs need abs paths for KDE/Dolphin).

if(COMPILE_FOR_LINUX)

  set(_XTM_LINUX_PKG      "${CMAKE_CURRENT_SOURCE_DIR}/${APP_DIRECTORY_PACKAGE_LINUX}")
  set(_XTM_LINUX_DESK_IN  "${_XTM_LINUX_PKG}/xtracemonitor.desktop.in")
  set(_XTM_LINUX_APPICON  "${_XTM_LINUX_PKG}/appicon.png")
  set(_XTM_LINUX_ICONS    "${_XTM_LINUX_PKG}/icons")
  set(_XTM_LINUX_GEN_DESK "${_XTM_LINUX_PKG}/xtm_gen_desktop.cmake")

  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_XTM_LINUX_APPICON}"
            "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/appicon.png"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${_XTM_LINUX_ICONS}"
            "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/icons"
    COMMAND ${CMAKE_COMMAND}
            "-DXTM_DESKTOP_IN=${_XTM_LINUX_DESK_IN}"
            "-DXTM_DESKTOP_OUT=$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/xtracemonitor.desktop"
            "-DXTM_BIN_DIR=$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
            "-DXTM_EXEC_PATH=$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
            "-DXTM_ICON_PATH=$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/appicon.png"
            "-DXTM_ICONS_SRC=${_XTM_LINUX_ICONS}"
            -P "${_XTM_LINUX_GEN_DESK}"
    COMMENT "[ XTM Package/Linux: desktop + icons → build dir and ~/.local/share ]"
    VERBATIM
  )

endif()
