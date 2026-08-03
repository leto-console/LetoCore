/**
 * @file SSD1306_Display.hpp
 * @date Apr 22, 2026
 * @author Rakhimov T.
 */

#ifdef USE_HAL_DRIVER

#ifndef INC_SSD1306_SSD1306_DISPLAY_HPP_
#define INC_SSD1306_SSD1306_DISPLAY_HPP_

#include "LibrariesExport.h"

#include "SSD1306/SSD1306_Properties.hpp"

#include <stdint.h>
#include <HAL_include/HAL.hpp>

#include <Graphics/IFont.hpp>
#include <Graphics/IDisplay.hpp>

struct LETO_CORE_EXPORT SSD1306_HalDisplay : public IDisplay
{
protected:
	uint8_t render_part{}, render_column{};

	I2C_HandleTypeDef* hi2c;
	const bool dma_mode{};

public:
	SSD1306_HalDisplay(I2C_HandleTypeDef* hi2c, bool dma_mode = false);
	~SSD1306_HalDisplay();

	bool Init() override;

	bool IsOpen() override;

	int Width() const override { return SSD1306_Width; }
	int Height() const override { return SSD1306_Height; }

    bool Render(IDrawer* drawer) override;

	void Loop() override;

};

#endif /* INC_SSD1306_SSD1306_SCREEN_HPP_ */

#endif /* USE_HAL_DRIVER */
