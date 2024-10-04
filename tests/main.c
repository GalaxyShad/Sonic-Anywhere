#include "src/compressors.h"

#include "tests/lib/test_framework.h"

#include "compresseddata.h"

void test____compressors__kosinski_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__kosinski_decompress(KOSINSKI_COMPRESSED, ARR_SIZE(KOSINSKI_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), KOSINSKI_DECOMPRESSED, ARR_SIZE(KOSINSKI_DECOMPRESSED));
}

void test____compressors__enigma_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__enigma_decompress(ENIGMA_COMPRESSED, ARR_SIZE(ENIGMA_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), ENIGMA_DECOMPRESSED, ARR_SIZE(ENIGMA_DECOMPRESSED));
}

void test____compressors__nemesis_decompress____should_decompress() {
    unsigned char buffer[4096] = {0};

    compressors__nemesis_decompress(NEMESIS_COMPRESSED, ARR_SIZE(NEMESIS_COMPRESSED), buffer, ARR_SIZE(buffer));

    TEST_ASSERT_ARRAY_EQUALS(buffer, ARR_SIZE(buffer), NEMESIS_DECOMPRESSED, ARR_SIZE(NEMESIS_DECOMPRESSED));
}

int main() {
    test____compressors__kosinski_decompress____should_decompress();
    test____compressors__enigma_decompress____should_decompress();
    test____compressors__nemesis_decompress____should_decompress();

    return 0;
}