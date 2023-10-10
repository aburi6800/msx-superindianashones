// License:MIT License
// copyright-holders:Hitoshi Iwai

#include <stdint.h>
#include <msx.h>


// ゲーム状態ENUM
typedef enum{
    TITLE,
    ROUND_START,
    GAME_MAIN,
    ROUND_CLEAR,
    ALL_CLEAR,
    MISS,
    GAME_OVER
} state_t;


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
 * tick値加算
 * - tick1は毎フレーム加算する
 * - tick2は60フレームごとに加算する（＝秒カウント）
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void count_tick();


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
void change_state(state_t distState);


/**
 * スプライトアトリビュート更新
 * - スプライトアトリビュートを更新する
 *
 * args:
 * - uint8_t        chr_idx     対象のキャラクタデータインデックス
 * - uint8_t        attr_idx    対象のアトリビュートデータインデックス
 * - bool           isPlayer    プレイヤーの場合はtrue
 *
 * return:
 * - void
 */
void update_sprite_attr(uint8_t attr_idx, uint8_t chr_idx, bool isPlayer);


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
 * 画面作成処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void make_screen();


/**
 * ドアアニメーション処理
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void door_anim();


/**
 * タイトル画面
 *
 * args:
 * - none
 *
 * return:
 * - void
 */
void title();


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


