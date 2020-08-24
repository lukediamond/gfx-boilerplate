#pragma once

#include "image.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/vec2.hpp>

struct GlyphMetrics {
    int16_t advanceX;
    int16_t bearingX;
    int16_t bearingY;
};

struct GlyphAtlas {
    Image atlas {};
    GlyphMetrics* metrics;
    int padding {};
    int size {};
    uint32_t first {};
    uint32_t num {};
    int charsPerRow {};
};

GlyphAtlas Text_CreateAtlas(FT_Face& face, int size, uint32_t first, uint32_t num, int padding = 0);
void Text_DestroyAtlas(GlyphAtlas& atlas);
void Text_GetPos(const GlyphAtlas& atlas, uint32_t i, glm::ivec2& pos);
void Text_GetDrawPos(const GlyphAtlas& atlas, uint32_t i, glm::vec2& tl, glm::vec2& br);