// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include <stdint.h>
#include <msx.h>


// ゲーム状態ENUM
typedef enum {
    TITLE,
    ROUND_START,
    GAME_MAIN,
    ROUND_CLEAR,
    ALL_CLEAR,
    MISS,
    GAME_OVER
} state_t;


// ゲーム情報
typedef struct {
    // スコア
    unsigned char score[3];
    // ラウンド
    unsigned char round[1];
    // 残機
    unsigned char left[1];
    // ゲーム状態
    state_t game_state;
} game_t;


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
void screen_update();


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
void change_game_state(state_t distState);


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


/**
 * ゲームループ
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void game_loop();

#endif
