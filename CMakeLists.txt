cmake_minimum_required(VERSION 3.10)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(MyGame
    main.cpp
    game.cpp
    sound.cpp
    score.cpp
)

target_include_directories(MyGame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(MyGame ${SDL2_LIBRARIES} SDL2_ttf m)