#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "clock.h"
#include "config.h"

static TTF_Font *font = NULL;

static void draw_digital_clock(SDL_Renderer *r, int win_w, int win_h, int hour, int min)
{
    char time_str[6];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", hour, min);

    SDL_Surface *surface = TTF_RenderText_Blended(font, time_str, (SDL_Color){255, 255, 255, 255});
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(r, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }

    float scale = (float)win_h / (float)surface->h * 0.6f;
    int tex_w = (int)(surface->w * scale);
    int tex_h = (int)(surface->h * scale);
    SDL_FRect dst = { (win_w - tex_w) / 2.0f, (win_h - tex_h) / 2.0f, tex_w, tex_h };
    SDL_RenderTexture(r, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

int main(int argc, char *argv[])
{
    int use_digital = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--digital") == 0) {
            use_digital = 1;
        }
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;
    if (TTF_Init() != 0) {
        SDL_Quit();
        return 1;
    }

    int win_w = use_digital ? 600 : WINDOW_WIDTH;
    int win_h = use_digital ? 200 : WINDOW_HEIGHT;

    SDL_Window *window = SDL_CreateWindow(
        "sdl-xclock",
        win_w, win_h,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP
    );
    if (!window) {
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 72);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/consola.ttf", 72);
    }
    if (!font) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
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

        if (use_digital) {
            draw_digital_clock(renderer, w, h, t->tm_hour, t->tm_min);
        } else {
            int radius = (int)(SDL_min(w, h) * CLOCK_RADIUS);
            draw_clock_face(renderer, w/2, h/2, radius);
            draw_hands(renderer, w/2, h/2, radius, t);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
