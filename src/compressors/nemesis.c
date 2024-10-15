// The original code was created by Clownacy and adapted to the current codebase by Adol Eleador II.
// https://github.com/Clownacy/clownnemesis/tree/master

#include "../compressors.h"
#include "include_backend/debug.h"
#define NULL 0
#define MAXIMUM_CODE_BITS 8

static u8 check_out_of_range(const char* name_arr_1, u64 index_arr_1, const char* name_arr_2, u64 index_arr_2) {
    if (index_arr_1 > index_arr_2) {
        LOG_ERROR(
          "%s(%lld) out of range of %s %s_size(%lld).", name_arr_1, index_arr_1, name_arr_2, name_arr_2, index_arr_2
        );
        return 1;
    }
    return 0;
}

typedef struct MemoryStream {
    u64 size;
    u64 rw_index;
    u8* buffer;
} MemoryStream;

typedef struct StateCommon {
    void* read_byte_user_data;
    void* write_byte_user_data;
} StateCommon;

typedef struct NybbleRun {
    u8 total_code_bits;
    u8 value;
    u8 length;
} NybbleRun;

typedef struct State {
    StateCommon common;
    NybbleRun nybble_runs[1 << MAXIMUM_CODE_BITS];
    u64 output_buffer, previous_output_buffer;
    u8 output_buffer_nybbles_done;
    u8 xor_mode_enabled;
    u16 total_tiles;
    u8 bits_available;
    u8 bits_buffer;
} State;

u32 ReadByte(StateCommon* const stateCommon) {
    MemoryStream* const stream = (MemoryStream*) stateCommon->read_byte_user_data;
    if (check_out_of_range("read_byte_user_data", stream->rw_index, "buffer", stream->size))
        return -1;
    const u32 value = stream->buffer[stream->rw_index++];

    return value;
}
static u32 WriteByte(StateCommon* const stateCommon, const u8 byte) {
    MemoryStream* const stream = (MemoryStream*) stateCommon->write_byte_user_data;
    if (check_out_of_range("write_byte_user_data", stream->rw_index, "buffer", stream->size))
        return -1;

    stream->buffer[stream->rw_index++] = byte;
    return byte;
}


//////////////////////////////////////////////


static u32 PopBit(State* const state) {
    state->bits_buffer <<= 1;
    if (state->bits_available == 0) {
        state->bits_available = 8;
        state->bits_buffer = ReadByte(&state->common);
    }
    --state->bits_available;
    return (state->bits_buffer & 0x80) != 0;
}

static u32 PopBits(State* const state, const u32 total_bits) {
    u32 value = 0, i;
    for (i = 0; i < total_bits; ++i) {
        value <<= 1;
        value |= PopBit(state);
    }
    return value;
}

static const NybbleRun* FindCode(State* const state) {
    u32 code = 0, total_code_bits = 0;
    while (1) {
        if (total_code_bits == MAXIMUM_CODE_BITS) {
            LOG_ERROR("Exceeded total_code_bits(%d) max value %d", total_code_bits, MAXIMUM_CODE_BITS);
            return NULL;
        }

        code <<= 1;
        code |= PopBit(state);
        ++total_code_bits;

        if (total_code_bits == 6 && code == 0x3F) {
            return NULL;
        } else {
            const NybbleRun* const nybble_run = &state->nybble_runs[code << (8 - total_code_bits)];
            if ((nybble_run->length != 0 && nybble_run->total_code_bits == total_code_bits))
                return nybble_run;
        }
    }
}

static void OutputNybbles(State* const state, const u32 nybble, const u32 total_nybbles) {
    u32 i;
    for (i = 0; i < total_nybbles; ++i) {
        state->output_buffer <<= 4;
        state->output_buffer |= nybble;

        if ((++state->output_buffer_nybbles_done & 7) == 0) {
            u32 j;
            const u64 final_output =
              state->output_buffer ^ (state->xor_mode_enabled ? state->previous_output_buffer : 0);
            for (j = 0; j < 4; ++j) {
                WriteByte(&state->common, (final_output >> (4 - 1 - j) * 8) & 0xFF);
            }
            state->previous_output_buffer = final_output;
        }
    }
}


//////////////////////////////////////////////


void compressors__nemesis_decompress(const u8* src, size src_size, u8* dst, size dst_size) {
    MemoryStream compressed_memory_stream, decompressed_memory_stream;
    compressed_memory_stream.rw_index = 0;
    compressed_memory_stream.size = src_size;
    compressed_memory_stream.buffer = src;
    decompressed_memory_stream.rw_index = 0;
    decompressed_memory_stream.size = dst_size;
    decompressed_memory_stream.buffer = dst;

    State state = {0};
    state.common.read_byte_user_data = &compressed_memory_stream;
    state.common.write_byte_user_data = &decompressed_memory_stream;

    // Header
    const u32 header_word = (u32) ReadByte(&state.common) << 8 | ReadByte(&state.common);
    u32 i;
    state.xor_mode_enabled = (header_word & 0x8000) != 0;
    state.total_tiles = header_word & 0x7FFF;
    for (i = 0; i < (1 << MAXIMUM_CODE_BITS); i++) {
        state.nybble_runs[i].length = 0;
        state.nybble_runs[i].total_code_bits = 0;
        state.nybble_runs[i].value = 0;
    }


    // CodeTable
    u8 nybble_run_value = 0;
    u8 byte = ReadByte(&state.common);
    while (byte != 0xFF) {
        if ((byte & 0x80) != 0) {
            nybble_run_value = byte & 0xF;
            byte = ReadByte(&state.common);
        } else {
            const u8 run_length = ((byte >> 4) & 7) + 1;
            const u8 total_code_bits = byte & 0xF;
            const u8 code = ReadByte(&state.common);
            const u32 nybble_run_index = (u32) code << (8u - total_code_bits);
            NybbleRun* const nybble_run = &state.nybble_runs[nybble_run_index];

            if (total_code_bits > 8 || total_code_bits == 0 ||
                nybble_run_index > (sizeof(state.nybble_runs) / sizeof((state.nybble_runs)[0]))) {
                LOG_ERROR(
                  "total_code_bits(%d) out of range [1 8] or nybble_run_index(%d) out of range size nybble_runs(%d)",
                  total_code_bits, nybble_run_index, (sizeof(state.nybble_runs) / sizeof((state.nybble_runs)[0]))
                )
            }

            nybble_run->total_code_bits = total_code_bits;
            nybble_run->value = nybble_run_value;
            nybble_run->length = run_length;

            byte = ReadByte(&state.common);
        }
    }


    // Codes
    u64 nybbles_remaining = state.total_tiles * (8 * 8);
    u32 total_runs = 0; //?
    while (nybbles_remaining != 0) {
        /* TODO: Undo this hack! */
        NybbleRun* const nybble_run = (NybbleRun*) FindCode(&state);
        const u32 run_length = nybble_run != NULL ? nybble_run->length : PopBits(&state, 3) + 1;
        const u32 nybble = nybble_run != NULL ? nybble_run->value : PopBits(&state, 4);

        if (nybble_run != NULL)
            ++total_runs;
        if (run_length > nybbles_remaining) {
            LOG_ERROR("run_length(%d) is written to nybbles_remaining(%lld)", run_length, nybbles_remaining);
        }

        OutputNybbles(&state, nybble, run_length);
        nybbles_remaining -= run_length;
    }

    // RAISE_NOT_IMPLEMENTED;
}
