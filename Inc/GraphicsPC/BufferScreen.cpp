#include <stddef.h>
#include <cstring>

#include "BufferScreen.hpp"

const uint32_t WHITE = 0xffffff;
const uint32_t BLACK = 0x000000;

static uint32_t RGBColor_to_U32model(RGBColor color)
{
	return (color.R << 16) | (color.G << 8) | (color.B << 0);
}

static RGBColor U32model_to_RGBColor(uint32_t color)
{
	RGBColor rgb{};
	rgb.R = (color >> 16) & 0xFF;
	rgb.G = (color >> 8) & 0xFF;
	rgb.B = (color >> 0) & 0xFF;
	return rgb;
}

BufferScreen::BufferScreen(int width, int height)
	: width{ width }, height{ height }
{
	image = new uint32_t[width * height];
	ClearScreen();
}

void BufferScreen::SetSize(int width, int height)
{
	if (this->width == width &&
		this->height == height)
		return;
	if (image) delete[] image;
	this->width = width;
	this->height = height;
	image = new uint32_t[width * height];
}

void BufferScreen::PixelSet(int x, int y, RGBColor color)
{
	if (x >= width || y >= height ||
		x < 0 || y < 0)
		return;
	image[y * width + x] = RGBColor_to_U32model(color);
}

RGBColor BufferScreen::PixelGet(int x, int y)
{
	if (x >= width || y >= height ||
		x < 0 || y < 0)
		return BlackColor;
	
	return U32model_to_RGBColor(image[y * width + x]);
}

void BufferScreen::ClearScreen()
{
	FillScreen(BlackColor);
}

void BufferScreen::FillScreen(RGBColor color)
{
	memset(image, color == WhiteColor ? WHITE : BLACK, sizeof(uint32_t) * width * height);
}
