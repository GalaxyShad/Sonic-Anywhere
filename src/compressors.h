#ifndef SONIC_ANYWHERE_COMPRESSORS_H
#define SONIC_ANYWHERE_COMPRESSORS_H

#include "bytearray.h"

void compressors__kosinski_decompress(const u8* src, size src_size, u8* dst, size dst_size);
void compressors__enigma_decompress(const u8* src, size src_size, u8* dst, size dst_size);
void compressors__nemesis_decompress(const u8* src, size src_size, u8* dst, size dst_size);

#endif // SONIC_ANYWHERE_COMPRESSORS_H
