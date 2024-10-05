#include "game.h"

#include "include_backend/interrupt.h"
#include "include_backend/plc.h"
#include "include_backend/tinyint.h"
#include "include_backend/vpu.h"

#include "../resources/resourcestore.h"

#include "src/audio.h"

// ---------------------------------------------------------------------------
//  Sega screen
// ---------------------------------------------------------------------------

static u16 palette_fade_start__;
static u8 palette_fade_size__;

static u8 v_vbla_routine__;
static u8 palette__[16];


static void fadeout_to_black__() {
    //    FadeOut_ToBlack:
    vpu_palette__foreach(PALETTE_ID_MAIN, mdcolor__fade_black);
    vpu_palette__foreach(PALETTE_ID_WATER, mdcolor__fade_black);
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

void game_mode_sega() {
    // GM_Sega
    audio__stop_sounds();
    plc__clear();
    palette_fadeout__();

    vpu__set_color_mode(VPU_COLOR_MODE_8_COLOR);
    //    TODO move.w	#$8200+(vram_fg>>10),(a6) ; set foreground nametable address
    //    TODO move.w	#$8400+(vram_bg>>13),(a6) ; set background nametable address

    MDColor bg_color = vpu_palette__first_color();
    vpu__set_background_color(bg_color);

    //    TODO 	move.w	#$8B00,(a6)	; full-screen vertical scrolling

    vpu_palette__set_water_state(VPU_PALETTE_WATER_STATE__DRY_OR_PARTIALLY);

    DISABLE_INTERRUPTS();

    //    	move.w	(v_vdp_buffer1).w,d0
    //		andi.b	#$BF,d0
    //		move.w	d0,(vdp_control_port).l

    vpu__clear_screen();

    ReadonlyByteArray sega_logo_patterns_nem = resource_store__get(RESOURCE__ARTNEM__SEGA_LOGO_NEM);
    ReadonlyByteArray sega_logo_mappings = resource_store__get(RESOURCE__TILEMAPS__SEGA_LOGO_ENI);


}