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


#endif // SONIC_ANYWHERE_VPU_H
