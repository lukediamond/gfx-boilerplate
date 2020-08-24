#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>

#include <SDL2/SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "wrapped_gl.h"

#include "image.hpp"
#include "text.hpp"
#include "fileio.hpp"
#include "gl_shader.hpp"
#include "gl_prim.hpp"
#include "gl_texture.hpp"
#include "gl_text.hpp"

struct ProgramState {
    bool running = true;
    double delta = 0.0;
    double elapsed = 0.0;
};

static void HandleSDLEvent(ProgramState& ctx, SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            ctx.running = false;
            break;
    }
}

int main(int, char**) {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_Window* window = SDL_CreateWindow(
        "", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glctx);
    glewInit();
    SDL_GL_SetSwapInterval(1);

    ProgramState state;

    FT_Library freetype;
    FT_Init_FreeType(&freetype);

    FT_Face face;
    FT_New_Face(freetype, "../contrib/OpenSans/OpenSans-Regular.ttf", 0, &face);

    GL_FontContext fontctx = GL_CreateFontContext(face, 32);

    std::string str = "hello world\nnewline";
    auto gstr = GL_GetGlyphString(fontctx, str);

    GL_TextRenderer tr = GL_CreateTextRenderer();
    tr.res = {1280.0f, 720.0f};
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (state.running) {
        auto start = std::chrono::high_resolution_clock::now();
        for (SDL_Event e; SDL_PollEvent(&e); HandleSDLEvent(state, e));

        glClear(GL_COLOR_BUFFER_BIT);

        GL_DrawString(tr, gstr, {0.0f, 0.0f}, 32.0f);

        SDL_GL_SwapWindow(window);

        auto end = std::chrono::high_resolution_clock::now();
        {
            std::chrono::duration<double> dur = end - start;
            state.delta = dur.count();
            state.elapsed += state.delta;
        }
    }

    GL_DestroyTextRenderer(tr);
    GL_DestroyFontContext(fontctx);
    FT_Done_Face(face);
    FT_Done_FreeType(freetype);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
