#ifndef SONIC_ANYWHERE_VPU_H
#define SONIC_ANYWHERE_VPU_H

#include "../src/bytearray.h"
#include "mdcolor.h"

void vdp__init();

void vdp_palette__load(const ReadonlyByteArray* palette);

void vdp__sleep_until_vblank();

typedef enum VdpColorMode { VDP_COLOR_MODE_8_COLOR } VdpColorMode;

void vdp__set_color_mode(VdpColorMode mode);
void vdp__set_background_color(u8 palette_number, u8 color_number);
void vdp__clear_screen();


typedef enum VdpVScrollMode { VDP_VSCROLL_MODE__FULL_SCROLL, VDP_VSCROLL_MODE__EACH_2_CELL } VdpVScrollMode;

typedef enum VdpHScrollMode {
    VDP_HSCROLL_MODE__FULL_SCROLL,
    VDP_HSCROLL_MODE__PROHIBITED,
    VDP_HSCROLL_MODE__EACH_1_CELL,
    VDP_HSCROLL_MODE__EACH_1_LINE,
} VdpHScrollMode;

void vdp__set_scrolling_mode(VdpVScrollMode vertical_mode, VdpHScrollMode horizontal_mode, int enable_interrupt);

void vdp__set_window(const u8* window, size window_size);

typedef enum VdpPlane { VDP_PLANE__BACKGROUND, VDP_PLANE__FOREGROUND } VdpPlane;

void vdp__set_address_for_plane(VdpPlane plane_id, MutableByteArray* mem);
void vdp__copy_tilemap_to_layer_r(
  VdpPlane plane_id, i32 shift, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
);

typedef enum VdpRamAccessMode {
    VDP_RAM_ACCESS_MODE__VRAM_WRITE,
    VDP_RAM_ACCESS_MODE__CRAM_WRITE,
    VDP_RAM_ACCESS_MODE__VSRAM_WRITE,

    VDP_RAM_ACCESS_MODE__VRAM_READ,
    VDP_RAM_ACCESS_MODE__CRAM_READ,
    VDP_RAM_ACCESS_MODE__VSRAM_READ,
} VdpRamAccessMode;

void vdp__set_ram_address(VdpRamAccessMode access_mode, u16 adr, bool vram_to_vram_copy, bool dma);


#endif // SONIC_ANYWHERE_VPU_H
