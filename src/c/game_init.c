// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "define.h"
#include "game.h"
#include "sound.h"
#include "game_init.h"


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
    // 画面初期化
    set_color(15, 1, 1);
    set_mangled_mode();
    msx_set_sprite_mode(sprite_large);

    // パターンジェネレータテーブル設定
    vwrite(CHR_PTN_TBL, VRAM_PTN_GENR_TBL1, VRAM_PTN_GENR_SIZE);
    vwrite(CHR_PTN_TBL, VRAM_PTN_GENR_TBL2, VRAM_PTN_GENR_SIZE);
    vwrite(CHR_PTN_TBL, VRAM_PTN_GENR_TBL3, VRAM_PTN_GENR_SIZE);

    // カラーテーブル設定
    vwrite(CHR_COL_TBL, VRAM_COLOR_TBL1, VRAM_COLOR_SIZE);
    vwrite(CHR_COL_TBL, VRAM_COLOR_TBL2, VRAM_COLOR_SIZE);
    vwrite(CHR_COL_TBL, VRAM_COLOR_TBL3, VRAM_COLOR_SIZE);

    // スプライトパターンテーブル設定
    vwrite(SPR_PTN_TBL, VRAM_SPR_PTN_TBL, 32*19);

    // サウンドドライバ初期化
    sounddrv_init();

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

    // ゲーム状態初期化
    change_game_state(GAME_STATE_TITLE);
}
