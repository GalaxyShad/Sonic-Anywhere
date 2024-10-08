cmake_minimum_required(VERSION 3.28)

project(sonic_anywhere_tests C)
project(sonic_anywhere C)

set(CMAKE_C_STANDARD 99)

add_subdirectory("src_backend_desktop/lib/SDL2" EXCLUDE_FROM_ALL)

add_executable(sonic_anywhere
        src/bytearray.h
        src/compressors/kosinski.c
        src/compressors/enigma.c
        src/compressors/nemesis.c
        src/resources/resourcestore.c
        src/gamemode/game.c
        src/gamemode/gamemode_sega.c
        src/audio.c

        src_backend_desktop/main.c
        src_backend_desktop/audio.c
        src_backend_desktop/debug.c
        src_backend_desktop/input.c
        src_backend_desktop/plc.c
        src_backend_desktop/vpu.c
        src_backend_desktop/mdcolor.c
        src/gamevdp.h
        src/gamevdp.c
)

target_include_directories(sonic_anywhere PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(sonic_anywhere PRIVATE ".")

if(TARGET SDL2::SDL2main)
    target_link_libraries(sonic_anywhere PRIVATE SDL2::SDL2main)
endif()

if (WIN32)
    add_custom_command(
            TARGET sonic_anywhere POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:sonic_anywhere>"
            VERBATIM
    )
endif()

target_link_libraries(sonic_anywhere PRIVATE SDL2::SDL2)

add_executable(sonic_anywhere_tests
        tests/main.c
        tests/lib/test_framework.c
        src/compressors/kosinski.c
        src/compressors/nemesis.c
        src/compressors/enigma.c
        src_backend_desktop/debug.c
        tests/compresseddata.h)

target_include_directories(sonic_anywhere_tests PRIVATE "./")
