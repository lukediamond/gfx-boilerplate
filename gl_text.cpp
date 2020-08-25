#include "gl_text.hpp"
#include "gl_shader.hpp"
#include "fileio.hpp"
#include "utf8_decode.hpp"

#include "wrapped_gl.h"

// GL_GlyphAtlas

GL_GlyphAtlas GL_CreateGlyphAtlas(FT_Face face, int size, uint32_t codept) {
    GL_GlyphAtlas atlas;
    atlas.atlas = Text_CreateAtlas(face, size, codept, 0xff, 4);
    atlas.tex = GL_CreateTexture(atlas.atlas.atlas);
    GL_TextureFilter(atlas.tex, GL_NEAREST, GL_NEAREST);

    return atlas;
}

void GL_DestroyGlyphAtlas(GL_GlyphAtlas& atlas) {
    Text_DestroyAtlas(atlas.atlas);
    glDeleteTextures(1, &atlas.tex);
}

// GL_FontContext

GL_Glyph GL_GetGlyph(GL_FontContext& ctx, uint32_t codept) {
    // find atlas
    int atlasidx = -1;
    for (int i = 0; i < ctx.atlases.size(); ++i) {
        const auto& atlas = ctx.atlases[i].atlas;
        if (atlas.first <= codept && codept < atlas.first + atlas.num) {
            atlasidx = i;
            break;
        }
    }
    // create if absent
    if (atlasidx < 0) {
        printf("adding code point 0x%x\n", codept & ~0xff);
        ctx.atlases.push_back(GL_CreateGlyphAtlas(ctx.face, ctx.size, codept & ~(uint32_t) 0xff));
        atlasidx = ctx.atlases.size() - 1;
    }

    // populate glyph
    GL_Glyph glyph;
    const auto& atlas = ctx.atlases[atlasidx];
    uint32_t codeidx = codept - atlas.atlas.first;
    glyph.atlas = atlas.tex;
    glyph.pos = glm::vec2 {0.0f};
    Text_GetDrawPos(atlas.atlas, codeidx, glyph.tl, glyph.br);
    glyph.metrics = atlas.atlas.metrics[codeidx];

    return glyph;
}

GL_TextLayout GL_GetGlyphString(
    GL_FontContext& ctx, std::string str,
    const GL_TextLayoutInfo& info
) {
    GL_TextLayout layout;
    std::vector<int> lines = {0};
    glm::vec2 pos = {0.0f, 0.0f};

    layout.viewport = {info.width, info.height};

    auto str32 = UTF8_ToU32(str.data(), str.size());
    size_t wordstart = 0;
    bool lastws = false;

    int linecount = 0;
    for (size_t i = 0; i < str32.size(); ++i) {
        auto codept = str32[i];

        GL_Glyph g = GL_GetGlyph(ctx, codept);

        // check for newline or overflow
        bool overflow = (info.width > 0.0f && pos.x > info.width);
        if (codept == '\n' || overflow) {
            // if the current word spans the entire line, don't break unless breakWord is set
            if (info.breakWord || i - wordstart < linecount) {
                if (!info.breakWord && overflow) {
                    layout.glyphs.resize(wordstart);
                    i = wordstart;
                }
                pos.x = 0.0f;
                pos.y += g.metrics.advanceY;
                linecount = 0;

                if (lines.back() != layout.glyphs.size()) 
                    lines.push_back(layout.glyphs.size());
                continue;
            }
        }
        if (codept == '\r') continue;
        g.pos = pos;
        pos.x += g.metrics.advanceX;
        ++linecount;
        layout.glyphs.push_back(g);

        if (codept == ' ' || codept == '\n' || codept == '\t') {
            lastws = true;
        } else {
            if (lastws) wordstart = i - 1;
            lastws = false;
        }
    }
    if (lines.back() != layout.glyphs.size()) 
        lines.push_back(layout.glyphs.size());

    float fac = 0.0f;
    switch (info.align) {
        case GL_TextLayoutInfo::A_Center:
            fac = 0.5f;
            break;
        case GL_TextLayoutInfo::A_Right:
            fac = 1.0f;
            break;
        default:
            fac = 0.0f;
            break;
    }
    for (int i = 1; i < lines.size(); ++i) {
        float right = layout.glyphs[lines[i] - 1].pos.x;
        for (int j = lines[i - 1]; j < lines[i]; ++j) {
            layout.glyphs[j].pos.x += fac * fmax(0.0f, info.width - right);
        }
    }

    return layout;
}

GL_FontContext GL_CreateFontContext(FT_Face face, int size) {
    GL_FontContext ctx;
    ctx.face = face;
    ctx.size = size;
    ctx.atlases.push_back(GL_CreateGlyphAtlas(face, size, 0));

    return ctx;
}

void GL_DestroyFontContext(GL_FontContext& ctx) {
    for (auto& atlas : ctx.atlases) {
        GL_DestroyGlyphAtlas(atlas);
    }
    ctx.atlases.clear();
}

// GL_TextRenderer

GL_TextRenderer GL_CreateTextRenderer() {
    GL_TextRenderer r;

    r.atlasprog = GL_CreateProgram(
        ReadEntireFile("../shaders/text.vert").c_str(),
        ReadEntireFile("../shaders/text.frag").c_str());
    r.atlasprog_image = glGetUniformLocation(r.atlasprog, "u_image");
    r.atlasprog_tl = glGetUniformLocation(r.atlasprog, "u_tl");
    r.atlasprog_br = glGetUniformLocation(r.atlasprog, "u_br");
    r.atlasprog_pos = glGetUniformLocation(r.atlasprog, "u_pos");
    r.atlasprog_size = glGetUniformLocation(r.atlasprog, "u_size");
    r.atlasprog_res = glGetUniformLocation(r.atlasprog, "u_res");

    r.quad = GL_CreateQuad();
    return r;
}

void GL_DestroyTextRenderer(GL_TextRenderer& r) {
    glDeleteProgram(r.atlasprog);
    GL_DestroyPrimative(r.quad);
}

void GL_DrawString(GL_TextRenderer& r, const GL_TextLayout& layout, glm::vec2 pos, glm::vec2 offset, float size) {
    glUseProgram(r.atlasprog);
    glBindVertexArray(r.quad.vao);

    GL_PassUniform(r.atlasprog_res, r.res);
    GL_PassUniform(r.atlasprog_image, 0);
    int drawn = 0;
    for (const auto& g : layout.glyphs) {
        glm::vec2 bearing {g.metrics.bearingX, size - g.metrics.bearingY};
        if (g.pos.y + offset.y + g.metrics.advanceY < 0.0f) continue;
        if (layout.viewport.y > 0.0f && g.pos.y + offset.y > layout.viewport.y) {
            break;
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g.atlas);
        GL_PassUniform(r.atlasprog_tl, g.tl);
        GL_PassUniform(r.atlasprog_br, g.br);
        GL_PassUniform(r.atlasprog_pos, pos + offset + g.pos + bearing);
        GL_PassUniform(r.atlasprog_size, glm::vec2 {size});
        r.quad.Draw();
        ++drawn;
    }
}