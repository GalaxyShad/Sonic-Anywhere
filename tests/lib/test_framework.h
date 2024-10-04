#ifndef SONIC_ANYWHERE_TEST_FRAMEWORK_H
#define SONIC_ANYWHERE_TEST_FRAMEWORK_H

void test_assert_array_equals__(
  const unsigned char* a, unsigned long long a_size, const unsigned char* b, unsigned long long b_size, const char* fn,
  const char* file, int line
);

#define TEST_ASSERT_ARRAY_EQUALS(A, A_SIZE, B, B_SIZE) \
    test_assert_array_equals__(A, A_SIZE, B, B_SIZE, __func__, __FILE__, __LINE__)

#define ARR_SIZE(X) sizeof(X) / sizeof(X[0])

#endif // SONIC_ANYWHERE_TEST_FRAMEWORK_H
