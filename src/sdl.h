#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL3/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int init(void);

#endif