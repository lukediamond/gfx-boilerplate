#include <SDL2/SDL.h>
#include <GL/glew.h>

struct ProgramState {
    bool running = true;
};

void HandleSDLEvent(ProgramState& ctx, SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT:
            ctx.running = false;
            break;
    }
}

int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_Window* window = 
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                         1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glctx);
    glewInit();
    SDL_GL_SetSwapInterval(1);

    ProgramState state;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (state.running) {
        for (SDL_Event e; SDL_PollEvent(&e); HandleSDLEvent(state, e));

        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
