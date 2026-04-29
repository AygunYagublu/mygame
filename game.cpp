#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_W 800
#define SCREEN_H 600

static const uint8_t BALL_COLORS[MAX_BALLS][3] = {
    {50,  255, 100},
    {255, 100, 50 },
    {100, 150, 255},
};

Game::Game(SDL_Renderer* rend)
    : renderer(rend), running(true), score(0), high_score(0) {
    paddle_w = 120;
    paddle_h = 15;
    paddle_x = SCREEN_W / 2 - paddle_w / 2;
    paddle_y = SCREEN_H - 50;

    for (int i = 0; i < MAX_PARTICLES; i++)
        particles[i].active = false;

    reset_balls();
}

void Game::init_ball(int i) {
    balls[i].size        = 15;
    balls[i].active      = true;
    balls[i].flash_timer = 0.0f;
    balls[i].r           = BALL_COLORS[i][0];
    balls[i].g           = BALL_COLORS[i][1];
    balls[i].b           = BALL_COLORS[i][2];
    balls[i].x           = SCREEN_W / 4 + i * (SCREEN_W / 4);
    balls[i].y           = SCREEN_H / 3;

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

void Game::spawn_particles(float x, float y, uint8_t r, uint8_t g, uint8_t b) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES && spawned < 12; i++) {
        if (particles[i].active) continue;

        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
        float speed = 80.0f + (rand() % 150);

        particles[i].active   = true;
        particles[i].x        = x;
        particles[i].y        = y;
        particles[i].vx       = cosf(angle) * speed;
        particles[i].vy       = sinf(angle) * speed;
        particles[i].life     = 0.4f + (rand() % 30) / 100.0f;
        particles[i].max_life = particles[i].life;
        particles[i].r        = r;
        particles[i].g        = g;
        particles[i].b        = b;
        spawned++;
    }
}

void Game::update_particles(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;
        particles[i].x    += particles[i].vx * dt;
        particles[i].y    += particles[i].vy * dt;
        particles[i].life -= dt;
        if (particles[i].life <= 0)
            particles[i].active = false;
    }
}

void Game::render_particles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].active) continue;

        float alpha = particles[i].life / particles[i].max_life;
        uint8_t a   = (uint8_t)(alpha * 255);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,
            particles[i].r, particles[i].g, particles[i].b, a);

        int size = (int)(4 * alpha) + 1;
        SDL_Rect r = {
            (int)particles[i].x - size/2,
            (int)particles[i].y - size/2,
            size, size
        };
        SDL_RenderFillRect(renderer, &r);
    }
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
    update_particles(dt);
    int active_count = 0;

    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) continue;
        active_count++;

        Ball& b = balls[i];
        if (b.flash_timer > 0) b.flash_timer -= dt;

        b.x += b.vx * dt;
        b.y += b.vy * dt;

        if (b.x <= 0) {
            b.x  = 0;
            b.vx = -b.vx;
            sound.play_bounce();
            spawn_particles(b.x, b.y + b.size/2, b.r, b.g, b.b);
        }
        if (b.x + b.size >= SCREEN_W) {
            b.x  = SCREEN_W - b.size;
            b.vx = -b.vx;
            sound.play_bounce();
            spawn_particles(b.x + b.size, b.y + b.size/2, b.r, b.g, b.b);
        }
        if (b.y <= 0) {
            b.y  = 0;
            b.vy = -b.vy;
            sound.play_bounce();
            spawn_particles(b.x + b.size/2, b.y, b.r, b.g, b.b);
        }

        /* paddle collision */
        if (b.y + b.size >= paddle_y &&
            b.y + b.size <= paddle_y + paddle_h &&
            b.x + b.size >= paddle_x &&
            b.x <= paddle_x + paddle_w) {
            b.vy          = -b.vy;
            b.y           = paddle_y - b.size;
            b.flash_timer = 0.15f;
            score++;
            sound.play_score();
            spawn_particles(
                b.x + b.size/2,
                b.y + b.size,
                b.r, b.g, b.b
            );
        }

        if (b.y > SCREEN_H) {
            b.active = false;
            sound.play_gameover();
        }
    }

    if (active_count == 0) {
        if (score > high_score)
            high_score = score;
        reset_balls();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 15, 15, 35, 255);
    SDL_RenderClear(renderer);

    /* divarlar */
    SDL_SetRenderDrawColor(renderer, 80, 80, 120, 255);
    SDL_RenderDrawLine(renderer, 0, 0, SCREEN_W, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, SCREEN_H);
    SDL_RenderDrawLine(renderer, SCREEN_W-1, 0, SCREEN_W-1, SCREEN_H);

    render_particles();

    /* toplar */
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) continue;

        Ball& b = balls[i];

        /* flash efekti — ağ parıltı */
        if (b.flash_timer > 0) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, b.r, b.g, b.b, 255);
        }

        SDL_Rect r = {(int)b.x, (int)b.y, b.size, b.size};
        SDL_RenderFillRect(renderer, &r);
    }

    /* paddle */
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    SDL_Rect paddle_rect = {
        (int)paddle_x, (int)paddle_y,
        paddle_w,      paddle_h
    };
    SDL_RenderFillRect(renderer, &paddle_rect);

    /* skor ekranı */
    scoreboard.render(renderer, score, high_score);

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