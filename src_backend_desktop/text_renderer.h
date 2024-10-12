#ifndef SONIC_ANYWHERE_TEXT_RENDERER_H
#define SONIC_ANYWHERE_TEXT_RENDERER_H

#include "SDL3/SDL.h"

void sdl_utils__render_text(SDL_Renderer* renderer, const char* msg, float x, float y, float scale);

#endif // SONIC_ANYWHERE_TEXT_RENDERER_H
