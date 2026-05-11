#include <SDL3/SDL.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "clock.h"
#include "config.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    int win_w = WINDOW_WIDTH;
    int win_h = WINDOW_HEIGHT;

    SDL_Window *window = SDL_CreateWindow(
        "sdl-xclock",
        win_w, win_h,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
               (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, BG_R, BG_G, BG_B, 255);
        SDL_RenderClear(renderer);

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int radius = (int)(SDL_min(w, h) * CLOCK_RADIUS);
        draw_clock_face(renderer, w/2, h/2, radius);
        draw_hands(renderer, w/2, h/2, radius, t);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
