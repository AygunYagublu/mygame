#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class ScoreBoard {
public:
    ScoreBoard();
    ~ScoreBoard();

    void render(SDL_Renderer* renderer, int score, int high_score, int level);

private:
    TTF_Font* font_large;
    TTF_Font* font_small;
    bool initialized;

    void draw_text(SDL_Renderer* renderer, TTF_Font* font,
                   const char* text, int x, int y,
                   uint8_t r, uint8_t g, uint8_t b);
};