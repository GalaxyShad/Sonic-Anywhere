#include "src/compressors.h"

#include "tests/lib/test_framework.h"

#include "compresseddata.h"

void test____compressors__kosinski_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__kosinski_decompress(KOSINSKI_COMPRESSED, ARR_SIZE(KOSINSKI_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), KOSINSKI_DECOMPRESSED, ARR_SIZE(KOSINSKI_DECOMPRESSED))
}

void test____compressors__enigma_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__enigma_decompress(ENIGMA_COMPRESSED, ARR_SIZE(ENIGMA_COMPRESSED), buffer, ARR_SIZE(buffer), 0);

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), ENIGMA_DECOMPRESSED, ARR_SIZE(ENIGMA_DECOMPRESSED))
}

void test____compressors__nemesis_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__nemesis_decompress(NEMESIS_COMPRESSED, ARR_SIZE(NEMESIS_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), NEMESIS_DECOMPRESSED, ARR_SIZE(NEMESIS_DECOMPRESSED))
}

void test____tinyint____u8_should_be_1_byte_long() {
    TEST_ASSERT_EQUALS(sizeof(u8), 1)
}

void test____tinyint____u16_should_be_2_byte_long() {
    TEST_ASSERT_EQUALS(sizeof(u16), 2)
}

void test____tinyint____u32_should_be_4_byte_long() {
    TEST_ASSERT_EQUALS(sizeof(u32), 4)
}

void test____tinyint____u64_should_be_8_byte_long() {
    TEST_ASSERT_EQUALS(sizeof(u64), 8)
}

int main() {
    test____tinyint____u8_should_be_1_byte_long();
    test____tinyint____u16_should_be_2_byte_long();
    test____tinyint____u32_should_be_4_byte_long();
    test____tinyint____u64_should_be_8_byte_long();

    test____compressors__kosinski_decompress____should_decompress();
    test____compressors__enigma_decompress____should_decompress();
    test____compressors__nemesis_decompress____should_decompress();

    return 0;
}