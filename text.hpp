#pragma once

#include "image.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct GlyphAtlas {
    Image atlas;
    int padding;
    int size;
    uint32_t first;
    uint32_t num;
    int charsPerRow;
};

GlyphAtlas Text_CreateAtlas(FT_Face& face, int size, uint32_t first, uint32_t num, int padding = 0);
void Text_DestroyAtlas(GlyphAtlas& atlas);
void Text_GetPos(const GlyphAtlas& atlas, uint32_t i, uint16_t* x, uint16_t* y);