#include "include_backend/mem.h"

#include "include_backend/debug.h"

const MutableByteArray* mem__vram_foreground() {
    static u8 mem_foreground__[0x2000];

    static MutableByteArray byte_arr_foreground = {
      mem_foreground__, 0x2000,
    };

    return &byte_arr_foreground;
}

const MutableByteArray* mem__vram_background() {
    static u8 mem_background__[0x2000];

    static MutableByteArray byte_arr_background = {
      mem_background__, 0x2000
    };

    return &byte_arr_background;
}

////////////////////////////////////////////////////////////////////////

static u8 chunks[0x52 * 0x200];

static MutableByteArray byte_arr_chunks = {
  chunks, 0x52 * 0x200
};

const MutableByteArray* mem__chunks() {
    return &byte_arr_chunks;
}

MutableByteArray mem__chunks_shifted(int shift) {
    return (MutableByteArray) { chunks + shift, byte_arr_chunks.size - shift };
}

const MutableByteArray* mem__vram_sprites() {
    RAISE_NOT_IMPLEMENTED
}