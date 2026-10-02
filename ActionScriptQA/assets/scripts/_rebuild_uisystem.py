"""Rebuild UI_System intel32 with vcvars (XTRACE_NOINTERNET for local TraceServer)."""
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, r"e:\Projects\GEN_FrameWork\Common\Scripts\compile\internal")
import vsvarall

clean = {
    "SystemRoot": os.environ.get("SystemRoot", r"C:\Windows"),
    "SystemDrive": os.environ.get("SystemDrive", "C:"),
    "USERNAME": os.environ.get("USERNAME", "user"),
    "USERPROFILE": os.environ.get("USERPROFILE", ""),
    "TEMP": os.environ.get("TEMP", ""),
    "TMP": os.environ.get("TMP", ""),
    "ComSpec": os.environ.get("ComSpec", r"C:\Windows\system32\cmd.exe"),
    "PATHEXT": os.environ.get("PATHEXT", ".COM;.EXE;.BAT;.CMD"),
    "NUMBER_OF_PROCESSORS": os.environ.get("NUMBER_OF_PROCESSORS", "8"),
    "PROCESSOR_ARCHITECTURE": "AMD64",
    "PATH": r"C:\Windows\System32;C:\Windows;C:\Program Files\CMake\bin;C:\Program Files\Ninja",
}

res = vsvarall.resolve_vs_environment(target="INTEL32", compiler="MSC", year="2022", edition="Enterprise")
script = tempfile.NamedTemporaryFile("w", suffix=".cmd", delete=False, encoding="utf-8", newline="\r\n")
script.write(f'@echo off\r\ncall "{res.vcvarsall_bat}" {res.vcplatform}\r\nif errorlevel 1 exit /b %errorlevel%\r\nset\r\n')
script.close()
completed = subprocess.run(["cmd.exe", "/d", "/c", script.name], env=clean, capture_output=True, text=True, encoding="utf-8", errors="replace")
Path(script.name).unlink(missing_ok=True)
if completed.returncode != 0:
    print(completed.stdout[-1000:], completed.stderr[-1000:])
    sys.exit(1)
env = vsvarall.parse_set_output(completed.stdout)

build = Path(r"e:\Projects\GEN_FrameWork\Examples\Graphics\UI_System\CMake\Build\Windows\intel32")
# Force reconfigure so XTRACE_NOINTERNET is picked up
cache = build / "CMakeCache.txt"
if cache.exists():
    text = cache.read_text(encoding="utf-8", errors="replace")
    if "XTRACE_NOINTERNET_FEATURE:BOOL=ON" not in text:
        # Delete only the option line by reconfigure with -D
        pass

cmd = [
    "cmake", "-S", r"e:\Projects\GEN_FrameWork\Examples\Graphics\UI_System\CMake",
    "-B", str(build), "-G", "Ninja",
    "-DTARGET=INTEL32",
    "-DCMAKE_BUILD_TYPE=Debug",
    "-DXTRACE_NOINTERNET_FEATURE=ON",
    "-DCMAKE_CREATEDOCKERFILE_EXTERNAL_CFG=NOTCREATEDOCKERFILE",
]
r = subprocess.run(cmd, env=env, capture_output=True, text=True, encoding="utf-8", errors="replace")
print(r.stdout[-2000:])
if r.returncode:
    print(r.stderr[-2000:])
    sys.exit(r.returncode)

r2 = subprocess.run(["cmake", "--build", str(build), "--target", "ui_system"], env=env, capture_output=True, text=True, encoding="utf-8", errors="replace")
print(r2.stdout[-2500:])
if r2.returncode:
    print(r2.stderr[-2000:])
print("build_exit", r2.returncode)

# Confirm define
ninja = (build / "build.ninja").read_text(encoding="utf-8", errors="replace")
print("NOINTERNET in ninja", "XTRACE_NOINTERNET_ACTIVE" in ninja or "XTRACE_NOINTERNET" in ninja)
sys.exit(r2.returncode)
