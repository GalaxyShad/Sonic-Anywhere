
#include "src2/context.h"
#include "src2/sonic.h"

#include "include_backend/mdvdp.h"
#include "src2/variables.h"

static S1Context ctx__;

int should_vblank = 0;

void vbla_int() {
    should_vblank = 1;
}

void* game_thread__(void* ptr) {
    

    GameInit(&ctx__);

    return 0;
}

#ifndef _WIN32

#include <pthread.h>

int main(int argc, char* args[]) {
    ctx__.var = s1_getVariables();
    ctx__.varptr = s1_getVariablePointers();

    pthread_t game_thread;

    pthread_create(&game_thread, NULL, game_thread__, (void*) NULL);

    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__FOREGROUND, md_mem__vram()->plane_foreground_mut);
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__BACKGROUND, md_mem__vram()->plane_background_mut);
    
    md_vdp__set_vblank_interrupt(vbla_int);
    md_vdp__init();
    while (1) {
        md_vdp__render();
        if (should_vblank) {
            VBlank(&ctx__);
        }
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