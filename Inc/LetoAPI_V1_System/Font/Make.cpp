#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include <Graphics/DefaultFont.hpp>

const IFont* GetFont(uint32_t width, uint32_t height, uint32_t type)
{
    if (width == 8 && height == 8) return &Default_Font_8x8;
    if (width == 7 && height == 7) return type == 1 ? &Default_Font_7x7_small : &Default_Font_7x7;

    // Шрифт по умолчанию, если шрифт не найден
    return &Default_Font_8x8;
}

// ====================================================================================================

const FontAPI_V1* Make_FontAPI()
{
    static const FontAPI_V1 api
    {
        &GetFont
    };
    
    return &api;
}
