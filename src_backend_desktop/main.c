
#include "src/gamemode/game.h"

#include "include_backend/vdp.h"

#include <SDL3/SDL.h>

static int game_thread__(void *ptr) {
   game__init();
   return 0;
}


int main(int argc, char* args[]) {
    SDL_Thread* game_thread = SDL_CreateThread(game_thread__, "game_thread", (void*)NULL);

    vdp__init();
    while (1) {
        vdp__render();
    }
}