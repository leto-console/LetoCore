#include "DrawRectangle.hpp"

void DrawFunctions::DrawRectangle(IScreen &screen, Point2_i a, Point2_i b, RGBColor color)
{
    if (a.x > b.x)
	{
		int x_a_temp = a.x, y_a_temp = a.y;
		a.x = b.x;
		a.y = b.y;
		b.x = x_a_temp;
		b.y = y_a_temp;
	}

	for (int x = a.x; x <= b.x; ++x)
		for (int y = a.y; y <= b.y; ++y)
			screen.PixelSet(x, y, color);
}

void DrawFunctions::DrawOutlinedRectangle(IScreen &screen, Point2_i a, Point2_i b, RGBColor color, RGBColor outline_color)
{
    if (a.x > b.x)
	{
		int x_a_temp = a.x, y_a_temp = a.y;
		a.x = b.x;
		a.y = b.y;
		b.x = x_a_temp;
		b.y = y_a_temp;
	}

	for (int x = a.x; x <= b.x; ++x)
    {
        for (int y = a.y; y <= b.y; ++y)
        {
            if (x == a.x || x == b.x || y == a.y || y == b.y)
                screen.PixelSet(x, y, outline_color);
            else
                screen.PixelSet(x, y, color);
        }
    }
		
}
