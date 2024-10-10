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

void vpu_palette__load(const ReadonlyByteArray* palette);
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
