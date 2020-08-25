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

#include "utf8_decode.hpp"

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

    GLuint prog = GL_CreateProgram(
        ReadEntireFile("../shaders/quad.vert").c_str(),
        ReadEntireFile("../shaders/circle.frag").c_str());
    GLuint prog_pos = glGetUniformLocation(prog, "u_pos");
    GLuint prog_size = glGetUniformLocation(prog, "u_size");
    GLuint prog_res = glGetUniformLocation(prog, "u_res");

    Primative quad = GL_CreateQuad();

    while (state.running) {
        auto start = std::chrono::high_resolution_clock::now();
        for (SDL_Event e; SDL_PollEvent(&e); HandleSDLEvent(state, e));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(quad.vao);

        GL_PassUniform(prog_pos, glm::vec2 {0.0f, 0.0f});
        GL_PassUniform(prog_size, glm::vec2 {256.0f, 256.0f});
        GL_PassUniform(prog_res, glm::vec2 {1280.0f, 720.0f});
        quad.Draw();

        SDL_GL_SwapWindow(window);

        auto end = std::chrono::high_resolution_clock::now();
        {
            std::chrono::duration<double> dur = end - start;
            state.delta = dur.count();
            state.elapsed += state.delta;
        }
    }

    GL_DestroyPrimative(quad);
    glDeleteProgram(prog);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
