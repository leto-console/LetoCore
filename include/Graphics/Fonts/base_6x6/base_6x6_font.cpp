#include "base_6x6_font.hpp"

#include "sources/base_6x6_ascii.h"
#include "sources/base_6x6_rus.h"

const GlyphFont Base_6x6_Font
(
    6, 
    GFS_NORMAL, 
    GFT_BUILD_IN,
    __glyph_base_6x6_ascii__,
    __glyph_base_6x6_rus__
);
