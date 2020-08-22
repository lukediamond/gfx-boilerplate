#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/time.h>

#include <SDL2/SDL.h>
#include "wrapped_gl.h"

#include "image.hpp"
#include "shader.hpp"
#include "prim.hpp"

static double GetTime() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double) (ts.tv_sec + ts.tv_nsec * 1.0e-9);
}

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

inline std::string ReadEntireFile(const char* path) {
    std::ifstream stream(path);
    if (!stream.is_open()) return "";
    stream.seekg(0, std::ios::end);
    size_t sz = stream.tellg();
    stream.seekg(0);
    std::string data(sz, 0);
    stream.read(&data[0], sz);
    stream.close();
    return data;
}

int main(int, char**) {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_Window* window = 
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                         1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glctx);
    glewExperimental = GL_TRUE;
    glewInit();
    SDL_GL_SetSwapInterval(1);

    ProgramState state;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    while (state.running) {
        double start = GetTime();
        for (SDL_Event e; SDL_PollEvent(&e); HandleSDLEvent(state, e));

        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);

        double end = GetTime();
        state.delta = end - start;
        state.elapsed += state.delta;
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
