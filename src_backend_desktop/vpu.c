#include "include_backend/vdp.h"

#include "include_backend/debug.h"

#include "src/gamevdp.h"

static MDColor palette__[4 * 16];
static u8 palette_bg_index__ = 0;

static u8 tile_mapping_mem__[256 * 256];

////////////////////////////////////////////////////////

void vdp_palette__load(const ReadonlyByteArray* palette) {
    LOG("called [%s] pal_id = %d", __func__)

//    for (int i = 0; i < palette->size / 2; i++) {
//        palette__[i] = ((MDColor*)palette)[i];
//    }

    for (int i = 0; i < palette->size / 2; i++) {
        palette__[i] = (palette->arr[i * 2] << 8) | palette->arr[i * 2 + 1];
    }
}

void vdp_palette__load_u16(const u16* pal) {
    for (int i = 0; i < 16 * 4; i++) {
        palette__[i] = pal[i];
    }
}

void vdp__set_color_mode(VdpColorMode mode) {
    LOG("called [%s] VDP ColorMode = %d", __func__, mode)
    RAISE_NOT_IMPLEMENTED
}

void vdp__set_background_color(u8 palette_number, u8 color_number) {
    LOG("called [%s] VDP BG color was set to pal_row = %d and index = %d", __func__, palette_number, color_number)
    palette_bg_index__ = palette_number * 16 + color_number;
}

void vdp__clear_screen() {
    LOG("called [%s] VDP screen cleared", __func__)
    RAISE_NOT_IMPLEMENTED
}

void vdp__set_scrolling_mode(VdpVScrollMode vertical_mode, VdpHScrollMode horizontal_mode, int enable_interrupt){LOG(
  "called [%s] VDP srolling mode set to VScroll = %s, HScroll = %s", __func__,
  (vertical_mode == VDP_VSCROLL_MODE__FULL_SCROLL) ? "VDP_VSCROLL_MODE__FULL_SCROLL" : "VDP_VSCROLL_MODE__EACH_2_CELL",
  (horizontal_mode == VDP_HSCROLL_MODE__FULL_SCROLL)   ? "VDP_HSCROLL_MODE__FULL_SCROLL"
  : (horizontal_mode == VDP_HSCROLL_MODE__PROHIBITED)  ? "VDP_HSCROLL_MODE__PROHIBITED"
  : (horizontal_mode == VDP_HSCROLL_MODE__EACH_1_CELL) ? "VDP_HSCROLL_MODE__EACH_1_CELL"
                                                       : "VDP_HSCROLL_MODE__EACH_1_LINE"
) RAISE_NOT_IMPLEMENTED}

void vdp__set_ram_address(VdpRamAccessMode access_mode, u16 adr, bool vram_to_vram_copy, bool dma) {
    RAISE_NOT_IMPLEMENTED
}


///////////////////////////
///////////////////////////

#include <SDL3/SDL.h>
#include <string.h>
#include "text_renderer.h"

static SDL_Window* window__ = NULL;
static SDL_Renderer* renderer__ = NULL;

typedef struct Plane {
    u8* data;
    size data_size;
} Plane;



static u8 tiles__[4096 * 16];

static Plane planes__[2] = {};

static u8 cells_hor_count__ = 40;

void vdp__set_address_for_plane(VdpPlane plane_id, MutableByteArray* mem) {
    LOG("called [%s] VDP Plane [%s] addr set to %p", __func__, (plane_id == VDP_PLANE__BACKGROUND) ? "BG" : "FG", mem->arr)

    Plane* p = planes__ + plane_id;

    p->data = mem->arr;
    p->data_size = mem->size;
}

void vdp__set_window(const u8* window, size window_size) {
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

    SDL_Surface* temp_surf = SDL_CreateSurface(8, 8, SDL_PIXELFORMAT_ARGB8888);
    SDL_FillSurfaceRect(temp_surf, NULL, 0);
    SDL_SetSurfaceColorKey(temp_surf, 1, 0);

    for (int i = 0; i < 32; i++) {
        u8 px = tile[i];

        u8 a = (px & 0xF0) >> 4;
        u8 b = (px & 0x0F) >> 0;

        u32 color_a = (a != 0) ? (0xFF000000 | mdcolor_to_sdl__(palette__[pal_index * 16 + a])) : 0;
        u32 color_b = (b != 0) ? (0xFF000000 | mdcolor_to_sdl__(palette__[pal_index * 16 + b])) : 0;

        u8 x = (i % 4) * 2;
        u8 y = i / 4;

        u32* ss = (u32*) temp_surf->pixels;

        ss[y * 8 + (x + 0)] = color_a;
        ss[y * 8 + (x + 1)] = color_b;
    }

    return temp_surf;
}

