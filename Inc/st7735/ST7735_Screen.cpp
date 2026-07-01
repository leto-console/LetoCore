/*
 * ST7735_Screen.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifdef USE_HAL_DRIVER

#include "ST7735/ST7735_Screen.hpp"
#include <stddef.h>
#include <cstring>

#include <st7735/low_level/st7735.h>


static uint16_t RGBColor_to_U16Model(RGBColor color)
{
	uint16_t u16_clr = ((uint16_t)(color.R >> 3) << 11) |
					   ((uint16_t)(color.G >> 2) << 5)  |
						(uint16_t)(color.B >> 3);
	u16_clr = (u16_clr << 8) | (u16_clr >> 8); // Меняем байты местами

    return u16_clr;
}

static RGBColor U16Model_to_RGBColor(uint16_t color)
{
	RGBColor result;

    // 1. Извлекаем красный: маска 0xF800 (верхние 5 бит), двигаем вправо на 11
    // Затем растягиваем 5 бит до 8: (r << 3) | (r >> 2) для точности
    uint8_t r5 = (color & 0xF800) >> 11;
    result.R = (r5 * 255) / 31; // Самый точный способ масштабирования

    // 2. Извлекаем зеленый: маска 0x07E0 (средние 6 бит), двигаем вправо на 5
    uint8_t g6 = (color & 0x07E0) >> 5;
    result.G = (g6 * 255) / 63;

    // 3. Извлекаем синий: маска 0x001F (нижние 5 бит)
    uint8_t b5 = (color & 0x001F);
    result.B = (b5 * 255) / 31;

    return result;
}


ST7735_Screen::ST7735_Screen()
{
	ClearScreen();
}

void ST7735_Screen::SetArea(int x, int y)
{
	area_x = x;
	area_y = y;
}

void ST7735_Screen::PixelSet(int x, int y, RGBColor color)
{
	if (x >= (area_x + area_width) || y >= (area_y + area_height) ||
		x < area_x || y < area_y)
		return;
	image[(y - area_y) * area_width + x - area_x] = RGBColor_to_U16Model(color);
}

RGBColor ST7735_Screen::PixelGet(int x, int y)
{
	if (x >= (area_x + area_width) || y >= (area_y + area_height) ||
		x < area_x || y < area_y)
		return BlackColor;

	uint16_t color = image[(y - area_y) * area_width + x - area_x];

	return U16Model_to_RGBColor(color);
}

void ST7735_Screen::ClearScreen()
{
	FillScreen(BlackColor);
}

void ST7735_Screen::FillScreen(RGBColor color)
{
	uint16_t u16_color = RGBColor_to_U16Model(color);
	for (size_t i = 0; i < area_width * area_height; ++i)
        image[i] = u16_color;
}

#endif /* USE_HAL_DRIVER */
