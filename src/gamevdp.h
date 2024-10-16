#ifndef SONIC_ANYWHERE_GAMEVDP_H
#define SONIC_ANYWHERE_GAMEVDP_H

#include "include_backend/mdcolor.h"

typedef enum GameVdpPaletteID {
    GAME_VDP_PALETTE_ID__SEGA_LOGO,
    GAME_VDP_PALETTE_ID__TITLE,
    GAME_VDP_PALETTE_ID__LEVEL_SELECT,
    GAME_VDP_PALETTE_ID__SONIC,

    GAME_VDP_PALETTE_ID__GREEN_HILL_ZONE,
    GAME_VDP_PALETTE_ID__LABYRINTH_ZONE,
    GAME_VDP_PALETTE_ID__MARBLE_ZONE,
    GAME_VDP_PALETTE_ID__STAR_LIGHT_ZONE,
    GAME_VDP_PALETTE_ID__SPRING_YARD_ZONE,
    GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_1,

    GAME_VDP_PALETTE_ID__SPECIAL_STAGE,
    GAME_VDP_PALETTE_ID__LABYRINTH_ZONE_WATER,
    GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3,
    GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3_WATER,
    GAME_VDP_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_2,

    GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_LABYRINTH,
    GAME_VDP_PALETTE_ID__SONIC_UNDERWATER_SCRAP_BRAIN,

    GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS,
    GAME_VDP_PALETTE_ID__SPECIAL_STAGE_RESULTS_CONTINUE,
    GAME_VDP_PALETTE_ID__ENDING_SEQUENCE,

    GAME_VDP_PALETTE_ID__SIZE__,    // Just to determine size of Enum
} GameVdpPaletteID;

void game_vdp__load_palette(GameVdpPaletteID pal_id);

typedef enum GameVdpPaletteWaterState {
   GAME_VDP_PALETTE_WATER_STATE__DRY_OR_PARTIALLY,
   GAME_VDP_PALETTE_WATER_STATE__ALL_UNDERWATER
} GameVdpPaletteWaterState;

void game_vdp__set_palette_water_state(GameVdpPaletteWaterState water_state);

typedef enum GameVdpPaletteLayerID {
    GAME_VDP_PALETTE_LAYER__MAIN,
    GAME_VDP_PALETTE_LAYER__WATER
} GameVdpPaletteLayerID;

void game_vdp__palette_foreach(GameVdpPaletteLayerID pal_id, MDColor (*func)(MDColor));

void game_vdp__palette_set_color(u8 index, MDColor color);

// same as -> move.b $14,(v_vbla_routine).w
void game_vdp__set_vblank_routine_counter(u8 routine);

void game_vdp__wait_for_vblank();

void game_vdp__on_vblank_interrupt();

#endif // SONIC_ANYWHERE_GAMEVDP_H
