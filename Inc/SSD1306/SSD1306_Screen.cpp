/*
 * SSD1306_Screen.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#include "SSD1306/SSD1306_Screen.hpp"
#include <stddef.h>
#include <cstring>

SSD1306_Screen::SSD1306_Screen()
{
	ClearScreen();
}

void SSD1306_Screen::PixelSet(int x, int y, RGBColor color)
{
	if (x >= SSD1306_Width || y >= SSD1306_Height ||
		x < 0 || y < 0)
		return;

	//x = x < SSD1306_Width ? x : SSD1306_Width - 1;
	//y = y < SSD1306_Height ? y : SSD1306_Height - 1;

	//y = SSD1306_Height - 1 - y; // 31 - x

	if (color == WhiteColor)
		pages[y >> 3].columns[x] |= (1 << (y & 0x7));
	else
		pages[y >> 3].columns[x] &= ~(1 << (y & 0x7));
}

RGBColor SSD1306_Screen::PixelGet(int x, int y)
{
	if (x >= SSD1306_Width || y >= SSD1306_Height ||
		x < 0 || y < 0)
		return BlackColor;

	return ((pages[y >> 3].columns[x] >> (y & 0x7)) & 1) ? WhiteColor : BlackColor;
}

void SSD1306_Screen::ClearScreen()
{
	FillScreen(BlackColor);
}

void SSD1306_Screen::FillScreen(RGBColor color)
{
	memset(pages, color == WhiteColor ? 1 : 0, SSD1306_ColumnsCount * SSD1306_PagesCount);
}
