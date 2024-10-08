#include "resourcestore.h"

#include "include_backend/debug.h"

static u8 dummy[256];

ReadonlyByteArray resource_store__get(ResourceID res_id) {
    RAISE_NOT_IMPLEMENTED

    ReadonlyByteArray res = {
        dummy, 256
    };

    return res;
}
