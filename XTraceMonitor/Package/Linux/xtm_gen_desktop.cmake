# Generate xtracemonitor.desktop with absolute Exec=/Icon= for the build dir.
# Also installs into $HOME/.local/share so KDE can resolve the icon when the build
# lives on a Windows mount (/mnt/e) where chmod/xattrs and "trust" do not work.
#
# -DXTM_DESKTOP_IN= -DXTM_DESKTOP_OUT= -DXTM_BIN_DIR= -DXTM_EXEC_PATH=
# -DXTM_ICON_PATH=  -DXTM_ICONS_SRC= (optional hicolor tree)

cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0053 NEW)

if(NOT XTM_DESKTOP_IN OR NOT XTM_DESKTOP_OUT OR NOT XTM_BIN_DIR OR NOT XTM_EXEC_PATH OR NOT XTM_ICON_PATH)
  message(FATAL_ERROR "xtm_gen_desktop.cmake: missing XTM_DESKTOP_IN/OUT, XTM_BIN_DIR, XTM_EXEC_PATH or XTM_ICON_PATH")
endif()

function(xtm_write_desktop out_path icon_value)
  file(READ "${XTM_DESKTOP_IN}" _content)
  string(REPLACE "__XTM_BIN_DIR__"   "${XTM_BIN_DIR}"   _content "${_content}")
  string(REPLACE "__XTM_EXEC_PATH__" "${XTM_EXEC_PATH}" _content "${_content}")
  string(REPLACE "__XTM_ICON_PATH__" "${icon_value}"    _content "${_content}")
  file(WRITE "${out_path}" "${_content}")
  if(UNIX)
    execute_process(COMMAND chmod +x "${out_path}" ERROR_QUIET)
    # Mark as trusted launcher when gio is available (native Linux FS only).
    execute_process(
      COMMAND gio set "${out_path}" "metadata::trusted" true
      ERROR_QUIET OUTPUT_QUIET RESULT_VARIABLE _gio_rc
    )
  endif()
endfunction()

# 1) Build-dir copy (absolute Icon= so a trusted local copy can still show it).
xtm_write_desktop("${XTM_DESKTOP_OUT}" "${XTM_ICON_PATH}")

# 2) User-local Freedesktop install — this is what makes KDE show the icon.
if(DEFINED ENV{HOME} AND NOT "$ENV{HOME}" STREQUAL "")
  set(_XTM_HOME "$ENV{HOME}")
  set(_XTM_APP_DIR   "${_XTM_HOME}/.local/share/applications")
  set(_XTM_ICONS_DIR "${_XTM_HOME}/.local/share/icons")

  file(MAKE_DIRECTORY "${_XTM_APP_DIR}")

  if(XTM_ICONS_SRC AND EXISTS "${XTM_ICONS_SRC}")
    file(MAKE_DIRECTORY "${_XTM_ICONS_DIR}")
    file(COPY "${XTM_ICONS_SRC}/" DESTINATION "${_XTM_ICONS_DIR}/hicolor")
  endif()

  # Theme name "xtracemonitor" → ~/.local/share/icons/hicolor/*/apps/xtracemonitor.png
  xtm_write_desktop("${_XTM_APP_DIR}/xtracemonitor.desktop" "xtracemonitor")

  message(STATUS "[ XTM Package/Linux ] installed launcher: ${_XTM_APP_DIR}/xtracemonitor.desktop")
  message(STATUS "[ XTM Package/Linux ] installed icons:    ${_XTM_ICONS_DIR}/hicolor")
endif()
