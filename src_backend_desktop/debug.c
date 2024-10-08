#include "include_backend/debug.h"

#include <stdio.h>
#include <stdarg.h>

void assert__(const char* msg, const char* file, int line) {
    printf("[ASSERT__ERR] LINE: %d, FILE: %s, REASON: %s\n", line, file, msg);
}

void not_implemented__(const char* function, const char* file, int line) {
    printf("[ASSERT_ERR] function [%s] not implemented! LINE: %d, FILE: %s\n", function, line, file);
}

void log__(const char* function, const char* file, int line, int level, const char* format, ...) {
    switch (level) {
    case 0: printf("\033[0;00m" "[LOG      ] "); break;
    case 1: printf("\033[0;33m" "[LOG_WARN ] "); break;
    case 2: printf("\033[0;31m" "[LOG_ERROR] "); break;
    }

    printf("\033[0;0m");

    va_list args;
    int result;

    // Initialize the argument list
    va_start(args, format);

    // Call the original printf function with the variable arguments
    result = vprintf(format, args);

    // Clean up the argument list
    va_end(args);

    printf("\n");
}