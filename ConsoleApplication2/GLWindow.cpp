#include "GLWindow.h"

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include <stdexcept>

GLWindow *g_instance = NULL;

GLWindow::GLWindow(TCHAR* title, int width, int height, int bits, bool fullscreenflag) : fullscreen(fullscreenflag)
{
    g_instance = this;
    if (!CreateGLWindow(title, width, height, bits, fullscreenflag))
        throw std::logic_error("Can't create GL widow");
}

GLWindow::~GLWindow()
{
    g_instance = NULL;
}

void GLWindow::init()
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    initGL();
}

void GLWindow::resize(int width, int height)
{
    if (height == 0)										// Prevent A Divide By Zero By
    {
        height = 1;										// Making Height Equal One
    }

    _width = width;
    _height = height;

    glViewport(0, 0, width, height);						// Reset The Current Viewport
 

    resizeGL(width, height);
}
void GLWindow::update(bool swap)
{
    drawGL();
    if (swap)
        SwapBuffers(hDC);
}

void GLWindow::swapBuffers()
{
    SwapBuffers(hDC);
}

void GLWindow::KillGLWindow()
{
   /* if (fullscreen)										// Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
        ShowCursor(TRUE);								// Show Mouse Pointer
    }*/

    if (hRC)											// Do We Have A Rendering Context?
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        hRC = NULL;
    }

    if (hDC) 
    {
        ReleaseDC(hWnd, hDC);
        hDC = NULL;
    }

    if (hWnd)
    {
        DestroyWindow(hWnd);					// Are We Able To Destroy The Window?
        hWnd = NULL;										// Set hWnd To NULL
    }

    UnregisterClass(_T("OpenGL"), hInstance);
        hInstance = NULL;   
}

BOOL GLWindow::CreateGLWindow(TCHAR* title, int width, int height, int bits, bool fullscreenflag)
{
    GLuint		PixelFormat;			// Holds The Results After Searching For A Match
    WNDCLASS	wc;						// Windows Class Structure
    DWORD		dwExStyle;				// Window Extended Style
    DWORD		dwStyle;				// Window Style
    RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
    WindowRect.left = (long)0;			// Set Left Value To 0
    WindowRect.right = (long)width;		// Set Right Value To Requested Width
    WindowRect.top = (long)0;				// Set Top Value To 0
    WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

    fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

    hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc = (WNDPROC)s_wndProc;					// WndProc Handles Messages
    wc.cbClsExtra = 0;									// No Extra Window Data
    wc.cbWndExtra = 0;									// No Extra Window Data
    wc.hInstance = hInstance;							// Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
    wc.hbrBackground = NULL;									// No Background Required For GL
    wc.lpszMenuName = NULL;									// We Don't Want A Menu
    wc.lpszClassName = _T("OpenGL");								// Set The Class Name

    if (!RegisterClass(&wc))									// Attempt To Register The Window Class
    {
        MessageBox(NULL, _T("Failed To Register The Window Class."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
        return FALSE;											// Return FALSE
    }

    if (fullscreen)												// Attempt Fullscreen Mode?
    {
        DEVMODE dmScreenSettings;								// Device Mode
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
        dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
        dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        /*if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE;									// Return FALSE
            }
        }*/
    }

    if (fullscreen)												// Are We Still In Fullscreen Mode?
    {
        dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
        dwStyle = WS_POPUP;										// Windows Style
      //  ShowCursor(FALSE);										// Hide Mouse Pointer
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

    // Create The Window
    if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
        _T("OpenGL"),							// Class Name
        title,								// Window Title
        dwStyle |							// Defined Window Style
        WS_CLIPSIBLINGS |					// Required Window Style
        WS_CLIPCHILDREN,					// Required Window Style
        0, 0,								// Window Position
        WindowRect.right - WindowRect.left,	// Calculate Window Width
        WindowRect.bottom - WindowRect.top,	// Calculate Window Height
        NULL,								// No Parent Window
        NULL,								// No Menu
        hInstance,							// Instance
        NULL)))								// Dont Pass Anything To WM_CREATE
    {
        KillGLWindow();								// Reset The Display
        MessageBox(NULL, _T("Window Creation Error."), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
        return FALSE;								// Return FALSE
    }

    static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
        1,											// Version Number
        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
        PFD_TYPE_RGBA,								// Request An RGBA Format
        bits,										// Select Our Color Depth
        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        16,											// 16Bit Z-Buffer (Depth Buffer)  
        0,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    try{
        if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
           throw std::logic_error("Can't Create A GL Device Context.");

        if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
            throw std::logic_error("Can't Find A Suitable PixelFormat.");

        if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
            throw std::logic_error("Can't Set The PixelFormat.");

        if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
            throw std::logic_error("Can't Create A GL Rendering Context.");

        if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
            throw std::logic_error("Can't Activate The GL Rendering Context.");
    }
    catch (std::exception e)
    {
        printf("GLWindow error: %s", e.what());
        KillGLWindow();
        return FALSE;
    }

    //ShowWindow(hWnd, SW_SHOW);						// Show The Window
    SetForegroundWindow(hWnd);						// Slightly Higher Priority
    SetFocus(hWnd);									// Sets Keyboard Focus To The Window

    resize(width, height);					// Set Up Our Perspective GL Screen
    init();
    return TRUE;									// Success
}


LRESULT GLWindow::s_wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return g_instance->wndProc(hWnd, msg, wParam, lParam);
}



LRESULT GLWindow::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)									// Check For Windows Messages
    {
    case WM_SYSCOMMAND:							// Intercept System Commands
    {
        switch (wParam)							// Check System Calls
        {
        case SC_SCREENSAVE:					// Screensaver Trying To Start?
        case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
            return 0;							// Prevent From Happening
        }
        break;									// Exit
    }

    case WM_CLOSE:								// Did We Receive A Close Message?
    {
        PostQuitMessage(0);						// Send A Quit Message
        return 0;								// Jump Back
    }

    case WM_SIZE:								// Resize The OpenGL Window
    {
        resize(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
        return 0;								// Jump Back
    }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int GLWindow::width()
{
    return _width;
}
int GLWindow::height()
{
    return _height;
}


void GLWindow::initGL()
{
    //glEnable(GL_COLOR_MATERIAL);
    //glDisable(GL_LIGHTING);

}

void GLWindow::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    //gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glOrtho(0, width, height, 0, 0, 1000);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();
}

void GLWindow::drawGL()
{
    const int speed = 5;
    static int dx = speed; 
    static int dy = speed;
    static int size = 40;   

    static int x = 0;
    static int y = 0;


    if (x + dx + size > width()) dx = -dx;
    if (x + dx < 0) dx = -dx;

    if (y + dy + size > height()) dy = -dy;
    if (y + dy < 0) dy = -dy;

    x += dx;
    y += dy; 

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3ub(255,0,0); glVertex2d(x, y);
    glColor3ub(0, 255, 0); glVertex2d(x + size, y);
    glColor3ub(0, 0, 255); glVertex2d(x + size, y + size);
    glColor3ub(255, 255, 255); glVertex2d(x, y + size);
    glEnd();
}

void GLWindow::readScreen(UINT8 *buf)
{
    glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, buf);
}