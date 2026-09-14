#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>

// ====================================================================================================

#include <Graphics/IScreen.hpp>

static void DrawBitmap(LetoScreen_V1* screen, int32_t x, int32_t y, const LetoBitmap_V1* bitmap, LetoColor_V1 color)
{
    if (!screen || !bitmap) return;

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

static void DrawPixel(LetoScreen_V1* screen, int32_t x, int32_t y, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    impl_screen.PixelSet({x, y}, color);
}

static void PointsSort(int32_t& x1, int32_t& y1, int32_t& x2, int32_t& y2)
{
    if (x1 > x2)
    {
        int32_t x1_temp = x1, y1_temp = y1;
        x1 = x2;
        y1 = y2;
        x2 = x1_temp;
        y2 = y1_temp;
    }
}

static void DrawLine(LetoScreen_V1* screen, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t thickness, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    const bool verticalLine = x1 == x2;
	if (verticalLine)
	{
		if (y1 > y2)
		{
			int y_a_temp = y1;
			y1 = y2;
			y2 = y_a_temp;
		}
		for (int y = y1; y <= y2; ++y)
		{
			if (y >= impl_screen.Height())
				break;
			impl_screen.PixelSet(x1, y, color);
		}
	}
	else
	{
		const float K = 1.0f * (y2 - y1) / (x2 - x1);
		const float Ks = 1.0f * y1 - K * x1;

        PointsSort(x1, y1, x2, y2);

		for (int x = x1; x <= x2; ++x)
		{
			int y = static_cast<int>(K * x + Ks);
			if (x >= impl_screen.Width())
				break;
			else if (y >= impl_screen.Height())
				continue;
			impl_screen.PixelSet(x, y, color);
		}
	}

}

static void DrawRect(LetoScreen_V1* screen, int32_t x1, int32_t y1, int32_t width, int32_t height, int32_t thickness, LetoColor_V1 color)
{
    if (!screen) return;
    IScreen& impl_screen = *IScreen::FromHandle(screen);

    int32_t x2 = x1 + width, y2 = y1 + height;

    if (!thickness)
    {
        for (int32_t x = x1; x <= x2; ++x)
            for (int32_t y = y1; y <= y2; ++y)
                impl_screen.PixelSet(x, y, color);
    }
    else
    {
        for (int32_t x = x1; x <= x2; ++x)
        {
            for (int32_t y = y1; y <= y2; ++y)
            {
                if (x == x1 || x == x2 || y == y1 || y == y2)
                    impl_screen.PixelSet(x, y, color);
            }
        }
    }
}

// ====================================================================================================

const GraphicsAPI_V1* Make_GraphicsAPI()
{
    static const GraphicsAPI_V1 api
    {
        &DrawBitmap,
        &DrawPixel,
        &DrawLine,
        &DrawRect
    };
    
    return &api;
}
