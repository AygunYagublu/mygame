#include "game.h"
#include <stdio.h>

#define SCREEN_W 800
#define SCREEN_H 600

Game::Game(SDL_Renderer* rend) : renderer(rend), running(true), score(0) {
    ball_size = 15;
    paddle_w  = 120;
    paddle_h  = 15;
    paddle_x  = SCREEN_W / 2 - paddle_w / 2;
    paddle_y  = SCREEN_H - 50;
    reset_ball();
}

void Game::reset_ball() {
    ball_x  = SCREEN_W / 2;
    ball_y  = SCREEN_H / 2;
    ball_vx = 250.0f;
    ball_vy = -300.0f;
}

void Game::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            running = false;
    }

    /* paddle hərəkəti — mouse ilə */
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    paddle_x = mx - paddle_w / 2;

    /* paddle sərhədləri */
    if (paddle_x < 0) paddle_x = 0;
    if (paddle_x + paddle_w > SCREEN_W)
        paddle_x = SCREEN_W - paddle_w;
}

void Game::update(float dt) {
    ball_x += ball_vx * dt;
    ball_y += ball_vy * dt;

    /* sol və sağ divar */
    if (ball_x <= 0) {
        ball_x  = 0;
        ball_vx = -ball_vx;
    }
    if (ball_x + ball_size >= SCREEN_W) {
        ball_x  = SCREEN_W - ball_size;
        ball_vx = -ball_vx;
    }

    /* yuxarı divar */
    if (ball_y <= 0) {
        ball_y  = 0;
        ball_vy = -ball_vy;
    }

    /* paddle collision */
    if (ball_y + ball_size >= paddle_y &&
        ball_y + ball_size <= paddle_y + paddle_h &&
        ball_x + ball_size >= paddle_x &&
        ball_x <= paddle_x + paddle_w) {
        ball_vy = -ball_vy;
        ball_y  = paddle_y - ball_size;
        score++;
    }

    /* aşağı düşdü — yenidən başla */
    if (ball_y > SCREEN_H) {
        reset_ball();
        score = 0;
    }
}

void Game::render() {
    /* arxa fon — tünd mavi */
    SDL_SetRenderDrawColor(renderer, 15, 15, 35, 255);
    SDL_RenderClear(renderer);

    /* divarlar — parlaq xətt */
    SDL_SetRenderDrawColor(renderer, 80, 80, 120, 255);
    SDL_RenderDrawLine(renderer, 0, 0, SCREEN_W, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, SCREEN_H);
    SDL_RenderDrawLine(renderer, SCREEN_W-1, 0, SCREEN_W-1, SCREEN_H);

    /* top — parlaq yaşıl */
    SDL_SetRenderDrawColor(renderer, 50, 255, 100, 255);
    SDL_Rect ball_rect = {(int)ball_x, (int)ball_y, ball_size, ball_size};
    SDL_RenderFillRect(renderer, &ball_rect);

    /* paddle — parlaq mavi */
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    SDL_Rect paddle_rect = {(int)paddle_x, (int)paddle_y, paddle_w, paddle_h};
    SDL_RenderFillRect(renderer, &paddle_rect);

    /* skor — sol üst künc */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

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