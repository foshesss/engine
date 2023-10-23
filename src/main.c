#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h"
#include "./vector_math.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct player {
    float x;
    float y;
    float angle;
    int walkspeed;
    float rotate_speed;
} player;

struct key_handler {
    int w, a, s, d;
    int left, right;
} key_handler;

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define clamp(num, low, high) (min(max(num, low), high))

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("ERROR : SDL Initialization > %s\n", SDL_GetError());
        return FALSE;
    }

    window = SDL_CreateWindow(
        NULL, // title
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        SDL_Log("ERROR : SDL Window creation > %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        fprintf(stderr, "Error creating renderer.\n");
        return FALSE;
    }

    return TRUE;
}

void setup() {
    // spawn player
    player.x = 50;
    player.y = 50;
    player.angle = PI/2;
    player.rotate_speed = PI/4;
    player.walkspeed = 300;
}

void process_input() {
    SDL_PumpEvents();
    const Uint8* keyboard_states = SDL_GetKeyboardState(NULL);

    if (keyboard_states[SDL_SCANCODE_ESCAPE] == TRUE) game_is_running = FALSE;

    key_handler.w = keyboard_states[SDL_SCANCODE_W];
    key_handler.a = keyboard_states[SDL_SCANCODE_A];
    key_handler.s = keyboard_states[SDL_SCANCODE_S];
    key_handler.d = keyboard_states[SDL_SCANCODE_D];
    key_handler.left = keyboard_states[SDL_SCANCODE_Q];
    key_handler.right = keyboard_states[SDL_SCANCODE_E];

    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT) game_is_running = FALSE;
}

void update() {
    int current_time = SDL_GetTicks();
    int time_to_wait = FRAME_TARGET_TIME - (current_time - last_frame_time);
    if (time_to_wait > 0) SDL_Delay(time_to_wait);

    // update current time
    current_time = SDL_GetTicks();
    float dt = (current_time - last_frame_time)/1000.0f;
    last_frame_time = current_time;

    player.angle += (key_handler.left - key_handler.right) * player.rotate_speed * dt;

    player.x += (key_handler.d - key_handler.a) * (player.walkspeed * cos(player.angle) * dt);
    player.y += (key_handler.d - key_handler.a) * (player.walkspeed * sin(player.angle) * dt);


    // player.x += (key_handler.s - key_handler.w) * (player.walkspeed * sin(player.angle) * dt);
    // player.y += (key_handler.s - key_handler.w) * (player.walkspeed * cos(player.angle) * dt);
}

void render() {
    // fill background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    float angle = player.angle;
    Vector2 player_pos = Vector2(player.x, player.y);

    print(player_pos);

    // wall coords
    Vector2 wall0 = rotate_vector(Vector2(70, 20), player_pos, angle);
    Vector2 wall1 = rotate_vector(Vector2(70, 70), player_pos, angle);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(renderer, wall0.x, wall0.y, wall1.x, wall1.y);

    // player coordinates
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player_rect = {
        (int)player.x,
        (int)player.y,
        (int)3,
        (int)3
    };
    SDL_RenderFillRect(renderer, &player_rect);
    SDL_RenderPresent(renderer);
}

void clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    game_is_running = initialize_window();
    
    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    clean();

    return 0;
}