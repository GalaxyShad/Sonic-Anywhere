#include "include_backend/mdmem.h"

#include "include_backend/debug.h"

#define PLANE_SIZE 0x2000

const MutableByteArray* mem__vram_foreground() {
    static u8 mem_foreground__[PLANE_SIZE];

    static MutableByteArray byte_arr_foreground = {
      mem_foreground__, PLANE_SIZE,
    };

    return &byte_arr_foreground;
}

const MutableByteArray* mem__vram_background() {
    static u8 mem_background__[PLANE_SIZE];

    static MutableByteArray byte_arr_background = {
      mem_background__, PLANE_SIZE
    };

    return &byte_arr_background;
}

const MutableByteArray* mem__vram_window() {
    static u8 mem_window__[PLANE_SIZE];

    static MutableByteArray byte_arr_window = {
      mem_window__, PLANE_SIZE
    };

    return &byte_arr_window;
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
    NOT_IMPLEMENTED
}

