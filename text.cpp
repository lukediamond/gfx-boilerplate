#include "text.hpp"

#include <cmath>

static uint32_t NearestPowerOf2(uint32_t x) {
    int n = 1;
    while (n < x) n <<= 1;
    return n;
}

GlyphAtlas Text_CreateAtlas(FT_Face& face, int size, uint32_t first, uint32_t num, int padding) {
    int charsPerRow = (int) ceil(sqrt(num));
    uint32_t width = NearestPowerOf2(charsPerRow * (size + padding));

    GlyphAtlas atlas;
    atlas.atlas = Image_Create(width, width, 1);
    atlas.first = first;
    atlas.num = num;
    atlas.size = size;
    atlas.padding = padding;
    atlas.charsPerRow = charsPerRow;
    atlas.metrics = new GlyphMetrics[num] {};

    Image& img = atlas.atlas;
    uint8_t* imgbuf = (uint8_t*) img.data;

    FT_Set_Pixel_Sizes(face, 0, size);
    for (uint32_t i = 0; i < num; ++i) {
        uint32_t charidx = first + i;
        FT_Load_Glyph(face, FT_Get_Char_Index(face, charidx), FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        const FT_Glyph_Metrics& ftmetrics = face->glyph->metrics;
        GlyphMetrics& metrics = atlas.metrics[i];
        metrics.advanceX = ftmetrics.horiAdvance / 64;
        metrics.advanceY = ftmetrics.vertAdvance / 64;
        metrics.bearingX = ftmetrics.horiBearingX / 64;
        metrics.bearingY = ftmetrics.horiBearingY / 64;

        const FT_Bitmap& bmp = face->glyph->bitmap;
        glm::ivec2 dstpos;
        Text_GetPos(atlas, i, dstpos);
        dstpos *= (size + padding);

        for (int y = 0; y < bmp.rows; ++y) {
            int ypos = y + dstpos.y;
            for (int x = 0; x < bmp.width; ++x) {
                imgbuf[ypos * img.pitch + x + dstpos.x] = bmp.buffer[y * bmp.pitch + x];
            }
        }
    }

    return atlas;
}

void Text_DestroyAtlas(GlyphAtlas& atlas) {
    Image_Free(atlas.atlas);
    delete[] atlas.metrics;
}

void Text_GetPos(const GlyphAtlas& atlas, uint32_t i, glm::ivec2& pos) {
    pos.x = i % atlas.charsPerRow;
    pos.y = i / atlas.charsPerRow;
}

void Text_GetDrawPos(const GlyphAtlas& atlas, uint32_t i, glm::vec2& tl, glm::vec2& br) {
    glm::ivec2 pos;
    // assumes square image
    Text_GetPos(atlas, i, pos);
    tl = (glm::vec2) pos * (float) (atlas.size + atlas.padding) / (float) atlas.atlas.width;
    br = tl + glm::vec2 {(float) atlas.size / (float) atlas.atlas.width};
}