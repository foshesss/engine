#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./constants.h"
#include "./vector.h"
#include "./matrix.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct Camera {
    Vector3 position;
    Vector3 orientation;
    float angle;
    // float pan;
    // float tilt;
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
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
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
    camera.position = Vector3(0, 0, 5);
    camera.angle = 0;
    camera.rotate_speed = PI/3;
    camera.move_speed = 5;
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

    // exit
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

// TODO: Point in view: https://math.stackexchange.com/questions/4144827/

void draw_triangle() {
    // TODO: eventually, this: https://stackoverflow.com/questions/69447778
    // current method is mostly used for debugging

    SDL_Point points[4];
    points[0].x = 0; points[0].y = 0;
    points[1].x = 0; points[1].y = 720;
    points[2].x = 640; points[2].y = 360;
    points[3].x = 0; points[3].y = 0;

    SDL_RenderDrawLines(
        renderer,
        points,
        4
    );
}

void draw_pixel(int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

Vector3 draw_3d_point(Vector3 p) {

    float co = cos(camera.angle);
    float si = sin(camera.angle);

    // offset points by player
    p = Vector3_sub(p, camera.position);
    Vector3 translated;

    // discover world x position
    translated.x = p.x * co - p.y * si;
    
    // discover world y position
    translated.y = p.y * co + p.x * si;

    // discover world z position
    translated.z = p.z;

    // find position on screen
    translated.x = translated.x * 200/translated.y + SCREEN_CENTER.x;
    translated.y = translated.z * 200/translated.y + SCREEN_CENTER.y;

    // do not draw off screen points
    // if (translated.x < 0 || SCREEN_WIDTH < translated.x ||
    //     translated.y < 0 || SCREEN_HEIGHT < translated.y) return Vector3;

    return translated;
}

void draw_wall(Vector3 left, Vector3 right) {
    if (left.x < 1) left.x = 1;
    if (right.x < 1) right.x = 1;
    if (left.x > SCREEN_WIDTH - 1) left.x = SCREEN_WIDTH - 1;
    if (right.x > SCREEN_WIDTH - 1) right.x = SCREEN_WIDTH - 1;

    int dy = right.y - left.y; // y distance
    int dx = right.x - left.x; // x distance
    if (dx == 0) dx = 1;

    for (int x = left.x; x < right.x; x++) {
        int y1 = dy * (x - left.x + .5)/dx + left.y;
        printf("%d\n", y1);
        // for (int y = y1; y < y + 30; y++) {
        //     draw_pixel(x, y);
        // }
    }
}

void render() {
    // clear background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    printf("-----------\n");
    Vector3_print(camera.position);

    float fov = camera.fov * PI/180;
    float fRad = 1/tan(fov/2);

    matrix4x4 projection_matrix;
    projection_matrix.m[0][0] = ASPECT_RATIO * fRad;
    projection_matrix.m[1][1] = fRad;

    Vector3 points[] = {
        {5, -10, 0},
        {50, -10, 0}
    };

    // draw points
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 2; i++) {
        points[i] = draw_3d_point(points[i]);
        Vector3_print(points[i]);
    }

    draw_wall(points[0], points[1]);
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