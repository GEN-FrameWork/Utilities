#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#ifndef PW_CLIENTONLY
#define PW_CLIENTONLY 0x00000001
#endif
#ifndef PW_RENDERFULLCONTENT
#define PW_RENDERFULLCONTENT 0x00000002
#endif

/* Usage: gen_wndcapture.exe <hwnd_hex> <x> <y> <cx> <cy> <out.bmp>
 * Captures a client-area crop via PrintWindow into a 32bpp top-down BMP.
 * Runs as a fresh process so Chrome PW_RENDERFULLCONTENT works (ASQ/console hosts
 * sometimes get solid-black PrintWindow results in-process). */

int wmain(int argc, wchar_t** argv)
{
  if(argc < 7) return 2;

  HWND hwnd = (HWND)(uintptr_t)_wcstoui64(argv[1], NULL, 16);
  int x  = _wtoi(argv[2]);
  int y  = _wtoi(argv[3]);
  int cx = _wtoi(argv[4]);
  int cy = _wtoi(argv[5]);
  const wchar_t* outpath = argv[6];

  if(!hwnd || !IsWindow(hwnd) || cx <= 0 || cy <= 0) return 3;

  HWND root = GetAncestor(hwnd, GA_ROOT);
  if(root) hwnd = root;

  typedef HANDLE (WINAPI *PFN_SetThreadDpiAwarenessContext)(HANDLE);
  HMODULE u32 = GetModuleHandleW(L"user32.dll");
  PFN_SetThreadDpiAwarenessContext pDpi = u32
      ? (PFN_SetThreadDpiAwarenessContext)GetProcAddress(u32, "SetThreadDpiAwarenessContext")
      : NULL;
  if(pDpi) pDpi((HANDLE)(intptr_t)-4);

  RECT cr;
  if(!GetClientRect(hwnd, &cr)) return 4;
  int cw = cr.right - cr.left;
  int ch = cr.bottom - cr.top;
  if(x < 0) { cx += x; x = 0; }
  if(y < 0) { cy += y; y = 0; }
  if(x >= cw || y >= ch) return 5;
  if(x + cx > cw) cx = cw - x;
  if(y + cy > ch) cy = ch - y;
  if(cx <= 0 || cy <= 0) return 5;

  HDC scr = GetDC(NULL);
  HDC full = CreateCompatibleDC(scr);
  HBITMAP hf = CreateCompatibleBitmap(scr, cw, ch);
  HBITMAP of = (HBITMAP)SelectObject(full, hf);

  BOOL pw = PrintWindow(hwnd, full, PW_RENDERFULLCONTENT | PW_CLIENTONLY);
  if(!pw) pw = PrintWindow(hwnd, full, PW_RENDERFULLCONTENT);
  if(!pw) pw = PrintWindow(hwnd, full, 0);
  if(!pw)
    {
      SelectObject(full, of);
      DeleteObject(hf);
      DeleteDC(full);
      ReleaseDC(NULL, scr);
      return 6;
    }

  BITMAPINFO bi;
  memset(&bi, 0, sizeof(bi));
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = cx;
  bi.bmiHeader.biHeight = -cy;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage = (DWORD)cx * cy * 4;

  void* bits = NULL;
  HDC cropdc = CreateCompatibleDC(scr);
  HBITMAP hc = CreateDIBSection(scr, &bi, DIB_RGB_COLORS, &bits, NULL, 0);
  HBITMAP oc = (HBITMAP)SelectObject(cropdc, hc);
  BitBlt(cropdc, 0, 0, cx, cy, full, x, y, SRCCOPY);
  SelectObject(cropdc, oc);

  SelectObject(full, of);
  DeleteObject(hf);
  DeleteDC(full);

  unsigned sum = 0;
  unsigned* p = (unsigned*)bits;
  for(int i = 0; i < cx * cy; i++)
    sum += (p[i] & 0xFF) + ((p[i] >> 8) & 0xFF) + ((p[i] >> 16) & 0xFF);

  BITMAPFILEHEADER bf;
  memset(&bf, 0, sizeof(bf));
  bf.bfType = 0x4D42;
  bf.bfOffBits = sizeof(bf) + sizeof(BITMAPINFOHEADER);
  bf.bfSize = bf.bfOffBits + bi.bmiHeader.biSizeImage;

  HANDLE fh = CreateFileW(outpath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if(fh == INVALID_HANDLE_VALUE)
    {
      DeleteObject(hc);
      DeleteDC(cropdc);
      ReleaseDC(NULL, scr);
      return 7;
    }
  DWORD wr;
  WriteFile(fh, &bf, sizeof(bf), &wr, NULL);
  WriteFile(fh, &bi.bmiHeader, sizeof(BITMAPINFOHEADER), &wr, NULL);
  WriteFile(fh, bits, bi.bmiHeader.biSizeImage, &wr, NULL);
  CloseHandle(fh);

  DeleteObject(hc);
  DeleteDC(cropdc);
  ReleaseDC(NULL, scr);

  wprintf(L"sum=%u cx=%d cy=%d\n", sum, cx, cy);
  return (sum > 1000) ? 0 : 8;
}
