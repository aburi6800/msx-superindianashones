// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "game.h"

// 定数（インライン展開される）
#define MSX_H_TIMI          0xfd9f


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
void game_init()
{
    // H.TIMIフック設定
    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif
    uint8_t *h_timi = (uint8_t *)MSX_H_TIMI;
    uint16_t hook_addr = (uint16_t)&screen_update;
    h_timi[0] = 0xc3; // JP
    h_timi[1] = (uint8_t)(hook_addr & 0xff);
    h_timi[2] = (uint8_t)((hook_addr & 0xff00) >> 8);
    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    // 画面初期化
    set_color(15, 1, 1);
    set_mangled_mode();
    msx_set_sprite_mode(sprite_large);

    // パターンジェネレータテーブル設定
    vwrite(CHR_PTN_TBL, 0x0000, 0x0800);
    vwrite(CHR_PTN_TBL, 0x0800, 0x0800);
    vwrite(CHR_PTN_TBL, 0x1000, 0x0800);

    // カラーテーブル設定
    vwrite(CHR_COL_TBL, 0x2000, 0x0800);
    vwrite(CHR_COL_TBL, 0x2800, 0x0800);
    vwrite(CHR_COL_TBL, 0x3000, 0x0800);

    // スプライトパターンテーブル設定
    vwrite(SPR_PTN_TBL, 0x3800, 32*19);

    // ゲーム状態初期化
    change_game_state(TITLE);
}
