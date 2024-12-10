#ifndef SONIC_ANYWHERE_OBJECT_MAP_H
#define SONIC_ANYWHERE_OBJECT_MAP_H

#include "objects.h"

void s_obj_routine__credits_text(SObjectProps* p);

enum SObjectID {
    S_OBJECT_ID__CREDITS_TEXT = 0x8A,
    S_OBJECT_ID__LENGTH__
};

typedef void (*RoutinePtr)(SObjectProps*);

static const RoutinePtr S_OBJECT_ROUTINE_MAPPINGS[] = {
  // Title screen objects
  [S_OBJECT_ID__CREDITS_TEXT] = s_obj_routine__credits_text
};

#endif // SONIC_ANYWHERE_OBJECT_MAP_H
