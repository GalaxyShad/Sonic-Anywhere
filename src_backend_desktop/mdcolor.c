#include "include_backend/mdcolor.h"

MDColor mdcolor__fade_black(MDColor color) {
    if (color == 0x0000)
        return color;

    for (u16 c = 0; c != 24; c += 8) {
        color -= (0b0010 << c) * ((color & (0b1110 << c)) != 0);
    }

    return color;
}