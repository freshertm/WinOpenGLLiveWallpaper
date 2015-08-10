#include "overlay.h"
#include <stdexcept>

DXOverlay::DXOverlay(UINT32 width, UINT32 height, DWORD keyColor) : width(width), height(height), keyColor(keyColor)
{
    	// Initialize DirectDraw
	if( InitDDraw() < 0 )
        throw std::logic_error("Failed to initialize DirectDraw.");

	// Check overlay support
	if( CheckOverlaySupport() < 0 )
        throw std::logic_error("Your hardware doesn't support the overlay surfaces we are trying to create.");

	// Create the overlay surface
	if( CreateOverlay() < 0 )
        throw std::logic_error("Failed to create the overlay surface.");

	// Show the overlay surface
	if( ShowOverlay() < 0 )
        throw std::logic_error("Failed to show the overlay surface");
}

DXOverlay::~DXOverlay()
{
    DestroyOverlay();
    UninitDDraw();
}


int DXOverlay::InitDDraw()
{
    HRESULT hr;

    // Create the DirectDraw object 
    hr = DirectDrawCreateEx(NULL, (VOID**)&ddraw, IID_IDirectDraw7, NULL);
    if (FAILED(hr))
        goto ErrorOut;

    // Since we aren't going fullscreen we should not use exclusive mode
    // For NORMAL cooperative level we don't need to provide a HWND.
    hr = ddraw->SetCooperativeLevel(NULL, DDSCL_NORMAL);
    if (FAILED(hr))
        goto ErrorOut;

    // Obtain a reference for the primary surface that represents 
    // the entire screen. 
    hr = CreatePrimarySurface();
    if (FAILED(hr))
        goto ErrorOut;

    return 0;

ErrorOut:
    UninitDDraw();

    return -1;
}

void DXOverlay::UninitDDraw()
{
    if (primary)
    {
        primary->Release();
        primary = 0;
    }

    if (ddraw)
    {
        ddraw->Release();
        ddraw = 0;
    }
}

HRESULT DXOverlay::CreatePrimarySurface()
{
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    if (!ddraw)
        return E_FAIL;

    // Create the primary surface which is the entire screen.
    // In this demo we will not draw on this surface directly, 
    // but we still need it for reference.
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hr = ddraw->CreateSurface(&ddsd, &primary, 0);

    return hr;
}

int DXOverlay::CheckOverlaySupport()
{
    DDCAPS  capsDrv;
    HRESULT hr;

    // Get driver capabilities to determine overlay support.
    memset(&capsDrv, 0, sizeof(capsDrv));
    capsDrv.dwSize = sizeof(capsDrv);
    hr = ddraw->GetCaps(&capsDrv, 0);
    if (FAILED(hr))
        return -1;

    // Does the driver support overlays in the current mode? 
    if (!(capsDrv.dwCaps & DDCAPS_OVERLAY))
        return -2;

    // Make sure support for stretching is available
    if (!(capsDrv.dwCaps & DDCAPS_OVERLAYSTRETCH))
        return -3;

    // Make sure the hardware is able to stretch the overlay
    // to the size of the screen. The stretch capabilities may
    // vary with resolution and refresh rate. Only the x axis
    // must be verified. The stretch factor is multiplied with 
    // 1000 to keep three decimal digits 
    UINT stretch = GetSystemMetrics(SM_CXSCREEN) * 1000 / width;
    if (capsDrv.dwMinOverlayStretch > stretch)
        return -4;

    if (capsDrv.dwMaxOverlayStretch < stretch)
        return -5;

    // Check alignment restrictions (only x axis needed)
    // Src rect (0, 0, IMG_W, IMG_H)
    // Dst rect (0, 0, screenW, screenH)
    //	if( capsDrv.dwCaps & DDCAPS_ALIGNBOUNDARYDEST && 0 % capsDrv.dwAlignBoundaryDest != 0 )
    //	if( capsDrv.dwCaps & DDCAPS_ALIGNBOUNDARYSRC && 0 % capsDrv.dwAlignBoundarySrc != 0 )
    if (capsDrv.dwCaps & DDCAPS_ALIGNSIZEDEST && GetSystemMetrics(SM_CXSCREEN) % capsDrv.dwAlignSizeDest != 0)
        return -6;
    if (capsDrv.dwCaps & DDCAPS_ALIGNSIZESRC && width % capsDrv.dwAlignSizeSrc != 0)
        return -7;

    // Does the overlay hardware support dest color-keying? We will
    // use destination color keying to mask out the background 
    // behind the windows.
    if (!(capsDrv.dwCKeyCaps & DDCKEYCAPS_DESTOVERLAY))
        return -8;

    return 0;
}

