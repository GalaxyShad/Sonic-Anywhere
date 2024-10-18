#include "../compressors.h"

#include "include_backend/debug.h"

u8 utils__lshift_bits(const u8* src, size src_size, size shift) {
    size i = shift / 8;

    u8 a = shift % 8;
    u8 b = 8 - a;

    u8 curr = (i >= src_size) ? 0 : src[i];
    u8 next = ((i + 1) >= src_size) ? 0 : src[i +1];

    return (curr << a) | (next >> b);
}

// https://www.segaretro.org/Enigma_compression
void compressors__enigma_decompress(const u8* src, size src_size, u8* dst, size dst_size, u16 starting_art_tile) {
    //TODO starting_art_tile

    u8* header = src;

    u8 number_of_bits_inline_value = header[0]; // length of the inline render flags bitfield

    // clang-format off
    u8 bit_field = header[1] & 0x1F; // 000PCCVH

    u8 priority_flag        = bit_field & 0b10000;
    u8 base_pal_line_add    = bit_field & 0b01100;
    u8 vflip                = bit_field & 0b00010;
    u8 hflip                = bit_field & 0b00001;
    // clang-format on

    u8 bit_field_count = 0;

    u8 mask_f_priority = 0b0;
    if (priority_flag) {
        mask_f_priority = 0b1;
        bit_field_count++;
    }

    u8 mask_f_pal_line_add = 0b0;
    if (base_pal_line_add) {
        mask_f_priority <<= 2;
        mask_f_pal_line_add = 0b11;
        bit_field_count += 2;
    }

    u8 mask_f_vflip = 0b0;
    if (vflip) {
        mask_f_priority <<= 1;
        mask_f_pal_line_add <<= 1;
        mask_f_vflip = 0b1;
        bit_field_count++;
    }

    u8 mask_f_hflip = 0b0;
    if (hflip) {
        mask_f_priority <<= 1;
        mask_f_pal_line_add <<= 1;
        mask_f_vflip <<= 1;
        mask_f_hflip = 0b1;
        bit_field_count++;
    }

    u16 incremental_copy_word = (header[2] << 8) | header[3];
    u16 literal_copy_word = (header[4] << 8) | header[5];

    const u8* format_list = src + 6;

    u16 dst_i = 0;

    int bit_shift = 0;

    while (1) {
        u8 entry = utils__lshift_bits(format_list, src_size - 6, bit_shift);

        if ((entry & 0x80) == 0) {
            entry >>= 2;

            u8 type = (entry & 0b110000) >> 4;
            u8 count = (entry & 0b001111) >> 0;

            u8 use_incremental = type == 0b00;

            for (int i = 0; i < count + 1; i++) {
                if (dst_i >= dst_size)
                    return;

                u16 word_to_copy = use_incremental ? incremental_copy_word : literal_copy_word;

                dst[dst_i + 0]  = (word_to_copy & 0xFF00) >> 8;
                dst[dst_i + 1]  = (word_to_copy & 0x00FF) >> 0;

                if (use_incremental)
                    incremental_copy_word++;

                dst_i += 2;
            }

            bit_shift += 6;
        } else {
            entry >>= 1;

            u8 type = (entry & 0b1110000) >> 4;
            u8 count = (entry & 0b0001111) >> 0;

            if (type == 0b111 && count == 0xF)
                return;

            bit_shift += 7;

            u16 inline_value = 0;
            for (int i = 0; i < count + 1; i++) {
                if (dst_i >= dst_size)
                    return;

                // When the type is 100, 101 or 110,
                //      the repeat count bits will be followed by a
                //      single inline render flags bitfield and copy value pair.
                //
                // When the type is 111, the repeat count bits will be followed by
                //      repeat count + 1 inline render flags bitfield and copy value pairs.
                if ((i == 0) || (type == 0b111)) {
                    entry = utils__lshift_bits(format_list, src_size - 6, bit_shift);

                    // The length of the inline render flags bitfield in bits is the same as
                    // the number of bits set in byte 1 of the header.
                    u16 a = entry >> (8 - bit_field_count);

                    // This bitfield is used in conjunction with byte 1 of the header to determine
                    // the upper five bits of the inline copy value.
                    u16 flags = 0;

                    // As an example, if byte 1 is $13 (%0001_0011), it indicates that the
                    // high priority, vertical flip and horizontal flip flags may be set,
                    // and therefore the inline render flags bitfield will be three bits long.
                    //
                    // If the inline render flags bitfield is %001 only the horizontal flip flag will be set,
                    // if it's %010, only the vertical flip flag will be set,
                    // if it's %011, both flip flags will be set, and so on and so forth.
                    flags |= (a & mask_f_priority) ? priority_flag : 0;
                    flags |= (a & mask_f_pal_line_add)
                               ? ((a & mask_f_pal_line_add) >> ((mask_f_vflip != 0) + (mask_f_hflip != 0))) << 2
                               : 0;
                    flags |= (a & mask_f_vflip) ? vflip : 0;
                    flags |= (a & mask_f_hflip) ? hflip : 0;

                    flags <<= (8 + 3);

                    // After this bitfield is the actual copy value, the length (in bits)
                    // of which is determined by byte 0 of the header.
                    bit_shift += bit_field_count;
                    entry = utils__lshift_bits(format_list, src_size - 6, bit_shift);

                    if (number_of_bits_inline_value <= 8) {
                        inline_value = entry >> (8 - number_of_bits_inline_value);
                        bit_shift += number_of_bits_inline_value;
                    } else {
                        inline_value = entry << (number_of_bits_inline_value % 8);
                        bit_shift += 8;
                        entry = utils__lshift_bits(format_list, src_size - 6, bit_shift);
                        inline_value |= (entry >> (8 - (number_of_bits_inline_value % 8)));
                        bit_shift += (number_of_bits_inline_value % 8);
                    }

                    // This copy value is combined with its render bits to form the final copy value.
                    inline_value |= flags;
                }

                dst[dst_i + 0] = (inline_value & 0xFF00) >> 8;
                dst[dst_i + 1] = (inline_value & 0x00FF) >> 0;

                if (type == 0b101) {
                    inline_value++;
                } else if (type == 0b110) {
                    inline_value--;
                }

                dst_i += 2;
            }
        }
    }
}

void compressors__enigma_decompress_byte_arr(const ReadonlyByteArray* src, const MutableByteArray* dst, u16 starting_art_tile) {
    compressors__enigma_decompress(src->arr, src->size, dst->arr, dst->size, starting_art_tile);
}