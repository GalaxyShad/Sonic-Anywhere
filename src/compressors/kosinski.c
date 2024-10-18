#include "../compressors.h"
#include "include_backend/debug.h"

static u8 expand_description_byte__(u8 byte) {
    u8 reverse_byte = byte;
    reverse_byte = ((reverse_byte & 0x55) << 1) | ((reverse_byte & 0xAA) >> 1);
    reverse_byte = ((reverse_byte & 0x33) << 2) | ((reverse_byte & 0xCC) >> 2);
    reverse_byte = ((reverse_byte & 0x0F) << 4) | ((reverse_byte & 0xF0) >> 4);
    return reverse_byte;
}

static u8 check_out_of_range(const char* name_arr_1, u64 index_arr_1, const char* name_arr_2, u64 index_arr_2) {
    if (index_arr_1 > index_arr_2) {
        LOG_ERROR(
          "%s(%lld) out of range of %s %s_size(%lld).", name_arr_1, index_arr_1, name_arr_2, name_arr_2, index_arr_2
        );
        return 1;
    }
    return 0;
}

typedef struct State {
    u64 r_byte_index;
    u64 w_byte_index;
    u32 description_word;
    i64 description_length;

    const u8* src;
    size src_size;
    u8* dst;
    size dst_size;
} State;

static void uncompressed_data__(State* state) {
    if (check_out_of_range("r_byte_index", state->r_byte_index, "src", state->src_size))
        return;

    if (check_out_of_range("w_byte_index", state->w_byte_index, "dst", state->dst_size))
        return;

    state->dst[state->w_byte_index] = state->src[state->r_byte_index];
    state->w_byte_index++;
    state->r_byte_index++;

    state->description_length--;
};

static void inline_dictionary_match__(State* state) {
    if (check_out_of_range("r_byte_index", state->r_byte_index, "src", state->src_size))
        return;

    i32 count_copy_byte = ((state->description_word >> (state->description_length - 4)) & 0b11) + 2;
    i32 offset_byte = state->src[state->r_byte_index] - 0x100;
    state->r_byte_index++;

    if (check_out_of_range("w_byte_index", state->w_byte_index + count_copy_byte, "dst", state->dst_size))
        return;

    while (count_copy_byte > 0) {
        state->dst[state->w_byte_index] = state->dst[state->w_byte_index + offset_byte];
        state->w_byte_index++;
        count_copy_byte--;
    }

    state->description_length -= 4;
};

static void full_dictionary_matches__(State* state) {
    if (check_out_of_range("r_byte_index", state->r_byte_index + 3, "src", state->src_size))
        return;

    i32 offset_byte =
      (-0x2000) + (state->src[state->r_byte_index]) + (((state->src[state->r_byte_index + 1] >> 3) & 0x1F) * 256);
    i32 count_copy_byte = 0;
    if ((state->src[state->r_byte_index + 1] & 0b111) != 0) {
        count_copy_byte = (state->src[state->r_byte_index + 1] & 0b111) + 2;
        state->r_byte_index += 2;
    } else {
        count_copy_byte = state->src[state->r_byte_index + 2] + 1;
        state->r_byte_index += 3;
        if ((count_copy_byte) == 1) {
            state->description_word = 0;
            state->description_length = 0;
            return;
        }
        if ((count_copy_byte) == 0) {
            return;
        }
    }

    if (check_out_of_range("w_byte_index", state->w_byte_index + count_copy_byte, "dst", state->dst_size))
        return;

    while (count_copy_byte > 0) {
        state->dst[state->w_byte_index] = state->dst[state->w_byte_index + offset_byte];
        state->w_byte_index++;
        count_copy_byte--;
    }

    state->description_length -= 2;
};

void compressors__kosinski_decompress(const u8* src, size src_size, u8* dst, size dst_size) {
    State state = {0};
    state.dst = dst;
    state.dst_size = dst_size;
    state.src = src;
    state.src_size = src_size;

    while (state.r_byte_index < src_size) {
        if (check_out_of_range("r_byte_index", state.r_byte_index + 1, "src", src_size))
            return;
        state.description_word = (state.description_word & 0x0F) << 16;
        state.description_word |= (expand_description_byte__(src[state.r_byte_index]) << 8) |
                                  (expand_description_byte__(src[state.r_byte_index + 1]));
        state.description_length += 16;
        state.r_byte_index += 2;

        while (state.w_byte_index < dst_size) {
            u8 first_2_bits_of_word = (state.description_word >> (state.description_length - 2)) & 0b11;

            if ((first_2_bits_of_word & 0b10) != 0) {
                if (state.description_length < 2) {
                    break;
                }
                uncompressed_data__(&state);
            } else if (first_2_bits_of_word == 0b00) {
                if (state.description_length < 5) {
                    break;
                }
                inline_dictionary_match__(&state);
            } else {
                if (state.description_length < 3) {
                    break;
                }
                full_dictionary_matches__(&state);
            }
        }
    }
}

void compressors__kosinski_decompress_byte_arr(const ReadonlyByteArray* src, const MutableByteArray* dst) {
    compressors__kosinski_decompress(src->arr, src->size, dst->arr, dst->size);
}
