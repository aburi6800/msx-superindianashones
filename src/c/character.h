// License:MIT License
// copyright-holders:Hitoshi Iwai

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>


// キャラクタ種類ENUM
typedef enum {
    NONE,
    PLAYER,
    SIGHT,
    KNIFE,
    BAT,
    SKELTON
} character_type_t;


/**
 * キャラクタロジックの関数ポインタ宣言
 */
//typedef void (*update_character)();


// キャラクタ属性
typedef struct {
    // キャラクタ種類
    character_type_t type;

    // キャラクタ状態
    uint8_t f;

    // スプライトアトリビュート番号
    uint8_t attr_no;

    // キャラクタ座標
    int x;
    int y;

    // キャラクタ移動先座標
    int target_x;
    int target_y;

    int dx;
    int dy;

    int sx;
    int sy;

    int err;

    // キャラクタ移動スピード
    float speed;

    // キャラクタパターン
    uint8_t p;

    // キャラクタアニメーションカウンタ
    uint8_t r;

    // キャラクタカラー
    uint8_t color1;
    uint8_t color2;

    // 処理モジュールのポインタ
//    update_character update;
} character_t;
extern character_t characters[8+1];


// スプライトアトリビュートテーブル
extern uint8_t SPR_ATTR_TBL[32+1][4];


/**
 * キャラクタ移動量設定処理
 * - 予め、x,y,target_x,target_yを設定したcharacter_t型の変数ポインタを与える
 *
 * args:
 * - character_t    対象のキャラクタデータのポインタ
 *
 * return:
 * - void
 */
void set_movevalue(character_t *character);


/**
 * キャラクタ移動処理
 *
 * args:
 * - character_t    対象のキャラクタデータのポインタ
 *
 * return:
 * - uint8_t        0=移動中、1=移動終了
 */
uint8_t move_character(character_t *character);

#endif
