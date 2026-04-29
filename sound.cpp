#include "sound.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE 44100
#define PI 3.14159265358979323846

Sound::Sound() : initialized(false) {
    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq     = SAMPLE_RATE;
    want.format   = AUDIO_U8;
    want.channels = 1;
    want.samples  = 512;
    want.callback = nullptr;

    device = SDL_OpenAudioDevice(nullptr, 0, &want, &spec, 0);
    if (device == 0) {
        SDL_Log("Audio xetasi: %s", SDL_GetError());
        return;
    }

    SDL_PauseAudioDevice(device, 0);
    initialized = true;
}

Sound::~Sound() {
    if (initialized)
        SDL_CloseAudioDevice(device);
}

void Sound::generate_tone(int frequency, int duration_ms, uint8_t volume) {
    if (!initialized) return;

    int num_samples = SAMPLE_RATE * duration_ms / 1000;
    uint8_t* buffer = (uint8_t*)malloc(num_samples);
    if (!buffer) return;

    for (int i = 0; i < num_samples; i++) {
        /* sine dalğası */
        double t    = (double)i / SAMPLE_RATE;
        double wave = sin(2.0 * PI * frequency * t);

        /* envelope — səsin tədricən azalması */
        double env = 1.0 - (double)i / num_samples;

        buffer[i] = (uint8_t)(128 + wave * env * volume);
    }

    SDL_QueueAudio(device, buffer, num_samples);
    free(buffer);
}

void Sound::play_bounce() {
    /* divar zərbəsi — qısa, yüksək */
    generate_tone(440, 60, 100);
}

void Sound::play_score() {
    /* skor — iki not */
    generate_tone(523, 80, 120);
    generate_tone(659, 120, 120);
}

void Sound::play_gameover() {
    /* oyun bitti — aşağı düşən ton */
    generate_tone(400, 100, 150);
    generate_tone(300, 100, 150);
    generate_tone(200, 200, 150);
}