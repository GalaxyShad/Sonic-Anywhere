#include "game.h"

#include "include_backend/input.h"
#include "include_backend/interrupt.h"
#include "include_backend/plc.h"
#include "include_backend/tinyint.h"
#include "include_backend/vpu.h"

#include "../compressors.h"

#include "../resources/resourcestore.h"

#include "include_backend/audio.h"
#include "src/gamevdp.h"

// ---------------------------------------------------------------------------
//  Sega screen
// ---------------------------------------------------------------------------

static u16 palette_fade_start__;
static u8 palette_fade_size__;

static u8 v_vbla_routine__;
static u8 palette__[16];


static void fadeout_to_black__() {
    //    FadeOut_ToBlack:
    vdp_palette__foreach(PALETTE_ID_MAIN, mdcolor__fade_black);
    vdp_palette__foreach(PALETTE_ID_WATER, mdcolor__fade_black);
}

static void palette_fadeout__() {
    //    PaletteFadeOut
    palette_fade_start__ = 0x003F;

    for (u16 i = 0; i < 0x15; i++) {
        v_vbla_routine__ = 0x12;

        vpu__sleep_until_vblank();
        fadeout_to_black__();
        plc__run();
    }
}

static u16 palette_cycle_sega__() {
    // TODO Direct ASM translation. Function needs refactoring

    static u16 v_pcyc_time = 0;
    static u16 v_pcyc_num = 0;

    if (v_pcyc_time == 0) {
        // a1
        u16 pal_shift = 0x20;
        // a0
        ReadonlyByteArray pal_sega1 = resource_store__get(RESOURCE__PALETTE__SEGA1_BIN);
        u8* pal_sega_ptr = pal_sega1.arr;

        i16 d1 = 5;
        u16 d0 = v_pcyc_num;

        // loc_2020:
        while (d0 < 0) {
            pal_sega_ptr += 2;
            d1 -= 1;
            d0 += 2;
        }

        // loc_202A:
        u16 d2 = d0 & 0x1E;
        if (d2 != 0) {
            d0 += 2;
        }

        // loc_2034:
        if (d0 < 0x60) {
            u16 b = pal_sega_ptr[1];
            u16 col = (pal_sega_ptr[0] << 8) | b;

            vpu_palette__set_color(d0 + pal_shift, col);
        }

        // loc_203E:
        do {
            d0 += 2;
            d1--;
        } while (d1 != -1);

        d2 = (v_pcyc_num + 2) & 0x1E;
        if (d2 != 0) {
            d0 += 2;
        }

        // loc_2054:
        if (d0 >= 0x64) {
            v_pcyc_time = 0x401;
            d0 = -0xC;
        }

        // loc_2062:
        v_pcyc_num = d0;
        d0 = 1;

        return d0;
    }

    // loc_206A:
    v_pcyc_time--;

    if (v_pcyc_time < 0) {
        u16 d0 = 0;

        v_pcyc_time = 4;
        v_pcyc_num += 0x0C;

        if (v_pcyc_num >= 0x30) {
            d0 = 0;
            return d0;
        }

        // loc_2088:
        v_pcyc_num = d0;

        // a0
        u16* pal_sega_2 = resource_store__get(RESOURCE__PALETTE__SEGA2_BIN).arr;
        pal_sega_2 += d0;

        // a1
        u16 pal_shift = 4;

        for (int i = 0; i < 6; i++) {
            vpu_palette__set_color(pal_shift + d0 + i, pal_sega_2[i]);
        }

        d0 = 0;
        i16 d1 = 0x2C;

        // loc_20A8:
        u16 d2 = d0 & 0x1E;
        if (d2 != 0) {
            d0 += 2;
        }

        // loc_20B2:
        do {
            vpu_palette__set_color(pal_shift + d0, *pal_sega_2);
            d0 += 2;
            d1--;
        } while (d1 != -1);
    }

    // loc_20BC:
    u16 d0 = 0;
    return d0;
}

