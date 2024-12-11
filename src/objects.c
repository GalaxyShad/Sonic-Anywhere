#include "objects.h"

#include "include_backend/tinyint.h"
#include "include_backend/debug.h"
#include "sprite_buffer.h"

#include "object_map.h"

#define OBJ_PROPS_SIZE 0x80

static SObjectProps object_props_space__[OBJ_PROPS_SIZE];

static void draw_sprite(GameSprite* sprite, i16 x, i16 y, bool flip_x, bool flip_y) {
    for (int i = 0; i < sprite->pieces_len; i++) {
        s_sprite_buffer__add(&sprite->pieces[i], x, y);
    }
}

void s_object_pool__build_sprites() {
    // BuildSprites:

    // TODO priority

    int sprite_count = 0;
    for (int i = 0; i < sizeof(object_props_space__) / sizeof(object_props_space__[0]); i++) {
        SObjectProps* obj_props = &object_props_space__[i];

        if (obj_props->id == 0)
            continue;

        obj_props->render.is_on_screen = 0;

        if (obj_props->render.use_relative == 0) {
            // TODO BldSpr_ScrPos(pc,d0.w),a1
        }

        GameSprite* spr = &obj_props->mapping->sprites[obj_props->frame];

        draw_sprite(spr, obj_props->x.px, obj_props->screen_y, 0, 0);
//        obj_props->frame

        if (sprite_count >= 0x50) {
            LOG_WARN("Sprite limit reached %d", 0x50)
        }

        obj_props->render.is_on_screen = 1;

        sprite_count++;
    }
}

void s_object_pool__all_execute() {
    // ExecuteObjects:
    for (int i = 0; i < sizeof(object_props_space__) / sizeof(object_props_space__[0]); i++) {
        SObjectProps* obj_props = &object_props_space__[i];

        if (obj_props->id == 0)
            continue;

        RoutinePtr obj_routine = S_OBJECT_ROUTINE_MAPPINGS[obj_props->id];

        ASSERT(obj_routine != 0)

        obj_routine(obj_props);
    }
}

void s_object_pool__all_clear_props() {
    // FIXME
    u8* mem = (u8*)&object_props_space__[0];

    for (size i = 0; i < OBJ_PROPS_SIZE * sizeof(object_props_space__[0]); i++) {
        mem[i] = 0;
    }
}

void s_object_pool__load(SObjectID object_id, int object_space) {
    ASSERT(object_id >= 0 && object_id < S_OBJECT_ID__LENGTH__)
    ASSERT(object_space >= 0 && object_space < OBJ_PROPS_SIZE)

    object_props_space__[object_space].id = object_id;
}
