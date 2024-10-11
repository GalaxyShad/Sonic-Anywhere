#include "include_backend/vpu.h"

#include "include_backend/debug.h"

static MDColor palette__[4 * 16];

static u8 tile_mapping_mem__[256 * 256];

////////////////////////////////////////////////////////

void vpu_palette__load(const ReadonlyByteArray* palette){
    LOG("called [%s] pal_id = %d", __func__)

    for (int i = 0; i < palette->size / 2; i++) {
        palette__[i] = (palette->arr[i * 2] << 8) | palette->arr[i * 2 + 1];
    }
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

u8* vpu__get_mutable_memory_256x256_tile_mappings() {
    return tile_mapping_mem__;
}


///////////////////////////
///////////////////////////

#include <SDL.h>

static SDL_Window* window__ = NULL;
static SDL_Renderer* renderer__ = NULL;

typedef struct Plane {
    u8* data;
    size data_size;
    size cell_width;
    size cell_height;
} Plane;

static u8 tiles__[4096 * 16];

static Plane planes__[2] = {};

void vpu__set_address_for_layer(VpuLayer layer, MutableByteArray* mem) {
    LOG("called [%s] VDP Plane [%s] addr set to %p", __func__, (layer == VPU_LAYER__BACKGROUND) ? "BG" : "FG", mem->arr)

    Plane* p = planes__ + layer;

    p->data = mem->arr;
    p->data_size = mem->size;
    p->cell_width = 1;
    p->cell_height = 1;
}

void vpu__set_window(const u8* window, size window_size) {
    for (int i = 0; i < window_size; i++) {
        tiles__[i] = window[i];
    }
}

static u32 mdcolor_to_sdl__(u16 mdcolor) {
    u8 b = ((mdcolor & 0xE00) >> 8) << 4;
    u8 g = ((mdcolor & 0x0E0) >> 4) << 4;
    u8 r = ((mdcolor & 0x00E) >> 0) << 4;

    return 0xFF000000 | (r << 16) | (g << 8) | (b << 0);
}

static SDL_Surface* make_tile__(int tile_index, int pal_index) {
    u8* tile = tiles__ + tile_index * 32;

    SDL_Surface* temp_surf = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);

    for (int i = 0; i < 32; i++) {
        u8 px = tile[i];

        u8 a = (px & 0xF0) >> 4;
        u8 b = (px & 0x0F) >> 0;

        u32 color_a = (a != 0) ? (0xFF000000 | mdcolor_to_sdl__(palette__[pal_index * 16 + a - 1])) : 0;
        u32 color_b = (b != 0) ? (0xFF000000 | mdcolor_to_sdl__(palette__[pal_index * 16 + b - 1])) : 0;

        u8 x = (i % 4) * 2;
        u8 y = i / 4;

        u32* ss = (u32*) temp_surf->pixels;

        ss[y * 8 + (x + 0)] = color_a;
        ss[y * 8 + (x + 1)] = color_b;
    }

    return temp_surf;
}

static void draw_plane__(Plane* plane, int x, int y) {
    for (int i = 0; i < 0x2000 / 2; i++) {
        if ((i / plane->cell_width) >= plane->cell_height)
            break;

        u16 a = plane->data[i * 2 + 0];
        u16 b = plane->data[i * 2 + 1];

        u16 word = (a << 8) | b;

        //        PCCV HAAA AAAA AAAA

        u16 p = (word >> 15) & 1;
        u16 pal_id = (word >> 9) & 0b11;
        u16 vert_flip = (word >> 12) & 1;
        u16 hor_flip = (word >> 11) & 1;
        u16 tile_index = word & 0b0000011111111111;

        i32 pos = i;

        SDL_Rect r_src = {0, 0, 8, 8};

        int scale = 2;

        SDL_Rect r_dst = {
          (pos % plane->cell_width) * 8 * scale + x, (pos / plane->cell_width) * 8 * scale + y, 8 * scale, 8 * scale
        };

        SDL_Surface* tile = make_tile__(tile_index, pal_id);

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer__, tile);

        int sdl_flip = 0;

        if (vert_flip)
            sdl_flip |= SDL_FLIP_VERTICAL;
        if (hor_flip)
            sdl_flip |= SDL_FLIP_HORIZONTAL;

        SDL_RenderCopyEx(renderer__, tex, &r_src, &r_dst, 0, NULL, sdl_flip);

        SDL_DestroyTexture(tex);
        SDL_FreeSurface(tile);
    }
}

void vpu__init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LOG_ERROR("could not initialize sdl2: %s\n", SDL_GetError())
        return;
    }

    window__ =
      SDL_CreateWindow("hello_sdl2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    if (window__ == NULL) {
        LOG_ERROR("could not create window: %s\n", SDL_GetError())
        return;
    }

    renderer__ = SDL_CreateRenderer(window__, -1, SDL_RENDERER_ACCELERATED);

    SDL_PumpEvents();
}

static void vpu__debug_draw_palette__() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            SDL_Rect rect = {.x = j * 5, .y = i * 5, .w = 4, .h = 4};

            u32 col = mdcolor_to_sdl__(palette__[i * 16 + j]);
            u8 r = (col & 0xFF0000) >> 16;
            u8 g = (col & 0x00FF00) >> 8;
            u8 b = (col & 0x0000FF) >> 0;

            SDL_SetRenderDrawColor(renderer__, r, g, b, 0xFF);
            SDL_RenderFillRect(renderer__, &rect);
            SDL_SetRenderDrawColor(renderer__, 0x00, 0xAA, 0xAA, 0x00);
        }
    }
}

static void vpu__debug_draw_tiles__() {
    for (int i = 0; i < 512; i++) {
        SDL_Surface* tile = make_tile__(i, 0);

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer__, tile);

        const int in_row = 40;

        SDL_Rect rsrc = {0, 0, 8, 8};
        SDL_Rect rdst = {(i % in_row) * 9 * 3 + 32, (i / in_row) * 9 * 3 + 600, 8 * 3, 8 * 3};

        SDL_RenderCopy(renderer__, tex, &rsrc, &rdst);

        SDL_FreeSurface(tile);
        SDL_DestroyTexture(tex);
    }
}

void vpu__sleep_until_vblank() {
    SDL_RenderClear(renderer__);

    vpu__debug_draw_palette__();

    draw_plane__(&planes__[VPU_LAYER__BACKGROUND], 0, 100);
    draw_plane__(&planes__[VPU_LAYER__FOREGROUND], 400, 100);

    vpu__debug_draw_tiles__();

    SDL_RenderPresent(renderer__);

    SDL_Delay(100);
}


void vpu__copy_tilemap_to_layer_r(
  VpuLayer layer, i32 shift, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
) {
    LOG(
      "called [%s] VDP Plane [%s] shift = %d, cells_width = %d, cells_height = %d", __func__,
      (layer == VPU_LAYER__BACKGROUND) ? "BG" : "FG", shift, cells_width, cells_height
    )

    Plane* p = planes__ + layer;

    SDL_memcpy(p->data, tilemap->arr, p->data_size);
    p->cell_width = cells_width;
    p->cell_height = cells_height;
}