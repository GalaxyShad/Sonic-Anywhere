cmake_minimum_required(VERSION 3.28)

project(sonic_anywhere_tests C)
project(sonic_anywhere C)

set(CMAKE_C_STANDARD 99)

add_subdirectory("src_backend_desktop/lib/SDL2" EXCLUDE_FROM_ALL)
#
#add_executable(sonic_anywhere
#        src/bytearray.h
#        include_backend/tinyint.h
#        src/compressors.h
#        include_backend/debug.h
#        src/compressors/kosinski.c
#        src/compressors/enigma.c
#        src/compressors/nemesis.c
#        src_backend_desktop/debug.c
#        src_s1disasm_assets/s1disasm_assets.h
#        src/resources/resourcestore.h
#        src/resources/resourcestore.c
#        src/resources/resourceid.h
#        src/gamemode/game.h
#        src/gamemode/game.c
#        src/gamemode/gamemode_sega.c
#        src/audio.c
#        include_backend/audio.h
#        include_backend/mdcolor.h
#        include_backend/vpu.h
#        include_backend/plc.h
#        include_backend/interrupt.h
#        include_backend/input.h
#        src_backend_desktop/main.c)

add_executable(sonic_anywhere src_backend_desktop/main.c)

target_include_directories(sonic_anywhere PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(sonic_anywhere PRIVATE ".")

if(TARGET SDL2::SDL2main)
    target_link_libraries(sonic_anywhere PRIVATE SDL2::SDL2main)
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
