/*
 * IFont.hpp
 *
 *  Created on: Nov 09, 2025
 *      Author: Timur
 */

#ifndef INC_GRAPHICS_MANAGER_I_FONT_HPP_
#define INC_GRAPHICS_MANAGER_I_FONT_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <LetoAPI_V1/LetoAPI_V1_Def.h>
#include <LetoAPI_V1/Font/LetoFont_V1_Types.h>
#include <Graphics/GlyphData.hpp>
#include <Data/StaticList.hpp>

 // Интерфейс экранного шрифта
class LETO_CORE_EXPORT IFont : public LetoHandleImpl<IFont, LetoFont_V1>
{
protected:
	uint8_t width, height;

public:
	IFont(uint8_t width, uint8_t height) : width{ width }, height{ height } { }
	virtual ~IFont() = default;

	uint8_t GetWidth() const { return width; }
	uint8_t GetHeight() const { return height; }

	virtual const uint8_t* GetEmptyChar() const = 0;
	virtual const uint8_t* GetLoveChar() const = 0;
	virtual const uint8_t* GetCuteChar() const = 0;

	virtual const uint8_t* GetASCIIChar(uint32_t code) const = 0;
	virtual const uint8_t* GetRussianChar(uint32_t first_part, uint32_t second_part) const = 0;
};

enum GLYPH_FONT_PLACE : uint8_t
{
	GFP_NONE = 0,
	GFP_BUILD_IN,	///< Built-in font
	GFP_IMPORTED	///< Imported font
};

constexpr size_t TABLE_ASCII_SIZE = 16 * 6;
constexpr size_t TABLE_RUS_SIZE = 16 * 4 + 2;

class LETO_CORE_EXPORT GlyphFont : public LetoHandleImpl<GlyphFont, LetoFont_V1>
{
public:
	GlyphFont(uint8_t height, uint8_t type, uint8_t place,
		const GlyphData (&ascii)[TABLE_ASCII_SIZE],
		const GlyphData (&russian)[TABLE_RUS_SIZE]) 
		: height{ height }, type{ type }, place{ place }, ascii{ ascii }, russian{ russian } 
	{
		if (place == GFP_BUILD_IN)
			GlobalFonts().push_back(this);
	}

	static const GlyphFont* Find(uint8_t height, uint8_t type)
	{
		// TODO: Add sorting and binary search
		for (const GlyphFont* font : GlobalFonts())
		{
			if (font->height == height &&
				font->type == type)
				return font;
		}
		return nullptr;
	}

	uint8_t GetHeight() const { return height; }
	uint8_t GetType() const { return type; }

	GlyphData GetUTF8(uint8_t symbol1, uint8_t symbol2 = 0) const
	{
		if (symbol2)
		{
			switch (symbol1)
			{
			case 208:
			{
				if (symbol2 >= 144 && symbol2 <= 191)
					return russian[symbol2-144];
				if (symbol2 == 129)
					return russian[64];
			}
			case 209:
			{
				if (symbol2 >= 128 && symbol2 <= 143)
					return russian[0x30 + (symbol2-128)];
				if (symbol2 == 145)
					return russian[65];
			}
			default:
				break;
			}
		}
		else if (symbol1 >= 32 && 126)
		{
			return ascii[symbol1-32];
		}

		return GlyphData{};
	}

protected:
	static StaticList<const GlyphFont*, 16>& GlobalFonts()
	{
		static StaticList<const GlyphFont*, 16> global_fonts;
		return global_fonts;
	}

	const uint8_t height, type, place;

	const GlyphData (&ascii)[TABLE_ASCII_SIZE];
	const GlyphData (&russian)[TABLE_RUS_SIZE];
};

#endif
