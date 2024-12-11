#include "fade.h"

#include "gamevdp.h"
#include "plc.h"

static MDColor color_fade_to_black__(MDColor color, MDColor fade_color) {
    if (color == 0x0000)
        return color;

    u8 r = (color >> 1) & 0b111;
    u8 g = (color >> 5) & 0b111;
    u8 b = (color >> 9) & 0b111;

    if (r != 0) {
        r--;
    }

    if (g != 0) {
        g--;
    }

    if (b != 0) {
        b--;
    }

    color = (b << 9) | (g << 5) | (r << 1);

    return color;
}

static MDColor color_fade_from_black__(MDColor color, MDColor fade_color) {
//    if (color == 0x0000)
//        return color;

    u8 r = (color >> 1) & 0b111;
    u8 g = (color >> 5) & 0b111;
    u8 b = (color >> 9) & 0b111;

    u8 fr = (fade_color >> 1) & 0b111;
    u8 fg = (fade_color >> 5) & 0b111;
    u8 fb = (fade_color >> 9) & 0b111;

    if (r != fr) {
        r++;
    }

    if (g != fg) {
        g++;
    }

    if (b != fb) {
        b++;
    }

    color = (b << 9) | (g << 5) | (r << 1);

    return color;
}

void s_fade__in() {
    // PaletteFadeIn
    for (u16 i = 0; i < 0x15; i++) {
        //    FadeIn_FromBlack:
        game_vdp__set_vblank_routine_counter(0x12);
        game_vdp__wait_for_vblank();
        game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__MAIN, color_fade_from_black__);
        game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__WATER, color_fade_from_black__);

        plc__run();
    }
}

void s_fade__out() {
    // PaletteFadeOut
    for (u16 i = 0; i < 0x15; i++) {
        game_vdp__set_vblank_routine_counter(0x12);
        game_vdp__wait_for_vblank();
        //    FadeOut_ToBlack:
        game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__MAIN, color_fade_to_black__);
        game_vdp__palette_foreach(GAME_VDP_PALETTE_LAYER__WATER, color_fade_to_black__);

        plc__run();
    }
}
