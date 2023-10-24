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
    player.angle = 0;
    player.rotate_speed = PI/3;
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

    // update player position
    player.angle += (key_handler.right - key_handler.left) * player.rotate_speed * dt;
    float x_dir = 
        (key_handler.a - key_handler.d) * sin(player.angle) +
        (key_handler.w - key_handler.s) * cos(player.angle);

    float y_dir = 
        (key_handler.d - key_handler.a) * cos(player.angle) +
        (key_handler.w - key_handler.s) * sin(player.angle);

    // convert to unit vector
    Vector2 direction_vector = Vector2(x_dir, y_dir);
    Vector2_Unit(&direction_vector);

    // check for nan
    if (direction_vector.x != direction_vector.x) return;

    player.x += direction_vector.x * player.walkspeed * dt;
    player.y += direction_vector.y * player.walkspeed * dt;
}

void render() {
    // fill background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    Vector2 player_pivot = Vector2(player.x, player.y);
    Vector2 vectors[] = {
        Vector2(-250, -250),
        Vector2(-250, 250),
        Vector2(250, 250),
        Vector2(250, -250),
        Vector2(-250, -250)
    };

    int len = sizeof(vectors)/sizeof(vectors[0]);
    SDL_Point *points = (SDL_Point*)malloc(len * sizeof(SDL_Point));
    for (int i = 0; i < len; i++) {

        // tranform point about player
        Vector2 p = vectors[i];
        p = Vector2_rotate(p, player_pivot, player.angle);
        p = Vector2_sub(p, player_pivot);
        p = Vector2_sub(SCREEN_CENTER, p);

        // insert point into points array
        SDL_Point point;
        point.x = p.x;
        point.y = p.y;
        points[i] = point;
    }

    // plot line
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLines(renderer, points, len);
    free(points);

    // draw the player
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

    // switch to new render
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