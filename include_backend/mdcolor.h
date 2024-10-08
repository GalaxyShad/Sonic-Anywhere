#ifndef SONIC_ANYWHERE_MDCOLOR_H
#define SONIC_ANYWHERE_MDCOLOR_H

#include "tinyint.h"

// MegaDrive color, 0b0000_RRR0_GGG0_BBB0
typedef unsigned short MDColor;

MDColor mdcolor__fade_black(MDColor color);

#endif // SONIC_ANYWHERE_MDCOLOR_H