int DXOverlay::CreateOverlay()
{
    if (ddraw == 0 || primary == 0)
        return -1;

    // It is not possible to enumerate supported pixel format so we
    // will have to just try one by one until we find a working one

    // These are the formats we support. For overlay surfaces we
    // should support at least these formats. Some video accelerators
    // don't support YUV formats, but others support only YUV formats.
    DDPIXELFORMAT formats[] =
    { { sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16, 0xF800, 0x07e0, 0x001F, 0 },       // 16-bit RGB 5:6:5
    { sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16, 0x7C00, 0x03e0, 0x001F, 0 },       // 16-bit RGB 5:5:5
    { sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('U', 'Y', 'V', 'Y'), 0, 0, 0, 0, 0 },   // UYVY
    { sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y', 'U', 'Y', '2'), 0, 0, 0, 0, 0 } };  // YUY2

    // The surface must be placed in video memory as the video 
    // accelerator must have the memory at hand when it sends the
    // signal to the monitor.

    // We will try to create it with one backbuffer for smoother 
    // animations, this will normally work unless we are asking for
    // too much memory.
    DDSURFACEDESC2 ddsdOverlay;
    memset(&ddsdOverlay, 0, sizeof(ddsdOverlay));
    ddsdOverlay.dwSize = sizeof(ddsdOverlay);
    ddsdOverlay.ddsCaps.dwCaps = DDSCAPS_OVERLAY | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY;
    ddsdOverlay.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_BACKBUFFERCOUNT | DDSD_PIXELFORMAT;
    ddsdOverlay.dwWidth = width;
    ddsdOverlay.dwHeight = height;
    ddsdOverlay.dwBackBufferCount = 1;

    HRESULT hr;
    for (int n = 0; n < 4; n++)
    {
        ddsdOverlay.ddpfPixelFormat = formats[n];
        hr = ddraw->CreateSurface(&ddsdOverlay, &overlay, 0);
        if (SUCCEEDED(hr))
            break;
    }

    // We weren't able to create a flippable overlay surface
    // we could try to create a non-flippable surface, but I'll
    // leave that as an exercise to the interested developer
    if (FAILED(hr))
        return -1;

    // Get a pointer to the backbuffer that we will later draw to
    DDSCAPS2 caps;
    caps.dwCaps = DDSCAPS_BACKBUFFER;
    caps.dwCaps2 = caps.dwCaps3 = caps.dwCaps4 = 0;
    hr = overlay->GetAttachedSurface(&caps, &backbuffer);
    if (FAILED(hr))
        return -1;

    // Initialize the front buffer
/*    hr = CopyImageToSurface(overlay);
    if (FAILED(hr))
        return -1;*/

    return 0;
}

int DXOverlay::CopyImageToSurface(const UINT8 *pimage)
{
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    const DWORD *image = (const DWORD*)pimage;

    // Lock the surface so that we can write to it
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);   
    hr = backbuffer->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
    if (FAILED(hr))
        return -1;

    // Check surface format
    if (ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC)
    {
        // The surface is in one of the YUV formats we specified.
        // These YUV formats have three components Y, U and V, where
        // Y is sampled for every pixel and U and V is sampled only
        // every second pixel, thus every pair of pixels share U and V
        // values.		
        int w = ddsd.dwWidth;
        int h = ddsd.dwHeight;
        int pitch = ddsd.lPitch;
        BYTE *dest = (BYTE*)ddsd.lpSurface;

        for (int y = 0; y < h; y++)
        {
            // We'll update two pixels at a time
            for (int x = 0; x < w; x += 2)
            {
                // NOTE: The values used for the conversion between 
                // RGB and YUV are probably not the exact values, but
                // for this sample app they are good enough

                // We are using pure grayscale image which would allow
                // for plenty of optimizations but I'll show the complete
                // formula for those interested in showing color images
                DWORD clr = image[x + y*width];

                BYTE r = GetRValue(clr);
                BYTE g = GetGValue(clr);
                BYTE b = GetBValue(clr);

                if (r > 0)
                {
                    printf("qweqw");
                }

                //                          red    green    blue
                BYTE Y0 = (BYTE)(        0.29*r + 0.59*g + 0.14*b);
                BYTE U0 = (BYTE)(128.0 - 0.14*r - 0.29*g + 0.43*b);
                BYTE V0 = (BYTE)(128.0 + 0.36*r - 0.29*g - 0.07*b);

                clr = image[x + 1 + y*width];
                r = GetRValue(clr);
                g = GetGValue(clr);
                b = GetBValue(clr);

                BYTE Y1 = (BYTE)(        0.29*r + 0.57*g + 0.14*b);
                BYTE U1 = (BYTE)(128.0 - 0.14*r - 0.29*g + 0.43*b);
                BYTE V1 = (BYTE)(128.0 + 0.36*r - 0.29*g - 0.07*b);

                if (ddsd.ddpfPixelFormat.dwFourCC == MAKEFOURCC('Y', 'U', 'Y', '2'))
                {
                    *dest++ = Y0;
                    *dest++ = (U0 + U1) / 2;
                    *dest++ = Y1;
                    *dest++ = (V0 + V1) / 2;
                }
                else
                {
                    *dest++ = (U0 + U1) / 2;
                    *dest++ = Y0;
                    *dest++ = (V0 + V1) / 2;
                    *dest++ = Y1;
                }
            }

            dest += (pitch - w * 2);           
        }
    }
    else
    {
        // The surface is in normal 16bit RGB format 
        int w = ddsd.dwWidth;
        int h = ddsd.dwHeight;
        int pitch = ddsd.lPitch / 2;
        WORD *dest = (WORD*)ddsd.lpSurface;
        WORD pixel;

        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                // Simple gray scale value
                DWORD clr = image[x + y*width];
                BYTE r = GetRValue(clr);
                BYTE g = GetGValue(clr);
                BYTE b = GetBValue(clr);

                //                     red            green         blue
                if (ddsd.ddpfPixelFormat.dwGBitMask == 0x03e0)
                    // RGB 5:5:5
                    pixel = WORD(((r >> 3) << 10) + ((g >> 3) << 5) + (b >> 3));
                else
                    // RGB 5:6:5
                    pixel = WORD(((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3));

                *dest++ = pixel;
            }

            dest += (pitch - w);
        }
    }

    backbuffer->Unlock(0);

    return 0;
}

