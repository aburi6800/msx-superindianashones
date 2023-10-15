// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef GAME_INIT_H
#define GAME_INIT_H

#include <stdint.h>


// define_graphic_chars.asm:CHR_PTN_TBLへの参照
extern uint8_t CHR_PTN_TBL[];

// define_graphic_chars.asm:CHR_COL_TBLへの参照
extern uint8_t CHR_COL_TBL[];

// define_sprite_chars.asm:SPR_PTN_TBL ラベルの参照
extern uint8_t SPR_PTN_TBL[];


/**
 * 初期設定処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_init();


#endif