void game_mode_sega() {
    // GM_Sega
    audio__stop_sounds();
    plc__clear();
    palette_fadeout__();

#define VRAM_FG 0xC000
#define VRAM_BG 0xE000

    vpu__set_color_mode(VPU_COLOR_MODE_8_COLOR);
    vpu__set_address_for_layer(VPU_LAYER__FOREGROUND, VRAM_FG);
    vpu__set_address_for_layer(VPU_LAYER__BACKGROUND, VRAM_BG);
    vpu__set_background_color(0, 0);
    vpu__set_scrolling_mode(VPU_VSCROLL_MODE__FULL_SCROLL, VPU_HSCROLL_MODE__FULL_SCROLL, 0);

    game_vdp__set_palette_water_state(GAME_VDP_PALETTE_WATER_STATE__DRY_OR_PARTIALLY);

    DISABLE_INTERRUPTS();

    //    	move.w	(v_vdp_buffer1).w,d0
    //		andi.b	#$BF,d0
    //		move.w	d0,(vdp_control_port).l

    vpu__clear_screen();

    int use_japanese_logo = 0; // TODO

    ReadonlyByteArray sega_logo_patterns_nem =
      resource_store__get(use_japanese_logo ? RESOURCE__ARTNEM__SEGA_LOGO_JP1_NEM : RESOURCE__ARTNEM__SEGA_LOGO_NEM);

    vpu__set_window(sega_logo_patterns_nem.arr, sega_logo_patterns_nem.size);

//    compressors__nemesis_decompress(
//      sega_logo_patterns_nem.arr, sega_logo_patterns_nem.size, vpu__get_mutable_direct_memory(), 5024
//    );

    ReadonlyByteArray sega_logo_mappings = resource_store__get(
      use_japanese_logo ? RESOURCE__TILEMAPS__SEGA_LOGO_JP1_ENI : RESOURCE__TILEMAPS__SEGA_LOGO_ENI
    );
    u8* buffer = vpu__get_mutable_memory_256x256_tile_mappings();
    compressors__enigma_decompress(sega_logo_mappings.arr, sega_logo_mappings.size, buffer, 256*256, 0);

    ReadonlyByteArray buff_arr = {buffer, 256 * 256};
    vpu__copy_tilemap_to_layer_r(VPU_LAYER__BACKGROUND, 510, &buff_arr, 24, 8);

    ReadonlyByteArray buff_arr2 = {buffer + (24 * 8 * 2), 256 * 256};
    vpu__copy_tilemap_to_layer_r(VPU_LAYER__FOREGROUND, 0, &buff_arr2, 40, 28);

    // Decided to apply revision 1 fix
    if (use_japanese_logo) {
        ReadonlyByteArray white_rect = {buffer + 0xA40, 256 * 256};
        vpu__copy_tilemap_to_layer_r(VPU_LAYER__FOREGROUND, 0x53A, &white_rect, 3, 2);
    }

    // .loadpal
    u16 palette_cycling_current_ref_num = -10;
    u16 palette_cycling_time = 0;
    u16 palette_data_buffer[24] = {0};

    palette_data_buffer[0x12] = 0;
    palette_data_buffer[0x10] = 0;

    //      move.w	(v_vdp_buffer1).w,d0
    //		ori.b	#$40,d0
    //		move.w	d0,(vdp_control_port).l

    // Sega_WaitPal:
    do {
        //    move.b	#2,(v_vbla_routine).w
        vpu__sleep_until_vblank();
        game_vdp__load_palette(GAME_VDP_PALETTE_ID__SEGA_LOGO);
    } while (palette_cycle_sega__() != 0);

    audio__play_sound_special(SND__SEGA);
    //  move.b	#$14,(v_vbla_routine).w
    vpu__sleep_until_vblank();
    u16 demo_length = 0x1E;

    //    Sega_WaitEnd:
    while (1) {
        //    move.b	#2,(v_vbla_routine).w
        vpu__sleep_until_vblank();

        if ((demo_length == 0) || input__is_btn_pressed(BUTTON_CODE__START)) {
            // Sega_GotoTitle:
            game__load_game_mode(GM_TITLE);
            return;
        }
    }
}