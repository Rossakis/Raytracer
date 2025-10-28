// win_dib_framebuffer.cpp
#define WIN32_LEAN_AND_MEAN
#include <stdbool.h>
#include <windows.h>
#include <stdint.h>

static int gWidth = 800, gHeight = 600;
static BITMAPINFO gBMI = {};
static void* gPixels = NULL;

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_DESTROY) { PostQuitMessage(0); return 0; }
    return DefWindowProc(h, m, w, l);
}

static inline uint32_t BGRX(uint8_t r, uint8_t g, uint8_t b) {
    // 0x00BBGGRR in memory for BI_RGB 32bpp
    return (uint32_t)(b) << 16 | (uint32_t)(g) << 8 | (uint32_t)(r);
}

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int) {
    // 1) Window
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hi;
    wc.lpszClassName = TEXT("DIBSample");
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, TEXT("DIB framebuffer"),
                             WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, gWidth, gHeight,
                             0,0,hi,0);

    // 2) DIB section (top-down: negative height)
    gBMI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    gBMI.bmiHeader.biWidth = gWidth;
    gBMI.bmiHeader.biHeight = -gHeight; // top-down so y=0 is top
    gBMI.bmiHeader.biPlanes = 1;
    gBMI.bmiHeader.biBitCount = 32;
    gBMI.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(hwnd);
    HBITMAP dib = CreateDIBSection(hdc, &gBMI, DIB_RGB_COLORS, &gPixels, NULL, 0);
    HDC memdc = CreateCompatibleDC(hdc);
    SelectObject(memdc, dib);

    // 3) Main loop: write pixels, blit once per frame
    MSG msg;
    bool running = true;
    int t = 0;
    while (running) {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Write into the DIB memory (your "software framebuffer")
        uint32_t* fb = (uint32_t*)gPixels;
        for (int y = 0; y < gHeight; ++y) {
            for (int x = 0; x < gWidth; ++x) {
                uint8_t r = (uint8_t)((x + t) & 255);
                uint8_t g = (uint8_t)((y + t) & 255);
                uint8_t b = (uint8_t)(((x^y) + t) & 255);
                fb[y * gWidth + x] = BGRX(r, g, b);
            }
        }

        // Present: BitBlt from the memory DC to the window DC
        BitBlt(hdc, 0, 0, gWidth, gHeight, memdc, 0, 0, SRCCOPY);

        Sleep(16); // ~60 FPS
        ++t;
    }

    // Cleanup
    DeleteDC(memdc);
    DeleteObject(dib);
    ReleaseDC(hwnd, hdc);
    return 0;
}
