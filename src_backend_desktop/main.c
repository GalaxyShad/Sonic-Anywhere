
#include "src/gamemode/game.h"

#include "include_backend/mdvdp.h"

void* game_thread__(void *ptr) {
   game__init();
   return 0;
}

#ifndef _WINDOWS

#include <pthread.h>

int main(int argc, char* args[]) {
    pthread_t game_thread;

    pthread_create(&game_thread, NULL, game_thread__, (void*)NULL);

    vdp__init();
    while (1) {
        vdp__render();
    }
}

#else

#include <Windows.h>

DWORD WINAPI game_thread_windows(LPVOID lpParam) {
    game__init();
    return 0;
}

int main(int argc, char* args[]) {
    HANDLE game_thread = CreateThread(NULL, 0, game_thread_windows, NULL, 0, NULL);

    md_vdp__init();
    while (1) {
        md_vdp__render();
    }

    WaitForSingleObject(game_thread, INFINITE); // Wait for the game thread to finish
    CloseHandle(game_thread);
    return 0;
}

#endif