#include "game.h"

#include "include_backend/input.h"
#include "include_backend/interrupt.h"
#include "include_backend/plc.h"
#include "include_backend/tinyint.h"
#include "include_backend/vpu.h"
#include "include_backend/mem.h"

#include "../compressors.h"

#include "../resources/resourcestore.h"

#include "include_backend/audio.h"
#include "src/gamevdp.h"

// ---------------------------------------------------------------------------
//  Sega screen
// ---------------------------------------------------------------------------


static void fadeout_to_black__() {
    //    FadeOut_ToBlack:
    game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__MAIN, mdcolor__fade_black);
    game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__WATER, mdcolor__fade_black);
}

static void palette_fadeout__() {
    //    PaletteFadeOut
    u16 palette_fade_start = 0x003F;

    for (u16 i = 0; i < 0x15; i++) {
        game_vdp__set_vblank_routine_counter(0x12);
        vdp__sleep_until_vblank();
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

            game_vdp__palette_set_color(d0 + pal_shift, col);
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
            game_vdp__palette_set_color(pal_shift + d0 + i, pal_sega_2[i]);
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
            game_vdp__palette_set_color(pal_shift + d0, *pal_sega_2);
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

    vdp__set_color_mode(VDP_COLOR_MODE_8_COLOR);
    vdp__set_address_for_plane(VDP_PLANE__FOREGROUND, mem__vram_foreground());
    vdp__set_address_for_plane(VDP_PLANE__BACKGROUND, mem__vram_background());
    vdp__set_background_color(0, 0);
    vdp__set_scrolling_mode(VDP_VSCROLL_MODE__FULL_SCROLL, VDP_HSCROLL_MODE__FULL_SCROLL, 0);

    game_vdp__set_palette_water_state(GAME_VDP_PALETTE_WATER_STATE__DRY_OR_PARTIALLY);

    DISABLE_INTERRUPTS();

    //    	move.w	(v_vdp_buffer1).w,d0
    //		andi.b	#$BF,d0
    //		move.w	d0,(vdp_control_port).l

    vdp__clear_screen();

    int use_japanese_logo = 0; // TODO

    ReadonlyByteArray sega_logo_patterns_nem =
      resource_store__get(use_japanese_logo ? RESOURCE__ARTNEM__SEGA_LOGO_JP1_NEM : RESOURCE__ARTNEM__SEGA_LOGO_NEM);

    vdp__set_window(sega_logo_patterns_nem.arr, sega_logo_patterns_nem.size);

//    compressors__nemesis_decompress(
//      sega_logo_patterns_nem.arr, sega_logo_patterns_nem.size, vpu__get_mutable_direct_memory(), 5024
//    );

    ReadonlyByteArray sega_logo_mappings = resource_store__get(
      use_japanese_logo ? RESOURCE__TILEMAPS__SEGA_LOGO_JP1_ENI : RESOURCE__TILEMAPS__SEGA_LOGO_ENI
    );

    compressors__enigma_decompress(sega_logo_mappings.arr, sega_logo_mappings.size, mem__chunks()->arr, mem__chunks()->size, 0);

    vdp__copy_tilemap_to_layer_r(VDP_PLANE__BACKGROUND, 510, mem__chunks(), 24, 8);

    MutableByteArray fg = mem__chunks_shifted(24 * 8 * 2);
    vdp__copy_tilemap_to_layer_r(VDP_PLANE__FOREGROUND, 0, &fg, 40, 28);

    // Decided to apply revision 1 fix
    if (use_japanese_logo) {
//        ReadonlyByteArray white_rect = {buffer + 0xA40, 256 * 256};
//        vdp__copy_tilemap_to_layer_r(VDP_PLANE__FOREGROUND, 0x53A, &white_rect, 3, 2);
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
        game_vdp__set_vblank_routine_counter(2);
        vdp__sleep_until_vblank();
        game_vdp__load_palette(GAME_VDP_PALETTE_ID__SEGA_LOGO);
    } while (palette_cycle_sega__() != 0);

    audio__play_sound_special(SND__SEGA);
    game_vdp__set_vblank_routine_counter(0x14);
    vdp__sleep_until_vblank();
    u16 demo_length = 0x1E;

    //    Sega_WaitEnd:
    while (1) {
        game_vdp__set_vblank_routine_counter(2);
        vdp__sleep_until_vblank();

        if ((demo_length == 0) || input__is_btn_pressed(BUTTON_CODE__START)) {
            // Sega_GotoTitle:
            game__load_game_mode(GM_TITLE);
            return;
        }
    }
}

