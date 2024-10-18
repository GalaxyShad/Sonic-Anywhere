#include "include_backend/mdvdp.h"

#include "include_backend/debug.h"

#include "src/gamevdp.h"

static MDColor palette__[4 * 16];
static u8 palette_bg_index__ = 0;

////////////////////////////////////////////////////////

void md_vdp_palette__load(const ReadonlyByteArray* palette) {
    LOG("called [%s] pal_id = %d", __func__)

    for (int i = 0; i < palette->size / 2; i++) {
        palette__[i] = (palette->arr[i * 2] << 8) | palette->arr[i * 2 + 1];
    }
}


void md_vdp__set_color_mode(MdVdpColorMode mode) {
    LOG("called [%s] VDP ColorMode = %d", __func__, mode)
    NOT_IMPLEMENTED
}

void md_vdp__set_background_color(u8 palette_number, u8 color_number) {
    LOG("called [%s] VDP BG color was set to pal_row = %d and index = %d", __func__, palette_number, color_number)
    palette_bg_index__ = palette_number * 16 + color_number;
}

void md_vdp__clear_screen() {
    LOG("called [%s] VDP screen cleared", __func__)
    NOT_IMPLEMENTED
}

void md_vdp__set_scrolling_mode(MdVdpVScrollMode vertical_mode, MdVdpHScrollMode horizontal_mode, int enable_interrupt) {
    LOG(
      "called [%s] VDP srolling mode set to VScroll = %s, HScroll = %s", __func__,
      (vertical_mode == MD_VDP_VSCROLL_MODE__FULL_SCROLL) ? "MD_VDP_VSCROLL_MODE__FULL_SCROLL"
                                                       : "MD_VDP_VSCROLL_MODE__EACH_2_CELL",
      (horizontal_mode == MD_VDP_HSCROLL_MODE__FULL_SCROLL)   ? "MD_VDP_HSCROLL_MODE__FULL_SCROLL"
      : (horizontal_mode == MD_VDP_HSCROLL_MODE__PROHIBITED)  ? "MD_VDP_HSCROLL_MODE__PROHIBITED"
      : (horizontal_mode == MD_VDP_HSCROLL_MODE__EACH_1_CELL) ? "MD_VDP_HSCROLL_MODE__EACH_1_CELL"
                                                           : "MD_VDP_HSCROLL_MODE__EACH_1_LINE"
    )
    NOT_IMPLEMENTED
}

void md_vdp__set_ram_address(MdVdpRamAccessMode access_mode, u16 adr, bool vram_to_vram_copy, bool dma) {
    NOT_IMPLEMENTED
}


///////////////////////////
///////////////////////////

#include <raylib.h>

typedef struct Plane {
    u8* data;
    size data_size;
} Plane;


static u8 tiles__[4096 * 16 * 4];

static Plane planes__[2] = {};

static u8 cells_hor_count__ = 40;


static Texture2D vdp_window_tex__;
static Shader palette_shader__;
static int is_vdp_window_changed__ = 0;

static void vdp__set_window__(const u8* window, size window_size) {
    int w = 32;

    for (int i = 0; i < window_size; i++) {
        int y = i / 4;
        int x = i % 4;

        u8 hcol = ((window[i] >> 4) & 0xF);
        u8 lcol = ((window[i] >> 0) & 0xF);


        for (int j = 0; j < 4; j++) {
            tiles__[y * w + x * 2 + 0 + j * 8] = hcol + j * 16;
            tiles__[y * w + x * 2 + 1 + j * 8] = lcol + j * 16;
        }
    }

    is_vdp_window_changed__ = 1;
}

void md_vdp__set_name_table_location_for_plane(MdVdpPlane plane_id, const MutableByteArray* mem) {
    LOG(
      "called [%s] VDP Plane [%s] addr set to %p", __func__, (plane_id == MD_VDP_PLANE__BACKGROUND) ? "BG" : "FG", mem->arr
    )

    if (plane_id == MD_VDP_PLANE__WINDOW) {
        vdp__set_window__(mem->arr, mem->size);
        return;
    }

    Plane* p = planes__ + plane_id;

    p->data = mem->arr;
    p->data_size = mem->size;
}

static u32 mdcolor_to_rgba24__(u16 mdcolor) {
    u8 b = ((mdcolor & 0xE00) >> 8) << 4;
    u8 g = ((mdcolor & 0x0E0) >> 4) << 4;
    u8 r = ((mdcolor & 0x00E) >> 0) << 4;

    return (r << 24) | (g << 16) | (b << 8) | 0xFF;
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

        Rectangle r_src = {pal_id * 8, tile_index * 8, 8, 8};

        Rectangle r_dst = {
          (pos % plane_width) * 8 * scale + x, (pos / plane_width) * 8 * scale + y, 8 * scale, 8 * scale
        };

        if (vert_flip)
            r_src.height *= -1;
        if (hor_flip)
            r_src.width *= -1;

        DrawTexturePro(vdp_window_tex__, r_src, r_dst, (Vector2){0, 0}, 0, WHITE);
    }
}

