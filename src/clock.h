// clock.h
#ifndef CLOCK_H
#define CLOCK_H

#include <SDL3/SDL.h>
#include <time.h>

void draw_clock_face(SDL_Renderer* renderer, int cx, int cy, int radius);
void draw_hands(SDL_Renderer* renderer, int cx, int cy, int radius, const struct tm* t);

#endif
