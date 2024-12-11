cmake_minimum_required(VERSION 3.28)

project(sonic_anywhere_tests C)
project(sonic_anywhere C)

set(CMAKE_C_STANDARD 99)

add_subdirectory("src_backend_desktop/lib/raylib5" EXCLUDE_FROM_ALL)

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
        src/plc.c
        src_backend_desktop/vdp.c
        src_backend_desktop/mdcolor.c
        src/gamevdp.h
        src/gamevdp.c
        include_backend/mdmem.h
        src_backend_desktop/mem.c
        include_backend/mdsystem.h
        src_backend_desktop/system.c
        config/config.h
        include_backend/macroutils.h
        src/gamevdp.vblankinterrupts.c
        src/bytearray.c
        src/gamemode/gamemode_title.c
        src/objects.c
        src/objects.h
        src/object_map.h
        src/objects/obj_sonic_team_8a.c
        src/mappings/sprite_piece.h
        src/mappings/sprite_mapping_type.h
        src/mappings/credits.c
        src/mappings/sprite_mapping.c
        src/sprite_buffer.h
        src/sprite_buffer.c
)

target_include_directories(sonic_anywhere PRIVATE ${raylib_public_headers})
target_include_directories(sonic_anywhere PRIVATE ".")


target_link_libraries(sonic_anywhere PRIVATE raylib)

add_executable(sonic_anywhere_tests
        tests/main.c
        tests/lib/test_framework.c
        src/compressors/kosinski.c
        src/compressors/nemesis.c
        src/compressors/enigma.c
        src_backend_desktop/debug.c
        tests/compresseddata.h)

target_include_directories(sonic_anywhere_tests PRIVATE "./")
