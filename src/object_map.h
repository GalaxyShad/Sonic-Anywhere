#ifndef SONIC_ANYWHERE_OBJECT_MAP_H
#define SONIC_ANYWHERE_OBJECT_MAP_H

#include "objects.h"

void s_obj_routine__credits_text(SObjectProps* p);

static const SObject S_OBJECT_MAPPINGS[] = {
  // Title screen objects
  {.routine = s_obj_routine__credits_text, .props_space_id = 2}
};

#endif // SONIC_ANYWHERE_OBJECT_MAP_H
