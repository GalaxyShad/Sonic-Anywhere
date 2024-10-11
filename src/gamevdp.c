#include "gamevdp.h"

#include "include_backend/vpu.h"
#include "include_backend/debug.h"

#include "resources/resourcestore.h"



void game_vdp__load_palette(GameVdpPaletteID pal_id) {
    ResourceID res_id;

    switch (pal_id) {

    case GAME_VDP_PALETTE_ID__SEGA_LOGO:
        res_id = RESOURCE__PALETTE__SEGA1_BIN;
        break;
    case GAME_VDP_PALETTE_ID__TITLE:
        res_id = RESOURCE__PALETTE__TITLE_SCREEN_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LEVEL_SELECT:
        res_id = RESOURCE__PALETTE__LEVEL_SELECT_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC:
        res_id = RESOURCE__PALETTE__SONIC_BIN;
        break;
    case GAME_VDP_PALETTE_ID__GREEN_HILL_ZONE:
        res_id = RESOURCE__PALETTE__GREEN_HILL_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LABYRINTH_ZONE:
        res_id = RESOURCE__PALETTE__LABYRINTH_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__MARBLE_ZONE:
        res_id = RESOURCE__PALETTE__MARBLE_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__STAR_LIGHT_ZONE:
        res_id = RESOURCE__PALETTE__STAR_LIGHT_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPRING_YARD_ZONE:
        res_id = RESOURCE__PALETTE__SPRING_YARD_ZONE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_1:
        res_id = RESOURCE__PALETTE__SBZ_ACT_1_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_BIN;
        break;
    case GAME_VDP_PALETTE_ID__LABYRINTH_ZONE_WATER:
        res_id = RESOURCE__PALETTE__LABYRINTH_ZONE_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3:
        res_id = RESOURCE__PALETTE__SBZ_ACT_3_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3_WATER:
        res_id = RESOURCE__PALETTE__SBZ_ACT_3_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_2:
        res_id = RESOURCE__PALETTE__SBZ_ACT_2_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_LABYRINTH:
        res_id = RESOURCE__PALETTE__SONIC_LZ_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_SCRAP_BRAIN:
        res_id = RESOURCE__PALETTE__SONIC_SBZ3_UNDERWATER_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_RESULTS_BIN;
        break;
    case GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS_CONTINUE:
        res_id = RESOURCE__PALETTE__SPECIAL_STAGE_CONTINUE_BONUS_BIN;
        break;
    case GAME_VDP_PALETTE_ID__ENDING_SEQUENCE:
        res_id = RESOURCE__PALETTE__ENDING_BIN;
        break;
    }

    ReadonlyByteArray res = resource_store__get(res_id);

    vpu_palette__load(&res);
}

void game_vdp__set_palette_water_state(GameVdpPaletteWaterState water_state) {
    RAISE_NOT_IMPLEMENTED
}

void game_vdp__palette_foreach(GameVdpPaletteLayerID pal_id, MDColor (*func)(MDColor)) {
    RAISE_NOT_IMPLEMENTED
}

void game_vdp__palette_set_color(u8 index, MDColor color) {
    RAISE_NOT_IMPLEMENTED
}
void game_vdp__set_vblank_routine_counter(u8 routine) {
    RAISE_NOT_IMPLEMENTED
}
