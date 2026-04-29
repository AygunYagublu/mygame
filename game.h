#pragma once
#include <SDL2/SDL.h>
#include "sound.h"

#define MAX_BALLS 3

struct Ball {
    float x, y;
    float vx, vy;
    int   size;
    bool  active;
    /* rəng */
    uint8_t r, g, b;
};

class Game {
public:
    Game(SDL_Renderer* renderer);
    void run();

private:
    SDL_Renderer* renderer;
    Sound sound;
    bool running;

    /* toplar */
    Ball balls[MAX_BALLS];

    /* paddle */
    float paddle_x, paddle_y;
    int   paddle_w, paddle_h;

    /* skor */
    int score;
    int high_score;

    void handle_events();
    void update(float dt);
    void render();
    void init_ball(int i);
    void reset_balls();
};