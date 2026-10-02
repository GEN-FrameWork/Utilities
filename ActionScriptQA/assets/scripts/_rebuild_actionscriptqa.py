import json
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

res = vsvarall.resolve_vs_environment(target="INTEL64", compiler="MSC", year="2022", edition="Enterprise")
print(res)

script = tempfile.NamedTemporaryFile("w", suffix=".cmd", delete=False, encoding="utf-8", newline="\r\n")
script.write(f'@echo off\r\ncall "{res.vcvarsall_bat}" {res.vcplatform}\r\nif errorlevel 1 exit /b %errorlevel%\r\nset\r\n')
script.close()
completed = subprocess.run(
    ["cmd.exe", "/d", "/c", script.name],
    env=clean,
    capture_output=True,
    text=True,
    encoding="utf-8",
    errors="replace",
)
Path(script.name).unlink(missing_ok=True)
print("vcvars_exit", completed.returncode)
if completed.returncode != 0:
    print(completed.stderr[-1000:])
    print(completed.stdout[-1000:])
    sys.exit(1)

env = vsvarall.parse_set_output(completed.stdout)
print("PATH_len", len(env.get("PATH", "")))

build = Path(r"e:\Projects\GEN_FrameWork\Utilities\ActionScriptQA\CMake\Build\Windows\intel64")
# Keep exe but refresh cmake files carefully: only delete cache if needed
cache = build / "CMakeCache.txt"
if cache.exists():
    # Check compiler
    text = cache.read_text(encoding="utf-8", errors="replace")
    if "CMAKE_CXX_COMPILER:FILEPATH=CMAKE_CXX_COMPILER-NOTFOUND" in text or "No CMAKE_CXX_COMPILER" in text:
        shutil.rmtree(build / "CMakeFiles", ignore_errors=True)
        cache.unlink(missing_ok=True)

cmd = [
    "cmake",
    "-S",
    r"e:\Projects\GEN_FrameWork\Utilities\ActionScriptQA\CMake",
    "-B",
    str(build),
    "-G",
    "Ninja",
    "-DTARGET=INTEL64",
    "-DCMAKE_BUILD_TYPE=Debug",
    "-DCMAKE_CREATEDOCKERFILE_EXTERNAL_CFG=NOTCREATEDOCKERFILE",
]
r = subprocess.run(cmd, env=env, capture_output=True, text=True, encoding="utf-8", errors="replace")
print(r.stdout[-2500:])
if r.stderr:
    print("STDERR", r.stderr[-1500:])
print("cmake_exit", r.returncode)
if r.returncode:
    sys.exit(r.returncode)

r2 = subprocess.run(
    ["cmake", "--build", str(build), "--target", "actionscriptqa"],
    env=env,
    capture_output=True,
    text=True,
    encoding="utf-8",
    errors="replace",
)
print(r2.stdout[-3000:])
if r2.stderr:
    print("STDERR", r2.stderr[-1500:])
print("build_exit", r2.returncode)

# Verify TraceServer linked
ninja = (build / "build.ninja").read_text(encoding="utf-8", errors="replace")
print("has_TraceServer", "Script_Lib_TraceServer.cpp" in ninja)
sys.exit(r2.returncode)
