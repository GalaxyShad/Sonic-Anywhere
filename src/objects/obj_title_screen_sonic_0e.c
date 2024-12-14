#include "src/object_map.h"

#include "src/resources/resourcestore.h"

enum { ROUTINE_MAIN, ROUTINE_DELAY, ROUTINE_MOVE, ROUTINE_ANIMATE };

static void move__(SObjectProps* p) {
    p->screen_y -= 8;

}

static void delay__(SObjectProps* p) {
    if (p->animation.time_to_next_frame != 0) {
        p->animation.time_to_next_frame--;
        return;
    }

    // call DisplaySprite

    p->routine_number = ROUTINE_MOVE;
}

static void main__(SObjectProps* p) {
    p->routine_number = ROUTINE_DELAY;

    p->x.px = 0xF0;
    p->screen_y = 0xDE;

//    p->mapping =

    p->gfx.art_tile_location = 768;

    p->gfx.priority = 1;

    p->animation.time_to_next_frame = 0x29;

    // call AnimateSprite

    delay__(p);
}

void s_obj_routine__title_screen_sonic(SObjectProps* p) {
    switch (p->routine_number) {
    case ROUTINE_MAIN:
        main__(p);
        break;
    }
}