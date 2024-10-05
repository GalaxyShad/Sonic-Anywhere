#ifndef SONIC_ANYWHERE_TEST_FRAMEWORK_H
#define SONIC_ANYWHERE_TEST_FRAMEWORK_H

// clang-format off
#define FAIL_MSG    "[TASSERT] \033[0;31m|X| |FAIL|\033[0m "
#define OK_MSG      "[TASSERT] \033[0;32m|V| | OK |\033[0m "
// clang-format on

void test_print__(const char* format, ...);

void test_assert_array_equals__(
  const unsigned char* a, unsigned long long a_size, const unsigned char* b, unsigned long long b_size, const char* fn,
  const char* file, int line
);

#define TEST_ASSERT_ARRAY_EQUALS(A, A_SIZE, B, B_SIZE) \
    test_assert_array_equals__(A, A_SIZE, B, B_SIZE, __func__, __FILE__, __LINE__);

#define TEST_ASSERT_EQUALS(A, B)                                                                                   \
    if ((A) == (B)) {                                                                                              \
        test_print__(OK_MSG "[%s] FILE %s ; LINE %d\n", __func__, __FILE__, __LINE__);                             \
    } else {                                                                                                       \
        test_print__(                                                                                              \
          FAIL_MSG "[%s] ACTUAL: %llu | EXPECTED: %llu || FILE %s | LINE %d\n", __func__, A, B, __FILE__, __LINE__ \
        );                                                                                                         \
    }

#define ARR_SIZE(X) sizeof(X) / sizeof(X[0])

#endif // SONIC_ANYWHERE_TEST_FRAMEWORK_H
