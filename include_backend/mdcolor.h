#ifndef SONIC_ANYWHERE_MDCOLOR_H
#define SONIC_ANYWHERE_MDCOLOR_H

#include "tinyint.h"

// MegaDrive color, 0b0000_RRR0_GGG0_BBB0
typedef unsigned short MDColor;

MDColor mdcolor__fade_black(MDColor color) {
    if (color == 0x0000)
        return color;

    for (u16 c = 0; c != 24; c += 8) {
        color -= (0b0010 << c) * ((color & (0b1110 << c)) != 0);
    }

    return color;
}

#endif // SONIC_ANYWHERE_MDCOLOR_H
