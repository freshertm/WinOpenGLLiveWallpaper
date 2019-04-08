// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <memory>

#include "desktop.h"

#include "GLWindow.h"

#include "overlay.h"

#include "GLEarth.h"

#include "glWrap.h"

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


void mainLoop(GLWindow * win, DXOverlay *ovl)
{
    MSG		msg;									// Windows Message Structure
    BOOL	done = FALSE;								// Bool Variable To Exit Loop

    DWORD ticks = GetTickCount();

    char fname[1024];   

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
    const bool useOverlay = false;
    GLWindow *glWindow = NULL;
    
    glWindow = new GLEarth();
    //glWindow = new GLWindow();


    glWindow->create(_T("My"), width, height, bitDepth, useOverlay);

    DXOverlay *dxOverlay = nullptr;
    if (useOverlay)
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
