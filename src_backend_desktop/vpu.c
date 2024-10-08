#include "include_backend/vpu.h"

#include "include_backend/debug.h"

static MDColor palette__[4 * 16];

static u8 tile_mapping_mem__[2048];
static u8 direct_mem__[4096];

////////////////////////////////////////////////////////

void vdp_palette__foreach(PaletteID pal_id, MDColor (*func)(MDColor)) {
    for (int i = 0; i < 4 * 16; i++) {
        palette__[i] = func(palette__[i]);
    }
}

void vpu_palette__set_water_state(VpuPaletteWaterState water_state) {
    LOG("called [%s] water_state = %d", __func__, water_state)
}

void vpu_palette__load(VpuPaletteID pal_id) {
    LOG("called [%s] pal_id = %d", __func__, pal_id)
}

void vpu_palette__set_color(int color_index, MDColor color) {
    ASSERT(color_index > 0 && color_index <= 4 * 16)

    LOG("called [%s] index = %d color = %04X", __func__, color_index, color)

    palette__[color_index] = color;
}

void vpu__sleep_until_vblank() {
    RAISE_NOT_IMPLEMENTED
}

void vpu__set_color_mode(VpuColorMode mode) {
    LOG("called [%s] VDP ColorMode = %d", __func__, mode)
    RAISE_NOT_IMPLEMENTED
}

void vpu__set_background_color(u8 palette_number, u8 color_number) {
    LOG("called [%s] VDP BG color was set to pal_row = %d and index = %d", __func__, palette_number, color_number)
    RAISE_NOT_IMPLEMENTED
}

void vpu__clear_screen() {
    LOG("called [%s] VDP screen cleared", __func__)
    RAISE_NOT_IMPLEMENTED
}

void vpu__set_scrolling_mode(VpuVScrollMode vertical_mode, VpuHScrollMode horizontal_mode, int enable_interrupt){LOG(
  "called [%s] VDP srolling mode set to VScroll = %s, HScroll = %s", __func__,
  (vertical_mode == VPU_VSCROLL_MODE__FULL_SCROLL) ? "VPU_VSCROLL_MODE__FULL_SCROLL" : "VPU_VSCROLL_MODE__EACH_2_CELL",
  (horizontal_mode == VPU_HSCROLL_MODE__FULL_SCROLL)   ? "VPU_HSCROLL_MODE__FULL_SCROLL"
  : (horizontal_mode == VPU_HSCROLL_MODE__PROHIBITED)  ? "VPU_HSCROLL_MODE__PROHIBITED"
  : (horizontal_mode == VPU_HSCROLL_MODE__EACH_1_CELL) ? "VPU_HSCROLL_MODE__EACH_1_CELL"
                                                       : "VPU_HSCROLL_MODE__EACH_1_LINE"
) RAISE_NOT_IMPLEMENTED}

u8* vpu__get_mutable_direct_memory() {
    return direct_mem__;
}

u8* vpu__get_mutable_memory_256x256_tile_mappings() {
    return tile_mapping_mem__;
}

void vpu__set_address_for_layer(VpuLayer layer, u16 adr) {
    LOG("called [%s] VDP Plane [%s] addr set to $04X", __func__, (layer == VPU_LAYER__BACKGROUND) ? "BG" : "FG", adr)
    RAISE_NOT_IMPLEMENTED
}

void vpu__copy_tilemap_to_layer_r(
  VpuLayer layer, i32 shift, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
) {
    LOG(
      "called [%s] VDP Plane [%s] shift = %d, cells_width = %d, cells_height = %d", __func__,
      (layer == VPU_LAYER__BACKGROUND) ? "BG" : "FG", shift, cells_width, cells_height
    )
    RAISE_NOT_IMPLEMENTED
}
