// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "screen.h"
#include "sprite.h"
#include "control.h"
#include "tick.h"
#include "game_title.h"
#include "game_main.h"


// 定数（インライン展開される）
#define MSX_H_TIMI          0xfd9f
#define VRAM_START          0x1800
#define VRAM_WIDTH          32
#define VRAM_HEIGHT         24
#define CHR_SPACE           0x20


// マクロ（インライン展開される）
#define VPOS(x, y)  (VRAM_START + VRAM_WIDTH * y + x)
#define POS(x, y)   (VRAM_WIDTH * y + x)


// define_graphic_chars.asm:CHR_PTN_TBL ラベルの参照(パターンジェネレータテーブル)
extern uint8_t CHR_PTN_TBL[];

// define_graphic_chars.asm:CHR_COL_TBL ラベルの参照(カラーテーブル)
extern uint8_t CHR_COL_TBL[];


// ゲーム経過時間
extern tick_t tick;


// 画面更新完了フラグ
bool isUpdated = false;

// ゲーム状態
state_t state;

// ゲーム情報
game_t game;


/**
 * 画面更新
 * - H.TIMI割り込みから呼ばれる
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void screen_update()
{
    // 画面更新済（ロジック処理未終了）なら抜ける
    if (isUpdated) {
        return;
    }

    // 入力バッファ
    poling_controls();

    // 経過時間加算
    count_tick();

    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif

    // パターンネームテーブル更新
    transfer_ptn_name_tbl();

    // スプライトアトリビュートテーブル更新
    transfer_sprite_attr_tbl();

    #ifndef __INTELLISENSE__
    __asm
    EI
    __endasm;
    #endif

    // 画面更新済（ロジック処理可）に設定する
    isUpdated = true;
    return;
}


/**
 * ゲーム状態変更
 * - state_tで定義したゲーム状態を渡すことで状態を変更し、サブステータスをリセットする
 * - 状態変更後、経過時間をゼロにリセットする
 *
 * args:
 * - distState      state_t     変更後のゲーム状態
 *
 * return:
 * - void
*/
void change_game_state(state_t distState)
{
    // ゲーム状態を変更
    game.game_state = distState;

    // 経過時間リセット
    tick.tick1 = 0;
    tick.tick2 = 0;
}


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
    transfer_sprite_ptn_tbl(19);


    // 仮想画面クリア
    buff_clear();

    // ゲーム状態初期化
    game.score[0] = 0x12;
    game.score[1] = 0x34;
    game.score[2] = 0x56;
    game.round[0] = 0x78;
    game.left[0] = 0x90;
    change_game_state(TITLE);
}


/**
 * ゲームループ
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_loop()
{
    while(1) {
        // 画面更新済（ロジック処理可）なら処理する
        if (isUpdated) {
            switch (game.game_state) {
                case TITLE:
                    game_title();
                default:
                    break;
            }

            // 画面更新完了フラグを画面更新未完了に設定
            isUpdated = false;
        }

    }
}
