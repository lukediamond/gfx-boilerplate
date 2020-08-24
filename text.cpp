#include "text.hpp"

#include <cmath>

static uint32_t NearestPowerOf2(uint32_t x) {
    int n = 1;
    while (n < x) n <<= 1;
    return n;
}

GlyphAtlas Text_CreateAtlas(FT_Face& face, int size, uint32_t first, uint32_t num, int padding) {
    int pixelsTotal = (size + padding) * num;
    int charsPerRow = (int) ceil(sqrt(num));
    uint32_t width = NearestPowerOf2(charsPerRow * (size + padding));


    GlyphAtlas atlas;
    atlas.atlas = Image_Create(width, width, 1);
    atlas.first = first;
    atlas.num = num;
    atlas.size = size;
    atlas.charsPerRow = charsPerRow;

    Image& img = atlas.atlas;
    uint8_t* imgbuf = (uint8_t*) img.data;

    FT_Set_Pixel_Sizes(face, 0, size);
    for (uint32_t i = 0; i < num; ++i) {
        uint32_t charidx = first + i;
        FT_Load_Glyph(face, FT_Get_Char_Index(face, charidx), FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        const FT_Bitmap& bmp = face->glyph->bitmap;
        uint16_t dstX, dstY;
        Text_GetPos(atlas, i, &dstX, &dstY);
        dstX *= (size + padding);
        dstY *= (size + padding);

        for (int y = 0; y < bmp.rows; ++y) {
            int ypos = y + dstY;
            for (int x = 0; x < bmp.width; ++x) {
                imgbuf[ypos * img.pitch + x + dstX] =
                    bmp.buffer[y * bmp.pitch + x];
            }
        }
    }

    return atlas;
}

void Text_DestroyAtlas(GlyphAtlas& atlas) {
    Image_Free(atlas.atlas);
}

void Text_GetPos(const GlyphAtlas& atlas, uint32_t i, uint16_t* x, uint16_t* y) {
    *x = i % atlas.charsPerRow;
    *y = i / atlas.charsPerRow;
}