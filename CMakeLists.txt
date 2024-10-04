cmake_minimum_required(VERSION 3.28)

project(sonic_anywhere C)
project(sonic_anywhere_tests C)

set(CMAKE_C_STANDARD 99)


add_executable(sonic_anywhere src/main.c
        src/bytearray.h
        src_backend/include/tinyint.h
        src/compressors.h
        src_backend/include/debug.h
        src/compressors/kosinski.c
        src/compressors/enigma.c
        src/compressors/nemesis.c
        src_backend/src_desktop/debug.c
        src_s1disasm_assets/s1disasm_assets.h
        src/resources/resourceid.h
        src/resources/resourcestore.h
        src/resources/resourcestore.c)

target_include_directories(sonic_anywhere PRIVATE "./")


add_executable(sonic_anywhere_tests
        tests/main.c
        tests/lib/test_framework.c
        src/compressors/kosinski.c
        src/compressors/nemesis.c
        src/compressors/enigma.c
        src_backend/src_desktop/debug.c
        tests/compresseddata.h)

target_include_directories(sonic_anywhere_tests PRIVATE "./")
