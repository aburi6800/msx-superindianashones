// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <msx.h>
#include "define.h"
#include "character.h"


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
    state_t state;
    // サブゲーム状態
    uint8_t substate;
} game_t;
extern game_t game;


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
void update_sprite_attr(character_t character);


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
