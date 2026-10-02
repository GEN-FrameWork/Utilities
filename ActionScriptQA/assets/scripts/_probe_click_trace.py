"""Probe which click method produces UI_System Selected! traces."""
import ctypes
import os
import socket
import subprocess
import sys
import time
from ctypes import wintypes

USER32 = ctypes.windll.user32


class MOUSEINPUT(ctypes.Structure):
    _fields_ = [
        ("dx", ctypes.c_long),
        ("dy", ctypes.c_long),
        ("mouseData", wintypes.DWORD),
        ("dwFlags", wintypes.DWORD),
        ("time", wintypes.DWORD),
        ("dwExtraInfo", ctypes.c_void_p),
    ]


class INPUT(ctypes.Structure):
    _fields_ = [("type", wintypes.DWORD), ("mi", MOUSEINPUT)]


def find_hwnd(substr: str):
    found = []

    @ctypes.WINFUNCTYPE(ctypes.c_bool, wintypes.HWND, wintypes.LPARAM)
    def enum_proc(hwnd, _lp):
        if not USER32.IsWindowVisible(hwnd):
            return True
        n = USER32.GetWindowTextLengthW(hwnd)
        if n <= 0:
            return True
        buf = ctypes.create_unicode_buffer(n + 1)
        USER32.GetWindowTextW(hwnd, buf, n + 1)
        if substr.lower() in buf.value.lower():
            found.append(hwnd)
            return False
        return True

    USER32.EnumWindows(enum_proc, 0)
    return found[0] if found else None


def drain(sock, seconds=0.4):
    end = time.time() + seconds
    while time.time() < end:
        try:
            sock.recvfrom(65535)
        except OSError:
            pass


def listen_selected(sock, seconds=2.5):
    hits = []
    end = time.time() + seconds
    while time.time() < end:
        try:
            data, _ = sock.recvfrom(65535)
        except OSError:
            continue
        text = data.decode("utf-16-le", errors="ignore")
        if "Selected!" not in text:
            continue
        idx = text.find("UI Element")
        snippet = text[idx : idx + 90] if idx >= 0 else text[:90]
        hits.append(snippet.encode("ascii", "replace").decode("ascii"))
    return hits


def mouse_event_click(x, y):
    USER32.SetCursorPos(x, y)
    time.sleep(0.05)
    USER32.mouse_event(0x0002, 0, 0, 0, 0)
    time.sleep(0.03)
    USER32.mouse_event(0x0004, 0, 0, 0, 0)


def sendinput_click(x, y):
    USER32.SetCursorPos(x, y)
    time.sleep(0.05)
    inp = (INPUT * 2)()
    inp[0].type = 0
    inp[0].mi.dwFlags = 0x0002
    inp[1].type = 0
    inp[1].mi.dwFlags = 0x0004
    return USER32.SendInput(2, ctypes.byref(inp), ctypes.sizeof(INPUT))


def main():
    os.system("taskkill /IM ui_system.exe /F >nul 2>&1")
    os.system("taskkill /IM actionscriptqa.exe /F >nul 2>&1")
    time.sleep(0.6)

    exe = r"e:\Projects\GEN_FrameWork\Examples\Graphics\UI_System\CMake\Build\Windows\intel32\ui_system.exe"
    proc = subprocess.Popen([exe], cwd=os.path.dirname(exe))
    hwnd = None
    for _ in range(50):
        time.sleep(0.4)
        hwnd = find_hwnd("Monitor del Sistema")
        if hwnd:
            break
    if not hwnd:
        print("FAIL: window not found")
        return 1

    USER32.SetWindowPos(hwnd, 0, 40, 40, 0, 0, 0x0001)
    USER32.SetForegroundWindow(hwnd)
    time.sleep(2.0)

    wr = wintypes.RECT()
    cr = wintypes.RECT()
    USER32.GetWindowRect(hwnd, ctypes.byref(wr))
    USER32.GetClientRect(hwnd, ctypes.byref(cr))
    origin = wintypes.POINT(0, 0)
    USER32.ClientToScreen(hwnd, ctypes.byref(origin))
    print(f"window=({wr.left},{wr.top},{wr.right - wr.left}x{wr.bottom - wr.top})")
    print(f"client=({cr.right}x{cr.bottom}) origin=({origin.x},{origin.y})")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("", 10001))
    sock.settimeout(0.12)
    drain(sock, 1.0)

    points = [
        ("resumen", origin.x + 105, origin.y + 94),
        ("cpu", origin.x + 105, origin.y + 150),
        ("close", origin.x + 1414, origin.y + 24),
    ]

    for name, x, y in points:
        drain(sock, 0.2)
        mouse_event_click(x, y)
        hits = listen_selected(sock, 2.0)
        print(f"mouse_event {name} @ {x},{y} -> {hits}")

    for name, x, y in points:
        drain(sock, 0.2)
        n = sendinput_click(x, y)
        hits = listen_selected(sock, 2.0)
        print(f"SendInput({n}) {name} @ {x},{y} -> {hits}")

    proc.terminate()
    try:
        proc.wait(timeout=2)
    except Exception:
        proc.kill()
    os.system("taskkill /IM ui_system.exe /F >nul 2>&1")
    return 0


if __name__ == "__main__":
    sys.exit(main())
