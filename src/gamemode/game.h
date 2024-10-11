#ifndef SONIC_ANYWHERE_GAME_H
#define SONIC_ANYWHERE_GAME_H

// clang-format off
typedef enum GameMode {
    GM_SEGA             = 0x00,
    GM_TITLE            = 0x04,
    GM_DEMO             = 0x08,
    GM_LEVEL            = 0x0C,
    GM_SPECIAL_STAGE    = 0x10,
    GM_CONTINUE         = 0x14,
    GM_END              = 0x18,
    GM_CREDIT           = 0x1C,
    GM_PRE_LEVEL        = 0x8C
} GameMode;
// clang-format on

GameMode game__get_game_mode();

void game__load_game_mode(GameMode gm);
void game__init();

void game_mode_sega();
void game_mode_title();
void game_mode_demo();
void game_mode_level();
void game_mode_special_stage();
void game_mode_continue();
void game_mode_end();
void game_mode_credit();
void game_mode_prelevel();

#endif // SONIC_ANYWHERE_TESTS_GAME_H
