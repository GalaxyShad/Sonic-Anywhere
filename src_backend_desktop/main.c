#include <SDL.h>
#include "include_backend/debug.h"
#include "src/gamemode/game.h"
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char* args[]) {
    game__init();


//
//    SDL_Window* window = NULL;
//    SDL_Surface* screenSurface = NULL;
//    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
//        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
//        return 1;
//    }
//    window = SDL_CreateWindow(
//      "hello_sdl2",
//      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//      SCREEN_WIDTH, SCREEN_HEIGHT,
//      SDL_WINDOW_SHOWN
//    );
//    if (window == NULL) {
//        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
//        return 1;
//    }
//    screenSurface = SDL_GetWindowSurface(window);
//
//    SDL_PumpEvents();
//
//    SDL_Event e;
//    int quit = 0;
////    while (!quit){
//
//
//        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0));
//        SDL_UpdateWindowSurface(window);
//
//        while (SDL_PollEvent(&e)){
//            if (e.type == SDL_QUIT){
//                quit = 1;
//            }
//            if (e.type == SDL_KEYDOWN){
//                quit = 1;
//            }
//            if (e.type == SDL_MOUSEBUTTONDOWN){
//                quit = 1;
//            }
//        }
////    }
//
//        SDL_Delay(20000);
//
//    SDL_DestroyWindow(window);
//    SDL_Quit();
    return 0;
}