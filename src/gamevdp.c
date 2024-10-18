#include "gamevdp.h"

#include "include_backend/debug.h"
#include "include_backend/macroutils.h"
#include "include_backend/mdsystem.h"
#include "include_backend/mdvdp.h"

#include "gamemode/game.h"
#include "resources/resourcestore.h"

#include "config/config.h"

static MDColor palette__[16 * 4];
static MDColor palette_water__[16 * 4];

void game_vdp__load_palette(GameVdpPaletteID pal_id) {
    ResourceID res_id;

    // clang-format off
    switch (pal_id) {
        MAP_CASE(GAME_VDP_PALETTE_ID__SEGA_LOGO,                        RESOURCE__PALETTE__SEGA1_BIN,                           res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__TITLE,                            RESOURCE__PALETTE__TITLE_SCREEN_BIN,                    res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__LEVEL_SELECT,                     RESOURCE__PALETTE__LEVEL_SELECT_BIN,                    res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SONIC,                            RESOURCE__PALETTE__SONIC_BIN,                           res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__GREEN_HILL_ZONE,                  RESOURCE__PALETTE__GREEN_HILL_ZONE_BIN,                 res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__LABYRINTH_ZONE,                   RESOURCE__PALETTE__LABYRINTH_ZONE_BIN,                  res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__MARBLE_ZONE,                      RESOURCE__PALETTE__MARBLE_ZONE_BIN,                     res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__STAR_LIGHT_ZONE,                  RESOURCE__PALETTE__STAR_LIGHT_ZONE_BIN,                 res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SPRING_YARD_ZONE,                 RESOURCE__PALETTE__SPRING_YARD_ZONE_BIN,                res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_1,           RESOURCE__PALETTE__SBZ_ACT_1_BIN,                       res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SPECIAL_STAGE,                    RESOURCE__PALETTE__SPECIAL_STAGE_BIN,                   res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__LABYRINTH_ZONE_WATER,             RESOURCE__PALETTE__LABYRINTH_ZONE_UNDERWATER_BIN,       res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3,           RESOURCE__PALETTE__SBZ_ACT_3_BIN,                       res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3_WATER,     RESOURCE__PALETTE__CYCLE_SBZ3_WATERFALL_BIN,            res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_2,           RESOURCE__PALETTE__SBZ_ACT_2_BIN,                       res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_LABYRINTH,       RESOURCE__PALETTE__SONIC_LZ_UNDERWATER_BIN,             res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_SCRAP_BRAIN,     RESOURCE__PALETTE__SONIC_SBZ3_UNDERWATER_BIN,           res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS,            RESOURCE__PALETTE__SPECIAL_STAGE_RESULTS_BIN,           res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS_CONTINUE,   RESOURCE__PALETTE__SPECIAL_STAGE_CONTINUE_BONUS_BIN,    res_id)
        MAP_CASE(GAME_VDP_PALETTE_ID__ENDING_SEQUENCE,                  RESOURCE__PALETTE__ENDING_BIN,                          res_id)
    }
    // clang-format on

    ReadonlyByteArray res = s_resource(res_id);

    for (int i = 0; i < res.size / 2; i++) {
        MDColor color = res.arr[i * 2];
        color = (color << 8) | res.arr[i * 2 + 1];
        palette__[i] = color;
    }
}

static GameVdpPaletteWaterState water_state__;

void game_vdp__set_palette_water_state(GameVdpPaletteWaterState water_state) {
    water_state__ = water_state;
}

void game_vdp__palette_copy_to_vdp() {
    md_vdp_palette__load_u16(
      (water_state__ == GAME_VDP_PALETTE_WATER_STATE__ALL_UNDERWATER) ? palette_water__ : palette__
    );
}

void game_vdp__palette_foreach(GameVdpPaletteLayerID pal_id, MDColor (*func)(MDColor)) {
    MDColor* pal = (pal_id == GAME_VDP_PALETTE_LAYER__MAIN) ? palette__ : palette_water__;

    for (int i = 0; i < 16 * 4; i++) {
        pal[i] = func(pal[i]);
    }
}

void game_vdp__palette_set_color(u8 index, MDColor color) {
    ASSERT(index >= 0 && index < 16 * 4)
    palette__[index] = color;
}


//////////////////////////////////////////////////////////////////////
