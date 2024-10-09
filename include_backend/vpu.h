#ifndef SONIC_ANYWHERE_VPU_H
#define SONIC_ANYWHERE_VPU_H

#include "mdcolor.h"
#include "../src/bytearray.h"

void vpu__init();

typedef enum PaletteID {
    PALETTE_ID_MAIN,
    PALETTE_ID_WATER
} PaletteID;

void vdp_palette__foreach(PaletteID pal_id, MDColor (*func)(MDColor));

typedef enum VpuPaletteWaterState {
    VPU_PALETTE_WATER_STATE__DRY_OR_PARTIALLY,
    VPU_PALETTE_WATER_STATE__ALL_UNDERWATER
} VpuPaletteWaterState;

void vpu_palette__set_water_state(VpuPaletteWaterState water_state);

typedef enum VpuPaletteID {
    VPU_PALETTE_ID__SEGA_LOGO,
    VPU_PALETTE_ID__TITLE,
    VPU_PALETTE_ID__LEVEL_SELECT,
    VPU_PALETTE_ID__SONIC,

    VPU_PALETTE_ID__GREEN_HILL_ZONE,
    VPU_PALETTE_ID__LABYRINTH_ZONE,
    VPU_PALETTE_ID__MARBLE_ZONE,
    VPU_PALETTE_ID__STAR_LIGHT_ZONE,
    VPU_PALETTE_ID__SPRING_YARD_ZONE,
    VPU_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_1,

    VPU_PALETTE_ID__SPECIAL_STAGE,
    VPU_PALETTE_ID__LABYRINTH_ZONE_WATER,
    VPU_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3,
    VPU_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_3_WATER,
    VPU_PALETTE_ID__SCRAP_BRAIN_ZONE_ACT_2,

    VPU_PALETTE_ID__SONIC_UNDERWATER_LABYRINTH,
    VPU_PALETTE_ID__SONIC_UNDERWATER_SCRAP_BRAIN,

    VPU_PALETTE_ID__SPECIAL_STAGE_RESULTS,
    VPU_PALETTE_ID__SPECIAL_STAGE_RESULTS_CONTINUE,
    VPU_PALETTE_ID__ENDING_SEQUENCE
} VpuPaletteID;

void vpu_palette__load(VpuPaletteID pal_id);
void vpu_palette__set_color(int color_index, MDColor color);

void vpu__sleep_until_vblank();

typedef enum VpuColorMode {
    VPU_COLOR_MODE_8_COLOR
} VpuColorMode;

void vpu__set_color_mode(VpuColorMode mode);
void vpu__set_background_color(u8 palette_number, u8 color_number);
void vpu__clear_screen();



typedef enum VpuVScrollMode {
    VPU_VSCROLL_MODE__FULL_SCROLL,
    VPU_VSCROLL_MODE__EACH_2_CELL
} VpuVScrollMode;

typedef enum VpuHScrollMode {
    VPU_HSCROLL_MODE__FULL_SCROLL,
    VPU_HSCROLL_MODE__PROHIBITED,
    VPU_HSCROLL_MODE__EACH_1_CELL,
    VPU_HSCROLL_MODE__EACH_1_LINE,
} VpuHScrollMode;

void vpu__set_scrolling_mode(VpuVScrollMode vertical_mode, VpuHScrollMode horizontal_mode, int enable_interrupt);




void vpu__set_window(const u8* window, size window_size);



u8* vpu__get_mutable_memory_256x256_tile_mappings();

typedef enum VpuLayer {
    VPU_LAYER__BACKGROUND,
    VPU_LAYER__FOREGROUND
} VpuLayer;

void vpu__set_address_for_layer(VpuLayer layer, u16 adr);
void vpu__copy_tilemap_to_layer_r(VpuLayer layer, i32 shift, const ReadonlyByteArray* tilemap, size cells_width, size cells_height);


#endif // SONIC_ANYWHERE_VPU_H
