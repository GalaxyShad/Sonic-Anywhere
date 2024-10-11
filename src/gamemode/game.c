#include "game.h"

#include "include_backend/debug.h"

#include "include_backend/vpu.h"
#include "include_backend/mem.h"

static GameMode game_mode__;

void game__load_game_mode(GameMode gm) {
    ASSERT(gm >= GM_SEGA && gm <= GM_CREDIT);
    game_mode__ = gm;
}

void game__init() {
    vpu__init();

    vpu__set_address_for_layer(VPU_LAYER__BACKGROUND, mem__vram_background());
    vpu__set_address_for_layer(VPU_LAYER__FOREGROUND, mem__vram_foreground());

    // TODO clear_ram();

    // TODO	VDPSetupGame


    // TODO	DACDriverLoad
    // TODO	JoypadInit

    game__load_game_mode(GM_SEGA);
    while (1) {
        if (game_mode__ == GM_SEGA)
            game_mode_sega();
        else
            return;
    }
}