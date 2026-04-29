#pragma once
#include <SDL2/SDL.h>
#include <stdint.h>

class Sound {
public:
    Sound();
    ~Sound();

    void beep(int frequency, int duration_ms);
    void play_bounce();
    void play_score();
    void play_gameover();

private:
    SDL_AudioDeviceID device;
    SDL_AudioSpec spec;
    bool initialized;

    void generate_tone(int frequency, int duration_ms,
                       uint8_t volume = 128);
};