#include "objects.h"

#include "include_backend/tinyint.h"
#include "include_backend/debug.h"

#include "object_map.h"

#define OBJ_PROPS_SIZE 0x80

static SObjectProps object_props_space__[OBJ_PROPS_SIZE];

void s_object_pool__build_sprites() {
    // BuildSprites:

    // TODO priority

    for (int i = 0; i < sizeof(object_props_space__) / sizeof(object_props_space__[0]); i++) {
        SObjectProps* obj_props = &object_props_space__[i];

        if (obj_props->id == 0)
            continue;

        obj_props->render.is_on_screen = 0;
        if (obj_props->render.use_relative == 0) {
            // TODO BldSpr_ScrPos(pc,d0.w),a1
        }




        RoutinePtr obj_routine = S_OBJECT_ROUTINE_MAPPINGS[obj_props->id];

        ASSERT(obj_routine != 0)

        obj_routine(obj_props);
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
