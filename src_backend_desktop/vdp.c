#include "include_backend/mdvdp.h"

#include "include_backend/debug.h"

#include "src/gamevdp.h"
#include <_stdlib.h>

#define PALETTES_ROWS_COUNT 4
#define PALETTE_COLORS_COUNT 16

static MDColor palette__[PALETTES_ROWS_COUNT * PALETTE_COLORS_COUNT];
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

void md_vdp__set_scrolling_mode(
  MdVdpVScrollMode vertical_mode, MdVdpHScrollMode horizontal_mode, int enable_interrupt
) {
    LOG(
      "called [%s] VDP srolling mode set to VScroll = %s, HScroll = %s",
      __func__,
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


static Plane planes__[2] = {};

#define SPRITE_COUNT 0x50
#define SPRITE_WORD_LENGTH 8


static u8 cells_hor_count__ = 40;
static u8 sprite_table__[SPRITE_COUNT * SPRITE_WORD_LENGTH] = {0};


#define COLOR_RGBA_WORD_SIZE 4


#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define TILE_MAP_TILES_COUNT 1024

#define TEXTURE_TILE_MAP_WIDTH ((TILE_WIDTH * PALETTES_ROWS_COUNT) / 2)
#define TEXTURE_TILE_MAP_HEIGHT (TILE_HEIGHT * TILE_MAP_TILES_COUNT)

static Texture2D vdp_window_tex__;
static u8 vdp_window_tex_grayscale_pixel_buffer__[TEXTURE_TILE_MAP_HEIGHT * TEXTURE_TILE_MAP_WIDTH]; // 1 byte per pixel

static Shader palette_shader__;
static int is_vdp_window_changed__ = 0;

static void vdp__draw_sprites__(float x, float y, float scale) {
    u8 next = 0;

    do {
        // https://segaretro.org/Sega_Mega_Drive/Sprites
        u8* sprite = sprite_table__ + next * 8;

        u16 vertical_pos = ((sprite[0] & 0b11) << 8) | sprite[1];

        // Size
        u8 horizontal_size = (sprite[2] >> 2) & 0b11;
        u8 vertical_size = (sprite[2] >> 0) & 0b11;

        u8 priority = sprite[4] >> 7;         // Priority
        u8 palette = (sprite[4] >> 5) & 0b11; // Palette line

        // Vertical & Horizontal flipping
        u8 vertical_flip = (sprite[4] >> 4) & 0b1;
        u8 horizontal_flip = (sprite[4] >> 3) & 0b1;

        u16 gfx_tile_index =
          ((sprite[4] & 0b111) << 8) | sprite[5]; // Tile number from VRAM. i.e.
                                                  // VRAM address to read graphics data, divided by $20.
        u16 horizontal_pos = ((sprite[6] & 0b1) << 8) | sprite[7];

        for (int j = 0; j < horizontal_size * vertical_size; j++) {
            int jx = j / vertical_size;
            int jy = j % vertical_size;

            int tile_index = gfx_tile_index + j;

            float scr_x = (float) (horizontal_pos + jx * 8) * scale + x;
            float scr_y = (float) (vertical_pos + jy * 8) * scale + y;

            DrawTexturePro(
              vdp_window_tex__,
              (Rectangle){palette * 8, tile_index * 8, 8, 8},
              (Rectangle){scr_x, scr_y, 8 * scale, 8 * scale},
              (Vector2){0, 0},
              0,
              WHITE
            );
        }

        next = sprite[3] & 0b1111111; // Next sprite number to jump to. Earlier sprites go on top of later ones. The
                                      // final sprite must jump to
    } while (next != 0);

    DrawRectangleLinesEx((Rectangle){x, y, 512 * scale, 512 * scale}, 2, RED);
    DrawRectangleLinesEx((Rectangle){x + 128 * scale, y + 128 * scale, 320 * scale, 224 * scale}, 2, RED);
}

static void vdp__set_window__(const u8* window, size window_size) {
    int w = 32;

    for (int i = 0; i < window_size; i++) {
        int y = i / 4;
        int x = i % 4;

        u8 hcol = ((window[i] >> 4) & 0xF);
        u8 lcol = ((window[i] >> 0) & 0xF);


        for (int j = 0; j < 4; j++) {
            vdp_window_tex_grayscale_pixel_buffer__[y * w + x * 2 + 0 + j * 8] = hcol + j * TILE_WIDTH;
            vdp_window_tex_grayscale_pixel_buffer__[y * w + x * 2 + 1 + j * 8] = lcol + j * TILE_WIDTH;
        }
    }

    is_vdp_window_changed__ = 1;
}

void md_vdp__set_name_table_location_for_plane(MdVdpPlane plane_id, const MutableByteArray* mem) {
    LOG(
      "called [%s] VDP Plane [%s] addr set to %p",
      __func__,
      (plane_id == MD_VDP_PLANE__BACKGROUND) ? "BG" : "FG",
      mem->arr
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

        u16 priority = (word >> 15) & 1; //  TODO
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
    InitWindow(1280, 800, "Sonic Anywhere");
    InitAudioDevice();
    SetExitKey(0);

#define SHADER_PATH "src_backend_desktop/shaders"

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

#include <printf.h>
static void draw_tiles_debug(float x, float y, int in_row_len, float scale, bool is_back_layer) {
    for (int i = 0; i < vdp_window_tex__.height / 8; i++) {
        float space = 2;

        Vector2 pos = {.x = x + (i % in_row_len) * (8 + space), .y = y + (i / in_row_len) * (8 + space)};

        Rectangle rect = {0, i * 8, 8, 8};

        if (is_back_layer) {
            // Tooltip
            if (GetMouseX() > pos.x && GetMouseX() < pos.x + 8 && GetMouseY() > pos.y && GetMouseY() < pos.y + 8) {
                char s[20];
                sprintf(s, "%04D x%02X", i, i);

                Rectangle tooltip_rect = {pos.x, pos.y - 16, 64, 16};

                DrawRectangleRounded(tooltip_rect, 0.25f, 0, BLACK);
                DrawText(s, tooltip_rect.x + 4, tooltip_rect.y + 4, 8, WHITE);
            }

            DrawRectangleLines(pos.x, pos.y, 8, 8, BLACK);

            continue;
        }

        DrawTextureRec(vdp_window_tex__, rect, pos, WHITE);
    }
}

// Render without interrupts
void md_vdp__render() {
    int paletteLoc = GetShaderLocation(palette_shader__, "palette");
    SetShaderValueV(palette_shader__, paletteLoc, shader_palette__, SHADER_UNIFORM_IVEC3, 16 * 4);

    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) 
        exit(0);

    BeginDrawing();

    vdp__screen_clear__();

    if (is_vdp_window_changed__) {
        if (vdp_window_tex__.id != 0) {
            UnloadTexture(vdp_window_tex__);
        }
        Image img;

        img.data = vdp_window_tex_grayscale_pixel_buffer__;
        img.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
        img.width = TEXTURE_TILE_MAP_WIDTH;
        img.height = TEXTURE_TILE_MAP_HEIGHT;
        img.mipmaps = 1;

        vdp_window_tex__ = LoadTextureFromImage(img);

        is_vdp_window_changed__ = 0;
    }

    vpu__debug_draw_palette__(palette__, 900, 8);

    BeginShaderMode(palette_shader__);
    {
        DrawRectangle(140, 100, 320* 2, 224 * 2, (Color){.a=0xFF, .r = palette_bg_index__});


        draw_plane__(&planes__[MD_VDP_PLANE__BACKGROUND], 140, 100, 2);
        draw_plane__(&planes__[MD_VDP_PLANE__FOREGROUND], 140, 100, 2);

        vdp__draw_sprites__(140 - 128 * 2, 100 - 128 * 2, 2);

        draw_plane__(&planes__[MD_VDP_PLANE__BACKGROUND], 800, 100, 1);
        draw_plane__(&planes__[MD_VDP_PLANE__FOREGROUND], 800, 100 + 224, 1);

        if (vdp_window_tex__.id != 0) {
            draw_tiles_debug(32, 560, 120, 1, false);
            //            DrawTextureEx(vdp_window_tex__, (Vector2){32, 32}, 0, 1, WHITE);
        }
    }
    EndShaderMode();

    draw_tiles_debug(32, 560, 120, 1, true);

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

void md_vdp__set_window_horizontal_position(MdVdpWindowDirection dir, u8 units) {
    NOT_IMPLEMENTED
}

void md_vdp__set_window_vertical_position(MdVdpWindowDirection dir, u8 units) {
    NOT_IMPLEMENTED
}

const MdMemoryVram* md_vdp__dma_begin() {
    static MutableByteArray n = {0, 0};

    static MutableByteArray s = {sprite_table__, sizeof(sprite_table__) / sizeof(sprite_table__[0])};

    ///////////////////////////////////////////////

    static const MdMemoryVram vram = {&n, &n, &n, .plane_sprite_mut = &s};

    return &vram;
}

void md_vdp__dma_end() {
    NOT_IMPLEMENTED
}
