#include "include_backend/mdmem.h"

#include "include_backend/debug.h"

#define PLANE_SIZE 0x2000

// TODO Remove that
const MdMemoryVram* md_mem__vram() {
    static u8 fg[PLANE_SIZE];
    static MutableByteArray barr_fg = {fg, PLANE_SIZE};

    static u8 bg[PLANE_SIZE];
    static MutableByteArray barr_bg = {bg, PLANE_SIZE};

#define N 0x10000
    static u8 window[N];
    static MutableByteArray barr_window = {window, N};

    ///////////////////////////////////////////////

    static const MdMemoryVram vram = {
      &barr_fg,
      &barr_bg,
      &barr_window,
    };

    return &vram;
}

const MdMemory* md_mem() {
    static u8 chunks[0x52 * 0x200];
    static MutableByteArray byte_arr_chunks = {chunks, 0x52 * 0x200};

    static u8 tiles[0x1800];
    static MutableByteArray byte_arr_tiles = { tiles, 0x1800  };

    static const MdMemory mem = {
      &byte_arr_chunks,
      &byte_arr_tiles
    };

    return &mem;
}