static void vpu__debug_draw_palette__(MDColor* pal, int x, int y) {
    int scale = 4;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            Rectangle rect = {j * 5 * scale + x, i * 5 * scale + y, 4 * scale, 4 * scale};

            DrawRectangleRounded(rect, 0.5f, 4, GetColor(mdcolor_to_rgba24__(pal[i * 16 + j])));
        }
    }
}

static void vdp__screen_clear__() {
    u32 transparent = mdcolor_to_rgba24__(palette__[palette_bg_index__]);

    ClearBackground(GetColor(0x00AAAAFF));
}

void md_vdp__copy_tilemap_to_plane_r(
  MdVdpPlane plane_id, u8 shift_x, u8 shift_y, const ReadonlyByteArray* tilemap, size cells_width, size cells_height
) {
    Plane* p = planes__ + plane_id;

    u16* src = (u16*) tilemap->arr;
    u16* dst = (u16*) p->data;

    for (int i = 0; i < cells_height * cells_width; i++) {
        int x = i % cells_width;
        int y = i / cells_width;

        dst[(y + shift_y) * cells_hor_count__ + x + shift_x] = src[y * cells_width + x];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

static int shader_palette__[4 * 16 * 3] = {0};

void md_vdp_palette__load_u16(const u16* pal) {
    for (int i = 0; i < 16 * 4; i++) {
        palette__[i] = pal[i];

        Color col = GetColor(mdcolor_to_rgba24__(pal[i]));

        shader_palette__[i * 3 + 0] = col.r;
        shader_palette__[i * 3 + 1] = col.g;
        shader_palette__[i * 3 + 2] = col.b;
    }
}

void md_vdp__init() {
    InitWindow(1280, 720, "Sonic Anywhere");
    InitAudioDevice();

#define SHADER_PATH "../src_backend_desktop/shaders"

    palette_shader__ = LoadShader(0, SHADER_PATH "/fragment_shader.glsl");

    for (int i = 0; i < 4 * 16; i++) {
        Color col = ColorFromHSV(i * (360 / 64), 1, 1);

        shader_palette__[i * 3 + 0] = col.r;
        shader_palette__[i * 3 + 1] = col.g;
        shader_palette__[i * 3 + 2] = col.b;
    }

    SetMasterVolume(0.05f);
    SetTargetFPS(60);
}

// Render without interrupts
void md_vdp__render() {
    int paletteLoc = GetShaderLocation(palette_shader__, "palette");
    SetShaderValueV(palette_shader__, paletteLoc, shader_palette__, SHADER_UNIFORM_IVEC3, 16 * 4);

    BeginDrawing();

    vdp__screen_clear__();

    if (is_vdp_window_changed__) {
        if (vdp_window_tex__.id != 0) {
            UnloadTexture(vdp_window_tex__);
        }

        Image img;

        img.data = tiles__;
        img.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
        img.width = 32;
        img.height = (4096 * 16) / 32;
        img.mipmaps = 1;

        vdp_window_tex__ = LoadTextureFromImage(img);

        is_vdp_window_changed__ = 0;
    }

//    vpu__debug_draw_palette__(palette__, 900, 8);

    BeginShaderMode(palette_shader__);
    {
        draw_plane__(&planes__[MD_VDP_PLANE__BACKGROUND], 140, 100, 2);
        draw_plane__(&planes__[MD_VDP_PLANE__FOREGROUND], 140, 100, 2);
//
//        draw_plane__(&planes__[MD_VDP_PLANE__BACKGROUND], 800, 100, 1);
//        draw_plane__(&planes__[MD_VDP_PLANE__FOREGROUND], 800, 100 + 224, 1);

//        if (vdp_window_tex__.id != 0) {
//            DrawTextureEx(vdp_window_tex__, (Vector2){32, 32}, 0, 3, WHITE);
//        }
    }
    EndShaderMode();

    DrawFPS(16, 16);

    EndDrawing();

    game_vdp__on_vblank_interrupt();
}
void md_vdp__set_plane_size(MdVdpPlaneSize cells_width, MdVdpPlaneSize cells_height) {
    ASSERT(cells_height != MD_VDP_PLANE_SIZE__INVALID && cells_width != MD_VDP_PLANE_SIZE__INVALID)
    ASSERT(!(cells_height == MD_VDP_PLANE_SIZE__128_CELLS && cells_width == MD_VDP_PLANE_SIZE__128_CELLS))
    ASSERT(!(cells_height == MD_VDP_PLANE_SIZE__64_CELLS && cells_width == MD_VDP_PLANE_SIZE__128_CELLS))
    ASSERT(!(cells_height == MD_VDP_PLANE_SIZE__128_CELLS && cells_width == MD_VDP_PLANE_SIZE__64_CELLS))

    NOT_IMPLEMENTED
}
