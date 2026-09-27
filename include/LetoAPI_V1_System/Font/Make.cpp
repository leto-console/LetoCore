#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include <Graphics/DefaultFont.hpp>
#include <Graphics/Fonts/base_6x6/base_6x6_font.hpp>

static const LetoFont_V1* GetFont(uint32_t width, uint32_t height, uint32_t type)
{
    if (width == 8 && height == 8) return IFont::ToHandle(&Default_Font_8x8);
    if (width == 7 && height == 7) return IFont::ToHandle(type == 1 ? &Default_Font_7x7_small : &Default_Font_7x7);

    // Шрифт по умолчанию, если шрифт не найден
    return IFont::ToHandle(&Default_Font_8x8);
}

static const LetoFont_V1* FindFont(uint8_t height, LetoFont_V1_Type type)
{
    const GlyphFont* default_font = &Base_6x6_Font;
    const GlyphFont* find_font = GlyphFont::Find(height, type);
    
    return GlyphFont::ToHandle(find_font ? find_font : default_font);
}

// ====================================================================================================

const FontAPI_V1* Make_FontAPI()
{
    static const FontAPI_V1 api
    {
        &GetFont,
        &FindFont
    };
    
    return &api;
}