static void draw_plane__(Plane* plane, int x, int y, int scale) {
    int plane_width = cells_hor_count__;
    int plane_height = 28;

    for (int i = 0; i < plane->data_size / 2; i++) {
        if ((i / plane_width) >= plane_height)
            break;

        u16 a = plane->data[i * 2 + 0];
        u16 b = plane->data[i * 2 + 1];

        u16 word = (a << 8) | b;

        // PCCV HAAA AAAA AAAA

        u16 p = (word >> 15) & 1; //  TODO
        u16 pal_id = (word >> 13) & 0b11;
        u16 vert_flip = (word >> 12) & 1;
        u16 hor_flip = (word >> 11) & 1;
        u16 tile_index = word & 0b0000011111111111;

        i32 pos = i;

        SDL_FRect r_src = {0, 0, 8, 8};

        SDL_FRect r_dst = {
          (pos % plane_width) * 8 * scale + x, (pos / plane_width) * 8 * scale + y, 8 * scale, 8 * scale
        };

        SDL_Surface* tile = make_tile__(tile_index, pal_id);

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer__, tile);
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

        int sdl_flip = 0;

        if (vert_flip)
            sdl_flip |= SDL_FLIP_VERTICAL;
        if (hor_flip)
            sdl_flip |= SDL_FLIP_HORIZONTAL;

        SDL_RenderTextureRotated(renderer__, tex, &r_src, &r_dst, 0, NULL, sdl_flip);

        SDL_DestroyTexture(tex);

        SDL_DestroySurface(tile);
    }
}



static void sdl_set_draw_color_default__() {
    SDL_SetRenderDrawColor(renderer__, 0, 0, 0, 0xFF);
}

static void sdl_set_draw_color_u32__(u32 color) {
    u8 alpha = 0xFF & (color >> 24);

    u8 r = 0xFF & (color >> 16);
    u8 g = 0xFF & (color >> 8);
    u8 b = 0xFF & (color >> 0);

    SDL_SetRenderDrawColor(renderer__, r, g, b, alpha);
}


static void vpu__debug_draw_palette__(MDColor* pal, int x, int y) {
    int scale = 4;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            SDL_Rect rect = {.x = j * 5 * scale + x, .y = i * 5 * scale + y, .w = 4 * scale, .h = 4 * scale};

            u32 col = mdcolor_to_sdl__(pal[i * 16 + j]);

            sdl_set_draw_color_u32__(col);
            SDL_RenderFillRect(renderer__, &rect);
            sdl_set_draw_color_default__();
        }
    }
}

static void vpu__debug_draw_tiles__() {
    for (int i = 0; i < 512; i++) {
        SDL_Surface* tile = make_tile__(i, 0);

        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer__, tile);

        const int in_row = 40;

        SDL_FRect rsrc = {0, 0, 8, 8};

        int dst_x = (i % in_row) * 9 * 3 + 32;
        int dst_y = (i / in_row) * 9 * 3 + 600;
        SDL_FRect rdst = {dst_x, dst_y, 8 * 3, 8 * 3};

        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

        SDL_RenderTexture(renderer__, tex, &rsrc, &rdst);

        SDL_DestroySurface(tile);
        SDL_DestroyTexture(tex);
    }
}


static void vdp__screen_clear__() {
    u32 transparent = mdcolor_to_sdl__(palette__[palette_bg_index__]);

    sdl_set_draw_color_u32__(0xFF00AAAA);
    SDL_RenderClear(renderer__);
    sdl_set_draw_color_default__();
}




void vdp__copy_tilemap_to_layer_r(
  VdpPlane plane_id, u8 shift_x, u8 shift_y, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
) {
    Plane* p = planes__ + plane_id;

    u16* src = (u16*)tilemap->arr;
    u16* dst = (u16*)p->data;

    for (int i = 0; i < cells_height * cells_width; i++) {
        int x = i % cells_width;
        int y = i / cells_width;

        dst[(y + shift_y) * cells_hor_count__ + x + shift_x] = src[y * cells_width + x];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void vdp__init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR("could not initialize sdl2: %s\n", SDL_GetError())
        return;
    }

    window__ =
      SDL_CreateWindow("hello_sdl2", 1280, 720, 0);
    if (window__ == NULL) {
        LOG_ERROR("could not create window: %s\n", SDL_GetError())
        return;
    }

    renderer__ = SDL_CreateRenderer(window__, NULL);

    SDL_PumpEvents();
}

// Render without interrupts
void vdp__render() {
    u64 frame_start_ticks = SDL_GetTicks();

    vdp__screen_clear__();

    //    vpu__debug_draw_palette__(palette__, 8, 8);

        draw_plane__(&planes__[VDP_PLANE__BACKGROUND], 64, 100, 2);
        draw_plane__(&planes__[VDP_PLANE__FOREGROUND], 64, 100, 2);

    //    draw_plane__(&planes__[VDP_PLANE__BACKGROUND], 800, 100, 1);
    //    draw_plane__(&planes__[VDP_PLANE__FOREGROUND], 800, 100+224, 1);

    //    vpu__debug_draw_tiles__();

    u64 frame_duration = SDL_GetTicks() - frame_start_ticks;

    double fps = 0;
    if (frame_duration != 0) {
        fps = 1000.0 / (double) frame_duration;
    }

    char fps_text[20];
    snprintf(fps_text, 20, "fps: %lf", fps);

    sdl_utils__render_text(renderer__, fps_text, 16, 16, 4);

    SDL_RenderPresent(renderer__);

    game_vdp__on_vblank_interrupt();
}