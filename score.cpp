#include "score.h"
#include <stdio.h>
#include <string.h>

ScoreBoard::ScoreBoard() : initialized(false) {
    if (TTF_Init() < 0) {
        SDL_Log("TTF xetasi: %s", TTF_GetError());
        return;
    }

    /* sistem fontunu tap */
    const char* font_paths[] = {
        "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
        nullptr
    };

    font_large = nullptr;
    font_small = nullptr;

    for (int i = 0; font_paths[i] != nullptr; i++) {
        font_large = TTF_OpenFont(font_paths[i], 32);
        font_small = TTF_OpenFont(font_paths[i], 18);
        if (font_large && font_small) break;
    }

    if (!font_large || !font_small) {
        SDL_Log("Font tapilmadi: %s", TTF_GetError());
        return;
    }

    initialized = true;
}

ScoreBoard::~ScoreBoard() {
    if (font_large) TTF_CloseFont(font_large);
    if (font_small) TTF_CloseFont(font_small);
    TTF_Quit();
}

void ScoreBoard::draw_text(SDL_Renderer* renderer, TTF_Font* font,
                           const char* text, int x, int y,
                           uint8_t r, uint8_t g, uint8_t b) {
    if (!initialized || !font) return;

    SDL_Color color = {r, g, b, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect dst = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

void ScoreBoard::render(SDL_Renderer* renderer, int score, int high_score, int level) {    if (!initialized) return;

    char buf[64];

    /* cari skor — sol üst */
    snprintf(buf, sizeof(buf), "SKOR: %d", score);
    draw_text(renderer, font_large, buf, 20, 15, 255, 255, 100);

    /* ən yüksək skor — sağ üst */
    snprintf(buf, sizeof(buf), "EN COX: %d", high_score);
    /* səviyyə — mərkəz üst */
    snprintf(buf, sizeof(buf), "SΕVIYYΕ: %d", level);
    draw_text(renderer, font_small, buf, 350, 20, 100, 255, 150);
    int x = 800 - 200;
    draw_text(renderer, font_small, buf, x, 20, 180, 180, 255);

    /* oyun qaydası — aşağı mərkəz */
    draw_text(renderer, font_small,
        "Mouse ile paddle-i idar et | ESC - cix",
        160, 575, 100, 100, 150);
}