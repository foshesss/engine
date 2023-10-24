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

#define SCREEN_CENTER Vector2(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1)

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
    player.x = 0;
    player.y = 0;
    player.angle = 0;
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

    player.x += (key_handler.a - key_handler.d) * (player.walkspeed * sin(player.angle) * dt);
    player.y += (key_handler.d - key_handler.a) * (player.walkspeed * cos(player.angle) * dt);

    player.x += (key_handler.w - key_handler.s) * (player.walkspeed * cos(player.angle) * dt);
    player.y += (key_handler.w - key_handler.s) * (player.walkspeed * sin(player.angle) * dt);
}

void render() {
    // fill background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    float angle = player.angle;
    float px = player.x, py = player.y;

    // point coord
    float vx1 = 50, vy1 = 50;
    float vx2 = 150, vy2 = 150;

    // transform point
    float tx1 = vx1 - px, ty1 = vy1 - py;
    float tx2 = vx2 - px, ty2 = vy2 - py;

    // rotate around point
    float tz1 = tx1 * cos(angle) + ty1 * sin(angle);
    float tz2 = tx2 * cos(angle) + ty2 * sin(angle);
    tx1 = tx1 * sin(angle) - ty1 * cos(angle);
    tx2 = tx2 * sin(angle) - ty2 * cos(angle);

    // plot line
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(renderer,
        SCREEN_CENTER.x - tx1,
        SCREEN_CENTER.y - tz1,
        SCREEN_CENTER.x - tx2,
        SCREEN_CENTER.y - tz2
    );

    // player coordinates
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player_rect = {
        (int)SCREEN_CENTER.x - 8/2,
        (int)SCREEN_CENTER.y - 8/2,
        8,
        8
    };
    SDL_RenderFillRect(renderer, &player_rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 122);
    SDL_RenderDrawLine(renderer, SCREEN_CENTER.x, SCREEN_CENTER.y, SCREEN_CENTER.x, SCREEN_CENTER.y - 25);



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