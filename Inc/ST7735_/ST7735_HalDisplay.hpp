/*
 * ST7735_HalDisplay.hpp
 *
 *  Created on: Apr 22, 2026
 *      Author: Rakhimov T.
 */

#ifdef USE_HAL_DRIVER

#ifndef ST7735_ST7735_HALDISPLAY_HPP_
#define ST7735_ST7735_HALDISPLAY_HPP_

#include "LibrariesExport.h"

#include "ST7735/ST7735_Properties.hpp"

#include <stdint.h>
#include <HAL_include/HAL.hpp>

#include <Graphics/IFont.hpp>
#include <Graphics/IDisplay.hpp>

struct LETO_CORE_EXPORT ST7735_HalDisplay : public IDisplay
{
protected:
	uint8_t render_part{}, render_column{};

	SPI_HandleTypeDef* hspi;

public:
	ST7735_HalDisplay(SPI_HandleTypeDef* hspi);
	~ST7735_HalDisplay();

	bool Init() override;

	bool IsOpen() override;

	int Width() const override { return ST7735_Width; }
	int Height() const override { return ST7735_Height; }

    bool Render(IDrawer* drawer) override;

	void Loop() override;

};

#endif /* ST7735_ST7735_HALDISPLAY_HPP_ */

#endif /* USE_HAL_DRIVER */
