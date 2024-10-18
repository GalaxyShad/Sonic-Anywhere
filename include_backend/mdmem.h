//
//  Memory management
//  All game memory allocations (buffers, etc...) should be done in implementation of this
//

#ifndef SONIC_ANYWHERE_MDMEM_H
#define SONIC_ANYWHERE_MDMEM_H

#include "src/bytearray.h"

typedef struct MdMemoryVram {
    const MutableByteArray* plane_foreground_mut;
    const MutableByteArray* plane_background_mut;
    const MutableByteArray* plane_window_mut;
} MdMemoryVram;

const MdMemoryVram* md_mem__vram();

typedef struct MdMemory {
    const MutableByteArray* chunks_mut; // 256x256 tile mappings ($52 chunks)
    const MutableByteArray* tiles_mut;  // 16x16 tile mappings
} MdMemory;

const MdMemory* md_mem();

#endif // SONIC_ANYWHERE_MDMEM_H
