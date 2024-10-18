#include "bytearray.h"

inline MutableByteArray bytearray__shift_mut(const MutableByteArray* arr, i32 shift_bytes) {
    return (MutableByteArray){arr->arr + shift_bytes, arr->size - shift_bytes};
}