#ifndef SONIC_ANYWHERE_VPU_H
#define SONIC_ANYWHERE_VPU_H

#include "mdcolor.h"

typedef enum PaletteID {
    PALETTE_ID_MAIN,
    PALETTE_ID_WATER
} PaletteID;

MDColor vpu_palette__first_color();
void vpu_palette__foreach(PaletteID pal_id, MDColor (*func)(MDColor));

typedef enum VpuPaletteWaterState {
    VPU_PALETTE_WATER_STATE__DRY_OR_PARTIALLY,
    VPU_PALETTE_WATER_STATE__ALL_UNDERWATER
} VpuPaletteWaterState;

void vpu_palette__set_water_state(VpuPaletteWaterState water_state);

void vpu__sleep_until_vblank();

typedef enum VpuColorMode {
    VPU_COLOR_MODE_8_COLOR
} VpuColorMode;

void vpu__set_color_mode(VpuColorMode mode);
void vpu__set_background_color(MDColor bg_color);
void vpu__clear_screen();



#endif // SONIC_ANYWHERE_VPU_H
