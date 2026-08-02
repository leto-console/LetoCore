/*
 * ST7735_Screen.hpp
 *
 *  Created on: Apr 22, 2026
 *      Author: Timur
 */

#ifdef USE_HAL_DRIVER

#ifndef INC_ST7735_ST7735_SCREEN_HPP_
#define INC_ST7735_ST7735_SCREEN_HPP_

#include "LibrariesExport.h"

#include "ST7735/ST7735_Properties.hpp"

#include <stdint.h>

#include <Graphics/IFont.hpp>
#include <Graphics/IScreen.hpp>

struct LETO_CORE_EXPORT ST7735_Screen : public IScreen
{
protected:
	int area_x{}, area_y{};

public:
	enum {
		area_width = 160,
#ifdef STM32F411xE
		area_height = 128
#else
		area_height = 32
#endif
	};

	uint16_t image[area_width * area_height];

	ST7735_Screen();

	/// Задать область отрисовки (размером 160 на 32)
	void SetArea(int x, int y);

	int Width() const override { return ST7735_Width; }
	int Height() const override { return ST7735_Height; }

	void PixelSet(int x, int y, RGBColor color) override;
	RGBColor PixelGet(int x, int y) override;
	void ClearScreen() override;
	void FillScreen(RGBColor color) override;
};

#endif /* INC_ST7735_ST7735_SCREEN_HPP_ */

#endif /* USE_HAL_DRIVER */
