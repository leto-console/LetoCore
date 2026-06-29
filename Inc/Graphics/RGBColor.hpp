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

// Вспомогательная функция для одного символа (C++11 style: один return)
constexpr uint8_t HexCharToValue(char c) {
    return (c >= '0' && c <= '9') ? static_cast<uint8_t>(c - '0') :
           (c >= 'A' && c <= 'F') ? static_cast<uint8_t>(c - 'A' + 10) :
           (c >= 'a' && c <= 'f') ? static_cast<uint8_t>(c - 'a' + 10) :
           0;
}

constexpr uint8_t GetColor(const char* txt, uint8_t order) { return HexCharToValue(txt[1 + 2 * order]) << 0x4 | HexCharToValue(txt[1 + 2 * order + 1]); }

// Цвет в формате RGB
struct LIBRARIES_EXPORT RGBColor
{    
    uint8_t R, G, B;

    RGBColor() = default;

    constexpr RGBColor(uint8_t R, uint8_t G, uint8_t B) 
        : R{ R }, G{ G }, B{ B }
    {}

    constexpr RGBColor(const char* txt)
        : R { GetColor(txt, 0) }, G { GetColor(txt, 1) }, B{ GetColor(txt, 2) }
    {}

    bool operator==(const RGBColor& color) const { return memcmp(this, &color, sizeof(RGBColor)) == 0; }
    bool operator!=(const RGBColor& color) const { return !operator==(color); }
};

constexpr RGBColor WhiteColor 		{ "#ffffff" };
constexpr RGBColor BlackColor 		{ "#000000" };

constexpr RGBColor CyanColor        { "#00ffff" };

/// https://www.materialpalette.com/

constexpr RGBColor GrayColor 		{ "#9E9E9E" };
constexpr RGBColor DarkGrayColor 	{ "#616161" };

constexpr RGBColor RedColor 		{ "#F44336" };
constexpr RGBColor DarkRedColor		{ "#D32F2F" };

constexpr RGBColor GreenColor 		{ "#4CAF50" };
constexpr RGBColor LightGreenColor 	{ "#8BC34A" };

constexpr RGBColor BlueColor		{ "#2196f3" };

constexpr RGBColor IndigoColor      { "#3f51b5" };

#endif
