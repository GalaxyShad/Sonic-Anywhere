#ifndef SONIC_ANYWHERE_RESOURCESTORE_H
#define SONIC_ANYWHERE_RESOURCESTORE_H

#include "resourceid.h"
#include "../bytearray.h"

// TODO Maybe rename to just "s_resource"
ReadonlyByteArray resource_store__get(ResourceID res_id);

#endif // SONIC_ANYWHERE_RESOURCESTORE_H
