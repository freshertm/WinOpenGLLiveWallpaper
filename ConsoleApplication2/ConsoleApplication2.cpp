// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <memory>

#include "desktop.h"

#include "GLWindow.h"

#include "overlay.h"

#define COLORKEY   RGB(8,0,16)



HWND getDesktopHwnd()
{
    HWND hwnd = FindWindow(_T("Progman"), NULL);
    if (hwnd)
        hwnd = FindWindowEx(hwnd, NULL, _T("SHELLDLL_DefView"), NULL);
    if (hwnd)
        hwnd = FindWindowEx(hwnd, NULL, _T("SysListView32"), NULL);
    
    return hwnd;
}




/*
*		This Code Was Created By Jeff Molofee 2000
*		A HUGE Thanks To Fredric Echols For Cleaning Up
*		And Optimizing This Code, Making It More Flexible!
*		If You've Found This Code Useful, Please Let Me Know.
*		Visit My Site At nehe.gamedev.net
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library

HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application
TCHAR       oldWallpaperPath[MAX_PATH];


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc


void makeScreenShot(char* fname_part, int width, int height)
{
    uint32_t buflen = width * height * 4;
    uint8_t *buf = new uint8_t[buflen];

    RECT r;
    r.top = 0;
    r.left = 0;
    r.right = width;
    r.bottom = height;
    HBITMAP bmp = CreateCompatibleBitmap(hDC, r.right, r.bottom);
    HDC mem = CreateCompatibleDC(hDC);


    HBITMAP old = (HBITMAP)SelectObject(mem, bmp);

    // Copy the bits from the screen to our DC (and our BMP that's selected into it.
    BitBlt(mem, 0, 0, r.right, r.bottom, hDC, 0, 0, SRCCOPY | CAPTUREBLT);

    GetBitmapBits(bmp, buflen, buf);

    /*
    BITMAP bmpCapt;
    GetObject(bmp, sizeof(BITMAP), &bmpCapt);

    BITMAPINFOHEADER bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    
    

    GetDIBits(mem, bmp, 0, height, buf, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
    DWORD er = GetLastError();
    */
    char fname[2048] = { 0 };

    sprintf(fname, "D:\\%s -w %d -h %d -cs BGR4.raw",fname_part, width, height);

    FILE* f = fopen(fname, "wb");
    fwrite(buf, buflen, 1, f);

    fclose(f);

    delete[] buf;
}


void saveBitmap(char* path)
{
    FILE* f = fopen(path, "wb");
    BITMAPFILEHEADER hdr;


    PBITMAPINFOHEADER pbih;     // bitmap info-header  

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
    // Compute the size of the entire file.  
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD);

}



void mainLoop(GLWindow * win, DXOverlay *ovl)
{
    MSG		msg;									// Windows Message Structure
    BOOL	done = FALSE;								// Bool Variable To Exit Loop

    DWORD ticks = GetTickCount();

    char fname[1024];

    sprintf(fname, "D:\\gl -w %d -h %d -cs BGR4.raw", win->width(), win->height());

    FILE* f = fopen(fname, "wb");

    UINT32 buflen = win->width() * win->height() * 4;
    UINT8 *buf = new UINT8[buflen];

    while (!done)									// Loop That Runs While done=FALSE
    {
        if ((GetTickCount() - ticks) >= 15000) done = TRUE;  // run 3 sec and quit

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
        {
            if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
            {
                done = TRUE;							// If So done=TRUE
            }
            else									// If Not, Deal With Window Messages
            {
                TranslateMessage(&msg);				// Translate The Message
                DispatchMessage(&msg);				// Dispatch The Message
            }
        }
        else										// If There Are No Messages
        {
            
            if (ovl)
            {
                win->update(false);

                win->readScreen(buf);

                win->swapBuffers();

                ovl->UpdateOverlay(buf);
            }
            else 
                win->update();
        }
    }

    delete[] buf;
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}


int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
    HINSTANCE	hPrevInstance,		// Previous Instance
    LPSTR		lpCmdLine,			// Command Line Parameters
    int			nCmdShow)			// Window Show State
{   

    int width = 800;
    int height = 600;

    //GetDesktopResolution(width, height);

    const uint8_t bitDepth = 32;
    GLWindow *glWindow = NULL;
    try {
        glWindow = new GLWindow(_T("My"), width, height, bitDepth, false);
    }
    catch (...)
    {
        return FALSE;
    }  

    DXOverlay *dxOverlay = NULL;
    
    dxOverlay = new DXOverlay(width, height, COLORKEY);

    if (dxOverlay)
        SetDesktopColor(COLORKEY);

    mainLoop(glWindow, dxOverlay);

    if (dxOverlay)
    {
        RestoreDesktop();
        delete dxOverlay;
    }

    delete glWindow;

    return 0;							// Exit The Program
}
