/*
 * DefaultFont.hpp
 *
 *  Created on: Dec 09, 2025
 *      Author: Timur
 */

#ifndef INC_GRAPHICS_DEFAULT_FONT_H_
#define INC_GRAPHICS_DEFAULT_FONT_H_

#include "LibrariesExport.h"

#include <stdint.h>

#include <Graphics/IFont.hpp>

struct DefaultCharacter
{
	const uint8_t code[8];
};

// Шрифт
class LIBRARIES_EXPORT DefaultFont : public IFont
{
protected:
	const DefaultCharacter* EmptyCharacter{};
	const DefaultCharacter* LoveCharacter{};
	const DefaultCharacter* CuteCharacter{};

	const DefaultCharacter* SpecialCharacters{};		// 0::32 ... 0::47
	const DefaultCharacter* NumbericCharacters{};		// 0::48 ... 0::57
	const DefaultCharacter* SpecialCharacters2{};		// 0::58 ... 0::64
	const DefaultCharacter* EnglishCharacters{};		// 0::65 ... 0::90
	const DefaultCharacter* SpecialCharacters3{};		// 0::91 ... 0::96

	const DefaultCharacter* RussianCharacters{};		// 208::144 ... 208::175

public:
	DefaultFont(uint8_t width, uint8_t height) : IFont{ width, height } {}

	const uint8_t* GetEmptyChar() const override { return EmptyCharacter->code; };
	const uint8_t* GetLoveChar() const override { return LoveCharacter->code; };
	const uint8_t* GetCuteChar() const override { return CuteCharacter->code; };

	const uint8_t* GetASCIIChar(uint32_t code) const override
	{
		if (code >= 32 && code <= 47)
		{
			if (SpecialCharacters)
				return SpecialCharacters[code - 32].code;
		}
		else if (code >= 48 && code <= 57)
		{
			if (NumbericCharacters)
				return NumbericCharacters[code - 48].code;
		}
		else if (code >= 58 && code <= 64)
		{
			if (SpecialCharacters2)
				return SpecialCharacters2[code - 58].code;
		}
		else if (code >= 65 && code <= 90)
		{
			if (EnglishCharacters)
				return EnglishCharacters[code - 65].code;
		}
		else if (code >= 91 && code <= 96)
		{
			if (SpecialCharacters3)
				return SpecialCharacters3[code - 91].code;
		}
		else if (code >= 97 && code <= 122)
		{
			if (EnglishCharacters)
				return EnglishCharacters[code - 97].code;
		}

		return EmptyCharacter->code;
	};
	const uint8_t* GetRussianChar(uint32_t first_part, uint32_t second_part) const override
	{
		if (first_part == 208 && RussianCharacters)
		{
			if (second_part >= 144 && second_part <= 175)
			{
				return RussianCharacters[second_part - 144].code;
			}
			else if (second_part >= 176 && second_part <= 191)
			{
				return RussianCharacters[second_part - 176].code;
			}
		}
		else if (first_part == 209)
		{
			if (second_part >= 128 && second_part <= 143)
			{
				return RussianCharacters[second_part - 112].code;
			}
		}

		return EmptyCharacter->code;
	}
};

extern LIBRARIES_EXPORT const DefaultFont Default_Font_8x8;
extern LIBRARIES_EXPORT const DefaultFont Default_Font_7x7;
extern LIBRARIES_EXPORT const DefaultFont Default_Font_7x7_small;

#endif /* INC_SSD1306_CHARACTERS_HPP_ */
