#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>

// ====================================================================================================

#include <Graphics/IScreen.hpp>

void DrawBitmap(LetoScreen_V1* screen, int32_t x, int32_t y, const LetoBitmap_V1* bitmap, LetoColor_V1 color)
{
    if (!screen || !bitmap)
        return;
    
    IScreen& impl_screen = *IScreen::FromHandle(screen);
    const BitmapData& data = *BitmapData::FromHandle(bitmap);

    for (int _x = 0; _x < data.width; ++_x)
	{
		for (int _y = 0; _y < data.height; ++_y)
		{
            if (data.GetPixel(_x, _y))
                impl_screen.PixelSet(x + _x, y + _y, color);
		}
	}
}

// ====================================================================================================

const GraphicsAPI_V1* Make_GraphicsAPI()
{
    static const GraphicsAPI_V1 api
    {
        &DrawBitmap,
    };
    
    return &api;
}
