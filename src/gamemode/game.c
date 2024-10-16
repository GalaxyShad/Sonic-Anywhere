#include "game.h"

#include "include_backend/debug.h"

#include "include_backend/mdmem.h"
#include "include_backend/mdvdp.h"

static GameMode game_mode__;

void game__load_game_mode(GameMode gm) {
    ASSERT(gm >= GM_SEGA && gm <= GM_CREDIT);
    game_mode__ = gm;
}

void game__init() {
//    md_vdp__init();

md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__BACKGROUND, mem__vram_background());
    md_vdp__set_name_table_location_for_plane(MD_VDP_PLANE__FOREGROUND, mem__vram_foreground());

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

GameMode game__get_game_mode() {
    return game_mode__;
}