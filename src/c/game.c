// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "screen.h"
#include "character.h"
#include "control.h"
#include "tick.h"
#include "game_title.h"
#include "game.h"


#define VRAM_START          0x1800
#define VRAM_WIDTH          32
#define VRAM_HEIGHT         24
#define CHR_SPACE           0x20


// マクロ（インライン展開される）
#define VPOS(x, y)  (VRAM_START + VRAM_WIDTH * y + x)
#define POS(x, y)   (VRAM_WIDTH * y + x)


// 画面更新完了フラグ
bool isUpdated = false;

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
    vwrite_ptn_name_tbl();

    // スプライトアトリビュートテーブル更新
    vwrite_sprite_attr_tbl();

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
    game.state = distState;

    // ゲームサブ状態をリセット
    game.substate = 0;

    // 経過時間リセット
    reset_tick();
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
    // 画面更新済（ロジック処理可）なら処理する
    if (isUpdated) {
        switch (game.state) {
            case TITLE:
                game_title();
            default:
                break;
        }

        // 画面更新完了フラグを画面更新未完了に設定
        isUpdated = false;
    }
}
