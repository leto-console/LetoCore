/**
 * @file GlyphData.hpp
 * @date Sep 23, 2025
 * @author Rakhimov T.
 * 
 */

#ifndef INC_GRAPHICS_GLYPH_DATA_HPP_
#define INC_GRAPHICS_GLYPH_DATA_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <cstring>

#pragma pack(push, 1)

/**
 * @brief Глиф
 */
struct LETO_CORE_EXPORT GlyphData
{
	const uint8_t width{};	///< Ширина битмапа
	const uint8_t height{};	///< Высота битмапа

	const uint8_t* bitmap{};		///< Основное изображение

	constexpr GlyphData(uint8_t width = 0, uint8_t height = 0, const uint8_t* bitmap = nullptr)
		: width{ width }, height{ height }, bitmap{ bitmap }
	{ }

	/**
	 * @brief Размер глифа в памяти
	 */
	uint32_t Size() const
	{
		return Size(width, height);
	}

	static uint32_t Size(uint16_t width, uint16_t height)
	{
		return ((height + 7) >> 3) * width;
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

    bool RawGetPixel(int x, int y) const
    {
        const int bytes_per_column = (height + 7) >> 3;
		const int idx = ((bytes_per_column * x) + (y >> 3));

		return (bitmap[idx] >> (y & 0x7)) & 1;
    }
};

#pragma pack(pop)

#endif