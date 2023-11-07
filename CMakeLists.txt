cmake_minimum_required(VERSION 3.0)

# target name "shones.rom"
project(shones.rom C ASM)

enable_language(ASM)

# for debug
#set(CMAKE_VERBOSE_MAKEFILE 1)

include(./cmake/macros.cmake)

add_source_files(
    ./src/asm/define_graphic_chars.asm
    ./src/asm/define_sprite_chars.asm
    ./src/c/character.c
    ./src/c/character_player.c
    ./src/c/character_knife.c
    ./src/c/common.c
    ./src/c/control.c
    ./src/c/game_init.c
    ./src/c/game_main.c
    ./src/c/game_roundstart.c
    ./src/c/game_title.c
    ./src/c/game.c
    ./src/c/screen.c
    ./src/c/tick.c
    ./src/c/main.c
)

add_compile_flags(C
    +msx
    -I$ENV{Z88DK_HOME}/include
    -Isrc/c
    -vn
    -llib3d
    -lm
    --math16
    -lndos
    -lmsxbios
    --list
    -m
    # -debug
    # https://github.com/z88dk/z88dk/wiki/Classic-allocation#automatic-heap-configuration
    -DAMALLOC
)

add_compile_flags(ASM
    +msx
    --list
)

add_compile_flags(LD
    -create-app
    -subtype=rom
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../dist)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
