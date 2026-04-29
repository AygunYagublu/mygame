#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_W 800
#define SCREEN_H 600

/* hər topun öz rəngi */
static const uint8_t BALL_COLORS[MAX_BALLS][3] = {
    {50,  255, 100},  /* yaşıl  */
    {255, 100, 50 },  /* narıncı */
    {100, 150, 255},  /* mavi   */
};

Game::Game(SDL_Renderer* rend)
    : renderer(rend), running(true), score(0), high_score(0) {
    paddle_w = 120;
    paddle_h = 15;
    paddle_x = SCREEN_W / 2 - paddle_w / 2;
    paddle_y = SCREEN_H - 50;
    reset_balls();
}

void Game::init_ball(int i) {
    balls[i].size   = 15;
    balls[i].active = true;
    balls[i].r      = BALL_COLORS[i][0];
    balls[i].g      = BALL_COLORS[i][1];
    balls[i].b      = BALL_COLORS[i][2];

    /* hər top fərqli yerdən başlasın */
    balls[i].x  = SCREEN_W / 4 + i * (SCREEN_W / 4);
    balls[i].y  = SCREEN_H / 3;

    /* fərqli sürət */
    float speeds[MAX_BALLS][2] = {
        { 250.0f, -300.0f},
        {-220.0f, -280.0f},
        { 200.0f, -320.0f},
    };
    balls[i].vx = speeds[i][0];
    balls[i].vy = speeds[i][1];
}

void Game::reset_balls() {
    for (int i = 0; i < MAX_BALLS; i++)
        init_ball(i);
    score = 0;
}

void Game::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
        if (e.type == SDL_KEYDOWN &&
            e.key.keysym.sym == SDLK_ESCAPE)
            running = false;
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    paddle_x = mx - paddle_w / 2;

    if (paddle_x < 0) paddle_x = 0;
    if (paddle_x + paddle_w > SCREEN_W)
        paddle_x = SCREEN_W - paddle_w;
}

void Game::update(float dt) {
    int active_count = 0;

    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) continue;
        active_count++;

        Ball& b = balls[i];

        b.x += b.vx * dt;
        b.y += b.vy * dt;

        /* sol və sağ divar */
        if (b.x <= 0) {
            b.x  = 0;
            b.vx = -b.vx;
            sound.play_bounce();
        }
        if (b.x + b.size >= SCREEN_W) {
            b.x  = SCREEN_W - b.size;
            b.vx = -b.vx;
            sound.play_bounce();
        }

        /* yuxarı divar */
        if (b.y <= 0) {
            b.y  = 0;
            b.vy = -b.vy;
            sound.play_bounce();
        }

        /* paddle collision */
        if (b.y + b.size >= paddle_y &&
            b.y + b.size <= paddle_y + paddle_h &&
            b.x + b.size >= paddle_x &&
            b.x <= paddle_x + paddle_w) {
            b.vy  = -b.vy;
            b.y   = paddle_y - b.size;
            score++;
            sound.play_score();
        }

        /* aşağı düşdü */
        if (b.y > SCREEN_H) {
            b.active = false;
            sound.play_gameover();
        }
    }

    /* bütün toplar düşdü — yenidən başla */
    if (active_count == 0) {
        if (score > high_score)
            high_score = score;
        reset_balls();
    }
}

void Game::render() {
    /* arxa fon */
    SDL_SetRenderDrawColor(renderer, 15, 15, 35, 255);
    SDL_RenderClear(renderer);

    /* divarlar */
    SDL_SetRenderDrawColor(renderer, 80, 80, 120, 255);
    SDL_RenderDrawLine(renderer, 0, 0, SCREEN_W, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, SCREEN_H);
    SDL_RenderDrawLine(renderer, SCREEN_W-1, 0, SCREEN_W-1, SCREEN_H);

    /* toplar */
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) continue;
        SDL_SetRenderDrawColor(renderer,
            balls[i].r, balls[i].g, balls[i].b, 255);
        SDL_Rect r = {
            (int)balls[i].x, (int)balls[i].y,
            balls[i].size,   balls[i].size
        };
        SDL_RenderFillRect(renderer, &r);
    }

    /* paddle */
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    SDL_Rect paddle_rect = {
        (int)paddle_x, (int)paddle_y,
        paddle_w,      paddle_h
    };
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_RenderPresent(renderer);
}

void Game::run() {
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        Uint32 now = SDL_GetTicks();
        float dt   = (now - last_time) / 1000.0f;
        last_time  = now;

        if (dt > 0.05f) dt = 0.05f;

        handle_events();
        update(dt);
        render();
    }
}