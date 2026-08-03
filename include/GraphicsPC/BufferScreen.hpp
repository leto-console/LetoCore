/**
 * @file BufferScreen.hpp
 * @date Apr 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GRAPHICS_PC_BUFFER_SCREEN_HPP_
#define INC_GRAPHICS_PC_BUFFER_SCREEN_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <Graphics/IFont.hpp>
#include <Graphics/IScreen.hpp>

struct LETO_CORE_EXPORT BufferScreen : public IScreen
{
public:
    int width{}, height{};
public:
    uint32_t* image{};

    BufferScreen() = default;
	BufferScreen(int width, int height);
    
    void SetSize(int width, int height);

	int Width() const override { return width; }
	int Height() const override { return height; }

	void PixelSet(int x, int y, RGBColor color) override;
	RGBColor PixelGet(int x, int y) override;
	void ClearScreen() override;
	void FillScreen(RGBColor color) override;
};

#endif /* INC_GRAPHICS_PC_BUFFER_SCREEN_HPP_ */
