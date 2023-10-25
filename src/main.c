#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h"
#include "./vector.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct Camera {
    Vector3 position;
    float angle;
    int move_speed;
    int fov;
    float rotate_speed;
} camera;

struct key_handler {
    int w, a, s, d;
    int left, right; // rotating left/right
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


    // setup camera
    camera.position = Vector3(70,-110,20);
    camera.angle = 0;
    camera.rotate_speed = PI/3;
    camera.move_speed = 300;
    camera.fov = 200;
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

    // update camera position
    camera.angle += (key_handler.right - key_handler.left) * camera.rotate_speed * dt;
    float x_dir = 
        (key_handler.a - key_handler.d) * sin(camera.angle) +
        (key_handler.w - key_handler.s) * cos(camera.angle);

    float y_dir = 
        (key_handler.d - key_handler.a) * cos(camera.angle) +
        (key_handler.w - key_handler.s) * sin(camera.angle);

    // convert to unit vector
    Vector2 direction_vector = Vector2(x_dir, y_dir);
    direction_vector = Vector2_unit(direction_vector);

    // check for nan
    if (direction_vector.x != direction_vector.x) return;

    camera.position.x += direction_vector.x * camera.move_speed * dt;
    camera.position.y += direction_vector.y * camera.move_speed * dt;
}

void draw_wall(Vector3 p1, Vector3 p2, int height) {
    float angle = camera.angle;
    int fov = camera.fov;

    // translate wall points
    Vector3 points[2] = {p1, p2};
    float co = cos(angle), si = sin(angle);
    for (int i = 0; i < 2; i++) {
        Vector3 curr = points[i];
        Vector3 w;

        // translate to player
        curr = Vector3_sub(curr, camera.position);
        w.x = curr.x * co - curr.y * si;
        w.y = curr.x * si + curr.y * co;
        w.z = curr.z;

        // modify based on screen position
        w.x = w.x * fov/w.y + SCREEN_CENTER.x;
        w.y = w.z * fov/w.y + SCREEN_CENTER.y;

        points[i] = w;

        Vector3_print(w);

        int size = 4;
        SDL_Rect pixel = {
            w.x - size/2,
            w.y - size/2,
            size,
            size
        };
        SDL_RenderFillRect(renderer, &pixel);
    }

    // p1 = points[0]; p2 = points[1];

    // int dyb = p2.y - p1.y;
    // int dx = p2.x - p1.x; if (dx == 0) dx = 1;
    // int xs = p1.x;


    // for (int x = p1.x; x < p2.x; x++) {
    //     printf("%d\n", x);
    //     int y1 = dyb * (x - xs + .5)/dx + p1.y;
    //     SDL_Rect pixel = {
    //         x - 10/2,
    //         y1 - 10/2,
    //         10,
    //         10
    //     };
    //     SDL_RenderFillRect(renderer, &pixel);
    // }
}

// TODO: Point in view: https://math.stackexchange.com/questions/4144827/


void render() {
    // clear background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Vector3 p0 = {70, }


    // draw a 3D point
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int x = 0; x < 400; x++)
        for (int y = 0; y < 100; y++)
            SDL_RenderDrawPoint(renderer, x, 50 + y);

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