/*
 * SSD1306_Screen.hpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifndef INC_SSD1306_SSD1306_SCREEN_HPP_
#define INC_SSD1306_SSD1306_SCREEN_HPP_

#include "LibrariesExport.h"

#include "SSD1306/SSD1306_Properties.hpp"

#include <stdint.h>

#include <Graphics/IFont.hpp>
#include <Graphics/IScreen.hpp>

struct LETO_CORE_EXPORT SSD1306_Screen : public IScreen
{
protected:
	uint8_t render_part{}, render_column{};

public:
	struct SSD1306_Page
	{
		uint8_t columns[SSD1306_ColumnsCount];
	};
	SSD1306_Page pages[SSD1306_PagesCount];

	SSD1306_Screen();

	int Width() const override { return SSD1306_Width; }
	int Height() const override { return SSD1306_Height; }

	void PixelSet(int x, int y, RGBColor color) override;
	RGBColor PixelGet(int x, int y) override;
	void ClearScreen() override;
	void FillScreen(RGBColor color) override;
};

#endif /* INC_SSD1306_SSD1306_SCREEN_HPP_ */
