#pragma once
#include <Windows.h>
#include "stdafx.h"

class GLWindow
{
public:
    GLWindow();
    ~GLWindow();

    BOOL create(TCHAR* title, int width, int height, int bits, bool fullscreenflag);

    void init();
    void resize(int width, int height);
    void update(bool swap = true);
    void swapBuffers();

    int width();
    int height();

    void readScreen(UINT8 *buf);

    //user functions:
protected:
    virtual void initGL();
    virtual void resizeGL(int width, int height);
    virtual void drawGL();

private:
    void KillGLWindow();
    

    static LRESULT s_wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HDC			hDC = NULL;		// Private GDI Device Context
    HGLRC		hRC = NULL;		// Permanent Rendering Context
    HWND		hWnd = NULL;		// Holds Our Window Handle
    HINSTANCE	hInstance;		// Holds The Instance Of The Application

    bool fullscreen = false;

    int _width;
    int _height;
};