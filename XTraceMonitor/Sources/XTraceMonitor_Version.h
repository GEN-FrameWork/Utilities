/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XTraceMonitor_Version.h
*
* @brief      XTM version / identity (shared by C++ and Windows .rc VERSIONINFO).
* @ingroup    UTILITIES
*
* @note       Keep strings as narrow "..." so the Windows Resource Compiler can include this
*             header. Change APPLICATION_VERSION* here and both About UI and EXE Details update
*             (APPLICATION_VERSION_STR is built with the preprocessor).
*
* @copyright  EndoraSoft. All rights reserved.
*
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define APPLICATION_VERSION                         8
#define APPLICATION_SUBVERSION                      0
#define APPLICATION_SUBVERSIONERR                   1

#define APPLICATION_VERSION_STRINGIZE_(x)           #x
#define APPLICATION_VERSION_STRINGIZE(x)            APPLICATION_VERSION_STRINGIZE_(x)

#define APPLICATION_VERSION_STR                     APPLICATION_VERSION_STRINGIZE(APPLICATION_VERSION) "." \
                                                    APPLICATION_VERSION_STRINGIZE(APPLICATION_SUBVERSION) "." \
                                                    APPLICATION_VERSION_STRINGIZE(APPLICATION_SUBVERSIONERR) ".0"

#define APPLICATION_NAMEAPP                         "eXtended Trace Monitor"
#define APPLICATION_NAMEFILE                        "xtracemonitor"
#define APPLICATION_OWNER                           "EndoraSoft"
#define APPLICATION_YEAROFCREATION                  2026

#define APPLICATION_FILEDESCRIPTION_STR             "GEN eXtended Trace Monitor"
#define APPLICATION_COPYRIGHT_STR                   "Copyright (c) " APPLICATION_VERSION_STRINGIZE(APPLICATION_YEAROFCREATION) " EndoraSoft S.L."

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
