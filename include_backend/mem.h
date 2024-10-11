#ifndef SONIC_ANYWHERE_MEM_H
#define SONIC_ANYWHERE_MEM_H

#include "src/bytearray.h"

const MutableByteArray* mem__vram_foreground();
const MutableByteArray* mem__vram_background();
const MutableByteArray* mem__vram_sprites();

const MutableByteArray* mem__chunks(); // 256x256 tile mappings ($52 chunks)
MutableByteArray mem__chunks_shifted(int shift); // 256x256 tile mappings ($52 chunks)

#endif // SONIC_ANYWHERE_MEM_H
