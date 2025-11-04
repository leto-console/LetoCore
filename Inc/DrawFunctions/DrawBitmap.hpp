/*
 * DrawBitmap.hpp
 *
 *  Created on: Mar 21, 2026
 *      Author: Timur
 */

#ifndef INC_DRAW_FUNCTIONS_DRAW_BITMAP_HPP_
#define INC_DRAW_FUNCTIONS_DRAW_BITMAP_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>
#include <Graphics/RGBColor.hpp>

namespace DrawFunctions
{
    /* 
		Отображение битмапа произвольного размера на экране.
		Порядок байтов в bitmap (для примера, высота битмапа 10 пикселей) должен быть следующий:
		0-ый байт: с 7-го по 0-ый бит 1 столбца, где 0-ой - самый верхний
		1-ый байт: с 9-го по 8-ой бит 1 столбца, где 8-ой расположен ниже 7-го
		2-ый байт: с 7-го по 0-ый бит 2 столбца, где 0-ой - самый верхний
		3-ый байт: с 9-го по 8-ой бит 2 столбца, где 8-ой расположен ниже 7-го
		и т.д.
	*/

    // Отрисовать Bitmap определенным цветом (позиции битов = 1)
    extern LIBRARIES_EXPORT void DrawBitmap(IScreen& screen, Point2_i point, const uint8_t* bitmap, uint8_t width, uint8_t height, RGBColor bitmap_color, bool inverse = false);

	extern LIBRARIES_EXPORT void DrawBitmap(IScreen& screen, Point2_i point, const uint8_t* bitmap, uint8_t width, uint8_t height, RGBColor bitmap_color, RGBColor background_color, bool inverse = false);

    inline LIBRARIES_EXPORT void DrawBitmap(IScreen& screen, Point2_i point, const BitmapData& data, RGBColor bitmap_color, bool inverse = false)
	{
		DrawBitmap(screen, point, data.bitmap, data.width, data.height, bitmap_color, inverse);
	}

    inline void DrawBitmap(IScreen& screen, Point2_i point, const BitmapData& data, RGBColor bitmap_color, RGBColor background_color, bool inverse = false)
	{
		if (data.background)
			DrawBitmap(screen, point, data.background, 	data.width, data.height, background_color, inverse);
		else
			DrawBitmap(screen, point, data.bitmap, 		data.width, data.height, background_color, !inverse);
		
		DrawBitmap(screen, point, data.bitmap, data.width, data.height, bitmap_color, inverse);
	}
}

#endif