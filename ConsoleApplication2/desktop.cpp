#include "desktop.h"

DWORD oldColor = CLR_INVALID;

void SetDesktopColor(DWORD clr)
{
    // Remember current background color
    if (oldColor == CLR_INVALID)
        oldColor = GetSysColor(COLOR_BACKGROUND);

    // Set the desktop background color to the wanted one
    int colorID = COLOR_BACKGROUND;
    COLORREF color = clr;
    SetSysColors(1, &colorID, &color);

    // Remove the desktop wallpaper, without updating the registry
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, "", 0);
}

//-------------------------------------------------------------------

void RestoreDesktop()
{
    // Restore the original desktop wallpaper
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, 0, 0);

    if (oldColor != CLR_INVALID)
    {
        int colorID = COLOR_BACKGROUND;
        SetSysColors(1, &colorID, &oldColor);
    }
}