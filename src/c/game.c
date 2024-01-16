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
#include "sound.h"
#include "game.h"
#include "game_title.h"
#include "game_roundstart.h"
#include "game_main.h"


// 画面更新カウンタ
uint8_t update_count = 0;

// 処理終了フラグ
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
    SPR_ATTR_TBL[attr_idx][3] = character.color1;
    if (character.type == PLAYER) {
        attr_idx++;
        SPR_ATTR_TBL[attr_idx][0] = character.y;
        SPR_ATTR_TBL[attr_idx][1] = character.x;
        SPR_ATTR_TBL[attr_idx][2] = ptn + 4;
        SPR_ATTR_TBL[attr_idx][3] = character.color2;
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
    // サウンドドライバ処理呼び出し
    sounddrv_exec();

    // 入力バッファ取得
    get_controls();

    char v[];
    v[0] = 48 + update_count;
    vwrite(v, 0x1800, 1);

    // 画面更新カウンタ判定
    if (--update_count > 0) {
        return;
    }

    // 画面更新カウンタ初期化
    update_count = FRAME_RATE;

    // 経過時間加算
    count_tick();


    // ロジック処理未終了なら抜ける
    if (isUpdated == false) {
        return;
    }

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

    // ロジック処理未完了に設定する
    isUpdated = false;

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

    // ロジック処理未完了に設定する
    isUpdated = false;

    // 画面更新カウンタ初期化
    update_count = FRAME_RATE;
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
    // ロジック処理可なら処理する
    if (update_count == 0) {
        return;
    }

    if (isUpdated == false) {
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
        isUpdated = true;
    }
}
