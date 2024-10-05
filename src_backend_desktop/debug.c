#include "include_backend/debug.h"

#include <stdio.h>

void assert__(const char* msg, const char* file, int line) {
    printf("[ASSERT__ERR] LINE: %d, FILE: %s, REASON: %s\n", line, file, msg);
}

void not_implemented__(const char* function, const char* file, int line) {
    printf("[ASSERT_ERR] function [%s] not implemented! LINE: %d, FILE: %s\n", function, line, file);
}
