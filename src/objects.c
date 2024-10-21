#include "objects.h"

#include "include_backend/tinyint.h"
#include "include_backend/debug.h"

#include "object_map.h"

static SObjectProps object_props_space__[0x80];

void s_object_pool__all_execute() {
    for (int i = 0; i < sizeof(S_OBJECT_MAPPINGS) / sizeof(S_OBJECT_MAPPINGS[0]); i++) {
        ASSERT(S_OBJECT_MAPPINGS[i].props_space_id >= 0 && S_OBJECT_MAPPINGS[i].props_space_id < 0x80)
        ASSERT(S_OBJECT_MAPPINGS[i].routine != 0)

        S_OBJECT_MAPPINGS[i].routine(&object_props_space__[S_OBJECT_MAPPINGS[i].props_space_id]);
    }
}

void s_object_pool__all_clear_props() {
    // FIXME
    u8* mem = (u8*)&object_props_space__[0];

    for (size i = 0; i < 0x80 * sizeof(object_props_space__[0]); i++) {
        mem[i] = 0;
    }
}
