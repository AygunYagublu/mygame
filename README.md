# MyGame

A 2D arcade game and game engine written from scratch in C++ using SDL2.

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Library](https://img.shields.io/badge/library-SDL2-red)
![License](https://img.shields.io/badge/license-MIT-green)

## Gameplay
- Control the paddle with your mouse
- Keep all 3 balls from falling off the screen
- Each paddle hit increases your score
- Every 10 points the balls get faster
- You have 3 lives — lose them all and it's game over!

## Features
- SDL2 rendering engine
- Real-time physics with delta time
- 3 colored balls with individual tracking
- Particle system — sparks on every collision
- Flash effect on paddle hit
- Moving star background
- Difficulty system — speed increases every 10 score
- Lives system with game over screen
- Scoreboard with current score, high score, level and lives
- Procedural sound effects (bounce, score, game over)
- AABB collision detection

## Build & Run

### Requirements
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev cmake build-essential fonts-liberation
```

### Run
```bash
git clone https://github.com/AygunYagublu/mygame.git
cd mygame
mkdir build && cd build
cmake ..
make
./MyGame
```

## Controls
| Input            | Action            |
|------------------|-------------------|
| Mouse left/right | Move paddle       |
| R                | Restart (game over)|
| ESC              | Quit              |

## Project Structure
## What I Learned
- SDL2 window, renderer and audio management
- Game loop architecture with delta time
- 2D collision detection (AABB)
- Particle system design
- Object-oriented game engine in C++17
- Procedural audio generation
- CMake build system

## Tech Stack
- C++17
- SDL2 + SDL2_ttf
- CMake
