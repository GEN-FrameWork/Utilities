# Capture UI_System nav / close button templates for ActionScriptQA Tests_UISystem.js
# Root layout ypos is the BOTTOM edge: top = ypos - height.
import os
import sys
import time
import ctypes
import subprocess
from ctypes import wintypes
from pathlib import Path

from PIL import Image

USER32 = ctypes.windll.user32
GDI32 = ctypes.windll.gdi32
SRCCOPY = 0x00CC0020
PW_RENDERFULLCONTENT = 0x00000002


class RECT(ctypes.Structure):
    _fields_ = [("left", ctypes.c_long), ("top", ctypes.c_long),
                ("right", ctypes.c_long), ("bottom", ctypes.c_long)]


def find_window_by_title_substr(substr: str):
    found = []

    @ctypes.WINFUNCTYPE(ctypes.c_bool, wintypes.HWND, wintypes.LPARAM)
    def enum_proc(hwnd, _lparam):
        if not USER32.IsWindowVisible(hwnd):
            return True
        length = USER32.GetWindowTextLengthW(hwnd)
        if length <= 0:
            return True
        buf = ctypes.create_unicode_buffer(length + 1)
        USER32.GetWindowTextW(hwnd, buf, length + 1)
        if substr.lower() in buf.value.lower():
            found.append(hwnd)
        return True

    USER32.EnumWindows(enum_proc, 0)
    return found[0] if found else None


def capture_client(hwnd) -> Image.Image:
    rect = RECT()
    USER32.GetClientRect(hwnd, ctypes.byref(rect))
    width = rect.right - rect.left
    height = rect.bottom - rect.top
    if width <= 0 or height <= 0:
        raise RuntimeError("invalid client size")

    hdc_window = USER32.GetDC(hwnd)
    hdc_mem = GDI32.CreateCompatibleDC(hdc_window)
    hbmp = GDI32.CreateCompatibleBitmap(hdc_window, width, height)
    GDI32.SelectObject(hdc_mem, hbmp)

    if not USER32.PrintWindow(hwnd, hdc_mem, PW_RENDERFULLCONTENT):
        GDI32.BitBlt(hdc_mem, 0, 0, width, height, hdc_window, 0, 0, SRCCOPY)

    bmp_info = ctypes.create_string_buffer(40)
    ctypes.memset(bmp_info, 0, 40)
    ctypes.c_uint32.from_buffer(bmp_info, 0).value = 40
    ctypes.c_int32.from_buffer(bmp_info, 4).value = width
    ctypes.c_int32.from_buffer(bmp_info, 8).value = -height
    ctypes.c_uint16.from_buffer(bmp_info, 12).value = 1
    ctypes.c_uint16.from_buffer(bmp_info, 14).value = 32

    buf = ctypes.create_string_buffer(width * height * 4)
    GDI32.GetDIBits(hdc_mem, hbmp, 0, height, buf, bmp_info, 0)
    img = Image.frombuffer("RGB", (width, height), buf, "raw", "BGRX", 0, 1).copy()

    GDI32.DeleteObject(hbmp)
    GDI32.DeleteDC(hdc_mem)
    USER32.ReleaseDC(hwnd, hdc_window)
    return img


def main():
    root = Path(r"e:\Projects\GEN_FrameWork")
    exe32 = root / r"Examples\Graphics\UI_System\CMake\Build\Windows\intel32\ui_system.exe"
    exe64 = root / r"Examples\Graphics\UI_System\CMake\Build\Windows\intel64\ui_system.exe"
    exe = exe64 if exe64.exists() else exe32
    outdir = root / r"Utilities\ActionScriptQA\assets\graphics"
    outdir.mkdir(parents=True, exist_ok=True)

    if not exe.exists():
        print(f"FAIL: ui_system.exe not found ({exe})")
        return 1

    os.system('taskkill /IM ui_system.exe /F >nul 2>&1')
    time.sleep(0.5)

    p = subprocess.Popen([str(exe)], cwd=str(exe.parent))
    print(f"Launched {exe}")

    hwnd = None
    for _ in range(60):
        time.sleep(0.5)
        hwnd = find_window_by_title_substr("Monitor del Sistema")
        if hwnd:
            break
    if not hwnd:
        print("FAIL: window not found")
        p.terminate()
        return 1

    USER32.SetForegroundWindow(hwnd)
    time.sleep(1.5)

    img = capture_client(hwnd)
    print(f"Captured client {img.size}")
    img.save(outdir / "uisys_full_client.png")

    w, h = img.size
    sx = w / 1440.0
    sy = h / 900.0

    nav_bottoms = [
        ("uisys_nav_resumen.png", 118),
        ("uisys_nav_cpu.png", 174),
        ("uisys_nav_memoria.png", 230),
        ("uisys_nav_red.png", 286),
        ("uisys_nav_disco.png", 342),
        ("uisys_nav_procesos.png", 398),
        ("uisys_nav_alertas.png", 454),
        ("uisys_nav_configuracion.png", 510),
    ]

    for name, ybottom in nav_bottoms:
        top = ybottom - 48
        box = (int(16 * sx), int((top + 4) * sy), int(190 * sx), int((ybottom - 4) * sy))
        crop = img.crop(box)
        crop.save(outdir / name)
        print(f"Saved {name} {crop.size} box={box}")

    cx2 = w - int(8 * sx)
    cy1 = int(6 * sy)
    cx1 = cx2 - int(36 * sx)
    cy2 = cy1 + int(36 * sy)
    close = img.crop((cx1, cy1, cx2, cy2))
    close.save(outdir / "uisys_btn_chrome_close.png")
    print(f"Saved uisys_btn_chrome_close.png {close.size}")

    p.terminate()
    try:
        p.wait(timeout=3)
    except Exception:
        p.kill()
    os.system('taskkill /IM ui_system.exe /F >nul 2>&1')
    print("OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
