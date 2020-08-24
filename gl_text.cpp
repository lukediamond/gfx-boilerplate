#include "gl_text.hpp"
#include "gl_shader.hpp"
#include "fileio.hpp"

#include "wrapped_gl.h"

// GL_GlyphAtlas

GL_GlyphAtlas GL_CreateGlyphAtlas(FT_Face face, int size, uint32_t codept) {
    GL_GlyphAtlas atlas;
    atlas.atlas = Text_CreateAtlas(face, size, codept, 0xff, 2);
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
    GL_Glyph glyph;

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
        ctx.atlases.push_back(GL_CreateGlyphAtlas(ctx.face, ctx.size, codept & ~0xff));
        atlasidx = ctx.atlases.size() - 1;
    }

    // populate glyph
    const auto& atlas = ctx.atlases[atlasidx];
    glyph.atlas = atlas.tex;
    Text_GetDrawPos(atlas.atlas, codept, glyph.tl, glyph.br);
    glyph.metrics = atlas.atlas.metrics[codept - atlas.atlas.first];

    return glyph;
}

std::vector<GL_Glyph> GL_GetGlyphString(GL_FontContext& ctx, std::string str) {
    std::vector<GL_Glyph> glyphs;
    for (auto c : str)
        glyphs.push_back(GL_GetGlyph(ctx, c));
    return glyphs;
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
        ReadEntireFile("../shaders/quad.vert").c_str(),
        ReadEntireFile("../shaders/quad.frag").c_str());
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

void GL_DrawString(GL_TextRenderer& r, std::vector<GL_Glyph>& glyphs, glm::vec2 pos, float size) {
    glUseProgram(r.atlasprog);
    glBindVertexArray(r.quad.vao);

    GL_PassUniform(r.atlasprog_res, r.res);
    GL_PassUniform(r.atlasprog_image, 0);

    for (const auto& g : glyphs) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g.atlas);
        GL_PassUniform(r.atlasprog_tl, g.tl);
        GL_PassUniform(r.atlasprog_br, g.br);
        GL_PassUniform(r.atlasprog_pos, pos + glm::vec2 {g.metrics.bearingX, size - g.metrics.bearingY});
        GL_PassUniform(r.atlasprog_size, glm::vec2 {size});
        r.quad.Draw();

        pos.x += g.metrics.advanceX;
    }
}