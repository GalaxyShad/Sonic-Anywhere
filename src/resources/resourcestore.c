#include "resourcestore.h"

#include "include_backend/debug.h"

#include "src_s1disasm_assets/index.h"

ReadonlyByteArray resource_store__get(ResourceID res_id) {
#define $MAP(X)                          \
    (ReadonlyByteArray) {             \
        X, (sizeof(X) / sizeof(X[0])) \
    }

    // clang-format off
    switch (res_id) {
    case RESOURCE__PALETTE__SEGA1_BIN: return $MAP(S1DISASM__PALETTE__SEGA1_BIN_BODY);
    case RESOURCE__PALETTE__SEGA2_BIN: return $MAP(S1DISASM__PALETTE__SEGA2_BIN_BODY);

    case RESOURCE__TILEMAPS__SEGA_LOGO_ENI: return $MAP(S1DISASM__TILEMAPS__SEGA_LOGO_ENI_BODY);
    case RESOURCE__TILEMAPS__SEGA_LOGO_JP1_ENI: return $MAP(S1DISASM__TILEMAPS__SEGA_LOGO_JP1_ENI_BODY);

    case RESOURCE__ARTNEM__SEGA_LOGO_NEM: return $MAP(S1DISASM__ARTNEM__SEGA_LOGO_NEM_BODY);
    case RESOURCE__ARTNEM__SEGA_LOGO_JP1_NEM: return $MAP(S1DISASM__ARTNEM__SEGA_LOGO_JP1_NEM_BODY);

    default:
        LOG_ERROR("[Resource Store] No res_id DEC: %d HEX: 0x%04X implemented", res_id, res_id)
        return $MAP(S1DISASM__PALETTE__SEGA1_BIN_BODY);
    }
    // clang-format on

#undef $MAP //(X)
}
