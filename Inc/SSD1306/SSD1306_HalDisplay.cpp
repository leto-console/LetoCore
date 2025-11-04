#ifdef USE_HAL_DRIVER

#include "SSD1306/SSD1306_HalDisplay.hpp"
#include "SSD1306/SSD1306_Properties.hpp"
#include <SSD1306/SSD1306_Screen.hpp>

#define CTRL_ALL_COMM		0x00
#define CTRL_ONE_COMM		0x80
#define CTRL_ALL_DATA		0x40
#define CTRL_ONE_DATA		0xA0

#define COM_COLUMN_ADDR		0x21
#define COM_PAGE_ADDR		0x22

#define SSD1306_TIMEOUT 	1000
#define SSD1306_Address 	0x78

SSD1306_HalDisplay::SSD1306_HalDisplay(I2C_HandleTypeDef* hi2c, bool dma_mode)
	: hi2c{ hi2c }, dma_mode{ dma_mode }
{
}

SSD1306_HalDisplay::~SSD1306_HalDisplay()
{
}

bool SSD1306_HalDisplay::IsOpen()
{
	return true;
}

bool SSD1306_HalDisplay::Init()
{
	HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(hi2c, SSD1306_Address, 20, SSD1306_TIMEOUT);
	(void)ret;
	// Инициализация дисплея
	uint8_t data[] =
	{
			CTRL_ALL_COMM,							// (Mode ctrl)
			0xd5, 0x80,    							// Set Osc Frequency (clock div)
			0xae|0,									// Display OFF
			0xa8, (uint8_t) SSD1306_Height - 1, 			// Set MUX Ratio (Set multiplex)
			0xd3, 0x00,								// Set Display offset
			0x40|0,									// Set Display start line
			0x8d, 0x14,    							// Enable charge pump regulator (charge pump SSD1306)
			0xa0|1,									// Set segment re-map (non-reverse)
			0xc0|(1<<3),							// Set COM Output Scan Direction
			0xda, 0x02|( (uint8_t) (SSD1306_Height == 64) << 4), 						// Set COM Pins hardware configurations
			0x81, 0x7f,								// Set Contrast Control (Яркость)
			0xa4|0,									// Disable Entire Display On (force on)
			0xa6|0,									// Set normal display (inverse)
			0xae|1,									// Display ON
	};
	HAL_I2C_Master_Transmit(hi2c, SSD1306_Address, data, sizeof(data) / sizeof(uint8_t), SSD1306_TIMEOUT);

	if (dma_mode)
	{
		uint8_t add_data[] =
		{
			CTRL_ALL_COMM,
			0x20,0x00,								// Horizontal Addressing Mode
		};
		HAL_I2C_Master_Transmit(hi2c, SSD1306_Address, add_data, sizeof(add_data) / sizeof(uint8_t), SSD1306_TIMEOUT);
	}

	return true;
}

bool SSD1306_HalDisplay::Render(IDrawer* drawer)
{
	//HAL_StatusTypeDef ret = HAL_I2C_IsDeviceReady(hi2c, SSD1306_Address, 20, SSD1306_TIMEOUT);
	//(void) ret;

	static uint8_t page = 0, column = 0;
	static SSD1306_Screen screen;
	drawer->Draw(screen);

	uint8_t lower_nible = column & 0xF;
	uint8_t higher_nible = (column >> 4) & 0xF;

	if (dma_mode)
	{
		static uint8_t data[6 + 1024] =
		{
			COM_PAGE_ADDR, (uint8_t)(0x00), (uint8_t)(0xFF),
			COM_COLUMN_ADDR, (uint8_t)(0x00), (uint8_t)(SSD1306_Width - 1),
		};

		while (HAL_I2C_IsDeviceReady(hi2c, SSD1306_Address, 20, SSD1306_TIMEOUT) == HAL_BUSY) { }

		for (size_t i = 0; i < 6; ++i)
		{
			HAL_I2C_Mem_Write(hi2c, SSD1306_Address, 0x00, 1, &data[i], 1, 100);
		}

		for (size_t i = 0; i < 1024; ++i)
		{
			data[i + 6] = screen.pages[i / SSD1306_ColumnsCount].columns[i % SSD1306_ColumnsCount];
		}

		 HAL_I2C_Mem_Write_DMA(hi2c, SSD1306_Address, 0x40, 1, data + 6, 1024);

		return true;
	}

	uint8_t data[135] =
	{
		CTRL_ONE_COMM, lower_nible,							// lower nibble of the column start address register
		CTRL_ONE_COMM, (uint8_t)(0x10 | higher_nible),		// higher nibble of the column start address register
		CTRL_ONE_COMM, (uint8_t)(0xB0 | page),
		CTRL_ALL_DATA
	};
	for (size_t i = 0, iCol = column; i < SSD1306_Width && iCol != SSD1306_ColumnsCount; ++i, ++iCol)
	{
		data[i + 7] = screen.pages[page].columns[iCol];
	}
	HAL_I2C_Master_Transmit(hi2c, SSD1306_Address, data, 7 + SSD1306_Width, SSD1306_TIMEOUT);

	/// Обработка рендеринга

	if (page >= SSD1306_PagesCount && column == 1)
	{
		page = column = 0;
		return true;
	}
	else if (column == 1)
	{
		column = 0;
		page++;
	}
	else
	{
		column++;
	}

	return false;
}

void SSD1306_HalDisplay::Loop()
{
}

#endif /* USE_HAL_DRIVER */


