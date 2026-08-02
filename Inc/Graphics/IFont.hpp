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

 // Интерфейс экранного шрифта
class LETO_CORE_EXPORT IFont
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

#endif
