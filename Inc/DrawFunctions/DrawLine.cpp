#include "DrawLine.hpp"

void DrawFunctions::DrawLine(IScreen &screen, Point2_i a, Point2_i b, RGBColor color)
{
    // TODO:::
    //ScreenFeatures::DrawLine line{ a.x, a.y, b.x, b.y, color };
    //if (screen.Feature(line))
    //    return;

    const bool verticalLine = a.x == b.x;
	if (verticalLine)
	{
		if (a.y > b.y)
		{
			int y_a_temp = a.y;
			a.y = b.y;
			b.y = y_a_temp;
		}
		for (int y = a.y; y <= b.y; ++y)
		{
			if (y >= screen.Height())
				break;
			screen.PixelSet(a.x, y, color);
		}
	}
	else
	{
		const float K = 1.0f * (b.y - a.y) / (b.x - a.x);
		const float Ks = 1.0f * a.y - K * a.x;

		if (a.x > b.x)
		{
			int a_x_temp = a.x, a_y_temp = a.y;
			a.x = b.x;
			a.y = b.y;
			b.x = a_x_temp;
			b.y = a_y_temp;
		}

		for (int x = a.x; x <= b.x; ++x)
		{
			int y = static_cast<int>(K * x + Ks);
			if (x >= screen.Width())
				break;
			else if (y >= screen.Height())
				continue;
			screen.PixelSet(x, y, color);
		}
	}
}
