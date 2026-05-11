cmake_minimum_required(VERSION 3.16)
project(sdl-xclock C)

find_package(SDL3 REQUIRED)

add_executable(sdl-xclock src/main.c src/clock.c)

target_link_libraries(sdl-xclock PRIVATE 
    SDL3::SDL3
    m
)

target_include_directories(sdl-xclock PRIVATE include)
