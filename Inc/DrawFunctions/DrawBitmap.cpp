#include "DrawBitmap.hpp"

#define __GET_BIT(n, b) ((bool) ((n >> b) & 1))

void DrawFunctions::DrawBitmap(IScreen &screen, Point2_i point, const uint8_t *bitmap, uint8_t width, uint8_t height, RGBColor bitmap_color, bool inverse)
{
    if (!bitmap)
        return;

    for (int _x = 0; _x < width; ++_x)
	{
		for (int _y = 0; _y < height; ++_y)
		{
			int width_bytes = (height + 7) / 8;
			int idx = (width_bytes * _x + _y / 8);
			bool fill = __GET_BIT(bitmap[idx], _y % 8);
			if (fill != inverse)
				screen.PixelSet(point.x + _x, point.y + _y, bitmap_color);
		}
	}
}

void DrawFunctions::DrawBitmap(IScreen &screen, Point2_i point, const uint8_t *bitmap, uint8_t width, uint8_t height, RGBColor bitmap_color, RGBColor background_color, bool inverse)
{
	if (!bitmap)
        return;

    for (int _x = 0; _x < width; ++_x)
	{
		for (int _y = 0; _y < height; ++_y)
		{
			int width_bytes = (height + 7) / 8;
			int idx = (width_bytes * _x + _y / 8);
			bool fill = __GET_BIT(bitmap[idx], _y % 8);
			screen.PixelSet(point.x + _x, point.y + _y, fill != inverse ? bitmap_color : background_color);
		}
	}
}
