#include "DrawCircle.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

void DrawFunctions::DrawCircle(IScreen &screen, Point2_i center, int radius, RGBColor color)
{
    // TODO: !!! Разобраться с signed экранными координатами
	int top_bound = center.y + radius < screen.Height() ? center.y + radius : screen.Height() - 1;
	int bottom_bound = (center.y - radius >= 0) ? center.y - radius : 0;
	for (int y_slice = bottom_bound; y_slice <= top_bound; ++y_slice)
	{
		/// TODO: нужно ли так плотно связывать работу с LetoAPI? 
		/// или можно написать кастомную обертку над cmath, которая будет работать поверх
		int x_offset = static_cast<int>(leto_api_v1->Math->sqrtf(radius * radius - (y_slice - center.y) * (y_slice - center.y)));
		int left_bound = center.x - x_offset >= 0 ? center.x - x_offset : 0;
		int rigth_bound = center.x + x_offset < screen.Width() ? center.x + x_offset : screen.Width() - 1;
		for (int x_slice = left_bound; x_slice <= rigth_bound; ++x_slice)
		{
			screen.PixelSet(x_slice, y_slice, color);
		}
	}
}