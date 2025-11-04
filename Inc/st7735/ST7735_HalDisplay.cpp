#ifdef USE_HAL_DRIVER

#include "ST7735/ST7735_HalDisplay.hpp"
#include "ST7735/ST7735_Properties.hpp"
#include <ST7735/ST7735_Screen.hpp>

#include <st7735/low_level/st7735.h>


ST7735_HalDisplay::ST7735_HalDisplay(SPI_HandleTypeDef* hspi)
	: hspi{ hspi }
{
}

ST7735_HalDisplay::~ST7735_HalDisplay()
{
}

bool ST7735_HalDisplay::IsOpen()
{
	return true;
}

bool ST7735_HalDisplay::Init()
{
	ST7735_Init();
	return true;
}

bool ST7735_HalDisplay::Render(IDrawer* drawer)
{
	static uint8_t row = 0, column = 0;
	static ST7735_Screen screen;

	int x = column * ST7735_Screen::area_width;
	int y = row * ST7735_Screen::area_height;

	screen.ClearScreen();
	screen.SetArea(x, y);

	drawer->Draw(screen);

	ST7735_DrawImage(x, y, ST7735_Screen::area_width, ST7735_Screen::area_height, screen.image);

	/// Обработка рендеринга
	int max_column = ST7735_WIDTH / ST7735_Screen::area_width - 1;
	int max_row = ST7735_HEIGHT / ST7735_Screen::area_height - 1;

	if (column == max_column)
	{
		if (row >= max_row)
		{
			row = column = 0;
			return true;
		}
		column = 0;
		row++;
	}
	else
	{
		column++;
	}

	return false;
}

void ST7735_HalDisplay::Loop()
{
}

#endif /* USE_HAL_DRIVER */


