
#include "src/gamemode/game.h"

#include "include_backend/vdp.h"

#include <pthread.h>
#include <raylib.h>

void* game_thread__(void *ptr) {
   game__init();
   return 0;
}


int main(int argc, char* args[]) {
    pthread_t game_thread;

    pthread_create(&game_thread, NULL, game_thread__, (void*)NULL);

    vdp__init();
    while (!WindowShouldClose()) {
        vdp__render();
    }
}