void DXOverlay::DestroyOverlay()
{
    // Release the backbuffer
    if (backbuffer != 0)
    {
        backbuffer->Release();
        backbuffer = 0;
    }

    // Release the overlay surface
    if (overlay)
    {
        // Use UpdateOverlay() with the DDOVER_HIDE flag 
        // to remove an overlay from the display.
        overlay->UpdateOverlay(0, primary, 0, DDOVER_HIDE, 0);
        overlay->Release();
        overlay = 0;
    }
}

DWORD DXOverlay::GetPixelValue(IDirectDrawSurface7 *surface, COLORREF rgb)
{
    COLORREF       oldPixel;
    HDC            dc;
    DWORD          pixel = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT        hr;

    if (rgb == CLR_INVALID)
        return CLR_INVALID;

    // We will use GDI to put a pixel of the wanted color on the
    // surface as GDI automatically transforms the color into one
    // that exists in the palette.
    hr = surface->GetDC(&dc);
    if (SUCCEEDED(hr))
    {
        // Save current pixel value
        oldPixel = GetPixel(dc, 0, 0);

        // Set our value
        SetPixel(dc, 0, 0, rgb);

        surface->ReleaseDC(dc);
    }

    // Now lock the surface so we can read back the converted color
    ddsd.dwSize = sizeof(ddsd);
    hr = surface->Lock(0, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, 0);
    if (SUCCEEDED(hr))
    {
        // Read a DWORD from the surface
        pixel = *(DWORD *)ddsd.lpSurface;

        // Mask out bits that are not part of the pixel
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            pixel &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;

        surface->Unlock(NULL);
    }

    // Restore the original pixel
    hr = surface->GetDC(&dc);
    if (SUCCEEDED(hr))
    {
        SetPixel(dc, 0, 0, oldPixel);
        surface->ReleaseDC(dc);
    }
    return pixel;
}

int DXOverlay::ShowOverlay()
{
    if (ddraw == 0 || primary == 0 || overlay == 0)
        return -1;

    // Destination rectangle is the full screen
    RECT rcDst;
    rcDst.left = 0;
    rcDst.top = 0;
    rcDst.right = GetSystemMetrics(SM_CXSCREEN);
    rcDst.bottom = GetSystemMetrics(SM_CYSCREEN);

    // Source rectangle is the full overlay surface
    RECT rcSrc;
    rcSrc.left = 0;
    rcSrc.top = 0;
    rcSrc.right = width;
    rcSrc.bottom = height;

    // Set the destination color key. Note that the value
    // must have the same pixelformat as the destination surface
    DDOVERLAYFX fx;
    memset(&fx, 0, sizeof(fx));
    fx.dwSize = sizeof(fx);
    fx.dckDestColorkey.dwColorSpaceLowValue = GetPixelValue(primary, keyColor);
    fx.dckDestColorkey.dwColorSpaceHighValue = GetPixelValue(primary, keyColor);

    DWORD flags = DDOVER_SHOW | DDOVER_DDFX | DDOVER_KEYDESTOVERRIDE;

    // Show the overlay surface
    HRESULT hr;
    hr = overlay->UpdateOverlay(&rcSrc, primary, &rcDst, flags, &fx);
    if (FAILED(hr))
        return -1;

    return 0;
}

int DXOverlay::UpdateOverlay(const UINT8 *pimage)
{
    // Make sure we haven't lost the surface
    if (primary == 0 || primary->IsLost() != DD_OK)
    {
        // Lost the primary surface, which normally means another
        // application stole it from us. If we loose the primary
        // surface we might as well assume we've lost the overlay
        // surface too. At this moment we could try to recreate
        // the surface, or tell the application to try again later.
        return -1;
    }

    // Copy the image to the backbuffer
    CopyImageToSurface(pimage);

    // Show the backbuffer
    HRESULT hr = overlay->Flip(NULL, DDFLIP_WAIT);
    if (FAILED(hr))
        return -1;

    return 0;
}