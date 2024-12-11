#ifndef SONIC_ANYWHERE_MDVDP_H
#define SONIC_ANYWHERE_MDVDP_H

#include "../src/bytearray.h"
#include "mdmem.h"
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
    MD_VDP_PLANE__WINDOW,
    MD_VDP_PLANE__SPRITE
} MdVdpPlane;

void md_vdp__set_name_table_location_for_plane(MdVdpPlane plane_id, const MutableByteArray* mem);

void md_vdp__copy_tilemap_to_plane_r(
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


typedef enum MdVdpPlaneSize {
    MD_VDP_PLANE_SIZE__32_CELLS, // 256 pixels
    MD_VDP_PLANE_SIZE__64_CELLS, // 512 pixels
    MD_VDP_PLANE_SIZE__INVALID,
    MD_VDP_PLANE_SIZE__128_CELLS, // 1024 pixels
} MdVdpPlaneSize;

// reg 0x10
// Plane Size
// move.w #$90xx,($c00004).l
void md_vdp__set_plane_size(MdVdpPlaneSize cells_height, MdVdpPlaneSize cells_width);

typedef enum MdVdpWindowDirection {
    MD_VDP_WINDOW_POSITION__FROM_BACK_TO_FRONT,
    MD_VDP_WINDOW_POSITION__FROM_FRONT_TO_BACK,
} MdVdpWindowDirection;

// reg 0x11
// Window Plane Horizontal Position
// move.w #$91xx,($c00004).l
void md_vdp__set_window_horizontal_position(MdVdpWindowDirection dir, u8 units);

// reg 0x12
// Window Plane Vertical Position
// move.w #$92xx,($c00004).l
void md_vdp__set_window_vertical_position(MdVdpWindowDirection dir, u8 units);

// DMA (Direct Memory Access) access
const MdMemoryVram* md_vdp__dma_begin(); // get pointers to VRAM
void md_vdp__dma_end(); // update internal VDP state after VRAM modification


#endif // SONIC_ANYWHERE_MDVDP_H
