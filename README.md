# MyGame

A simple 2D game engine written from scratch in C++ using SDL2.

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue)
![Library](https://img.shields.io/badge/library-SDL2-red)
![License](https://img.shields.io/badge/license-MIT-green)

## Gameplay
- Control the paddle with your mouse
- Keep the ball from falling off the screen
- Each time the ball hits the paddle, score increases
- If the ball falls, score resets

## Features
- SDL2 rendering engine
- Real-time physics (velocity, collision detection)
- Mouse-controlled paddle
- Score tracking
- Fixed timestep game loop (delta time)
- AABB collision detection

## Build & Run

### Requirements
```bash
sudo apt install libsdl2-dev cmake build-essential
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
| Input        | Action          |
|--------------|-----------------|
| Mouse left/right | Move paddle |
| ESC          | Quit game       |

## Project Structure
## What I Learned
- SDL2 window and renderer management
- Game loop with delta time
- 2D collision detection (AABB)
- Object-oriented game architecture in C++
- CMake build system

## Tech Stack
- C++17
- SDL2
- CMake
