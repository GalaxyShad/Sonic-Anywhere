#include "test_framework.h"

#include <stdio.h>
#include <stdarg.h>

void test_assert_array_equals__(
  const unsigned char* a, unsigned long long int a_size, const unsigned char* b, unsigned long long int b_size,
  const char* fn, const char* file, int line
) {
    if (a_size != b_size) {
        printf(FAIL_MSG "[%s] array sizes are different | ACTUAL: %llu | EXPECTED: %llu || FILE %s | LINE %d\n", fn, a_size, b_size, file, line);
        return;
    }

    for (int i = 0; i < a_size; i++) {
        if (a[i] != b[i]) {
            printf(
              FAIL_MSG "[%s] INDEX: %d | ACTUAL: (%d, 0x%X) | EXPECTED: (%d, 0x%X) || FILE %s | LINE %d\n", fn, i, a[i],
              a[i], b[i], b[i], file, line
            );
            return;
        }
    }

    printf(OK_MSG "[%s] FILE %s ; LINE %d\n", fn, file, line);
}
void test_print__(const char* format, ...) {
    va_list args;
    int result;

    // Initialize the argument list
    va_start(args, format);

    // Call the original printf function with the variable arguments
    result = vprintf(format, args);

    // Clean up the argument list
    va_end(args);
}
