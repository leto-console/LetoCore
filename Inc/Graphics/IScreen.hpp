/**
 * @file IScreen.hpp
 * @date Oct 19, 2025
 * @author Rakhimov T.
 */

#ifndef INC_GRAPHICS_ISCREEN_HPP_
#define INC_GRAPHICS_ISCREEN_HPP_

#include <stdint.h>

#include <Graphics/IFont.hpp>
#include <Graphics/BitmapData.hpp>
#include <Graphics/RGBColor.hpp>

#include <Data/StaticTextView.hpp>
#include <Data/Point2.hpp>

/**
 * @brief Интерфейс отрисовки на экране
 */
struct IScreen
{
	virtual ~IScreen() = default;

	virtual int Width() const = 0;
	virtual int Height() const = 0;

	virtual void PixelSet(int x, int y, RGBColor color) = 0;
	virtual RGBColor PixelGet(int x, int y) = 0;

	virtual void FillScreen(RGBColor color) = 0;
	virtual void ClearScreen() = 0;

	// Версии функций с Point2_i
	virtual void PixelSet(Point2_i data, RGBColor color)
	{
		PixelSet(data.x, data.y, color);
	}

	virtual RGBColor PixelGet(Point2_i data)
	{
		return PixelGet(data.x, data.y);
	}
};

#endif
