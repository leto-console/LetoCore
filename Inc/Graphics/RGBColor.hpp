/*
 * RGBColor.hpp
 *
 *  Created on: Mar 21, 2026
 *      Author: Timur
 */

#ifndef INC_GRAPHICS_RGB_COLOR_HPP_
#define INC_GRAPHICS_RGB_COLOR_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <cstring>

// Цвет в формате RGB
struct LIBRARIES_EXPORT RGBColor
{    
    uint8_t R, G, B;

    bool operator==(const RGBColor& color) const { return memcmp(this, &color, sizeof(RGBColor)) == 0; }
};

constexpr RGBColor WhiteColor 		{ 0xFF, 0xFF, 0xFF };
constexpr RGBColor BlackColor 		{ 0x00, 0x00, 0x00 };

constexpr RGBColor CyanColor        { 0x00, 0xFF, 0xFF };

/// https://www.materialpalette.com/

constexpr RGBColor GrayColor 		{ 0x9E, 0x9E, 0x9E };
constexpr RGBColor DarkGrayColor 	{ 0x61, 0x61, 0x61 };

constexpr RGBColor RedColor 		{ 0xF4, 0x43, 0x36 };
constexpr RGBColor DarkRedColor		{ 0xD3, 0x2F, 0x2F };

constexpr RGBColor GreenColor 		{ 0x4C, 0xAF, 0x50 };
constexpr RGBColor LightGreenColor 	{ 0x8B, 0xC3, 0x4A };

constexpr RGBColor BlueColor		{ 0x21, 0x96, 0xf3 };

#endif
