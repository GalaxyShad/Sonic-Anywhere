#ifndef SONIC_ANYWHERE_MDVDP_H
#define SONIC_ANYWHERE_MDVDP_H

#include "../src/bytearray.h"
#include "mdcolor.h"

void md_vdp__init();

void md_vdp_palette__load(const ReadonlyByteArray* palette);
void md_vdp_palette__load_u16(const u16* pal); // FIXME temp solution for MSB and LSB byte orders

void md_vdp__render();

typedef enum MdVdpColorMode { MD_VDP_COLOR_MODE_8_COLOR } MdVdpColorMode;

void md_vdp__set_color_mode(MdVdpColorMode mode);
void md_vdp__set_background_color(u8 palette_number, u8 color_number);
void md_vdp__clear_screen();


typedef enum MdVdpVScrollMode { MD_VDP_VSCROLL_MODE__FULL_SCROLL, MD_VDP_VSCROLL_MODE__EACH_2_CELL } MdVdpVScrollMode;

typedef enum MdVdpHScrollMode {
    MD_VDP_HSCROLL_MODE__FULL_SCROLL,
    MD_VDP_HSCROLL_MODE__PROHIBITED,
    MD_VDP_HSCROLL_MODE__EACH_1_CELL,
    MD_VDP_HSCROLL_MODE__EACH_1_LINE,
} MdVdpHScrollMode;

void md_vdp__set_scrolling_mode(MdVdpVScrollMode vertical_mode, MdVdpHScrollMode horizontal_mode, int enable_interrupt);

typedef enum MdVdpPlane {
    MD_VDP_PLANE__FOREGROUND,  // (Plane A)
    MD_VDP_PLANE__BACKGROUND,  // (Plane B)
    MD_VDP_PLANE__WINDOW
} MdVdpPlane;

void md_vdp__set_name_table_location_for_plane(MdVdpPlane plane_id, MutableByteArray* mem);

void md_vdp__copy_tilemap_to_layer_r(
  MdVdpPlane plane_id, u8 shift_x, u8 shift_y, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
);

typedef enum MdVdpRamAccessMode {
    MD_VDP_RAM_ACCESS_MODE__VRAM_WRITE,
    MD_VDP_RAM_ACCESS_MODE__CRAM_WRITE,
    MD_VDP_RAM_ACCESS_MODE__VSRAM_WRITE,

    MD_VDP_RAM_ACCESS_MODE__VRAM_READ,
    MD_VDP_RAM_ACCESS_MODE__CRAM_READ,
    MD_VDP_RAM_ACCESS_MODE__VSRAM_READ,
} MdVdpRamAccessMode;

void md_vdp__set_ram_address(MdVdpRamAccessMode access_mode, u16 adr, bool vram_to_vram_copy, bool dma);


#endif // SONIC_ANYWHERE_MDVDP_H
