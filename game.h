#pragma once
#include <SDL2/SDL.h>
#include "sound.h"
#include "score.h"

#define MAX_BALLS 3
#define MAX_PARTICLES 50

struct Ball {
    float x, y;
    float vx, vy;
    int   size;
    bool  active;
    uint8_t r, g, b;
    /* parıltı efekti */
    float flash_timer;
};

struct Particle {
    float x, y;
    float vx, vy;
    float life;
    float max_life;
    uint8_t r, g, b;
    bool active;
};

class Game {
public:
    Game(SDL_Renderer* renderer);
    void run();

private:
    SDL_Renderer* renderer;
    Sound sound;
    bool running;
    ScoreBoard scoreboard;

    Ball     balls[MAX_BALLS];
    Particle particles[MAX_PARTICLES];

    int   level;
    float speed_multiplier;

    float paddle_x, paddle_y;
    int   paddle_w, paddle_h;

    int score;
    int high_score;

    void handle_events();
    void update(float dt);
    void render();
    void init_ball(int i);
    void reset_balls();
    void spawn_particles(float x, float y, uint8_t r, uint8_t g, uint8_t b);
    void update_particles(float dt);
    void render_particles();
};