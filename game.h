#pragma once
#include <SDL2/SDL.h>
#include "sound.h"

class Game {
public:
    Game(SDL_Renderer* renderer);
    void run();

private:
    SDL_Renderer* renderer;
    Sound sound;
    bool running;

    /* top */
    float ball_x, ball_y;
    float ball_vx, ball_vy;
    int   ball_size;

    /* paddle */
    float paddle_x, paddle_y;
    int   paddle_w, paddle_h;

    /* skor */
    int score;
    int high_score;

    void handle_events();
    void update(float dt);
    void render();
    void reset_ball();
};