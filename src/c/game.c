// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>
#include <msx/gfx.h>
#include "define.h"
#include "character.h"
#include "control.h"
#include "tick.h"
#include "screen.h"
#include "game.h"
#include "game_title.h"
#include "game_roundstart.h"
#include "game_main.h"


// 画面更新完了フラグ
bool isUpdated = false;

// ゲーム状態
game_t game;


/**
 * スプライトアトリビュート更新
 * - スプライトアトリビュートを更新する
 *
 * args:
 * - character_t    character   対象のキャラクタデータ
 *
 * return:
 * - void
 */
void update_sprite_attr(character_t character)
{
    uint8_t ptn = (character.p * 4) + (character.r * 8);
    uint8_t attr_idx = character.attr_no;

    SPR_ATTR_TBL[attr_idx][0] = character.y;
    SPR_ATTR_TBL[attr_idx][1] = character.x;
    SPR_ATTR_TBL[attr_idx][2] = ptn;
    SPR_ATTR_TBL[attr_idx][3] = character.c[0];
    if (character.type == PLAYER) {
        attr_idx++;
        SPR_ATTR_TBL[attr_idx][0] = character.y;
        SPR_ATTR_TBL[attr_idx][1] = character.x;
        SPR_ATTR_TBL[attr_idx][2] = ptn + 4;
        SPR_ATTR_TBL[attr_idx][3] = character.c[1];
    }
}


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

    // 入力バッファ取得
    get_controls();

    // 経過時間加算
    count_tick();

    #ifndef __INTELLISENSE__
    __asm
    DI
    __endasm;
    #endif

    // パターンネームテーブル更新
    vwrite(PTN_NAME_TBL, VRAM_PTN_NAME_TBL, VRAM_PTN_NAME_SIZE);

    // スプライトアトリビュートテーブル更新
    vwrite(SPR_ATTR_TBL, VRAM_SPR_ATTR_TBL, VRAM_SPR_ATTR_SIZE);

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
 * - distState      game_state_t 変更後のゲーム状態
 *
 * return:
 * - void
*/
void change_game_state(game_state_t distState)
{
    // ゲーム状態を変更
    game.game_state = distState;

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
        switch (game.game_state) {
            case GAME_STATE_TITLE:
                game_title();
                break;
            case GAME_STATE_ROUNDSTART:
                game_roundstart();
                break;
            case GAME_STATE_MAIN:
                game_main();
                break;
            default:
                break;
        }

        // 画面更新完了フラグを画面更新未完了に設定
        isUpdated = false;
    }
}
