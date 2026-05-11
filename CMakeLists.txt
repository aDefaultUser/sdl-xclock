cmake_minimum_required(VERSION 3.16)
project(sdl-xclock LANGUAGES C)

set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(sdl-xclock
    src/main.c
    src/clock.c
)

target_include_directories(sdl-xclock PRIVATE include)
target_link_libraries(sdl-xclock PRIVATE SDL3::SDL3 m)
