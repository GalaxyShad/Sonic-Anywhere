#include "src/compressors.h"

#include "tests/lib/test_framework.h"

#include "compresseddata.h"

void test____compressors__kosinski_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__kosinski_decompress(KOSINSKI_COMPRESSED, ARR_SIZE(KOSINSKI_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), KOSINSKI_DECOMPRESSED, ARR_SIZE(KOSINSKI_DECOMPRESSED))
}


void test____compressors__enigma_decompress____should_decompress_data_from_sega_retro() {
    // https://www.segaretro.org/Enigma_compression
    const u8 input[] = {0x07, 0x0C, 0x00, 0x00, 0x00, 0x10, 0x05, 0x3D, 0x11, 0x8F, 0xE0};
    const u8 output[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x40, 0x18, 0x40,
                         0x17, 0x40, 0x16, 0x40, 0x15, 0x40, 0x14, 0x40, 0x13, 0x40, 0x12, 0x40, 0x11, 0x40, 0x10};

    u8 buffer[ARR_SIZE(output)] = {0};

    compressors__enigma_decompress(input, ARR_SIZE(input), buffer, ARR_SIZE(buffer), 0);

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), output, ARR_SIZE(output))
}

void test____compressors__enigma_decompress____should_decompress() {
    unsigned char buffer[1568] = {0};

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

void test____utils__lshift_bits____10010110b10110101_shifted_by_0_should_be_10010110() {
    const u8 buff[] = {0b10010110, 0b10110101};
    TEST_ASSERT_EQUALS(utils__lshift_bits(buff, 2, 0), 0b10010110)
}

void test____utils__lshift_bits____10010110b10110101_shifted_by_2_should_be_01011010() {
    const u8 buff[] = {0b10010110, 0b10110101};
    TEST_ASSERT_EQUALS(utils__lshift_bits(buff, 2, 2), 0b01011010)
}

void test____utils__lshift_bits____10010110b10110101_shifted_by_14_should_be_01000000() {
    const u8 buff[] = {0b10010110, 0b10110101};
    TEST_ASSERT_EQUALS(utils__lshift_bits(buff, 2, 14), 0b01000000)
}

void test____utils__lshift_bits____a_shifted_by_14_should_be_01000000() {
    const u8 buff[] = {0x05, 0x3D, 0x11, 0x8F, 0xE0 };
    TEST_ASSERT_EQUALS(utils__lshift_bits(buff, 5, 6), 0b01001111)
}

void test____utils__lshift_bits____ff() {
    const u8 buff[] = {0xFF, 0xFF };
    TEST_ASSERT_EQUALS(utils__lshift_bits(buff, 2, 6), 0b11111111)
}

int main() {
    //    test____tinyint____u8_should_be_1_byte_long();
    //    test____tinyint____u16_should_be_2_byte_long();
    //    test____tinyint____u32_should_be_4_byte_long();
    //    test____tinyint____u64_should_be_8_byte_long();

    //    test____compressors__kosinski_decompress____should_decompress();
        test____compressors__enigma_decompress____should_decompress();
    test____compressors__enigma_decompress____should_decompress_data_from_sega_retro();
//        test____compressors__nemesis_decompress____should_decompress();

//        test____utils__lshift_bits____10010110b10110101_shifted_by_0_should_be_10010110();
//        test____utils__lshift_bits____10010110b10110101_shifted_by_2_should_be_01011010();
//        test____utils__lshift_bits____10010110b10110101_shifted_by_14_should_be_01000000();
//        test____utils__lshift_bits____a_shifted_by_14_should_be_01000000();
//        test____utils__lshift_bits____ff();

    return 0;
}