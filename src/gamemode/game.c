#include "game.h"

#include "include_backend/debug.h"

static GameMode game_mode__;

void game__load_game_mode(GameMode gm) {
    ASSERT(gm >= GM_SEGA && gm <= GM_CREDIT);
    game_mode__ = gm;
}

_Noreturn void game__init() {
    // TODO clear_ram();

    // TODO	VDPSetupGame
    // TODO	DACDriverLoad
    // TODO	JoypadInit

    game__load_game_mode(GM_SEGA);
    while (1) {
        game_mode_sega();
    }
}