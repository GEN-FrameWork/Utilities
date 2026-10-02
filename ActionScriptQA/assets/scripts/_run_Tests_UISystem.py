"""Launch ActionScriptQA with ACTIONSCRIPTQA_AUTORUN=1 and wait for Tests_UISystem result."""
import os
import re
import subprocess
import sys
import time
from pathlib import Path


def main():
    build = Path(r"e:\Projects\GEN_FrameWork\Utilities\ActionScriptQA\CMake\Build\Windows\intel64")
    exe = build / "actionscriptqa.exe"
    possible_logs = [
        Path(r"e:\Projects\GEN_FrameWork\Utilities\ActionScriptQA\assets\scripts.log"),
        Path(r"e:\Projects\GEN_FrameWork\Utilities\ActionScriptQA\assets\scripts\scripts.log"),
        build / "scripts.log",
    ]

    os.system("taskkill /IM actionscriptqa.exe /F >nul 2>&1")
    os.system("taskkill /IM ui_system.exe /F >nul 2>&1")
    time.sleep(0.8)

    for log in possible_logs:
        try:
            if log.exists():
                log.write_text("", encoding="utf-8", errors="ignore")
        except Exception as ex:
            print("truncate", log, ex)

    env = os.environ.copy()
    env["ACTIONSCRIPTQA_AUTORUN"] = "1"

    # Start minimized so the console does not intercept InpSim mouse clicks on UI_System.
    si = subprocess.STARTUPINFO()
    si.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    si.wShowWindow = 6  # SW_MINIMIZE

    p = subprocess.Popen(
        [str(exe)],
        cwd=str(build),
        env=env,
        startupinfo=si,
        creationflags=subprocess.CREATE_NEW_CONSOLE,
    )
    print(f"Started {exe} pid={p.pid} AUTORUN=1 (minimized console)")

    deadline = time.time() + 180
    combined = ""
    while time.time() < deadline:
        combined = ""
        for log in possible_logs:
            if log.exists() and log.stat().st_size > 0:
                try:
                    combined += log.read_text(encoding="utf-8", errors="replace")
                except Exception:
                    pass
        if ("Tests_UISystem" in combined) and ("Result ok=" in combined) and (
            "End script" in combined or "feedback OK" in combined or "feedback FAILED" in combined
        ):
            break
        if p.poll() is not None:
            print("process exited", p.returncode)
            break
        time.sleep(1.0)

    try:
        p.wait(timeout=2)
    except subprocess.TimeoutExpired:
        p.terminate()
        try:
            p.wait(timeout=5)
        except Exception:
            p.kill()

    os.system("taskkill /IM ui_system.exe /F >nul 2>&1")
    os.system("taskkill /IM actionscriptqa.exe /F >nul 2>&1")

    combined = ""
    for log in possible_logs:
        if log.exists():
            text = log.read_text(encoding="utf-8", errors="replace")
            combined += f"\n===== {log} ({len(text)} bytes) =====\n{text}"

    print(combined[-12000:] if combined else "NO LOG CONTENT")

    m = re.search(r"Tests_UISystem\.js\].*Result ok=(\d+) fail=(\d+)", combined)
    if not m:
        print("FAIL: no result line")
        return 1
    ok_n, fail_n = int(m.group(1)), int(m.group(2))
    print(f"PARSED Result ok={ok_n} fail={fail_n}")
    feedback_ok = "UI_System TraceServer feedback OK" in combined
    feedback_fail = "UI_System TraceServer feedback FAILED" in combined
    print("feedback_ok=", feedback_ok, "feedback_fail=", feedback_fail)
    return 0 if (fail_n == 0 and ok_n > 0 and feedback_ok and not feedback_fail) else 1


if __name__ == "__main__":
    sys.exit(main())
