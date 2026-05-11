// clock.c
#include "clock.h"
#include "config.h"
#include <math.h>

static void render_fill_circle(SDL_Renderer* renderer, int cx, int cy, int radius)
{
    for (int y = -radius; y <= radius; y++) {
        int w = (int)sqrtf(radius * radius - y * y);
        SDL_RenderLine(renderer, cx - w, cy + y, cx + w, cy + y);
    }
}

void draw_clock_face(SDL_Renderer* renderer, int cx, int cy, int radius)
{
    SDL_SetRenderDrawColor(renderer, FACE_R, FACE_G, FACE_B, 255);
    for (int w = -2; w <= 2; w++)
        for (int h = -2; h <= 2; h++)
            render_fill_circle(renderer, cx + w, cy + h, radius);
    SDL_SetRenderDrawColor(renderer, 230, 230, 245, 255);
    render_fill_circle(renderer, cx, cy, radius - 3);

    for (int i = 0; i < 12; i++) {
        float angle = i * 30.0f * M_PI / 180.0f;
        int inner = radius - ((i % 3 == 0) ? 25 : 15);
        int outer = radius - 5;
        int x1 = cx + (int)(cosf(angle) * inner);
        int y1 = cy + (int)(sinf(angle) * inner);
        int x2 = cx + (int)(cosf(angle) * outer);
        int y2 = cy + (int)(sinf(angle) * outer);
        SDL_SetRenderDrawColor(renderer, 50, 50, 70, 255);
        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

void draw_hands(SDL_Renderer* renderer, int cx, int cy, int radius, const struct tm* t)
{
    float hour_angle = (t->tm_hour % 12 + t->tm_min / 60.0f) * 30.0f * M_PI / 180.0f;
    float min_angle  = (t->tm_min + t->tm_sec / 60.0f) * 6.0f * M_PI / 180.0f;
    float sec_angle  = t->tm_sec * 6.0f * M_PI / 180.0f;

    int hx = cx + (int)(cosf(hour_angle) * radius * 0.50f);
    int hy = cy + (int)(sinf(hour_angle) * radius * 0.50f);
    SDL_SetRenderDrawColor(renderer, HOUR_R, HOUR_G, HOUR_B, 255);
    for (int off = -2; off <= 2; off++)
        SDL_RenderLine(renderer, cx + off, cy, hx + off, hy);

    int mx = cx + (int)(cosf(min_angle) * radius * 0.78f);
    int my = cy + (int)(sinf(min_angle) * radius * 0.78f);
    SDL_SetRenderDrawColor(renderer, MIN_R, MIN_G, MIN_B, 255);
    for (int off = -1; off <= 1; off++)
        SDL_RenderLine(renderer, cx + off, cy, mx + off, my);

    int sx = cx + (int)(cosf(sec_angle) * radius * 0.85f);
    int sy = cy + (int)(sinf(sec_angle) * radius * 0.85f);
    SDL_SetRenderDrawColor(renderer, SEC_R, SEC_G, SEC_B, 255);
    SDL_RenderLine(renderer, cx, cy, sx, sy);

    SDL_SetRenderDrawColor(renderer, 255, 220, 80, 255);
    render_fill_circle(renderer, cx, cy, 10);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    render_fill_circle(renderer, cx, cy, 4);
}
