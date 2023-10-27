#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "constants.h"

// Private variables
int engine_running = FALSE;

SDL_Window* window;
SDL_GLContext renderer;

// Private functions

void get_opengl_version() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading lang: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int initialize_window() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERROR : SDL Initialization > %s\n", SDL_GetError());
        return FALSE;
    }

    // SDL GL 4.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    // cannot use deprecated functions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        SDL_Log("ERROR : SDL window creation failed > %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_GL_CreateContext(window);
    if (renderer == NULL) {
        SDL_Log("ERROR : GL context creation failed > %s\n", SDL_GetError());
        return FALSE;
    }

    // init glad library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        SDL_Log("ERROR : Glad was not initialized. > %s\n", SDL_GetError());
        return FALSE;
    }

    // get_opengl_version();

    return TRUE;
}

void process_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e) == TRUE) {
        switch (e.type) {
            case SDL_QUIT:
                engine_running = FALSE;
                break;
        }
    }
}

void draw() {
    SDL_GL_SwapWindow(window);
}

void clean() {
    SDL_GL_DeleteContext(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main() {
    engine_running = initialize_window();

    while (engine_running) {
        // determine delta time per frame
        process_input();
        draw();
    }

    clean();

    return 0;
}