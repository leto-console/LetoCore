/**
 * @file BitmapData.hpp
 * @date Dec 26, 2025
 * @author Rakhimov T.
 * 
 */

#ifndef INC_GRAPHICS_BITMAP_DATA_HPP_
#define INC_GRAPHICS_BITMAP_DATA_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <cstring>
#include <LetoAPI_V1/LetoAPI_V1_Def.h>

/**
 * @brief Флаг битмапа
 */
enum BitmapFlags
{
	BMF_RLE = (1 << 1),		///< Используется RLE для кодирования изображения
};

/**
 * @brief Битмап
 * @headerfile BitmapData.hpp <Graphics/BitmapData.hpp>
 * 
 * Используется для хранения и передачи битовых карт - битмапов
 */
struct LETO_CORE_EXPORT BitmapData : public LetoHandleImpl<BitmapData, LetoBitmap_V1>
{
protected:
	bool editable{};	///< Можно ли менять битмап (должен быть расположен в изменяемой области памяти)

public:
	uint16_t width{};	///< Ширина битмапа
	uint16_t height{};	///< Высота битмапа

	uint8_t* bitmap{};		///< Основное изображение

	uint32_t ID{};		///< Идентификатор для поиска
	uint32_t flags{};	///< Флаги битмапа

	BitmapData(
		uint16_t width = 0, uint16_t height = 0, 
		const uint8_t* bitmap = nullptr, 
		uint32_t ID = 0, uint32_t flags = 0)

		: width{ width }, height{ height }, 
		bitmap{ const_cast<uint8_t*>(bitmap) }, //background{ const_cast<uint8_t*>(background) },
		ID{ ID }, flags{ flags },
		editable{ false }
	{ }

	BitmapData(
		uint16_t width, uint16_t height, 
		uint8_t* bitmap, 
		uint32_t ID = 0, uint32_t flags = 0)

		: width{ width }, height{ height }, 
		bitmap{ bitmap }, //background{ background },
		ID{ ID }, flags{ flags },
		editable{ true }
	{ }

	/**
	 * @brief Размер битмапа в памяти
	 */
	uint32_t Size() const
	{
		return Size(width, height);
	}

	static uint32_t Size(uint16_t width, uint16_t height)
	{
		return ((height + 7) >> 3) * width;
	}

	void SetPixel(int x, int y, bool state)
	{
		if (!editable || !bitmap)
			return;

		if (x < 0 || x >= width || y < 0 || y >= height)
			return;

		const int bytes_per_column = (height + 7) >> 3;
		const int idx = ((bytes_per_column * x) + (y >> 3));

		if (state)
			bitmap[idx] |= 1 << (y & 0x7);
		else
			bitmap[idx] &= ~(1 << (y & 0x7));
	}

	bool GetPixel(int x, int y) const
	{
		if (!bitmap)
			return false;

		if (x < 0 || x >= width || y < 0 || y >= height)
			return false;
		
		const int bytes_per_column = (height + 7) >> 3;
		const int idx = ((bytes_per_column * x) + (y >> 3));

		return (bitmap[idx] >> (y & 0x7)) & 1;
	}
};

#endif