#include "../compressors.h"
#include "include_backend/debug.h"

static u8 read_kosinski_byte(u8 byte) {
    u8 reverse_byte = byte;
    reverse_byte = ((reverse_byte & 0x55) << 1) | ((reverse_byte & 0xAA) >> 1);
    reverse_byte = ((reverse_byte & 0x33) << 2) | ((reverse_byte & 0xCC) >> 2);
    reverse_byte = ((reverse_byte & 0x0F) << 4) | ((reverse_byte & 0xF0) >> 4);
    return reverse_byte;
}

void compressors__kosinski_decompress(const u8* src, size src_size, u8* dst, size dst_size) {
    u64 r_byte_index = 0;
    u64 w_byte_index = 0;
    u32 description_word = 0;
    i64 description_index = 0;

    while (r_byte_index < src_size - 3) {
        description_word &= 0x0F;
        description_word <<= 16;
        if (r_byte_index > src_size - 2) {
            LOG_ERROR("r_byte_index(%lld) out of range of src src_size(%lld).", r_byte_index, src_size);
            return;
        }
        description_word |= (read_kosinski_byte(src[r_byte_index]) << 8) | (read_kosinski_byte(src[r_byte_index + 1]));
        description_index += 16;
        r_byte_index += 2;



        while (w_byte_index < dst_size) {
            if (((description_word >> (description_index - 1)) & 0b1) == 0b1) {
                if (description_index < 2) {
                    break;
                }
                if (r_byte_index > src_size - 1) {
                    LOG_ERROR("r_byte_index(%lld) out of range of src src_size(%lld).", r_byte_index, src_size);
                    return;
                }
                if (w_byte_index > dst_size) {
                    LOG_ERROR("w_byte_index(%lld) out of range of dst dst_size(%lld).", w_byte_index, dst_size);
                    return;
                }

                dst[w_byte_index] = src[r_byte_index];
                w_byte_index++;
                r_byte_index++;

                description_index--;
            } else if (((description_word >> (description_index - 2)) & 0b11) == 0b00) {
                if (description_index < 5) {
                    break;
                }
                if (r_byte_index > src_size - 1) {
                    LOG_ERROR("r_byte_index(%lld) out of range of src src_size(%lld).", r_byte_index, src_size);
                    return;
                }
                if (w_byte_index > dst_size) {
                    LOG_ERROR("w_byte_index(%lld) out of range of dst dst_size(%lld).", w_byte_index, dst_size);
                    return;
                }

                i32 countCopyByte = ((description_word >> (description_index - 4)) & 0b11) + 2;
                i32 offsetByte = src[r_byte_index] - 0x100;
                r_byte_index++;

                while (countCopyByte > 0) {
                    dst[w_byte_index] = dst[w_byte_index + offsetByte];
                    w_byte_index++;
                    countCopyByte--;
                }

                description_index -= 4;
            } else if (((description_word >> (description_index - 2)) & 0b11) == 0b01) {
                if (description_index < 3) {
                    break;
                }
                if (r_byte_index > src_size - 3) {
                    LOG_ERROR("r_byte_index(%lld) out of range of src src_size(%lld).", r_byte_index, src_size);
                    return;
                }
                if (w_byte_index > dst_size) {
                    LOG_ERROR("w_byte_index(%lld) out of range of dst dst_size(%lld).", w_byte_index, dst_size);
                    return;
                }

                i32 offsetByte = (-0x2000) + (src[r_byte_index]) + (((src[r_byte_index + 1] >> 3) & 0x1F) * 256);
                i32 countCopyByte = 0;
                if ((src[r_byte_index + 1] & 0b111) != 0) {
                    countCopyByte = (src[r_byte_index + 1] & 0b111) + 2;
                    r_byte_index += 2;
                } else {
                    countCopyByte = src[r_byte_index + 2] + 1;
                    r_byte_index += 3;
                    if ((countCopyByte) == 1) {
                        description_word = 0;
                        description_index = 0;
                        break;
                    }
                    if ((countCopyByte) == 0) {

                        return;
                    }
                }

                while (countCopyByte > 0) {
                    dst[w_byte_index] = dst[w_byte_index + offsetByte];
                    w_byte_index++;
                    countCopyByte--;
                }

                description_index -= 2;
            }
        }
    }
}
