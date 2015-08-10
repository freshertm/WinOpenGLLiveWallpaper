#pragma once

#define INITGUID
#include <objbase.h>
#include <initguid.h>
#include <ddraw.h>

class DXOverlay
{
public:
    DXOverlay(UINT32 width, UINT32 height, DWORD keyColor);
    ~DXOverlay();


    int UpdateOverlay(const UINT8 *pimage);


private:
    int InitDDraw();
    void UninitDDraw();

    HRESULT CreatePrimarySurface();


    int CheckOverlaySupport();
    int CreateOverlay();

    int CopyImageToSurface(const UINT8 *pimage);

    void DestroyOverlay();

    DWORD GetPixelValue(IDirectDrawSurface7 *surface, COLORREF rgb);

    int ShowOverlay();

private:

    UINT32 width;
    UINT32 height;

    DWORD keyColor;

    // Global variables
    IDirectDraw7        *ddraw = 0;
    IDirectDrawSurface7 *primary = 0;
    IDirectDrawSurface7 *overlay = 0;
    IDirectDrawSurface7 *backbuffer = 0;
};